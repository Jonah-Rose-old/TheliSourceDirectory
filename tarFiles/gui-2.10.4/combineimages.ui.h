#include <qdir.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qstring.h>
#include <qprocess.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include "functions.h"

QString combima_pid, instrument, editor;
int started_proc_combima, combima_errcount;
long combima_errline;
QProcess *proc_combima;
QString combima_concatenate_logs, combima_remove_individuallogs;
QString combima_fullpathlog;

void combineimages::validate()
{
  QString directory;
  
  // integer fields
  QRegExp ri( "^[-]{0,1}[0-9]+" );
  QValidator* validator_int = new QRegExpValidator( ri, this );
  combima_dminLineEdit->setValidator( validator_int );
  combima_nlowLineEdit->setValidator( validator_int );
  combima_nhighLineEdit->setValidator( validator_int );
  
  // detect_thresh and rescale level can also accept floats
  QRegExp rf( "^[-]{0,1}[0-9]*[.]{0,1}[0-9]+" );
  QValidator* validator_float = new QRegExpValidator( rf, this );
  combima_dtLineEdit->setValidator( validator_float );
  combima_levelLineEdit->setValidator( validator_float );
  
  // directories and similar ones must not accept blank characters
  QRegExp rx( "^\\S+$" );
  QValidator* validator_string = new QRegExpValidator( rx, this );
  combima_nameLineEdit->setValidator( validator_string );
  combima_filterLineEdit->setValidator( validator_string );
  combima_dirLineEdit->setValidator( validator_string );
  
  // check if the image directory exists, and paint it accordingly
  directory = combima_dirLineEdit->text();
  QDir dir = QDir(directory);
  
  if (dir.exists() && !directory.isEmpty()) 
    combima_dirLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
  if (!dir.exists() && !directory.isEmpty()) 
    combima_dirLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
  if (directory.isEmpty()) 
    combima_dirLineEdit->setPaletteBackgroundColor(QColor(255,255,255));
  
  // rescaling level
  if (combima_rescaleCheckBox->isChecked()) combima_levelLineEdit->show();
  if (!combima_rescaleCheckBox->isChecked()) combima_levelLineEdit->hide();  
}


void combineimages::clearfields()
{
  combima_dtLineEdit->clear();
  combima_dminLineEdit->clear();
  combima_nlowLineEdit->clear();
  combima_nhighLineEdit->clear();
  combima_nameLineEdit->clear();
  combima_filterLineEdit->clear();
  combima_dirLineEdit->clear();
  combima_statusLineEdit->clear();
  combima_levelLineEdit->clear();
  combima_rescaleCheckBox->setChecked(false);
  combima_methodComboBox->setCurrentItem(0);
  combima_statusLineEdit->setPaletteBackgroundColor(QColor(255,254,212));
}


