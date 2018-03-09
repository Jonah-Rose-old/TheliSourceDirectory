#include <qstylefactory.h>
#include <qfiledialog.h>
#include <qbutton.h>
#include <qsettings.h>
#include "functions.h"

void theliForm::write_logfile()
{
  int numinstruments;
  QString string, editor, viewer, pdfviewer, instrumentname;
  QString maindir, ra, dec, ap_dt, ap_dmin;
  QSettings settings (QSettings::Ini);
  
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  // settings for the main form
  settings.beginGroup("/theliform_settings");
    guifont = qApp->font();
    settings.writeEntry("/TF_STYLE", tf_styleSpinBox->cleanText());
    settings.writeEntry("/TF_FONTFAMILY", guifont.family());
    settings.writeEntry("/TF_FONTSIZE", guifont.pointSize());
    settings.writeEntry("/TF_FONTWEIGHT", guifont.weight());
    settings.writeEntry("/TF_FONTITALICS", guifont.italic());
    settings.writeEntry("/TF_FITSHEADERFONTSIZE", fitsheaderfontsize);
    settings.writeEntry("/TF_LASTPROCESSINGGROUP", mainmenu_TabWidget->currentPageIndex());
  settings.endGroup();
  
  // settings for the initialise process group
  settings.beginGroup("/INITIALISE");
    if (nano->isOn())   editor = "nano";
    if (emacs->isOn())  editor = "emacs";
    if (xemacs->isOn()) editor = "xemacs";
    if (nedit->isOn())  editor = "nedit";
    if (vi->isOn())     editor = "vi";
    if (skycat->isOn()) viewer = "skycat";
    if (ds9->isOn())    viewer = "ds9";
    if (pdfacroread->isOn()) pdfviewer = "acroread";
    if (pdfgvv->isOn())      pdfviewer = "gvv";
    if (pdfxpdf->isOn())     pdfviewer = "xpf";
    numinstruments = in_instrumentListBox->numRows();
    if (numinstruments != 0) {
      instrumentname = in_instrumentListBox->currentText();
    } else instrumentname="";
    
    maindir = in_maindirLineEdit->text();
    if (!maindir.endsWith("/")) maindir.append("/");
    
    settings.writeEntry("/maindir", maindir); 
    settings.writeEntry("/biasdir", in_biasdirLineEdit->text()); 
    settings.writeEntry("/darkdir", in_darkdirLineEdit->text()); 
    settings.writeEntry("/flatdir", in_flatdirLineEdit->text()); 
    settings.writeEntry("/flatoffdir", in_flatoffdirLineEdit->text()); 
    settings.writeEntry("/sciencedir", in_sciencedirLineEdit->text()); 
    settings.writeEntry("/skydir", in_skydirLineEdit->text()); 
    settings.writeEntry("/standarddir", in_standarddirLineEdit->text()); 
    settings.writeEntry("/numcpus", in_numcpuSpinBox->cleanText());
    settings.writeEntry("/datadiskwarn", in_datadiskwarnSpinBox->cleanText());
    settings.writeEntry("/homediskwarn", in_homediskwarnSpinBox->cleanText());
    settings.writeEntry("/editor", editor);
    settings.writeEntry("/viewer", viewer);
    settings.writeEntry("/pdfviewer", pdfviewer);
    settings.writeEntry("/instrumentname", instrumentname);
    settings.writeEntry("/processsky", in_processskyCheckBox->isChecked());
    settings.writeEntry("/inst_prof", in_profinstRadioButton->isChecked());
    settings.writeEntry("/inst_comm", in_comminstRadioButton->isChecked());
    settings.writeEntry("/inst_user", in_userinstRadioButton->isChecked());
    settings.writeEntry("/inst_prof_index", inst_prof_index);
    settings.writeEntry("/inst_comm_index", inst_comm_index);
    settings.writeEntry("/inst_user_index", inst_user_index);
  settings.endGroup();
  
  // settings for the preparation process group
  settings.beginGroup("/PREPARATION");
    settings.writeEntry("/chipsscratch", pre_linkchipsLineEdit->text());
    settings.writeEntry("/scratchdir", pre_scratchdirLineEdit->text());
    settings.writeEntry("/splitMIRcube", pre_splitMIRCheckBox->isChecked());
    settings.writeEntry("/sortscript_checkbox", pre_sortdataCheckBox->isChecked());
    settings.writeEntry("/splitscript_checkbox", pre_splitCheckBox->isChecked());
    settings.writeEntry("/createlinksscript_checkbox", pre_createlinksCheckBox->isChecked());
    //    CB_variable = 1 means DONE, = 0 means NOTDONE
    settings.writeEntry("/sortscript", CB_sortrawdata);
    settings.writeEntry("/splitscript", CB_processsplit);
    settings.writeEntry("/createlinksscript", CB_createlinks);
  settings.endGroup();
  
  // settings for the calibration process group
  settings.beginGroup("/CALIBRATION");
    settings.writeEntry("/biasmin", cal_biasminLineEdit->text());
    settings.writeEntry("/biasmax", cal_biasmaxLineEdit->text());
    settings.writeEntry("/darkmin", cal_darkminLineEdit->text());
    settings.writeEntry("/darkmax", cal_darkmaxLineEdit->text());
    settings.writeEntry("/flatmin", cal_flatminLineEdit->text());
    settings.writeEntry("/flatmax", cal_flatmaxLineEdit->text());
    settings.writeEntry("/sciencemin", cal_scienceminLineEdit->text());
    settings.writeEntry("/sciencemax", cal_sciencemaxLineEdit->text());
    settings.writeEntry("/dobias", cal_nobiasCheckBox->isChecked());
    settings.writeEntry("/doflat", cal_noflatCheckBox->isChecked());
    settings.writeEntry("/nonlinearity", cal_nonlinCheckBox->isChecked());
    settings.writeEntry("/usedark", cal_darkinsteadbiasCheckBox->isChecked());
    settings.writeEntry("/processbiasscript_checkbox", cal_procbiasCheckBox->isChecked());
    settings.writeEntry("/processdarkscript_checkbox", cal_procdarkCheckBox->isChecked());
    settings.writeEntry("/processflatscript_checkbox", cal_procflatCheckBox->isChecked());
    settings.writeEntry("/processsciencescript_checkbox", cal_procscienceCheckBox->isChecked());
    settings.writeEntry("/processbiasscript", CB_processbias);
    settings.writeEntry("/processdarkscript", CB_processdark);
    settings.writeEntry("/processflatscript", CB_processflat);
    settings.writeEntry("/processsciencescript", CB_processscience);
  settings.endGroup();

  // settings for the background process group
  settings.beginGroup("/BACKGROUND");
    settings.writeEntry("/spreadsequencescript", CB_spreadsequence);
    settings.writeEntry("/spreadsequencescript_checkbox", sf_spreadsequenceCheckBox->isChecked());
    settings.writeEntry("/numbergroups", sf_spread_numbergroupsLineEdit->text());
    settings.writeEntry("/lengthsequence", sf_spread_lengthsequenceLineEdit->text());
    settings.writeEntry("/backgroundscript", CB_background);
    settings.writeEntry("/background_checkbox", sf_backgroundCheckBox->isChecked());
    settings.writeEntry("/mergesequencescript", CB_mergesequence);
    settings.writeEntry("/mergesequencescript_checkbox", sf_mergesequenceCheckBox->isChecked());
    settings.writeEntry("/chopnodpattern", sf_chopnodpatternComboBox->currentItem());
    settings.writeEntry("/chopnodinvert", sf_chopnodinvertCheckBox->isChecked());
    settings.writeEntry("/chopnodscript", CB_chopnod);
    settings.writeEntry("/chopnodscript_checkbox", sf_chopnodCheckBox->isChecked());
    settings.writeEntry("/collapsescript", CB_collapse);
    settings.writeEntry("/collapsescript_checkbox", sf_collapseCheckBox->isChecked());
  settings.endGroup();
  
  // settings for the weighting process group
  settings.beginGroup("/WEIGHTING");
    settings.writeEntry("/minoverlap", w_minoverlapLineEdit->text());
    settings.writeEntry("/debloomthresh", w_bloomthreshLineEdit->text());
    settings.writeEntry("/debloomscript_checkbox", w_debloomCheckBox->isChecked());
    settings.writeEntry("/globalweightscript_checkbox", w_createglobwCheckBox->isChecked());
    settings.writeEntry("/binnedmosaicsscript_checkbox", w_createbinmosCheckBox->isChecked());
    settings.writeEntry("/createweightsscript_checkbox", w_createweightsCheckBox->isChecked());
    settings.writeEntry("/distributescript_checkbox", w_distsetCheckBox->isChecked());
    settings.writeEntry("/debloomscript", CB_debloom);
    settings.writeEntry("/globalweightscript", CB_globalweights);
    settings.writeEntry("/binnedmosaicsscript", CB_binnedmosaics);
    settings.writeEntry("/createweightsscript", CB_createweights);
    settings.writeEntry("/distributescript", CB_distributesets);
  settings.endGroup();

  // settings for the astromphotom process group
  settings.beginGroup("/ASTROMPHOTOM");
    ra = ap_refraLineEdit->text();
    dec = ap_refdecLineEdit->text();
    if (ra.isEmpty()) ra = "from header";
    if (dec.isEmpty()) dec = "from header";
    ap_dt = ap_refimagedetectthreshLineEdit->text();
    ap_dmin = ap_refimagedetectminareaLineEdit->text();
    if (ap_dt.isEmpty()) ap_dt = "10";
    if (ap_dmin.isEmpty()) ap_dmin = "10";
    
    settings.writeEntry("/ra", ra);
    settings.writeEntry("/dec", dec);
    settings.writeEntry("/radius", ap_radiusLineEdit->text());
    settings.writeEntry("/maglim", ap_maglimLineEdit->text());
    settings.writeEntry("/astrommethod", ap_astrommethodComboBox->currentItem());
    settings.writeEntry("/refcatsource", ap_arcserverComboBox->currentItem()); 
    settings.writeEntry("/onlinecatalog", ap_arccatComboBox->currentItem()); 
    settings.writeEntry("/refimagepath", ap_refimageLineEdit->text()); 
    settings.writeEntry("/refimageDT", ap_dt); 
    settings.writeEntry("/refimageDMIN", ap_dmin); 
    settings.writeEntry("/absphotomdirectscript_checkbox", ap_absphotom_direct_CheckBox->isChecked());
    settings.writeEntry("/absphotomindirectscript_checkbox", ap_absphotom_indirect_CheckBox->isChecked());
    settings.writeEntry("/sourcecatscript_checkbox", ap_createsourcecatCheckBox->isChecked());
    settings.writeEntry("/astrometryscript_checkbox", ap_astrometryCheckBox->isChecked());
    settings.writeEntry("/absphotomdirectscript", CB_absphotom_direct);
    settings.writeEntry("/absphotomindirectscript", CB_absphotom_indirect);
    settings.writeEntry("/sourcecatscript", CB_singleastrom);
    settings.writeEntry("/astrometryscript", CB_astrometry);
  settings.endGroup();
  
  // settings for the coaddition process group
  settings.beginGroup("/COADDITION");
    settings.writeEntry("/resolvelinkdir", cc_resolvedirLineEdit->text());
    settings.writeEntry("/skysubscript_checkbox", cc_createskysubCheckBox->isChecked());
    settings.writeEntry("/createcoaddscript_checkbox", cc_coaddCheckBox->isChecked());
    settings.writeEntry("/resolvelinkscript_checkbox", cc_resolvelinksCheckBox->isChecked());
    settings.writeEntry("/skysubscript", CB_skysub);
    settings.writeEntry("/createcoaddscript", CB_createcoadd);
    settings.writeEntry("/resolvelinkscript", CB_resolvelinks);
  settings.endGroup();
  
  // settings for the processing status
  settings.beginGroup("/PROCESSINGSTATUS");
    QString status, s1, s2, s3, s4, s5, s6;
    s1 = s1.setNum(ext_img[0]);
    s2 = s2.setNum(ext_img[1]);
    s3 = s3.setNum(ext_img[2]);
    s4 = s4.setNum(ext_img[3]);
    s5 = s5.setNum(ext_img[4]);
    s6 = s6.setNum(ext_img[5]);
    status = status + s1 + s2 + s3 + s4 + s5 + s6;
    settings.writeEntry("/status", status);
  settings.endGroup();

  settings.endGroup();
  
  // the logfile has changed, either its content or its name.
  // thus update the lastlog
  update_lastlog();
}


