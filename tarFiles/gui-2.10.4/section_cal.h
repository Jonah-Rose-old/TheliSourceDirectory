#ifndef SECTION_CAL_H
#define SECTION_CAL_H

#include <qmessagebox.h>

void theliForm::resetfields_cal_pushbutton()
{
  cal_nobiasCheckBox->setChecked(false);
  cal_noflatCheckBox->setChecked(false);
  cal_darkinsteadbiasCheckBox->setChecked(false);
  cal_biasminLineEdit->setText("");
  cal_biasmaxLineEdit->setText("");
  cal_darkminLineEdit->setText("");
  cal_darkmaxLineEdit->setText("");
  cal_flatminLineEdit->setText("");
  cal_flatmaxLineEdit->setText("");
  cal_scienceminLineEdit->setText("");
  cal_sciencemaxLineEdit->setText("");
  cal_messagesTextEdit->clear();
  update_logfile();
  update_cal_commandlist();
}


void theliForm::clearscripts_cal_pushbutton()
{
  cal_procbiasCheckBox->setChecked(false);
  cal_procdarkCheckBox->setChecked(false);
  cal_procflatCheckBox->setChecked(false);
  cal_procscienceCheckBox->setChecked(false);
  cal_messagesTextEdit->clear();
  CB_processbias = 0;
  CB_processdark = 0;
  CB_processflat = 0;
  CB_processscience = 0;
  paint_checkboxes();
  science_ofc = 0;
  sky_ofc = 0;
  update_logfile();
  update_cal_commandlist();
  statusCPSDebiasingAction->setOn(false);
}


