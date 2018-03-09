#!/usr/bin/env perl

# Copyright (C) 2007,2008,2009,2010,2011,2012,2013 Ole Tange and Free Software
# Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>
# or write to the Free Software Foundation, Inc., 51 Franklin St,
# Fifth Floor, Boston, MA 02110-1301 USA

# open3 used in Job::start
use IPC::Open3;
# &WNOHANG used in reaper
use POSIX qw(:sys_wait_h setsid ceil :errno_h);
# gensym used in Job::start
use Symbol qw(gensym);
# tempfile used in Job::start
use File::Temp qw(tempfile tempdir);
# mkpath used in openresultsfile
use File::Path;
# GetOptions used in get_options_from_array
use Getopt::Long;
# Used to ensure code quality
use strict;
use File::Basename;

if(not $ENV{SHELL}) {
    # $ENV{SHELL} is sometimes not set on Mac OS X and Windows
    ::warning("\$SHELL not set. Using /bin/sh.\n");
    $ENV{SHELL} = "/bin/sh";
}
if(not $ENV{HOME}) {
    # $ENV{HOME} is sometimes not set if called from PHP
    ::warning("\$HOME not set. Using /tmp\n");
    $ENV{HOME} = "/tmp";
}

save_stdin_stdout_stderr();
save_original_signal_handler();
parse_options();
::debug("Open file descriptors: ".join(" ",keys %Global::fd)."\n");
my $number_of_args;
if($Global::max_number_of_args) {
    $number_of_args=$Global::max_number_of_args;
} elsif ($opt::X or $opt::m or $opt::xargs) {
    $number_of_args = undef;
} else {
    $number_of_args = 1;
}

my $command = "";
if(@ARGV) {
    if($Global::quoting) {
	$command = shell_quote_empty(@ARGV);
    } else {
	$command = join(" ", @ARGV);
    }
}

