#ifndef SECTION_PRE_H
#define SECTION_PRE_H

#include <qregexp.h>
#include <qvalidator.h>
#include <qmessagebox.h>


QString notification_pre;

void theliForm::clearscripts_pre_pushbutton()
{
  pre_sortdataCheckBox->setChecked(false);
  pre_splitCheckBox->setChecked(false);
  pre_createlinksCheckBox->setChecked(false);
  CB_sortrawdata = 0;
  CB_processsplit = 0;
  CB_createlinks = 0;
  paint_checkboxes();
  pre_messagesTextEdit->clear();
  update_logfile();
  update_pre_commandlist();
}


void theliForm::resetfields_pre_pushbutton()
{
  pre_linkchipsLineEdit->clear();
  pre_scratchdirLineEdit->clear();
  pre_messagesTextEdit->clear();
  update_logfile();
  update_pre_commandlist();
}


void theliForm::pre_splitdirs(QLineEdit *defaultdir)
{
  QSettings settings(QSettings::Ini);
  QString mainpath, split_dir;
  QString splitscriptname, instrumentname;
  bool xtalk_nor_checked, xtalk_row_checked, xtalk_multi_checked;
  int numinst;
  
  mainpath = in_maindirLineEdit->text();
  split_dir = defaultdir->text();
  
  QString reductionlog = in_reductionlogLineEdit->text();
  
  settings.beginGroup("/theli_"+reductionlog+"_");
  settings.beginGroup("/PREPARATION");
  settings.beginGroup("/corrections");
  xtalk_nor_checked = settings.readBoolEntry("/xtalk_norcheckbox",false);
  xtalk_row_checked = settings.readBoolEntry("/xtalk_rowcheckbox",false);
  xtalk_multi_checked = settings.readBoolEntry("/xtalk_multicheckbox",false);
  settings.endGroup();
  settings.endGroup();
  settings.endGroup();
  
  numinst = in_instrumentListBox->numRows();
  if (numinst > 0) {
    instrumentname = in_instrumentListBox->currentText();
  } else instrumentname = "";
  
  splitscriptname = "process_split_";
  splitscriptname.append(instrumentname);
  splitscriptname.append(".sh");
  
  if (!instrumentname.isEmpty() ) {
    create_scriptcall_split(pre_commandTextEdit, 0, splitscriptname, mainpath, 
			    split_dir);
    if (xtalk_nor_checked || xtalk_row_checked || xtalk_multi_checked)
      create_scriptcall_1(pre_commandTextEdit, 1, "correct_xtalk.sh", mainpath, 
			  split_dir);
  }
}


void theliForm::pre_createlinks(QLineEdit *defaultdir)
{
  QString mainpath, split_dir;
  QString scratch_dir, linkedchips; 
  
  scratch_dir = pre_scratchdirLineEdit->text();
  linkedchips = pre_linkchipsLineEdit->text();
  
  mainpath = in_maindirLineEdit->text();
  split_dir = defaultdir->text();
  mainpath.append(split_dir);
  
  create_scriptcall_2(pre_commandTextEdit, 1, "createlinks.sh", mainpath, 
		      scratch_dir, linkedchips);
}