void theliForm::update_cal_commandlist()
{
  QString mainpath, flat_min, flat_max;
  QString bias_min, bias_max, dark_min, dark_max, science_min, science_max;
  QString bias, dark, flat, flatoff, science, sky, standard;
  QString modestring;
  QString execstr;
  int CB_array[4];
  int checkbias, checkdark, checkflat, checkflatoff, checkscience; 
  int checksky, checkstandard;
  int darkinsteadbias, nobias, noflat;
  int numfiles, numfiles_std, numfiles_OFC;
  QDir e;
  
  cal_commandTextEdit->setReadOnly(false);
  
  science_ofc = 0;
  science_background = 0;
  science_chopnod = 0;
  science_coll = 0;
  science_debloom = 0;
  science_directphot = 0;
  sky_ofc = 0;
  sky_background = 0;
  sky_chopnod = 0;
  sky_coll = 0;
  sky_debloom = 0;
  sky_directphot = 0;
  
  // make that numeric fields only accept numeric input
  QRegExp ri( "^[-]{0,1}[0-9]+" );
  QValidator* validator_int = new QRegExpValidator( ri, this );
  cal_biasminLineEdit->setValidator( validator_int );
  cal_biasmaxLineEdit->setValidator( validator_int );
  cal_darkminLineEdit->setValidator( validator_int );
  cal_darkmaxLineEdit->setValidator( validator_int );
  cal_flatminLineEdit->setValidator( validator_int );
  cal_flatmaxLineEdit->setValidator( validator_int );
  cal_scienceminLineEdit->setValidator( validator_int );
  cal_sciencemaxLineEdit->setValidator( validator_int );
  
  // clear the command list window
  cal_commandTextEdit->setText(empty);
  
  // check the status of the check boxes.
  // write the status into an array for easier handling
  if (cal_procbiasCheckBox->isChecked())  
    CB_array[0] = 1; else CB_array[0] = 0;
  if (cal_procdarkCheckBox->isChecked())  
    CB_array[1] = 1; else CB_array[1] = 0;
  if (cal_procflatCheckBox->isChecked())  
    CB_array[2] = 1; else CB_array[2] = 0;
  if (cal_procscienceCheckBox->isChecked())  
    CB_array[3] = 1; else CB_array[3] = 0;
  
  // check that the main path is absolute
  QDir absmainpath = QDir(in_maindirLineEdit->text());
  if (absmainpath.isRelative() && sum(CB_array,4) > 0) {
    warn(cal_commandTextEdit,"STOP: The main path given under \"Initialise\" must be absolute!");
    return;
  }
  
  // do some checks (dark instead of the bias, no bias, no flat)
  if (cal_darkinsteadbiasCheckBox->isChecked()) darkinsteadbias = 1;
  else darkinsteadbias = 0;
  if (cal_nobiasCheckBox->isChecked()) nobias = 1;
  else nobias = 0;
  if (cal_noflatCheckBox->isChecked()) noflat = 1;
  else noflat = 0;
  
  // the main data directory
  mainpath = in_maindirLineEdit->text();
  
  // the subdirectories
  bias = in_biasdirLineEdit->text();
  dark = in_darkdirLineEdit->text();
  flat = in_flatdirLineEdit->text();
  flatoff = in_flatoffdirLineEdit->text();
  science = in_sciencedirLineEdit->text();
  sky = in_skydirLineEdit->text();
  standard = in_standarddirLineEdit->text();
  
  // check if the data directories actually exist
  checkbias = checkdir(mainpath, bias);
  checkdark = checkdir(mainpath, dark);
  checkflat = checkdir(mainpath, flat);
  checkflatoff = checkdir(mainpath, flatoff);
  checkscience = checkdir(mainpath, science);
  checksky = checkdir(mainpath, sky);
  checkstandard = checkdir(mainpath, standard);
  if (checkstandard == 1) 
    numfiles_std = count_files(mainpath+"/"+standard,"");
  else numfiles_std = 0;
  
  // create the commands and update the command list window
  
  // process the BIASes
  if (CB_array[0] == 1) {
    if (checkbias == 1) {
      bias_min = cal_biasminLineEdit->text();
      bias_max = cal_biasmaxLineEdit->text();
      numfiles = count_files(mainpath+"/"+bias,"");
      if (numfiles < 3) {
	warn(cal_commandTextEdit,"STOP: Process bias: At least 3 exposures are required per CCD.");
      }
      if (numfiles >= 3) {
	if (!bias_min.isEmpty() && !bias_max.isEmpty()) {
	  create_scriptcall_4(cal_commandTextEdit, 1, "check_files_para.sh", mainpath, 
			      bias, "empty", bias_min, bias_max);
	}
	create_scriptcall_1(cal_commandTextEdit, 1, "process_bias_para.sh", 
			    mainpath, bias);
      }
    }
    if (checkbias == 0) {
      warn(cal_commandTextEdit,"STOP: Process bias: No BIAS directory found.");
    }
  }
  
  // process the DARK
  if (CB_array[1] == 1) {
    if(checkdark == 1) {
      dark_min = cal_darkminLineEdit->text();
      dark_max = cal_darkmaxLineEdit->text();	
      numfiles = count_files(mainpath+"/"+dark,"");
      if (numfiles < 3) {
	warn(cal_commandTextEdit,"STOP: Process dark: At least 3 exposures are required per CCD.");
      }
      if (numfiles >=3) {
	if (!dark_min.isEmpty() && !dark_max.isEmpty()) {
	  create_scriptcall_4(cal_commandTextEdit, 1, "check_files_para.sh", mainpath, 
			      dark, "empty", dark_min, dark_max);
	}
	create_scriptcall_1(cal_commandTextEdit, 1, "process_dark_para.sh", 
			    mainpath, dark);
      }
    }
    if (checkdark == 0) {
      warn(cal_commandTextEdit,"STOP: Process dark: No DARK directory found.");
    }
  }
  
  // process the FLAT
  if (CB_array[2] == 1) {
    numfiles = count_files(mainpath+"/"+flat,"");
    if (numfiles < 3) {
      warn(cal_commandTextEdit,"STOP: Process flat: At least 3 exposures are required per CCD.");
    }
    if (numfiles >= 3) {
      if ((checkbias == 1 && checkflat == 1 && nobias == 0) || 
	  (checkflat == 1 && nobias == 1)) {
	if (nobias == 1) bias = "BIAS";
	flat_min = cal_flatminLineEdit->text();
	flat_max = cal_flatmaxLineEdit->text();	
	if (!flat_min.isEmpty() && !flat_max.isEmpty()) {
	  create_scriptcall_4(cal_commandTextEdit, 1, "check_files_para.sh", mainpath, 
			      flat, "empty", flat_min, flat_max);
	}
	create_scriptcall_2(cal_commandTextEdit, 1, "process_flat_para.sh", 
			    mainpath, bias, flat);
	// create flatratio now only if no dark flat is present. otherwise it will be created below
	if (checkflatoff != 1) 
	  create_scriptcall_1(cal_commandTextEdit, 0, "create_flat_ratio.sh", mainpath, flat);
	// subtract the flat-off from the flat-on
	if (checkflatoff == 1) {
	  create_scriptcall_2(cal_commandTextEdit, 1, "process_flat_para.sh", 
			      mainpath, bias, flatoff);
	  create_scriptcall_2(cal_commandTextEdit, 1, "subtract_flat_flatoff_para.sh", 
			      mainpath, flat, flatoff);	  
	  create_scriptcall_1(cal_commandTextEdit, 0, "create_flat_ratio.sh", mainpath, flat);
	}
	create_scriptcall_1(cal_commandTextEdit, 1, "create_norm_para.sh", 
			    mainpath, flat);
      }
      else {
	if (checkbias == 0 && nobias == 0) {
	  warn(cal_commandTextEdit,"STOP: Process flats: BIAS directory not found.");
	  warn(cal_commandTextEdit,"    If you do not want to subtract a bias, mark the \"Do not apply BIAS/DARK\" switch.");
	}
	if (checkflat == 0) {
	  warn(cal_commandTextEdit,"STOP: Process flats: FLAT directory not found.");
	}
      }
    }
  }
  
  // process the SCIENCE + SKY
  
  // lets make only one section for noBIAS/noDARK and copy the parameters
  // accordingly to avoid unneeded duplicating
  if (darkinsteadbias == 1) {
    checkbias = checkdark;
    bias = dark;
  }
  
  if (CB_array[3] == 1) {
    if ((checkbias == 1 && checkflat == 1) ||  // bias and flat exist
	(checkflat == 1 && nobias == 1) ||     // flat exists, do not apply bias
	(checkbias == 1 && noflat == 1) ||     // bias exists, do not apply flat
	(nobias == 1 && noflat == 1)) {        // do not apply bias and flat
      if (nobias == 1) bias = "BIAS";
      if (noflat == 1) flat = "FLAT";
      
      numfiles = count_files(mainpath+"/"+science,"");
      numfiles_OFC = count_files(mainpath+"/"+science,"OFC");

      QFile maxlevel(mainpath+"/"+flat+"/.maxlevel.dat");
      if (!maxlevel.exists() && noflat == 0) {
	// check if that script was already added previously 
	QString text = cal_commandTextEdit->text();
        if (!text.contains("create_flat_ratio.sh"))
	  create_scriptcall_1(cal_commandTextEdit, 0, "create_flat_ratio.sh", mainpath, flat);
      }
      if (numfiles < 1 && numfiles_OFC < 1 && checkstandard == 0)
	warn(cal_commandTextEdit,"STOP: Process science: No split images and no OFC images found.");

      if (numfiles < 1 && numfiles_OFC < 1 && checkstandard == 0)
	warn(cal_commandTextEdit,"STOP: Process science: No split images and no OFC images found.");
      
      if (numfiles < 1 && checkstandard == 1 && numfiles_std < 1)
	warn(cal_commandTextEdit,"STOP: Process science: No standard images found.");
      
      science_min = cal_scienceminLineEdit->text();
      science_max = cal_sciencemaxLineEdit->text();
      
      if (checkscience == 1 && numfiles >= 1) {
	science_ofc = 1;
	if (!science_min.isEmpty() && !science_max.isEmpty()) {
	  create_scriptcall_4(cal_commandTextEdit, 1, "check_files_para.sh", 
			      mainpath, science, "empty", science_min, 
			      science_max);
	}
	create_scriptcall_3(cal_commandTextEdit, 1, "process_science_para.sh",
			    mainpath, bias, flat, science);
      } 
      else science_ofc = 0;
      
      if (checkscience == 1 && numfiles == 0 && numfiles_OFC >= 1) {
	warn(cal_commandTextEdit,"STOP: Process science: No unprocessed SCIENCE images found.");
      }
      
      if (checkstandard == 1 && numfiles_std > 0) {
	create_scriptcall_3(cal_commandTextEdit, 1, "process_science_para.sh",
			    mainpath, bias, flat, standard);
      }		  
      if (checkscience == 0)
	warn(cal_commandTextEdit,"STOP: Calibrate data: SCIENCE directory not found.");
      
      if (checksky == 1) {
	sky_ofc = 1;
	if (!science_min.isEmpty() && !science_max.isEmpty()) {
	  create_scriptcall_4(cal_commandTextEdit, 1, "check_files_para.sh", 
			      mainpath, sky, "empty", science_min, science_max);
	}
	create_scriptcall_3(cal_commandTextEdit, 1, "process_science_para.sh",
			    mainpath, bias, flat, sky);
      } 
      else {
	science_ofc = 0; 
	sky_ofc = 0;
      }
      
      if (checkscience == 0)
	warn(cal_commandTextEdit,"STOP: Calibrate data: SCIENCE directory not found.");
    }
    else {
  	science_ofc = 0;
  	sky_ofc = 0;
  	if (checkbias == 0 && nobias == 0 && darkinsteadbias == 0) {
  	  warn(cal_commandTextEdit,"STOP: Calibrate data: BIAS directory not found.");
  	  warn(cal_commandTextEdit,"    If you do not want to subtract a bias, set the \"Do not apply BIAS/DARK\" switch.");
  	  warn(cal_commandTextEdit,"    If you have a dark and want to use it instead, set the \"Use dark\" switch.");
  	}
  	if (checkbias == 0 && nobias == 0 && darkinsteadbias == 1) {
  	  warn(cal_commandTextEdit,"STOP: Calibrate data: DARK directory not found.");
  	  warn(cal_commandTextEdit,"    If you do not want to subtract a dark, mark the \"Do not apply BIAS/DARK\" switch.");
  	  warn(cal_commandTextEdit,"    If you have a bias and want to use it instead, unmark the \"Use dark\" switch.");
  	}
  	if (checkflat == 0 && noflat == 0) {
  	  warn(cal_commandTextEdit,"STOP: Calibrate data: FLAT directory not found.");
  	  warn(cal_commandTextEdit,"    If you do not want to flat-field the data, mark the \"Do not apply FLAT\" switch.");
  	}
    }
  }
}


