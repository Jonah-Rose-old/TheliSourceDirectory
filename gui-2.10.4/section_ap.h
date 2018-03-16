#ifndef SECTION_AP_H
#define SECTION_AP_H

#include <qmessagebox.h>
#include <stdlib.h>


void theliForm::get_full_instpath(QString &instpath)
{
  QString path, instname;

  instname = tf_instrumentLineEdit->text();

  if (instname.isEmpty()) return;

  if (in_profinstRadioButton->isChecked()) path = scriptdir+"/instruments_professional/";
  if (in_comminstRadioButton->isChecked()) path = scriptdir+"/instruments_commercial/";
  if (in_userinstRadioButton->isChecked()) path = userthelidir+"/instruments_user/";
  instpath = path + instname + ".ini";
}


void theliForm::get_full_aheadpath(QString &aheadpath)
{
  QString path, instname;

  instname = tf_instrumentLineEdit->text();

  if (instname.isEmpty()) return;

  if (in_profinstRadioButton->isChecked()) path = scriptdir+"/instruments_professional/";
  if (in_comminstRadioButton->isChecked()) path = scriptdir+"/instruments_commercial/";
  if (in_userinstRadioButton->isChecked()) path = userthelidir+"/instruments_user/";
  aheadpath = path + instname + ".ahead";
}


void theliForm::clean_refcoordsLineEdit()
{
  QString ra, dec;
  
  ra = ap_refraLineEdit->text();
  dec = ap_refdecLineEdit->text();
  
  if (ra.compare("from header") == 0)	ap_refraLineEdit->clear();
  if (dec.compare("from header") == 0) ap_refdecLineEdit->clear();
}


void theliForm::enable_server()
{
  if (ap_arccatComboBox->currentText() == "SDSS-DR9") ap_arcserverComboBox->setDisabled(true);
  else ap_arcserverComboBox->setDisabled(false);
}


void theliForm::clearscripts_ap_pushbutton()
{
  ap_createsourcecatCheckBox->setChecked(false);
  ap_astrometryCheckBox->setChecked(false);
  ap_absphotom_direct_CheckBox->setChecked(false);
  ap_absphotom_indirect_CheckBox->setChecked(false);
  ap_messagesTextEdit->clear();
  CB_directphot = 0;
  CB_singleastrom = 0;
  CB_absphotom_direct = 0;
  CB_absphotom_indirect = 0;
  CB_astrometry = 0;
  CB_photometry = 0;
  paint_checkboxes();
  science_directphot = 0;
  sky_directphot = 0;
  update_logfile();
  update_ap_commandlist();
  statusCPSDirectphotAction->setOn(false);
}


void theliForm::resetfields_ap_pushbutton()
{
  ap_refraLineEdit->setText("from header");
  ap_refdecLineEdit->setText("from header");
  ap_resolveLineEdit->clear();
  ap_astrommethodComboBox->setCurrentItem(0);
  ap_maglimLineEdit->setText("20");
  ap_refimagedetectthreshLineEdit->setText("10");
  ap_refimagedetectminareaLineEdit->setText("10");
  ap_messagesTextEdit->clear();
  update_logfile();
  update_ap_variousfields();
  update_ap_commandlist();
}


void theliForm::update_ap_variousfields()
{
  QString imagepath, detectthresh, detectminarea;
  QString maglim;
  int checkimagepath;
  
  if (ap_arcserverComboBox->currentItem() <= 8) {
    ap_maglimTextLabel->show();
    ap_radiusTextLabel->show();
    ap_maglimLineEdit->show();
    ap_radiusLineEdit->show();
    ap_resolveTextLabel->show();
    ap_resolveLineEdit->show();
    ap_resolvePushButton->show();
    ap_selectimagePushButton->hide();
    ap_dtdminTextLabel->hide();
    ap_refimagedetectthreshLineEdit->hide();
    ap_refimagedetectminareaLineEdit->hide();
    ap_refimageLineEdit->hide();
    ap_arccatComboBox->show();
    maglim = ap_maglimLineEdit->text();
    if (maglim.isEmpty()) 
      ap_maglimLineEdit->setPaletteBackgroundColor(tfred);
    else ap_maglimLineEdit->setPaletteBackgroundColor(tfwhite);
  }
  
  if (ap_arcserverComboBox->currentItem() == 9) {
    ap_arccatComboBox->hide();
    ap_maglimTextLabel->hide();
    ap_radiusTextLabel->hide();
    ap_maglimLineEdit->hide();
    ap_radiusLineEdit->hide();
    ap_resolveTextLabel->hide();
    ap_resolveLineEdit->hide();
    ap_resolvePushButton->hide();
    ap_selectimagePushButton->show();
    ap_dtdminTextLabel->show();
    ap_refimagedetectthreshLineEdit->show();
    ap_refimagedetectminareaLineEdit->show();
    ap_refimageLineEdit->show();
    imagepath = ap_refimageLineEdit->text();
    QFileInfo imageinfo = QFileInfo(imagepath);
    checkimagepath = checkfile(imagepath);
    detectthresh = ap_refimagedetectthreshLineEdit->text();
    detectminarea = ap_refimagedetectminareaLineEdit->text();
    if (detectthresh.isEmpty()) 
      ap_refimagedetectthreshLineEdit->setPaletteBackgroundColor(tfred);
    else ap_refimagedetectthreshLineEdit->setPaletteBackgroundColor(tfwhite);
    if (detectminarea.isEmpty())
      ap_refimagedetectminareaLineEdit->setPaletteBackgroundColor(tfred);
    else ap_refimagedetectminareaLineEdit->setPaletteBackgroundColor(tfwhite);
    if (imagepath.isEmpty() || checkimagepath == 0 || imageinfo.isDir())
      ap_refimageLineEdit->setPaletteBackgroundColor(tfred);
    if (checkimagepath == 1 && !imageinfo.isDir())
      ap_refimageLineEdit->setPaletteBackgroundColor(tfgreen);
  }
}