void theliForm::read_logfile()
{
  int numstyles;
  QString tmp, tmpread, progsini;
  QString editor, viewer, pdfviewer, instrumentname;
  QSettings settings(QSettings::Ini);
  
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  // settings for the main form
  settings.beginGroup("/theliform_settings");
  
  // handle the GUI style
  GUIstyle = settings.readNumEntry("/TF_STYLE", 2);
  styles = QStyleFactory::keys();
  if (GUIstyle > 0) {
    // set the current style, update the style push button
    qApp->setStyle( styles[ GUIstyle - 1] );
    numstyles = styles.count();
    qnumstyles = qnumstyles.setNum(numstyles);
    tmp=("/");
    tmp.append(qnumstyles);
    tf_styleSpinBox->setMaxValue(numstyles);
    tf_styleSpinBox->setSuffix(tmp);
    tf_styleSpinBox->setValue(GUIstyle);
  }
  else {
    numstyles = styles.count();
    qnumstyles = qnumstyles.setNum(numstyles);
    tmp=("/");
    tmp.append(qnumstyles);
    tf_styleSpinBox->setMaxValue(numstyles);
    tf_styleSpinBox->setSuffix(tmp);
    tf_styleSpinBox->setValue(1);
  }

  // handle the GUI font
  guifont.setFamily(settings.readEntry("/TF_FONTFAMILY"));
  guifont.setPointSize(settings.readNumEntry("/TF_FONTSIZE", 10));
  guifont.setWeight(settings.readNumEntry("/TF_FONTWEIGHT"));
  guifont.setItalic(settings.readBoolEntry("/TF_FONTITALICS", false));
  qApp->setFont( guifont, true );
      
  // handle the FITS header font size
  fitsheaderfontsize = settings.readNumEntry("/TF_FITSHEADERFONTSIZE", 10);
  
  mainmenu_TabWidget->setCurrentPage(settings.readNumEntry("/TF_LASTPROCESSINGGROUP", 0));
  settings.endGroup();
  
  // the initialise process group
  settings.beginGroup("/INITIALISE");
  in_maindirLineEdit->setText(settings.readEntry("/maindir", ""));
  in_biasdirLineEdit->setText(settings.readEntry("/biasdir", ""));
  in_darkdirLineEdit->setText(settings.readEntry("/darkdir", ""));
  in_flatdirLineEdit->setText(settings.readEntry("/flatdir", ""));
  in_flatoffdirLineEdit->setText(settings.readEntry("/flatoffdir", ""));
  in_sciencedirLineEdit->setText(settings.readEntry("/sciencedir", ""));
  in_skydirLineEdit->setText(settings.readEntry("/skydir", ""));
  in_standarddirLineEdit->setText(settings.readEntry("/standarddir", ""));
  in_numcpuSpinBox->setValue(settings.readNumEntry("/numcpus", 1));
  in_datadiskwarnSpinBox->setValue(settings.readNumEntry("/datadiskwarn", 500));
  in_homediskwarnSpinBox->setValue(settings.readNumEntry("/homediskwarn", 100));
  in_processskyCheckBox->setChecked(settings.readBoolEntry("/processsky", false));
  in_profinstRadioButton->setChecked(settings.readBoolEntry("/inst_prof", true));
  in_comminstRadioButton->setChecked(settings.readBoolEntry("/inst_comm", false));
  in_userinstRadioButton->setChecked(settings.readBoolEntry("/inst_user", false));
  inst_prof_index = settings.readNumEntry("/inst_prof_index", 0);
  inst_comm_index = settings.readNumEntry("/inst_comm_index", 0);
  inst_user_index = settings.readNumEntry("/inst_user_index", 0);
  
  refresh_instlist();

  editor = settings.readEntry("/editor", "nano");
  viewer = settings.readEntry("/viewer", "skycat");
  pdfviewer = settings.readEntry("/pdfviewer", "acroread");

  if ( editor.compare("nano") == 0) nano->setOn(true);
  if ( editor.compare("emacs") == 0) emacs->setOn(true);
  if ( editor.compare("xemacs") == 0) xemacs->setOn(true);
  if ( editor.compare("nedit") == 0) nedit->setOn(true);
  if ( editor.compare("vi") == 0) vi->setOn(true);
  if ( viewer.compare("skycat") == 0) skycat->setOn(true);
  if ( viewer.compare("ds9") == 0) ds9->setOn(true);
  if ( pdfviewer.compare("acroread") == 0) pdfacroread->setOn(true);
  if ( pdfviewer.compare("gvv") == 0) pdfgvv->setOn(true);
  if ( pdfviewer.compare("xpdf") == 0) pdfxpdf->setOn(true);
  
  instrumentname = settings.readEntry("/instrumentname", "");
  highlight_instrumentname(instrumentname);
  settings.endGroup();
  
  // the preparation process group
  settings.beginGroup("/PREPARATION");
  pre_linkchipsLineEdit->setText(settings.readEntry("/chipsscratch", ""));
  pre_scratchdirLineEdit->setText(settings.readEntry("/scratchdir", ""));
  pre_splitMIRCheckBox->setChecked(settings.readBoolEntry("/splitMIRcube"));
  
  pre_sortdataCheckBox->setChecked(settings.readBoolEntry("/sortscript_checkbox"));
  pre_splitCheckBox->setChecked(settings.readBoolEntry("/splitscript_checkbox"));
  pre_createlinksCheckBox->setChecked(settings.readBoolEntry("/createlinksscript_checkbox"));
  
  CB_sortrawdata = settings.readNumEntry("/sortscript");
  CB_processsplit = settings.readNumEntry("/splitscript");
  CB_createlinks = settings.readNumEntry("/createlinksscript");
  settings.endGroup();
  
  // the calibration process group
  settings.beginGroup("/CALIBRATION");
  cal_biasminLineEdit->setText(settings.readEntry("/biasmin", ""));
  cal_biasmaxLineEdit->setText(settings.readEntry("/biasmax", ""));
  cal_darkminLineEdit->setText(settings.readEntry("/darkmin", ""));
  cal_darkmaxLineEdit->setText(settings.readEntry("/darkmax", ""));
  cal_flatminLineEdit->setText(settings.readEntry("/flatmin", ""));
  cal_flatmaxLineEdit->setText(settings.readEntry("/flatmax", ""));
  cal_scienceminLineEdit->setText(settings.readEntry("/sciencemin", ""));
  cal_sciencemaxLineEdit->setText(settings.readEntry("/sciencemax", ""));
  cal_nobiasCheckBox->setChecked(settings.readBoolEntry("/dobias"));
  cal_noflatCheckBox->setChecked(settings.readBoolEntry("/doflat"));
  cal_nonlinCheckBox->setChecked(settings.readBoolEntry("/nonlinearity"));
  cal_darkinsteadbiasCheckBox->setChecked(settings.readBoolEntry("/usedark"));
  
  cal_procbiasCheckBox->setChecked(settings.readBoolEntry("/processbiasscript_checkbox"));
  cal_procdarkCheckBox->setChecked(settings.readBoolEntry("/processdarkscript_checkbox"));
  cal_procflatCheckBox->setChecked(settings.readBoolEntry("/processflatscript_checkbox"));
  cal_procscienceCheckBox->setChecked(settings.readBoolEntry("/processsciencescript_checkbox"));
  
  CB_processbias = settings.readNumEntry("/processbiasscript");
  CB_processdark = settings.readNumEntry("/processdarkscript");
  CB_processflat = settings.readNumEntry("/processflatscript");
  CB_processscience = settings.readNumEntry("/processsciencescript");
  settings.endGroup();
  
  // the background process group
  settings.beginGroup("/BACKGROUND");
  sf_spread_numbergroupsLineEdit->setText(settings.readEntry("/numbergroups", "3"));
  sf_spread_lengthsequenceLineEdit->setText(settings.readEntry("/lengthsequence", "12"));
  sf_chopnodinvertCheckBox->setChecked(settings.readBoolEntry("/chopnodinvert"));
  sf_chopnodpatternComboBox->setCurrentItem(settings.readNumEntry("/chopnodpattern", 0));
  
  sf_spreadsequenceCheckBox->setChecked(settings.readBoolEntry("/spreadsequencescript_checkbox"));
  sf_backgroundCheckBox->setChecked(settings.readBoolEntry("/backgroundscript_checkbox"));
  sf_mergesequenceCheckBox->setChecked(settings.readBoolEntry("/mergesequencescript_checkbox"));
  sf_chopnodCheckBox->setChecked(settings.readBoolEntry("/chopnodscript_checkbox"));
  sf_collapseCheckBox->setChecked(settings.readBoolEntry("/collapsescript_checkbox"));
  
  CB_spreadsequence = settings.readNumEntry("/spreadsequencescript");
  CB_background = settings.readNumEntry("/backgroundscript");
  CB_mergesequence = settings.readNumEntry("/mergesequencescript");
  CB_chopnod = settings.readNumEntry("/chopnodscript");
  CB_collapse = settings.readNumEntry("/collapsescript");
  settings.endGroup();
  
  // the weighting process group
  settings.beginGroup("/WEIGHTING");
  w_minoverlapLineEdit->setText(settings.readEntry("/minoverlap", "1000"));
  w_bloomthreshLineEdit->setText(settings.readEntry("/debloomthresh", "55000"));
  w_debloomCheckBox->setChecked(settings.readBoolEntry("/debloomscript_checkbox"));
  w_createglobwCheckBox->setChecked(settings.readBoolEntry("/globalweightscript_checkbox"));
  w_createbinmosCheckBox->setChecked(settings.readBoolEntry("/binnedmosaicsscript_checkbox"));
  w_createweightsCheckBox->setChecked(settings.readBoolEntry("/createweightsscript_checkbox"));
  w_distsetCheckBox->setChecked(settings.readBoolEntry("/distributescript_checkbox"));
  
  CB_debloom = settings.readNumEntry("/debloomscript");
  CB_globalweights = settings.readNumEntry("/globalweightscript");
  CB_binnedmosaics = settings.readNumEntry("/binnedmosaicsscript");
  CB_createweights = settings.readNumEntry("/createweightsscript");
  CB_distributesets = settings.readNumEntry("/distributescript");
  settings.endGroup();
  
  // the astromphotom process group
  settings.beginGroup("/ASTROMPHOTOM");
  ap_refraLineEdit->setText(settings.readEntry("/ra", "from header"));
  ap_refdecLineEdit->setText(settings.readEntry("/dec", "from header"));
  ap_radiusLineEdit->setText(settings.readEntry("/radius", ""));
  ap_maglimLineEdit->setText(settings.readEntry("/maglim", "20"));
  ap_arcserverComboBox->setCurrentItem(settings.readNumEntry("/refcatsource",0));
  ap_arccatComboBox->setCurrentItem(settings.readNumEntry("/onlinecatalog",0));
  ap_refimageLineEdit->setText(settings.readEntry("refimagepath",""));
  ap_refimagedetectthreshLineEdit->setText(settings.readEntry("refimageDT","10"));
  ap_refimagedetectminareaLineEdit->setText(settings.readEntry("refimageDMIN","10"));
  ap_astrommethodComboBox->setCurrentItem(settings.readNumEntry("/astrommethod", 0));
  ap_absphotom_direct_CheckBox->setChecked(settings.readBoolEntry("/absphotomdirectscript_checkbox"));
  ap_absphotom_indirect_CheckBox->setChecked(settings.readBoolEntry("/absphotomindirectscript_checkbox"));
  ap_createsourcecatCheckBox->setChecked(settings.readBoolEntry("/sourcecatscript_checkbox"));
  ap_astrometryCheckBox->setChecked(settings.readBoolEntry("/astrometryscript_checkbox"));
  
  CB_absphotom_direct   = settings.readNumEntry("/absphotomdirectscript");
  CB_absphotom_indirect = settings.readNumEntry("/absphotomindirectscript");
  CB_singleastrom = settings.readNumEntry("/sourcecatscript");
  CB_astrometry   = settings.readNumEntry("/astrometryscript");
  settings.endGroup();
  
  // the coaddition process group
  settings.beginGroup("/COADDITION");
  cc_resolvedirLineEdit->setText(settings.readEntry("/resolvelinkdir", ""));
  
  cc_createskysubCheckBox->setChecked(settings.readBoolEntry("/skysubscript_checkbox"));
  cc_coaddCheckBox->setChecked(settings.readBoolEntry("/createcoaddscript_checkbox"));
  cc_resolvelinksCheckBox->setChecked(settings.readBoolEntry("/resolvelinkscript_checkbox"));
  
  CB_skysub       = settings.readNumEntry("/skysubscript");
  CB_createcoadd  = settings.readNumEntry("/createcoaddscript");
  CB_resolvelinks = settings.readNumEntry("/resolvelinkscript");
  settings.endGroup();
  
  settings.endGroup();
  
  // paint checkboxes green / grey, depending on the script execution status
  paint_checkboxes();
  
  // update the instrument fields
  update_presinst();
  
  // update the reductionChangeprocessingstatus actions
  changestatus_obtain_imextension();
  
  // update the lastlog
  update_lastlog();

  // update the reference catalog server button
  enable_server();
}