void theliForm::toggle_darkinsteadbiascheckbox()
{
  if (cal_nobiasCheckBox->isChecked()) 
    cal_darkinsteadbiasCheckBox->hide();
  if (!cal_nobiasCheckBox->isChecked()) 
    cal_darkinsteadbiasCheckBox->show();
}


void theliForm::check_nonlinearity()
{
  QString instrument = in_instrumentListBox->currentText();
  QString dir;
  
  
  if (cal_nonlinCheckBox->isChecked()) {
    if (in_profinstRadioButton->isChecked()) dir = profinstdir;
    if (in_comminstRadioButton->isChecked()) dir = comminstdir;
    if (in_userinstRadioButton->isChecked()) dir = userinstdir;
    
    QFile nonlincoeffs(dir+"/"+instrument+".nldat");
    
    if (!nonlincoeffs.exists()) {
      QMessageBox::information( this, "THELI",
	     "Could not find the nonlinearity coefficients file ("+instrument+".nldat).\n"
	     "A nonlinearity correction will not be performed.\n\n"
	     "If you know the nonlinearity correction polynomial for\n"
	     +instrument+", then add the coefficients to "+instrument+".nldat\n"
	     "and save it in the same place as "+instrument+".ini.\n"
	     "The polynomial is assumed to be of 3rd order and has the form\n"
	     "a0 + a1*x + a2*x^2 + a3*x^3.\n"
	     "You must write the coefficients\n"
	     "a0 a1 a2 a3\n"
	     "into "+instrument+".nldat. If the instrument has several CCDs,\n"
             "then add one line for each CCD. Unknown coefficients must be set\n"
             "explicitly to zero. For example, if the correction polynomial is of\n"
	     "2nd order, then you must set a3 = 0.0.\n");
      cal_nonlinCheckBox->setChecked(false);
    }
  }
}