void combineimages::compile_scriptsyntax()
{
  QString scriptstring, dir, filter, name, method;
  QString nlow, nhigh, dt, dmin, dirtmp, lastchar;
  QString rescale, level, imagelistfilter;
  int length, numfiles;
  
  dir    = combima_dirLineEdit->text();
  filter = combima_filterLineEdit->text();
  name   = combima_nameLineEdit->text();
  nlow   = combima_nlowLineEdit->text();
  nhigh  = combima_nhighLineEdit->text();
  dt     = combima_dtLineEdit->text();
  dmin   = combima_dminLineEdit->text();
  
  QDir imagedir = QDir(dir);
  imagelistfilter = "*";
  imagelistfilter.append(filter);
  imagelistfilter.append(".fits");
  numfiles = imagedir.entryList(imagelistfilter).count();
  
  if (numfiles == 0) {
    QMessageBox::warning( this, "No images found",
			  "No images found, check the directory path!\n");
    return;
  }
  
  if (nlow.isEmpty()) {
    nlow = "0";
    combima_nlowLineEdit->setText("0");
  }
  if (nhigh.isEmpty()) {
    nhigh = "1";
    combima_nhighLineEdit->setText("1");
  }
  if (dt.isEmpty() && !dmin.isEmpty()) {
    dt = "1.0";
    combima_dtLineEdit->setText("1.0");
  }
  if (dmin.isEmpty() && !dt.isEmpty()) {
    dmin = "5";
    combima_dminLineEdit->setText("5");
  }
  
  // get the folder name
  if (name.isEmpty()) {
    length = dir.length();
    lastchar = dir.right(1);
    dirtmp = dir;
    if (lastchar.compare("/") == 0) {
      dirtmp.truncate(length-1);
    }
    name = dirtmp.section( '/', -1 );
  }
  
  // the stacking method
  if (combima_methodComboBox->currentItem() == 0) method = "MEDIAN";
  if (combima_methodComboBox->currentItem() == 1) method = "MEAN";
  
  // rescaling
  if (combima_rescaleCheckBox->isChecked()) {
    rescale = "1";
    level = combima_levelLineEdit->text();
  }
  else rescale = "0";
  
  // now we can compile the script syntax
  scriptstring = "./parallel_manager.sh combine_images_para.sh -m ";
  scriptstring.append(dir);
  if ( !filter.isEmpty()) {
    scriptstring.append(" -f ");
    scriptstring.append(filter);
  }
  if ( !name.isEmpty()) {
    scriptstring.append(" -n ");
    scriptstring.append(name);
  }
  if ( !dt.isEmpty() && !dmin.isEmpty()) {
    scriptstring.append(" -d ");
    scriptstring.append(dt);
    scriptstring.append(" ");
    scriptstring.append(dmin);
  }
  if ( !nlow.isEmpty() && !nhigh.isEmpty()) {
    scriptstring.append(" -r ");
    scriptstring.append(nlow);
    scriptstring.append(" ");
    scriptstring.append(nhigh);
  }
  if ( rescale.compare("1") == 0) {
    if ( !level.isEmpty()) {
      scriptstring.append(" -l ");
      scriptstring.append(level);
    }
  }
  scriptstring.append(" -s ");
  scriptstring.append(rescale);
  scriptstring.append(" -c ");
  scriptstring.append(method);
  
  QString fullpath = homedir;
  fullpath.append("/");
  fullpath.append(".theli/scripts/theli_combima_comm.sh");
  
  // write the superscript and execute it
  QDir datadir = QDir(dir);
  
  if (datadir.exists() && !dir.isEmpty()) {
    write_superscriptbody(fullpath, scriptstring);
  }
}