void theliForm::update_pre_commandlist()
{
  QString mainpath, sort_std, standard;
  QString bias, dark, flat, flatoff, science, sky;
  QString split_dir, scratch_dir, linkedchips;
  QString splitscriptname, instrumentname;
  QDir e;
  int CB_array[3];
  int checkbias, checkdark, checkflat, checkflatoff;
  int checkscratch, checksky;
  int checkscience, checkstandard, notecount = 0;
  
  pre_commandTextEdit->setReadOnly(false);
  
  repaint_parameter_fields_pre();
  
  notification_pre = "";
  
  QRegExp rx( "^\\S+$" );
  QValidator* validator_string = new QRegExpValidator( rx, this );
  QRegExp rint( "^[0-9]+[0-9\\s]+$" );
  QValidator* validator_intblank = new QRegExpValidator( rint, this );
  pre_scratchdirLineEdit->setValidator( validator_string );
  pre_linkchipsLineEdit->setValidator( validator_intblank );
  
  // clear the command list window
  pre_commandTextEdit->setText(empty);
  
  // check the status of the check boxes. write the status into 
  // an array for easier handling
  if (pre_sortdataCheckBox->isChecked())  CB_array[0] = 1; 
  else CB_array[0] = 0;
  if (pre_splitCheckBox->isChecked())  CB_array[1] = 1; 
  else CB_array[1] = 0;
  if (pre_createlinksCheckBox->isChecked())  CB_array[2] = 1; 
  else CB_array[2] = 0;
  
  // check that the main path is absolute
  QDir absmainpath = QDir(in_maindirLineEdit->text());
  if (absmainpath.isRelative() && sum(CB_array,3) > 0) {
    warn(pre_commandTextEdit,"STOP: The main path given under \"Initialise\" must be absolute!");
    return;
  }
  
  // the main data directory
  mainpath = in_maindirLineEdit->text();
  
  // the subdirectories
  bias      = in_biasdirLineEdit->text();
  dark      = in_darkdirLineEdit->text();
  flat      = in_flatdirLineEdit->text();
  flatoff   = in_flatoffdirLineEdit->text();
  science   = in_sciencedirLineEdit->text();
  sky       = in_skydirLineEdit->text();
  standard  = in_standarddirLineEdit->text();
  
  // read the directories entered
  scratch_dir = pre_scratchdirLineEdit->text();
  
  // check if the data directories actually exist
  checkbias      = checkdir(mainpath, bias);
  checkdark      = checkdir(mainpath, dark);
  checkflat      = checkdir(mainpath, flat);
  checkflatoff   = checkdir(mainpath, flatoff);
  checkscience   = checkdir(mainpath, science);
  checksky       = checkdir(mainpath, sky);
  checkstandard  = checkdir(mainpath, standard);
  checkscratch   = checkdir2(scratch_dir);
  
  // create the commands and update the command list window
  
  // try automatic data sorting
  if (CB_array[0] == 1)
    create_scriptcall_0(pre_commandTextEdit, 0, "sort_rawdata.sh", mainpath);
  
  // split files and correct header
  if (CB_array[1] == 1) {
    
    // if directories contain files with names longer than
    // 60 characters, print a warning and return
    int counter = 0;
    check_filenamelength(mainpath, bias, counter);
    check_filenamelength(mainpath, dark, counter);
    check_filenamelength(mainpath, flat, counter);
    check_filenamelength(mainpath, flatoff, counter);
    check_filenamelength(mainpath, science, counter);
    check_filenamelength(mainpath, sky, counter);
    check_filenamelength(mainpath, standard, counter);
    if (counter > 0) return;
    
    // Check that the splitting script exists. If not, return;
    int numinst = in_instrumentListBox->numRows();
    if (numinst > 0) {
      instrumentname = in_instrumentListBox->currentText();
    } else instrumentname = "";
    splitscriptname = "process_split_";
    splitscriptname.append(instrumentname);
    splitscriptname.append(".sh");
    QFile script1(scriptdir+"/"+splitscriptname);
    QFile script2(userscriptdir+"/"+splitscriptname);
    
    if (!script1.exists() && !script2.exists()) {
      warn(pre_commandTextEdit,"STOP: The splitting script for "+instrumentname+" does not exist!");
      warn(pre_commandTextEdit,"Probably you forgot to create or save it after you created the instrument configuration?");
      return;
    }
    
    // check if master bias/dark/flat exists already
    // checking for the first chip is sufficient
    QFile mbias(mainpath+"/"+bias+"/"+bias+"_1.fits"); 
    QFile mdark(mainpath+"/"+dark+"/"+dark+"_1.fits"); 
    QFile mflat(mainpath+"/"+flat+"/"+flat+"_1.fits"); 
    QFile mflatoff(mainpath+"/"+flatoff+"/"+flatoff+"_1.fits"); 
    if (mbias.exists()) { 
      note(pre_messagesTextEdit, "Skipping "+bias+"... master bias found.");
      notecount++;
    }
    if (mdark.exists()) {
      note(pre_messagesTextEdit, "Skipping "+dark+"... master dark found.");
      notecount++;
    }
    if (mflat.exists()) {
      note(pre_messagesTextEdit, "Skipping "+flat+"... master flat found.");
      notecount++;
    }
    if (mflatoff.exists()) {
      note(pre_messagesTextEdit, "Skipping "+flatoff+"... master flat off found.");
      notecount++;
    }
    
    int testresult = 0;
    int testbias = 0, testflat = 0, testflatoff = 0, testdark = 0;
    int testscience = 0, testsky = 0, teststandard = 0;
    if ( checkbias == 1 && !mbias.exists()) {
      check_splittingstatus(bias, testresult);
      if (testresult == 1) {
	testbias = 1;
	note(pre_messagesTextEdit, "Skipping "+bias+"... split images found.");
	notecount++;
      }
    }
    if ( checkdark == 1 && !mdark.exists()) {
      check_splittingstatus(dark, testresult);
      if (testresult == 1) {
	testdark = 1;
	note(pre_messagesTextEdit, "Skipping "+dark+"... split images found.");
	notecount++;
      }
    }
    if ( checkflat == 1 && !mflat.exists()) {
      check_splittingstatus(flat, testresult);
      if (testresult == 1) {
	testflat = 1;
	note(pre_messagesTextEdit, "Skipping "+flat+"... split images found.");
	notecount++;
      }
    }
    if ( checkflatoff == 1 && !mflatoff.exists()) {
      check_splittingstatus(flatoff, testresult);
      if (testresult == 1) {
	testflatoff = 1;
	note(pre_messagesTextEdit, "Skipping "+flatoff+"... split images found.");
	notecount++;
      }
    }
    if ( checkscience == 1) {
      check_splittingstatus(science, testresult);
      if (testresult == 1) {
	testscience = 1;
	note(pre_messagesTextEdit, "Skipping "+science+"... split images found.");
	notecount++;
      }
    }
    if ( checksky == 1) {
      check_splittingstatus(sky, testresult);
      if (testresult == 1) {
	testsky = 1;
	note(pre_messagesTextEdit, "Skipping "+sky+"... split images found.");
	notecount++;
      }
    }
    if ( checkstandard == 1) {
      check_splittingstatus(standard, testresult);
      if (testresult == 1) {
	teststandard = 1;
	note(pre_messagesTextEdit, "Skipping "+standard+"... split images found.");
	notecount++;
      }
    }
    
    if (notecount > 0) {
      pre_messagesTextEdit->append("\n");
    }
    
    if ( checkbias == 1 && !mbias.exists() && testbias == 0) pre_splitdirs(in_biasdirLineEdit);
    if ( checkdark == 1 && !mdark.exists() && testdark == 0) pre_splitdirs(in_darkdirLineEdit);
    if ( checkflat == 1 && !mflat.exists() && testflat == 0) pre_splitdirs(in_flatdirLineEdit);
    if ( checkflatoff == 1 && !mflatoff.exists() && testflatoff == 0) pre_splitdirs(in_flatoffdirLineEdit);
    if ( checkscience   == 1 && testscience   == 0) pre_splitdirs(in_sciencedirLineEdit);
    if ( checksky == 1 && testsky == 0) pre_splitdirs(in_skydirLineEdit);
    if ( checkstandard  == 1 && teststandard  == 0) pre_splitdirs(in_standarddirLineEdit);
    if (checkbias == 0 && checkdark == 0 && checkflat == 0 && checkflatoff == 0 && 
	checkscience == 0 && checksky == 0 && checkstandard == 0)
      warn(pre_commandTextEdit,"STOP: Split FITS: No valid data directories were specified.");
  }
  
  // create linked images
  if (CB_array[2] == 1) {
    linkedchips = pre_linkchipsLineEdit->text();
    if (linkedchips.isEmpty())
      warn(pre_commandTextEdit,"STOP: Create links: No chips were specified.");
    if (scratch_dir.isEmpty() || checkscratch == 0) {
      pre_scratchdirLineEdit->setPaletteBackgroundColor(tfred);
      warn(pre_commandTextEdit,"STOP: Create links: No valid scratch directory found.");
    }
    if (checkscratch == 1 && !scratch_dir.isEmpty()) 
      pre_scratchdirLineEdit->setPaletteBackgroundColor(tfgreen);
    if (checkscratch == 1 && !linkedchips.isEmpty()) {
      if ( checkbias == 1 ) pre_createlinks(in_biasdirLineEdit);
      if ( checkdark == 1 ) pre_createlinks(in_darkdirLineEdit);
      if ( checkflat == 1 ) pre_createlinks(in_flatdirLineEdit);
      if ( checkflatoff == 1 ) pre_createlinks(in_flatoffdirLineEdit);
      if ( checkscience == 1 ) pre_createlinks(in_sciencedirLineEdit);
      if ( checksky == 1 ) pre_createlinks(in_skydirLineEdit);
      if ( checkstandard == 1 ) pre_createlinks(in_standarddirLineEdit);
      if (checkbias == 0 && checkdark == 0 && checkflat == 0 && checkflatoff == 0 && 
	  checkscience == 0 && checksky == 0)
	warn(pre_commandTextEdit,"STOP: Create links: No valid data directories were specified.");
    }
  }
  else {
    if (CB_array[2] == 0) 
      pre_scratchdirLineEdit->setPaletteBackgroundColor(tfcyan);
  }
}