void theliForm::validate_refcoords()
{
  QString ra, dec;
  QStringList chars;
  
  ra = ap_refraLineEdit->text();
  dec = ap_refdecLineEdit->text();
  
  ra = ra.stripWhiteSpace();
  dec = dec.stripWhiteSpace();

  chars += "f";
  chars += "r";
  chars += "o";
  chars += "m";
  chars += " ";
  chars += "h";
  chars += "e";
  chars += "a";
  chars += "d";
  
  if (ap_refraLineEdit->isModified() || ap_refdecLineEdit->isModified()) {
    for ( QStringList::Iterator it = chars.begin(); it != chars.end(); ++it ) {
      ra.remove(*it);
      dec.remove(*it);
    }
  }
  
  ap_refraLineEdit->setText(ra);
  ap_refdecLineEdit->setText(dec);
}


void theliForm::update_ap_commandlist()
{
  int CB_array[4], checkstandard, flag = 0;
  int flag_ini = 0;
  int checkscience, checksky, processsky;
  QString call, mainpath, refra, refdec;
  QString science, sky, standard;
  QString tmpscience, tmpsky, maglim;
  QString tmppath, imagepath, detectthresh, detectminarea;
  QString astrommethod, shiftmethod;
  QDir e;
  bool stopcondition, abs_fittingmethod, do_stdastrom;
  
  do_directphot = false;

  do_stdastrom = true;

  ap_commandTextEdit->setReadOnly(false);

  get_numchips();

  check_last_char(scriptdir);
  
  repaint_parameter_fields_ap();
  
  science_directphot = 0;
  sky_directphot = 0;
  
  // check which reference catalogue the user selected
  QSettings settings (QSettings::Ini);
  QString reductionlog = in_reductionlogLineEdit->text();
  settings.beginGroup("/theli_"+reductionlog+"_");
  settings.beginGroup("/ASTROMPHOTOM");
  settings.beginGroup("/absphotom");
  int fittingmethodM2 = settings.readNumEntry("/fittingmethodM2",0);
  if (fittingmethodM2 == 0) abs_fittingmethod = true; //append "P"
  else abs_fittingmethod = false;
  // whether we should do standard star astrometry or not:
  do_stdastrom = settings.readBoolEntry("/wcserrorCheckBox",false);
  settings.endGroup();
  settings.endGroup();
  settings.endGroup();

  QRegExp ri( "^[-]{0,1}[0-9]+" );
  QValidator* validator_int = new QRegExpValidator( ri, this );
  ap_radiusLineEdit->setValidator( validator_int );
  ap_refimagedetectminareaLineEdit->setValidator( validator_int );

  QRegExp rfloat( "^[-]{0,1}[0-9]*[.]{0,1}[0-9]+" );
  QValidator* validator_float = new QRegExpValidator( rfloat, this );
  ap_refimagedetectthreshLineEdit->setValidator( validator_float );
  ap_maglimLineEdit->setValidator( validator_float );
  
  // a very strict validator. works, but if the user deletes the 
  // field he might not be allowed to do so. ODD.   
  // QRegExp rx1( "^[-]{0,1}[0-9]+[.:]{0,1}[0-9]+
  //                 [:]{0,1}[0-9]+[.]{0,1}[0-9]+" );

  refra = ap_refraLineEdit->text();
  refdec = ap_refdecLineEdit->text();

  refra = refra.stripWhiteSpace();
  refdec = refdec.stripWhiteSpace();

  if (!refra.isEmpty() && refdec.isEmpty())
	ap_refdecLineEdit->setPaletteBackgroundColor(tfred);
  if (refra.isEmpty() && !refdec.isEmpty())
	ap_refraLineEdit->setPaletteBackgroundColor(tfred);
  if (refra.isEmpty() && refdec.isEmpty()) {
	ap_refraLineEdit->setPaletteBackgroundColor(tfwhite);
	ap_refdecLineEdit->setPaletteBackgroundColor(tfwhite);
  }
  if (!refra.isEmpty() && !refdec.isEmpty()) {
	ap_refraLineEdit->setPaletteBackgroundColor(tfwhite);
	ap_refdecLineEdit->setPaletteBackgroundColor(tfwhite);
  }
  
  // directories must not accept blank characters
  QRegExp rx2( "^\\S+$" );
  QValidator* validator_string2 = new QRegExpValidator( rx2, this );
  ap_refimageLineEdit->setValidator( validator_string2 );
	    
  // clear the command list window
  ap_commandTextEdit->setText(empty);

  // check the status of the check boxes. 
  // write the status into an array for easier handling
  if (ap_absphotom_indirect_CheckBox->isChecked()) CB_array[0] = 1; 
  else CB_array[0] = 0;
  if (ap_absphotom_direct_CheckBox->isChecked()) CB_array[1] = 1; 
  else CB_array[1] = 0;
  if (ap_createsourcecatCheckBox->isChecked()) CB_array[2] = 1; 
  else CB_array[2] = 0;
  if (ap_astrometryCheckBox->isChecked()) CB_array[3] = 1; 
  else CB_array[3] = 0;

  // check that the main path is absolute
  QDir absmainpath = QDir(in_maindirLineEdit->text());
  if (absmainpath.isRelative() && sum(CB_array,4) > 0) {
    warn(ap_commandTextEdit,"STOP: The main path given under \"Initialise\" must be absolute!");
    return;
  }
  
  if (CB_array[0] == 1 && CB_array[1] == 1) {
    QMessageBox::warning(
        this, "Duplicate absolute photometry",
	QString( "You want to perform both a direct and indirect determination\n"
		 "of the absolute photometric zeropoint. Only one method can be\n"
		 "applied.\n"));
    ap_absphotom_indirect_CheckBox->setChecked(false);
    ap_absphotom_direct_CheckBox->setChecked(false);
    return;
  }

  // the main data directory
  mainpath = in_maindirLineEdit->text();
  
  // the subdirectories
  science   = in_sciencedirLineEdit->text();
  sky = in_skydirLineEdit->text();
  standard  = in_standarddirLineEdit->text();
  
  // check if the data directories actually exist
  checkscience   = checkdir(mainpath, science);
  checksky = checkdir(mainpath, sky);
  checkstandard  = checkdir(mainpath, standard);
  
  if (checkscience == 0 && (
      CB_array[0] == 1 || CB_array[1] == 1 || CB_array[2] == 1 || CB_array[3] == 1)) {
    warn(ap_commandTextEdit,"STOP: SCIENCE directory not found.");
    return;
  }

  // only process the sky data if the user explicitly requested it
  processsky = 1;
  if (!in_processskyCheckBox->isChecked() || checksky == 0) 
    processsky = 0;

  // create the commands and update the command list window
  if (ap_astrommethodComboBox->currentItem() == 0) astrommethod="scamp";
  if (ap_astrommethodComboBox->currentItem() == 1) astrommethod="anet";
  if (ap_astrommethodComboBox->currentItem() == 2) {
	astrommethod="shift";
	shiftmethod="float";
  }
  if (ap_astrommethodComboBox->currentItem() == 3) {
	astrommethod="shift";
	shiftmethod="int";
  }
  if (ap_astrommethodComboBox->currentItem() == 4) astrommethod="xcorr";
  if (ap_astrommethodComboBox->currentItem() == 5) astrommethod="header";

  //#################################################################
  // STOP condition for absolute photometric calibration
  //#################################################################

  if (CB_array[0] == 1) {
    get_fitsext_forweights(ext_img,ext_imgch);
    if (invoked_fromexecall == 1) 
	get_fitsext_fromcheckboxes("forweights");

    stopcondition = false;
    int numfiles = count_files(mainpath+"/WEIGHTS/",ext_imgch+".flag");
    if (numfiles < 1) {
	warn(ap_commandTextEdit,"STOP: Absolute photometry: No FLAG images found.");
	stopcondition = true;
    }
    if (stopcondition) return;
  }

  //#################################################################
  // indirect photometric calibration (external standard fields)
  //#################################################################

  if (CB_array[0] == 1) {
    if (checkstandard == 0) {
      warn(ap_commandTextEdit,"STOP: Absolute photometry (indirect): STANDARD directory not found.");
      warn(ap_commandTextEdit,"      If you want to calibrate directly against SDSS or 2MASS,");
      warn(ap_commandTextEdit,"      then use the direct absolute photometric calibration.");
      return;
    } else {
      // get the standard catalog

      // i think this can be commented out, as here the astrometric refcat is downloaded within scamp
      // if (do_stdastrom) get_astref_catalog_normal(standard,"standard");
      QString scripts = ap_commandTextEdit->text();
      
      // create source cat for the standard field
      if (flag == 0) check_filepresence(ap_commandTextEdit, standard, ext_imgch, flag, flag_ini);
      create_scriptcall_2(ap_commandTextEdit, 1, "create_astromcats_phot_para.sh", 
			  mainpath, standard, ext_imgch);
      if (numchips.compare("1") != 0) { 
	if (do_stdastrom) 
	  create_scriptcall_2(ap_commandTextEdit, 0, "create_scampcats.sh", 
			      mainpath, standard, ext_imgch);
      }
      
      // do the astrometry
      if (flag == 0) check_filepresence(ap_commandTextEdit, science, ext_imgch, flag, flag_ini);
      if (do_stdastrom) 
	create_scriptcall_3(ap_commandTextEdit, 0, "create_scamp.sh", mainpath, 
			    standard, ext_imgch, "photom");
      create_scriptcall_2(ap_commandTextEdit, 1, "create_stdphotom_prepare.sh", 
			  mainpath, standard, ext_imgch);
      create_scriptcall_3(ap_commandTextEdit, 0, "create_abs_photo_info.sh", 
			  mainpath, standard, science, ext_imgch);

      if (processsky == 1) {
	if (flag == 0) check_filepresence(ap_commandTextEdit, sky, ext_imgch, flag, flag_ini);
	create_scriptcall_3(ap_commandTextEdit, 0, "create_abs_photo_info.sh", 
			    mainpath, standard, sky, ext_imgch);
      }
    }
  }
  
  //#################################################################
  // direct photometric calibration (2MASS or SDSS)
  //#################################################################

  if (CB_array[1] == 1) {
    get_astref_catalog_normal(science,"photref");
    get_astref_catalog_normal(science,"science");
    QString scripts = ap_commandTextEdit->text();
    if (flag == 0) check_filepresence(ap_commandTextEdit, science, ext_imgch, flag, flag_ini);
    create_scriptcall_2(ap_commandTextEdit, 1, "create_astromcats_phot_para.sh", 
			mainpath, science, ext_imgch);
    if (numchips.compare("1") != 0) {
      create_scriptcall_2(ap_commandTextEdit, 0, "create_scampcats.sh", 
			  mainpath, science, ext_imgch);
    }
    create_scriptcall_2(ap_commandTextEdit, 0, "create_scamp.sh", 
			mainpath, science, ext_imgch);
    create_scriptcall_2(ap_commandTextEdit, 1, "create_photillcorr_corrcat_para.sh", 
			mainpath, science, ext_imgch);
    create_scriptcall_2(ap_commandTextEdit, 0, "create_photillcorr_getZP.sh", 
			mainpath, science, ext_imgch);
    science_directphot = 1;

    if (processsky == 1) {
      if (flag == 0) check_filepresence(ap_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      get_astref_catalog_normal(sky,"photref");
      get_astref_catalog_normal(sky,"sky");
      QString scripts = ap_commandTextEdit->text();
      create_scriptcall_2(ap_commandTextEdit, 1, "create_astromcats_phot_para.sh", 
			  mainpath, sky, ext_imgch);
      if (numchips.compare("1") != 0) {
	create_scriptcall_2(ap_commandTextEdit, 0, "create_scampcats.sh", 
			    mainpath, sky, ext_imgch);
      }
      create_scriptcall_2(ap_commandTextEdit, 0, "create_scamp.sh", 
			  mainpath, sky, ext_imgch);
      create_scriptcall_2(ap_commandTextEdit, 1, "create_photillcorr_corrcat_para.sh", 
			  mainpath, sky, ext_imgch);
      create_scriptcall_2(ap_commandTextEdit, 0, "create_photillcorr_getZP.sh", 
			  mainpath, sky, ext_imgch);
      sky_directphot = 1;
    }
    do_directphot = true;
  }

  //#################################################################
  // correct CRVAL1/2, CDELT1/2
  // create source cat
  //#################################################################

  if (CB_array[2] == 1) {
    // get image extension
    get_fitsext_forastromcoadd(ext_img, ext_imgch);
    get_lastchar(ext_imgch);
    if (CB_array[1] == 1 && lastchar.compare("P") != 0 && 
	do_directphot && abs_fittingmethod) 
      ext_imgch.append("P");
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forastrometry");

    if (flag == 0) check_filepresence(ap_commandTextEdit, science, ext_imgch, flag, flag_ini);
    if (!refra.isEmpty() && !refdec.isEmpty() &&
	refra.compare("from header") != 0 &&
	refdec.compare("from header") != 0) {
      create_scriptcall_2(ap_commandTextEdit, 1, "correct_crval_para.sh", 
			  mainpath, science, ext_imgch);
    }
    create_scriptcall_2(ap_commandTextEdit, 1, "create_astromcats_para.sh", 
			mainpath, science, ext_imgch);
    if (numchips.compare("1") != 0) { 
      create_scriptcall_2(ap_commandTextEdit, 0, "create_scampcats.sh", 
			  mainpath, science, ext_imgch);
    }
    
    if (processsky == 1) {
      if (flag == 0) check_filepresence(ap_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      if (!refra.isEmpty() && !refdec.isEmpty() &&
	  refra.compare("from header") != 0 &&
	  refdec.compare("from header") != 0) {
	create_scriptcall_2(ap_commandTextEdit, 1, "correct_crval_para.sh", 
			    mainpath, sky, ext_imgch);
      }
      create_scriptcall_2(ap_commandTextEdit, 1, "create_astromcats_para.sh", 
			  mainpath, sky, ext_imgch);
      if (numchips.compare("1") != 0) { 
	create_scriptcall_2(ap_commandTextEdit, 0, "create_scampcats.sh", 
			    mainpath, sky, ext_imgch);
      }
    }
  }
  else ap_refimageLineEdit->setPaletteBackgroundColor(tfred);
  update_ap_variousfields();


  //#################################################################
  // ASTROMETRY
  //#################################################################

  if (CB_array[3] == 1) {

    // the image ending
    get_fitsext_forastromcoadd(ext_img, ext_imgch);
    get_lastchar(ext_imgch);
    if (CB_array[1] == 1 && lastchar.compare("P") != 0 && 
	do_directphot && abs_fittingmethod) 
      ext_imgch.append("P");
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forastrometry");

    // get the reference catalogue automatically only if
    // sky data is reduced as well; otherwise the
    // user has to get it explicitly by hand.
    if (flag == 0) check_filepresence(ap_commandTextEdit, science, ext_imgch, flag, flag_ini);
    if (astrommethod.compare("shift") != 0 &&
	astrommethod.compare("header") != 0 &&
	astrommethod.compare("xcorr") != 0 &&
	processsky == 1) 
      get_astref_catalog_normal(science,"science");
    if (astrommethod.compare("scamp") == 0)
      create_scriptcall_2(ap_commandTextEdit, 0, "create_scamp.sh", 
			  mainpath, science, ext_imgch);
    if (astrommethod.compare("anet") == 0) {
      create_scriptcall_2(ap_commandTextEdit, 1, "create_astrometrynet.sh", 
			  mainpath, science, ext_imgch);
      create_scriptcall_2(ap_commandTextEdit, 0, "create_astrometrynet_photom.sh", 
			  mainpath, science, ext_imgch);
    }
    if (astrommethod.compare("shift") == 0)
      create_scriptcall_3(ap_commandTextEdit, 0, "create_zeroorderastrom.sh", 
			  mainpath, science, ext_imgch, shiftmethod);
    if (astrommethod.compare("xcorr") == 0)
      create_scriptcall_2(ap_commandTextEdit, 0, "create_xcorrastrom.sh", 
			  mainpath, science, ext_imgch);
    if (astrommethod.compare("header") == 0)
      create_scriptcall_2(ap_commandTextEdit, 0, "create_headerastrom.sh", 
			  mainpath, science, ext_imgch);
    create_scriptcall_3(ap_commandTextEdit, 0, "create_stats_table.sh", 
			mainpath, science, ext_imgch, "headers");
    create_scriptcall_1(ap_commandTextEdit, 0, "create_absphotom_coadd.sh", 
			mainpath, science);
    
    if (processsky == 1) {
      if (flag == 0) check_filepresence(ap_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      if (astrommethod.compare("shift") != 0 && 
	  astrommethod.compare("header") != 0)
	get_astref_catalog_normal(sky,"sky");
      if (astrommethod.compare("scamp") == 0)
	create_scriptcall_2(ap_commandTextEdit, 0, "create_scamp.sh", 
			    mainpath, sky, ext_imgch);
      if (astrommethod.compare("anet") == 0) {
	create_scriptcall_2(ap_commandTextEdit, 1, "create_astrometrynet.sh", 
			    mainpath, sky, ext_imgch);
	create_scriptcall_2(ap_commandTextEdit, 0, "create_astrometrynet_photom.sh", 
			    mainpath, sky, ext_imgch);
      }
      if (astrommethod.compare("shift") == 0) 
	create_scriptcall_3(ap_commandTextEdit, 0, "create_zeroorderastrom.sh", 
			    mainpath, sky, ext_imgch, shiftmethod);
      if (astrommethod.compare("xcorr") == 0)
	create_scriptcall_2(ap_commandTextEdit, 0, "create_xcorrastrom.sh", 
			  mainpath, sky, ext_imgch);
      if (astrommethod.compare("header") == 0)
	create_scriptcall_2(ap_commandTextEdit, 0, "create_headerastrom.sh", 
			    mainpath, sky, ext_imgch);
      create_scriptcall_3(ap_commandTextEdit, 0, "create_stats_table.sh", 
			  mainpath, sky, ext_imgch, "headers");
      create_scriptcall_1(ap_commandTextEdit, 0, "create_absphotom_coadd.sh", 
			  mainpath, sky);
    }
  }
}


void theliForm::resolve_target()
{
  FILE *of;
  QString target, command, execstr;

  unresolved = 0;

  target = ap_resolveLineEdit->text();

  // only do something if the user entered a target
  if (!target.isEmpty()) {

    command  = bindir+"/sesame -oi \""+target+"\" >& "+usertmpdir+"/sesame.out; ";
    command += "gawk '($1==\"%J\" || $0~\"Nothing found\" || $0~\"Unknown host\")' ";
    command += usertmpdir+"/sesame.out ";
    command += "| gawk '{if ($1==\"%J\") print $2, $3; if ($0~\"Nothing found\") print \"null null\"; if ($0~\"Unknown host\") print \"no internet\"}'";

    // we have to put the command in a shell as otherwise QProcess does not start;
    of = fopen(usertmpdir+"/theli_resolvetarget.sh", "w");
    fprintf(of, "#!/bin/bash\n");
    fprintf(of, "%s\n", command.ascii());
    fclose(of);
    QString fullpath = usertmpdir+"/theli_resolvetarget.sh";
    execstr = "chmod +x "+fullpath;
    my_system(execstr);

    proc_resolve = new QProcess( this );
    proc_resolve->addArgument( fullpath );

    connect(proc_resolve, SIGNAL(readyReadStdout()), this, SLOT(resolve_target_readstdout()));
    connect(proc_resolve, SIGNAL(processExited()), this, SLOT(resolve_target_exit()));
    if (!proc_resolve->start()) {
      printf("Could not start target resolution process!\n");
      return;
    }
    ap_refraLineEdit->setText("Resolving target");
    ap_refdecLineEdit->setText("Please wait ...");
  }
}


void theliForm::resolve_target_readstdout()
{
  QString radec, ra, dec, execstr;
  QByteArray data = proc_resolve->readStdout();
  int l;

  unresolved = 1;

  radec = QString(data);
  radec = radec.simplifyWhiteSpace();

  // this is in case the user enters a string that cannot be interpreted by sesame
  if (radec.isEmpty()) {
    ap_refraLineEdit->clear();
    ap_refdecLineEdit->clear();
    QMessageBox::warning(
        this, "Target unresolved",
	QString( "The target could not be found in the\n"
		 "Simbad, NED and VizieR databases.\n"
		 "Enter the coordinates manually.\n"));
    return;
  } 

  ra  = radec.section(' ', 0, 0);
  dec = radec.section(' ', 1, 1);

  if (ra.compare("null") == 0 && dec.compare("null") == 0) {
    ap_refraLineEdit->clear();
    ap_refdecLineEdit->clear();
    QMessageBox::warning(
        this, "Target unresolved",
	QString( "The target could not be found in the\n"
		 "Simbad, NED and VizieR databases.\n"
		 "Enter the coordinates manually.\n"));
  } 
  else if (ra.compare("no") == 0 && dec.compare("internet") == 0) {
    ap_refraLineEdit->clear();
    ap_refdecLineEdit->clear();
    QMessageBox::warning(
        this, "Bad internet connection",
	QString( "Could not connect to the database server.\n"
		 "Check your internet connection and firewall settings,\n"
		 "or enter the coordinates manually.\n"));
  }
  else {
    execstr = bindir+"/decimaltohms "+ra;
    my_fscanf(execstr, ra);
    l = ra.length() - 5;
    ra.truncate(l);

    execstr = bindir+"/decimaltodms "+dec;
    my_fscanf(execstr, dec);
    l = dec.length() - 5;
    dec.truncate(l);

    // target was successfully resolved
    ap_refraLineEdit->setText(ra);
    ap_refdecLineEdit->setText(dec);
    ap_resolveLineEdit->clear();
  }
}


void theliForm::resolve_target_exit()
{
  if (unresolved == 0) { 
    ap_refraLineEdit->clear();
    ap_refdecLineEdit->clear();
    QMessageBox::warning(
        this, "Target unresolved",
	QString( "The target could not be found in the\n"
		 "Simbad, NED and VizieR databases.\n"
		 "Enter the coordinates manually.\n"));
  } 
}


void theliForm::get_astref_catalog_button()
{
  int checkscience, checkimagepath, flag = 0, flag_ini = 0;
  QString mainpath, refra, refdec, command;
  QString science, server;
  QString tmpscience, tmpsky; 
  QString catalogtype, refra2, refdec2, maglim;
  QString tmppath, imagepath, detectthresh, detectminarea;
  QString fullpath, scripts, presentinst;
  QDir e;
  
  get_numchips();
  
  check_last_char(scriptdir);

  refra  = ap_refraLineEdit->text();
  refdec = ap_refdecLineEdit->text();
  refra  = refra.stripWhiteSpace();
  refdec = refdec.stripWhiteSpace();

  check_coordinate_format();

  // the data directories
  mainpath = in_maindirLineEdit->text();
  science = in_sciencedirLineEdit->text();
  
  if (mainpath.isEmpty()) mainpath="_";
  if (science.isEmpty()) science="_";

  // check if the data directories actually exist
  checkscience = checkdir(mainpath, science);
  if (checkscience == 0 &&
      ((refra.compare("from header") == 0 && refdec.compare("from header") == 0) ||
       (refra.isEmpty() && refdec.isEmpty()))) {
    warn(ap_commandTextEdit,"STOP: SCIENCE directory not found.");
    return;
  }

  switch(ap_arcserverComboBox->currentItem()) {
  case 0: server = "vizier.u-strasbg.fr"; break;
  case 1: server = "vizier.cfa.harvard.edu"; break;
  case 2: server = "vizier.hia.nrc.ca"; break;
  case 3: server = "vizier.nao.ac.jp"; break;
  case 4: server = "vizier.iucaa.ernet.in"; break;
  case 5: server = "data.bao.ac.cn"; break;
  case 6: server = "vizier.ast.cam.ac.uk"; break;
  case 7: server = "www.ukirt.jach.hawaii.edu"; break;
  case 8: server = "vizier.inasan.ru"; break;
  default: server = "vizier.u-strasbg.fr"; break;
  }
  
  get_fitsext_forastromcoadd(ext_img, ext_imgch);
  get_lastchar(ext_imgch);
  
  // if reference catalogue is retrieved but not based on 
  // coordinates in the image header, then we can tolerate
  // an empty extension string
  if (ext_imgch.isEmpty()) {
    if (refra.compare("from header") != 0 &&
	refdec.compare("from header") != 0)
      ext_imgch = "_";
    else {
      int result;
      is_imextension_empty(result);
      if (result == 1)
	return;
    }
  }
  
  if (invoked_fromexecall == 1) 
    get_fitsext_fromcheckboxes("forastrometry");
  
  // handle the catalogue creation
  if (refra.isEmpty() || refra.compare("from header") == 0) refra2 = "header";
  else refra2 = refra;
  if (refdec.isEmpty() || refdec.compare("from header") == 0) refdec2 = "header";
  else refdec2 = refdec;
  maglim = ap_maglimLineEdit->text();
  detectthresh = ap_refimagedetectthreshLineEdit->text();
  detectminarea = ap_refimagedetectminareaLineEdit->text();
  imagepath = ap_refimageLineEdit->text();
  checkimagepath = checkfile(imagepath);
  if (imagepath.isEmpty() || checkimagepath == 0) 
    ap_refimageLineEdit->setPaletteBackgroundColor(tfred);
  if (checkimagepath == 1) 
    ap_refimageLineEdit->setPaletteBackgroundColor(tfgreen);
  tmppath = mainpath;
  check_last_char(mainpath);
  tmppath.append(science);
  if (ap_arcserverComboBox->currentItem() <= 8 && !maglim.isEmpty()) {
    catalogtype = ap_arccatComboBox->currentText();
    if (flag == 0 && 
	(refra.isEmpty() || refra.compare("from header") == 0) &&
	(refdec.isEmpty() || refdec.compare("from header") == 0)) {
      check_filepresence(ap_commandTextEdit, science, ext_imgch, flag, flag_ini);
    }
    create_scriptcall_4ap(command, 0, "create_astrorefcat_fromWEB.sh", 
			  mainpath, science, ext_imgch, catalogtype, server);
  }
  imagepath = ap_refimageLineEdit->text();
  QFileInfo imageinfo = QFileInfo(imagepath);
  if (ap_arcserverComboBox->currentItem() == 9) {
    if (!detectthresh.isEmpty() && !detectminarea.isEmpty() &&
	!imagepath.isEmpty() && checkfile(imagepath) == 1 && 
	!imageinfo.isDir()) {
      create_scriptcall_3ap(command, 0, "create_astrorefcat_fromIMAGE.sh", 
			    imagepath, detectthresh, detectminarea, mainpath+"/"+science+"/cat");
    }
    else {
      if (detectthresh.isEmpty())
	warn(ap_commandTextEdit,"STOP: Create source cat: Detection threshold is empty!");
      if (detectminarea.isEmpty())
	warn(ap_commandTextEdit,"STOP: Create source cat: Detection min-area is empty!");
      if (imagepath.isEmpty() || checkfile(imagepath) == 0 || imageinfo.isDir())
	warn(ap_commandTextEdit,"STOP: Create source cat: The path you entered for the reference catalog image is incomplete.");
      return;
    }
  }
  
  // update the params.ini
  modify_paramsini();
  modify_paramset2();
  
  fullpath = userscriptdir;
  fullpath.append("/");
  fullpath.append("theli_ap_comm.sh");
  
  scripts = ap_commandTextEdit->text();
  if (scripts.contains("STOP:") != 0) return;
  
  scripts = command;
  presentinst = tf_instrumentLineEdit->text();
  
  // check the network connection
  if (ap_arcserverComboBox->currentItem() <= 8 && 
      scripts.contains("create_astrorefcat_fromWEB.sh")) {
    if (!test_network()) {
      ap_commandTextEdit->clear();
      ap_createsourcecatCheckBox->setChecked(false);
      warn(ap_commandTextEdit,"STOP: Get catalog: Could not connect to the data server!");
      warn(ap_commandTextEdit,"Either your network connection or the server is down.");
      warn(ap_commandTextEdit,"Try connecting to a different server.");
      return;
    }
  }
  
  // write the superscript and execute it
  process_group = "ap";
  if (checkdir2(userscriptdir) == 1 && !presentinst.isEmpty() && 
      !scripts.isEmpty()) {	
    write_superscriptbody(fullpath, scripts);
  }
}


void theliForm::check_coordinate_format()
{
  QString refra = ap_refraLineEdit->text();
  QString refdec = ap_refdecLineEdit->text();

  refra = refra.stripWhiteSpace();
  refdec = refdec.stripWhiteSpace();

  // make sure that a floating point number was entered (if not hh:mm:ss)
  if (refra.contains(':') == 0 && !refra.isEmpty() &&
      refra.compare("from header") != 0 && refra.contains('.') == 0 ) {
    refra.append(".0");
    ap_refraLineEdit->setText(refra);
  }
  if (refdec.contains(':') == 0 && !refdec.isEmpty() &&
      refdec.compare("from header") != 0 && refdec.contains('.') == 0 ) {
    refdec.append(".0");
    ap_refdecLineEdit->setText(refdec);
  }
  if (refra.contains(':') == 1 || refdec.contains(':') == 1) {
    QMessageBox::warning(
        this, "Coordinate format error",
	QString( "The correct format for the reference coordinates is\n"
		 "   hh:mm:ss   (right ascension)\n"
		 "   dd:mm:ss   (declination)\n"
		 "A decimal representation (in degrees) is also valid:\n"
		 "   ddd.ddd    (for both coordinates)\n"));
    return;
  }
}


void theliForm::get_astref_catalog_normal(QString imagedir, QString type)
{
  int checkimagedir, checkimagepath;
  QString mainpath, refra, refdec, command;
  QString server, tmppath, imagepath, detectthresh, detectminarea;; 
  QString catalogtype, refra2, refdec2, maglim;
  QString fullpath, scripts, presentinst;
  QDir e;
  
  get_numchips();
  
  check_last_char(scriptdir);
  
  refra  = ap_refraLineEdit->text();
  refdec = ap_refdecLineEdit->text();
  refra  = refra.stripWhiteSpace();
  refdec = refdec.stripWhiteSpace();

  check_coordinate_format();

  // the data directories
  mainpath = in_maindirLineEdit->text();
  if (type.compare("science") == 0)   imagedir = in_sciencedirLineEdit->text();
  if (type.compare("standard") == 0)  imagedir = in_standarddirLineEdit->text();
  if (type.compare("sky") == 0) imagedir = in_skydirLineEdit->text();

  // check if the data directories actually exist
  checkimagedir = checkdir(mainpath, imagedir);
  if (checkimagedir == 0) {
    warn(ap_commandTextEdit,"STOP: "+type.upper()+" directory not found.");
    return;
  }

  switch(ap_arcserverComboBox->currentItem()) {
    case 0: server = "vizier.u-strasbg.fr"; break;
    case 1: server = "vizier.cfa.harvard.edu"; break;
    case 2: server = "vizier.hia.nrc.ca"; break;
    case 3: server = "vizier.nao.ac.jp"; break;
    case 4: server = "vizier.iucaa.ernet.in"; break;
    case 5: server = "data.bao.ac.cn"; break;
    case 6: server = "vizier.ast.cam.ac.uk"; break;
    case 7: server = "www.ukirt.jach.hawaii.edu"; break;
    case 8: server = "vizier.inasan.ru"; break;
    default: server = "vizier.u-strasbg.fr"; break;
  }

  get_fitsext_forastromcoadd(ext_img, ext_imgch);
  get_lastchar(ext_imgch);

  bool abs_fittingmethod;
  QSettings settings (QSettings::Ini);
  QString reductionlog = in_reductionlogLineEdit->text();
  settings.beginGroup("/theli_"+reductionlog+"_");
    settings.beginGroup("/ASTROMPHOTOM");
      settings.beginGroup("/absphotom");
        int fittingmethodM2 = settings.readNumEntry("/fittingmethodM2",0);
	if (fittingmethodM2 == 0) abs_fittingmethod = true; //append "P"
	else abs_fittingmethod = false;
      settings.endGroup();
    settings.endGroup();
  settings.endGroup();

  if (ap_absphotom_direct_CheckBox->isChecked() && lastchar.compare("P") != 0 && 
      do_directphot && abs_fittingmethod) ext_imgch.append("P");
  if (invoked_fromexecall == 1)
    get_fitsext_fromcheckboxes("forastrometry");
  
  // handle the catalogue creation
  if (refra.isEmpty() || refra.compare("from header") == 0) refra2 = "header";
  else refra2 = refra;
  if (refdec.isEmpty() || refdec.compare("from header") == 0) refdec2 = "header";
  else refdec2 = refdec;
  maglim = ap_maglimLineEdit->text();
  detectthresh = ap_refimagedetectthreshLineEdit->text();
  detectminarea = ap_refimagedetectminareaLineEdit->text();
  imagepath = ap_refimageLineEdit->text();
  checkimagepath = checkfile(imagepath);
  if (imagepath.isEmpty() || checkimagepath == 0) 
    ap_refimageLineEdit->setPaletteBackgroundColor(tfred);
  if (checkimagepath == 1) 
    ap_refimageLineEdit->setPaletteBackgroundColor(tfgreen);
  tmppath = mainpath;
  check_last_char(mainpath);
  tmppath.append(imagedir);
  
  if (!maglim.isEmpty() && type.compare("photref") == 0) {
    if (ap_arcserverComboBox->currentItem() > 8) server = "vizier.u-strasbg.fr";
    create_scriptcall_3(ap_commandTextEdit, 0, "create_photorefcat_fromWEB.sh", 
			mainpath, imagedir, ext_imgch, server);
  }

  if (type.compare("photref") != 0) {
    if ((ap_arcserverComboBox->currentItem() <= 8 && !maglim.isEmpty()) ||
	(ap_arcserverComboBox->currentItem() == 9 && type.compare("standard") == 0 && !maglim.isEmpty())) {
      if (type.compare("standard") == 0) server = "vizier.u-strasbg.fr";
      catalogtype = ap_arccatComboBox->currentText();
      if (type.compare("standard") != 0) 
	create_scriptcall_4(ap_commandTextEdit, 0, "create_astrorefcat_fromWEB.sh", 
			    mainpath, imagedir, ext_imgch, catalogtype, server);
      else
	// add an extra argument (which will trigger a different name for the standard star catalog)
	create_scriptcall_5(ap_commandTextEdit, 0, "create_astrorefcat_fromWEB.sh", 
			    mainpath, imagedir, ext_imgch, catalogtype, server, "standard");
    }
    imagepath = ap_refimageLineEdit->text();
    QFileInfo imageinfo = QFileInfo(imagepath);
    if (ap_arcserverComboBox->currentItem() == 9 && type.compare("standard") != 0) {
      if (!detectthresh.isEmpty() && !detectminarea.isEmpty() &&
	  !imagepath.isEmpty() && checkfile(imagepath) == 1 && 
	  !imageinfo.isDir()) {
	create_scriptcall_2(ap_commandTextEdit, 0, "create_astrorefcat_fromIMAGE.sh", 
			    imagepath, detectthresh, detectminarea);
      }
      else {
	if (detectthresh.isEmpty())
	  warn(ap_commandTextEdit,"STOP: Create source cat: Detection threshold is empty!");
	if (detectminarea.isEmpty())
	  warn(ap_commandTextEdit,"STOP: Create source cat: Detection min-area is empty!");
	if (imagepath.isEmpty() || checkfile(imagepath) == 0 || imageinfo.isDir())
	  warn(ap_commandTextEdit,"STOP: Create source cat: The path you entered for the reference catalog image is incomplete.");
	return;
      }
    }
  }
    
  // update the params.ini
  modify_paramsini();
  modify_paramset2();
  
  fullpath = userscriptdir;
  fullpath.append("/");
  fullpath.append("theli_ap_comm.sh");
  
  scripts = ap_commandTextEdit->text();
  if (scripts.contains("STOP:") != 0) return;
  
  presentinst = tf_instrumentLineEdit->text();
  
  // check the network connection
  if (ap_arcserverComboBox->currentItem() <= 8) {
    if (!test_network()) {
      ap_commandTextEdit->clear();
      ap_createsourcecatCheckBox->setChecked(false);
      warn(ap_commandTextEdit,"STOP: Get catalog: Could not connect to the data server!");
      warn(ap_commandTextEdit,"Either your network connection or the server is down.");
      warn(ap_commandTextEdit,"Try connecting to a different server.");
      return;
    }
  }
}


void theliForm::repaint_parameter_fields_ap()
{
  if (ap_arcserverComboBox->currentItem() != 9) {
    repaint_parameter_fields_helper(ap_createsourcecatCheckBox, ap_maglimLineEdit);
  }
  if (ap_arcserverComboBox->currentItem() == 9 ) {
    repaint_parameter_fields_helper(ap_createsourcecatCheckBox, ap_refimagedetectthreshLineEdit);
    repaint_parameter_fields_helper(ap_createsourcecatCheckBox, ap_refimagedetectminareaLineEdit);
    repaint_parameter_fields_helper(ap_createsourcecatCheckBox, ap_refimageLineEdit);
  }
  update_ap_variousfields();
}


void theliForm::exec_ap()
{
  int answer;
  QString fullpath, scripts, presentinst;
  QString refra, refdec;
  
  int result;
  is_imextension_empty(result);
  if (result == 1) return;
  
  // update the params.ini
  modify_paramsini();
  modify_paramset2();
  
  check_coordinate_format();
  
  answer = 0;
  
  fullpath = userscriptdir;
  fullpath.append("/");
  fullpath.append("theli_ap_comm.sh");
  
  scripts = ap_commandTextEdit->text();
  if (scripts.contains("STOP:") != 0) return;
  presentinst = tf_instrumentLineEdit->text();
  
  // check the network connection
  if (ap_arcserverComboBox->currentItem() <= 8 && 
      scripts.contains("create_astrorefcat_fromWEB.sh")) {
    if (!test_network()) {
      ap_commandTextEdit->clear();
      ap_createsourcecatCheckBox->setChecked(false);
      warn(ap_commandTextEdit,"STOP: Create source cat: Could not connect to the CDS-server!");
      warn(ap_commandTextEdit,"Either your network connection or the CDS-server is down.");
      warn(ap_commandTextEdit,"If you want to proceed, delete the command starting with \"create_astrorefcat_fromWEB.sh\".");
      return;
    }
  }
  
  // write the superscript and execute it
  process_group = "ap";
  if (checkdir2(userscriptdir) == 1 && !presentinst.isEmpty() && 
      !scripts.isEmpty()) {	
    QString crvalscript = scripts;
    refra = ap_refraLineEdit->text();
    refdec = ap_refdecLineEdit->text();
    refra = refra.stripWhiteSpace();
    refdec = refdec.stripWhiteSpace();
    if (crvalscript.find("correct_crval_para.sh") != -1) {
      answer = QMessageBox::warning(
		this, "Overwrite CRVAL1/2 and CDi_j keywords?",
		QString( "Do you want to overwrite the CRVAL1/2 keywords in the FITS\n"
                         "headers with the values for RA and DEC entered above?\n"
			 "Optionally, you can also force the CD matrix to have North up and East left.\n\n" ),
		"Update RA/DEC", "Update RA/DEC, reset CD matrix", "Leave header unchanged", 0, 2);
    }

    QString newscripts = "";
    QStringList list = list.split("\n", scripts);
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
      QString line = *it;
      if (!line.contains("correct_crval_para.sh")) {
	newscripts.append(line);
	newscripts.append("\n");
      }
      else {
	// overwrite RA/DEC
	if (answer == 0) {
	  newscripts.append(line);
	  newscripts.append(" crval\n");
	}
	// overwrite RA/DEC and CD matrix
	if (answer == 1) {
	  newscripts.append(line);
	  newscripts.append(" crval+cd\n");
	}
	// if (answer == 2) do nothing (i.e. the correct_crval_para.sh line is removed
      }
    }
    write_superscriptbody(fullpath, newscripts);
  }
}


void theliForm::kill_ap()
{
  QString execstr;
  
  if (started_proc_ap == 1) {
    if (proc_ap->isRunning()) {
      process_id = proc_ap->processIdentifier();
      pid = pid.setNum(process_id);
      execstr = scriptdir;
      execstr.append("/parakill.sh ");
      execstr.append(pid);
      execstr.append(" > /dev/null 2>&1");
      my_system(execstr);
      ap_messagesTextEdit->append("All processes killed.");	
    }
    started_proc_ap = 0;
  }
}

#endif