my @fhlist;
@fhlist = map { open_or_exit($_) } @opt::a;
if(not @fhlist and not $opt::pipe) {
    @fhlist = (*STDIN);
}
if($opt::skip_first_line) {
    # Skip the first line for the first file handle
    my $fh = $fhlist[0];
    <$fh>;
}
if($opt::header and not $opt::pipe) {
    my $fh = $fhlist[0];
    # split with colsep or \t
    # $header force $colsep = \t if undef?
    my $delimiter = $opt::colsep;
    $delimiter ||= "\$";
    my $id = 1;
    for my $fh (@fhlist) {
	my $line = <$fh>;
	chomp($line);
	::debug("Delimiter: '$delimiter'");
	for my $s (split /$delimiter/o, $line) {
	    ::debug("Colname: '$s'");
	    $command =~ s:\{$s(|/|//|\.|/\.)\}:\{$id$1\}:g;
	    $Global::input_source_header{$id} = $s;
	    $id++;
	}
    }
} else {
    my $id = 1;
    for my $fh (@fhlist) {
	$Global::input_source_header{$id} = $id;
	$id++;
    }
}


if($opt::filter_hosts and (@opt::sshlogin or @opt::sshloginfile)) {
    # Parallel check all hosts are up. Remove hosts that are down
    filter_hosts();
}



if($opt::nonall or $opt::onall) {
    onall();
    wait_and_exit(min(undef_as_zero($Global::exitstatus),254));
}

# TODO --transfer foo/./bar --cleanup
# multiple --transfer and --basefile with different /./

$Global::JobQueue = JobQueue->new(
    $command,\@fhlist,$Global::ContextReplace,$number_of_args,\@Global::ret_files);
if($opt::pipe and @opt::a) {
    # ... | parallel --pipe cmd ::: arg1 arg2
    # The command to run is:
    # tee >((cmd arg1) >/tmp/tmp1 2>/tmp/err1) >((cmd arg2) >/tmp/tmp2 2>/tmp/err2) >/dev/null
    my @jobs;
    # Get all jobs
    my $sshlogin = SSHLogin->new(":");
    while(not $Global::JobQueue->empty()) {
	my $job = $Global::JobQueue->get();
	$job->openoutputfiles(); # Do not delete
	$job->set_virgin(0);
	$job->set_sshlogin($sshlogin);
	push @jobs, $job;
    }
    @Global::tee_jobs = @jobs;
    $command = "tee".join("",map {" >((".$_->replaced().")".
				       " >".$_->fd_file_name(1).
				       " 2>".$_->fd_file_name(2).")" } @jobs)
	." >/dev/null";
    $Global::JobQueue = JobQueue->new(
	$command,\@fhlist,$Global::ContextReplace,$number_of_args,\@Global::ret_files);
    #print ::my_dump($jobs[0]);
    #print ::my_dump($Global::JobQueue);
    $Global::JobQueue->{'commandlinequeue'}->{'arg_queue'}->unget([Arg->new("")]);
    #my $cmd = $Global::JobQueue->get();
    #print ::my_dump($cmd);
}

if($opt::eta or $opt::bar) {
    # Count the number of jobs before starting any
    $Global::JobQueue->total_jobs();
}
for my $sshlogin (values %Global::host) {
    $sshlogin->max_jobs_running();
}

init_run_jobs();
my $sem;
if($Global::semaphore) {
    $sem = acquire_semaphore();
}
$SIG{TERM} = \&start_no_new_jobs;
start_more_jobs();
if($opt::pipe) {
    spreadstdin();
}
::debug("Start draining\n");
drain_job_queue();
::debug("Done draining\n");
reaper();
if($opt::pipe and @opt::a) {
    for my $job (@Global::tee_jobs) {
	unlink $job->fd_file_name(2);
	$job->set_fd_file_name(2,"");
	$job->print();
	unlink $job->fd_file_name(1);
    }
}
cleanup();
if($Global::semaphore) {
    $sem->release();
}
if($opt::halt_on_error) {
    wait_and_exit($Global::halt_on_error_exitstatus);
} else {
    wait_and_exit(min(undef_as_zero($Global::exitstatus),254));
}

sub __PIPE_MODE__ {}

sub spreadstdin {
    # read a record
    # Spawn a job and print the record to it.
    my $buf = "";
    my $header = "";
    if($opt::header) {
	if($opt::header eq ":") { $opt::header = "(.*\n)"; }
	# Number = number of lines
	$opt::header =~ s/^(\d+)$/"(.*\n)"x$1/e;
	while(read(STDIN,substr($buf,length $buf,0),$opt::blocksize)) {
	    if($buf=~s/^($opt::header)//) {
		$header = $1; 
		last;
	    }
	}
    }
    my ($recstart,$recend);
    if(defined($opt::recstart) and defined($opt::recend)) {
	# If both --recstart and --recend is given then both must match
	$recstart = $opt::recstart;
	$recend = $opt::recend;
    } elsif(defined($opt::recstart)) {
	# If --recstart is given it must match start of record
	$recstart = $opt::recstart;
	$recend = "";
    } elsif(defined($opt::recend)) {
	# If --recend is given then it must match end of record
	$recstart = "";
	$recend = $opt::recend;
    }

    if($opt::regexp) {
	# If $recstart/$recend contains '|' this should only apply to the regexp
	$recstart = "(?:".$recstart.")";
	$recend = "(?:".$recend.")";
    } else {
	# $recstart/$recend = printf strings (\n)
	$recstart =~ s/\\([rnt\'\"\\])/"qq|\\$1|"/gee;
	$recend =~ s/\\([rnt\'\"\\])/"qq|\\$1|"/gee;
    }
    my $recendrecstart = $recend.$recstart;
    my $chunk_number = 1;
    my $one_time_through;
    my $blocksize = $opt::blocksize;
    my $in = *STDIN;
    while(1) {
      my $anything_written = 0;
      if(not read($in,substr($buf,length $buf,0),$blocksize)) {
	  # End-of-file
	  $chunk_number != 1 and last;
	  # Force the while-loop once if everything was read by header reading
	  $one_time_through++ and last;
      }
      if($opt::r) {
	  # Remove empty lines
	  $buf=~s/^\s*\n//gm;
	  if(length $buf == 0) {
	      next;
	  }
      }
      if($Global::max_lines and not $Global::max_number_of_args) {
	  # Read n-line records 
	  my $n_lines = $buf=~tr/\n/\n/;
	  my $last_newline_pos = rindex($buf,"\n");
	  while($n_lines % $Global::max_lines) {
	      $n_lines--;
	      $last_newline_pos = rindex($buf,"\n",$last_newline_pos-1);
	  }
	  # Chop at $last_newline_pos as that is where n-line record ends
	  $anything_written +=
	      write_record_to_pipe($chunk_number++,\$header,\$buf,
				   $recstart,$recend,$last_newline_pos+1);
	  substr($buf,0,$last_newline_pos+1) = "";
      } elsif($opt::regexp) {
	  if($Global::max_number_of_args) {
	      # -N => (start..*?end){n}
	      # -L -N => (start..*?end){n*l}
	      my $read_n_lines = $Global::max_number_of_args * ($Global::max_lines || 1);
	      while($buf =~ s/((?:$recstart.*?$recend){$read_n_lines})($recstart.*)$/$2/os) {
		  # Copy to modifiable variable
		  my $b = $1;
		  $anything_written +=
		      write_record_to_pipe($chunk_number++,\$header,\$b,
					   $recstart,$recend,length $1);
	      }
	  } else {
	      # Find the last recend-recstart in $buf
	      if($buf =~ s/(.*$recend)($recstart.*?)$/$2/os) {
		  # Copy to modifiable variable
		  my $b = $1;
		  $anything_written +=
		      write_record_to_pipe($chunk_number++,\$header,\$b,
					   $recstart,$recend,length $1);
	      }
	  }
      } else {
	  if($Global::max_number_of_args) {
	      # -N => (start..*?end){n}
	      my $i = 0;
	      my $read_n_lines = $Global::max_number_of_args * ($Global::max_lines || 1);
	      while(($i = nindex(\$buf,$recendrecstart,$read_n_lines)) != -1) {
		  $i += length $recend; # find the actual splitting location
		  $anything_written +=
		      write_record_to_pipe($chunk_number++,\$header,\$buf,
					   $recstart,$recend,$i);
		  substr($buf,0,$i) = "";
	      }
	  } else {
	      # Find the last recend-recstart in $buf
	      my $i = rindex($buf,$recendrecstart);
	      if($i != -1) {
		  $i += length $recend; # find the actual splitting location
		  $anything_written +=
		      write_record_to_pipe($chunk_number++,\$header,\$buf,
					   $recstart,$recend,$i);
		  substr($buf,0,$i) = "";
	      }
	  }
      }
      if(not $anything_written and not eof($in)) {
	  # Nothing was written - maybe the block size < record size?
	  # Increase blocksize exponentially
	  $blocksize = ceil($blocksize * 1.3 + 1);
	  ::warning("A full record was not matched in a block. Increasing to --blocksize ".$blocksize."\n");
      }
    }
    # If there is anything left in the buffer write it
    substr($buf,0,0) = "";
    write_record_to_pipe($chunk_number++,\$header,\$buf,$recstart,$recend,length $buf);

    ::debug("Done reading input\n");
    $Global::start_no_new_jobs ||= 1;
    if($opt::roundrobin) {
	for my $job (values %Global::running) {
	    my $fh = $job->fd(0);
	    close $fh;
	}
	my %incomplete_jobs = %Global::running;
	my $sleep = 1;
	while(keys %incomplete_jobs) {
	    my $something_written = 0;
	    for my $pid (keys %incomplete_jobs) {
		my $job = $incomplete_jobs{$pid};
		if($job->stdin_buffer_length()) {
		    $something_written += $job->non_block_write();
		} else {
		    delete $incomplete_jobs{$pid}
		}
	    }
	    if($something_written) {
		$sleep = $sleep/2+0.001;
	    }
	    $sleep = ::reap_usleep($sleep);
	}
    }
}

sub nindex {
    # See if string is in buffer N times
    # Returns:
    #   the position where the Nth copy is found
    my $buf_ref = shift;
    my $str = shift;
    my $n = shift;
    my $i = 0;
    for(1..$n) {
	$i = index($$buf_ref,$str,$i+1);
	if($i == -1) { last }
    }
    return $i;
}

sub round_robin_write {
    my ($header_ref,$block_ref,$recstart,$recend,$endpos) = @_;
    my $something_written = 0;
    my $block_passed = 0;
    while(not $block_passed) {
	while(my ($pid,$job) = each %Global::running) {
	    if($job->stdin_buffer_length() > 0) {
		$something_written += $job->non_block_write();
	    } else {
		$job->set_stdin_buffer($header_ref,$block_ref,$endpos,$recstart,$recend);
		$block_passed = 1;
		$job->set_virgin(0);
		$something_written += $job->non_block_write();
		last;
	    }
	}
    }

    # http://docstore.mik.ua/orelly/perl/cookbook/ch07_15.htm
    start_more_jobs();
    return $something_written;
}


sub write_record_to_pipe {
    # Fork then
    # Write record from pos 0 .. $endpos to pipe
    my ($chunk_number,$header_ref,$record_ref,$recstart,$recend,$endpos) = @_;
    if($endpos == 0) { return 0; }
    if(vec($Global::job_already_run,$chunk_number,1)) { return 1; }
    if($opt::roundrobin) {
	return round_robin_write($header_ref,$record_ref,$recstart,$recend,$endpos);
    }
    # If no virgin found, backoff
    my $sleep = 0.0001; # 0.01 ms - better performance on highend
    while(not @Global::virgin_jobs) {
	::debug("No virgin jobs");
	$sleep = ::reap_usleep($sleep);
	# Jobs may not be started because of loadavg
	# or too little time between each ssh login.
	start_more_jobs();
    }
    my $job = shift @Global::virgin_jobs;
    # Job is no longer virgin
    $job->set_virgin(0);
    if(fork()) {
	# Skip
    } else {
	# Chop of at $endpos as we do not know how many rec_sep will
	# be removed.
	substr($$record_ref,$endpos,length $$record_ref) = "";
	# Remove rec_sep
	if($opt::remove_rec_sep) {
	    Job::remove_rec_sep($record_ref,$recstart,$recend);
	}
	$job->write($header_ref);
	$job->write($record_ref);
	my $stdin_fh = $job->fd(0);
	close $stdin_fh;
	exit(0);
    }
    my $stdin_fh = $job->fd(0);
    close $stdin_fh;
    return 1;
}

sub __SEM_MODE__ {}

sub acquire_semaphore {
    # Acquires semaphore. If needed: spawns to the background
    # Returns:
    #   The semaphore to be released when jobs is complete
    $Global::host{':'} = SSHLogin->new(":");
    my $sem = Semaphore->new($Semaphore::name,$Global::host{':'}->max_jobs_running());
    $sem->acquire();
    debug("run");
    if($Semaphore::fg) {
	# skip
    } else {
	# If run in the background, the PID will change
	# therefore release and re-acquire the semaphore
	$sem->release();
	if(fork()) {
	    exit(0);
	} else {
	    # child
	    # Get a semaphore for this pid
	    ::die_bug("Can't start a new session: $!") if setsid() == -1;
	    $sem = Semaphore->new($Semaphore::name,$Global::host{':'}->max_jobs_running());
	    $sem->acquire();
	}
    }
    return $sem;
}

sub __PARSE_OPTIONS__ {}

sub options_hash {
    # Returns a hash of the GetOptions config
    return
	("debug|D" => \$opt::D,
	 "xargs" => \$opt::xargs,
	 "m" => \$opt::m,
	 "X" => \$opt::X,
	 "v" => \@opt::v,
	 "joblog=s" => \$opt::joblog,
	 "results|result|res=s" => \$opt::results,
	 "resume" => \$opt::resume,
	 "resume-failed|resumefailed" => \$opt::resume_failed,
	 "silent" => \$opt::silent,
	 #"silent-error|silenterror" => \$opt::silent_error,
	 "keep-order|keeporder|k" => \$opt::keeporder,
	 "group" => \$opt::group,
	 "g" => \$opt::retired,
	 "ungroup|u" => \$opt::u,
	 "linebuffer|linebuffered|line-buffer|line-buffered" => \$opt::linebuffer,
	 "null|0" => \$opt::0,
	 "quote|q" => \$opt::q,
	 "I=s" => \$opt::I,
	 "extensionreplace|er=s" => \$opt::U,
	 "U=s" => \$opt::retired,
	 "basenamereplace|bnr=s" => \$opt::basenamereplace,
	 "dirnamereplace|dnr=s" => \$opt::dirnamereplace,
	 "basenameextensionreplace|bner=s" => \$opt::basenameextensionreplace,
	 "seqreplace=s" => \$opt::seqreplace,
	 "jobs|j=s" => \$opt::P,
	 "delay=f" => \$opt::delay,
	 "sshdelay=f" => \$opt::sshdelay,
	 "load=s" => \$opt::load,
	 "noswap" => \$opt::noswap,
	 "max-line-length-allowed" => \$opt::max_line_length_allowed,
	 "number-of-cpus" => \$opt::number_of_cpus,
	 "number-of-cores" => \$opt::number_of_cores,
	 "use-cpus-instead-of-cores" => \$opt::use_cpus_instead_of_cores,
	 "shellquote|shell_quote|shell-quote" => \$opt::shellquote,
	 "nice=i" => \$opt::nice,
	 "timeout=s" => \$opt::timeout,
	 "tag" => \$opt::tag,
	 "tagstring|tag-string=s" => \$opt::tagstring,
	 "onall" => \$opt::onall,
	 "nonall" => \$opt::nonall,
	 "filter-hosts|filterhosts|filter-host" => \$opt::filter_hosts,
	 "sshlogin|S=s" => \@opt::sshlogin,
	 "sshloginfile|slf=s" => \@opt::sshloginfile,
	 "controlmaster|M" => \$opt::controlmaster,
	 "return=s" => \@opt::return,
	 "trc=s" => \@opt::trc,
	 "transfer" => \$opt::transfer,
	 "cleanup" => \$opt::cleanup,
	 "basefile|bf=s" => \@opt::basefile,
	 "B=s" => \$opt::retired,
	 "ctrlc|ctrl-c" => \$opt::ctrlc,
	 "noctrlc|no-ctrlc|no-ctrl-c" => \$opt::noctrlc,
	 "workdir|work-dir|wd=s" => \$opt::workdir,
	 "W=s" => \$opt::retired,
	 "tmpdir=s" => \$opt::tmpdir,
	 "tempdir=s" => \$opt::tmpdir,
	 "use-compress-program|compress-program=s" => \$opt::compress_program,
	 "compress" => \$opt::compress,
	 "tty" => \$opt::tty,
	 "T" => \$opt::retired,
	 "halt-on-error|halt=i" => \$opt::halt_on_error,
	 "H=i" => \$opt::retired,
	 "retries=i" => \$opt::retries,
	 "dry-run|dryrun" => \$opt::dryrun,
	 "progress" => \$opt::progress,
	 "eta" => \$opt::eta,
	 "bar" => \$opt::bar,
	 "arg-sep|argsep=s" => \$opt::arg_sep,
	 "arg-file-sep|argfilesep=s" => \$opt::arg_file_sep,
	 "trim=s" => \$opt::trim,
	 "env=s" => \@opt::env,
	 "recordenv|record-env" => \$opt::record_env,
	 "plain" => \$opt::plain,
	 "profile|J=s" => \@opt::profile,
	 "pipe|spreadstdin" => \$opt::pipe,
	 "robin|round-robin|roundrobin" => \$opt::roundrobin,
	 "recstart=s" => \$opt::recstart,
	 "recend=s" => \$opt::recend,
	 "regexp|regex" => \$opt::regexp,
	 "remove-rec-sep|removerecsep|rrs" => \$opt::remove_rec_sep,
	 "files|output-as-files|outputasfiles" => \$opt::files,
	 "block|block-size|blocksize=s" => \$opt::blocksize,
	 "tollef" => \$opt::tollef,
	 "gnu" => \$opt::gnu,
	 "xapply" => \$opt::xapply,
	 "bibtex" => \$opt::bibtex,
	 "nn|nonotice|no-notice" => \$opt::no_notice,
	 # xargs-compatibility - implemented, man, testsuite
	 "max-procs|P=s" => \$opt::P,
	 "delimiter|d=s" => \$opt::d,
	 "max-chars|s=i" => \$opt::max_chars,
	 "arg-file|a=s" => \@opt::a,
	 "no-run-if-empty|r" => \$opt::r,
	 "replace|i:s" => \$opt::i,
	 "E=s" => \$opt::E,
	 "eof|e:s" => \$opt::E,
	 "max-args|n=i" => \$opt::max_args,
	 "max-replace-args|N=i" => \$opt::max_replace_args,
	 "colsep|col-sep|C=s" => \$opt::colsep,
	 "help|h" => \$opt::help,
	 "L=f" => \$opt::L,
	 "max-lines|l:f" => \$opt::max_lines,
	 "interactive|p" => \$opt::p,
	 "verbose|t" => \$opt::verbose,
	 "version|V" => \$opt::version,
	 "minversion|min-version=i" => \$opt::minversion,
	 "show-limits|showlimits" => \$opt::show_limits,
	 "exit|x" => \$opt::x,
	 # Semaphore
	 "semaphore" => \$opt::semaphore,
	 "semaphoretimeout=i" => \$opt::semaphoretimeout,
	 "semaphorename|id=s" => \$opt::semaphorename,
	 "fg" => \$opt::fg,
	 "bg" => \$opt::bg,
	 "wait" => \$opt::wait,
	 # Shebang #!/usr/bin/parallel --shebang
	 "shebang|hashbang" => \$opt::shebang,
	 "internal-pipe-means-argfiles" => \$opt::internal_pipe_means_argfiles,
	 "Y" => \$opt::retired,
         "skip-first-line" => \$opt::skip_first_line,
	 "header=s" => \$opt::header,
	);
}

sub get_options_from_array {
    # Run GetOptions on @array
    # Returns:
    #   true if parsing worked
    #   false if parsing failed
    #   @array is changed
    my $array_ref = shift;
    my @keep_only = @_;
    # A bit of shuffling of @ARGV needed as GetOptionsFromArray is not
    # supported everywhere
    my @save_argv;
    my $this_is_ARGV = (\@::ARGV == $array_ref);
    if(not $this_is_ARGV) {
	@save_argv = @::ARGV;
	@::ARGV = @{$array_ref};
    }
    # If @keep_only set: Ignore all values except @keep_only
    my %options = options_hash();
    if(@keep_only) {
	my (%keep,@dummy);
	@keep{@keep_only} = @keep_only;
	for my $k (grep { not $keep{$_} } keys %options) {
	    # Store the value of the option in @dummy
	    $options{$k} = \@dummy;
	}
    }
    my $retval = GetOptions(%options);
    if(not $this_is_ARGV) {
	@{$array_ref} = @::ARGV;
	@::ARGV = @save_argv;
    }
    return $retval;
}

sub parse_options {
    # Returns: N/A
    # Defaults:
    $Global::version = 20131222;
    $Global::progname = 'parallel';
    $Global::infinity = 2**31;
    $Global::debug = 0;
    $Global::verbose = 0;
    $Global::grouped = 1;
    $Global::keeporder = 0;
    $Global::quoting = 0;
    $Global::replace{'{}'} = '{}';
    $Global::replace{'{.}'} = '{.}';
    $Global::replace{'{/}'} = '{/}';
    $Global::replace{'{//}'} = '{//}';
    $Global::replace{'{/.}'} = '{/.}';
    $Global::replace{'{#}'} = '{#}';
    $/="\n";
    $Global::ignore_empty = 0;
    $Global::interactive = 0;
    $Global::stderr_verbose = 0;
    $Global::default_simultaneous_sshlogins = 9;
    $Global::exitstatus = 0;
    $Global::halt_on_error_exitstatus = 0;
    $Global::arg_sep = ":::";
    $Global::arg_file_sep = "::::";
    $Global::trim = 'n';
    $Global::max_jobs_running = 0;
    $Global::job_already_run = '';

    @ARGV=read_options();

    if(@opt::v) { $Global::verbose = $#opt::v+1; } # Convert -v -v to v=2
    $Global::debug = (defined $opt::D);
    if(defined $opt::X) { $Global::ContextReplace = 1; }
    if(defined $opt::silent) { $Global::verbose = 0; }
    if(defined $opt::keeporder) { $Global::keeporder = 1; }
    if(defined $opt::group) { $Global::grouped = 1; }
    if(defined $opt::u) { $Global::grouped = 0; }
    if(defined $opt::0) { $/ = "\0"; }
    if(defined $opt::d) { my $e="sprintf \"$opt::d\""; $/ = eval $e; }
    if(defined $opt::p) { $Global::interactive = $opt::p; }
    if(defined $opt::q) { $Global::quoting = 1; }
    if(defined $opt::r) { $Global::ignore_empty = 1; }
    if(defined $opt::verbose) { $Global::stderr_verbose = 1; }
    if(defined $opt::I) { $Global::replace{'{}'} = $opt::I; }
    if(defined $opt::U) { $Global::replace{'{.}'} = $opt::U; }
    if(defined $opt::i) {
	$Global::replace{'{}'} = $opt::i eq "" ? "{}" : $opt::i;
    }
    if(defined $opt::basenamereplace) { $Global::replace{'{/}'} = $opt::basenamereplace; }
    if(defined $opt::dirnamereplace) { $Global::replace{'{//}'} = $opt::dirnamereplace; }
    if(defined $opt::basenameextensionreplace) {
        $Global::replace{'{/.}'} = $opt::basenameextensionreplace;
    }
    if(defined $opt::seqreplace) {
        $Global::replace{'{#}'} = $opt::seqreplace;
    }
    if(defined $opt::E) { $Global::end_of_file_string = $opt::E; }
    if(defined $opt::max_args) { $Global::max_number_of_args = $opt::max_args; }
    if(defined $opt::timeout) { $Global::timeoutq = TimeoutQueue->new($opt::timeout); }
    if(defined $opt::tmpdir) { $ENV{'TMPDIR'} = $opt::tmpdir; }
    if(defined $opt::help) { die_usage(); }
    if(defined $opt::colsep) { $Global::trim = 'lr'; }
    if(defined $opt::header) { $opt::colsep = defined $opt::colsep ? $opt::colsep : "\t"; }
    if(defined $opt::trim) { $Global::trim = $opt::trim; }
    if(defined $opt::arg_sep) { $Global::arg_sep = $opt::arg_sep; }
    if(defined $opt::arg_file_sep) { $Global::arg_file_sep = $opt::arg_file_sep; }
    if(defined $opt::number_of_cpus) { print SSHLogin::no_of_cpus(),"\n"; wait_and_exit(0); }
    if(defined $opt::number_of_cores) {
        print SSHLogin::no_of_cores(),"\n"; wait_and_exit(0);
    }
    if(defined $opt::max_line_length_allowed) {
        print Limits::Command::real_max_length(),"\n"; wait_and_exit(0);
    }
    if(defined $opt::version) { version(); wait_and_exit(0); }
    if(defined $opt::bibtex) { bibtex(); wait_and_exit(0); }
    if(defined $opt::record_env) { record_env(); wait_and_exit(0); }
    if(defined $opt::show_limits) { show_limits(); }
    if(@opt::sshlogin) { @Global::sshlogin = @opt::sshlogin; }
    if(@opt::sshloginfile) { read_sshloginfiles(@opt::sshloginfile); }
    if(@opt::return) { push @Global::ret_files, @opt::return; }
    if(not defined $opt::recstart and
       not defined $opt::recend) { $opt::recend = "\n"; }
    if(not defined $opt::blocksize) { $opt::blocksize = "1M"; }
    $opt::blocksize = multiply_binary_prefix($opt::blocksize);
    if(defined $opt::semaphore) { $Global::semaphore = 1; }
    if(defined $opt::semaphoretimeout) { $Global::semaphore = 1; }
    if(defined $opt::semaphorename) { $Global::semaphore = 1; }
    if(defined $opt::fg) { $Global::semaphore = 1; }
    if(defined $opt::bg) { $Global::semaphore = 1; }
    if(defined $opt::wait) { $Global::semaphore = 1; }
    if(defined $opt::timeout and $opt::timeout !~ /^\d+(\.\d+)?%?$/) {
	::error("--timeout must be seconds or percentage\n");
	wait_and_exit(255);
    }
    if(defined $opt::minversion) {
	print $Global::version,"\n";
	if($Global::version < $opt::minversion) {
	    wait_and_exit(255);
	} else {
	    wait_and_exit(0);
	}
    }
    if(not defined $opt::delay) {
	# Set --delay to --sshdelay if not set
	$opt::delay = $opt::sshdelay;
    }
    if($opt::compress_program) {
	$opt::compress = 1;
	$opt::decompress_program = $opt::compress_program." -dc";
    }
    if($opt::compress) {
	my ($compress, $decompress) = find_compression_program();
	$opt::compress_program ||= $compress;
	$opt::decompress_program ||= $decompress;
    }
    if($opt::tollef and not $opt::gnu and not $opt::plain) {
	# Behave like tollef parallel (from moreutils)
	if($Global::version > 2014_02_22) {
	    ::error("--tollef has been retired. See http://lists.gnu.org/archive/html/parallel/2013-02/msg00018.html\n");
	    ::wait_and_exit(255);
	}
	::warning("YOU ARE USING --tollef. IF THINGS ARE ACTING WEIRD USE --gnu.\n");
	::warning("--tollef is obsolete and will be retired 20140222.\n");
	::warning("See: http://lists.gnu.org/archive/html/parallel/2013-02/msg00018.html\n");
	$opt::u = 1;
	$Global::grouped = 0;
	$Global::quoting = 1;
	$opt::q = 1; # =
	if(defined $opt::max_lines) {
	    $opt::load = $opt::max_lines;
	    $opt::max_lines = undef;
	}
	if(not defined $opt::arg_sep) {
	    $Global::arg_sep = "--";
	}
	if(not grep(/$Global::arg_sep/, @ARGV)) {
	    unshift(@ARGV, $ENV{SHELL}, "-c", "--");
	}
    }

    if(defined $opt::nonall) {
	# Append a dummy empty argument
	push @ARGV, $Global::arg_sep, "";
    }
    if(defined $opt::tty) {
        # Defaults for --tty: -j1 -u
        # Can be overridden with -jXXX -g
        if(not defined $opt::P) {
            $opt::P = 1;
        }
        if(not defined $opt::group) {
            $Global::grouped = 0;
        }
    }
    if(@opt::trc) {
        push @Global::ret_files, @opt::trc;
        $opt::transfer = 1;
        $opt::cleanup = 1;
    }
    if(defined $opt::max_lines) {
	if($opt::max_lines eq "-0") {
	    # -l -0 (swallowed -0)
	    $opt::max_lines = 1;
	    $opt::0 = 1;
	    $/ = "\0";
	} elsif ($opt::max_lines == 0) {
	    # If not given (or if 0 is given) => 1
	    $opt::max_lines = 1;
	}
	$Global::max_lines = $opt::max_lines;
	if(not $opt::pipe) {
	    # --pipe -L means length of record - not max_number_of_args
	    $Global::max_number_of_args ||= $Global::max_lines;
	}
    }

    # Read more than one arg at a time (-L, -N)
    if(defined $opt::L) {
	$Global::max_lines = $opt::L;
	if(not $opt::pipe) {
	    # --pipe -L means length of record - not max_number_of_args
	    $Global::max_number_of_args ||= $Global::max_lines;
	}
    }
    if(defined $opt::max_replace_args) {
	$Global::max_number_of_args = $opt::max_replace_args;
	$Global::ContextReplace = 1;
    }
    if((defined $opt::L or defined $opt::max_replace_args)
       and
       not ($opt::xargs or $opt::m)) {
	$Global::ContextReplace = 1;
    }

    for (keys %Global::replace) {
	$Global::replace{$_} = ::maybe_quote($Global::replace{$_});
    }
    %Global::replace_rev = reverse %Global::replace;
    if(defined $opt::tag and not defined $opt::tagstring) {
	$opt::tagstring = $Global::replace{'{}'};
    }

    if(grep /^$Global::arg_sep$|^$Global::arg_file_sep$/o, @ARGV) {
        # Deal with ::: and ::::
        @ARGV=read_args_from_command_line();
    }

    # Semaphore defaults
    # Must be done before computing number of processes and max_line_length
    # because when running as a semaphore GNU Parallel does not read args
    $Global::semaphore ||= ($0 =~ m:(^|/)sem$:); # called as 'sem'
    if($Global::semaphore) {
        # A semaphore does not take input from neither stdin nor file
        @opt::a = ("/dev/null");
        push(@Global::unget_argv, [Arg->new("")]);
        $Semaphore::timeout = $opt::semaphoretimeout || 0;
        if(defined $opt::semaphorename) {
            $Semaphore::name = $opt::semaphorename;
        } else {
            $Semaphore::name = `tty`;
            chomp $Semaphore::name;
        }
        $Semaphore::fg = $opt::fg;
        $Semaphore::wait = $opt::wait;
        $Global::default_simultaneous_sshlogins = 1;
        if(not defined $opt::P) {
            $opt::P = 1;
        }
	if($Global::interactive and $opt::bg) {
	    ::error("Jobs running in the ".
		    "background cannot be interactive.\n");
            ::wait_and_exit(255);
	}
    }
    if(defined $opt::eta) {
        $opt::progress = $opt::eta;
    }
    if(defined $opt::bar) {
        $opt::progress = $opt::bar;
    }
    if(defined $opt::retired) {
	    ::error("-g has been retired. Use --group.\n");
	    ::error("-B has been retired. Use --bf.\n");
	    ::error("-T has been retired. Use --tty.\n");
	    ::error("-U has been retired. Use --er.\n");
	    ::error("-W has been retired. Use --wd.\n");
	    ::error("-Y has been retired. Use --shebang.\n");
	    ::error("-H has been retired. Use --halt.\n");
            ::wait_and_exit(255);
    }
    citation_notice();

    parse_sshlogin();
    parse_env_var();

    if(remote_hosts() and ($opt::X or $opt::m or $opt::xargs)) {
        # As we do not know the max line length on the remote machine
        # long commands generated by xargs may fail
        # If opt_N is set, it is probably safe
        ::warning("Using -X or -m with --sshlogin may fail.\n");
    }

    if(not defined $opt::P) {
        $opt::P = "100%";
    }
    open_joblog();
}

sub env_quote {
    my $v = shift;
    $v =~ s/([\\])/\\$1/g;
    $v =~ s/([\[\] \#\'\&\<\>\(\)\;\{\}\t\"\$\`\*\174\!\?\~])/\\$1/g;
    $v =~ s/\n/"\n"/g;
    return $v;
}

sub record_env {
    # Record current %ENV-keys in ~/.parallel/ignored_vars
    # Returns: N/A
    my $ignore_filename = $ENV{'HOME'} . "/.parallel/ignored_vars";
    if(open(my $vars_fh, ">", $ignore_filename)) {
	print $vars_fh map { $_,"\n" } keys %ENV;
    } else {
	::error("Cannot write to $ignore_filename\n");
	::wait_and_exit(255);
    }
}

sub parse_env_var {
    # Parse --env and set $Global::envvar
    # Returns: N/A
    $Global::envvar = "";
    my @vars = ();
    for my $varstring (@opt::env) {
        # Split up --env VAR1,VAR2
	push @vars, split /,/, $varstring;
    }
    if(grep { /^_$/ } @vars) {
	# Include all vars that are not in a clean environment
	if(open(my $vars_fh, "<", $ENV{'HOME'} . "/.parallel/ignored_vars")) {
	    my @ignore = <$vars_fh>;
	    chomp @ignore;
	    my %ignore;
	    @ignore{@ignore} = @ignore;
	    close $vars_fh;
	    push @vars, grep { not defined $ignore{$_} } keys %ENV;
	    @vars = grep { not /^_$/ } @vars;
	} else {
	    ::error("Run '$Global::progname --record-env' in a clean environment first.\n");
	    ::wait_and_exit(255);
	}
    }
    # Keep only defined variables
    @vars = grep { defined($ENV{$_}) } @vars;
    my @qcsh = map { my $a=$_; "setenv $a " . env_quote($ENV{$a})  } @vars;
    my @qbash = map { my $a=$_; "export $a=" . env_quote($ENV{$a}) } @vars;
    my @bash_functions = grep { substr($ENV{$_},0,4) eq "() {" } @vars;
    if(@bash_functions) {
	# Functions are not supported for all shells
	if($ENV{'SHELL'} !~ m:/(bash|rbash|zsh|rzsh|dash|ksh):) {
	    ::warning("Shell functions may not be supported in $ENV{'SHELL'}\n");
	}
    }
    push @qbash, map { my $a=$_; "eval $a\"\$$a\"" } @bash_functions;

    # Create lines like:
    # echo $SHELL | grep "/t\\{0,1\\}csh" >/dev/null && setenv V1 val1 && setenv V2 val2 || export V1=val1 && export V2=val2 ; echo "$V1$V2"
    if(@vars) {
	$Global::envvar = 
	    join"", 
	    (q{echo $SHELL | grep "/t\\{0,1\\}csh" > /dev/null && }
	     . join(" && ", @qcsh)
	     . q{ || }
	     . join(" && ", @qbash)
	     .q{;});
    }
    $Global::envvarlen = length $Global::envvar;
}

sub open_joblog {
    my $append = 0;
    if(($opt::resume or $opt::resume_failed)
       and
       not ($opt::joblog or $opt::results)) {
        ::error("--resume and --resume-failed require --joblog or --results.\n");
	::wait_and_exit(255);
    }
    if($opt::joblog) {
	if($opt::resume || $opt::resume_failed) {
	    if(open(my $joblog_fh, "<", $opt::joblog)) {
		# Read the joblog
		$append = <$joblog_fh>; # If there is a header: Open as append later
		my $joblog_regexp;
		if($opt::resume_failed) {
		    # Make a regexp that only matches commands with exit+signal=0
		    # 4 host 1360490623.067 3.445 1023 1222 0 0 command
		    $joblog_regexp='^(\d+)(?:\t[^\t]+){5}\t0\t0\t';
		} else {
		    # Just match the job number
		    $joblog_regexp='^(\d+)';
		}			
		while(<$joblog_fh>) {
		    if(/$joblog_regexp/o) {
			# This is 30% faster than set_job_already_run($1);
			vec($Global::job_already_run,($1||0),1) = 1;
		    } elsif(not /\d+\t[^\t]+\t([0-9.]+\t){6}/) {
			::error("Format of '$opt::joblog' is wrong: $_");
			::wait_and_exit(255);
		    }
		}
		close $joblog_fh;
	    }
	}
	if($append) {
	    # Append to joblog
	    if(not open($Global::joblog, ">>", $opt::joblog)) {
		::error("Cannot append to --joblog $opt::joblog.\n");
		::wait_and_exit(255);
	    }
	} else {
	    if($opt::joblog eq "-") {
		# Use STDOUT as joblog
		$Global::joblog = $Global::fd{1};
	    } elsif(not open($Global::joblog, ">", $opt::joblog)) {
		# Overwrite the joblog
		::error("Cannot write to --joblog $opt::joblog.\n");
		::wait_and_exit(255);
	    }
	    print $Global::joblog
		join("\t", "Seq", "Host", "Starttime", "Runtime",
		     "Send", "Receive", "Exitval", "Signal", "Command"
		). "\n";
	}
    }
}

sub find_compression_program {
    # Find a fast compression program
    # Returns:
    #   $compress_program = compress program with options
    #   $decompress_program = decompress program with options

    # Search for these. Sorted by speed
    my @prg = qw(lzop pigz gzip pbzip2 plzip bzip2 lzma lzip xz);
    for my $p (@prg) {
	if(which($p)) {
	    return ("$p -c -1","$p -dc");
	}
    }
    # Fall back to cat
    return ("cat","cat");
}

sub which {
    # Input:
    #   $program = program to find the path to
    # Returns:
    #   $full_path = full path to $program. undef if not found
    my $program = $_[0];
    
    return (grep { -e $_."/".$program } split(":",$ENV{'PATH'}))[0];
}

sub read_options {
    # Read options from command line, profile and $PARALLEL
    # Returns:
    #   @ARGV_no_opt = @ARGV without --options
    # This must be done first as this may exec myself
    if(defined $ARGV[0] and ($ARGV[0] =~ /^--shebang/ or
			     $ARGV[0] =~ /^--shebang-?wrap/ or
			     $ARGV[0] =~ /^--hashbang/)) {
        # Program is called from #! line in script
	# remove --shebang-wrap if it is set
        $opt::shebang_wrap = ($ARGV[0] =~ s/^--shebang-?wrap *//);
	# remove --shebang if it is set
	$opt::shebang = ($ARGV[0] =~ s/^--shebang *//);
	# remove --hashbang if it is set
        $opt::shebang .= ($ARGV[0] =~ s/^--hashbang *//);
	if($opt::shebang) {
	    my $argfile = shell_quote_scalar(pop @ARGV);
	    # exec myself to split $ARGV[0] into separate fields
	    exec "$0 --skip-first-line -a $argfile @ARGV";
	}
	if($opt::shebang_wrap) {
	    my $parser = shift @ARGV;
	    my $scriptfile = shell_quote_scalar(shift @ARGV);
	    # exec myself to split $ARGV[0] into separate fields
	    exec "$0 --internal-pipe-means-argfiles $parser $scriptfile ::: @ARGV";
	}
    }

    Getopt::Long::Configure("bundling","require_order");
    my @ARGV_copy = @ARGV;
    # Check if there is a --profile to set @opt::profile
    get_options_from_array(\@ARGV_copy,"profile|J=s","plain") || die_usage();
    my @ARGV_profile = ();
    my @ARGV_env = ();
    if(not $opt::plain) {
	# Add options from .parallel/config and other profiles
	my @config_profiles = (
	    "/etc/parallel/config",
	    $ENV{'HOME'}."/.parallel/config",
	    $ENV{'HOME'}."/.parallelrc");
	my @profiles = @config_profiles;
	if(@opt::profile) {
	    # --profile overrides default profiles
	    @profiles = ();
	    for my $profile (@opt::profile) {
		if(-r $profile) {
		    push @profiles, $profile;
		} else {
		    push @profiles, $ENV{'HOME'}."/.parallel/".$profile;
		}
	    }
	}
	for my $profile (@profiles) {
	    if(-r $profile) {
		open (my $in_fh, "<", $profile) || ::die_bug("read-profile: $profile");
		while(<$in_fh>) {
		    /^\s*\#/ and next;
		    chomp;
		    push @ARGV_profile, shell_unquote(split/(?<![\\])\s/, $_);
		}
		close $in_fh;
	    } else {
		if(grep /^$profile$/, @config_profiles) {
		    # config file is not required to exist
		} else {
		    ::error("$profile not readable.\n");
		    wait_and_exit(255);
		}
	    }
	}
	# Add options from shell variable $PARALLEL
	if($ENV{'PARALLEL'}) {
	    @ARGV_env = shell_unquote(split/(?<![\\])\s/, $ENV{'PARALLEL'});
	}
    }
    Getopt::Long::Configure("bundling","require_order");
    get_options_from_array(\@ARGV_profile) || die_usage();
    get_options_from_array(\@ARGV_env) || die_usage();
    get_options_from_array(\@ARGV) || die_usage();

    # Prepend non-options to @ARGV (such as commands like 'nice')
    unshift @ARGV, @ARGV_profile, @ARGV_env;
    return @ARGV;
}

sub read_args_from_command_line {
    # Arguments given on the command line after:
    #   ::: ($Global::arg_sep)
    #   :::: ($Global::arg_file_sep)
    # Removes the arguments from @ARGV and:
    # - puts filenames into -a
    # - puts arguments into files and add the files to -a
    # Input:
    #   @::ARGV = command option ::: arg arg arg :::: argfiles
    # Returns:
    #   @argv_no_argsep = @::ARGV without ::: and :::: and following args
    my @new_argv = ();
    for(my $arg = shift @ARGV; @ARGV; $arg = shift @ARGV) {
        if($arg eq $Global::arg_sep
	   or
	   $arg eq $Global::arg_file_sep) {
	    my $group = $arg; # This group of arguments is args or argfiles
	    my @group;
	    while(defined ($arg = shift @ARGV)) {
		if($arg eq $Global::arg_sep
		   or
		   $arg eq $Global::arg_file_sep) {
		    # exit while loop if finding new separator
		    last;
		} else {
		    # If not hitting ::: or ::::
		    # Append it to the group
		    push @group, $arg;
		}
	    }

	    if($group eq $Global::arg_file_sep
	       or ($opt::internal_pipe_means_argfiles and $opt::pipe)
		) {
		# Group of file names on the command line.
		# Append args into -a
		push @opt::a, @group;
	    } elsif($group eq $Global::arg_sep) {
		# Group of arguments on the command line.
		# Put them into a file.
		# Create argfile
		my ($outfh,$name) = ::tempfile(SUFFIX => ".arg");
		unlink($name);
		# Put args into argfile
		print $outfh map { $_,$/ } @group;
		seek $outfh, 0, 0;
		# Append filehandle to -a
		push @opt::a, $outfh;
	    } else {
		::die_bug("Unknown command line group: $group");
	    }
	    if(defined($arg)) {
		# $arg is ::: or ::::
		redo;
	    } else {
		# $arg is undef -> @ARGV empty
		last;
	    }
	}
	push @new_argv, $arg;
    }
    # Output: @ARGV = command to run with options
    return @new_argv;
}

sub cleanup {
    # Returns: N/A
    if(@opt::basefile) { cleanup_basefile(); }
}

sub __QUOTING_ARGUMENTS_FOR_SHELL__ {}

sub shell_quote {
    my @strings = (@_);
    for my $a (@strings) {
        $a =~ s/([\002-\011\013-\032\\\#\?\`\(\)\{\}\[\]\*\>\<\~\|\; \"\!\$\&\'\202-\377])/\\$1/g;
        $a =~ s/[\n]/'\n'/g; # filenames with '\n' is quoted using \'
    }
    return wantarray ? @strings : "@strings";
}

sub shell_quote_empty {
    my @strings = shell_quote(@_);
    for my $a (@strings) {
	if($a eq "") {
	    $a = "''";
	}
    }
    return wantarray ? @strings : "@strings";
}

sub shell_quote_scalar {
    # Quote the string so shell will not expand any special chars
    # Inputs:
    #   $string = string to be quoted
    # Returns:
    #   $shell_quoted = string quoted with \ as needed by the shell
    my $a = $_[0];
    if(defined $a) {
	$a =~ s/([\002-\011\013-\032\\\#\?\`\(\)\{\}\[\]\*\>\<\~\|\; \"\!\$\&\'\202-\377])/\\$1/g;
	$a =~ s/[\n]/'\n'/g; # filenames with '\n' is quoted using \'
    }
    return $a;
}

sub shell_quote_file {
    # Quote the string so shell will not expand any special chars and prepend ./ if needed
    # Input:
    #   $filename = filename to be shell quoted
    # Returns:
    #   $quoted_filename = filename quoted with \ as needed by the shell and ./ if needed
    my $a = shell_quote_scalar(shift);
    if(defined $a) {
	if($a =~ m:^/: or $a =~ m:^\./:) {
	    # /abs/path or ./rel/path => skip
	} else {
	    # rel/path => ./rel/path
	    $a = "./".$a;
	}
    }
    return $a;
}


sub maybe_quote {
    # If $Global::quoting is set then quote the string so shell will not expand any special chars
    # Else do not quote
    # Inputs:
    #   $string = string to be quoted
    # Returns:
    #   $maybe_quoted_string = $string quoted if needed
    if($Global::quoting) {
	return shell_quote_scalar(@_);
    } else {
	return "@_";
    }
}

sub maybe_unquote {
    # If $Global::quoting then unquote the string as shell would
    # Else do not unquote
    # Inputs:
    #   $maybe_quoted_string = string to be maybe unquoted
    # Returns:
    #   $string = $maybe_quoted_string unquoted if needed
    if($Global::quoting) {
	return shell_unquote(@_);
    } else {
	return "@_";
    }
}

sub shell_unquote {
    # Unquote strings from shell_quote
    # Inputs:
    #   @strings = strings to be unquoted
    # Returns:
    #   @unquoted_strings = @strings with shell quoting removed
    my @strings = (@_);
    my $arg;
    for my $arg (@strings) {
        if(not defined $arg) {
            $arg = "";
        }
        $arg =~ s/'\n'/\n/g; # filenames with '\n' is quoted using \'
        $arg =~ s/\\([\002-\011\013-\032])/$1/g;
        $arg =~ s/\\([\#\?\`\(\)\{\}\*\>\<\~\|\; \"\!\$\&\'])/$1/g;
        $arg =~ s/\\\\/\\/g;
    }
    return wantarray ? @strings : "@strings";
}

sub __FILEHANDLES__ {}


sub save_stdin_stdout_stderr {
    # Remember the original STDIN, STDOUT and STDERR
    # and file descriptors opened by the shell (e.g. 3>/tmp/foo)
    # Returns: N/A

    # Find file descriptors that are already opened (by the shell)
    for my $fdno (1..61) { 
	# /dev/fd/62 and above are used by bash for <(cmd)
	my $fh;
	if(open($fh,">&=",$fdno)) {
	    $Global::fd{$fdno}=$fh;
	}
    }
    open $Global::original_stderr, ">&", "STDERR" or
	::die_bug("Can't dup STDERR: $!");
    open $Global::original_stdin, "<&", "STDIN" or
	::die_bug("Can't dup STDIN: $!");
}

sub enough_file_handles {
    # check that we have enough filehandles available for starting
    # another job
    # Returns:
    #   1 if ungrouped (thus not needing extra filehandles)
    #   0 if too few filehandles
    #   1 if enough filehandles
    if($Global::grouped) {
        my %fh;
        my $enough_filehandles = 1;
  	# perl uses 7 filehandles for something?
        # open3 uses 2 extra filehandles temporarily
        # We need a filehandle for each redirected file descriptor 
	# (normally just STDOUT and STDERR)
	for my $i (1..(7+2+keys %Global::fd)) {
            $enough_filehandles &&= open($fh{$i}, "<", "/dev/null");
        }
        for (values %fh) { close $_; }
        return $enough_filehandles;
    } else {
	# Ungrouped does not need extra file handles
	return 1;
    }
}

sub open_or_exit {
    # Open a file name or exit if the fille cannot be opened
    # Inputs:
    #   $file = filehandle or filename to open
    # Returns:
    #   $fh = file handle to read-opened file
    my $file = shift;
    if($file eq "-") {
	$Global::stdin_in_opt_a = 1;
	return ($Global::original_stdin || *STDIN);
    }
    if(ref $file eq "GLOB") {
	# This is an open filehandle
	return $file;
    }
    my $fh = gensym;
    if(not open($fh, "<", $file)) {
        ::error("Cannot open input file `$file': No such file or directory.\n");
        wait_and_exit(255);
    }
    return $fh;
}

sub __RUNNING_THE_JOBS_AND_PRINTING_PROGRESS__ {}

# Variable structure:
#
#    $Global::running{$pid} = Pointer to Job-object
#    @Global::virgin_jobs = Pointer to Job-object that have received no input
#    $Global::host{$sshlogin} = Pointer to SSHLogin-object
#    $Global::total_running = total number of running jobs
#    $Global::total_started = total jobs started

sub init_run_jobs {
    $Global::total_running = 0;
    $Global::total_started = 0;
    $Global::tty_taken = 0;
    $SIG{USR1} = \&list_running_jobs;
    $SIG{USR2} = \&toggle_progress;
    if(@opt::basefile) { setup_basefile(); }
}

sub start_more_jobs {
    # Run start_another_job() but only if:
    #   * not $Global::start_no_new_jobs set
    #   * not JobQueue is empty
    #   * not load on server is too high
    #   * not server swapping
    #   * not too short time since last remote login
    # Returns:
    #   $jobs_started = number of jobs started
    my $jobs_started = 0;
    my $jobs_started_this_round = 0;
    if($Global::start_no_new_jobs) {
	return $jobs_started;
    }
    if($Global::max_procs_file) {
	my $mtime = (stat($Global::max_procs_file))[9];
	if($mtime > $Global::max_procs_file_last_mod) {
	    $Global::max_procs_file_last_mod = $mtime;
	    for my $sshlogin (values %Global::host) {
		$sshlogin->set_max_jobs_running(undef);
	    }
	}
    }
    do {
	$jobs_started_this_round = 0;
	# This will start 1 job on each --sshlogin (if possible)
	# thus distribute the jobs on the --sshlogins round robin
	for my $sshlogin (values %Global::host) {
	    if($Global::JobQueue->empty() and not $opt::pipe) {
		last;
	    }
	    debug("Running jobs before on ".$sshlogin->string().": ".$sshlogin->jobs_running()."\n");
	    if ($sshlogin->jobs_running() < $sshlogin->max_jobs_running()) {
		if($opt::load and $sshlogin->loadavg_too_high()) {
		    # The load is too high or unknown
		    next;
		}
		if($opt::noswap and $sshlogin->swapping()) {
		    # The server is swapping
		    next;
		}
		if($sshlogin->too_fast_remote_login()) {
		    # It has been too short since 
		    next;
		}
		debug($sshlogin->string()." has ".$sshlogin->jobs_running()
		      . " out of " . $sshlogin->max_jobs_running()
		      . " jobs running. Start another.\n");
		if(start_another_job($sshlogin) == 0) {
		    # No more jobs to start on this $sshlogin
		    debug("No jobs started on ".$sshlogin->string()."\n");
		    next;
		}
		$sshlogin->inc_jobs_running();
		$sshlogin->set_last_login_at(::now());
		$jobs_started++;
		$jobs_started_this_round++;
	    }
	    debug("Running jobs after on ".$sshlogin->string().": ".$sshlogin->jobs_running()
		  ." of ".$sshlogin->max_jobs_running() ."\n");
	}
    } while($jobs_started_this_round);

    return $jobs_started;
}

sub start_another_job {
    # If there are enough filehandles
    #   and JobQueue not empty
    #   and not $job is in joblog
    # Then grab a job from Global::JobQueue,
    #   start it at sshlogin
    #   mark it as virgin_job
    # Inputs:
    #   $sshlogin = the SSHLogin to start the job on
    # Returns:
    #   1 if another jobs was started
    #   0 otherwise
    my $sshlogin = shift;
    # Do we have enough file handles to start another job?
    if(enough_file_handles()) {
        if($Global::JobQueue->empty() and not $opt::pipe) {
            # No more commands to run
	    debug("Not starting: JobQueue empty\n");
	    return 0;
        } else {
            my $job;
	    # Skip jobs already in job log
	    # Skip jobs already in results
            do {
		$job = get_job_with_sshlogin($sshlogin);
		if(not defined $job) {
		    # No command available for that sshlogin
		    debug("Not starting: no jobs available for ".$sshlogin->string()."\n");
		    return 0;
		}
	    } while ($job->is_already_in_joblog()
		     or
		     ($opt::results and $opt::resume and $job->is_already_in_results()));
	    debug("Command to run on '".$job->sshlogin()->string()."': '".$job->replaced()."'\n");
            if($job->start()) {
                $Global::running{$job->pid()} = $job;
		if($opt::pipe) {
		    push(@Global::virgin_jobs,$job);
		}
                debug("Started as seq ",$job->seq()," pid:",$job->pid(),"\n");
                return 1;
            } else {
                # Not enough processes to run the job.
		# Put it back on the queue.
		$Global::JobQueue->unget($job);
		# Count down the number of jobs to run for this SSHLogin.
		my $max = $sshlogin->max_jobs_running();
		if($max > 1) { $max--; }
		$sshlogin->set_max_jobs_running($max);
		# Sleep up to 300 ms to give other processes time to die
		::usleep(rand()*300);
		::warning("No more processes: ",
			  "Decreasing number of running jobs to $max. ",
			  "Raising ulimit -u or /etc/security/limits.conf may help.\n");
		return 0;
            }
        }
    } else {
        # No more file handles
	$Global::no_more_file_handles_warned++ or
	    ::warning("No more file handles. ".
		      "Raising ulimit -n or /etc/security/limits.conf may help.\n");
        return 0;
    }
}

sub init_progress {
    # Returns:
    #   list of computers for progress output
    $|=1;
    if($opt::bar) {
	return("","");
    }
    my %progress = progress();
    return ("\nComputers / CPU cores / Max jobs to run\n",
            $progress{'workerlist'});
}

sub drain_job_queue {
    # Returns: N/A
    $Private::first_completed ||= time;
    if($opt::progress) {
        print $Global::original_stderr init_progress();
    }
    my $last_header="";
    my $sleep = 0.2;
    do {
        while($Global::total_running > 0) {
            debug($Global::total_running, "==", scalar
		  keys %Global::running," slots: ", $Global::max_jobs_running);
	    if($opt::pipe) {
		# When using --pipe sometimes file handles are not closed properly
		for my $job (values %Global::running) {
		    my $stdin_fh = $job->fd(0);
		    close $stdin_fh;
		}
	    }
            if($opt::progress) {
                my %progress = progress();
                if($last_header ne $progress{'header'}) {
                    print $Global::original_stderr "\n", $progress{'header'}, "\n";
                    $last_header = $progress{'header'};
                }
                print $Global::original_stderr "\r",$progress{'status'};
		flush $Global::original_stderr;
            }
	    if($Global::total_running < $Global::max_jobs_running
	       and not $Global::JobQueue->empty()) {
		# These jobs may not be started because of loadavg
		# or too little time between each ssh login.
		if(start_more_jobs() > 0) {
		    # Exponential back-on if jobs were started
		    $sleep = $sleep/2+0.001;
		}
	    }
            # Sometimes SIGCHLD is not registered, so force reaper
	    $sleep = ::reap_usleep($sleep);
        }
        if(not $Global::JobQueue->empty()) {
	    # These jobs may not be started because of loadavg
	    # or too little time between each ssh login.
            start_more_jobs();
	    $sleep = ::reap_usleep($sleep);
        }
    } while ($Global::total_running > 0
	     or
	     not $Global::start_no_new_jobs and not $Global::JobQueue->empty());
    if($opt::progress) {
	my %progress = progress();
	print $Global::original_stderr "\r", $progress{'status'}, "\n";
	flush $Global::original_stderr;
    }
}

sub toggle_progress {
    # Turn on/off progress view
    # Returns: N/A
    $opt::progress = not $opt::progress;
    if($opt::progress) {
        print $Global::original_stderr init_progress();
    }
}

sub progress {
    # Returns:
    #   list of workers
    #   header that will fit on the screen
    #   status message that will fit on the screen
    my $termcols = terminal_columns();
    my @workers = sort keys %Global::host;
    my %sshlogin = map { $_ eq ":" ? ($_=>"local") : ($_=>$_) } @workers;
    my $workerno = 1;
    my %workerno = map { ($_=>$workerno++) } @workers;
    my $workerlist = "";
    for my $w (@workers) {
        $workerlist .=
        $workerno{$w}.":".$sshlogin{$w} ." / ".
            ($Global::host{$w}->ncpus() || "-")." / ".
            $Global::host{$w}->max_jobs_running()."\n";
    }
    my $eta = "";
    my ($status,$header)=("","");
    if($opt::eta or $opt::bar) {
        my $completed = 0;
        for(@workers) { $completed += $Global::host{$_}->jobs_completed() }
        if($completed) {
	    my $total = $Global::JobQueue->total_jobs();
	    my $left = $total - $completed;
	    my $pctcomplete = $completed / $total;
	    my $timepassed = (time - $Private::first_completed);
	    my $avgtime = $timepassed / $completed;
	    $Private::smoothed_avg_time ||= $avgtime;
	    # Smooth the eta so it does not jump wildly
	    $Private::smoothed_avg_time = (1 - $pctcomplete) *
		$Private::smoothed_avg_time + $pctcomplete * $avgtime;
	    my $this_eta;
	    $Private::last_time ||= $timepassed;
	    if($timepassed != $Private::last_time
	       or not defined $Private::last_eta) {
		$Private::last_time = $timepassed;
		$this_eta = $left * $Private::smoothed_avg_time;
		$Private::last_eta = $this_eta;
	    } else {
		$this_eta = $Private::last_eta;
	    }
	    $eta = sprintf("ETA: %ds %dleft %.2favg  ", $this_eta, $left, $avgtime);
	    if($opt::bar) {
		my $arg = $Global::newest_job ? 
		    $Global::newest_job->{'commandline'}->simple_replace_placeholders($Global::replace{"{}"}) : "";
		my $bar = sprintf("%d%% %ds %s", $pctcomplete*100, $this_eta, $arg);
		my $rev = '[7m';
		my $reset = '[0m';
		my $terminal_width = terminal_columns();
		my $s = sprintf("%-${terminal_width}s",$bar);
		my $width = int($terminal_width * $pctcomplete);
		$s =~ s/^(.{$width})/$1$reset/;
		$s = "\r#   ".int($this_eta)." sec $arg" . "\r". $pctcomplete*100 # Prefix with zenity header
		    . "\r" . $rev . $s . $reset;  
		$status = $s;
	    }
        }
    }
    if($opt::bar) {
	return ("workerlist" => "", "header" => "", "status" => $status);
    }
    $status = "x"x($termcols+1);
    if(length $status > $termcols) {
        # sshlogin1:XX/XX/XX%/XX.Xs sshlogin2:XX/XX/XX%/XX.Xs sshlogin3:XX/XX/XX%/XX.Xs
        $header = "Computer:jobs running/jobs completed/%of started jobs/Average seconds to complete";
        $status = $eta .
            join(" ",map
                 {
                     if($Global::total_started) {
                         my $completed = ($Global::host{$_}->jobs_completed()||0);
                         my $running = $Global::host{$_}->jobs_running();
                         my $time = $completed ? (time-$^T)/($completed) : "0";
                         sprintf("%s:%d/%d/%d%%/%.1fs ",
                                 $sshlogin{$_}, $running, $completed,
                                 ($running+$completed)*100
                                 / $Global::total_started, $time);
                     }
                 } @workers);
    }
    if(length $status > $termcols) {
        # 1:XX/XX/XX%/XX.Xs 2:XX/XX/XX%/XX.Xs 3:XX/XX/XX%/XX.Xs 4:XX/XX/XX%/XX.Xs
        $header = "Computer:jobs running/jobs completed/%of started jobs";
        $status = $eta .
            join(" ",map
                 {
                     my $completed = ($Global::host{$_}->jobs_completed()||0);
                     my $running = $Global::host{$_}->jobs_running();
                     my $time = $completed ? (time-$^T)/($completed) : "0";
                     sprintf("%s:%d/%d/%d%%/%.1fs ",
                             $workerno{$_}, $running, $completed,
                             ($running+$completed)*100
                             / $Global::total_started, $time);
                 } @workers);
    }
    if(length $status > $termcols) {
        # sshlogin1:XX/XX/XX% sshlogin2:XX/XX/XX% sshlogin3:XX/XX/XX%
        $header = "Computer:jobs running/jobs completed/%of started jobs";
        $status = $eta .
            join(" ",map
                 { sprintf("%s:%d/%d/%d%%",
                           $sshlogin{$_},
                           $Global::host{$_}->jobs_running(),
                           ($Global::host{$_}->jobs_completed()||0),
                           ($Global::host{$_}->jobs_running()+
                            ($Global::host{$_}->jobs_completed()||0))*100
                           / $Global::total_started) }
                 @workers);
    }
    if(length $status > $termcols) {
        # 1:XX/XX/XX% 2:XX/XX/XX% 3:XX/XX/XX% 4:XX/XX/XX% 5:XX/XX/XX% 6:XX/XX/XX%
        $header = "Computer:jobs running/jobs completed/%of started jobs";
        $status = $eta .
            join(" ",map
                 { sprintf("%s:%d/%d/%d%%",
                           $workerno{$_},
                           $Global::host{$_}->jobs_running(),
                           ($Global::host{$_}->jobs_completed()||0),
                           ($Global::host{$_}->jobs_running()+
                            ($Global::host{$_}->jobs_completed()||0))*100
                           / $Global::total_started) }
                 @workers);
    }
    if(length $status > $termcols) {
        # sshlogin1:XX/XX/XX% sshlogin2:XX/XX/XX% sshlogin3:XX/XX sshlogin4:XX/XX
        $header = "Computer:jobs running/jobs completed";
        $status = $eta .
            join(" ",map
                       { sprintf("%s:%d/%d",
                                 $sshlogin{$_}, $Global::host{$_}->jobs_running(),
                                 ($Global::host{$_}->jobs_completed()||0)) }
                       @workers);
    }
    if(length $status > $termcols) {
        # sshlogin1:XX/XX sshlogin2:XX/XX sshlogin3:XX/XX sshlogin4:XX/XX
        $header = "Computer:jobs running/jobs completed";
        $status = $eta .
            join(" ",map
                       { sprintf("%s:%d/%d",
                                 $sshlogin{$_}, $Global::host{$_}->jobs_running(),
                                 ($Global::host{$_}->jobs_completed()||0)) }
                       @workers);
    }
    if(length $status > $termcols) {
        # 1:XX/XX 2:XX/XX 3:XX/XX 4:XX/XX 5:XX/XX 6:XX/XX
        $header = "Computer:jobs running/jobs completed";
        $status = $eta .
            join(" ",map
                       { sprintf("%s:%d/%d",
                                 $workerno{$_}, $Global::host{$_}->jobs_running(),
                                 ($Global::host{$_}->jobs_completed()||0)) }
                       @workers);
    }
    if(length $status > $termcols) {
        # sshlogin1:XX sshlogin2:XX sshlogin3:XX sshlogin4:XX sshlogin5:XX
        $header = "Computer:jobs completed";
        $status = $eta .
            join(" ",map
                       { sprintf("%s:%d",
                                 $sshlogin{$_},
                                 ($Global::host{$_}->jobs_completed()||0)) }
                       @workers);
    }
    if(length $status > $termcols) {
        # 1:XX 2:XX 3:XX 4:XX 5:XX 6:XX
        $header = "Computer:jobs completed";
        $status = $eta .
            join(" ",map
                       { sprintf("%s:%d",
                                 $workerno{$_},
                                 ($Global::host{$_}->jobs_completed()||0)) }
                       @workers);
    }
    return ("workerlist" => $workerlist, "header" => $header, "status" => $status);
}

sub terminal_columns {
    # Get the number of columns of the display
    # Returns:
    #   number of columns of the screen
    if(not $Private::columns) {
        $Private::columns = $ENV{'COLUMNS'};
        if(not $Private::columns) {
            my $resize = qx{ resize 2>/dev/null };
            $resize =~ /COLUMNS=(\d+);/ and do { $Private::columns = $1; };
        }
        $Private::columns ||= 80;
    }
    return $Private::columns;
}

sub get_job_with_sshlogin {
    # Returns:
    #   next job object for $sshlogin if any available
    my $sshlogin = shift;

    my $job = $Global::JobQueue->get();
    if(not defined $job) {
        # No more jobs
	::debug("No more jobs: JobQueue empty\n");
        return undef;
    }

    my $clean_command = $job->replaced();
    if($clean_command =~ /^\s*$/) {
        # Do not run empty lines
        if(not $Global::JobQueue->empty()) {
            return get_job_with_sshlogin($sshlogin);
        } else {
            return undef;
        }
    }
    $job->set_sshlogin($sshlogin);
    if($opt::retries and $clean_command and
       $job->failed_here()) {
        # This command with these args failed for this sshlogin
        my ($no_of_failed_sshlogins,$min_failures) = $job->min_failed();
        if($no_of_failed_sshlogins == keys %Global::host and
           $job->failed_here() == $min_failures) {
            # It failed the same or more times on another host:
            # run it on this host
        } else {
            # If it failed fewer times on another host:
            # Find another job to run
            my $nextjob;
            if(not $Global::JobQueue->empty()) {
		# This can potentially recurse for all args
                no warnings 'recursion';
                $nextjob = get_job_with_sshlogin($sshlogin);
            }
            # Push the command back on the queue
            $Global::JobQueue->unget($job);
            return $nextjob;
        }
    }
    return $job;
}

sub __REMOTE_SSH__ {}

sub read_sshloginfiles {
    # Returns: N/A
    for my $s (@_) {
	read_sshloginfile($s);
    }
}

sub read_sshloginfile {
    # Returns: N/A
    my $file = shift;
    my $close = 1;
    my $in_fh;
    if($file eq "..") {
        $file = $ENV{'HOME'}."/.parallel/sshloginfile";
    }
    if($file eq ".") {
        $file = "/etc/parallel/sshloginfile";
    }
    if($file eq "-") {
	$in_fh = *STDIN;
	$close = 0;
    } else {
	if(not open($in_fh, "<", $file)) {
	    # Try the filename
	    if(not open($in_fh, "<", $ENV{'HOME'}."/.parallel/".$file)) {
		# Try prepending ~/.parallel
		::error("Cannot open $file.\n");
		::wait_and_exit(255);
	    }
	}
    }
    while(<$in_fh>) {
        chomp;
        /^\s*#/ and next;
        /^\s*$/ and next;
        push @Global::sshlogin, $_;
    }
    if($close) {
	close $in_fh;
    }
}

sub parse_sshlogin {
    # Returns: N/A
    my @login;
    if(not @Global::sshlogin) { @Global::sshlogin = (":"); }
    for my $sshlogin (@Global::sshlogin) {
        # Split up -S sshlogin,sshlogin
        for my $s (split /,/, $sshlogin) {
            if ($s eq ".." or $s eq "-") {
                read_sshloginfile($s);
            } else {
                push (@login, $s);
            }
        }
    }
    for my $sshlogin_string (@login) {
        my $sshlogin = SSHLogin->new($sshlogin_string);
        $sshlogin->set_maxlength(Limits::Command::max_length());
        $Global::host{$sshlogin->string()} = $sshlogin;
    }
    debug("sshlogin: ", my_dump(%Global::host),"\n");
    if($opt::transfer or @opt::return or $opt::cleanup or @opt::basefile) {
        if(not remote_hosts()) {
            # There are no remote hosts
            if(@opt::trc) {
		::warning("--trc ignored as there are no remote --sshlogin.\n");
            } elsif (defined $opt::transfer) {
		::warning("--transfer ignored as there are no remote --sshlogin.\n");
            } elsif (@opt::return) {
                ::warning("--return ignored as there are no remote --sshlogin.\n");
            } elsif (defined $opt::cleanup) {
		::warning("--cleanup ignored as there are no remote --sshlogin.\n");
            } elsif (@opt::basefile) {
                ::warning("--basefile ignored as there are no remote --sshlogin.\n");
            }
        }
    }
}

sub remote_hosts {
    # Return sshlogins that are not ':'
    # Returns:
    #   list of sshlogins with ':' removed
    return grep !/^:$/, keys %Global::host;
}

sub setup_basefile {
    # Transfer basefiles to each $sshlogin
    # This needs to be done before first jobs on $sshlogin is run
    # Returns: N/A
    my $cmd = "";
    my $rsync_destdir;
    my $workdir;
    for my $sshlogin (values %Global::host) {
      if($sshlogin->string() eq ":") { next }
      for my $file (@opt::basefile) {
	if($file !~ m:^/: and $opt::workdir eq "...") {
	  ::error("Work dir '...' will not work with relative basefiles\n");
	  ::wait_and_exit(255);
	}
	$workdir ||= Job->new("")->workdir();
	$cmd .= $sshlogin->rsync_transfer_cmd($file,$workdir) . "&";
      }
    }
    $cmd .= "wait;";
    debug("basesetup: $cmd\n");
    print `$cmd`;
}

sub cleanup_basefile {
    # Remove the basefiles transferred
    # Returns: N/A
    my $cmd="";
    my $workdir = Job->new("")->workdir();
    for my $sshlogin (values %Global::host) {
        if($sshlogin->string() eq ":") { next }
        for my $file (@opt::basefile) {
	  $cmd .= $sshlogin->cleanup_cmd($file,$workdir)."&";
        }
    }
    $cmd .= "wait;";
    debug("basecleanup: $cmd\n");
    print `$cmd`;
}

sub filter_hosts {
    my(@cores, @cpus, @maxline, @echo);
    while (my ($host, $sshlogin) = each %Global::host) {
	# The 'true' is used to get the $host out later
	my $sshcmd = "true $host;" . $sshlogin->sshcommand()." ".$sshlogin->serverlogin();
	push(@cores, $host."\t".$sshcmd." parallel --number-of-cores\n");
	push(@cpus, $host."\t".$sshcmd." parallel --number-of-cpus\n");
	push(@maxline, $host."\t".$sshcmd." parallel --max-line-length-allowed\n");
	# 'echo' is used to get the best possible value for an ssh login time
	push(@echo, $host."\t".$sshcmd." echo\n");
    }
    my ($fh, $tmpfile) = ::tempfile(SUFFIX => ".ssh");
    print $fh @cores, @cpus, @maxline, @echo;
    close $fh;
    # --timeout 5: Setting up an SSH connection and running a simple
    #              command should never take > 5 sec.
    # --delay 0.1: If multiple sshlogins use the same proxy the delay
    #              will make it less likely to overload the ssh daemon.
    # --retries 3: If the ssh daemon it overloaded, try 3 times
    my $cmd = "cat $tmpfile | $0 -j0 --timeout 5 -s 1000 --joblog - --plain --delay 0.1 --retries 3 --tag --tagstring {1} --colsep '\t' -k eval {2} 2>/dev/null";
    ::debug($cmd."\n");
    open(my $host_fh, "-|", $cmd) || ::die_bug("parallel host check: $cmd");
    my (%ncores, %ncpus, %time_to_login, %maxlen, %echo, @down_hosts);
    while(<$host_fh>) {
	chomp;
	my @col = split /\t/, $_;
	if(defined $col[6]) {
	    # This is a line from --joblog
	    # seq host time spent sent received exit signal command
	    # 2 : 1372607672.654 0.675 0 0 0 0 eval true\ m\;ssh\ m\ parallel\ --number-of-cores
	    if($col[0] eq "Seq" and $col[1] eq "Host" and
		    $col[2] eq "Starttime" and $col[3] eq "Runtime") {
		# Header => skip
		next;
	    }
	    # Get server from: eval true server\;
	    $col[8] =~ /eval true..([^;]+).;/ or ::die_bug("col8 does not contain host: $col[8]");
	    my $host = $1;
	    $host =~ s/\\//g;
	    $Global::host{$host} or next;
	    if($col[6] eq "255" or $col[7] eq "15") {
		# exit == 255 or signal == 15: ssh failed
		# Remove sshlogin
		::debug("--filtered $host\n");
		push(@down_hosts, $host);
		@down_hosts = uniq(@down_hosts);
	    } elsif($col[6] eq "127") {
		# signal == 127: parallel not installed remote
		# Set ncpus and ncores = 1
		::warning("Could not figure out ",
			  "number of cpus on $host. Using 1.\n");
		$ncores{$host} = 1;
		$ncpus{$host} = 1;
		$maxlen{$host} = Limits::Command::max_length();
	    } elsif($col[0] =~ /^\d+$/ and $Global::host{$host}) {
		# Remember how log it took to log in
		# 2 : 1372607672.654 0.675 0 0 0 0 eval true\ m\;ssh\ m\ echo
		$time_to_login{$host} = ::min($time_to_login{$host},$col[3]);
	    } else {
		::die_bug("host check unmatched long jobline: $_");
	    }
	} elsif($Global::host{$col[0]}) {
	    # This output from --number-of-cores, --number-of-cpus,
	    # --max-line-length-allowed
	    # ncores: server       8
	    # ncpus:  server       2
	    # maxlen: server       131071
	    if(not $ncores{$col[0]}) {
		$ncores{$col[0]} = $col[1];
	    } elsif(not $ncpus{$col[0]}) {
		$ncpus{$col[0]} = $col[1];
	    } elsif(not $maxlen{$col[0]}) {
		$maxlen{$col[0]} = $col[1];
	    } elsif(not $echo{$col[0]}) {
		$echo{$col[0]} = $col[1];
	    } elsif(m/perl: warning:|LANGUAGE =|LC_ALL =|LANG =|are supported and installed/) {
		# Skip these:
		# perl: warning: Setting locale failed.
		# perl: warning: Please check that your locale settings:
		#         LANGUAGE = (unset),
		#         LC_ALL = (unset),
		#         LANG = "en_US.UTF-8"
		#     are supported and installed on your system.
		# perl: warning: Falling back to the standard locale ("C").
	    } else {
		::die_bug("host check too many col0: $_");
	    }
	} else {
	    ::die_bug("host check unmatched short jobline ($col[0]): $_");
	}
    }
    close $host_fh;
    unlink $tmpfile;
    delete @Global::host{@down_hosts};
    @down_hosts and ::warning("Removed @down_hosts\n");
    while (my ($sshlogin, $obj) = each %Global::host) {
	$ncpus{$sshlogin} or ::die_bug("ncpus missing: ".$obj->serverlogin());
	$ncores{$sshlogin} or ::die_bug("ncores missing: ".$obj->serverlogin());
	$time_to_login{$sshlogin} or ::die_bug("time_to_login missing: ".$obj->serverlogin());
	$maxlen{$sshlogin} or ::die_bug("maxlen missing: ".$obj->serverlogin());
	if($opt::use_cpus_instead_of_cores) {
	    $obj->set_ncpus($ncpus{$sshlogin});
	} else {
	    $obj->set_ncpus($ncores{$sshlogin});
	}
	$obj->set_time_to_login($time_to_login{$sshlogin});
        $obj->set_maxlength($maxlen{$sshlogin});
	::debug("Timing from -S:$sshlogin ncpus:$ncpus{$sshlogin} ncores:$ncores{$sshlogin} ",
		"time_to_login:$time_to_login{$sshlogin} maxlen:$maxlen{$sshlogin}\n");
    }
}

sub onall {
    sub tmp_joblog {
	my $joblog = shift;
	if(not defined $joblog) {
	    return undef;
	}
	my ($fh, $tmpfile) = ::tempfile(SUFFIX => ".log");
	close $fh;
	return $tmpfile;
    }
    # Copy all @fhlist into tempfiles
    my @argfiles = ();
    for my $fh (@fhlist) {
	my ($outfh, $name) = ::tempfile(SUFFIX => ".all", UNLINK => 1);
	print $outfh (<$fh>);
	close $outfh;
	push @argfiles, $name;
    }
    if(@opt::basefile) { setup_basefile(); }
    # for each sshlogin do:
    # parallel -S $sshlogin $command :::: @argfiles
    #
    # Pass some of the options to the sub-parallels, not all of them as
    # -P should only go to the first, and -S should not be copied at all.
    my $options =
	join(" ",
	     ((defined $opt::P) ? "-P $opt::P" : ""),
	     ((defined $opt::u) ? "-u" : ""),
	     ((defined $opt::group) ? "-g" : ""),
	     ((defined $opt::keeporder) ? "--keeporder" : ""),
	     ((defined $opt::D) ? "-D" : ""),
	     ((defined $opt::plain) ? "--plain" : ""),
	     ((defined $opt::max_chars) ? "--max-chars ".$opt::max_chars : ""),
	);
    my $suboptions =
	join(" ",
	     ((defined $opt::u) ? "-u" : ""),
	     ((defined $opt::group) ? "-g" : ""),
	     ((defined $opt::files) ? "--files" : ""),
	     ((defined $opt::keeporder) ? "--keeporder" : ""),
	     ((defined $opt::colsep) ? "--colsep ".shell_quote($opt::colsep) : ""),
	     ((@opt::v) ? "-vv" : ""),
	     ((defined $opt::D) ? "-D" : ""),
	     ((defined $opt::timeout) ? "--timeout ".$opt::timeout : ""),
	     ((defined $opt::plain) ? "--plain" : ""),
	     ((defined $opt::retries) ? "--retries ".$opt::retries : ""),
	     ((defined $opt::max_chars) ? "--max-chars ".$opt::max_chars : ""),
	     (@opt::env ? map { "--env ".::shell_quote_scalar($_) } @opt::env : ""),
	);
    ::debug("| $0 $options\n");
    open(my $parallel_fh, "|-", "$0 --no-notice -j0 $options") ||
	::die_bug("This does not run GNU Parallel: $0 $options");
    my @joblogs;
    for my $host (sort keys %Global::host) {
	my $sshlogin = $Global::host{$host};
	my $joblog = tmp_joblog($opt::joblog);
	if($joblog) {
	    push @joblogs, $joblog;
	    $joblog = "--joblog $joblog";
	}
	print $parallel_fh "$0 $suboptions -j1 $joblog ".
	    ((defined $opt::tag) ?
	     "--tagstring ".shell_quote_scalar($sshlogin->string()) : "").
	     " -S ". shell_quote_scalar($sshlogin->string())." ".
	     shell_quote_scalar($command)." :::: @argfiles\n";
    }
    close $parallel_fh;
    $Global::exitstatus = $? >> 8;
    debug("--onall exitvalue ",$?);
    if(@opt::basefile) { cleanup_basefile(); }
    unlink(@argfiles);
    my %seen;
    for my $joblog (@joblogs) {
	# Append to $joblog
	open(my $fh, "<", $joblog) || ::die_bug("Cannot open tmp joblog $joblog");
	# Skip first line (header);
	<$fh>;
	print $Global::joblog (<$fh>);
	close $fh;
	unlink($joblog);
    }
}

sub __SIGNAL_HANDLING__ {}

sub save_original_signal_handler {
    # Remember the original signal handler
    # Returns: N/A
    $SIG{TERM} ||= sub { exit 0; }; # $SIG{TERM} is not set on Mac OS X
    %Global::original_sig = %SIG;
    $SIG{TERM} = sub {}; # Dummy until jobs really start
}

sub list_running_jobs {
    # Returns: N/A
    for my $v (values %Global::running) {
        print $Global::original_stderr "$Global::progname: ",$v->replaced(),"\n";
    }
}

sub start_no_new_jobs {
    # Returns: N/A
    $SIG{TERM} = $Global::original_sig{TERM};
    print $Global::original_stderr
        ("$Global::progname: SIGTERM received. No new jobs will be started.\n",
         "$Global::progname: Waiting for these ", scalar(keys %Global::running),
         " jobs to finish. Send SIGTERM again to stop now.\n");
    list_running_jobs();
    $Global::start_no_new_jobs ||= 1;
}

sub reaper {
    # A job finished.
    # Print the output.
    # Start another job
    # Returns: N/A
    my $stiff;
    my $children_reaped = 0;
    debug("Reaper ");
    while (($stiff = waitpid(-1, &WNOHANG)) > 0) {
	$children_reaped++;
        if($Global::sshmaster{$stiff}) {
            # This is one of the ssh -M: ignore
            next;
        }
        my $job = $Global::running{$stiff};
	# '-a <(seq 10)' will give us a pid not in %Global::running
        $job or next;
        $job->set_exitstatus($? >> 8);
        $job->set_exitsignal($? & 127);
        debug("died (".$job->exitstatus()."): ".$job->seq());
        $job->set_endtime(::now());
        if($stiff == $Global::tty_taken) {
            # The process that died had the tty => release it
            $Global::tty_taken = 0;
        }

        if(not $job->should_be_retried()) {
	    if($opt::timeout) {
		# Update average runtime for timeout
		$Global::timeoutq->update_delta_time($job->runtime());
	    }
            # Force printing now if the job failed and we are going to exit
            my $print_now = ($job->exitstatus() and
                             $opt::halt_on_error and $opt::halt_on_error == 2);
            if($Global::keeporder and not $print_now) {
                $Private::print_later{$job->seq()} = $job;
                $Private::job_end_sequence ||= 1;
                debug("Looking for: $Private::job_end_sequence ".
                      "Current: ".$job->seq()."\n");
                while($Private::print_later{$Private::job_end_sequence}
		      or
		      vec($Global::job_already_run,$Private::job_end_sequence,1)) {
                    debug("Found job end $Private::job_end_sequence");
		    defined $Private::print_later{$Private::job_end_sequence} and 
                    $Private::print_later{$Private::job_end_sequence}->print();
                    delete $Private::print_later{$Private::job_end_sequence};
                    $Private::job_end_sequence++;
                }
            } else {
                $job->print();
            }
            if($job->exitstatus()) {
                # The jobs had a exit status <> 0, so error
                $Global::exitstatus++;
                if($opt::halt_on_error) {
                    if($opt::halt_on_error == 1) {
                        # If halt on error == 1 we should gracefully exit
                        print $Global::original_stderr
                            ("$Global::progname: Starting no more jobs. ",
                             "Waiting for ", scalar(keys %Global::running),
                             " jobs to finish. This job failed:\n",
                             $job->replaced(),"\n");
                        $Global::start_no_new_jobs ||= 1;
                        $Global::halt_on_error_exitstatus = $job->exitstatus();
                    } elsif($opt::halt_on_error == 2) {
                        # If halt on error == 2 we should exit immediately
                        print $Global::original_stderr
                            ("$Global::progname: This job failed:\n",
                             $job->replaced(),"\n");
                        exit ($job->exitstatus());
                    }
                }
            }
        }
        my $sshlogin = $job->sshlogin();
        $sshlogin->dec_jobs_running();
        $sshlogin->inc_jobs_completed();
        $Global::total_running--;
        delete $Global::running{$stiff};
        start_more_jobs();
    }
    debug("done ");
    return $children_reaped;
}

sub __USAGE__ {}

sub wait_and_exit {
    # If we do not wait, we sometimes get segfault
    # Returns: N/A
    my $error = shift;
    if($error) {
	# Kill all without printing
	for my $job (values %Global::running) {
	    $job->kill("TERM");
	    $job->kill("TERM");
	}
    }
    for (keys %Global::unkilled_children) {
        kill 9, $_;
        waitpid($_,0);
        delete $Global::unkilled_children{$_};
    }
    wait();
    exit($error);
}

sub die_usage {
    # Returns: N/A
    usage();
    wait_and_exit(255);
}

sub usage {
    # Returns: N/A
    print join
	("\n",
	 "Usage:",
	 "$Global::progname [options] [command [arguments]] < list_of_arguments",
	 "$Global::progname [options] [command [arguments]] (::: arguments|:::: argfile(s))...",
	 "cat ... | $Global::progname --pipe [options] [command [arguments]]",
	 "",
	 "-j n           Run n jobs in parallel",
	 "-k             Keep same order",
	 "-X             Multiple arguments with context replace",
	 "--colsep regexp      Split input on regexp for positional replacements",
	 "{} {.} {/} {/.} {#}  Replacement strings",
	 "{3} {3.} {3/} {3/.}  Positional replacement strings",
	 "",
	 "-S sshlogin    Example: foo\@server.example.com",
	 "--slf ..       Use ~/.parallel/sshloginfile as the list of sshlogins",
	 "--trc {}.bar   Shorthand for --transfer --return {}.bar --cleanup",
	 "--onall        Run the given command with argument on all sshlogins",
	 "--nonall       Run the given command with no arguments on all sshlogins",
	 "",
	 "--pipe         Split stdin (standard input) to multiple jobs.",
	 "--recend str   Record end separator for --pipe.",
	 "--recstart str Record start separator for --pipe.",
	 "",
	 "See 'man $Global::progname' for details",
	 "",
	 "When using GNU Parallel for a publication please cite:",
	 "",
	 "O. Tange (2011): GNU Parallel - The Command-Line Power Tool,",
	 ";login: The USENIX Magazine, February 2011:42-47.",
	 "");
}


sub citation_notice {
    # if --no-notice or --plain: do nothing
    # if stderr redirected: do nothing
    # if ~/.parallel/will-cite: do nothing
    # else: print citation notice to stderr
    if($opt::no_notice
       or
       $opt::plain
       or
       not -t $Global::original_stderr
       or
       -e $ENV{'HOME'}."/.parallel/will-cite") {
	# skip
    } else {
	print $Global::original_stderr 
	    ("When using programs that use GNU Parallel to process data for publication please cite:\n",
	     "\n",
	     "  O. Tange (2011): GNU Parallel - The Command-Line Power Tool,\n",
	     "  ;login: The USENIX Magazine, February 2011:42-47.\n",
	     "\n",
	     "This helps funding further development; and it won't cost you a cent.\n",
	     "\n",
	     "To silence this citation notice run 'parallel --bibtex' once or use '--no-notice'.\n\n",
	    );
	flush $Global::original_stderr;
    }
}


sub warning {
    my @w = @_;
    my $fh = $Global::original_stderr || *STDERR;
    my $prog = $Global::progname || "parallel";
    print $fh $prog, ": Warning: ", @w;
}


sub error {
    my @w = @_;
    my $fh = $Global::original_stderr || *STDERR;
    my $prog = $Global::progname || "parallel";
    print $fh $prog, ": Error: ", @w;
}


sub die_bug {
    my $bugid = shift;
    print STDERR
	("$Global::progname: This should not happen. You have found a bug.\n",
	 "Please contact <parallel\@gnu.org> and include:\n",
	 "* The version number: $Global::version\n",
	 "* The bugid: $bugid\n",
	 "* The command line being run\n",
	 "* The files being read (put the files on a webserver if they are big)\n",
	 "\n",
	 "If you get the error on smaller/fewer files, please include those instead.\n");
    ::wait_and_exit(255);
}

sub version {
    # Returns: N/A
    if($opt::tollef and not $opt::gnu) {
	print "WARNING: YOU ARE USING --tollef. IF THINGS ARE ACTING WEIRD USE --gnu.\n";
    }
    print join("\n",
               "GNU $Global::progname $Global::version",
               "Copyright (C) 2007,2008,2009,2010,2011,2012,2013 Ole Tange and Free Software Foundation, Inc.",
               "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>",
               "This is free software: you are free to change and redistribute it.",
               "GNU $Global::progname comes with no warranty.",
               "",
               "Web site: http://www.gnu.org/software/${Global::progname}\n",
	       "When using GNU Parallel for a publication please cite:\n",
	       "O. Tange (2011): GNU Parallel - The Command-Line Power Tool, ",
	       ";login: The USENIX Magazine, February 2011:42-47.\n",
        );
}

sub bibtex {
    # Returns: N/A
    if($opt::tollef and not $opt::gnu) {
	print "WARNING: YOU ARE USING --tollef. IF THINGS ARE ACTING WEIRD USE --gnu.\n";
    }
    print join("\n",
	       "When using programs that use GNU Parallel to process data for publication please cite:",
	       "",
               "\@article{Tange2011a,",
	       " title = {GNU Parallel - The Command-Line Power Tool},",
	       " author = {O. Tange},",
	       " address = {Frederiksberg, Denmark},",
	       " journal = {;login: The USENIX Magazine},",
	       " month = {Feb},",
	       " number = {1},",
	       " volume = {36},",
	       " url = {http://www.gnu.org/s/parallel},",
	       " year = {2011},",
	       " pages = {42-47}",
	       "}",
	       "",
	       "(Feel free to use \\nocite{Tange2011a})",
	       "",
	       "This helps funding further development.",
	       ""
        );
    while(not -e $ENV{'HOME'}."/.parallel/will-cite") {
	print "\nType: 'will cite' and press enter.\n> ";
	my $input = <STDIN>;
	if($input =~ /will cite/i) {
	    mkdir $ENV{'HOME'}."/.parallel";
	    open (my $fh, ">", $ENV{'HOME'}."/.parallel/will-cite") 
		|| ::die_bug("Cannot write: ".$ENV{'HOME'}."/.parallel/will-cite");
	    close $fh;
	    print "\nThank you for your support. It is much appreciated. The citation\n",
	    "notice is now silenced.\n";
	}
    }
}

sub show_limits {
    # Returns: N/A
    print("Maximal size of command: ",Limits::Command::real_max_length(),"\n",
          "Maximal used size of command: ",Limits::Command::max_length(),"\n",
          "\n",
          "Execution of  will continue now, and it will try to read its input\n",
          "and run commands; if this is not what you wanted to happen, please\n",
          "press CTRL-D or CTRL-C\n");
}

sub __GENERIC_COMMON_FUNCTION__ {}

sub uniq {
    # Remove duplicates and return unique values
    return keys %{{ map { $_ => 1 } @_ }};
}

sub min {
    # Returns:
    #   Minimum value of array
    my $min;
    for (@_) {
        # Skip undefs
        defined $_ or next;
        defined $min or do { $min = $_; next; }; # Set $_ to the first non-undef
        $min = ($min < $_) ? $min : $_;
    }
    return $min;
}

sub max {
    # Returns:
    #   Maximum value of array
    my $max;
    for (@_) {
        # Skip undefs
        defined $_ or next;
        defined $max or do { $max = $_; next; }; # Set $_ to the first non-undef
        $max = ($max > $_) ? $max : $_;
    }
    return $max;
}

sub sum {
    # Returns:
    #   Sum of values of array
    my @args = @_;
    my $sum = 0;
    for (@args) {
        # Skip undefs
        $_ and do { $sum += $_; }
    }
    return $sum;
}

sub undef_as_zero {
    my $a = shift;
    return $a ? $a : 0;
}

sub undef_as_empty {
    my $a = shift;
    return $a ? $a : "";
}

sub hostname {
    if(not $Private::hostname) {
        my $hostname = `hostname`;
        chomp($hostname);
        $Private::hostname = $hostname || "nohostname";
    }
    return $Private::hostname;
}

sub reap_usleep {
    # Reap dead children.
    # If no dead children: Sleep specified amount with exponential backoff
    # Returns:
    #   $ms/2+0.001 if children reaped
    #   $ms*1.1 if no children reaped
    my $ms = shift;
    if(reaper()) {
	# Sleep exponentially shorter (1/2^n) if a job finished
	return $ms/2+0.001;
    } else {
	usleep($ms);
	Job::exit_if_disk_full();
	if($opt::linebuffer) {
	    for my $job (values %Global::running) {
		$job->print();
	    }
	}
	# Sleep exponentially longer (1.1^n) if a job did not finish
	# though at most 1000 ms.
	return (($ms < 1000) ? ($ms * 1.1) : ($ms));
    }
}

sub usleep {
    # Sleep this many milliseconds.
    my $secs = shift;
    ::debug(int($secs),"ms ");
    select(undef, undef, undef, $secs/1000);
    if($opt::timeout) {
	$Global::timeoutq->process_timeouts();
    }
}

sub now {
    # Returns time since epoch as in seconds with 3 decimals

    if(not $Global::use{"Time::HiRes"}) {
	if(eval "use Time::HiRes qw ( time );") {
	    eval "sub TimeHiRestime { return Time::HiRes::time };";
	} else {
	    eval "sub TimeHiRestime { return time() };";
	}
	$Global::use{"Time::HiRes"} = 1;
    }

    return (int(TimeHiRestime()*1000))/1000;
}

sub multiply_binary_prefix {
    # Evalualte numbers with binary prefix
    # Ki=2^10, Mi=2^20, Gi=2^30, Ti=2^40, Pi=2^50, Ei=2^70, Zi=2^80, Yi=2^80
    # ki=2^10, mi=2^20, gi=2^30, ti=2^40, pi=2^50, ei=2^70, zi=2^80, yi=2^80
    # K =2^10, M =2^20, G =2^30, T =2^40, P =2^50, E =2^70, Z =2^80, Y =2^80
    # k =10^3, m =10^6, g =10^9, t=10^12, p=10^15, e=10^18, z=10^21, y=10^24
    # 13G = 13*1024*1024*1024 = 13958643712
    my $s = shift;
    $s =~ s/ki/*1024/gi;
    $s =~ s/mi/*1024*1024/gi;
    $s =~ s/gi/*1024*1024*1024/gi;
    $s =~ s/ti/*1024*1024*1024*1024/gi;
    $s =~ s/pi/*1024*1024*1024*1024*1024/gi;
    $s =~ s/ei/*1024*1024*1024*1024*1024*1024/gi;
    $s =~ s/zi/*1024*1024*1024*1024*1024*1024*1024/gi;
    $s =~ s/yi/*1024*1024*1024*1024*1024*1024*1024*1024/gi;
    $s =~ s/xi/*1024*1024*1024*1024*1024*1024*1024*1024*1024/gi;

    $s =~ s/K/*1024/g;
    $s =~ s/M/*1024*1024/g;
    $s =~ s/G/*1024*1024*1024/g;
    $s =~ s/T/*1024*1024*1024*1024/g;
    $s =~ s/P/*1024*1024*1024*1024*1024/g;
    $s =~ s/E/*1024*1024*1024*1024*1024*1024/g;
    $s =~ s/Z/*1024*1024*1024*1024*1024*1024*1024/g;
    $s =~ s/Y/*1024*1024*1024*1024*1024*1024*1024*1024/g;
    $s =~ s/X/*1024*1024*1024*1024*1024*1024*1024*1024*1024/g;

    $s =~ s/k/*1000/g;
    $s =~ s/m/*1000*1000/g;
    $s =~ s/g/*1000*1000*1000/g;
    $s =~ s/t/*1000*1000*1000*1000/g;
    $s =~ s/p/*1000*1000*1000*1000*1000/g;
    $s =~ s/e/*1000*1000*1000*1000*1000*1000/g;
    $s =~ s/z/*1000*1000*1000*1000*1000*1000*1000/g;
    $s =~ s/y/*1000*1000*1000*1000*1000*1000*1000*1000/g;
    $s =~ s/x/*1000*1000*1000*1000*1000*1000*1000*1000*1000/g;

    $s = eval $s;
    ::debug($s);
    return $s;
}

sub __DEBUGGING__ {}

sub debug {
    # Returns: N/A
    $Global::debug or return;
    @_ = grep { defined $_ ? $_ : "" } @_;
    if($Global::fd{1}) {
	# Original stdout was saved
	my $stdout = $Global::fd{1};
        print $stdout @_;
    } else {
        print @_;
    }
}

sub my_memory_usage {
    # Returns:
    #   memory usage if found
    #   0 otherwise
    use strict;
    use FileHandle;

    my $pid = $$;
    if(-e "/proc/$pid/stat") {
        my $fh = FileHandle->new("</proc/$pid/stat");

        my $data = <$fh>;
        chomp $data;
        $fh->close;

        my @procinfo = split(/\s+/,$data);

        return undef_as_zero($procinfo[22]);
    } else {
        return 0;
    }
}

sub my_size {
    # Returns:
    #   size of object if Devel::Size is installed
    #   -1 otherwise
    my @size_this = (@_);
    eval "use Devel::Size qw(size total_size)";
    if ($@) {
        return -1;
    } else {
        return total_size(@_);
    }
}

sub my_dump {
    # Returns:
    #   ascii expression of object if Data::Dump(er) is installed
    #   error code otherwise
    my @dump_this = (@_);
    eval "use Data::Dump qw(dump);";
    if ($@) {
        # Data::Dump not installed
        eval "use Data::Dumper;";
        if ($@) {
            my $err =  "Neither Data::Dump nor Data::Dumper is installed\n".
                "Not dumping output\n";
            print $Global::original_stderr $err;
            return $err;
        } else {
            return Dumper(@dump_this);
        }
    } else {
	# Create a dummy Data::Dump:dump as Hans Schou sometimes has
	# it undefined
	eval "sub Data::Dump:dump {}";
        eval "use Data::Dump qw(dump);";
        return (Data::Dump::dump(@dump_this));
    }
}

sub __OBJECT_ORIENTED_PARTS__ {}

package SSHLogin;

sub new {
    my $class = shift;
    my $sshlogin_string = shift;
    my $ncpus;
    if($sshlogin_string =~ s:^(\d+)/:: and $1) {
        # Override default autodetected ncpus unless zero or missing
        $ncpus = $1;
    }
    my $string = $sshlogin_string;
    my @unget = ();
    my $no_slash_string = $string;
    $no_slash_string =~ s/[^-a-z0-9:]/_/gi;
    return bless {
        'string' => $string,
        'jobs_running' => 0,
        'jobs_completed' => 0,
        'maxlength' => undef,
        'max_jobs_running' => undef,
        'ncpus' => $ncpus,
        'sshcommand' => undef,
        'serverlogin' => undef,
        'control_path_dir' => undef,
        'control_path' => undef,
	'time_to_login' => undef,
	'last_login_at' => undef,
        'loadavg_file' => $ENV{'HOME'} . "/.parallel/tmp/loadavg-" .
            $$."-".$no_slash_string,
        'loadavg' => undef,
	'last_loadavg_update' => 0,
        'swap_activity_file' => $ENV{'HOME'} . "/.parallel/tmp/swap_activity-" .
            $$."-".$no_slash_string,
        'swap_activity' => undef,
    }, ref($class) || $class;
}

sub DESTROY {
    my $self = shift;
    # Remove temporary files if they are created.
    unlink $self->{'loadavg_file'};
    unlink $self->{'swap_activity_file'};
}

sub string {
    my $self = shift;
    return $self->{'string'};
}

sub jobs_running {
    my $self = shift;

    return ($self->{'jobs_running'} || "0");
}

sub inc_jobs_running {
    my $self = shift;
    $self->{'jobs_running'}++;
}

sub dec_jobs_running {
    my $self = shift;
    $self->{'jobs_running'}--;
}

#sub set_jobs_running {
#    my $self = shift;
#    $self->{'jobs_running'} = shift;
#}

sub set_maxlength {
    my $self = shift;
    $self->{'maxlength'} = shift;
}

sub maxlength {
    my $self = shift;
    return $self->{'maxlength'};
}

sub jobs_completed {
    my $self = shift;
    return $self->{'jobs_completed'};
}

sub inc_jobs_completed {
    my $self = shift;
    $self->{'jobs_completed'}++;
}

sub set_max_jobs_running {
    my $self = shift;
    if(defined $self->{'max_jobs_running'}) {
        $Global::max_jobs_running -= $self->{'max_jobs_running'};
    }
    $self->{'max_jobs_running'} = shift;
    if(defined $self->{'max_jobs_running'}) {
        # max_jobs_running could be resat if -j is a changed file
        $Global::max_jobs_running += $self->{'max_jobs_running'};
    }
}

sub swapping {
    my $self = shift;
    my $swapping = $self->swap_activity();
    return (not defined $swapping or $swapping)
}

sub swap_activity {
    # If the currently known swap activity is too old:
    #   Recompute a new one in the background
    # Returns:
    #   last swap activity computed
    my $self = shift;
    # Should we update the swap_activity file?
    my $update_swap_activity_file = 0;
    if(-r $self->{'swap_activity_file'}) {
        open(my $swap_fh, "<", $self->{'swap_activity_file'}) || ::die_bug("swap_activity_file-r");
        my $swap_out = <$swap_fh>;
        close $swap_fh;
        if($swap_out =~ /^(\d+)$/) {
            $self->{'swap_activity'} = $1;
            ::debug("New swap_activity: ".$self->{'swap_activity'});
        }
        ::debug("Last update: ".$self->{'last_swap_activity_update'});
        if(time - $self->{'last_swap_activity_update'} > 10) {
            # last swap activity update was started 10 seconds ago
            ::debug("Older than 10 sec: ".$self->{'swap_activity_file'});
            $update_swap_activity_file = 1;
        }
    } else {
        ::debug("No swap_activity file: ".$self->{'swap_activity_file'});
        $self->{'swap_activity'} = undef;
        $update_swap_activity_file = 1;
    }
    if($update_swap_activity_file) {
        ::debug("Updating swap_activity file".$self->{'swap_activity_file'});
        $self->{'last_swap_activity_update'} = time;
        -e $ENV{'HOME'}."/.parallel" or mkdir $ENV{'HOME'}."/.parallel";
        -e $ENV{'HOME'}."/.parallel/tmp" or mkdir $ENV{'HOME'}."/.parallel/tmp";
        my $swap_activity;
	# If the (remote) machine is Mac we should use vm_stat
	# swap_in and swap_out on GNU/Linux is $7 and $8
	# swap_in and swap_out on Mac is $10 and $11
	$swap_activity = q[ { vmstat 1 2 2> /dev/null || vm_stat 1; } | ].
	    q[ awk 'NR!=4{next} NF==17||NF==16{print $7*$8} NF==11{print $10*$11} {exit}' ];
        if($self->{'string'} ne ":") {
            $swap_activity = $self->sshcommand() . " " . $self->serverlogin() . " " .
		::shell_quote_scalar($swap_activity);
        }
        # Run swap_activity measuring.
        # As the command can take long to run if run remote
        # save it to a tmp file before moving it to the correct file
        my $file = $self->{'swap_activity_file'};
        my ($dummy_fh, $tmpfile) = ::tempfile(SUFFIX => ".swp");
        qx{ ($swap_activity > $tmpfile; mv $tmpfile $file) & };
    }
    return $self->{'swap_activity'};
}

sub too_fast_remote_login {
    my $self = shift;
    if($self->{'last_login_at'} and $self->{'time_to_login'}) {
	# sshd normally allows 10 simultaneous logins
	# A login takes time_to_login
	# So time_to_login/5 should be safe
	# If now <= last_login + time_to_login/5: Then it is too soon.
	my $too_fast = (::now() <= $self->{'last_login_at'}
			+ $self->{'time_to_login'}/5);
	::debug("Too fast? $too_fast ");
	return $too_fast;
    } else {
	# No logins so far (or time_to_login not computed): it is not too fast
	return 0;
    }
}

sub last_login_at {
    my $self = shift;
    return $self->{'last_login_at'};
}

sub set_last_login_at {
    my $self = shift;
    $self->{'last_login_at'} = shift;
}

sub loadavg_too_high {
    my $self = shift;
    my $loadavg = $self->loadavg();
    return (not defined $loadavg or
            $loadavg > $self->max_loadavg());
}

sub loadavg {
    # If the currently know loadavg is too old:
    #   Recompute a new one in the background
    # Returns:
    #   $last_loadavg = last load average computed (undef if none)
    my $self = shift;
    # Should we update the loadavg file?
    my $update_loadavg_file = 0;
    if(-r $self->{'loadavg_file'}) {
        open(my $load_fh, "<", $self->{'loadavg_file'}) ||
	    ::die_bug("loadavg_file-r: ".$self->{'loadavg_file'});
	local $/ = undef;
        my $load_out = <$load_fh>;
        close $load_fh;
	my $load =()= ($load_out=~/(^[DR]....[^\[])/gm);
        if($load > 0) {
	    # load is overestimated by 1
            $self->{'loadavg'} = $load - 1;
            ::debug("New loadavg: ".$self->{'loadavg'});
        } else {
	    ::die_bug("loadavg_invalid_content: $load_out");
	}
        ::debug("Last update: ".$self->{'last_loadavg_update'});
        if(time - $self->{'last_loadavg_update'} > 10) {
            # last loadavg was started 10 seconds ago
            ::debug((time - $self->{'last_loadavg_update'}). " secs old: ".$self->{'loadavg_file'});
            $update_loadavg_file = 1;
        }
    } else {
        ::debug("No loadavg file: ".$self->{'loadavg_file'});
        $self->{'loadavg'} = undef;
        $update_loadavg_file = 1;
    }
    if($update_loadavg_file) {
        ::debug("Updating loadavg file".$self->{'loadavg_file'}."\n");
        $self->{'last_loadavg_update'} = time;
        -e $ENV{'HOME'}."/.parallel" or mkdir $ENV{'HOME'}."/.parallel";
        -e $ENV{'HOME'}."/.parallel/tmp" or mkdir $ENV{'HOME'}."/.parallel/tmp";
        my $cmd = "";
        if($self->{'string'} ne ":") {
	    $cmd = $self->sshcommand() . " " . $self->serverlogin() . " ";
	}
	$cmd .= "ps ax -o state,command";
        # As the command can take long to run if run remote
        # save it to a tmp file before moving it to the correct file
        my $file = $self->{'loadavg_file'};
        my ($dummy_fh, $tmpfile) = ::tempfile(SUFFIX => ".loa");
        qx{ ($cmd > $tmpfile && mv $tmpfile $file) & };
    }
    return $self->{'loadavg'};
}


sub max_loadavg {
    my $self = shift;
    # If --load is a file it might be changed
    if($Global::max_load_file) {
	my $mtime = (stat($Global::max_load_file))[9];
	if($mtime > $Global::max_load_file_last_mod) {
	    $Global::max_load_file_last_mod = $mtime;
	    for my $sshlogin (values %Global::host) {
		$sshlogin->set_max_loadavg(undef);
	    }
	}
    }
    if(not defined $self->{'max_loadavg'}) {
        $self->{'max_loadavg'} =
            $self->compute_max_loadavg($opt::load);
    }
    ::debug("max_loadavg: ".$self->string()." ".$self->{'max_loadavg'});
    return $self->{'max_loadavg'};
}

sub set_max_loadavg {
    my $self = shift;
    $self->{'max_loadavg'} = shift;
}

sub compute_max_loadavg {
    # Parse the max loadaverage that the user asked for using --load
    # Returns:
    #   max loadaverage
    my $self = shift;
    my $loadspec = shift;
    my $load;
    if(defined $loadspec) {
        if($loadspec =~ /^\+(\d+)$/) {
            # E.g. --load +2
            my $j = $1;
            $load =
                $self->ncpus() + $j;
        } elsif ($loadspec =~ /^-(\d+)$/) {
            # E.g. --load -2
            my $j = $1;
            $load =
                $self->ncpus() - $j;
        } elsif ($loadspec =~ /^(\d+)\%$/) {
            my $j = $1;
            $load =
                $self->ncpus() * $j / 100;
        } elsif ($loadspec =~ /^(\d+(\.\d+)?)$/) {
            $load = $1;
        } elsif (-f $loadspec) {
            $Global::max_load_file = $loadspec;
            $Global::max_load_file_last_mod = (stat($Global::max_load_file))[9];
            if(open(my $in_fh, "<", $Global::max_load_file)) {
                my $opt_load_file = join("",<$in_fh>);
                close $in_fh;
                $load = $self->compute_max_loadavg($opt_load_file);
            } else {
                print $Global::original_stderr "Cannot open $loadspec\n";
                ::wait_and_exit(255);
            }
        } else {
            print $Global::original_stderr "Parsing of --load failed\n";
            ::die_usage();
        }
        if($load < 0.01) {
            $load = 0.01;
        }
    }
    return $load;
}

sub time_to_login {
    my $self = shift;
    return $self->{'time_to_login'};
}

sub set_time_to_login {
    my $self = shift;
    $self->{'time_to_login'} = shift;
}

sub max_jobs_running {
    my $self = shift;
    if(not defined $self->{'max_jobs_running'}) {
        my $nproc = $self->compute_number_of_processes($opt::P);
        $self->set_max_jobs_running($nproc);
    }
    return $self->{'max_jobs_running'};
}

sub compute_number_of_processes {
    # Number of processes wanted and limited by system resources
    # Returns:
    #   Number of processes
    my $self = shift;
    my $opt_P = shift;
    my $wanted_processes = $self->user_requested_processes($opt_P);
    if(not defined $wanted_processes) {
        $wanted_processes = $Global::default_simultaneous_sshlogins;
    }
    ::debug("Wanted procs: $wanted_processes\n");
    my $system_limit =
        $self->processes_available_by_system_limit($wanted_processes);
    ::debug("Limited to procs: $system_limit\n");
    return $system_limit;
}

sub processes_available_by_system_limit {
    # If the wanted number of processes is bigger than the system limits:
    # Limit them to the system limits
    # Limits are: File handles, number of input lines, processes,
    # and taking > 1 second to spawn 10 extra processes
    # Returns:
    #   Number of processes
    my $self = shift;
    my $wanted_processes = shift;

    my $system_limit = 0;
    my @jobs = ();
    my $job;
    my @args = ();
    my $arg;
    my $more_filehandles = 1;
    my $max_system_proc_reached = 0;
    my $slow_spawining_warning_printed = 0;
    my $time = time;
    my %fh;
    my @children;

    # Reserve filehandles
    # perl uses 7 filehandles for something?
    # parallel uses 1 for memory_usage
    for my $i (1..8) {
        open($fh{"init-$i"}, "<", "/dev/null");
    }

    for(1..2) {
        # System process limit
        my $child;
        if($child = fork()) {
            push (@children,$child);
            $Global::unkilled_children{$child} = 1;
        } elsif(defined $child) {
            # The child takes one process slot
            # It will be killed later
            $SIG{TERM} = $Global::original_sig{TERM};
            sleep 10000000;
            exit(0);
        } else {
            $max_system_proc_reached = 1;
        }
    }
    my $count_jobs_already_read = $Global::JobQueue->next_seq();
    my $wait_time_for_getting_args = 0;
    my $start_time = time;
    while(1) {
        $system_limit >= $wanted_processes and last;
        not $more_filehandles and last;
        $max_system_proc_reached and last;
	my $before_getting_arg = time;
        if($Global::semaphore or $opt::pipe) {
	    # Skip: No need to get args
        } elsif(defined $opt::retries and $count_jobs_already_read) {
            # For retries we may need to run all jobs on this sshlogin
            # so include the already read jobs for this sshlogin
            $count_jobs_already_read--;
        } else {
            if($opt::X or $opt::m) {
                # The arguments may have to be re-spread over several jobslots
                # So pessimistically only read one arg per jobslot
                # instead of a full commandline
                if($Global::JobQueue->{'commandlinequeue'}->{'arg_queue'}->empty()) {
		    if($Global::JobQueue->empty()) {
			last;
		    } else {
			($job) = $Global::JobQueue->get();
			push(@jobs, $job);
		    }
		} else {
		    ($arg) = $Global::JobQueue->{'commandlinequeue'}->{'arg_queue'}->get();
		    push(@args, $arg);
		}
            } else {
                # If there are no more command lines, then we have a process
                # per command line, so no need to go further
                $Global::JobQueue->empty() and last;
                ($job) = $Global::JobQueue->get();
                push(@jobs, $job);
	    }
        }
	$wait_time_for_getting_args += time - $before_getting_arg;
        $system_limit++;

        # Every simultaneous process uses 2 filehandles when grouping
        $more_filehandles = open($fh{$system_limit*10}, "<", "/dev/null")
            && open($fh{$system_limit*10+2}, "<", "/dev/null");

        # System process limit
        my $child;
        if($child = fork()) {
            push (@children,$child);
            $Global::unkilled_children{$child} = 1;
        } elsif(defined $child) {
            # The child takes one process slot
            # It will be killed later
            $SIG{TERM} = $Global::original_sig{TERM};
            sleep 10000000;
            exit(0);
        } else {
            $max_system_proc_reached = 1;
        }
	my $forktime = time - $time - $wait_time_for_getting_args;
        ::debug("Time to fork $system_limit procs: $wait_time_for_getting_args ",
		$forktime,
		" (processes so far: ", $system_limit,")\n");
        if($system_limit > 10 and
	   $forktime > 1 and
	   $forktime > $system_limit * 0.01
	   and not $slow_spawining_warning_printed) {
            # It took more than 0.01 second to fork a processes on avg.
            # Give the user a warning. He can press Ctrl-C if this
            # sucks.
            print $Global::original_stderr
                ("parallel: Warning: Starting $system_limit processes took > $forktime sec.\n",
                 "Consider adjusting -j. Press CTRL-C to stop.\n");
            $slow_spawining_warning_printed = 1;
        }
    }
    # Cleanup: Close the files
    for (values %fh) { close $_ }
    # Cleanup: Kill the children
    for my $pid (@children) {
        kill 9, $pid;
        waitpid($pid,0);
        delete $Global::unkilled_children{$pid};
    }
    # Cleanup: Unget the command_lines or the @args
    $Global::JobQueue->{'commandlinequeue'}->{'arg_queue'}->unget(@args);
    $Global::JobQueue->unget(@jobs);
    if($system_limit < $wanted_processes) {
	# The system_limit is less than the wanted_processes
	if($system_limit < 1 and not $Global::JobQueue->empty()) {
	    ::warning("Cannot spawn any jobs. Raising ulimit -u or /etc/security/limits.conf may help.\n");
	    ::wait_and_exit(255);
	}
	if(not $more_filehandles) {
	    ::warning("Only enough file handles to run ", $system_limit, " jobs in parallel.\n",
		      "Raising ulimit -n or /etc/security/limits.conf may help.\n");
	}
	if($max_system_proc_reached) {
	    ::warning("Only enough available processes to run ", $system_limit,
		      " jobs in parallel. Raising ulimit -u or /etc/security/limits.conf may help.\n");
	}
    }
    if($] == 5.008008 and $system_limit > 1000) {
	# https://savannah.gnu.org/bugs/?36942
	$system_limit = 1000;
    }
    if($Global::JobQueue->empty()) {
	$system_limit ||= 1;
    }
    if($self->string() ne ":" and
       $system_limit > $Global::default_simultaneous_sshlogins) {
        $system_limit =
            $self->simultaneous_sshlogin_limit($system_limit);
    }
    return $system_limit;
}

sub simultaneous_sshlogin_limit {
    # Test by logging in wanted number of times simultaneously
    # Returns:
    #   min($wanted_processes,$working_simultaneous_ssh_logins-1)
    my $self = shift;
    my $wanted_processes = shift;
    if($self->{'time_to_login'}) {
	return $wanted_processes;
    }

    # Try twice because it guesses wrong sometimes
    # Choose the minimal
    my $ssh_limit =
        ::min($self->simultaneous_sshlogin($wanted_processes),
	      $self->simultaneous_sshlogin($wanted_processes));
    if($ssh_limit < $wanted_processes) {
        my $serverlogin = $self->serverlogin();
        ::warning("ssh to $serverlogin only allows ",
		  "for $ssh_limit simultaneous logins.\n",
		  "You may raise this by changing ",
		  "/etc/ssh/sshd_config:MaxStartup on $serverlogin.\n",
		  "Using only ",$ssh_limit-1," connections ",
		  "to avoid race conditions.\n");
    }
    # Race condition can cause problem if using all sshs.
    if($ssh_limit > 1) { $ssh_limit -= 1; }
    return $ssh_limit;
}

sub simultaneous_sshlogin {
    # Using $sshlogin try to see if we can do $wanted_processes
    # simultaneous logins
    # (ssh host echo simultaneouslogin & ssh host echo simultaneouslogin & ...)|grep simul|wc -l
    # Returns:
    #   Number of succesful logins
    my $self = shift;
    my $wanted_processes = shift;
    my $sshcmd = $self->sshcommand();
    my $serverlogin = $self->serverlogin();
    my $sshdelay = $opt::sshdelay ? "sleep $opt::sshdelay;" : "";
    my $cmd = "$sshdelay$sshcmd $serverlogin echo simultaneouslogin </dev/null 2>&1 &"x$wanted_processes;
    ::debug("Trying $wanted_processes logins at $serverlogin\n");
    open (my $simul_fh, "-|", "($cmd)|grep simultaneouslogin | wc -l") or
	::die_bug("simultaneouslogin");
    my $ssh_limit = <$simul_fh>;
    close $simul_fh;
    chomp $ssh_limit;
    return $ssh_limit;
}

sub set_ncpus {
    my $self = shift;
    $self->{'ncpus'} = shift;
}

sub user_requested_processes {
    # Parse the number of processes that the user asked for using -j
    # Returns:
    #   the number of processes to run on this sshlogin
    my $self = shift;
    my $opt_P = shift;
    my $processes;
    if(defined $opt_P) {
        if($opt_P =~ /^\+(\d+)$/) {
            # E.g. -P +2
            my $j = $1;
            $processes =
                $self->ncpus() + $j;
        } elsif ($opt_P =~ /^-(\d+)$/) {
            # E.g. -P -2
            my $j = $1;
            $processes =
                $self->ncpus() - $j;
        } elsif ($opt_P =~ /^(\d+)\%$/) {
            my $j = $1;
            $processes =
                $self->ncpus() * $j / 100;
        } elsif ($opt_P =~ /^(\d+)$/) {
            $processes = $1;
            if($processes == 0) {
                # -P 0 = infinity (or at least close)
                $processes = $Global::infinity;
            }
        } elsif (-f $opt_P) {
            $Global::max_procs_file = $opt_P;
            $Global::max_procs_file_last_mod = (stat($Global::max_procs_file))[9];
            if(open(my $in_fh, "<", $Global::max_procs_file)) {
                my $opt_P_file = join("",<$in_fh>);
                close $in_fh;
                $processes = $self->user_requested_processes($opt_P_file);
            } else {
                ::error("Cannot open $opt_P.\n");
                ::wait_and_exit(255);
            }
        } else {
            ::error("Parsing of --jobs/-j/--max-procs/-P failed.\n");
            ::die_usage();
        }
        if($processes < 1) {
            $processes = 1;
        }
    }
    return $processes;
}

sub ncpus {
    my $self = shift;
    if(not defined $self->{'ncpus'}) {
        my $sshcmd = $self->sshcommand();
        my $serverlogin = $self->serverlogin();
        if($serverlogin eq ":") {
            if($opt::use_cpus_instead_of_cores) {
                $self->{'ncpus'} = no_of_cpus();
            } else {
                $self->{'ncpus'} = no_of_cores();
            }
        } else {
            my $ncpu;
            if($opt::use_cpus_instead_of_cores) {
                $ncpu = qx(echo|$sshcmd $serverlogin parallel --number-of-cpus);
            } else {
                $ncpu = qx(echo|$sshcmd $serverlogin parallel --number-of-cores);
            }
	    chomp $ncpu;
            if($ncpu =~ /^\s*[0-9]+\s*$/s) {
                $self->{'ncpus'} = $ncpu;
            } else {
                ::warning("Could not figure out ",
			  "number of cpus on $serverlogin ($ncpu). Using 1.\n");
                $self->{'ncpus'} = 1;
            }
        }
    }
    return $self->{'ncpus'};
}

sub no_of_cpus {
    # Returns:
    #   Number of physical CPUs
    local $/="\n"; # If delimiter is set, then $/ will be wrong
    my $no_of_cpus;
    if ($^O eq 'linux') {
        $no_of_cpus = no_of_cpus_gnu_linux() || no_of_cores_gnu_linux();
    } elsif ($^O eq 'freebsd') {
        $no_of_cpus = no_of_cpus_freebsd();
    } elsif ($^O eq 'netbsd') {
        $no_of_cpus = no_of_cpus_netbsd();
    } elsif ($^O eq 'openbsd') {
        $no_of_cpus = no_of_cpus_openbsd();
    } elsif ($^O eq 'gnu') {
        $no_of_cpus = no_of_cpus_hurd();
    } elsif ($^O eq 'darwin') {
	$no_of_cpus = no_of_cpus_darwin();
    } elsif ($^O eq 'solaris') {
        $no_of_cpus = no_of_cpus_solaris();
    } elsif ($^O eq 'aix') {
        $no_of_cpus = no_of_cpus_aix();
    } elsif ($^O eq 'hpux') {
        $no_of_cpus = no_of_cpus_hpux();
    } elsif ($^O eq 'nto') {
        $no_of_cpus = no_of_cpus_qnx();
    } elsif ($^O eq 'svr5') {
        $no_of_cpus = no_of_cpus_openserver();
    } elsif ($^O eq 'irix') {
        $no_of_cpus = no_of_cpus_irix();
    } elsif ($^O eq 'dec_osf') {
        $no_of_cpus = no_of_cpus_tru64();
    } else {
	$no_of_cpus = (no_of_cpus_gnu_linux()
		       || no_of_cpus_freebsd()
		       || no_of_cpus_netbsd()
		       || no_of_cpus_openbsd()
		       || no_of_cpus_hurd()
		       || no_of_cpus_darwin()
		       || no_of_cpus_solaris()
		       || no_of_cpus_aix()
		       || no_of_cpus_hpux()
		       || no_of_cpus_qnx()
		       || no_of_cpus_openserver()
		       || no_of_cpus_irix()
		       || no_of_cpus_tru64()
	    );
    }
    if($no_of_cpus) {
	chomp $no_of_cpus;
        return $no_of_cpus;
    } else {
        ::warning("Cannot figure out number of cpus. Using 1.\n");
        return 1;
    }
}

sub no_of_cores {
    # Returns:
    #   Number of CPU cores
    local $/="\n"; # If delimiter is set, then $/ will be wrong
    my $no_of_cores;
    if ($^O eq 'linux') {
	$no_of_cores = no_of_cores_gnu_linux();
    } elsif ($^O eq 'freebsd') {
        $no_of_cores = no_of_cores_freebsd();
    } elsif ($^O eq 'netbsd') {
        $no_of_cores = no_of_cores_netbsd();
    } elsif ($^O eq 'openbsd') {
        $no_of_cores = no_of_cores_openbsd();
    } elsif ($^O eq 'gnu') {
        $no_of_cores = no_of_cores_hurd();
    } elsif ($^O eq 'darwin') {
	$no_of_cores = no_of_cores_darwin();
    } elsif ($^O eq 'solaris') {
	$no_of_cores = no_of_cores_solaris();
    } elsif ($^O eq 'aix') {
        $no_of_cores = no_of_cores_aix();
    } elsif ($^O eq 'hpux') {
        $no_of_cores = no_of_cores_hpux();
    } elsif ($^O eq 'nto') {
        $no_of_cores = no_of_cores_qnx();
    } elsif ($^O eq 'svr5') {
        $no_of_cores = no_of_cores_openserver();
    } elsif ($^O eq 'irix') {
        $no_of_cores = no_of_cores_irix();
    } elsif ($^O eq 'dec_osf') {
        $no_of_cores = no_of_cores_tru64();
    } else {
	$no_of_cores = (no_of_cores_gnu_linux()
			|| no_of_cores_freebsd()
			|| no_of_cores_netbsd()
			|| no_of_cores_openbsd()
			|| no_of_cores_hurd()
			|| no_of_cores_darwin()
			|| no_of_cores_solaris()
			|| no_of_cores_aix()
			|| no_of_cores_hpux()
			|| no_of_cores_qnx()
			|| no_of_cores_openserver()
			|| no_of_cores_irix()
			|| no_of_cores_tru64()
	    );
    }
    if($no_of_cores) {
	chomp $no_of_cores;
        return $no_of_cores;
    } else {
        ::warning("Cannot figure out number of CPU cores. Using 1.\n");
        return 1;
    }
}

sub no_of_cpus_gnu_linux {
    # Returns:
    #   Number of physical CPUs on GNU/Linux
    #   undef if not GNU/Linux
    my $no_of_cpus;
    my $no_of_cores;
    if(-e "/proc/cpuinfo") {
        $no_of_cpus = 0;
        $no_of_cores = 0;
        my %seen;
        open(my $in_fh, "-|", "cat /proc/cpuinfo") || return undef;
        while(<$in_fh>) {
            if(/^physical id.*[:](.*)/ and not $seen{$1}++) {
                $no_of_cpus++;
            }
            /^processor.*[:]/ and $no_of_cores++;
        }
        close $in_fh;
    }
    return ($no_of_cpus||$no_of_cores);
}

sub no_of_cores_gnu_linux {
    # Returns:
    #   Number of CPU cores on GNU/Linux
    #   undef if not GNU/Linux
    my $no_of_cores;
    if(-e "/proc/cpuinfo") {
        $no_of_cores = 0;
        open(my $in_fh, "-|", "cat /proc/cpuinfo") || return undef;
        while(<$in_fh>) {
            /^processor.*[:]/ and $no_of_cores++;
        }
        close $in_fh;
    }
    return $no_of_cores;
}

sub no_of_cpus_freebsd {
    # Returns:
    #   Number of physical CPUs on FreeBSD
    #   undef if not FreeBSD
    my $no_of_cpus =
	(`sysctl -a dev.cpu 2>/dev/null | grep \%parent | awk '{ print \$2 }' | uniq | wc -l | awk '{ print \$1 }'`
	 or
	 `sysctl hw.ncpu 2>/dev/null | awk '{ print \$2 }'`);
    chomp $no_of_cpus;
    return $no_of_cpus;
}

sub no_of_cores_freebsd {
    # Returns:
    #   Number of CPU cores on FreeBSD
    #   undef if not FreeBSD
    my $no_of_cores =
	(`sysctl hw.ncpu 2>/dev/null | awk '{ print \$2 }'`
	 or
	 `sysctl -a hw  2>/dev/null | grep [^a-z]logicalcpu[^a-z] | awk '{ print \$2 }'`);
    chomp $no_of_cores;
    return $no_of_cores;
}

sub no_of_cpus_netbsd {
    # Returns:
    #   Number of physical CPUs on NetBSD
    #   undef if not NetBSD
    my $no_of_cpus = `sysctl -n hw.ncpu 2>/dev/null`;
    chomp $no_of_cpus;
    return $no_of_cpus;
}

sub no_of_cores_netbsd {
    # Returns:
    #   Number of CPU cores on NetBSD
    #   undef if not NetBSD
    my $no_of_cores = `sysctl -n hw.ncpu 2>/dev/null`;
    chomp $no_of_cores;
    return $no_of_cores;
}

sub no_of_cpus_openbsd {
    # Returns:
    #   Number of physical CPUs on OpenBSD
    #   undef if not OpenBSD
    my $no_of_cpus = `sysctl -n hw.ncpu 2>/dev/null`;
    chomp $no_of_cpus;
    return $no_of_cpus;
}

sub no_of_cores_openbsd {
    # Returns:
    #   Number of CPU cores on OpenBSD
    #   undef if not OpenBSD
    my $no_of_cores = `sysctl -n hw.ncpu 2>/dev/null`;
    chomp $no_of_cores;
    return $no_of_cores;
}

sub no_of_cpus_hurd {
    # Returns:
    #   Number of physical CPUs on HURD
    #   undef if not HURD
    my $no_of_cpus = `nproc`;
    chomp $no_of_cpus;
    return $no_of_cpus;
}

sub no_of_cores_hurd {
    # Returns:
    #   Number of physical CPUs on HURD
    #   undef if not HURD
    my $no_of_cores = `nproc`;
    chomp $no_of_cores;
    return $no_of_cores;
}

sub no_of_cpus_darwin {
    # Returns:
    #   Number of physical CPUs on Mac Darwin
    #   undef if not Mac Darwin
    my $no_of_cpus =
	(`sysctl -n hw.physicalcpu 2>/dev/null`
	 or
	 `sysctl -a hw 2>/dev/null | grep [^a-z]physicalcpu[^a-z] | awk '{ print \$2 }'`);
    return $no_of_cpus;
}

sub no_of_cores_darwin {
    # Returns:
    #   Number of CPU cores on Mac Darwin
    #   undef if not Mac Darwin
    my $no_of_cores =
	(`sysctl -n hw.logicalcpu 2>/dev/null`
	 or
	 `sysctl -a hw  2>/dev/null | grep [^a-z]logicalcpu[^a-z] | awk '{ print \$2 }'`);
    return $no_of_cores;
}

sub no_of_cpus_solaris {
    # Returns:
    #   Number of physical CPUs on Solaris
    #   undef if not Solaris
    if(-x "/usr/sbin/psrinfo") {
        my @psrinfo = `/usr/sbin/psrinfo`;
        if($#psrinfo >= 0) {
            return $#psrinfo +1;
        }
    }
    if(-x "/usr/sbin/prtconf") {
        my @prtconf = `/usr/sbin/prtconf | grep cpu..instance`;
        if($#prtconf >= 0) {
            return $#prtconf +1;
        }
    }
    return undef;
}

sub no_of_cores_solaris {
    # Returns:
    #   Number of CPU cores on Solaris
    #   undef if not Solaris
    if(-x "/usr/sbin/psrinfo") {
        my @psrinfo = `/usr/sbin/psrinfo`;
        if($#psrinfo >= 0) {
            return $#psrinfo +1;
        }
    }
    if(-x "/usr/sbin/prtconf") {
        my @prtconf = `/usr/sbin/prtconf | grep cpu..instance`;
        if($#prtconf >= 0) {
            return $#prtconf +1;
        }
    }
    return undef;
}

sub no_of_cpus_aix {
    # Returns:
    #   Number of physical CPUs on AIX
    #   undef if not AIX
    my $no_of_cpus = 0;
    if(-x "/usr/sbin/lscfg") {
	open(my $in_fh, "-|", "/usr/sbin/lscfg -vs |grep proc | wc -l|tr -d ' '")
	    || return undef;
	$no_of_cpus = <$in_fh>;
	chomp ($no_of_cpus);
	close $in_fh;
    }
    return $no_of_cpus;
}

sub no_of_cores_aix {
    # Returns:
    #   Number of CPU cores on AIX
    #   undef if not AIX
    my $no_of_cores;
    if(-x "/usr/bin/vmstat") {
	open(my $in_fh, "-|", "/usr/bin/vmstat 1 1") || return undef;
	while(<$in_fh>) {
	    /lcpu=([0-9]*) / and $no_of_cores = $1;
	}
	close $in_fh;
    }
    return $no_of_cores;
}

sub no_of_cpus_hpux {
    # Returns:
    #   Number of physical CPUs on HP-UX
    #   undef if not HP-UX
    my $no_of_cpus =
        (`/usr/bin/mpsched -s 2>&1 | grep 'Locality Domain Count' | awk '{ print \$4 }'`);
    return $no_of_cpus;
}

sub no_of_cores_hpux {
    # Returns:
    #   Number of CPU cores on HP-UX
    #   undef if not HP-UX
    my $no_of_cores =
        (`/usr/bin/mpsched -s 2>&1 | grep 'Processor Count' | awk '{ print \$4 }'`);
    return $no_of_cores;
}

sub no_of_cpus_qnx {
    # Returns:
    #   Number of physical CPUs on QNX
    #   undef if not QNX
    # BUG: It is now known how to calculate this.
    my $no_of_cpus = 0;
    return $no_of_cpus;
}

sub no_of_cores_qnx {
    # Returns:
    #   Number of CPU cores on QNX
    #   undef if not QNX
    # BUG: It is now known how to calculate this.
    my $no_of_cores = 0;
    return $no_of_cores;
}

sub no_of_cpus_openserver {
    # Returns:
    #   Number of physical CPUs on SCO OpenServer
    #   undef if not SCO OpenServer
    my $no_of_cpus = 0;
    if(-x "/usr/sbin/psrinfo") {
        my @psrinfo = `/usr/sbin/psrinfo`;
        if($#psrinfo >= 0) {
            return $#psrinfo +1;
        }
    }
    return $no_of_cpus;
}

sub no_of_cores_openserver {
    # Returns:
    #   Number of CPU cores on SCO OpenServer
    #   undef if not SCO OpenServer
    my $no_of_cores = 0;
    if(-x "/usr/sbin/psrinfo") {
        my @psrinfo = `/usr/sbin/psrinfo`;
        if($#psrinfo >= 0) {
            return $#psrinfo +1;
        }
    }
    return $no_of_cores;
}

sub no_of_cpus_irix {
    # Returns:
    #   Number of physical CPUs on IRIX
    #   undef if not IRIX
    my $no_of_cpus =
        (`hinv | grep HZ | grep Processor | awk '{print \$1}'`);
    return $no_of_cpus;
}

sub no_of_cores_irix {
    # Returns:
    #   Number of CPU cores on IRIX
    #   undef if not IRIX
    my $no_of_cores = 
        (`hinv | grep HZ | grep Processor | awk '{print \$1}'`);
    return $no_of_cores;
}

sub no_of_cpus_tru64 {
    # Returns:
    #   Number of physical CPUs on Tru64
    #   undef if not Tru64
    my $no_of_cpus =
        (`sizer -pr`);
    return $no_of_cpus;
}

sub no_of_cores_tru64 {
    # Returns:
    #   Number of CPU cores on Tru64
    #   undef if not Tru64
    my $no_of_cores = 
        (`sizer -pr`);
    return $no_of_cores;
}

sub sshcommand {
    my $self = shift;
    if (not defined $self->{'sshcommand'}) {
        $self->sshcommand_of_sshlogin();
    }
    return $self->{'sshcommand'};
}

sub serverlogin {
    my $self = shift;
    if (not defined $self->{'serverlogin'}) {
        $self->sshcommand_of_sshlogin();
    }
    return $self->{'serverlogin'};
}

sub sshcommand_of_sshlogin {
    # 'server' -> ('ssh -S /tmp/parallel-ssh-RANDOM/host-','server')
    # 'user@server' -> ('ssh','user@server')
    # 'myssh user@server' -> ('myssh','user@server')
    # 'myssh -l user server' -> ('myssh -l user','server')
    # '/usr/bin/myssh -l user server' -> ('/usr/bin/myssh -l user','server')
    # Returns:
    #   sshcommand - defaults to 'ssh'
    #   login@host
    my $self = shift;
    my ($sshcmd, $serverlogin);
    if($self->{'string'} =~ /(.+) (\S+)$/) {
        # Own ssh command
        $sshcmd = $1; $serverlogin = $2;
    } else {
        # Normal ssh
        if($opt::controlmaster) {
            # Use control_path to make ssh faster
            my $control_path = $self->control_path_dir()."/ssh-%r@%h:%p";
            $sshcmd = "ssh -S ".$control_path;
            $serverlogin = $self->{'string'};
	    # OpenSSH_3.6.1p2 gives 'tcgetattr: Invalid argument' with -tt
	    # 2>/dev/null to ignore "process_mux_new_session: tcgetattr: Invalid argument"
            my $master = "ssh -MTS $control_path $serverlogin sleep 1 2>/dev/null";
            if(not $self->{'control_path'}{$control_path}++) {
                # Master is not running for this control_path
                # Start it
                my $pid = fork();
                if($pid) {
                    $Global::sshmaster{$pid} ||= 1;
                } else {
                    ::debug($master,"\n");
                    `$master </dev/null`;
                    ::wait_and_exit(0);
                }
            }
        } else {
            $sshcmd = "ssh"; $serverlogin = $self->{'string'};
        }
    }
    $self->{'sshcommand'} = $sshcmd;
    $self->{'serverlogin'} = $serverlogin;
}

sub control_path_dir {
    # Returns:
    #   path to directory
    my $self = shift;
    if(not defined $self->{'control_path_dir'}) {
        -e $ENV{'HOME'}."/.parallel" or mkdir $ENV{'HOME'}."/.parallel";
        -e $ENV{'HOME'}."/.parallel/tmp" or mkdir $ENV{'HOME'}."/.parallel/tmp";
        $self->{'control_path_dir'} =
	    File::Temp::tempdir($ENV{'HOME'}
				. "/.parallel/tmp/control_path_dir-XXXX",
				CLEANUP => 1);
    }
    return $self->{'control_path_dir'};
}


sub rsync_transfer_cmd {
  # Command to run to transfer a file
  # Input:
  #   $file = filename of file to transfer
  #   $workdir = destination dir
  # Returns:
  #   $cmd = rsync command to run to transfer $file ("" if unreadable)
  my $self = shift;
  my $file = shift;
  my $workdir = shift;
  if(not -r $file) {
    ::warning($file, " is not readable and will not be transferred.\n");
    return "true";
  }
  my $rsync_destdir;
  if($file =~ m:^/:) {
    # rsync /foo/bar /
    $rsync_destdir = "/";
  } else {
    $rsync_destdir = ::shell_quote_file($workdir);
  }
  $file = ::shell_quote_file($file);
  my $sshcmd = $self->sshcommand();
  my $rsync_opt = "-rlDzR -e" . ::shell_quote_scalar($sshcmd);
  my $serverlogin = $self->serverlogin();
  # Make dir if it does not exist
  return "( $sshcmd $serverlogin mkdir -p $rsync_destdir;" .
    "rsync $rsync_opt $file $serverlogin:$rsync_destdir )";
}

sub cleanup_cmd {
  # Command to run to remove the remote file
  # Input:
  #   $file = filename to remove
  #   $workdir = destination dir
  # Returns:
  #   $cmd = ssh command to run to remove $file and empty parent dirs
  my $self = shift;
  my $file = shift;
  my $workdir = shift;
  my $f = $file;
  if($f =~ m:/\./:) {
      # foo/bar/./baz/quux => workdir/baz/quux
      # /foo/bar/./baz/quux => workdir/baz/quux
      $f =~ s:.*/\./:$workdir/:;
  } elsif($f =~ m:^[^/]:) {
      # foo/bar => workdir/foo/bar
      $f = $workdir."/".$f;
  }
  my @subdirs = split m:/:, ::dirname($f);
  my @rmdir;
  my $dir = "";
  for(@subdirs) {
    $dir .= $_."/";
    unshift @rmdir, ::shell_quote_file($dir);
  }
  my $rmdir = @rmdir ? "rmdir @rmdir 2>/dev/null;" : "";
  if(defined $opt::workdir and $opt::workdir eq "...") {
    $rmdir .= "rm -rf " . ::shell_quote_file($workdir).';';
  }

  $f = ::shell_quote_file($f);
  my $sshcmd = $self->sshcommand();
  my $serverlogin = $self->serverlogin();
  return "$sshcmd $serverlogin ".::shell_quote_scalar("(rm -f $f; $rmdir)");
}

package JobQueue;

sub new {
    my $class = shift;
    my $command = shift;
    my $read_from = shift;
    my $context_replace = shift;
    my $max_number_of_args = shift;
    my $return_files = shift;
    my $commandlinequeue = CommandLineQueue->new(
        $command,$read_from,$context_replace,$max_number_of_args,$return_files);
    my @unget = ();
    return bless {
        'unget' => \@unget,
        'commandlinequeue' => $commandlinequeue,
        'total_jobs' => undef,
    }, ref($class) || $class;
}

sub get {
    my $self = shift;

    if(@{$self->{'unget'}}) {
        my $job = shift @{$self->{'unget'}};
        return ($job);
    } else {
        my $commandline = $self->{'commandlinequeue'}->get();
        if(defined $commandline) {
            my $job = Job->new($commandline);
            return $job;
        } else {
            return undef;
        }
    }
}

sub unget {
    my $self = shift;
    unshift @{$self->{'unget'}}, @_;
}

sub empty {
    my $self = shift;
    my $empty = (not @{$self->{'unget'}})
	&& $self->{'commandlinequeue'}->empty();
    ::debug("JobQueue->empty $empty ");
    return $empty;
}

sub total_jobs {
    my $self = shift;
    if(not defined $self->{'total_jobs'}) {
        my $job;
        my @queue;
        while($job = $self->get()) {
            push @queue, $job;
        }
        $self->unget(@queue);
        $self->{'total_jobs'} = $#queue+1;
    }
    return $self->{'total_jobs'};
}

sub next_seq {
    my $self = shift;

    return $self->{'commandlinequeue'}->seq();
}

sub quote_args {
    my $self = shift;
    return $self->{'commandlinequeue'}->quote_args();
}


package Job;

sub new {
    my $class = shift;
    my $commandline = shift;
    return bless {
        'commandline' => $commandline, # The commandline with no args
        'workdir' => undef, # --workdir
        'stdin' => undef, # filehandle for stdin (used for --pipe)
	# filename for writing stdout to (used for --files)
        'remaining' => "", # remaining data not sent to stdin (used for --pipe)
	'datawritten' => 0, # amount of data sent via stdin (used for --pipe)
        'transfersize' => 0, # size of files using --transfer
        'returnsize' => 0, # size of files using --return
        'pid' => undef,
        # hash of { SSHLogins => number of times the command failed there }
        'failed' => undef,
        'sshlogin' => undef,
        # The commandline wrapped with rsync and ssh
        'sshlogin_wrap' => undef,
        'exitstatus' => undef,
        'exitsignal' => undef,
	# Timestamp for timeout if any
	'timeout' => undef,
	'virgin' => 1,
    }, ref($class) || $class;
}

sub replaced {
    my $self = shift;
    $self->{'commandline'} or Carp::croak("cmdline empty");
    return $self->{'commandline'}->replaced();
}

sub seq {
    my $self = shift;
    return $self->{'commandline'}->seq();
}

sub openoutputfiles {
    # Open files for STDOUT and STDERR
    # Set file handles in $self->fd and possibly $self->fd_input
    my $self = shift;
    my ($outfh, $errfh, $outname, $errname, $unlink_out, $unlink_err);
    if($opt::results) {
	my $args_as_dirname = $self->{'commandline'}->args_as_dirname();
	# prefix/name1/val1/name2/val2/
	my $dir = $opt::results."/".$args_as_dirname;
	File::Path::mkpath($dir);
	# prefix/name1/val1/name2/val2/stdout
	$outname = "$dir/stdout";
	if(not open($outfh, "+>", $outname)) {
	    ::error("Cannot write to `$outname'.\n");
	    ::wait_and_exit(255);
	}
	# prefix/name1/val1/name2/val2/stderr
	$errname = "$dir/stderr";
	if(not open($errfh, "+>", $errname)) {
	    ::error("Cannot write to `$errname'.\n");
	    ::wait_and_exit(255);
	}
    } elsif($Global::grouped) {
	# To group we create temporary files for STDOUT and STDERR
	# To avoid the cleanup unlink the files immediately (but keep them open)
	if(@Global::tee_jobs) {
	    # files must be removed when the tee is done
	} elsif($opt::files) {
	    ($outfh, $outname) = ::tempfile(SUFFIX => ".par");
	    ($errfh, $errname) = ::tempfile(SUFFIX => ".par");
	    # --files => only remove stderr
	    $unlink_err = 1;
	} else {
	    ($outfh, $outname) = ::tempfile(SUFFIX => ".par");
	    ($errfh, $errname) = ::tempfile(SUFFIX => ".par");
	    $unlink_out = 1;
	    $unlink_err = 1;
	}
    } else {
	# --ungroup
	$outfh = *STDOUT;
	$errfh = *STDERR;
	# File name must be empty as it will otherwise be printed
	$outname = "";
	$errname = "";
	$unlink_out = 1;
	$unlink_err = 1;
    }
    $self->set_fd_file_name(1,$outname);
    $self->set_fd_file_name(2,$errname);
    if($opt::compress) {
	$self->set_fd_input(1, $outfh);
	$self->set_fd_input(2, $errfh);
	my $rm = $unlink_out ? "rm" : "true";
	::debug("| ($rm $outname; $opt::compress_program) > $outname\n");
	open(my $coutfh,"|-","($rm $outname; $opt::compress_program) > $outname") || die $?;
	$rm = $unlink_err ? "rm" : "true";
	::debug("| ($rm $errname; $opt::compress_program) > $errname\n");
	open(my $cerrfh,"|-","($rm $errname; $opt::compress_program) > $errname") || die $?;
        $self->set_fd(1,$coutfh);
	$self->set_fd(2,$cerrfh);
    } else {
        $self->set_fd(1,$outfh);
	$self->set_fd(2,$errfh);
	$unlink_out && unlink($outname);
	$unlink_err && unlink($errname);
    }
}

sub set_fd {
    # Set file descriptor
    my ($self, $fd_no, $fd) = @_;
    $self->{'fd'}{$fd_no} = $fd;
}

sub fd {
    # Get file descriptor
    my ($self, $fd_no) = @_;
    return $self->{'fd'}{$fd_no};
}

sub set_fd_input {
    my ($self, $fd_no, $fd_input) = @_;
    $self->{'fd_input'}{$fd_no} = $fd_input;
}

sub fd_input {
    # Get input file descriptor
    my ($self, $fd_no) = @_;
    return $self->{'fd_input'}{$fd_no};
}

sub set_fd_file_name {
    # Set file name for a file descriptor
    my $self = shift;
    my $fd_no = shift;
    $self->{'fd_file_name',$fd_no} = shift;
}

sub fd_file_name {
    # Get file name for a file descriptor
    my $self = shift;
    my $fd_no = shift;
    return $self->{'fd_file_name',$fd_no};
}

sub write {
    my $self = shift;
    my $remaining_ref = shift;
    my $stdin_fh = $self->fd(0);
    syswrite($stdin_fh,$$remaining_ref);
}

sub set_stdin_buffer {
    my $self = shift;
    my ($header_ref,$block_ref,$endpos,$recstart,$recend) = @_;
    $self->{'stdin_buffer'} = ($self->virgin() ? $$header_ref : "").substr($$block_ref,0,$endpos);
    if($opt::remove_rec_sep) {
	remove_rec_sep(\$self->{'stdin_buffer'},$recstart,$recend);
    }
    $self->{'stdin_buffer_length'} = length $self->{'stdin_buffer'};
    $self->{'stdin_buffer_pos'} = 0;
}

sub stdin_buffer_length {
    my $self = shift;
    return $self->{'stdin_buffer_length'};
}

sub remove_rec_sep {
    my ($block_ref,$recstart,$recend) = @_;
    # Remove record separator
    $$block_ref =~ s/$recend$recstart//gos;
    $$block_ref =~ s/^$recstart//os;
    $$block_ref =~ s/$recend$//os;
}

sub non_block_write {
    my $self = shift;
    my $something_written = 0;
    use POSIX qw(:errno_h);
#    use Fcntl;
#    my $flags = '';
    for my $buf (substr($self->{'stdin_buffer'},$self->{'stdin_buffer_pos'})) {
	my $in = $self->fd(0);
#	fcntl($in, F_GETFL, $flags)
#	    or die "Couldn't get flags for HANDLE : $!\n";
#	$flags |= O_NONBLOCK;
#	fcntl($in, F_SETFL, $flags)
#	    or die "Couldn't set flags for HANDLE: $!\n";
	my $rv = syswrite($in, $buf);
	if (!defined($rv) && $! == EAGAIN) {
	    # would block
	    $something_written = 0;
	} elsif ($self->{'stdin_buffer_pos'}+$rv != $self->{'stdin_buffer_length'}) {
	    # incomplete write
	    # Remove the written part
	    $self->{'stdin_buffer_pos'} += $rv;
	    $something_written = $rv;
	} else {
	    # successfully wrote everything
	    my $a="";
	    $self->set_stdin_buffer(\$a,\$a,"","");
	    $something_written = $rv;
	}
    }

    ::debug("Non-block: $something_written");
    return $something_written;
}


sub virgin {
    my $self = shift;
    return $self->{'virgin'};
}

sub set_virgin {
    my $self = shift;
    $self->{'virgin'} = shift;
}

sub pid {
    my $self = shift;
    return $self->{'pid'};
}

sub set_pid {
    my $self = shift;
    $self->{'pid'} = shift;
}

sub starttime {
    # Returns:
    #   UNIX-timestamp this job started
    my $self = shift;
    return sprintf("%.3f",$self->{'starttime'});
}

sub set_starttime {
    my $self = shift;
    my $starttime = shift || ::now();
    $self->{'starttime'} = $starttime;
}

sub runtime {
    # Returns:
    #   Run time in seconds
    my $self = shift;
    return sprintf("%.3f",int(($self->endtime() - $self->starttime())*1000)/1000);
}

sub endtime {
    # Returns:
    #   UNIX-timestamp this job ended
    #   0 if not ended yet
    my $self = shift;
    return ($self->{'endtime'} || 0);
}

sub set_endtime {
    my $self = shift;
    my $endtime = shift;
    $self->{'endtime'} = $endtime;
}

sub timedout {
    my $self = shift;
    my $delta_time = shift;
    return time > $self->{'starttime'} + $delta_time;
}

sub kill {
    # kill the jobs
    my $self = shift;
    my @signals = @_;
    my @family_pids = $self->family_pids();
    # Record this jobs as failed
    $self->set_exitstatus(-1);
    # Send two TERMs to give time to clean up
    ::debug("Kill seq ".$self->seq()."\n");
    my @send_signals = @signals || ("TERM", "TERM", "KILL");
    for my $signal (@send_signals) {
	my $alive = 0;
	for my $pid (@family_pids) {
	    if(kill 0, $pid) {
		# The job still running
		kill $signal, $pid;
		$alive = 1;
	    }
	}
	# If a signal was given as input, do not do the sleep below
	@signals and next;

	if($signal eq "TERM" and $alive) {
	    # Wait up to 200 ms between TERMs - but only if any pids are alive
	    my $sleep = 1;
	    for (my $sleepsum = 0; kill 0, $family_pids[0] and $sleepsum < 200;
		 $sleepsum += $sleep) {
		$sleep = ::reap_usleep($sleep);
	    }
	}
    }
}

sub family_pids {
    # Find the pids with this->pid as (grand)*parent
    # TODO test this on different OS as 'ps' is known to be different
    my $self = shift;
    my $pid = $self->pid();
    my (@pidtable,%children_of,@pids);
    if ($^O eq 'linux' or $^O eq 'solaris') {
	# Table with pid parentpid (SysV + GNU)
	@pidtable = `ps -ef | awk '{print \$2" "\$3}'`;
    } else {
	# Table with pid parentpid (BSD)
	@pidtable = `ps -o pid,ppid -ax`;
    }
    for (@pidtable) {
	/(\S+)\s+(\S+)/ or ::die_bug("pidtable format");
	push(@{$children_of{$2}},$1);
    }
    my @more = ($pid);
    while(@more) {
	my @m;
	push @pids,@more;
	for my $parent (@more) {
	    if($children_of{$parent}) {
		push @m, @{$children_of{$parent}};
	    }
	}
	@more = @m;
    }

    return (@pids);
}

sub failed {
    # return number of times failed for this $sshlogin
    my $self = shift;
    my $sshlogin = shift;
    return $self->{'failed'}{$sshlogin};
}

sub failed_here {
    # return number of times failed for the current $sshlogin
    my $self = shift;
    return $self->{'failed'}{$self->sshlogin()};
}

sub add_failed {
    # increase the number of times failed for this $sshlogin
    my $self = shift;
    my $sshlogin = shift;
    $self->{'failed'}{$sshlogin}++;
}

sub add_failed_here {
    # increase the number of times failed for the current $sshlogin
    my $self = shift;
    $self->{'failed'}{$self->sshlogin()}++;
}

sub reset_failed {
    # increase the number of times failed for this $sshlogin
    my $self = shift;
    my $sshlogin = shift;
    delete $self->{'failed'}{$sshlogin};
}

sub reset_failed_here {
    # increase the number of times failed for this $sshlogin
    my $self = shift;
    delete $self->{'failed'}{$self->sshlogin()};
}

sub min_failed {
    # Returns:
    #   the number of sshlogins this command has failed on
    #   the minimal number of times this command has failed
    my $self = shift;
    my $min_failures =
	::min(map { $self->{'failed'}{$_} }
		keys %{$self->{'failed'}});
    my $number_of_sshlogins_failed_on = scalar keys %{$self->{'failed'}};
    return ($number_of_sshlogins_failed_on,$min_failures);
}

sub total_failed {
    # Returns:
    #   the number of times this command has failed
    my $self = shift;
    my $total_failures = 0;
    for (values %{$self->{'failed'}}) {
	$total_failures += $_;
    }
    return ($total_failures);
}

sub set_sshlogin {
    my $self = shift;
    my $sshlogin = shift;
    $self->{'sshlogin'} = $sshlogin;
    delete $self->{'sshlogin_wrap'}; # If sshlogin is changed the wrap is wrong
}

sub sshlogin {
    my $self = shift;
    return $self->{'sshlogin'};
}

sub sshlogin_wrap {
    # Wrap the command with the commands needed to run remotely
    my $self = shift;
    if(not defined $self->{'sshlogin_wrap'}) {
	my $sshlogin = $self->sshlogin();
	my $sshcmd = $sshlogin->sshcommand();
	my $serverlogin = $sshlogin->serverlogin();
	my $next_command_line = $Global::envvar.$self->replaced();
	my ($pre,$post,$cleanup)=("","","");
	if($serverlogin eq ":") {
	    $self->{'sshlogin_wrap'} = $next_command_line;
	} else {
	    # --transfer
	    $pre .= $self->sshtransfer();
	    # --return
	    $post .= $self->sshreturn();
	    # --cleanup
	    $post .= $self->sshcleanup();
	    if($post) {
		# We need to save the exit status of the job
		$post = '_EXIT_status=$?; ' . $post . ' exit $_EXIT_status;';
	    }
	    # If the remote login shell is (t)csh then use 'setenv'
	    # otherwise use 'export'
	    # We cannot use parse_env_var(), as PARALLEL_SEQ changes
	    # for each command
	    my $parallel_env =
		(q{ 'eval `echo $SHELL | grep "/t\\{0,1\\}csh" > /dev/null }
		 . q{ && echo setenv PARALLEL_SEQ '$PARALLEL_SEQ'\; }
		 . q{ setenv PARALLEL_PID '$PARALLEL_PID' }
		 . q{ || echo PARALLEL_SEQ='$PARALLEL_SEQ'\;export PARALLEL_SEQ\; }
		 . q{ PARALLEL_PID='$PARALLEL_PID'\;export PARALLEL_PID` ;' });
	    my $remote_pre = "";
	    my $ssh_options = "";
	    if($opt::pipe and $opt::ctrlc
	       or
	       not $opt::pipe and not $opt::noctrlc) {
		# Propagating CTRL-C to kill remote jobs requires
		# remote jobs to be run with a terminal.
		$ssh_options = "-tt -oLogLevel=quiet";
		# tty - check if we have a tty.
		# stty:
		#   -onlcr - make output 8-bit clean
		#   isig - pass CTRL-C as signal
		#   -echo - do not echo input
		$remote_pre .= ::shell_quote_scalar('tty >/dev/null && stty isig -onlcr -echo;');
	    }
	    if($opt::workdir) {
		my $wd = ::shell_quote_file($self->workdir());
		$remote_pre .= ::shell_quote_scalar("mkdir -p ") . $wd .
		    ::shell_quote_scalar("; cd ") . $wd . 
		    ::shell_quote_scalar(qq{ || exit 255;});
	    }
	    $self->{'sshlogin_wrap'} =
		($pre
		 . "$sshcmd $ssh_options $serverlogin $parallel_env "
		 . $remote_pre
		 . ::shell_quote_scalar($next_command_line) . ";" 
		 . $post);
	}
    }
    return $self->{'sshlogin_wrap'};
}

sub transfer {
    # Files to transfer
    # Returns:
    #   @transfer - File names of files to transfer
    my $self = shift;
    my @transfer = ();
    $self->{'transfersize'} = 0;
    if($opt::transfer) {
	for my $record (@{$self->{'commandline'}{'arg_list'}}) {
	    # Merge arguments from records into args
	    for my $arg (@$record) {
		CORE::push @transfer, $arg->orig();
		# filesize
		if(-e $arg->orig()) {
		    $self->{'transfersize'} += (stat($arg->orig()))[7];
		}
	    }
	}
    }
    return @transfer;
}

sub transfersize {
    my $self = shift;
    return $self->{'transfersize'};
}

sub sshtransfer {
  # Returns for each transfer file:
  #   rsync $file remote:$workdir
    my $self = shift;
    my @pre;
    my $sshlogin = $self->sshlogin();
    my $workdir = $self->workdir();
    for my $file ($self->transfer()) {
      push @pre, $sshlogin->rsync_transfer_cmd($file,$workdir).";";
    }
    return join("",@pre);
}

sub return {
    # Files to return
    # Non-quoted and with {...} substituted
    # Returns:
    #   @non_quoted_filenames
    my $self = shift;
    my @return = ();
    for my $return (@{$self->{'commandline'}{'return_files'}}) {
	CORE::push @return,
	$self->{'commandline'}->replace_placeholders($return,0);
    }
    return @return;
}

sub returnsize {
    # This is called after the job has finished
    # Returns:
    #   $number_of_bytes transferred in return
    my $self = shift;
    for my $file ($self->return()) {
	if(-e $file) {
	    $self->{'returnsize'} += (stat($file))[7];
	}
    }
    return $self->{'returnsize'};
}

sub sshreturn {
  # Returns for each return-file:
  #   rsync remote:$workdir/$file .
    my $self = shift;
    my $sshlogin = $self->sshlogin();
    my $sshcmd = $sshlogin->sshcommand();
    my $serverlogin = $sshlogin->serverlogin();
    my $rsync_opt = "-rlDzR -e".::shell_quote_scalar($sshcmd);
    my $pre = "";
    for my $file ($self->return()) {
	$file =~ s:/\./:/:g; # Rsync treats /./ special. We dont want that
	$file =~ s:^\./::g; # Remove ./ if any
	my $relpath = ($file !~ m:^/:); # Is the path relative?
	my $cd = "";
	my $wd = "";
	if($relpath) {
	    #   rsync -avR /foo/./bar/baz.c remote:/tmp/
	    # == (on old systems)
	    #   rsync -avR --rsync-path="cd /foo; rsync" remote:bar/baz.c /tmp/
	    $wd = ::shell_quote_file($self->workdir()."/");
	}
	# Only load File::Basename if actually needed
	$Global::use{"File::Basename"} ||= eval "use File::Basename; 1;";
	$cd = ::shell_quote_file(dirname($file));
	my $rsync_cd = '--rsync-path='.::shell_quote_scalar("cd $wd$cd; rsync");
	my $basename = ::shell_quote_scalar(::shell_quote_file(basename($file)));
	# --return
	#   mkdir -p /home/tange/dir/subdir/; 
        #   rsync -rlDzR --rsync-path="cd /home/tange/dir/subdir/; rsync" 
        #   server:file.gz /home/tange/dir/subdir/
	$pre .= "mkdir -p $cd; rsync $rsync_cd $rsync_opt $serverlogin:".
	     $basename . " ".$cd.";";
    }
    return $pre;
}

sub sshcleanup {
    # Return the sshcommand needed to remove the file
    # Returns:
    #   ssh command needed to remove files from sshlogin
    my $self = shift;
    my $sshlogin = $self->sshlogin();
    my $sshcmd = $sshlogin->sshcommand();
    my $serverlogin = $sshlogin->serverlogin();
    my $workdir = $self->workdir();
    my $cleancmd = "";

    for my $file ($self->cleanup()) {
	my @subworkdirs = parentdirs_of($file);
	$cleancmd .= $sshlogin->cleanup_cmd($file,$workdir).";";
    }
    if(defined $opt::workdir and $opt::workdir eq "...") {
	$cleancmd .= "$sshcmd $serverlogin rm -rf " . ::shell_quote_scalar($workdir).';';
    }
    return $cleancmd;
}

sub cleanup {
    # Returns:
    #   Files to remove at cleanup
    my $self = shift;
    if($opt::cleanup) {
	my @transfer = $self->transfer();
	my @return = $self->return();
	return (@transfer,@return);
    } else {
	return ();
    }
}

sub workdir {
    # Returns:
    #   the workdir on a remote machine
    my $self = shift;
    if(not defined $self->{'workdir'}) {
	my $workdir;
	if(defined $opt::workdir) {
	    if($opt::workdir eq ".") {
		# . means current dir
		my $home = $ENV{'HOME'};
		eval 'use Cwd';
		my $cwd = cwd();
		$workdir = $cwd;
		if($home) {
		    # If homedir exists: remove the homedir from
		    # workdir if cwd starts with homedir
		    # E.g. /home/foo/my/dir => my/dir
		    # E.g. /tmp/my/dir => /tmp/my/dir
		    my ($home_dev, $home_ino) = (stat($home))[0,1];
		    my $parent = "";
		    my @dir_parts = split(m:/:,$cwd);
		    my $part;
		    while(defined ($part = shift @dir_parts)) {
			$part eq "" and next;
			$parent .= "/".$part;
			my ($parent_dev, $parent_ino) = (stat($parent))[0,1];
			if($parent_dev == $home_dev and $parent_ino == $home_ino) {
			    # dev and ino is the same: We found the homedir.
			    $workdir = join("/",@dir_parts);
			    last;
			}
		    }
		}
		if($workdir eq "") {
		    $workdir = ".";
		}
	    } elsif($opt::workdir eq "...") {
		$workdir = ".parallel/tmp/" . ::hostname() . "-" . $$
		    . "-" . $self->seq();
	    } else {
		$workdir = $opt::workdir;
		# Rsync treats /./ special. We dont want that
		$workdir =~ s:/\./:/:g; # Remove /./
		$workdir =~ s:/+$::; # Remove ending / if any
		$workdir =~ s:^\./::g; # Remove starting ./ if any
	    }
	} else {
	    $workdir = ".";
	}
	$self->{'workdir'} = ::shell_quote_scalar($workdir);
    }
    return $self->{'workdir'};
}

sub parentdirs_of {
    # Return:
    #   all parentdirs except . of this dir or file - sorted desc by length
    my $d = shift;
    my @parents = ();
    while($d =~ s:/[^/]+$::) {
	if($d ne ".") {
	    push @parents, $d;
	}
    }
    return @parents;
}

sub start {
    # Setup STDOUT and STDERR for a job and start it.
    # Returns:
    #   job-object or undef if job not to run
    my $job = shift;
    # Get the shell command to be executed (possibly with ssh infront).
    my $command = $job->sshlogin_wrap();

    if($Global::interactive or $Global::stderr_verbose) {
	if($Global::interactive) {
	    print $Global::original_stderr "$command ?...";
	    open(my $tty_fh, "<", "/dev/tty") || ::die_bug("interactive-tty");
	    my $answer = <$tty_fh>;
	    close $tty_fh;
	    my $run_yes = ($answer =~ /^\s*y/i);
	    if (not $run_yes) {
		$command = "true"; # Run the command 'true'
	    }
	} else {
	    print $Global::original_stderr "$command\n";
	}
    }

    my $pid;
    $job->openoutputfiles();
    my($stdout_fh,$stderr_fh) = ($job->fd(1),$job->fd(2));
    local (*IN,*OUT,*ERR);
    open OUT, '>&', $stdout_fh or ::die_bug("Can't redirect STDOUT: $!");
    open ERR, '>&', $stderr_fh or ::die_bug("Can't dup STDOUT: $!");

    if(($opt::dryrun or $Global::verbose) and not $Global::grouped) {
	if($Global::verbose <= 1) {
	    print $stdout_fh $job->replaced(),"\n";
	} else {
	    # Verbose level > 1: Print the rsync and stuff
	    print $stdout_fh $command,"\n";
	}
    }
    if($opt::dryrun) {
	$command = "true";
    }
    $ENV{'PARALLEL_SEQ'} = $job->seq();
    $ENV{'PARALLEL_PID'} = $$;
    ::debug("$Global::total_running processes. Starting ("
	    . $job->seq() . "): $command\n");
    if($opt::pipe) {
	my ($stdin_fh);
	# Wrap command with end-of-file detector, 
	# so we do not spawn a program if there is no input.
	# Exit value:
	#   empty input = true
	#   some input = exit val from command
	# Bug:
	#   If the command does not read the first char, the temp file
	#   is not deleted.
        my ($dummy_fh, $tmpfile) = ::tempfile(SUFFIX => ".chr");
	$command = qq{
             sh -c 'dd bs=1 count=1 of=$tmpfile 2>/dev/null';
             test \! -s "$tmpfile" && rm -f "$tmpfile" && exec true;
             (cat $tmpfile; rm $tmpfile; cat - ) | } .
		 "($command);";
	# The eval is needed to catch exception from open3
	eval {
	    $pid = ::open3($stdin_fh, ">&OUT", ">&ERR", $ENV{SHELL}, "-c", $command) ||
		::die_bug("open3-pipe");
	    1;
	};
	$job->set_fd(0,$stdin_fh);
    } elsif(@opt::a and not $Global::stdin_in_opt_a and $job->seq() == 1
	    and $job->sshlogin()->string() eq ":") {
	# Give STDIN to the first job if using -a (but only if running
	# locally - otherwise CTRL-C does not work for other jobs Bug#36585)
	*IN = *STDIN;
	# The eval is needed to catch exception from open3
	eval {
	    $pid = ::open3("<&IN", ">&OUT", ">&ERR", $ENV{SHELL}, "-c", $command) ||
		::die_bug("open3-a");
	    1;
	};
	# Re-open to avoid complaining
	open(STDIN, "<&", $Global::original_stdin)
	    or ::die_bug("dup-\$Global::original_stdin: $!");
    } elsif ($opt::tty and not $Global::tty_taken and -c "/dev/tty" and
	     open(my $devtty_fh, "<", "/dev/tty")) {
	# Give /dev/tty to the command if no one else is using it
	*IN = $devtty_fh;
	# The eval is needed to catch exception from open3
	eval {
	    $pid = ::open3("<&IN", ">&OUT", ">&ERR", $ENV{SHELL}, "-c", $command) ||
		::die_bug("open3-/dev/tty");
	    $Global::tty_taken = $pid;
	    close $devtty_fh;
	    1;
	};
    } else {
	eval {
	    $pid = ::open3(::gensym, ">&OUT", ">&ERR", $ENV{SHELL}, "-c", $command) ||
		::die_bug("open3-gensym");
	    1;
	};
    }
    if($pid) {
	# A job was started
	$Global::total_running++;
	$Global::total_started++;
	$job->set_pid($pid);
	$job->set_starttime();
	if($opt::timeout) {
	    $Global::timeoutq->insert($job);
	}
	if($opt::delay) {
	    $Global::JobQueue->empty() or ::usleep($opt::delay*1000);
	}
	$Global::newest_job = $job;
	return $job;
    } else {
	# No more processes
	::debug("Cannot spawn more jobs.\n");
	return undef;
    }
}

sub is_already_in_results {
    # Do we already have results for this job?
    # Returns:
    #   $job_already_run = bool whether there is output for this or not
    my $job = $_[0];
    my $args_as_dirname = $job->{'commandline'}->args_as_dirname();
    # prefix/name1/val1/name2/val2/
    my $dir = $opt::results."/".$args_as_dirname;
    ::debug("Test $dir/stdout",  -e "$dir/stdout","\n");
    return -e "$dir/stdout";
}

sub is_already_in_joblog {
    my $job = shift;
    return vec($Global::job_already_run,$job->seq(),1);
}

sub set_job_in_joblog {
    my $job = shift;
    vec($Global::job_already_run,$job->seq(),1) = 1;
}

sub should_be_retried {
    # Should this job be retried?
    # Returns
    #   0 - do not retry
    #   1 - job queued for retry
    my $self = shift;
    if (not $opt::retries) {
	return 0;
    }
    if(not $self->exitstatus()) {
	# Completed with success. If there is a recorded failure: forget it
	$self->reset_failed_here();
	return 0
    } else {
	# The job failed. Should it be retried?
	$self->add_failed_here();
	if($self->total_failed() == $opt::retries) {
	    # This has been retried enough
	    return 0;
	} else {
	    # This command should be retried
	    $self->set_endtime(undef);
	    $Global::JobQueue->unget($self);
	    ::debug("Retry ".$self->seq()."\n");
	    return 1;
	}
    }
}

sub print {
    # Print the output of the jobs
    # Returns: N/A

    my $self = shift;
    ::debug(">>joboutput ".$self->replaced()."\n");
    if($opt::pipe and $self->virgin()) {
	# Nothing was printed to this job:
	# cleanup tmp files if --files was set
	unlink $self->fd_file_name(1);
	return;
    }
    if($opt::dryrun) {
	# Nothing was printed to this job:
	# cleanup tmp files if --files was set
	unlink $self->fd_file_name(1);
    }

    # Check for disk full
    exit_if_disk_full();
    if($Global::joblog) { $self->print_joblog() }

    # Printing is only relevant for grouped output.
    $Global::grouped or return;
    my $command = $self->sshlogin_wrap();

    if(($opt::dryrun or $Global::verbose) and $Global::grouped
       and
       not $self->{'verbose_printed'}) {
	$self->{'verbose_printed'}++;
	if($Global::verbose <= 1) {
	    print STDOUT $self->replaced(),"\n";
	} else {
	    # Verbose level > 1: Print the rsync and stuff
	    print STDOUT $command,"\n";
	}
	# If STDOUT and STDERR are merged,
	# we want the command to be printed first
	# so flush to avoid STDOUT being buffered
	flush STDOUT;
    }
    for my $fdno (sort { $a <=> $b } keys %Global::fd) {
	# Sort by file descriptor numerically: 1,2,3,..,9,10,11
	$fdno == 0 and next;
	my $out_fd = $Global::fd{$fdno};
	my $in_fd = $self->fd($fdno);
	if(not $in_fd) {
	    if(not $Job::file_descriptor_warning_printed{$fdno}++) {
		# ::warning("File descriptor $fdno not defined\n");
	    }
	    next;
	}
	::debug("File descriptor $fdno:");
	if($opt::files) {
	    # If --compress: $in_fd must be closed first.
	    close($in_fd);
	    if($fdno == 1 and $self->fd_file_name($fdno)) {
		print $out_fd $self->fd_file_name($fdno),"\n";
	    }
	} elsif($opt::linebuffer) {
	    # Line buffered print out
	    my $partial = \$self->{'partial_line',$fdno};
	    # This seek will clear EOF
	    seek $in_fd, tell($in_fd), 0;
	    while(read($in_fd,substr($$partial,length $$partial),1_000_000)) {
		# Append to $$partial
		# Find the last \n
		my $i = rindex($$partial,"\n");
		if($i != -1) {
		    # One or more complete lines were found
		    if($fdno == 2 and not $self->{'printed_first_line',$fdno}++) {
			# OpenSSH_3.6.1p2 gives 'tcgetattr: Invalid argument' with -tt
			# This is a crappy way of ignoring it.
			$$partial =~ s/^tcgetattr: Invalid argument\n//;
			# Length of partial line has changed: Find the last \n again
			$i = rindex($$partial,"\n");
		    }
		    if($opt::tag or defined $opt::tagstring) {
			# Replace ^ with $tag within the full line
			my $tag = $self->tag();
			substr($$partial,0,$i+1) =~ s/^/$tag/gm;
			# Length of partial line has changed: Find the last \n again
			$i = rindex($$partial,"\n");
		    }
		    # Print up to and including the last \n
		    print $out_fd substr($$partial,0,$i+1);
		    # Remove the printed part
		    substr($$partial,0,$i+1)="";
		}
	    }

	    if(defined $self->{'exitstatus'} and length $$partial > 0) {
		# If the job is dead: print the remaining partial line
		if($opt::tag or defined $opt::tagstring) {
		    my $tag = $self->tag();
		    $$partial =~ s/^/$tag/gm;
		}
		print $out_fd $$partial;
		$$partial = undef;
		# then close fd
		close $in_fd;
	    }
	} else {
	    my $buf;
	    if($opt::compress) {
		# Close for writing so the compressor can finish up
		# (Are we really sure it finishes? Potential bug)
		close($in_fd);
		# Save STDIN file handle
		open(my $stdin_copy, "<&", "STDIN") or ::die_bug("Can't dup STDIN: $!");
		my $fd = $self->fd_input($fdno);
		# Replace STDIN with the file on disk
		open(STDIN, "<&", $fd);
		::debug("$opt::decompress_program |");
		# Start decompression with the fake STDIN
		open($in_fd, "-|", $opt::decompress_program) ||
		    ::die_bug("Decompressing failed ($opt::decompress_program): $!");
		# Put STDIN file handle back
		open(STDIN, "<&", $stdin_copy) or ::die_bug("Can't dup STDIN: $!");
	    } else {
		# Seek to start
		seek $in_fd, 0, 0;
	    }
	    # $in_fd is now ready for reading at position 0
	    if($opt::tag or defined $opt::tagstring) {
		my $tag = $self->tag();
		if($fdno == 2) {
		    # OpenSSH_3.6.1p2 gives 'tcgetattr: Invalid argument' with -tt
		    # This is a crappy way of ignoring it.
		    while(<$in_fd>) {
			if($_ ne "tcgetattr: Invalid argument\n") {
			    print $out_fd $tag,$_;
			}
			# At most run the loop once
			last;
		    }
		}
		while(<$in_fd>) {
		    print $out_fd $tag,$_;
		}
	    } else {
		my $buf;
		if($fdno == 2) {
		    # OpenSSH_3.6.1p2 gives 'tcgetattr: Invalid argument' with -tt
		    # This is a crappy way of ignoring it.
		    sysread($in_fd,$buf,1_000);
		    $buf =~ s/^tcgetattr: Invalid argument\n//;
		    print $out_fd $buf;
		}
		while(sysread($in_fd,$buf,1_000_000)) {
		    print $out_fd $buf;
		}
	    }
	    close $in_fd;   
	}
	flush $out_fd;
	::debug("<<joboutput $command\n");
    }
}

sub print_joblog {
    my $self = shift;
    my $cmd;
    if($Global::verbose <= 1) {
	$cmd = $self->replaced();
    } else {
	# Verbose level > 1: Print the rsync and stuff
	$cmd = $command;
    }
    print $Global::joblog
	join("\t", $self->seq(), $self->sshlogin()->string(),
	     $self->starttime(), $self->runtime(),
	     $self->transfersize(), $self->returnsize(),
	     $self->exitstatus(), $self->exitsignal(), $cmd
	). "\n";
    flush $Global::joblog;
    $self->set_job_in_joblog();
}

sub tag {
    my $self = shift;
    if(not defined $self->{'tag'}) {
	$self->{'tag'} = $self->{'commandline'}->
	    replace_placeholders($opt::tagstring,0)."\t";
    }
    return $self->{'tag'};
}

sub exitstatus {
    my $self = shift;
    return $self->{'exitstatus'};
}

sub set_exitstatus {
    my $self = shift;
    my $exitstatus = shift;
    if($exitstatus) {
	# Overwrite status if non-zero
	$self->{'exitstatus'} = $exitstatus;
    } else {
	# Set status but do not overwrite
	# Status may have been set by --timeout
	$self->{'exitstatus'} ||= $exitstatus;
    }
}

sub exitsignal {
    my $self = shift;
    return $self->{'exitsignal'};
}

sub set_exitsignal {
    my $self = shift;
    my $exitsignal = shift;
    $self->{'exitsignal'} = $exitsignal;
}

{
    my ($disk_full_fh,$error_printed);
    sub exit_if_disk_full {
	# Checks if $TMPDIR is full by writing 8kb to a tmpfile
	# If the disk is full: Exit immediately.
	# Returns:
	#   N/A
	if(not $disk_full_fh) {
	    $disk_full_fh = ::tempfile();
	}
	my $pos = tell $disk_full_fh;
	print $disk_full_fh "x"x8193;
	if(not $disk_full_fh
	   or
	   tell $disk_full_fh == $pos) {
	    ::error("Output is incomplete. Cannot append to buffer file in \$TMPDIR. Is the disk full?\n");
	    ::error("Change \$TMPDIR with --tmpdir.\n");
	    ::wait_and_exit(255);
	}
	truncate $disk_full_fh, $pos;
    }
}


package CommandLine;

sub new {
    my $class = shift;
    my $seq = shift;
    my $command = ::undef_as_empty(shift);
    my $arg_queue = shift;
    my $context_replace = shift;
    my $max_number_of_args = shift; # for -N and normal (-N1)
    my $return_files = shift;
    my $len = {
	'{}' => 0, # Total length of all {} replaced with all args
	'{/}' => 0, # Total length of all {/} replaced with all args
	'{//}' => 0, # Total length of all {//} replaced with all args
	'{.}' => 0, # Total length of all {.} replaced with all args
	'{/.}' => 0, # Total length of all {/.} replaced with all args
	'no_args' => undef, # Length of command w/ all replacement args removed
	'context' => undef, # Length of context of an additional arg
    };
    my($sum,%replacecount);
    ($sum,$len->{'no_args'},$len->{'context'},$len->{'contextgroups'},
     %replacecount) = number_of_replacements($command,$context_replace);
    if($sum == 0) {
	if($command eq "") {
	    $command = $Global::replace{'{}'};
	} elsif($opt::pipe) {
	    # With --pipe you can have ::: or not
	    if(@opt::a) {
		$command .=" ".$Global::replace{'{}'};
	    }
        } else {
	    # Add {} to the command if there are no {...}'s
	    $command .=" ".$Global::replace{'{}'};
	}
	($sum,$len->{'no_args'},$len->{'context'},$len->{'contextgroups'},
	 %replacecount) = number_of_replacements($command,$context_replace);
    }
    if(defined $opt::tagstring) {
	my ($dummy1,$dummy2,$dummy3,$dummy4,%repcount) =
	    number_of_replacements($opt::tagstring,$context_replace);
	# Merge %repcount with %replacecount to get the keys
	# for replacing replacement strings in $opt::tagstring
	# The number, however, does not matter.
	for (keys %repcount) {
	    $replacecount{$_} ||= 0;
	}
    }

    my %positional_replace;
    my %multi_replace;
    for my $used (keys %replacecount) {
	if($used =~ /^{(\d+)(\D*)}$/) {
	    $positional_replace{$1} = '\{'.$2.'\}';
	} else {
	    $multi_replace{$used} = $used;
	}
    }
    return bless {
	'command' => $command,
	'seq' => $seq,
	'len' => $len,
	'arg_list' => [],
	'arg_queue' => $arg_queue,
	'max_number_of_args' => $max_number_of_args,
	'replacecount' => \%replacecount,
	'context_replace' => $context_replace,
	'return_files' => $return_files,
	'positional_replace' => \%positional_replace,
	'multi_replace' => \%multi_replace,
	'replaced' => undef,
    }, ref($class) || $class;
}

sub seq {
    my $self = shift;
    return $self->{'seq'};
}

sub populate {
    # Add arguments from arg_queue until the number of arguments or
    # max line length is reached
    # Returns: N/A
    my $self = shift;
    my $next_arg;
    while (not $self->{'arg_queue'}->empty()) {
	$next_arg = $self->{'arg_queue'}->get();
	if(not defined $next_arg) {
	    next;
	}
	$self->push($next_arg);
	if($self->len() >= Limits::Command::max_length()) {
	    # Command length is now > max_length
	    # If there are arguments: remove the last
	    # If there are no arguments: Error
	    # TODO stuff about -x opt_x
	    if($self->number_of_args() > 1) {
		# There is something to work on
		$self->{'arg_queue'}->unget($self->pop());
		last;
	    } else {
		my $args = join(" ", map { $_->orig() } @$next_arg);
		::error("Command line too long (", 
			$self->len(), " >= ",
			Limits::Command::max_length(),
			") at number ",
			$self->{'arg_queue'}->arg_number(),
			": ".
			(substr($args,0,50))."...\n");
		$self->{'arg_queue'}->unget($self->pop());
		::wait_and_exit(255);
	    }
	}

	if(defined $self->{'max_number_of_args'}) {
	    if($self->number_of_args() >= $self->{'max_number_of_args'}) {
		last;
	    }
	}
    }
    if(($opt::m or $opt::X) and not $CommandLine::already_spread
       and $self->{'arg_queue'}->empty() and $Global::max_jobs_running) {
	# -m or -X and EOF => Spread the arguments over all jobslots
	# (unless they are already spread)
	$CommandLine::already_spread ||= 1;
	if($self->number_of_args() > 1) {
	    $self->{'max_number_of_args'} =
		::ceil($self->number_of_args()/$Global::max_jobs_running);
	    $Global::JobQueue->{'commandlinequeue'}->{'max_number_of_args'} =
		$self->{'max_number_of_args'};
	    $self->{'arg_queue'}->unget($self->pop_all());
	    while($self->number_of_args() < $self->{'max_number_of_args'}) {
		$self->push($self->{'arg_queue'}->get());
	    }
	}
    }
}

sub push {
    # Add one or more records as arguments
    # Returns: N/A
    my $self = shift;
    my $record = shift;
    push @{$self->{'arg_list'}}, $record;
    #::my_dump($record);
    my $arg_no = ($self->number_of_args()-1) * ($#$record+1);

    for my $arg (@$record) {
	$arg_no++;
	if(defined $arg) {
	    if($self->{'positional_replace'}{$arg_no}) {
		# TODO probably bug here if both {1.} and {1} are used
		for my $used (keys %{$self->{'replacecount'}}) {
		    # {} {/} {//} {.} or {/.}
		    my $replacementfunction =
			$self->{'positional_replace'}{$arg_no};
		    # Find the single replacements
		    $self->{'len'}{$used} +=
			length $arg->replace($replacementfunction,1);
		}
	    }
	    for my $used (keys %{$self->{'multi_replace'}}) {
		# Add to the multireplacement
		$self->{'len'}{$used} += length $arg->replace($used,1);
	    }
	}
    }
}

sub pop {
    # Remove last argument
    # Returns:
    #   the last record
    my $self = shift;
    my $record = pop @{$self->{'arg_list'}};
    for my $arg (@$record) {
	if(defined $arg) {
	    for my $replacement_string (keys %{$self->{'replacecount'}}) {
		$self->{'len'}{$replacement_string} -=
		    length $arg->replace($replacement_string,1);
	    }
	}
    }
    return $record;
}

sub pop_all {
    # Remove all arguments and zeros the length of replacement strings
    # Returns:
    #   all records
    my $self = shift;
    my @popped = @{$self->{'arg_list'}};
    for my $replacement_string (keys %{$self->{'replacecount'}}) {
	$self->{'len'}{$replacement_string} = 0;
    }
    $self->{'arg_list'} = [];
    return @popped;
}

sub number_of_args {
    # The number of records
    # Returns:
    #   number of records
    my $self = shift;
    # Ftq rudef oaawuq ime dqxqmeqp az 2011-01-24 mzp ime iaz nk MQhmd
    # Mdzrvadp Nvmdymeaz az 2011-04-10. Ftue oaawuq dqxqmeqp az
    # 2013-08-18 ue m nuf tmdpqd me kag tmhq fa geq daf14. Bxqmeq
    # qymux oaawuq@fmzsq.pw itqz kag dqmp ftue.
    # 
    # U my ftq ymuzfmuzqd ar m buqoq ar rdqq earfimdq omxxqp SZG
    # Bmdmxxqx. Rdqq earfimdq sgmdmzfqqe kag mooqee fa ftq eagdoq
    # oapq, ngf U tmhq nqqz iazpqduzs tai ymzk mofgmxxk _dqmp_ ftq
    # eagdoq oapq.
    # 
    # Fa fqef ftue U bgf uz m oayyqzf fqxxuzs bqabxq fa qymux yq itqz
    # ftqk dqmp ftue. Ftq oayyqzf ime bgf uz m eqofuaz ar ftq oapq
    # ftmf za azq iagxp xaaw fa ruj ad uybdahq ftq earfimdq - ea ftq
    # eagdoq oapq qcguhmxqzf fa m pgefk oadzqd. Fa ymwq egdq ftq
    # oayyqzf iagxp zaf etai gb ur eayq azq vgef sdqbbqp ftdagst ftq
    # eagdoq oapq U daf13'qp ftq eagdoq oapq
    # tffb://qz.iuwubqpum.ads/iuwu/DAF13
    # 
    # 2.5 yazfte xmfqd U dqoquhqp mz qymux rday eayqazq ita zaf azxk
    # ymzmsqp fa ruzp ftq oayyqzf, ngf mxea ymzmsqp fa sgqee ftq oapq
    # tmp fa nq daf13'qp.
    # 
    # Ftue nduzse yq fa ftq oazoxgeuaz ftmf ftqdq _mdq_ bqabxq, ita
    # mdq zaf mrruxumfqp iuft ftq bdavqof, ftmf iuxx dqmp ftq eagdoq
    # oapq - ftagst uf ymk zaf tmbbqz hqdk arfqz.
    #
    # This is really the number of records
    return $#{$self->{'arg_list'}}+1;
}

sub args_as_string {
    # Returns:
    #  all unmodified arguments joined with ' ' (similar to {})
    my $self = shift;
    return (join " ", map { $_->orig() }
	    map { @$_ } @{$self->{'arg_list'}});
}

sub args_as_dirname {
    # Returns:
    #  all unmodified arguments joined with '/' (similar to {})
    #  \t \0 \\ and / are quoted
    my $self = shift;
    my @res = ();

    for my $rec_ref (@{$self->{'arg_list'}}) {
	my @header_indexes_sorted = header_indexes_sorted($#$rec_ref+1);

	for my $n (@header_indexes_sorted) {
	    CORE::push(@res,
		 $Global::input_source_header{$n},
		 map { my $s = $_;
		       $s =~ s/\\/\\\\/g;
		       $s =~ s/\t/\\t/g;
		       $s =~ s/\0/\\0/g;
		       $s =~ s:/:\\_:g; $s; }
		 $rec_ref->[$n-1]->orig());
	}
    }
    return join "/", @res;
}

sub header_indexes_sorted {
    # Sort headers first by number then by name.
    # E.g.: 1a 1b 11a 11b
    # Returns:
    #  Indexes of %Global::input_source_header sorted
    my $max_col = shift;
    
    no warnings 'numeric';
    for my $col (1 .. $max_col) {
	# Make sure the header is defined. If it is not: use column number
	if(not defined $Global::input_source_header{$col}) {
	    $Global::input_source_header{$col} = $col;
	}
    }
    my @header_indexes_sorted = sort {
	# Sort headers numerically then asciibetically
	$Global::input_source_header{$a} <=> $Global::input_source_header{$b}
	or
	    $Global::input_source_header{$a} cmp $Global::input_source_header{$b}
    } 1 .. $max_col;
    return @header_indexes_sorted;
}

sub len {
    # The length of the command line with args substituted
    my $self = shift;
    my $len = 0;
    # Add length of the original command with no args
    $len += $self->{'len'}{'no_args'};
    if($self->{'context_replace'}) {
	$len += $self->number_of_args()*$self->{'len'}{'context'};
	for my $replstring (keys %{$self->{'replacecount'}}) {
	    if(defined $self->{'len'}{$replstring}) {
		$len += $self->{'len'}{$replstring} *
		    $self->{'replacecount'}{$replstring};
	    }
	}
	$len += ($self->number_of_args()-1) * $self->{'len'}{'contextgroups'};
    } else {
	# Each replacement string may occur several times
	# Add the length for each time
	for my $replstring (keys %{$self->{'replacecount'}}) {
	    if(defined $self->{'len'}{$replstring}) {
		$len += $self->{'len'}{$replstring} *
		    $self->{'replacecount'}{$replstring};
	    }
	    if($Global::replace{$replstring}) {
		# This is a multi replacestring ({} {/} {//} {.} {/.})
		# Add each space between two arguments
		my $number_of_args = ($#{$self->{'arg_list'}[0]}+1) *
		    $self->number_of_args();
		$len += ($number_of_args-1) *
		    $self->{'replacecount'}{$replstring};
	    }
	}
    }
    if($opt::nice) {
	# Pessimistic length if --nice is set
	# Worse than worst case: every char needs to be quoted with \
	$len *= 2;
    }
    if($opt::shellquote) {
	# Pessimistic length if --shellquote is set
	# Worse than worst case: every char needs to be quoted with \ twice
	$len *= 4;
    }
    # If we are using --env, add the prefix for that, too.
    $len += $Global::envvarlen;
    return $len;
}

sub multi_regexp {
    if(not $CommandLine::multi_regexp) {
	$CommandLine::multi_regexp =
	"(?:".
	join("|",map {my $a=$_; $a =~ s/(\W)/\\$1/g; $a}
	     ($Global::replace{"{}"},
	      $Global::replace{"{.}"},
	      $Global::replace{"{/}"},
	      $Global::replace{"{//}"},
	      $Global::replace{"{/.}"})
	).")";
    }
    return $CommandLine::multi_regexp;
}

sub number_of_replacements {
    # Returns:
    #  sum_of_count, length_of_command_with_no_args,
    #  length_of_context { 'replacementstring' => count }
    my $command = shift;
    my $context_replace = shift;
    my %count = ();
    my $sum = 0;
    my $cmd = $command;
    my $multi_regexp = multi_regexp();
    my $replacement_regexp =
	"(?:". ::maybe_quote('\{') .
	'-?\d+(?:|\.|/\.|/|//)?' . # {n} {n.} {n/.} {n/} {n//} {-n} {-n.} {-n/.} {-n/} {-n//}
	::maybe_quote('\}') .
	'|'.
	join("|",map {$a=$_;$a=~s/(\W)/\\$1/g; $a} values %Global::replace).
	")";
    my %c = ();
    $cmd =~ s/($replacement_regexp)/$c{$1}++;"\0"/ogex;
    for my $k (keys %c) {
	if(defined $Global::replace_rev{$k}) {
	    $count{$Global::replace_rev{$k}} = $c{$k};
	} else {
	    $count{::maybe_unquote($k)} = $c{$k};
	}
	$sum += $c{$k};
    }
    my $number_of_context_groups = 0;
    my $no_args;
    my $context;
    if($context_replace) {
	$cmd = $command;
	while($cmd =~ s/\S*$multi_regexp\S*//o) {
	    $number_of_context_groups++;
	}
	$no_args = length $cmd;
	$context = length($command) - $no_args;
    } else {
	$cmd = $command;
	$cmd =~ s/$multi_regexp//go;
	$cmd =~ s/$replacement_regexp//go;
	$no_args = length($cmd);
	$context = length($command) - $no_args;
    }
    for my $k (keys %count) {
	if(defined $Global::replace{$k}) {
	    # {} {/} {//} {.} {/.} {#}
	    $context -= (length $Global::replace{$k}) * $count{$k};
	} else {
	    # {n}
	    $context -= (length $k) * $count{$k};
	}
    }
    return ($sum,$no_args,$context,$number_of_context_groups,%count);
}

sub replaced {
    my $self = shift;
    if(not defined $self->{'replaced'}) {
	$self->{'replaced'} = $self->replace_placeholders($self->{'command'},$Global::JobQueue->quote_args());
	if($self->{'replaced'} =~ /^\s*(-\S+)/) {
	    # Is this really a command in $PATH starting with '-'?
	    my $cmd = $1;
	    if(not which($cmd)) {
		::error("Command ($cmd) starts with '-'. Is this a wrong option?\n");
		::wait_and_exit(255);
	    }
	}
	if($opt::nice) {
	    # Prepend \nice -n19 $SHELL -c
	    # and quote
	    # \ before nice is needed to avoid tcsh's built-in
	    $self->{'replaced'} = '\nice' ." -n" . $opt::nice . " "
		. $ENV{SHELL}." -c "
		. ::shell_quote_scalar($self->{'replaced'});
	}
	if($opt::shellquote) {
	    # Prepend echo
	    # and quote twice
	    $self->{'replaced'} = "echo " .
		::shell_quote_scalar(::shell_quote_scalar($self->{'replaced'}));
	}
    }
    return $self->{'replaced'};
}

sub replace_placeholders {
    my $self = shift;
    my $target = shift;
    my $quote = shift;
    my $context_replace = $self->{'context_replace'};
    my $replaced;
    if($self->{'context_replace'}) {
	$replaced = $self->context_replace_placeholders($target,$quote);
    } else {
	$replaced = $self->simple_replace_placeholders($target,$quote);
    }
    return $replaced;
}

sub context_replace_placeholders {
    # Replace foo{}bar with fooargbar 
    # Input:
    #   target = foo{}bar
    #   quote = should this be quoted?
    # Returns: $target
    my $self = shift;
    my $target = shift;
    my $quote = shift;
    # -X = context replace
    # maybe multiple input sources
    # maybe --xapply
    # $self->{'arg_list'} = [ [Arg11, Arg12], [Arg21, Arg22], [Arg31, Arg32] ]

    my @args=();
    my @used_multi;
    my %replace;

    for my $record (@{$self->{'arg_list'}}) {
	# Merge arguments from records into args for easy access
	CORE::push @args, @$record;
    }

    # Replacement functions
    my @rep = qw({} {/} {//} {.} {/.});
    # Inner part of replacement functions
    my @rep_inner = ('', '/', '//', '.', '/.');
    # Regexp for replacement functions
    my $rep_regexp = "(?:". join('|', map { my $s = $_; $s =~ s/(\W)/\\$1/g; $s } @rep) . ")";
    # Regexp for inner replacement functions
    my $rep_inner_regexp = "(?:". join('|', map { my $s = $_; $s =~ s/(\W)/\\$1/g; $s } @rep_inner) . ")";
    # Seq replace string: {#}
    my $rep_seq_regexp = '(?:'.::maybe_quote('\{\#\}').")";
    # Normal replace strings
    my $rep_str_regexp = multi_regexp();
    # Positional replace strings
    my $rep_str_pos_regexp = ::maybe_quote('\{').'-?\d+'.$rep_inner_regexp.::maybe_quote('\}');

    # Fish out the words that have replacement strings in them
    my $tt = $target;
    my %word;
    while($tt =~ s/(\S*(?:$rep_str_regexp|$rep_str_pos_regexp|$rep_seq_regexp)\S*)/\0/o) {
	$word{$1} ||= 1;
    }
    if(not %word) {
	# The line did no contain any replacement strings => return unchanged
	return $target;	
    }
    # For each word: Generate the replacement string for that word.
    for my $origword (keys %word) {
	my @pos_replacements=();
	my @replacements=();
	my $w;
	my $word = $origword; # Make a local modifyable copy

	# replace {#} if it exists
	$word =~ s/$rep_seq_regexp/$self->seq()/geo;
	if($word =~ /$rep_str_pos_regexp/o) {
	    # There are positional replacement strings
	    my @argset;
	    if($#{$self->{'arg_list'}->[0]} == 0) {
		# Only one input source: Treat it as a set
		@argset = [ @args ];
	    } else {
		# Multiple input sources
		@argset = @{$self->{'arg_list'}};
	    }
	    # Match -n..-1,1..n where n = max args in a argset
	    my $n = $#{$argset[0]}+1;
	    my $pos_regexp = "(?:".join("|", -$n..-1, 1..$n).")";
	    my $pos_inner_regexp = ::maybe_quote('\{') .
		"($pos_regexp)($rep_inner_regexp)" .
		::maybe_quote('\}');
	    for my $argset (@argset) {
		# Replace all positional arguments - e.g. {7/.}
		# with the replacement function - e.g. {/.}
		# of that argument
		if(defined $self->{'max_number_of_args'}) {
		    # Fill up if we have a half completed line, so {n} will be empty
		    while($#$argset < $self->{'max_number_of_args'}) {
			CORE::push @$argset, Arg->new("");
		    }
		}
		$w = $word;
		# Replace positive replacement strings with arg[$1-1]
		# Replace negative replacement strings with arg[$n+$1]
		$w =~ s/$pos_inner_regexp/$argset->[$1 > 0 ? $1-1 : $n+$1]->replace('{'.$2.'}',$quote)/geo;
		CORE::push @pos_replacements, $w;
	    }
	}
	if(not @pos_replacements) {
	    @pos_replacements = ($word);
	}

	if($word =~ m:$rep_str_regexp:) {
	    # There are normal replacement strings
	    for my $w (@pos_replacements) {
		for my $arg (@args) {
		    my $wmulti = $w;
		    $wmulti =~ s/($rep_str_regexp)/$arg->replace($Global::replace_rev{$1},$quote)/geo;
		    CORE::push @replacements, $wmulti;
		}
	    }
	}
	if(@replacements) {
	    CORE::push @{$replace{$origword}}, @replacements;
	} else {
	    CORE::push @{$replace{$origword}}, @pos_replacements;
	}
    }
    # Substitute the replace strings with the replacement values
    # Must be sorted by length if a short word is a substring of a long word
    my $regexp = join('|', map { my $s = $_; $s =~ s/(\W)/\\$1/g; $s }
		      sort { length $b <=> length $a } keys %word);
    $target =~ s/($regexp)/join(" ",@{$replace{$1}})/ge;
    return $target;
}

sub simple_replace_placeholders {
    # no context (no -X)
    # maybe multiple input sources
    # maybe --xapply
    my $self = shift;
    my $target = shift;
    my $quote = shift;
    my @args=();
    my @used_multi;
    my %replace;

    for my $record (@{$self->{'arg_list'}}) {
	# Merge arguments from records into args for easy access
	CORE::push @args, @$record;
    }
    my $n = $#args+1;
    # Which replace strings are used?
    # {#} {} {/} {//} {.} {/.} {n} {n/} {n//} {n.} {n/.}
    for my $used (keys %{$self->{'replacecount'}}) {
	# What are the replacement values for the replace strings?
	if(grep { $used eq $_ } qw({} {/} {//} {.} {/.})) {
	    # {} {/} {//} {.} {/.}
	    $replace{$Global::replace{$used}} =
		join(" ", map { $_->replace($used,$quote) } @args);
	} elsif($used =~ /^\{(-?\d+)(|\/|\/\/|\.|\/\.)\}$/) {
	    # {n} {n/} {n//} {n.} {n/.} {-n} {-n/} {-n//} {-n.} {-n/.}
	    my $positional = $1 > 0 ? $1 : $n+$1+1;
	    my $replacementfunction = "{".::undef_as_empty($2)."}"; # {} {/} {//} {.} or {/.}
	    # If -q then the replacementstrings will be quoted, too
	    # {1.} -> \{1.\}
	    $Global::replace{$used} ||= ::maybe_quote($used);
	    if(defined $args[$positional-1]) {
		# we have a matching argument for {n}
		$replace{$Global::replace{$used}} =
		    $args[$positional-1]->replace($replacementfunction,$quote);
	    } else {
		if($positional <= $self->{'max_number_of_args'}) {
		    # Fill up if we have a half completed line
		    $replace{$Global::replace{$used}} = "";
		}
	    }
	} elsif($used eq "{#}") {
	    # {#}
	    $replace{$Global::replace{$used}} = $self->seq();
	} else {
	    ::die_bug('simple_replace_placeholders_20110530');
	}
    }
    # Substitute the replace strings with the replacement values
    my $regexp = join('|', map { my $s = $_; $s =~ s/(\W)/\\$1/g; $s } keys %replace);
    if($regexp) {
	$target =~ s/($regexp)/$replace{$1}/g;
    }
    return $target;
}


package CommandLineQueue;

sub new {
    my $class = shift;
    my $command = shift;
    my $read_from = shift;
    my $context_replace = shift;
    my $max_number_of_args = shift;
    my $return_files = shift;
    my @unget = ();
    return bless {
	'unget' => \@unget,
	'command' => $command,
	'arg_queue' => RecordQueue->new($read_from,$opt::colsep),
	'context_replace' => $context_replace,
	'max_number_of_args' => $max_number_of_args,
	'size' => undef,
	'return_files' => $return_files,
	'seq' => 1,
    }, ref($class) || $class;
}

sub get {
    my $self = shift;
    if(@{$self->{'unget'}}) {
	my $cmd_line = shift @{$self->{'unget'}};
	return ($cmd_line);
    } else {
	my $cmd_line;
	$cmd_line = CommandLine->new($self->seq(),
				     $self->{'command'},
				     $self->{'arg_queue'},
				     $self->{'context_replace'},
				     $self->{'max_number_of_args'},
				     $self->{'return_files'},
	    );
	$cmd_line->populate();
	::debug("cmd_line->number_of_args ".$cmd_line->number_of_args()."\n");
	if($opt::pipe) {
	    if($cmd_line->replaced() eq "") {
		# Empty command - pipe requires a command
		::error("--pipe must have a command to pipe into (e.g. 'cat').\n");
		::wait_and_exit(255);
	    }
	} else {
	    if($cmd_line->number_of_args() == 0) {
		# We did not get more args - maybe at EOF string?
		return undef;
	    } elsif($cmd_line->replaced() eq "") {
		# Empty command - get the next instead
		return $self->get();
	    }
	}
	$self->set_seq($self->seq()+1);
	return $cmd_line;
    }
}

sub unget {
    my $self = shift;
    unshift @{$self->{'unget'}}, @_;
}

sub empty {
    my $self = shift;
    my $empty = (not @{$self->{'unget'}}) && $self->{'arg_queue'}->empty();
    ::debug("CommandLineQueue->empty $empty");
    return $empty;
}

sub seq {
    my $self = shift;
    return $self->{'seq'};
}

sub set_seq {
    my $self = shift;
    $self->{'seq'} = shift;
}

sub quote_args {
    my $self = shift;
    # If there is not command emulate |bash
    return $self->{'command'};
}

sub size {
    my $self = shift;
    if(not $self->{'size'}) {
	my @all_lines = ();
	while(not $self->{'arg_queue'}->empty()) {
	    push @all_lines, CommandLine->new($self->{'command'},
					      $self->{'arg_queue'},
					      $self->{'context_replace'},
					      $self->{'max_number_of_args'});
	}
	$self->{'size'} = @all_lines;
	$self->unget(@all_lines);
    }
    return $self->{'size'};
}


package Limits::Command;

# Maximal command line length (for -m and -X)
sub max_length {
    # Find the max_length of a command line and cache it
    # Returns:
    #   number of chars on the longest command line allowed
    if(not $Limits::Command::line_max_len) {
	# Disk cache of max command line length 
	my $len_cache = $ENV{'HOME'} . "/.parallel/tmp/linelen-" . ::hostname();
	my $cached_limit;
	if(-e $len_cache) {
	    open(my $fh, "<", $len_cache) || ::die_bug("Cannot read $len_cache");
	    $cached_limit = <$fh>;
	    close $fh;
	} else {
	    $cached_limit = real_max_length();
	    # If $HOME is write protected: Do not fail
	    mkdir($ENV{'HOME'} . "/.parallel");
	    mkdir($ENV{'HOME'} . "/.parallel/tmp");
	    open(my $fh, ">", $len_cache);
	    print $fh $cached_limit;
	    close $fh;
	}
	$Limits::Command::line_max_len = $cached_limit;
	if($opt::max_chars) {
	    if($opt::max_chars <= $cached_limit) {
		$Limits::Command::line_max_len = $opt::max_chars;
	    } else {
		::warning("Value for -s option ",
			  "should be < $cached_limit.\n");
	    }
	}
    }
    return $Limits::Command::line_max_len;
}

sub real_max_length {
    # Find the max_length of a command line
    # Returns:
    #   The maximal command line length
    # Use an upper bound of 8 MB if the shell allows for for infinite long lengths
    my $upper = 8_000_000;
    my $len = 8;
    do {
	if($len > $upper) { return $len };
	$len *= 16;
    } while (is_acceptable_command_line_length($len));
    # Then search for the actual max length between 0 and upper bound
    return binary_find_max_length(int($len/16),$len);
}

sub binary_find_max_length {
    # Given a lower and upper bound find the max_length of a command line
    # Returns:
    #   number of chars on the longest command line allowed
    my ($lower, $upper) = (@_);
    if($lower == $upper or $lower == $upper-1) { return $lower; }
    my $middle = int (($upper-$lower)/2 + $lower);
    ::debug("Maxlen: $lower,$upper,$middle : ");
    if (is_acceptable_command_line_length($middle)) {
	return binary_find_max_length($middle,$upper);
    } else {
	return binary_find_max_length($lower,$middle);
    }
}

sub is_acceptable_command_line_length {
    # Test if a command line of this length can run
    # Returns:
    #   0 if the command line length is too long
    #   1 otherwise
    my $len = shift;

    local *STDERR;
    open (STDERR, ">", "/dev/null");
    system "true "."x"x$len;
    close STDERR;
    ::debug("$len=$? ");
    return not $?;
}


package RecordQueue;

sub new {
    my $class = shift;
    my $fhs = shift;
    my $colsep = shift;
    my @unget = ();
    my $arg_sub_queue;
    if($colsep) {
	# Open one file with colsep
	$arg_sub_queue = RecordColQueue->new($fhs);
    } else {
	# Open one or more files if multiple -a
	$arg_sub_queue = MultifileQueue->new($fhs);
    }
    return bless {
	'unget' => \@unget,
	'arg_number' => 0,
	'arg_sub_queue' => $arg_sub_queue,
    }, ref($class) || $class;
}

sub get {
    # Returns:
    #   reference to array of Arg-objects
    my $self = shift;
    if(@{$self->{'unget'}}) {
	return shift @{$self->{'unget'}};
    }
    $self->{'arg_number'}++;
    my $ret = $self->{'arg_sub_queue'}->get();
    if(defined $Global::max_number_of_args
       and $Global::max_number_of_args == 0) {
	::debug("Read 1 but return 0 args\n");
	return [Arg->new("")];
    } else {
	return $ret;
    }
}

sub unget {
    my $self = shift;
    ::debug("RecordQueue-unget '@_'\n");
    $self->{'arg_number'}--;
    unshift @{$self->{'unget'}}, @_;
}

sub empty {
    my $self = shift;
    my $empty = not @{$self->{'unget'}};
    $empty &&= $self->{'arg_sub_queue'}->empty();
    ::debug("RecordQueue->empty $empty");
    return $empty;
}

sub arg_number {
    my $self = shift;
    return $self->{'arg_number'};
}


package RecordColQueue;

sub new {
    my $class = shift;
    my $fhs = shift;
    my @unget = ();
    my $arg_sub_queue = MultifileQueue->new($fhs);
    return bless {
	'unget' => \@unget,
	'arg_sub_queue' => $arg_sub_queue,
    }, ref($class) || $class;
}

sub get {
    # Returns:
    #   reference to array of Arg-objects
    my $self = shift;
    if(@{$self->{'unget'}}) {
	return shift @{$self->{'unget'}};
    }
    my $unget_ref=$self->{'unget'};
    if($self->{'arg_sub_queue'}->empty()) {
	return undef;
    }
    my $in_record = $self->{'arg_sub_queue'}->get();
    if(defined $in_record) {
	my @out_record = ();
	for my $arg (@$in_record) {
	    ::debug("RecordColQueue::arg $arg\n");
	    my $line = $arg->orig();
	    ::debug("line='$line'\n");
	    if($line ne "") {
		for my $s (split /$opt::colsep/o, $line, -1) {
		    push @out_record, Arg->new($s);
		}
	    } else {
		push @out_record, Arg->new("");
	    }
	}
	return \@out_record;
    } else {
	return undef;
    }
}

sub unget {
    my $self = shift;
    ::debug("RecordColQueue-unget '@_'\n");
    unshift @{$self->{'unget'}}, @_;
}

sub empty {
    my $self = shift;
    my $empty = (not @{$self->{'unget'}} and $self->{'arg_sub_queue'}->empty());
    ::debug("RecordColQueue->empty $empty");
    return $empty;
}


package MultifileQueue;

@Global::unget_argv=();

sub new {
    my $class = shift;
    my $fhs = shift;
    for my $fh (@$fhs) {
	if(-t $fh) {
	    ::warning("Input is read from the terminal. ".
		      "Only experts do this on purpose. ".
		      "Press CTRL-D to exit.\n");
	}
    }
    return bless {
	'unget' => \@Global::unget_argv,
	'fhs' => $fhs,
	'arg_matrix' => undef,
    }, ref($class) || $class;
}

sub get {
    my $self = shift;
    if($opt::xapply) {
	return $self->xapply_get();
    } else {
	return $self->nest_get();
    }
}

sub unget {
    my $self = shift;
    ::debug("MultifileQueue-unget '@_'\n");
    unshift @{$self->{'unget'}}, @_;
}

sub empty {
    my $self = shift;
    my $empty = (not @Global::unget_argv
		 and not @{$self->{'unget'}});
    for my $fh (@{$self->{'fhs'}}) {
	$empty &&= eof($fh);
    }
    ::debug("MultifileQueue->empty $empty");
    return $empty;
}

sub xapply_get {
    my $self = shift;
    if(@{$self->{'unget'}}) {
	return shift @{$self->{'unget'}};
    }
    my @record = ();
    my $prepend = undef;
    my $empty = 1;
    for my $fh (@{$self->{'fhs'}}) {
	my $arg = read_arg_from_fh($fh);
	if(defined $arg) {
	    # Record $arg for recycling at end of file
	    push @{$self->{'arg_matrix'}{$fh}}, $arg;
	    push @record, $arg;
	    $empty = 0;
	} else {
	    ::debug("EOA");
	    # End of file: Recycle arguments
	    push @{$self->{'arg_matrix'}{$fh}}, shift @{$self->{'arg_matrix'}{$fh}};
	    # return last @{$args->{'args'}{$fh}};
	    push @record, @{$self->{'arg_matrix'}{$fh}}[-1];
	}
    }
    if($empty) {
	return undef;
    } else {
	return \@record;
    }
}

sub nest_get {
    my $self = shift;
    if(@{$self->{'unget'}}) {
	return shift @{$self->{'unget'}};
    }
    my @record = ();
    my $prepend = undef;
    my $empty = 1;
    my $no_of_inputsources = $#{$self->{'fhs'}} + 1;
    if(not $self->{'arg_matrix'}) {
	# Initialize @arg_matrix with one arg from each file
	# read one line from each file
	my @first_arg_set;
	my $all_empty = 1;
	for (my $fhno = 0; $fhno < $no_of_inputsources ; $fhno++) {
	    my $arg = read_arg_from_fh($self->{'fhs'}[$fhno]);
	    if(defined $arg) {
		$all_empty = 0;
	    }
	    $self->{'arg_matrix'}[$fhno][0] = $arg || Arg->new("");
	    push @first_arg_set, $self->{'arg_matrix'}[$fhno][0];
	}
	if($all_empty) {
	    # All filehandles were at eof or eof-string
	    return undef;
	}
	return [@first_arg_set];
    }

    # Treat the case with one input source special.  For multiple
    # input sources we need to remember all previously read values to
    # generate all combinations. But for one input source we can
    # forget the value after first use.
    if($no_of_inputsources == 1) {
	my $arg = read_arg_from_fh($self->{'fhs'}[0]);
	if(defined($arg)) {
	    return [$arg];
	}
	return undef;
    }
    for (my $fhno = $no_of_inputsources - 1; $fhno >= 0; $fhno--) {
	if(eof($self->{'fhs'}[$fhno])) {
	    next;
	} else {
	    # read one
	    my $arg = read_arg_from_fh($self->{'fhs'}[$fhno]);
	    defined($arg) || next; # If we just read an EOF string: Treat this as EOF
	    my $len = $#{$self->{'arg_matrix'}[$fhno]} + 1;
	    $self->{'arg_matrix'}[$fhno][$len] = $arg;
	    # make all new combinations
	    my @combarg = ();
	    for (my $fhn = 0; $fhn < $no_of_inputsources; $fhn++) {
		push @combarg, [0, $#{$self->{'arg_matrix'}[$fhn]}];
	    }
	    $combarg[$fhno] = [$len,$len]; # Find only combinations with this new entry
	    # map combinations
	    # [ 1, 3, 7 ], [ 2, 4, 1 ]
	    # =>
	    # [ m[0][1], m[1][3], m[3][7] ], [ m[0][2], m[1][4], m[2][1] ]
	    my @mapped;
	    for my $c (expand_combinations(@combarg)) {
		my @a;
		for my $n (0 .. $no_of_inputsources - 1 ) {
		    push @a,  $self->{'arg_matrix'}[$n][$$c[$n]];
		}
		push @mapped, \@a;
	    }
	    # append the mapped to the ungotten arguments
	    push @{$self->{'unget'}}, @mapped;
	    # get the first
	    return shift @{$self->{'unget'}};
	}
    }
    # all are eof or at EOF string; return from the unget queue
    return shift @{$self->{'unget'}};
}

sub read_arg_from_fh {
    # Read one Arg from filehandle
    # Returns:
    #   Arg-object with one read line
    #   undef if end of file
    my $fh = shift;
    my $prepend = undef;
    my $arg;
    do {{
	if(eof($fh)) {
	    if(defined $prepend) {
		return Arg->new($prepend);
	    } else {
		return undef;
	    }
	}
	$arg = <$fh>;
	::debug("read $arg\n");
	# Remove delimiter
	$arg =~ s:$/$::;
	if($Global::end_of_file_string and
	   $arg eq $Global::end_of_file_string) {
	    # Ignore the rest of input file
	    while (<$fh>) {}
	    ::debug("EOF-string $arg\n");
	    if(defined $prepend) {
		return Arg->new($prepend);
	    } else {
		return undef;
	    }
	}
	if(defined $prepend) {
	    $arg = $prepend.$arg; # For line continuation
	    $prepend = undef; #undef;
	}
	if($Global::ignore_empty) {
	    if($arg =~ /^\s*$/) {
		redo; # Try the next line
	    }
	}
	if($Global::max_lines) {
	    if($arg =~ /\s$/) {
		# Trailing space => continued on next line
		$prepend = $arg;
		redo;
	    }
	}
    }} while (1 == 0); # Dummy loop for redo
    if(defined $arg) {
	return Arg->new($arg);
    } else {
	::die_bug("multiread arg undefined");
    }
}

sub expand_combinations {
    # Input:
    #   ([xmin,xmax], [ymin,ymax], ...)
    # Returns: ([x,y,...],[x,y,...])
    # where xmin <= x <= xmax and ymin <= y <= ymax
    my $minmax_ref = shift;
    my $xmin = $$minmax_ref[0];
    my $xmax = $$minmax_ref[1];
    my @p;
    if(@_) {
	# If there are more columns: Compute those recursively
	my @rest = expand_combinations(@_);
	for(my $x = $xmin; $x <= $xmax; $x++) {
	    push @p, map { [$x, @$_] } @rest;
	}
    } else {
	for(my $x = $xmin; $x <= $xmax; $x++) {
	    push @p, [$x];
	}
    }
    return @p;
}


package Arg;

sub new {
    my $class = shift;
    my $orig = shift;
    return bless {
	'orig' => $orig,
    }, ref($class) || $class;
}

sub replace {
    # Calculates the corresponding value for {} {/} {//} {.} {/.}
    # Returns:
    #   The calculated string
    my $self = shift;
    my $replacement_string = shift; # {} {/} {//} {.} {/.}
    my $quote = shift; # should the string be quoted?
    if(not defined $self->{$quote,$replacement_string}) {
	my $s;
	if($Global::trim eq "n") {
	    $s = $self->{'orig'};
	} else {
	    $s = trim_of($self->{'orig'});
	}
	if($replacement_string eq "{}") {
	    # skip
	} elsif($replacement_string eq "{.}") {
	    $s =~ s:\.[^/\.]*$::; # Remove .ext from argument
	} elsif($replacement_string eq "{/}") {
	    $s =~ s:^.*/([^/]+)/?$:$1:; # Remove dir from argument. If ending in /, remove final /
	} elsif($replacement_string eq "{//}") {
	    # Only load File::Basename if actually needed
	    $Global::use{"File::Basename"} ||= eval "use File::Basename; 1;";
	    $s = dirname($s); # Keep dir from argument.
	} elsif($replacement_string eq "{/.}") {
	    $s =~ s:^.*/([^/]+)/?$:$1:; # Remove dir from argument. If ending in /, remove final /
	    $s =~ s:\.[^/\.]*$::; # Remove .ext from argument
	}
	if($quote) {
	    $s = ::shell_quote_scalar($s);
	}
	$self->{$quote,$replacement_string} = $s;
    }
    return $self->{$quote,$replacement_string};
}

sub orig {
    my $self = shift;
    return $self->{'orig'};
}

sub trim_of {
    # Removes white space as specifed by --trim:
    # n = nothing
    # l = start
    # r = end
    # lr|rl = both
    # Returns:
    #   string with white space removed as needed
    my @strings = map { defined $_ ? $_ : "" } (@_);
    my $arg;
    if($Global::trim eq "n") {
	# skip
    } elsif($Global::trim eq "l") {
	for my $arg (@strings) { $arg =~ s/^\s+//; }
    } elsif($Global::trim eq "r") {
	for my $arg (@strings) { $arg =~ s/\s+$//; }
    } elsif($Global::trim eq "rl" or $Global::trim eq "lr") {
	for my $arg (@strings) { $arg =~ s/^\s+//; $arg =~ s/\s+$//; }
    } else {
	::error("--trim must be one of: r l rl lr.\n");
	::wait_and_exit(255);
    }
    return wantarray ? @strings : "@strings";
}


package TimeoutQueue;

sub new {
    my $class = shift;
    my $delta_time = shift;
    my ($pct);
    if($delta_time =~ /(\d+(\.\d+)?)%/) {
	# Timeout in percent
	$pct = $1/100;
	$delta_time = 1_000_000;
    }
    return bless {
	'queue' => [],
	'delta_time' => $delta_time,
	'pct' => $pct,
	'remedian_idx' => 0,
	'remedian_arr' => [],
	'remedian' => undef,
    }, ref($class) || $class;
}

sub delta_time {
    my $self = shift;
    return $self->{'delta_time'};
}

sub set_delta_time {
    my $self = shift;
    $self->{'delta_time'} = shift;
}

sub remedian {
    my $self = shift;
    return $self->{'remedian'};
}

sub set_remedian {
    # Set median of the last 999^3 (=997002999) values using Remedian 
    #
    # Rousseeuw, Peter J., and Gilbert W. Bassett Jr. "The remedian: A
    # robust averaging method for large data sets." Journal of the
    # American Statistical Association 85.409 (1990): 97-104.
    my $self = shift;
    my $val = shift;
    my $i = $self->{'remedian_idx'}++;
    my $rref = $self->{'remedian_arr'};
    $rref->[0][$i%999] = $val;
    $rref->[1][$i/999%999] = (sort @{$rref->[0]})[$#{$rref->[0]}/2];
    $rref->[2][$i/999/999%999] = (sort @{$rref->[1]})[$#{$rref->[1]}/2];
    $self->{'remedian'} = (sort @{$rref->[2]})[$#{$rref->[2]}/2];
#    die((sort @{$rref->[2]})[$#{$rref->[2]}/2]);
}

sub update_delta_time {
    # Update delta_time based on runtime of finished job if timeout is
    # a percentage
    my $self = shift;
    my $runtime = shift;
    if($self->{'pct'}) {
	$self->set_remedian($runtime);
	$self->{'delta_time'} = $self->{'pct'} * $self->remedian();
	::debug("Timeout: $self->{'delta_time'}s ");
    }
}

sub process_timeouts {
    # Check if there was a timeout
    my $self = shift;
    # $self->{'queue'} is sorted by start time
    while (@{$self->{'queue'}}) {
	my $job = $self->{'queue'}[0];
	if($job->endtime()) {
	    # Job already finished. No need to timeout the job
	    # This could be because of --keep-order
	    shift @{$self->{'queue'}};
	} elsif($job->timedout($self->{'delta_time'})) {
	    # Need to shift off queue before kill
	    # because kill calls usleep that calls process_timeouts
	    shift @{$self->{'queue'}};
	    $job->kill();
	} else {
	    # Because they are sorted by start time the rest are later
	    last;
	}
    }
}

sub insert {
    my $self = shift;
    my $in = shift;
    push @{$self->{'queue'}}, $in;
}


package Semaphore;

# This package provides a counting semaphore
#
# If a process dies without releasing the semaphore the next process
# that needs that entry will clean up dead semaphores
#
# The semaphores are stored in ~/.parallel/semaphores/id-<name> Each
# file in ~/.parallel/semaphores/id-<name>/ is the process ID of the
# process holding the entry. If the process dies, the entry can be
# taken by another process.

sub new {
    my $class = shift;
    my $id = shift;
    my $count = shift;
    $id=~s/([^-_a-z0-9])/unpack("H*",$1)/ige; # Convert non-word chars to hex
    $id="id-".$id; # To distinguish it from a process id
    my $parallel_dir = $ENV{'HOME'}."/.parallel";
    -d $parallel_dir or mkdir_or_die($parallel_dir);
    my $parallel_locks = $parallel_dir."/semaphores";
    -d $parallel_locks or mkdir_or_die($parallel_locks);
    my $lockdir = "$parallel_locks/$id";
    my $lockfile = $lockdir.".lock";
    if($count < 1) { ::die_bug("semaphore-count: $count"); }
    return bless {
	'lockfile' => $lockfile,
	'lockfh' => Symbol::gensym(),
	'lockdir' => $lockdir,
	'id' => $id,
	'idfile' => $lockdir."/".$id,
	'pid' => $$,
	'pidfile' => $lockdir."/".$$.'@'.::hostname(),
	'count' => $count + 1 # nlinks returns a link for the 'id-' as well
    }, ref($class) || $class;
}

sub acquire {
    my $self = shift;
    my $sleep = 1; # 1 ms
    my $start_time = time;
    while(1) {
	$self->atomic_link_if_count_less_than() and last;
	::debug("Remove dead locks");
	my $lockdir = $self->{'lockdir'};
	for my $d (glob "$lockdir/*") {
	    ::debug("Lock $d $lockdir\n");
	    $d =~ m:$lockdir/([0-9]+)\@([-\._a-z0-9]+)$:o or next;
	    my ($pid, $host) = ($1, $2);
	    if($host eq ::hostname()) {
		if(not kill 0, $1) {
		    ::debug("Dead: $d");
		    unlink $d;
		} else {
		    ::debug("Alive: $d");
		}
	    }
	}
	# try again
	$self->atomic_link_if_count_less_than() and last;
	# Retry slower and slower up to 1 second
	$sleep = ($sleep < 1000) ? ($sleep * 1.1) : ($sleep);
	# Random to avoid every sleeping job waking up at the same time
	::usleep(rand()*$sleep);
	if(defined($opt::timeout) and
	   $start_time + $opt::timeout > time) {
	    # Acquire the lock anyway
	    if(not -e $self->{'idfile'}) {
		open (my $fh, ">", $self->{'idfile'}) or
		    ::die_bug("timeout_write_idfile: $self->{'idfile'}");
		close $fh;
	    }
	    link $self->{'idfile'}, $self->{'pidfile'};
	    last;
	}
    }
    ::debug("acquired $self->{'pid'}\n");
}

sub release {
    my $self = shift;
    unlink $self->{'pidfile'};
    if($self->nlinks() == 1) {
	# This is the last link, so atomic cleanup
	$self->lock();
	if($self->nlinks() == 1) {
	    unlink $self->{'idfile'};
	    rmdir $self->{'lockdir'};
	}
	$self->unlock();
    }
    ::debug("released $self->{'pid'}\n");
}

sub atomic_link_if_count_less_than {
    # Link $file1 to $file2 if nlinks to $file1 < $count
    my $self = shift;
    my $retval = 0;
    $self->lock();
    ::debug($self->nlinks()."<".$self->{'count'});
    if($self->nlinks() < $self->{'count'}) {
	-d $self->{'lockdir'} or mkdir_or_die($self->{'lockdir'});
	if(not -e $self->{'idfile'}) {
	    open (my $fh, ">", $self->{'idfile'}) or
		::die_bug("write_idfile: $self->{'idfile'}");
	    close $fh;
	}
	$retval = link $self->{'idfile'}, $self->{'pidfile'};
    }
    $self->unlock();
    ::debug("atomic $retval");
    return $retval;
}

sub nlinks {
    my $self = shift;
    if(-e $self->{'idfile'}) {
	::debug("nlinks".((stat(_))[3])."\n");
	return (stat(_))[3];
    } else {
	return 0;
    }
}

sub lock {
    my $self = shift;
    my $sleep = 100; # 100 ms
    open $self->{'lockfh'}, ">", $self->{'lockfile'}
	or ::die_bug("Can't open semaphore file $self->{'lockfile'}: $!");
    chmod 0666, $self->{'lockfile'}; # assuming you want it a+rw
    $Global::use{"Fcntl"} ||= eval "use Fcntl qw(:DEFAULT :flock); 1;";
    while(not flock $self->{'lockfh'}, LOCK_EX()|LOCK_NB()) {
	if ($! =~ m/Function not implemented/) {
            ::warning("flock: $!");
	    ::warning("Will wait for a random while\n");
	    ::usleep(rand(5000));
	    last;
	}

	::debug("Cannot lock $self->{'lockfile'}");
	# TODO if timeout: last
	$sleep = ($sleep < 1000) ? ($sleep * 1.1) : ($sleep);
	# Random to avoid every sleeping job waking up at the same time
	::usleep(rand()*$sleep);
    }
    ::debug("locked $self->{'lockfile'}");
}

sub unlock {
    my $self = shift;
    unlink $self->{'lockfile'};
    close $self->{'lockfh'};
    ::debug("unlocked\n");
}

sub mkdir_or_die {
    # If dir is not writable: die
    my $dir = shift;
    my @dir_parts = split(m:/:,$dir);
    my ($ddir,$part);
    while(defined ($part = shift @dir_parts)) {
	$part eq "" and next;
	$ddir .= "/".$part;
	-d $ddir and next;
	mkdir $ddir;
    }
    if(not -w $dir) {
	::error("Cannot write to $dir: $!\n");
	::wait_and_exit(255);
    }
}

# Keep perl -w happy
$opt::x = $Semaphore::timeout = $Semaphore::wait = $opt::shebang = $Global::no_more_file_handles_warned =
$Job::file_descriptor_warning_printed = 0;