// clears the processing status in a log file 
// (sets all scripts to "NOTDONE" etc.)
void theliForm::clear_processingstatus_in_logfile()
{
  QString line, value, ident, number, filename_in, tmp;
  QString history, execstr, filename_out;
  QStringList status;
  QSettings settings (QSettings::Ini);
  
  // make sure that all scrips will be set to "not done", from the 
  // index point of view
  
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
  
  // force the update; this creates the log file in case it doesn't yet exist
  update_logfile();
  
  // get the log file name
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  filename_in = homedir;
  filename_in.append("/.theli/reduction_logs/");
  tmp = in_reductionlogLineEdit->text();
  filename_in.append(tmp);
  history = filename_in;
  history.append(".com");
  
  // update the logfile    
  settings.beginGroup("/PREPARATION");
  settings.writeEntry("/sortscript", 0);
  settings.writeEntry("/splitscript", 0);
  settings.writeEntry("/createlinksscript", 0);
  settings.endGroup();
  settings.beginGroup("/CALIBRATION");
  settings.writeEntry("/processbiasscript", 0);
  settings.writeEntry("/processdarkscript", 0);
  settings.writeEntry("/processflatscript", 0);
  settings.writeEntry("/processsciencescript", 0);
  settings.endGroup();
  settings.beginGroup("/BACKGROUND");
  settings.writeEntry("/spreadsequencescript", 0);
  settings.writeEntry("/backgroundscript", 0);
  settings.writeEntry("/chopnodscript", 0);
  settings.writeEntry("/mergesequencescript", 0);
  settings.writeEntry("/collapsescript", 0);
  settings.endGroup();
  settings.beginGroup("/WEIGHTING");
  settings.writeEntry("/debloomscript", 0);
  settings.writeEntry("/globalweightscript", 0);
  settings.writeEntry("/binnedmosaicsscript", 0);
  settings.writeEntry("/createweightsscript", 0);
  settings.writeEntry("/distributescript", 0);
  settings.endGroup();
  settings.beginGroup("/ASTROMPHOTOM");
  settings.writeEntry("/absphotomdirectscript", 0);
  settings.writeEntry("/absphotomindirectscript", 0);
  settings.writeEntry("/sourcecatscript", 0);
  settings.writeEntry("/astrometryscript", 0);
  settings.endGroup();
  settings.beginGroup("/COADDITION");
  settings.writeEntry("/skysubscript", 0);
  settings.writeEntry("/createcoaddscript", 0);
  settings.writeEntry("/resolvelinkscript", 0);
  settings.endGroup();
  
  settings.endGroup();
  
  // empty the command history
  // delete the history
  QFile file2 (history);
  file2.remove();
  
  // touch (= create an empty) history
  execstr = "touch ";
  execstr.append(history);
  my_system(execstr);
  
  // reset the fields
  resetfields_pre_pushbutton();
  resetfields_cal_pushbutton();
  resetfields_sf_pushbutton();
  resetfields_w_pushbutton();
  resetfields_ap_pushbutton();
  resetfields_cc_pushbutton();
  
  // clear all the scripts
  clearscripts_pre_pushbutton();
  clearscripts_cal_pushbutton();
  clearscripts_sf_pushbutton();
  clearscripts_w_pushbutton();
  clearscripts_ap_pushbutton();
  clearscripts_cc_pushbutton();
  
  // apply the changes to the GUI
  apply_logfile();
  
  // the menubar status has to be cleared separately (check why)
  statusCPSDebiasingAction->setOn(false);
  statusCPSBackgroundAction->setOn(false);
  statusCPSChopnodAction->setOn(false);
  statusCPSCollapseAction->setOn(false);
  statusCPSDebloomingAction->setOn(false);
  statusCPSDirectphotAction->setOn(false);
  
  // clear the checkboxes
  paint_checkboxes();
  
  // wipe the maindir line edit (has a "/" as default)
  // in_maindirLineEdit->clear();

  //  make sure theli also resets the parameters in the parameter dialogue
  settings.beginGroup("/theligeneral_");
    settings.writeEntry("/paramreset", true);
  settings.endGroup();
}


