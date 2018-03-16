#ifndef SECTION_CC_H
#define SECTION_CC_H

void theliForm::clearscripts_cc_pushbutton()
{
  cc_createskysubCheckBox->setChecked(false);
  cc_coaddCheckBox->setChecked(false);
  cc_resolvelinksCheckBox->setChecked(false);
  cc_messagesTextEdit->clear();
  CB_skysub = 0;
  CB_createcoadd = 0;
  CB_resolvelinks = 0;
  paint_checkboxes();
  update_logfile();
  update_cc_commandlist();
}


void theliForm::resetfields_cc_pushbutton()
{
  cc_messagesTextEdit->clear();
  update_logfile();
  update_cc_commandlist();
}


void theliForm::update_cc_commandlist()
{
  int CB_array[3], skychipnum, buttonID, flag = 0, flag_ini = 0;
  int checkscience, checksky, checkresolvedir, processsky;
  QString call, mainpath, chips;
  QString science, sky, refra, refdec, pixscale;
  QString skybacksize, constskychip;
  QString skysubdetectthresh, skysubdetectminarea;
  QString resolvedir, ident, ext_imgch_orig;
  QString execstr, manual;
  QString filterthreshold, smoothedge;
  QString cc_reductionlog;
  QDir e;
  QFile f;
  
  QSettings settings (QSettings::Ini);
  
  cc_commandTextEdit->setReadOnly(false);
  
  // directories must not accept blank characters
  QRegExp rx3( "^\\S+$" );
  QValidator* validator_string3 = new QRegExpValidator( rx3, this );
  cc_resolvedirLineEdit->setValidator( validator_string3 );
  
  // if entered, check if reference coordinates are in decimal or 
  // hh:mm:ss format.
  // if in hh:mm:ss format, convert them do decimal.
  // we do a simple check only, i.e. searching for the ":" in the string.
  
  // clear the command list window
  cc_commandTextEdit->setText(empty);
  
  // check the status of the check boxes. 
  // write the status into an array for easier handling
  if (cc_createskysubCheckBox->isChecked()) CB_array[0] = 1; 
  else CB_array[0] = 0;
  if (cc_coaddCheckBox->isChecked()) CB_array[1] = 1; 
  else CB_array[1] = 0;
  if (cc_resolvelinksCheckBox->isChecked()) CB_array[2] = 1; 
  else CB_array[2] = 0;
  
  // check that the main path is absolute
  QDir absmainpath = QDir(in_maindirLineEdit->text());
  if (absmainpath.isRelative() && sum(CB_array,3) > 0) {
    warn(cc_commandTextEdit,"STOP: The main path given under \"Initialise\" must be absolute!");
    return;
  }
  
  // the main data directory
  mainpath = in_maindirLineEdit->text();
  
  // the subdirectories
  science = in_sciencedirLineEdit->text();
  sky = in_skydirLineEdit->text();
  
  resolvedir = cc_resolvedirLineEdit->text();
  
  // check if the data directories actually exist
  checkscience = checkdir(mainpath, science);
  checksky = checkdir(mainpath, sky);
  
  if (checkscience == 0 && (
      CB_array[0] == 1 || CB_array[1] == 1)) {
    warn(cc_commandTextEdit,"STOP: SCIENCE directory not found.");
    return;
  }
  
  // only process the sky data if the user explicitly requested it
  processsky = 1;
  if (!in_processskyCheckBox->isChecked() ||
      checksky == 0) 
    processsky = 0;
  
  settings.beginGroup("/theligeneral_");
  cc_reductionlog = settings.readEntry("/lastlog", "noname");
  settings.endGroup();
  
  // read optional arguments
  settings.beginGroup("/theli_"+cc_reductionlog+"_");
  settings.beginGroup("/COADDITION");
  settings.beginGroup("/skysub");
  buttonID = settings.readNumEntry("/radiobuttonID", 0);
  manual = settings.readEntry("/manual", "");
  skychipnum = settings.readNumEntry("/chip", 0);
  settings.endGroup();
  settings.beginGroup("/coaddition");
  refra = settings.readEntry("/ra", "");
  refdec = settings.readEntry("/dec", "");
  pixscale = settings.readEntry("/pixscale", "");
  ident = settings.readEntry("/id", "");
  filterthreshold = settings.readEntry("/filterthreshold", "");
  smoothedge = settings.readEntry("/smoothedge", "");
  settings.endGroup();
  settings.endGroup();
  settings.endGroup();
  
  // transform hh:mm:ss format to dd.ddd (if applicable)
  QRegExp rx4( "[0-9-]+[:][0-9]+[:][0-9.]+" );
  if (!refra.isEmpty()) {
    QString refstr = refra;
    if (rx4.search(refstr) == 0) {
      execstr = bindir;
      execstr.append("/hmstodecimal ");
      execstr.append(refra);
      my_fscanf(execstr, refra);
    }
  }
  if (!refdec.isEmpty()) {
    QString refstr = refdec;
    if (rx4.search(refstr) == 0) {
      execstr = bindir;
      execstr.append("/dmstodecimal ");
      execstr.append(refdec);
      my_fscanf(execstr, refra);
    }
  }  
  
  
  // create the commands and update the command list window
  
  // sky subtraction
  
  // sky modelling
  if (CB_array[0] == 1 && buttonID == 0) {
    get_fitsext_forastromcoadd(ext_img, ext_imgch);
    get_lastchar(ext_imgch);
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forastrometry");
    
    if (flag == 0) check_filepresence(cc_commandTextEdit, science, ext_imgch, flag, flag_ini);
      create_scriptcall_2(cc_commandTextEdit, 1, "create_skysub_para.sh", 
			  mainpath, science, ext_imgch);
    
    if (processsky == 1) {
      if (flag == 0) check_filepresence(cc_commandTextEdit, sky, ext_imgch, flag, flag_ini);
	create_scriptcall_2(cc_commandTextEdit, 1, "create_skysub_para.sh", 
			    mainpath, sky, ext_imgch);
    }
  }
  
  // subtract a constant sky
  if (CB_array[0] == 1 && buttonID != 0) {
    if (skychipnum == 0 && manual.isEmpty() && buttonID == 1) 
      constskychip = "ALL";
    if (skychipnum > 0 && manual.isEmpty() && buttonID == 1) 
      constskychip.setNum(skychipnum);
    if (!manual.isEmpty() && buttonID == 1) constskychip = "MANUAL";
    get_fitsext_forastromcoadd(ext_img, ext_imgch);
    get_lastchar(ext_imgch);
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forastrometry");
    if (flag == 0) check_filepresence(cc_commandTextEdit, science, ext_imgch, flag, flag_ini);

    // check if the file with manual sky values is present
    // (can be missing if data is manually copied around)
    QFile skyvalues(mainpath+"/"+science+"/skybackground.use");
    if (!skyvalues.exists() && constskychip.compare("MANUAL") == 0) 
      warn(cc_commandTextEdit,"STOP: Sky sub: Missing config: Reconfigure the manual sky subtraction.");

    create_scriptcall_1(cc_commandTextEdit, 0, "create_skysubconst_clean.sh", mainpath, science);
    create_scriptcall_3(cc_commandTextEdit, 1, "create_skysubconst_para.sh", 
			mainpath, science, ext_imgch, constskychip);
    
    if (processsky == 1) {
      if (flag == 0) check_filepresence(cc_commandTextEdit, sky, ext_imgch, flag, flag_ini);
      QFile skyvalues(mainpath+"/"+sky+"/skybackground.use");
      if (!skyvalues.exists() && constskychip.compare("MANUAL") == 0 ) 
	warn(cc_commandTextEdit,"STOP: Sky sub: Missing config: Reconfigure the manual sky subtraction.");
      create_scriptcall_1(cc_commandTextEdit, 0, "create_skysubconst_clean.sh", mainpath, sky);
      create_scriptcall_3(cc_commandTextEdit, 1, "create_skysubconst_para.sh", 
			  mainpath, sky, ext_imgch, constskychip);
    }
  }
  
  
  // prepare coaddition, resample images, stack
  if (ident.isEmpty()) ident = "default";
  
  if (CB_array[1] == 1) {
    get_fitsext_forastromcoadd(ext_img, ext_imgch);
    if (invoked_fromexecall == 1) 
      get_fitsext_fromcheckboxes("forastrometry");
    
    // check if the headers and the weights exist, and complain if not
    QDir testsciencedir, testweightsdir, testheadersdir;
    int weights_check = 0, headers_check = 0, science_check = 0;
    bool stopcondition = false;
    testsciencedir.setPath(mainpath+"/"+science+"/");
    testweightsdir.setPath(mainpath+"/WEIGHTS/");
    testheadersdir.setPath(mainpath+"/"+science+"/headers/");
    QStringList sciencelist = testsciencedir.entryList("*"+ext_imgch+".fits");
    QStringList weightslist = testweightsdir.entryList("*"+ext_imgch+".weight.fits");
    QStringList headerslist = testheadersdir.entryList("*.head");
    QString science1comp, science2comp, weightcomp, headercomp;
    for ( QStringList::Iterator sci_it = sciencelist.begin(); 
	  sci_it != sciencelist.end(); ++sci_it ) {
      science_check++;
      science1comp = *sci_it;
      science2comp = *sci_it;
      science1comp.replace(".fits","");
      science2comp.replace(ext_imgch+".fits","");
      for ( QStringList::Iterator wgt_it = weightslist.begin(); 
	    wgt_it != weightslist.end(); ++wgt_it ) {
	weightcomp = *wgt_it;
	weightcomp.replace(".weight.fits","");
	if ( science1comp.compare(weightcomp) == 0) weights_check++;
      }
      for ( QStringList::Iterator hdr_it = headerslist.begin(); 
	    hdr_it != headerslist.end(); ++hdr_it ) {
	headercomp = *hdr_it;
	headercomp.replace(".head","");
	if ( science2comp.compare(headercomp) == 0) headers_check++;
      }
    }
    if (weights_check != science_check) {
      warn(cc_commandTextEdit,"STOP: Coaddition: Not all science images have weights!");
      stopcondition = true;
    }
    if (headers_check != science_check) {
      warn(cc_commandTextEdit,"STOP: Coaddition: Not all science images have separate astrometric headers!");
      stopcondition = true;
    }
    if (stopcondition) return;
    
    // do the reduction
    
    ext_imgch_orig = ext_imgch;
    if (flag == 0) check_filepresence(cc_commandTextEdit, science, ext_imgch, flag, flag_ini);
    if (processsky == 1)
      if (flag == 0) check_filepresence(cc_commandTextEdit, sky, ext_imgch, flag, flag_ini);
    
    ext_imgch.append(".sub");
    if (!smoothedge.isEmpty()) {
      create_scriptcall_2(cc_commandTextEdit, 1, "create_smoothedge_para.sh", 
			  mainpath, science, ext_imgch_orig);
    }
    create_scriptcall_2(cc_commandTextEdit, 0, "prepare_coadd_swarp.sh", 
			mainpath, science, ext_imgch);
    create_scriptcall_2(cc_commandTextEdit, 1, "resample_coadd_swarp_para.sh", 
			mainpath, science, ext_imgch);
    if (!filterthreshold.isEmpty()) {
      create_scriptcall_1(cc_commandTextEdit, 0, "resample_filtercosmics.sh", 
			  mainpath, science);
    }
    create_scriptcall_1(cc_commandTextEdit, 0, "perform_coadd_swarp.sh", mainpath, 
			science);
    create_scriptcall_2(cc_commandTextEdit, 0, "update_coadd_header.sh", mainpath, 
			science, ext_imgch_orig);
    
    if (processsky == 1) {
      if (!smoothedge.isEmpty()) {
	create_scriptcall_2(cc_commandTextEdit, 1, "create_smoothedge_para.sh", 
			    mainpath, science, ext_imgch_orig);
      }
      create_scriptcall_2(cc_commandTextEdit, 0, "prepare_coadd_swarp.sh", 
			  mainpath, sky, ext_imgch);
      create_scriptcall_2(cc_commandTextEdit, 1, "resample_coadd_swarp_para.sh", 
			  mainpath, sky, ext_imgch);
      if (!filterthreshold.isEmpty()) {
	create_scriptcall_1(cc_commandTextEdit, 0, "resample_filtercosmics.sh", 
			    mainpath, sky);
      }
      create_scriptcall_1(cc_commandTextEdit, 0, "perform_coadd_swarp.sh", mainpath, 
			  sky);
      create_scriptcall_2(cc_commandTextEdit, 0, "update_coadd_header.sh", mainpath, 
			  sky, ext_imgch_orig);
    }
  }
  
  
  // resolve link structure
  if (CB_array[2] == 1) {
    checkresolvedir = checkdir2(resolvedir);
    if (resolvedir.isEmpty() || checkresolvedir == 0) {
      cc_resolvedirLineEdit->setPaletteBackgroundColor(tfred);
      warn(cc_commandTextEdit,"STOP: Resolve links: Directory not found.");
    }
    if (checkresolvedir == 1 && !resolvedir.isEmpty()) 
      cc_resolvedirLineEdit->setPaletteBackgroundColor(tfgreen);
    if (checkresolvedir == 1) {
      create_scriptcall_0(cc_commandTextEdit, 0, "resolvelinks.sh", resolvedir);
    }
  }
  else cc_resolvedirLineEdit->setPaletteBackgroundColor(tfcyan);
}