void theliForm::repaint_parameter_fields_pre()
{
    repaint_parameter_fields_helper(pre_createlinksCheckBox, 
				    pre_linkchipsLineEdit);
}


void theliForm::make_ascii_conform_helper(QString directory)
{
  QDir dir = QDir(directory);  
  QStringList list, charlist;
  QString oldname, newname, oldpath, newpath;
  QString badchars("[ [](){}\n\t\f\"\'$\%&?*°^;#/!]");
  QChar delchar;
  int i;
  uint j;

  dir.setFilter(QDir::Files);
  list = dir.entryList();

  i = 0;
  for (QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
    QFileInfo fInfo = QFileInfo(*it);
    oldname = fInfo.fileName();
    oldpath = directory+"/"+oldname;
    newname = oldname.simplifyWhiteSpace();
    newname = newname.replace(" ", "_");

    j = 0;
    while (j<badchars.length()) {
      delchar = badchars.at(j);
      newname.remove(delchar);
      j++;
    }

    if (newname.compare(oldname) != 0 && 
	!oldname.isEmpty() && 
	!newname.isEmpty()) {
      newpath = directory+"/"+newname;
      if (!dir.rename(oldpath, newpath)) 
	printf("ERROR: Problem removing invalid characters from file %s!\n", oldname.ascii());
    }
    i++;
  }
}