void combineimages::write_superscriptbody(QString fullpath, QString scripts)
{
  FILE *of, *of2;
  QString reply, execstr;
  QString historypath, tmp, command;
  int length, numcommands, i;
  
  // leave if the process is running already!
  if (started_proc_combima == 1) return;
  
  // write everything into the file
  of = fopen(fullpath, "w");
  
  fprintf(of, "#!%s\n\n", bash.ascii());
  fprintf(of, "# ******************************************\n");
  fprintf(of, "# *****  Created on the fly by THELI   *****\n");
  fprintf(of, "# ******************************************\n\n");
  fprintf(of, "export INSTRUMENT=%s\n", instrument.ascii());
  fprintf(of, "cd %s\n\n", scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  
  tmp = fullpath;
  tmp.append("_tmp");
  of2 = fopen(tmp, "w");
  fprintf(of2,"%s\n",scripts.ascii());
  fclose(of2);
  
  combima_fullpathlog = fullpath;
  combima_fullpathlog.append(".log");
  
  // get the number of commands issued
  execstr = "gawk 'END {print NR}' ";
  execstr.append(tmp);
  
  my_fgets(execstr,command);
  
  numcommands = atoi(command);
  
  if (scripts.length() > 0) {
    
    // now write each command to a script file and execute
    combima_concatenate_logs = "cat ";
    combima_remove_individuallogs = "\\rm -f ";
    for (i=1; i<=numcommands; i++) {
      QString i_string;
      i_string = i_string.setNum( i );
      execstr = "gawk '(NR == ";
      execstr.append(i_string);
      execstr.append(") {print $0}' ");
      execstr.append(tmp);
      
      my_fgets(execstr, command);
      
      length = command.length() - 1;
      command.truncate(length);
      
      if (!command.isEmpty() ) {
	reply = "Running...";
	fprintf(of, "echo %s\n", reply.ascii());
	fprintf(of, "%s%s%s%d%s\n", command.ascii(), 
		" > ", combima_fullpathlog.ascii(), i, " 2>&1");
	fprintf(of, "\n\n");
	// prepare the concatenation of the individual log files
	combima_concatenate_logs.append(combima_fullpathlog);
	combima_concatenate_logs.append(i_string);
	combima_concatenate_logs.append(" ");
	// prepare the deletion of the individual log files
	combima_remove_individuallogs.append(combima_fullpathlog);
	combima_remove_individuallogs.append(i_string);
	combima_remove_individuallogs.append(" ");
      }
    }
    
    combima_concatenate_logs.append(" > ");
    combima_concatenate_logs.append(combima_fullpathlog);
    combima_concatenate_logs.append(" 2>&1");
    
    fprintf(of, "cd %s\n\n", startdir.ascii());
    fprintf(of, "THELIMESSAGE: finished\n");
    fclose(of);
    
    // delete the temporary script
    QFile tmpfile(tmp);
    tmpfile.remove();
    
    // make the superscript executable
    execstr = "chmod +x ";
    execstr.append(fullpath);
    my_system(execstr);
    
    // execute
    execstr = fullpath;
    proc_combima = new QProcess( this );
    proc_combima->addArgument( fullpath );
    //	connect(proc_combima, SIGNAL(readyReadStderr()), this, SLOT(combima_updateError()));
    connect(proc_combima, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
    connect(proc_combima, SIGNAL(processExited()), this, SLOT(updateExit()));
    if (started_proc_combima == 0) {
      while( !proc_combima->isRunning() ) proc_combima->launch("");
      started_proc_combima = 1;
      combima_statusLineEdit->setText("Running...");
      combima_statusLineEdit->setPaletteBackgroundColor(QColor(255,254,212));
    }
  }
}


void combineimages::updateText()
{
  QString tmp, tmp2;
  QByteArray data = proc_combima->readStdout();
  tmp = QString(data);
  if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
    tmp2 = tmp.right( tmp.length() - 14);
    if (tmp2.compare("finished\n") == 0) {
      combima_statusLineEdit->setText("Done.");
      combima_statusLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
    }
  }
}


void combineimages::updateExit()
{
  int falseerrcount;
  QString line, errormessage, errorcode, falseerrorcode;
  QStringList errorlist, falseerrorlist;
  
  // scan for these error messages
  errorlist << "error";
  
  // ignore these error messages
  falseerrorlist << "found no such key: e1";
  falseerrorlist << "echo \"error in filtering list";
  falseerrorlist << "not recreating links led to errors when";
  
  my_system(combima_concatenate_logs);

  my_system(combima_remove_individuallogs);

  // error handling
  QFile file(combima_fullpathlog);
  
  if ( file.open( IO_ReadOnly ) && file.exists()) {
    errormessage = "";
    combima_errcount = 0;
    combima_errline = 0;
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      line.stripWhiteSpace();
      combima_errline++;
      falseerrcount = 0;
      for ( QStringList::Iterator it2 = falseerrorlist.begin(); 
	    it2 != falseerrorlist.end(); ++it2 ) {
	falseerrorcode = *it2;
	falseerrcount = falseerrcount + 
	  line.contains(falseerrorcode, false);
      }
      if (falseerrcount == 0) {
	for ( QStringList::Iterator it = errorlist.begin(); 
	      it != errorlist.end(); ++it ) {
	  errorcode = *it;
	  combima_errcount = line.contains(errorcode, false);
	  if (combima_errcount > 0) {
	    errormessage = "THELI_ERROR";
	    errormessage.append(line);
	    break;
	  }
	  else combima_errcount = 0;
	}
	if (combima_errcount > 0) break;
      }
    }
    file.close();
    
    if (proc_combima->normalExit()) 
      show_scriptlog(errormessage);
    else {
      combima_statusLineEdit->setText("Aborted.");
    } 
  }
}