void theliForm::obtain_imextension()
{
  int i;
  QString value, tmp;
  QStringList status;
  QSettings settings (QSettings::Ini);
  
  // get the log file name
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  value = settings.readEntry("/PROCESSINGSTATUS/status");
  status = status.split( " ", value );
  for (i = 0; i < 6; i++) {
    tmp=value.at(i);
    ext_img[i] = tmp.toInt();
  }
  
  settings.endGroup();
}


void theliForm::set_imextension(int val, int position)
{
  int i;
  QString sciencestring, number;
  QSettings settings (QSettings::Ini);
  
  obtain_imextension();
  
  // science
  for (i=0; i<6; i++) {
    number = number.setNum(ext_img[i]);
    if (i == position) number = number.setNum(val);
    sciencestring.append(number);
  }
  
  // get the log file name
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  settings.writeEntry("/PROCESSINGSTATUS/status", sciencestring);
  
  settings.endGroup();
}


void theliForm::update_imextension()
{
  int i;
  
  QString sciencestring;
  QString filename_out, filename_in, execstr, tmp, line, value;
  QString ident, number;
  QStringList status;
  QSettings settings (QSettings::Ini);
  
  sciencestring = "";
  
  if ( CB_processscience == 0) ext_img[0] = 0; else ext_img[0] = 1;
  if ( CB_background == 0)     ext_img[1] = 0; else ext_img[1] = 1;
  if ( CB_chopnod == 0)        ext_img[2] = 0; else ext_img[2] = 1;
  if ( CB_collapse == 0)       ext_img[3] = 0; else ext_img[3] = 1;
  if ( CB_debloom == 0)        ext_img[4] = 0; else ext_img[4] = 1;
  if ( CB_directphot == 0)     ext_img[5] = 0; else ext_img[5] = 1; 
  
  {
    // a little extra sausage; the status P is not read properly if set
    // lets obtain it from the STATUS string instead
    QString value, tmp;
    QStringList status;
    QSettings settings (QSettings::Ini);
    
    // get the log file name
    QString reductionlog = in_reductionlogLineEdit->text();
    if (reductionlog.isEmpty()) {
      reductionlog = "noname";
      in_reductionlogLineEdit->setText("noname");
    }
    settings.beginGroup("/theli_"+reductionlog+"_");
    
    value = settings.readEntry("/PROCESSINGSTATUS/status");
    status = status.split( " ", value );
    tmp = value.at(5);
    ext_img[5] = tmp.toInt();
    settings.endGroup();
  }

  // science
  for (i=0; i<6; i++) {
    number = number.setNum(ext_img[i]);
    sciencestring.append(number);
  }
  
  // get the log file name
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  settings.writeEntry("/PROCESSINGSTATUS/status", sciencestring);
  
  settings.endGroup();
}