void theliForm::update_biascheckboxcolor()
{
  if (cal_nobiasCheckBox->isChecked())
    cal_procbiasCheckBox->setPaletteForegroundColor(QColor(0,0,0));
  else
    cal_procbiasCheckBox->setPaletteForegroundColor(QColor(170,0,0));
  
  if (cal_noflatCheckBox->isChecked())
    cal_procflatCheckBox->setPaletteForegroundColor(QColor(0,0,0));
  else
    cal_procflatCheckBox->setPaletteForegroundColor(QColor(170,0,0));
}


void theliForm::exec_cal()
{
  QString fullpath, scripts, presentinst, dynamicsize;
  
  modify_paramsini();
  modify_paramset2();
  
  fullpath = userscriptdir;
  fullpath.append("/");
  fullpath.append("theli_cal_comm.sh");
  
  scripts = cal_commandTextEdit->text();
  if (scripts.contains("STOP:") != 0) return;
  presentinst = tf_instrumentLineEdit->text();
  
  // write the superscript and execute it
  process_group = "cal";
  if (checkdir2(userscriptdir) == 1 && !presentinst.isEmpty() && 
      !scripts.isEmpty()) {
    write_superscriptbody(fullpath, scripts);
  }
}


void theliForm::kill_cal()
{
  QString execstr;
  
  if (started_proc_cal == 1) {
    if (proc_cal->isRunning()) {
      process_id = proc_cal->processIdentifier();
      pid = pid.setNum(process_id);
      execstr = scriptdir;
      execstr.append("/parakill.sh ");
      execstr.append(pid);
      execstr.append(" > /dev/null 2>&1");
      my_system(execstr);
      cal_messagesTextEdit->append("All processes killed.");	
    }
    started_proc_cal = 0;
  }
}

#endif