void combineimages::show_scriptlog(QString errormessage)
{
  QString execstr, completemessage, errorline;
  
  if (errormessage.left(11).compare("THELI_ERROR") != 0 && 
      errormessage.length() < 11 && combima_errcount == 0) {
    combima_statusLineEdit->setText("Done.");
    combima_statusLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
  }
  
  if (errormessage.left(11).compare("THELI_ERROR") == 0 && 
      errormessage.length() > 11 && combima_errcount != 0) {
    errorline.setNum(combima_errline);
    completemessage = "ERROR (line ";
    completemessage.append(errorline);
    completemessage.append(")");
    combima_statusLineEdit->setText(completemessage);
    combima_statusLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
    
    // display the log file
    execstr = editor + " ";
    execstr.append(combima_fullpathlog);
    execstr.append(" &");
    my_system(execstr);
  } 
  
  started_proc_combima = 0;    
}


void combineimages::kill_combineimage()
{
  QString execstr;
  long process_id;
  
  if (started_proc_combima == 1) {
    if (proc_combima->isRunning()) {
      process_id = proc_combima->processIdentifier();
      combima_pid = combima_pid.setNum(process_id);
      execstr = scriptdir;
      execstr.append("/parakill.sh ");
      execstr.append(combima_pid);
      execstr.append(" > /dev/null 2>&1");
      my_system(execstr);
      combima_statusLineEdit->setText("Killed.");	
    }
    started_proc_combima = 0;
  }
}


void combineimages::init()
{
  QString logpath, qtmp, lastlog, execstr, sciencepath;
  QSettings settings (QSettings::Ini);
  
  // get general info
  settings.beginGroup("/theligeneral_");
  lastlog = settings.readEntry("/lastlog", "noname");
  //      scriptdir = settings.readEntry("/mainpath", "");
  homedir = settings.readEntry("/homedir", "");
  startdir = settings.readEntry("/startdir", "");
  bash = settings.readEntry("/bash", "");
  settings.endGroup();
  
  // grep the currently selected instrument and sciencedir
  settings.beginGroup("/theli_"+lastlog+"_");
  settings.beginGroup("/INITIALISE");
  sciencepath = settings.readEntry("/maindir");
  sciencepath.append(settings.readEntry("/sciencedir"));
  // in this case we do not need to distinguish between the
  // various intrument paths. We just need the name, the rest is
  // handled in the combima_images_para.sh script
  editor = settings.readEntry("/editor");
  if (strcmp(editor,"vi") == 0)
    editor = "xterm -e vi ";
  instrument = settings.readEntry("/instrumentname");
  settings.endGroup();
  settings.endGroup();
  
  started_proc_combima = 0;
  
  combima_methodComboBox->setCurrentItem(0);
  combima_rescaleCheckBox->setChecked(false);
  combima_levelLineEdit->hide();
  
  // fill in some values
  combima_dirLineEdit->setText(sciencepath);
  
  // check if the image directory exists, and paint it accordingly
  QDir dir = QDir(sciencepath);
  
  if (dir.exists() && !sciencepath.isEmpty()) 
    combima_dirLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
  if (!dir.exists() && !sciencepath.isEmpty()) 
    combima_dirLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
  if (sciencepath.isEmpty()) 
    combima_dirLineEdit->setPaletteBackgroundColor(QColor(255,255,255));
  
  // set the focus to the filter line edit
  combima_filterLineEdit->setFocus(); 
}


void combineimages::quit()
{
  // only close the dialog if no processes are running
  if (started_proc_combima == 0) 
    combineimages::close();
  else {
    int answer = QMessageBox::warning(this, "Process still running",
				  QString( "A process is still running!\n If you Quit, it will be aborted.\n\n" ),
				  "&Cancel", "&Quit", QString::null, 0, 1 );

    if (answer == 0) return;
    else {
      kill_combineimage();
      combineimages::close();
    }
  }
}
