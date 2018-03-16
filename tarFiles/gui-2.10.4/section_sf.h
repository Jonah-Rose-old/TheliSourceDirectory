#ifndef SECTION_SF_H
#define SECTION_SF_H

void theliForm::resetfields_sf_pushbutton()
{
  sf_chopnodinvertCheckBox->setChecked(false);
  sf_chopnodpatternComboBox->setCurrentItem(0);
  sf_spread_numbergroupsLineEdit->setText("3");
  sf_spread_lengthsequenceLineEdit->setText("12");
  sf_messagesTextEdit->clear();
  update_logfile();
  update_sf_commandlist();
}


void theliForm::clearscripts_sf_pushbutton()
{
  sf_spreadsequenceCheckBox->setChecked(false);
  sf_backgroundCheckBox->setChecked(false);
  sf_mergesequenceCheckBox->setChecked(false);
  sf_chopnodCheckBox->setChecked(false);
  sf_collapseCheckBox->setChecked(false);
  sf_messagesTextEdit->clear();
  CB_spreadsequence = 0;
  CB_background = 0;
  CB_mergesequence = 0;
  CB_chopnod = 0;
  CB_collapse = 0;
  paint_checkboxes();
  science_background = 0;
  science_coll = 0;
  science_chopnod = 0;
  sky_background = 0;
  sky_coll = 0;
  sky_chopnod = 0;
  update_logfile();
  update_sf_commandlist();
  statusCPSBackgroundAction->setOn(false);
  statusCPSChopnodAction->setOn(false);
  statusCPSCollapseAction->setOn(false);
  statusCPSCollapseAction->setOn(false);
}