void theliForm::getfilterlist(QString &result)
{
  QString dfits, fitsort, execstr, filterstring, chipnr, sub;
  QString mainpath, science, sciencedir;
  QStringList imagelist;
  int l, test, chip, nchip;
  QDir dir;

  // path
  mainpath   = in_maindirLineEdit->text();
  science    = in_sciencedirLineEdit->text();
  sciencedir = mainpath+"/"+science+"/";

  // check if images actually exist:
  dir.setPath(sciencedir);
  
  get_fitsext_forastromcoadd(ext_img, ext_imgch);

  get_numchips();

  nchip = numchips.toInt();

  // search for images, starting with chip no. 1;
  // we do not want to collect filter strings from all chips in an exposure
  // as this takes very long for mosaic cameras
  test = 0;
  chip = 1;
  while (test == 0 && chip <= nchip) {
    chipnr = chipnr.setNum(chip);
    imagelist = dir.entryList("*_"+chipnr+ext_imgch+".fits");
    if (imagelist.count() == 0) chip++;
    else {
      test = 1;
      sub = "";
    }
  }

  // if no images are found, loop over sky subtracted images
  if (test == 0) {
    chip = 1;
    while (test == 0 && chip <= nchip) {
      chipnr = chipnr.setNum(chip);
      imagelist = dir.entryList("*_"+chipnr+ext_imgch+".sub.fits");
      if (imagelist.count() == 0) chip++;
      else {
	test = 1;
	sub = ".sub";
      }
    }
  }

  // leave if no filters are found
  if (test == 0) return;

  dfits = bindir+"/dfits ";
  fitsort = bindir+"/fitsort ";
  
  execstr = dfits + " " + sciencedir + "/*_" + chipnr + ext_imgch + sub +".fits | ";
  execstr += fitsort + " FILTER | gawk '(NR>1) {print $2}' | sort | uniq | ";
  execstr += "gawk '{printf(\"%s_xyz_\",$1)} END{printf(\"\\n\")}'";

  my_fscanf(execstr, filterstring);

  // remove the trailing "_xyz_"
  l = filterstring.length() - 5;
  filterstring.truncate(l);
  filterstring.replace("/","");
  filterstring.replace("[","");
  filterstring.replace("]","");
  filterstring.replace("$","");
  filterstring.replace("(","");
  filterstring.replace(")","");
  filterstring.replace("^","");
  filterstring.replace(";","");
  filterstring.replace(" ","");
  filterstring.replace("*","");
  filterstring.replace("#","");

  QStringList filterlist = filterlist.split("_xyz_", filterstring);

  //  if (filterlist.count() != 1) printf("nfilters: %d\n", (int) filterlist.count());

  if (filterlist.count() != 1) return;

  filterstring.replace("_xyz_","");
  
  // update the database
  QSettings settings (QSettings::Ini);
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  settings.beginGroup("/COADDITION");
  settings.beginGroup("/coaddition");
  settings.writeEntry("/coaddfiltertext", filterstring);
  settings.endGroup();
  settings.endGroup();
  settings.endGroup();

  result = filterstring;
}


