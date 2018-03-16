#ifndef SECTION_W_H
#define SECTION_W_H

bool distrib_done;

void theliForm::clearscripts_w_pushbutton()
{
    w_debloomCheckBox->setChecked(false);
    w_createglobwCheckBox->setChecked(false);
    w_createbinmosCheckBox->setChecked(false);
    w_createweightsCheckBox->setChecked(false);
    w_distsetCheckBox->setChecked(false);
    w_messagesTextEdit->clear();
    CB_debloom = 0;
    CB_globalweights = 0;
    CB_binnedmosaics = 0;
    CB_createweights = 0;
    CB_distributesets = 0;
    paint_checkboxes();
    if (science_debloom == 1 || sky_debloom == 1) {
	science_directphot = 0;
	sky_directphot = 0;
    }
    science_debloom = 0;
    sky_debloom = 0;
    
    update_logfile();
    update_w_commandlist();
    statusCPSDebloomingAction->setOn(false);
}


void theliForm::resetfields_w_pushbutton()
{
    w_minoverlapLineEdit->setText("1000");
    w_bloomthreshLineEdit->setText("55000");
    w_messagesTextEdit->clear();
    update_logfile();
    update_w_commandlist();    
}


void theliForm::update_w_commandlist()
{
  QString mainpath, dark, bloomthresh, minoverlap;
  QString bias, flat, science, sky, binscriptpath;
  QString binningscript, instrumentused, standard;
  QString dmin, dmax, darkdir, flatnorm, string;
  QString binscriptconf;
  QDir e;
  int CB_array[5], flag = 0, flag_ini = 0;
  int checkscience, checksky;
  int numinstruments, usebias, checkflatnorm, processsky;
  int checkbiasdark, checkflat, checkstandard, returnflag = 0;
  
  w_commandTextEdit->setReadOnly(false);
  
  repaint_parameter_fields_w();
  
  science_debloom = 0;
  science_directphot = 0;
  sky_debloom = 0;
  sky_directphot = 0;
  
  QRegExp rfint( "[0-9]+" );
  QValidator* validator_int = new QRegExpValidator(rfint, this );
  w_bloomthreshLineEdit->setValidator( validator_int );
  w_minoverlapLineEdit->setValidator( validator_int );
  
  // clear the command list window
  w_commandTextEdit->setText(empty);
  
  // check the status of the check boxes. 
  // write the status into an array for easier handling
  if (w_debloomCheckBox->isChecked()) CB_array[0] = 1; 
  else CB_array[0] = 0;
  if (w_createbinmosCheckBox->isChecked()) CB_array[1] = 1; 
  else CB_array[1] = 0;
  if (w_createglobwCheckBox->isChecked()) CB_array[2] = 1; 
  else CB_array[2] = 0;
  if (w_createweightsCheckBox->isChecked()) CB_array[3] = 1; 
  else CB_array[3] = 0;
  if (w_distsetCheckBox->isChecked()) {
    CB_array[4] = 1; 
    distrib_done = true;
  }
  else {
    CB_array[4] = 0;
    distrib_done = false;
  }
  
  // check that the main path is absolute
  QDir absmainpath = QDir(in_maindirLineEdit->text());
  if (absmainpath.isRelative() && sum(CB_array,5) > 0) {
    warn(w_commandTextEdit,"STOP: The main path given under \"Initialise\" must be absolute!");
    return;
  }

  // the main data directory
  mainpath = in_maindirLineEdit->text();
  
  // the subdirectories
  bias = in_biasdirLineEdit->text();
  dark = in_darkdirLineEdit->text();
  flat = in_flatdirLineEdit->text();
  science = in_sciencedirLineEdit->text();
  sky = in_skydirLineEdit->text();
  standard = in_standarddirLineEdit->text();
  
  QSettings settings (QSettings::Ini);
  QString reductionlog = in_reductionlogLineEdit->text();
  settings.beginGroup("/theli_"+reductionlog+"_");
  settings.beginGroup("/WEIGHTING");
  if (settings.readBoolEntry("/usebias", false)) usebias = 1; 
  else usebias = 0;
  settings.endGroup();
  settings.endGroup();
  
  // these are the normalised directories
  flatnorm = flat;
  flatnorm.append("_norm");
  
  if (usebias == 1) darkdir = bias; 
  else darkdir = dark;
  
  // check if the data directories actually exist
  checkflat = checkdir(mainpath, flat);
  checkscience = checkdir(mainpath, science);
  checksky = checkdir(mainpath, sky);
  checkstandard = checkdir(mainpath, standard);
  checkflatnorm = checkdir(mainpath, flatnorm);
  checkbiasdark = checkdir(mainpath, darkdir);
    
  if (checkscience == 0 && (
	CB_array[0] == 1 || CB_array[1] == 1 || 
	CB_array[3] == 1 || CB_array[4] == 1)) {
      // we don't have to exit in case of the global weights as these 
      // require only flats (unless they need a superflat)
    warn(w_commandTextEdit,"STOP: SCIENCE directory not found.");
    return;
  }
  
  // only process the sky data if the user explicitly requested it
  processsky = 1;
  if (!in_processskyCheckBox->isChecked() ||
      checksky == 0) 
    processsky = 0;
  
  // create the commands and update the command list window
  
  // create debloomed images
  if (CB_array[0] == 1) {
    bloomthresh = w_bloomthreshLineEdit->text();
    get_fitsext_fordebloom(ext_img, ext_imgch);
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("fordebloom");
    
    if (bloomthresh.isEmpty())
      warn(w_commandTextEdit,"STOP: Debloom images: No blooming threshold was specified!");
    else {
      if (flag == 0) check_filepresence(w_commandTextEdit, science, ext_imgch, flag, flag_ini);
      create_scriptcall_3(w_commandTextEdit, 1, "create_debloomedimages_para.sh", 
			  mainpath, science, ext_imgch, bloomthresh);
      science_debloom = 1;
    }
    
    if (processsky == 1 && !bloomthresh.isEmpty()) {
      if (flag == 0) check_filepresence(w_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      create_scriptcall_3(w_commandTextEdit, 1, "create_debloomedimages_para.sh", 
			  mainpath, sky, ext_imgch, bloomthresh);
      sky_debloom = 1;
    } else sky_debloom = 0;
    
    if (checkstandard == 1 && !bloomthresh.isEmpty()) {
      if (flag == 0) check_filepresence(w_commandTextEdit, standard, ext_imgch, flag, flag_ini);
      create_scriptcall_3(w_commandTextEdit, 1, "create_debloomedimages_para.sh", 
			  mainpath, standard, ext_imgch, bloomthresh);
    }
  } 
  else {
    science_debloom = 0;
    sky_debloom = 0;
  }
  
  settings.beginGroup("/theli_"+reductionlog+"_");
  settings.beginGroup("/WEIGHTING");
 
  // create binned previews
  if (CB_array[1] == 1) {
    numinstruments = in_instrumentListBox->numRows();
    if (numinstruments > 0) 
      instrumentused = in_instrumentListBox->currentText();
    binningscript = "make_album_";
    binningscript.append(instrumentused);
    binningscript.append(".sh");
    binscriptconf = "album_";
    binscriptconf.append(instrumentused);
    binscriptconf.append(".conf");
    binscriptpath = scriptdir;
    check_last_char(binscriptpath);
    binscriptpath.append(binningscript);
    get_numchips();
    if (checkfile(binscriptpath) == 0 && numchips.compare("1") != 0) {
      warn(w_commandTextEdit,"STOP: Create binned mosaics: Could not find "+binningscript+"!");
      warn(w_commandTextEdit,"Did you create it and its configuration file "+binscriptconf+"?");
      warn(w_commandTextEdit,"Note that this script is only useful for multi-chip cameras!");
    }
    
    get_fitsext_forweights(ext_img, ext_imgch);
    get_lastchar(ext_imgch);
    if (CB_array[0] == 1 && lastchar.compare("D") != 0) 
      ext_imgch.append("D");
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forweights");
    
    if (flag == 0) check_filepresence(w_commandTextEdit, science, ext_imgch, flag, flag_ini);
    if (numchips.compare("1") != 0 && checkfile(binscriptpath) == 1) {
      create_scriptcall_2(w_commandTextEdit, 0, binningscript, mainpath, science, ext_imgch);
    }
    create_scriptcall_2(w_commandTextEdit, 0, "create_tiff.sh", mainpath, science, ext_imgch);
    
    if (processsky == 1) {
      if (flag == 0) check_filepresence(w_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      if (numchips.compare("1") != 0 && checkfile(binscriptpath) == 1) {
	create_scriptcall_2(w_commandTextEdit, 0, binningscript, mainpath, sky, ext_imgch);
      }
      create_scriptcall_2(w_commandTextEdit, 0, "create_tiff.sh", mainpath, sky, ext_imgch);
    }
    
    if (checkstandard == 1) {
      if (flag == 0) check_filepresence(w_commandTextEdit, standard, ext_imgch, flag, flag_ini);
      if (numchips.compare("1") != 0 && checkfile(binscriptpath) == 1) {
	create_scriptcall_2(w_commandTextEdit, 0, binningscript, mainpath, standard, ext_imgch);
      }
      create_scriptcall_2(w_commandTextEdit, 0, "create_tiff.sh", mainpath, standard, ext_imgch);
    }
  }
  
  // create globalweights
  if (CB_array[2] == 1) {
    dmin = settings.readEntry("/darkmin", "");
    dmax = settings.readEntry("/darkmax", "");
    bool uniformweight = settings.readBoolEntry("uniformweight", false);
    // if the user wants a dark as additional constraint
    if (!uniformweight) {
      if (checkflatnorm == 0 && checkflat == 0) {
	warn(w_commandTextEdit,"STOP: Create global weights: Could not find the directory with");
	warn(w_commandTextEdit,"    normalised flats nor a directory with unnormalised flats.");
	warn(w_commandTextEdit,"    If your flat directory is called e.g. \"FLAT\", then the directory with the normalised flats");
	warn(w_commandTextEdit,"    should be called \"FLAT_norm\", containing files \"FLAT_norm_i.fits\" .");
	warn(w_commandTextEdit,"    If you do not want to use the flat as a basis for the weight, or do not have a flat,");
	warn(w_commandTextEdit,"    you can instead assign to all pixels an initial weight of 1 by setting");
	warn(w_commandTextEdit,"    \"Same weight for all pixels\" in the configuration.");
	warn(w_commandTextEdit,"    Re-activate this task for the changes to take effect.");
	returnflag = 1;
      }
      if (checkbiasdark == 0 && !dmin.isEmpty() && !dmax.isEmpty()) {
	warn(w_commandTextEdit,"STOP: Create global weights: Thresholds for a DARK were supplied");
	warn(w_commandTextEdit,"    in the configuration dialogue, but no DARK directory could be found!");
	returnflag = 1;
      }
    }
    if (returnflag == 0) {
      // If a flat dir is present, but the normalised flat is missing, create it
      if (checkflatnorm == 0 && checkflat == 1) {
	create_scriptcall_1(w_commandTextEdit, 1, "create_norm_para.sh", mainpath, flat);
      }
      // a science directory is not essential here. if it is empty, replace it with
      // an "empty" string.
      QString sciencetmp;
      if (science.compare("") == 0) sciencetmp = "empty";
      else sciencetmp = science;
      create_scriptcall_2(w_commandTextEdit, 1, "create_global_weights_para.sh", 
			  mainpath, flatnorm, sciencetmp);
    }
  }
  
  // create the WEIGHTS
  if (CB_array[3] == 1) {
    get_fitsext_forweights(ext_img, ext_imgch);
    get_lastchar(ext_imgch);
    if (CB_array[0] == 1 && lastchar.compare("D") != 0) 
      ext_imgch.append("D");
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forweights");
    if (flag == 0) check_filepresence(w_commandTextEdit, science, ext_imgch, flag, flag_ini);

    create_scriptcall_1(w_commandTextEdit, 0, "transform_ds9_reg.sh", 
			mainpath, science);
    create_scriptcall_2(w_commandTextEdit, 1, "create_weights_para.sh", 
			mainpath, science, ext_imgch);
    
    if (processsky == 1) {
      if (flag == 0) check_filepresence(w_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      create_scriptcall_1(w_commandTextEdit, 0, "transform_ds9_reg.sh",  
			  mainpath, sky);
      create_scriptcall_2(w_commandTextEdit, 1, "create_weights_para.sh", 
			  mainpath, sky, ext_imgch);
    }
    
    if (checkstandard == 1) {
      if (flag == 0) check_filepresence(w_commandTextEdit, standard, ext_imgch, flag, flag_ini);
      create_scriptcall_1(w_commandTextEdit, 0, "transform_ds9_reg.sh",  
			  mainpath, standard);
      create_scriptcall_2(w_commandTextEdit, 1, "create_weights_para.sh", 
			  mainpath, standard, ext_imgch);
    }
  }
  settings.endGroup();
  settings.endGroup();

  // distribute sets
  if (CB_array[4] == 1) {
    minoverlap = w_minoverlapLineEdit->text();
    get_fitsext_forweights(ext_img,ext_imgch);
    get_lastchar(ext_imgch);
    if (CB_array[0] == 1 && lastchar.compare("D") != 0) 
      ext_imgch.append("D");
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forweights");

    if (minoverlap.isEmpty())
      warn(w_commandTextEdit,"STOP: Distribute sets: You must specify a minimum overlap!");
    if (!minoverlap.isEmpty()) {
      if (flag == 0) check_filepresence(w_commandTextEdit, science, ext_imgch, flag, flag_ini);
      create_scriptcall_3(w_commandTextEdit, 0, "distribute_sets.sh", mainpath, 
			  science, ext_imgch, minoverlap);
    }
    if (!minoverlap.isEmpty() && processsky == 1) {
      if (flag == 0) check_filepresence(w_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      create_scriptcall_3(w_commandTextEdit, 0, "distribute_sets.sh", mainpath, 
			  sky, ext_imgch, minoverlap);
    }
  }
}


void theliForm::repaint_parameter_fields_w()
{
    repaint_parameter_fields_helper(w_distsetCheckBox, w_minoverlapLineEdit);
    repaint_parameter_fields_helper(w_debloomCheckBox, w_bloomthreshLineEdit);
}


void theliForm::exec_w()
{
    QString fullpath, scripts, presentinst, science, sky;
    QString minoverlap;

    int result;
    is_imextension_empty(result);
    if (result == 1) return;

    // if the maximum tolerance of the distribute sets task is larger
    // than 1024, then set it to 1024
    if (w_minoverlapLineEdit->text().toInt() > 1024) 
	w_minoverlapLineEdit->setText("1024");

    // update the params.ini
    modify_paramsini();
    modify_paramset2();

    fullpath = userscriptdir;
    fullpath.append("/");
    fullpath.append("theli_w_comm.sh");
    
    scripts = w_commandTextEdit->text();
    if (scripts.contains("STOP:") != 0) return;
    presentinst = tf_instrumentLineEdit->text();
    
    // write the superscript and execute it
    process_group = "w";
    if (checkdir2(userscriptdir) == 1 && !presentinst.isEmpty() && 
	!scripts.isEmpty()) {
	write_superscriptbody(fullpath, scripts);
    }
    // if the distribute sets task was executed, then
    // rename the SCIENCE path in the initialise section
    if (distrib_done) {
      science = in_sciencedirLineEdit->text();
      sky = in_skydirLineEdit->text();
      in_sciencedirLineEdit->setText(science+"_set_1");

      if (!sky.isEmpty() && in_processskyCheckBox->isChecked())
	in_skydirLineEdit->setText(sky+"_set_1");

      repaint_defaultpaths();
    }
}


void theliForm::kill_w()
{
  QString execstr;
  
  if (started_proc_w == 1) {
    if (proc_w->isRunning()) {
      process_id = proc_w->processIdentifier();
      pid = pid.setNum(process_id);
      execstr = scriptdir;
      execstr.append("/parakill.sh ");
      execstr.append(pid);
      execstr.append(" > /dev/null 2>&1");
      my_system(execstr);
      w_messagesTextEdit->append("All processes killed.");	
    }
    started_proc_w = 0;
  }
}

#endif