void theliForm::update_sf_commandlist()
{
  QString mainpath;
  QString sciencespread, skyspread, standardspread;
  QString science, sky, standard;
  QString numbergroups, lengthsequence, numbergroupsstring, pattern, invert;
  QDir e;
  int CB_array[5], stop = 0;
  int checkscience, checksky, processsky;
  int checkstandard;
  int i, numbergroupsint;
  int flag = 0, flag_ini = 0;
  
  sf_commandTextEdit->setReadOnly(false);

  repaint_parameter_fields_sf();
  
  science_background = 0;
  science_chopnod = 0;
  science_coll = 0;
  science_debloom = 0;
  science_directphot = 0;

  sky_background = 0;
  sky_chopnod = 0;
  sky_coll = 0;
  sky_debloom = 0;
  sky_directphot = 0;
  
  QRegExp ri( "^[-]{0,1}[0-9]+" );
  QValidator* validator_int = new QRegExpValidator( ri, this );
  sf_spread_numbergroupsLineEdit->setValidator( validator_int );
  sf_spread_lengthsequenceLineEdit->setValidator( validator_int );
  
  // clear the command list window
  sf_commandTextEdit->setText(empty);

  // check the status of the check boxes. 
  // write the status into an array for easier handling
  for (i=0; i<5; i++) CB_array[i] = 0;

  if (sf_spreadsequenceCheckBox->isChecked()) CB_array[0] = 1; else CB_array[0] = 0;
  if (sf_backgroundCheckBox->isChecked())     CB_array[1] = 1; else CB_array[1] = 0;
  if (sf_mergesequenceCheckBox->isChecked())  CB_array[2] = 1; else CB_array[2] = 0;
  if (sf_chopnodCheckBox->isChecked())        CB_array[3] = 1; else CB_array[3] = 0;
  if (sf_collapseCheckBox->isChecked())       CB_array[4] = 1; else CB_array[4] = 0;
  
  // check that the main path is absolute
  QDir absmainpath = QDir(in_maindirLineEdit->text());
  if (absmainpath.isRelative() && sum(CB_array,6) > 0) {
    warn(sf_commandTextEdit,"STOP: The main path given under \"Initialise\" must be absolute!");
    return;
  }
  
  // the main data directory
  mainpath = in_maindirLineEdit->text();
  
  // the subdirectories
  science   = in_sciencedirLineEdit->text();
  sky       = in_skydirLineEdit->text();
  standard  = in_standarddirLineEdit->text();
  
  // check if the data directories actually exist
  checkscience  = checkdir(mainpath, science);
  checksky      = checkdir(mainpath, sky);
  checkstandard = checkdir(mainpath, standard);

  if (checkscience == 0 && (
  	CB_array[0] == 1 || CB_array[1] == 1 || CB_array[2] == 1 || 
  	CB_array[3] == 1 || CB_array[4] == 1)) {
    warn(sf_commandTextEdit,"STOP: SCIENCE directory not found.");
    return;
  }

  // only process the sky data if the user explicitly requested it
  processsky = 1;
  if (!in_processskyCheckBox->isChecked() || checksky == 0) 
    processsky = 0;

  // create the commands and update the command list window
  
  // spread images in sequence in groups
  if (CB_array[0] == 1) {
    numbergroups = sf_spread_numbergroupsLineEdit->text();
    lengthsequence = sf_spread_lengthsequenceLineEdit->text();	
    if (numbergroups.isEmpty())
      warn(cal_commandTextEdit,"STOP: Spread sequence: Please specify the number of groups.");
    if (lengthsequence.isEmpty())
      warn(cal_commandTextEdit,"STOP: Spread sequence: Please specify the sequence length.");
    if (!numbergroups.isEmpty() && !lengthsequence.isEmpty()) {
      if (checkscience == 1) {
	create_scriptcall_4(sf_commandTextEdit, 0, "spread_sequence.sh", mainpath, 
			    science, "OFC", numbergroups, lengthsequence);
      }
      else 
	warn(cal_commandTextEdit,"STOP: Spread sequence: SCIENCE directory not found.");
      if (checksky == 1) {
	create_scriptcall_4(sf_commandTextEdit, 0, "spread_sequence.sh", mainpath, 
			    sky, "OFC", numbergroups, lengthsequence);
      }
      if (checkstandard == 1) {
	create_scriptcall_4(sf_commandTextEdit, 0, "spread_sequence.sh", mainpath, 
			    standard, "OFC", numbergroups, lengthsequence);
      }
    }
  }


  // now do the background modeling; command syntax depends on several parameter settings

  // loop over the spread sequence
  sciencespread = science;
  skyspread = sky;
  standardspread = standard;
  numbergroupsint = 1;
  i = 1;

  if ( (sf_spreadsequenceCheckBox->isChecked() || CB_spreadsequence == 1)) {
    numbergroupsstring = sf_spread_numbergroupsLineEdit->text();
    numbergroupsint = numbergroupsstring.toInt();
  }

  QSettings settings (QSettings::Ini);
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  settings.beginGroup("/BACKGROUND");
  settings.beginGroup("/background");
  QString maglimit = settings.readEntry("/maglimit","");
  QString distance = settings.readEntry("/distance","");
  settings.endGroup();
  settings.endGroup();
  settings.endGroup();

  get_fitsext_fordebloom(ext_img,ext_imgch);
  get_lastchar(ext_imgch);
  
  //  if (lastchar.compare("C") == 0) ext_imgch.append("B");
  if (invoked_fromexecall == 1) 
    get_fitsext_fromcheckboxes("fordebloom");
  
  for (i=1; i<=numbergroupsint; i++) {
    
    if ( (sf_spreadsequenceCheckBox->isChecked() || CB_spreadsequence == 1)) {
      sciencespread = science;
      sciencespread.append("_S");
      sciencespread.append(numbergroupsstring.setNum(i));
      skyspread = sky;
      skyspread.append("_S");
      skyspread.append(numbergroupsstring.setNum(i));
      standardspread = standard;
      standardspread.append("_S");
      standardspread.append(numbergroupsstring.setNum(i));
    }
    else {
      sciencespread = science;
      skyspread = sky;
      standardspread = standard;
    }

    // We have to provide a 'noskydir' argument if no sky is present
    if (checksky == 0) skyspread = "noskydir";
    
    // now do the actual data reduction
    science_background = 0;
    sky_background = 0;
    if (CB_array[1] == 1) {
      if (checkscience == 0 && checksky == 0) {
	warn(cal_commandTextEdit,"STOP: Background model correction: SCIENCE directory not found.");
	stop = 1;
      }

      // Create masks, models and subtract it
      if (checkscience == 1 && checksky == 0 && stop != 1) {
	if (flag == 0) check_filepresence(sf_commandTextEdit, science, "OFC", flag, flag_ini);	

	if (!maglimit.isEmpty() && !distance.isEmpty()) {
	  create_scriptcall_1(sf_commandTextEdit, 0, "id_bright_objects.sh", mainpath, sciencespread);
	}

	create_scriptcall_2(sf_commandTextEdit, 1, "process_background_para.sh", mainpath, 
			    sciencespread, skyspread);

	science_background = 1;
      }

      if (checksky == 1 && stop != 1) {
	if (flag == 0) check_filepresence(sf_commandTextEdit, sky, "OFC", flag, flag_ini);	

	if (!maglimit.isEmpty() && !distance.isEmpty()) {
	  create_scriptcall_1(sf_commandTextEdit, 0, "id_bright_objects.sh", mainpath, skyspread);
	}

	create_scriptcall_2(sf_commandTextEdit, 1, "process_background_para.sh", mainpath, 
			    sciencespread, skyspread);
	sky_background = 1;
      }

      if (checkstandard == 1 && stop != 1) {
	// we assume a standard observation is always 'empty', hence the 'noskydir' argument
	if (flag == 0) check_filepresence(sf_commandTextEdit, standard, "OFC", flag, flag_ini);	
	create_scriptcall_2(sf_commandTextEdit, 1, "process_background_para.sh", mainpath, 
			    standardspread, "noskydir");
      }
    }
  }
  
  // Merge sequence
  if (CB_array[2] == 1) {
    numbergroups = sf_spread_numbergroupsLineEdit->text();
    get_fitsext_forchopnod(ext_img,ext_imgch);
    get_lastchar(ext_imgch);
    if (numbergroups.isEmpty())
      warn(sf_commandTextEdit,"STOP: Merge sequence: Please specify the number of groups under spread sequence!");
    else {
      if (lastchar.compare("C") == 0) ext_imgch.append("B");
      
      if (flag == 0) {
	for (i=1; i<=numbergroupsint; i++) {
	  sciencespread = science;
	  sciencespread.append("_S");
	  sciencespread.append(numbergroupsstring.setNum(i));
	}
	check_filepresence(sf_commandTextEdit, sciencespread, ext_imgch, flag, flag_ini);
      }
      create_scriptcall_3(sf_commandTextEdit, 0, "merge_sequence.sh", 
			  mainpath, science, ext_imgch, numbergroups);
      
      if (processsky == 1) {
	if (flag == 0) {
	  for (i=1; i<=numbergroupsint; i++) {
	    skyspread = sky;
	    skyspread.append("_S");
	    skyspread.append(numbergroupsstring.setNum(i));
	  }
	  check_filepresence(sf_commandTextEdit, sky, ext_imgch, flag, flag_ini);
	}
	create_scriptcall_3(sf_commandTextEdit, 0, "merge_sequence.sh", 
			    mainpath, sky, ext_imgch, numbergroups);
      }
    }
  }

  // Do the chopnod sky subtraction
    
  // do the pairwise sky subtraction
  if (CB_array[3] == 1) {
    get_fitsext_forchopnod(ext_img,ext_imgch);
    get_lastchar(ext_imgch);
    if (CB_array[1] == 1 && lastchar.compare("C") == 0) ext_imgch.append("B");

    if (sf_chopnodinvertCheckBox->isChecked()) invert="1";
    else invert = "0";

    if (checkscience == 0) 
      warn(sf_commandTextEdit,"STOP: Chop/Nod sky subtraction: SCIENCE directory not found!"); 
    if (checkscience == 1) {
      pattern = "P" + sf_chopnodpatternComboBox->currentText();
      //if (flag == 0) check_filepresence(sf_commandTextEdit, science, ext_imgch, flag, flag_ini);
      create_scriptcall_4(sf_commandTextEdit, 1, "process_science_chopnod_para.sh",
			  mainpath, science, ext_imgch, pattern, invert);
      science_chopnod = 1;
    } else science_chopnod = 0;
    
    // mid-IR does not have separate sky observations as the near-IR, hence we don't loop over the sky directory
    // (which probably doesn't even exist)
    sky_chopnod = 0;
  }
  else {
    science_chopnod = 0;
    sky_chopnod = 0;
  }

  // collapse correction 
  if (CB_array[4] == 1 && stop == 0) {
    get_fitsext_forcollmerge(ext_img,ext_imgch);
    get_lastchar(ext_imgch);

    if (CB_array[1] == 1 && CB_array[3] == 0 && lastchar.compare("C") == 0) ext_imgch.append("B");
    if (CB_array[1] == 1 && CB_array[3] == 1 && lastchar.compare("C") == 0) ext_imgch.append("BH");
    if (CB_array[1] == 1 && CB_array[3] == 1 && lastchar.compare("H") != 0) ext_imgch.append("H");
    if (CB_array[1] == 0 && CB_array[3] == 1 && lastchar.compare("H") != 0) ext_imgch.append("H");

    // CHECK
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forcollapse");

    //    if (flag == 0) check_filepresence(sf_commandTextEdit, science, ext_imgch, flag, flag_ini);
    create_scriptcall_2(sf_commandTextEdit, 1, "process_collapsecorr_para.sh", 
			mainpath, science, ext_imgch);
    science_coll = 1;
    
    if (processsky == 1) {
      //if (flag == 0) check_filepresence(sf_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      create_scriptcall_2(sf_commandTextEdit, 1, "process_collapsecorr_para.sh", 
			  mainpath, sky, ext_imgch);
      sky_coll = 1;
    } else sky_coll = 0;
  }
  else {
    science_coll = 0;
    sky_coll = 0;
  }
}