void theliForm::make_ascii_conform()
{
  QString mainpath, bias, dark, flat, flatoff, science, sky, standard;

  mainpath  = in_maindirLineEdit->text() + "/";
  bias      = mainpath + in_biasdirLineEdit->text();
  dark      = mainpath + in_darkdirLineEdit->text();
  flat      = mainpath + in_flatdirLineEdit->text();
  flatoff   = mainpath + in_flatoffdirLineEdit->text();
  science   = mainpath + in_sciencedirLineEdit->text();
  sky       = mainpath + in_skydirLineEdit->text();
  standard  = mainpath + in_standarddirLineEdit->text();

  make_ascii_conform_helper(bias);
  make_ascii_conform_helper(dark);
  make_ascii_conform_helper(flat);
  make_ascii_conform_helper(flatoff);
  make_ascii_conform_helper(science);
  make_ascii_conform_helper(sky);
  make_ascii_conform_helper(standard);
}


// in the following "exec_" script we need to check if the textEdit 
// with the commands is not empty. Otherwise the GUI hangs if the 
// "start" button is clicked.

void theliForm::exec_pre()
{
    QString fullpath, scripts, presentinst;
    
    make_ascii_conform();

    modify_paramsini();
    modify_paramset2();

    fullpath = userscriptdir;
    fullpath.append("/");
    fullpath.append("theli_pre_comm.sh");
    
    scripts = pre_commandTextEdit->text();
    if (scripts.contains("STOP:") != 0) return;
    presentinst = tf_instrumentLineEdit->text();
    
    // write the superscript and execute it
    process_group = "pre";
    if (checkdir2(userscriptdir) == 1 && !presentinst.isEmpty() && 
	!scripts.isEmpty()) {
	write_superscriptbody(fullpath, scripts);
    }
}


void theliForm::kill_pre()
{
  QString execstr;
  
  if (started_proc_pre == 1) {
    if (proc_pre->isRunning()) {
      process_id = proc_pre->processIdentifier();
      pid = pid.setNum(process_id);
      execstr = scriptdir;
      execstr.append("/parakill.sh ");
      execstr.append(pid);
      execstr.append(" > /dev/null 2>&1");
      my_system(execstr);
      pre_messagesTextEdit->append("All processes killed.");	
    }
    started_proc_pre = 0;
  }
}


void theliForm::check_splittingstatus(QString subdir, int &testresult)
{
  QString execstr, mainpath, path, name, result;

  mainpath = in_maindirLineEdit->text();
  path = mainpath + "/" + subdir;

  testresult = 0;

  QDir dir = QDir(path);
  QStringList fil = dir.entryList("*_1.fits");

  if (fil.isEmpty()) return;

  // look at the first image in the list
  QStringList::Iterator it = fil.begin();
  QFileInfo fi = QFileInfo(path+"/"+(*it));
  if ( fi.isFile() || fi.isSymLink()) {
    name = fi.fileName();
    execstr = bindir + "/dfits " + path + "/" + name + 
      " | gawk 'BEGIN{s=\"no\"} {if ($0 ~ /HISTORY mefsplit:/ || $0 ~ /THELI Pipeline Version/) s=\"yes\"} END {print s}'";
    my_fscanf(execstr,result);
    if (result == "yes") {
      testresult = 1;
      return;
    }
  }
}


void theliForm::check_filenamelength(QString dir1, QString dir2, int &counter)
{
  QDir dir = QDir(dir1+"/"+dir2);
  QStringList images = dir.entryList("*.fits");
  QString file;

  if (counter > 0 || dir1.isEmpty() || dir2.isEmpty()) return;

  for ( QStringList::Iterator it = images.begin(); it != images.end(); ++it ) {
    file = *it;
    if (file.length() >= 60) {
      warn(pre_commandTextEdit,"STOP: Some files in the directory");
      warn(pre_commandTextEdit,"      "+dir1+"/"+dir2);
      warn(pre_commandTextEdit,"      have names longer than 60 characters. They would be truncated in");
      warn(pre_commandTextEdit,"      FITS table headers, leading to incomplete propagation of information.");
      warn(pre_commandTextEdit,"      Rename the files to shorter file names to avoid this problem.\n");
      counter++;
      break;
    }
  }
}

#endif