void theliForm::exec_cc()
{
  QString fullpath, ident, scripts, presentinst, filterstring, tmp;
  QSettings settings (QSettings::Ini);
  
  int result;
  is_imextension_empty(result);
  if (result == 1) return;
  
  // update the params.ini
  modify_paramsini();
  modify_paramset2();
  
  fullpath = userscriptdir;
  fullpath.append("/");
  fullpath.append("theli_cc_comm.sh");
  
  scripts = cc_commandTextEdit->text();  
  if (scripts.contains("STOP:") != 0) return;
  presentinst = tf_instrumentLineEdit->text();
  
  if (cc_coaddCheckBox->isChecked()) {
    QString reductionlog = in_reductionlogLineEdit->text();
    if (reductionlog.isEmpty()) {
      reductionlog = "noname";
    }
    settings.beginGroup("/theli_"+reductionlog+"_");
    settings.beginGroup("/COADDITION");
    settings.beginGroup("/coaddition");
    filterstring = settings.readEntry("/scriptid","");

    if (filterstring.isEmpty() || filterstring.compare("(null)") == 0) {
      getfilterlist(filterstring);
    }
    else {
      QString filterstring2 = settings.readEntry("/coaddfiltertext","");
      if (filterstring2.isEmpty()) {
	getfilterlist(filterstring);
      }
    }
    tmp = settings.readEntry("/scriptid","");
    if ( (tmp.isEmpty() || tmp.compare("(null)") == 0) && !filterstring.isEmpty()) {
      settings.writeEntry("/scriptid",filterstring);
    }

    settings.endGroup();
    settings.endGroup();
    settings.endGroup();

    modify_paramset2();

    //    filterstring = settings.readEntry("/scriptid","");
    if (filterstring.isEmpty()) {
      QMessageBox::information( this, "",
	    "THELI does not know which FILTER to stack.\n"
	    "Please select a FILTER from the pulldown menu in the configuration dialog.\n");
      call_parameters_cc1();	
      return;
    }
  }
  
  // write the superscript and execute it
  process_group = "cc";
  if (checkdir2(userscriptdir) == 1 && !presentinst.isEmpty() && 
      !scripts.isEmpty()) {
    write_superscriptbody(fullpath, scripts);
  }
}


void theliForm::kill_cc()
{
  QString execstr;
  
  if (started_proc_cc == 1) {
    if (proc_cc->isRunning()) {
      process_id = proc_cc->processIdentifier();
      pid = pid.setNum(process_id);
      execstr = scriptdir;
      execstr.append("/parakill.sh ");
      execstr.append(pid);
      execstr.append(" > /dev/null 2>&1");
      my_system(execstr);
      cc_messagesTextEdit->append("All processes killed.");	
    }
    started_proc_cc = 0;
  }
}

#endif