void theliForm::repaint_parameter_fields_sf()
{
  repaint_parameter_fields_helper(sf_spreadsequenceCheckBox, sf_spread_numbergroupsLineEdit);
  repaint_parameter_fields_helper(sf_spreadsequenceCheckBox, sf_spread_lengthsequenceLineEdit);
}


void theliForm::exec_sf()
{
  QString fullpath, scripts, presentinst;
  
  int result;
  is_imextension_empty(result);
  if (result == 1) return;
  
  // update the params.ini
  modify_paramsini();
  modify_paramset2();
  
  fullpath = userscriptdir;
  fullpath.append("/");
  fullpath.append("theli_bg_comm.sh");
  
  scripts = sf_commandTextEdit->text();
  if (scripts.contains("STOP:") != 0) return;
  presentinst = tf_instrumentLineEdit->text();
  
  // write the superscript and execute it
  process_group = "sf";
  
  if (checkdir2(userscriptdir) == 1 && !presentinst.isEmpty() && 
      !scripts.isEmpty()) {	
    write_superscriptbody(fullpath, scripts);
  }
}


void theliForm::kill_sf()
{
  QString execstr;
  
  if (started_proc_sf == 1) {
    if (proc_sf->isRunning()) {
      process_id = proc_sf->processIdentifier();
      pid = pid.setNum(process_id);
      execstr = scriptdir;
      execstr.append("/parakill.sh ");
      execstr.append(pid);
      execstr.append(" > /dev/null 2>&1");
      my_system(execstr);
      sf_messagesTextEdit->append("All processes killed.");	
    }
    started_proc_sf = 0;
  }
}

#endif