void theliForm::update_instrument_inlogfile()
{
  int numinstruments;
  QString filename_out, filename_in, tmp, execstr;
  QString line, value, ident, number;
  QStringList status;
  QSettings settings (QSettings::Ini);
  
  // get the log file name
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  numinstruments = in_instrumentListBox->numRows();
  if (numinstruments != 0) {
    settings.writeEntry("/INITIALISE/instrumentname", in_instrumentListBox->currentText());
  } else settings.writeEntry("/INITIALISE/instrumentname", "");
  
  settings.endGroup();
}


void theliForm::load_reductionlog()
{
  FILE *of;
  QString logdir, qtdir, execstr, fullpath, name;
  
  // link all logs into the .theli/reduction_logs/ directory
  
  logdir = homedir;
  logdir.append("/.theli/reduction_logs/");
  qtdir = homedir;
  qtdir.append(".qt/");
  
  fullpath = userscriptdir+"/theli_rclist.sh";
  
  of = fopen(fullpath, "w");
  
  fprintf(of, "#!%s\n\n", bash.ascii());
  fprintf(of, "cd %s\n", qtdir.ascii());
  fprintf(of, "ls theli_*_rc > rc_list_$$\n");
  fprintf(of, "cat rc_list_$$ |\\\n");
  fprintf(of, "{\n");
  fprintf(of, "  while read file\n");
  fprintf(of, "  do\n");
  fprintf(of, "    BASE=`basename ${file} _rc | gawk 'BEGIN {FS=\"theli_\"} {print $2}'`\n");
  fprintf(of, "    if [ -f %s${BASE}.log ]; then\n", logdir.ascii());
  fprintf(of, "        \\rm %s${BASE}.log\n", logdir.ascii());
  fprintf(of, "    fi\n");
  fprintf(of, "    ln -s %s${file} %s${BASE}.log\n", qtdir.ascii(), logdir.ascii());
  fprintf(of, "  done\n");
  fprintf(of, "}\n");
  fprintf(of, "\\rm rc_list_$$\n");
  
  fclose(of);
  
  execstr = "chmod +x ";
  execstr.append(fullpath);
  my_system(execstr);
  
  my_system(fullpath);
  
  execstr = "\\rm ";
  execstr.append(fullpath);
  my_system(execstr);
  
  QString currentFileName = QFileDialog::getOpenFileName(
            logdir,
	    "Reduction log files (*.log)",
	    this,
	    "open file dialog",
	    "Choose a log file" );
  
  if ( currentFileName.isEmpty() )
    return;
  
  currentFileName = currentFileName.section('/', -1);
  
  currentFileName.replace(".log","");
  in_reductionlogLineEdit->setText(currentFileName);
  
  read_logfile();

  theliForm::setCaption("THELI GUI "+guiversion+" --- "+currentFileName);
}


void theliForm::apply_logfile()
{
  QString tmp;
  
  tmp = in_reductionlogLineEdit->text();
  if (!tmp.isEmpty()) read_logfile();
  else {
    in_reductionlogLineEdit->setText("noname");
    // update_lastlog() called from read_logfile()
    update_lastlog();
  }
}


void theliForm::update_logfile()
{
  int numinstruments;
  
  update_imextension();
  write_logfile();
  
  numinstruments = in_instrumentListBox->numRows();
  if (numinstruments > 0) 
    lastinstrument = in_instrumentListBox->currentText();
}


void theliForm::read_lastlog()
{
  QSettings settings (QSettings::Ini);
  
  settings.beginGroup("/theligeneral_");
  QString lastlog = settings.readEntry("/lastlog", "noname");
  settings.endGroup();
  
  in_reductionlogLineEdit->setText(lastlog);
  apply_logfile();
}
