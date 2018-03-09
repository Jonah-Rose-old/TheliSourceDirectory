#include "scamp_mosaictype.h"
#include <qapplication.h>
#include <qiconset.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qpixmap.h>
#include <qbuttongroup.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qpixmap.h>
#include <qthread.h>
#include <qlabel.h>
#include <qsettings.h>
#include <qmessagebox.h>
#include <qprocess.h>
#include "functions.h"
#include "theliform.h"

int page, nfilt, nchips_int;
int proc_param_zeroheader_started, proc_param_restoreheader_started;
QString lastlog, sciencedir, standarddir, imext, nfiltstring, stdcatold;
QProcess *proc_param_zeroheader, *proc_param_restoreheader;
QString param_maindir, param_sciencedir, param_imext, param_stddir;


void parameters::show_absphot_readme()
{
  QMessageBox::information( this, "About standard star astrometry",
		     "This task makes use of both parameter settings for object\n"
		     "detection (Create source cat) and astrometry (using Scamp).\n"
		     "Make sure you have chosen a working configuration.\n\n"
		     "If you DO NOT want the astrometry, e.g. because the source\n"
		     "density in your standard star observations is very low and\n"
		     "will likely cause problems during astrometric solution, the\n"
		     "latter may be switched off. This will only work if the angular\n"
		     "offset between nominal WCS and true WCS in the FITS headers\n"
		     "is small, such that the reference source can be identified\n"
		     "unambiguously! You must provide an estimate of the astrometric\n"
		     "uncertainty.\n");
}


void parameters::show_hide_absphot_wcserror()
{
  if (absphot_wcserrorCheckBox->isChecked()) absphot_wcserrorLineEdit->setEnabled(false);
  else absphot_wcserrorLineEdit->setEnabled(true);
}


void parameters::show_hide_twopass()
{
  if (sf_back_twopassCheckBox->isChecked()) {
    sf_back_twopass2TextLabel->setEnabled(true);
    sf_back_nlow2LineEdit->setEnabled(true);
    sf_back_nhigh2LineEdit->setEnabled(true);
  } else {
    sf_back_twopass2TextLabel->setEnabled(false);
    sf_back_nlow2LineEdit->setEnabled(false);
    sf_back_nhigh2LineEdit->setEnabled(false);
  }
}


void parameters::update_refcoord_textlabel()
{
  QString ctype = coc_celestialtypeComboBox->currentText();

  if (ctype.compare("EQUATORIAL") == 0) coc_refraTextLabel->setText("Ref. RA | DEC:");
  if (ctype.compare("GALACTIC") == 0) coc_refraTextLabel->setText("Ref. GLON | GLAT:");
  if (ctype.compare("ECLIPTIC") == 0) coc_refraTextLabel->setText("Ref. ELON | ELAT:");
  if (ctype.compare("SUPERGALACTIC") == 0) coc_refraTextLabel->setText("Ref. SLON | SLAT:");
}


void parameters::call_mtype_readme()
{
  scamp_mosaictype* mtype = new scamp_mosaictype();
  mtype->show();
}

void parameters::update_headertextlabel(int page)
{
  if (page == 0) param_head2TextLabel->setText("Preparation");
  if (page == 1) param_head2TextLabel->setText("Calibration");
  if (page == 2) param_head2TextLabel->setText("Background model (1)");
  if (page == 3) param_head2TextLabel->setText("Background model (2)");
  if (page == 4) param_head2TextLabel->setText("Weighting (1)");
  if (page == 5) param_head2TextLabel->setText("Weighting (2)");
  if (page == 6) param_head2TextLabel->setText("Astrom/Photom (1)");
  if (page == 7) param_head2TextLabel->setText("Astrom/Photom (2)");
  if (page == 8) param_head2TextLabel->setText("Astrom/Photom (3)");
  if (page == 9) param_head2TextLabel->setText("Astrom/Photom (4)");
  if (page == 10) param_head2TextLabel->setText("Coaddition");
}


void parameters::switch_page_next()
{
  if (page == 10) page = 9;
  page++;
  param_WidgetStack->raiseWidget(page);
  if (page == 10) {
    param_nextPushButton->setDisabled(true);
    fill_coadd_filtercombobox();
    get_pixscale();
  }
  if (page > 0) param_backPushButton->setEnabled(true);
  
  update_headertextlabel(page);    
}


void parameters::show_hide_filtercombobox()
{
  if (nfilt == 1) {
    coc_filterComboBox->hide();
    coc_filterTextLabel->hide();
  }
  if (nfilt > 1) {
    coc_filterComboBox->show();
    coc_filterTextLabel->show();
  }
}


void parameters::fill_coadd_filtercombobox()
{
  QString dfits, fitsort, execstr, filterstring, chipnr, sub;
  QStringList imagelist;
  int l, n;
  QDir dir;

  // check if images actually exist:
  dir.setPath(sciencedir);
  
  // search for images, starting with chip no. 1;
  // we do not want to collect filter strings from all chips in an exposure
  // as this takes very long for mosaic cameras
  int test = 0;
  int chip = 1;
  while (test == 0 && chip < 200) {
    chipnr = chipnr.setNum(chip);
    imagelist = dir.entryList("*_"+chipnr+imext+".fits");
    if (imagelist.count() == 0) chip++;
    else {
      test = 1;
      sub = "";
    }
  }

  // if no images are found, loop over sky subtracted images
  if (test == 0) {
    chip = 1;
    while (test == 0 && chip < 200) {
      chipnr = chipnr.setNum(chip);
      imagelist = dir.entryList("*_"+chipnr+imext+".sub.fits");
      if (imagelist.count() == 0) chip++;
      else {
	test = 1;
	sub = ".sub";
      }
    }
  }

  // leave if no filters are found
  if (test == 0) return;

  n = coc_filterComboBox->count();

  dfits = bindir+"/dfits ";
  fitsort = bindir+"/fitsort ";
  
  execstr = dfits + " " + sciencedir + "/*_" + chipnr + imext + sub +".fits | ";
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

  coc_filterComboBox->clear();
  coc_filterComboBox->insertStringList(filterlist);

  nfilt = coc_filterComboBox->count();
  nfiltstring = nfiltstring.setNum(nfilt);

  if (nfilt > 1) {
    coc_filterComboBox->insertItem("All", -1);
    if (n == 0) coc_filterComboBox->setCurrentItem(0);
  }
  else {
    if (n == 0) coc_filterComboBox->setCurrentItem(0);
  }

  // update the database
  QSettings settings (QSettings::Ini);
  QString reductionlog = lastlog;
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  settings.beginGroup("/COADDITION");
  settings.beginGroup("/coaddition");
  //  settings.writeEntry("/coaddfilteritem", coc_filterComboBox->currentItem());
  settings.endGroup();
  settings.endGroup();
  settings.endGroup();
}


void parameters::fill_skyposangle()
{
  QString execstr, skypa, header, headerpath, cd11, cd12, cd21, cd22;
  QString line, lineini, value, ident, image;
  QStringList list;
  QDir dir;
  
  dir.setPath(sciencedir);
  list = dir.entryList("*_*"+imext+".fits");
  if (list.count() == 0) {
      QMessageBox::warning( this, "Data not found!",
	 "No "+imext+"-images were found in the science directory.\n");
      return;
  }

  QStringList::Iterator it = list.begin();
  image = *it;
  int length = image.length();
  int length2 = imext.length();
  length = length - length2 - 5;  // truncate image extension and ".fits"
  image.truncate(length);

  // locate the corresponding header file
  headerpath = sciencedir + "/headers/" + image + ".head";
  
  QFile file(headerpath);

  if (!file.exists()) {
    QMessageBox::warning( this, "Header not found!",
	"Could not find astrometric header for the\n"
        "first image. Did you run the astrometry?\n"
	"Missing header:\n"
	""+image+".head\n");
    return;
  }

  // reading the first header file in the list

  cd11 = "";
  cd12 = "";
  cd21 = "";
  cd22 = "";

  if ( file.open( IO_ReadOnly )) {
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      lineini = stream.readLine();
      line = lineini.simplifyWhiteSpace();
      if (!line.isEmpty()) {
	ident = line.section(' ',0,0);
	//	printf("%s\n", ident.ascii());
	if (ident.compare("CD1_1") == 0)
	  cd11=line.section(' ',2,2);
	if (ident.compare("CD1_2") == 0)
	  cd12=line.section(' ',2,2);
	if (ident.compare("CD2_1") == 0)
	  cd21=line.section(' ',2,2);
	if (ident.compare("CD2_2") == 0)
	  cd22=line.section(' ',2,2);
      }
    }
    file.close();
  }

  if (!cd11.isEmpty() && !cd12.isEmpty() && 
      !cd21.isEmpty() && !cd22.isEmpty()) {

    execstr = bindir + "/get_posangle -c " + cd11 + " " + 
      cd12 + " " + cd21 + " " + cd22;
    execstr += + " | gawk '{printf(\"%.2f\\n\",$1)}'";

    my_fscanf(execstr, skypa);

    if (skypa.compare("999") == 0) {
      QMessageBox::warning( this, "Problem estimating position angle",
	"The CD matrix is non-orthogonal enough such that the\n"
        "rotation angle could not be determined unambiguously.\n"
	"Please estimate the correct angle manually.\n"
	"From header: "+header);
      return;
    }

    coc_skyposangleLineEdit->setText(skypa);
  }
  else
    QMessageBox::warning( this, "Header information incomplete!",
       "The CD matrix appears to be incomplete in\n"
       ""+header);
  return;
}


void parameters::switch_page_back()
{
  if (page == 0) page = 1;
  page--;
  param_WidgetStack->raiseWidget(page);
  if (page == 0) param_backPushButton->setDisabled(true);
  if (page < 10) param_nextPushButton->setEnabled(true);
  
  update_headertextlabel(page);    
}


void parameters::check_for_reset()
{
  QSettings settings(QSettings::Ini);

  // reset all parameter values if the 'reset logfile' button has been pressed
  settings.beginGroup("/theligeneral_");
  if (settings.readBoolEntry("/paramreset", false)) {
    defaults_all();
  }
  settings.endGroup();
}


void parameters::init()
{
  QSettings settings(QSettings::Ini);
  QString param_instrument, param_inst_path;
  QString maindir, value, tmp;
  int i, arr[6];
  QStringList status;
  
  // get general info
  settings.beginGroup("/theligeneral_");
  lastlog   = settings.readEntry("/lastlog", "noname");
  scriptdir = settings.readEntry("/mainpath", "");
  homedir   = settings.readEntry("/homedir", "");
  bindir    = settings.readEntry("/bindir", "");
  settings.endGroup();

  settings.beginGroup("/theli_"+lastlog+"_");
  value = settings.readEntry("/PROCESSINGSTATUS/status");
  settings.beginGroup("/theliform_settings");
  page = settings.readNumEntry("/TF_PARAMETERSPAGE");
  settings.endGroup();
  settings.beginGroup("/INITIALISE");
  param_maindir    = settings.readEntry("/maindir", "");
  param_sciencedir = settings.readEntry("/sciencedir", "");
  param_stddir     = settings.readEntry("/standarddir", "");
  param_instrument = settings.readEntry ("/instrumentname");
  if (settings.readBoolEntry("/inst_prof",true)) param_inst_path = scriptdir + "/instruments_professional/";
  if (settings.readBoolEntry("/inst_comm",true)) param_inst_path = scriptdir + "/instruments_commercial/";
  if (settings.readBoolEntry("/inst_user",true)) param_inst_path = homedir + "/.theli/instruments_user/";
  maindir    = settings.readEntry("/maindir");
  sciencedir = maindir+"/"+settings.readEntry("/sciencedir");
  standarddir = maindir+"/"+settings.readEntry("/standarddir");
  settings.endGroup();
  settings.endGroup();
  
  // obtain the image extension
  for (i = 0; i < 6; i++) {
    tmp = value.at(i);
    arr[i] = tmp.toInt();
  }
  imext = "";
  if (arr[0] == 1) imext.append("OFC");
  if (arr[1] == 1) imext.append("B");
  if (arr[2] == 1) imext.append("H");
  if (arr[3] == 1) imext.append("C");
  if (arr[4] == 1) imext.append("D");
  if (arr[5] == 1) imext.append("P");
  
  param_WidgetStack->raiseWidget(page);

  if (page == 0) param_backPushButton->setDisabled(true);
  if (page == 10) {
    fill_coadd_filtercombobox();
    param_nextPushButton->setDisabled(true);
  }

  // initialise the dialog
  populate_xtalkbuttons();
  untoggle_xtalkbuttons();
  adjust_collcorr_direction();

  // fill in the STD filter and colour comboboxes such that they aren't empty
  init_stdcolor_comboboxes();
  //update_stdcolor_comboboxes();
  //    update_stdcolorfilter();
  
  stdcatold = "SDSS";
  if (absphot_direct_stdcatComboBox->currentText() == "2MASS")
    stdcatold = "2MASS";

  read_parameters();

  link_checkbox_xtalk();

  update_headertextlabel(page);    
  showhide_bloom();

  toggle_globalweight_params();

  showhide_defectdetection();

  update_refcoord_textlabel();

  // check if the user clicked the 'reset logfile' button;
  // if so, then set all parameters to default values;
  // i don't understand why the corresponding 
  // reading of QSettings does not work here. It appears that within
  // a function I can address one QSettings file only once, not twice.
  // That's why I capsuled it in the separate function below:
  check_for_reset();

  // set the collapse exclusion pixmap
  /*
  QString path = scriptdir + "../images/";
  collapseregion_groupbox->setEnabled(true);
  colldir_PixmapLabel->setPixmap(QPixmap(path+"collapse.png"));
  */

  // The standard star astrometry
  show_hide_absphot_wcserror();

  // Update the absolute photometry module
  populate_absphotmenu();

  // Display (or not) some coadd parameters
  showhide_clipped();

  // Display (or not) some background modeling parameters
  show_hide_twopass();
}


void parameters::populate_absphotmenu()
{
  QDir nightcaldir;
  QStringList photsolutions;
  QString night;
  QPushButton *pushbutton;
  QComboBox *combobox;

  // First, hide all nights
  NIGHT_1_ComboBox->hide();
  NIGHT_2_ComboBox->hide();
  NIGHT_3_ComboBox->hide();
  NIGHT_4_ComboBox->hide();
  NIGHT_5_ComboBox->hide();
  NIGHT_6_ComboBox->hide();
  NIGHT_7_ComboBox->hide();
  NIGHT_8_ComboBox->hide();
  NIGHT_9_ComboBox->hide();
  NIGHT_10_ComboBox->hide();
  NIGHT_1_PushButton->hide();
  NIGHT_2_PushButton->hide();
  NIGHT_3_PushButton->hide();
  NIGHT_4_PushButton->hide();
  NIGHT_5_PushButton->hide();
  NIGHT_6_PushButton->hide();
  NIGHT_7_PushButton->hide();
  NIGHT_8_PushButton->hide();
  NIGHT_9_PushButton->hide();
  NIGHT_10_PushButton->hide();

  // The directory with the photometric calibrations
  nightcaldir.setPath(standarddir+"/calib/");

  // Search for photometric solutions
  photsolutions = nightcaldir.entryList("night_*_result.asc");

  // Return if none are found
  if (photsolutions.count() == 0) {
    absphot_textlabel->setText("No photometrically calibrated nights were found under <STANDARD>/calib/");
    return;
  }
  
  combobox   = NIGHT_1_ComboBox;
  pushbutton = NIGHT_1_PushButton;

  // Otherwise, populate the groupbox with the solutions
  int nightid = 1;
  QString nightidstring;
  for ( QStringList::Iterator it = photsolutions.begin(); it != photsolutions.end(); ++it ) {
    if (nightid == 1) {
      combobox = NIGHT_1_ComboBox;
      pushbutton = NIGHT_1_PushButton;
    }
    if (nightid == 2) {
      combobox = NIGHT_2_ComboBox;
      pushbutton = NIGHT_2_PushButton;
    }
    if (nightid == 3) {
      combobox = NIGHT_3_ComboBox;
      pushbutton = NIGHT_3_PushButton;
    }
    if (nightid == 4) {
      combobox = NIGHT_4_ComboBox;
      pushbutton = NIGHT_4_PushButton;
    }
    if (nightid == 5) {
      combobox = NIGHT_5_ComboBox;
      pushbutton = NIGHT_5_PushButton;
    }
    if (nightid == 6) {
      combobox = NIGHT_6_ComboBox;
      pushbutton = NIGHT_6_PushButton;
    }
    if (nightid == 7) {
      combobox = NIGHT_7_ComboBox;
      pushbutton = NIGHT_7_PushButton;
    }
    if (nightid == 8) {
      combobox = NIGHT_8_ComboBox;
      pushbutton = NIGHT_8_PushButton;
    }
    if (nightid == 9) {
      combobox = NIGHT_9_ComboBox;
      pushbutton = NIGHT_9_PushButton;
    }
    if (nightid == 10) {
      combobox = NIGHT_10_ComboBox;
      pushbutton = NIGHT_10_PushButton;
    }
    if (nightid >= 11) return;

    // Show the night
    combobox->show();
    pushbutton->show();

    // Set the night label
    pushbutton->setText("Show "+(*it).remove("_result.asc"));

    nightid++;
  }
}


void parameters::show_photcal1()
{
  QString image = ((NIGHT_1_PushButton->text()).remove("Show "))+"_result.png";
  QString execstr = "display "+standarddir+"/calib/"+image+" &";
  my_system(execstr);
}


void parameters::show_photcal2()
{
  QString image = ((NIGHT_2_PushButton->text()).remove("Show "))+"_result.png";
  QString execstr = "display "+standarddir+"/calib/"+image+" &";
  my_system(execstr);
}

void parameters::show_photcal3()
{
  QString image = ((NIGHT_3_PushButton->text()).remove("Show "))+"_result.png";
  QString execstr = "display "+standarddir+"/calib/"+image+" &";
  my_system(execstr);
}

void parameters::show_photcal4()
{
  QString image = ((NIGHT_4_PushButton->text()).remove("Show "))+"_result.png";
  QString execstr = "display "+standarddir+"/calib/"+image+" &";
  my_system(execstr);
}

void parameters::show_photcal5()
{
  QString image = ((NIGHT_5_PushButton->text()).remove("Show "))+"_result.png";
  QString execstr = "display "+standarddir+"/calib/"+image+" &";
  my_system(execstr);
}

void parameters::show_photcal6()
{
  QString image = ((NIGHT_6_PushButton->text()).remove("Show "))+"_result.png";
  QString execstr = "display "+standarddir+"/calib/"+image+" &";
  my_system(execstr);
}

void parameters::show_photcal7()
{
  QString image = ((NIGHT_7_PushButton->text()).remove("Show "))+"_result.png";
  QString execstr = "display "+standarddir+"/calib/"+image+" &";
  my_system(execstr);
}


void parameters::get_crossidradius(QString &radius)
{    
  QString pscale, crossidradius;
  
  QString fullinstpath = get_instpath();
  get_fileparameter(fullinstpath, "PIXSCALE", pscale);
  float pscale_float = pscale.toFloat();
  if (pscale_float < 0.7 && pscale_float >= 0.2) crossidradius = "2.0";
  else if (pscale_float >= 0.7) crossidradius = crossidradius.setNum(pscale_float*2.5);
  else crossidradius = crossidradius.setNum(pscale_float*10.0);
  crossidradius.truncate(3);
  radius = crossidradius;
}


void parameters::get_pixscale()
{    
  QString pscale;
  QString fullinstpath = get_instpath();

  get_fileparameter(fullinstpath, "PIXSCALE", pscale);
  QString pixscale = coc_pixscaleLineEdit->text();
  if (pixscale.isEmpty()) coc_pixscaleLineEdit->setText(pscale);
}


void parameters::get_numchips()
{    
  QString nchips;

  QString fullinstpath = get_instpath();

  get_fileparameter(fullinstpath, "NCHIPS", nchips);
  nchips_int = nchips.toInt();
}


void parameters::update_crossidradius()
{    
  QString radius;
  
  get_crossidradius(radius);
  ac_sc_crossidradiusLineEdit->setText(radius);
}


void parameters::check_distortion_consistency(int &answer)
{
  QString distort_groups = ac_sc_distortgroupsLineEdit->text();
  QString distort_degrees2 = ac_sc_distortorder2LineEdit->text();
  QString distort_keys = ac_sc_distortkeysLineEdit->text();

  // all fields empty
  if (distort_groups.isEmpty() && 
      distort_degrees2.isEmpty() &&
      distort_keys.isEmpty()) {
    answer = 0;
    return;
  }
  
  // all fields contain input
  if (!distort_groups.isEmpty() && 
      !distort_degrees2.isEmpty() &&
      !distort_keys.isEmpty()) {
    int ng = distort_groups.contains(',');
    int nd = distort_degrees2.contains(',');
    int nk = distort_keys.contains(',');
    if ( ng != nd || ng != nk || nd != nk) {
      QMessageBox::warning( this, "Inconsistent input",
	"Scamp config: The number of elements for additional DISTORT_DEGREES,\n"
        "DISTORT_GROUPS and DISTORT_KEYS must be identical!\n");
      answer = 1;
      return;
    }
    if ( ng == nd || ng == nk || nd == nk) {
      answer = 0;
      return;
    }
  }

  // if we are still here, then at least one field is empty while
  // the others are not.
  QMessageBox::warning( this, "Inconsistent input",
     "The number of elements for additional DISTORT_DEGREES\n"
     "and DISTORT_GROUPS must be identical!\n");
  answer = 1;
  return;
}



void parameters::parameters_quit()
{
  int answer;
  check_distortion_consistency(answer);
  if (answer == 1) return;
  
  // make sure the pixel scale is not left empty
  get_pixscale();

  // other consistency checks
  int check;
  check_nhigh_window(check);
  if (!check) return;

  // make sure the smoothing scale is set for defringing options
  QString smooth = sf_back_illumsmoothLineEdit->text();
  if ((sf_back_applymodeComboBox->currentItem() == 2 || 
       sf_back_applymodeComboBox->currentItem() == 3) &&
      smooth.isEmpty()) {
    QMessageBox::warning( this, "Missing parameter: Smoothing scale for background model",
			  "To extract a fringing model, we need a smoothing\n"
			  "scale for the background model.\n");
    sf_back_illumsmoothLineEdit->setFocus();
    return;
  }

  write_parameters();
  
  QSettings settings(QSettings::Ini);
  
  // deactivate the parameter resetting
  settings.beginGroup("/theligeneral_");
  settings.writeEntry("/paramreset", false);
  settings.endGroup();

  parameters::close();
}


void parameters::update_stdcolor_comboboxes()
{
  QStringList stdcatfiltersM2;

  QString stdcatnew = absphot_direct_stdcatComboBox->currentText();

  if (stdcatnew.compare(stdcatold) != 0) {
    if (absphot_direct_stdcatComboBox->currentText() == "SDSS") {
      stdcatfiltersM2 += "u";
      stdcatfiltersM2 += "g";
      stdcatfiltersM2 += "r";
      stdcatfiltersM2 += "i";
      stdcatfiltersM2 += "z";
      absphot_stdfilterM2_ComboBox->clear();
      absphot_stdfilterM2_ComboBox->insertStringList(stdcatfiltersM2);
      stdcatold = stdcatnew;
    }
    if (absphot_direct_stdcatComboBox->currentText() == "2MASS") {
      stdcatfiltersM2 += "J";
      stdcatfiltersM2 += "H";
      stdcatfiltersM2 += "Ks";
      absphot_stdfilterM2_ComboBox->clear();
      absphot_stdfilterM2_ComboBox->insertStringList(stdcatfiltersM2);
      stdcatold = stdcatnew;
    }
  }
}


void parameters::check_stripe82()
{
  QString cat1, cat2;
  cat1 = scriptdir + "../photstdcats/STRIPE82.cat";
  cat2 = scriptdir + "../photstdcats/STRIPE82_primed.cat";

  QFile file1(cat1);
  QFile file2(cat2);
  if (!file1.exists() || !file2.exists()) {
    QMessageBox::warning( this, "Catalog not found!",
			  "Could not find the STRIPE82 ugriz photometric catalog.\n"
			  "It can be downloaded from this URL:\n"
			  "http://www.astro.uni-bonn.de/~theli/gui/download/STRIPE82.tgz\n"
			  "It must be unpacked in the gui/photstdcats/ directory!\n");
    absphot_indirect_stdcatComboBox->setCurrentItem(0);
    return;
  }
}


void parameters::init_stdcolor_comboboxes()
{
  QStringList stdcatfiltersM1, stdcatfiltersM2, stdcatcolorsM1;

  if (absphot_indirect_stdcatComboBox->currentItem() == 0) {
    stdcatfiltersM1 =  "U";
    stdcatfiltersM1 += "B";
    stdcatfiltersM1 += "V";
    stdcatfiltersM1 += "R";
    stdcatfiltersM1 += "I";
    stdcatcolorsM1 =  "U-B";
    stdcatcolorsM1 += "B-V";
    stdcatcolorsM1 += "V-R";
    stdcatcolorsM1 += "R-I";
    stdcatcolorsM1 += "V-I";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 1) {
    stdcatfiltersM1  = "U";
    stdcatfiltersM1  = "B";
    stdcatfiltersM1 += "V";
    stdcatfiltersM1 += "R";
    stdcatfiltersM1 += "I";
    stdcatcolorsM1 =  "U-B";
    stdcatcolorsM1  = "B-V";
    stdcatcolorsM1 += "V-R";
    stdcatcolorsM1 += "R-I";
    stdcatcolorsM1 += "V-I";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 2) {
    check_stripe82();
    stdcatfiltersM1 =  "u";
    stdcatfiltersM1 += "g";
    stdcatfiltersM1 += "r";
    stdcatfiltersM1 += "i";
    stdcatfiltersM1 += "z";
    stdcatcolorsM1 =  "u-g";
    stdcatcolorsM1 += "g-r";
    stdcatcolorsM1 += "r-i";
    stdcatcolorsM1 += "i-z";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 3) {
    check_stripe82();
    stdcatfiltersM1 =  "u";
    stdcatfiltersM1 += "g";
    stdcatfiltersM1 += "r";
    stdcatfiltersM1 += "i";
    stdcatfiltersM1 += "z";
    stdcatcolorsM1 =  "u-g";
    stdcatcolorsM1 += "g-r";
    stdcatcolorsM1 += "r-i";
    stdcatcolorsM1 += "i-z";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 4) {
    check_stripe82();
    stdcatfiltersM1 =  "u";
    stdcatfiltersM1 += "g";
    stdcatfiltersM1 += "r";
    stdcatfiltersM1 += "i";
    stdcatfiltersM1 += "z";
    stdcatcolorsM1 =  "u-g";
    stdcatcolorsM1 += "g-r";
    stdcatcolorsM1 += "r-i";
    stdcatcolorsM1 += "i-z";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 5) {
    stdcatfiltersM1  = "J";
    stdcatfiltersM1 += "H";
    stdcatfiltersM1 += "K";
    stdcatcolorsM1  = "J-H";
    stdcatcolorsM1 += "H-K";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 6) {
    stdcatfiltersM1  = "J";
    stdcatfiltersM1 += "H";
    stdcatfiltersM1 += "K";
    stdcatcolorsM1  = "J-H";
    stdcatcolorsM1 += "H-K";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 7) {
    stdcatfiltersM1  = "J";
    stdcatfiltersM1 += "H";
    stdcatfiltersM1 += "K";
    stdcatcolorsM1  = "J-H";
    stdcatcolorsM1 += "H-K";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 8) {
    stdcatfiltersM1  = "J";
    stdcatfiltersM1 += "H";
    stdcatfiltersM1 += "K";
    stdcatfiltersM1 += "Ks";
    stdcatcolorsM1  = "J-H";
    stdcatcolorsM1 += "H-K";
    stdcatcolorsM1 += "H-Ks";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 9) {
    stdcatfiltersM1  = "Y";
    stdcatfiltersM1 += "J";
    stdcatfiltersM1 += "H";
    stdcatfiltersM1 += "K";
    stdcatfiltersM1 += "L";
    stdcatfiltersM1 += "M";
    stdcatcolorsM1  = "Y-J";
    stdcatcolorsM1 += "J-H";
    stdcatcolorsM1 += "H-K";
    stdcatcolorsM1 += "K-L";
    stdcatcolorsM1 += "L-M";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 10) {
    stdcatfiltersM1  = "L";
    stdcatfiltersM1 += "M";
    stdcatcolorsM1  = "L-M";
  }
  if (absphot_indirect_stdcatComboBox->currentItem() == 11) {
    stdcatfiltersM1  = "C";
    stdcatfiltersM1 += "M";
    stdcatfiltersM1 += "T1";
    stdcatfiltersM1 += "T2";
    stdcatcolorsM1  = "C-M";
    stdcatcolorsM1 += "M-T1";
    stdcatcolorsM1 += "T1-T2";
  }
 
  if (absphot_direct_stdcatComboBox->currentItem() == 0) {
    stdcatfiltersM2 += "u";
    stdcatfiltersM2 += "g";
    stdcatfiltersM2 += "r";
    stdcatfiltersM2 += "i";
    stdcatfiltersM2 += "z";
  }
  if (absphot_direct_stdcatComboBox->currentItem() == 1) {
    stdcatfiltersM2 += "J";
    stdcatfiltersM2 += "H";
    stdcatfiltersM2 += "Ks";
  }

  absphot_stdfilterComboBox->clear();
  absphot_stdfilterM2_ComboBox->clear();
  absphot_stdcolorComboBox->clear();
  absphot_stdfilterComboBox->insertStringList(stdcatfiltersM1);
  absphot_stdfilterM2_ComboBox->insertStringList(stdcatfiltersM2);
  absphot_stdcolorComboBox->insertStringList(stdcatcolorsM1);
}


void parameters::update_stdcolorfilter()
{
  // select the filter and colour that was chosen last
  QSettings settings (QSettings::Ini);
  QString reductionlog = lastlog;
  if (reductionlog.isEmpty()) reductionlog = "noname";
  settings.beginGroup("/theli_"+reductionlog+"_");
    settings.beginGroup("/ASTROMPHOTOM");
      settings.beginGroup("/absphotom");
      absphot_stdfilterComboBox->setCurrentItem(settings.readNumEntry("/stdfilter",0));
      absphot_stdcolorComboBox->setCurrentItem(settings.readNumEntry("/stdcolor",0));
      absphot_stdfilterM2_ComboBox->setCurrentItem(settings.readNumEntry("/stdfilterM2",0));
      settings.endGroup();
    settings.endGroup();
  settings.endGroup();
}


void parameters::check_nhigh_window(int &checkresult)
{
  QString size = sf_back_windowsizeLineEdit->text();
  int window = (sf_back_windowsizeLineEdit->text()).toInt();
  int nhigh1 = (sf_back_nhigh1LineEdit->text()).toInt();
  int nlow1  = (sf_back_nlow1LineEdit->text()).toInt();
  int nhigh2 = (sf_back_nhigh2LineEdit->text()).toInt();
  int nlow2  = (sf_back_nlow2LineEdit->text()).toInt();

  if (!size.isEmpty() && window > 0 && (window - nhigh1 - nlow1 < 2 || window - nhigh2 - nlow2 < 2)) {
    param_WidgetStack->raiseWidget(2);
    QMessageBox::warning( this, "ERROR: Dynamic background model",
			  "Too many data points are rejected (nlow, nhigh) for the given window size.\n"
			  "Less than two data points remain. Increase the window size, or lower the\n"
			  "nlow/nhigh parameters\n");
    checkresult = 0;
  }
  else
    checkresult = 1;
}


void parameters::toggle_statdyn()
{
  QString size = sf_back_windowsizeLineEdit->text();
  if (!size.isEmpty()) {
    int window = (sf_back_windowsizeLineEdit->text()).toInt();
    if (window == 0) sf_back_static_dynamicTextlabel->setText("Model is: STATIC");
    else sf_back_static_dynamicTextlabel->setText("Model is: DYNAMIC");
  }
  else 
    sf_back_static_dynamicTextlabel->setText("Model is: STATIC");
}


void parameters::toggle_globalweight_params()
{
  if (globw_uniformweightCheckBox->isChecked()) {
    globw_flatminLineEdit->setEnabled(false);
    globw_flatmaxLineEdit->setEnabled(false);
    globw_darkminLineEdit->setEnabled(false);
    globw_darkmaxLineEdit->setEnabled(false);
    defect_kernelsizeLineEdit->setEnabled(false);
    defect_rowtolLineEdit->setEnabled(false);
    defect_coltolLineEdit->setEnabled(false);
    defect_clustolLineEdit->setEnabled(false);
    defect_kernelsize_sfLineEdit->setEnabled(false);
    defect_rowtol_sfLineEdit->setEnabled(false);
    defect_coltol_sfLineEdit->setEnabled(false);
    defect_clustol_sfLineEdit->setEnabled(false);
    globw_usebiasCheckBox->setEnabled(false);
  }
  else {
    globw_flatminLineEdit->setEnabled(true);
    globw_flatmaxLineEdit->setEnabled(true);
    globw_darkminLineEdit->setEnabled(true);
    globw_darkmaxLineEdit->setEnabled(true);
    defect_kernelsizeLineEdit->setEnabled(true);
    defect_rowtolLineEdit->setEnabled(true);
    defect_coltolLineEdit->setEnabled(true);
    defect_clustolLineEdit->setEnabled(true);
    defect_kernelsize_sfLineEdit->setEnabled(true);
    defect_rowtol_sfLineEdit->setEnabled(true);
    defect_coltol_sfLineEdit->setEnabled(true);
    defect_clustol_sfLineEdit->setEnabled(true);
    globw_usebiasCheckBox->setEnabled(true);
  }
}


// write the parameters into a Qt file for memory
void parameters::write_parameters()
{
  QString execstr;
  
  // catch all nonsense the user might enter and replace it by
  // default values
  
  // only those values where we need to check something are
  // put into v_xxxxx variables. The rest is written directly
  // into the Qt file
  
  // PREPARATION
  QString v_rename_fitskey = pre_rename_fitskeyLineEdit->text();
  QString v_sort_fitskey   = pre_sort_fitskeyLineEdit->text();
  QString v_sort_bias      = pre_sort_biasLineEdit->text();
  QString v_sort_dark      = pre_sort_darkLineEdit->text();
  QString v_sort_domeflat  = pre_sort_domeflatLineEdit->text();
  QString v_sort_skyflat   = pre_sort_skyflatLineEdit->text();
  QString v_sort_std       = pre_sort_stdLineEdit->text();
  
  // CALIBRATION
  QString v_cal_ovscannlow    = ovscan_nlowLineEdit->text();
  QString v_cal_ovscannhigh   = ovscan_nhighLineEdit->text();
  QString v_cal_biasnlow      = bias_nlowLineEdit->text();
  QString v_cal_biasnhigh     = bias_nhighLineEdit->text();
  QString v_cal_darknlow      = dark_nlowLineEdit->text();
  QString v_cal_darknhigh     = dark_nhighLineEdit->text();
  QString v_cal_flatnlow      = flat_nlowLineEdit->text();
  QString v_cal_flatnhigh     = flat_nhighLineEdit->text();
  
  if (v_cal_ovscannlow.isEmpty())    v_cal_ovscannlow    = "0";
  if (v_cal_ovscannhigh.isEmpty())   v_cal_ovscannhigh   = "1";
  if (v_cal_biasnlow.isEmpty())      v_cal_biasnlow      = "0";
  if (v_cal_biasnhigh.isEmpty())     v_cal_biasnhigh     = "1";
  if (v_cal_darknlow.isEmpty())      v_cal_darknlow      = "0";
  if (v_cal_darknhigh.isEmpty())     v_cal_darknhigh     = "1";
  if (v_cal_flatnlow.isEmpty())      v_cal_flatnlow      = "0";
  if (v_cal_flatnhigh.isEmpty())     v_cal_flatnhigh     = "1";
  
  // BACKGROUND
  QString v_back_nlow1         = sf_back_nlow1LineEdit->text();
  QString v_back_nhigh1        = sf_back_nhigh1LineEdit->text();
  QString v_back_nlow2         = sf_back_nlow2LineEdit->text();
  QString v_back_nhigh2        = sf_back_nhigh2LineEdit->text();
  QString v_back_maglimit      = sf_back_maglimitLineEdit->text();
  QString v_back_distance      = sf_back_distanceLineEdit->text();
  QString v_back_detectthresh  = sf_back_detectthreshLineEdit->text();
  QString v_back_detectminarea = sf_back_detectminareaLineEdit->text();
  QString v_back_maskexpand    = sf_back_maskexpandfactorLineEdit->text();
  QString v_back_windowsize    = sf_back_windowsizeLineEdit->text();
  QString v_back_gapsize       = sf_back_gapsizeLineEdit->text();
  QString v_back_illumsmooth   = sf_back_illumsmoothLineEdit->text();
  QString v_back_fringesmooth  = sf_back_fringesmoothLineEdit->text();
  QString v_coll_detectthresh  = ccc_colldetectthreshLineEdit->text();
  QString v_coll_detectminarea = ccc_colldetectminareaLineEdit->text();
  QString v_coll_maskexpand    = ccc_collmaskexpandfactorLineEdit->text();
  QString v_coll_rejectthresh  = ccc_collrejectthreshLineEdit->text();
  QString v_coll_xmin          = ccc_collxminLineEdit->text();
  QString v_coll_xmax          = ccc_collxmaxLineEdit->text();
  QString v_coll_ymin          = ccc_collyminLineEdit->text();
  QString v_coll_ymax          = ccc_collymaxLineEdit->text();
  
  if (v_back_nlow1.isEmpty())        v_back_nlow1      = "0";
  if (v_back_nhigh1.isEmpty())       v_back_nhigh1     = "0";
  if (v_back_nlow2.isEmpty())        v_back_nlow2      = "0";
  if (v_back_nhigh2.isEmpty())       v_back_nhigh2     = "1";
  if (v_back_windowsize.isEmpty())   v_back_windowsize = "0";
  if (v_back_gapsize.isEmpty())      v_back_gapsize    = "";
  if (v_coll_rejectthresh.isEmpty()) v_coll_rejectthresh  = "1.5";

  int sum = 0;
  if (!v_coll_xmin.isEmpty()) sum++;
  if (!v_coll_xmax.isEmpty()) sum++;
  if (!v_coll_ymin.isEmpty()) sum++;
  if (!v_coll_ymax.isEmpty()) sum++;
  if (sum>0 && sum<4) {
    QMessageBox::warning( this, "Incomplete information",
			  "You must specify all 4 boundaries in order to exclude\n"
			  "a region from the collapse correction. No region will\n"
			  "be excluded with the current settings!\n");
  }
  
  // WEIGHTING
  QString v_w_binmin          = w_binminLineEdit->text();
  QString v_w_binmax          = w_binmaxLineEdit->text();
  QString v_w_flatmin         = globw_flatminLineEdit->text();
  QString v_w_flatmax         = globw_flatmaxLineEdit->text();
  QString v_w_darkmin         = globw_darkminLineEdit->text();
  QString v_w_darkmax         = globw_darkmaxLineEdit->text();
  QString v_cosmicsthreshold  = weight_cosmicLineEdit->text();
  QString v_cosmicDT          = weight_cosmicDTLineEdit->text();
  QString v_cosmicDMIN        = weight_cosmicDMINLineEdit->text();
  QString v_bloomlowlimit     = weight_bloomlowlimitLineEdit->text();
  QString v_bloomminarea      = weight_bloomminareaLineEdit->text();
  QString v_bloomwidth        = weight_bloomwidthLineEdit->text();
  QString v_def_kernelsize    = defect_kernelsizeLineEdit->text();
  QString v_def_rowtol        = defect_rowtolLineEdit->text();
  QString v_def_coltol        = defect_coltolLineEdit->text();
  QString v_def_clustol       = defect_clustolLineEdit->text();
  QString v_def_kernelsize_sf = defect_kernelsize_sfLineEdit->text();
  QString v_def_rowtol_sf     = defect_rowtol_sfLineEdit->text();
  QString v_def_coltol_sf     = defect_coltol_sfLineEdit->text();
  QString v_def_clustol_sf    = defect_clustol_sfLineEdit->text();
  
  if (!v_cosmicsthreshold.isEmpty() && 
      (v_cosmicsthreshold.toFloat() == 0.0 ||
       v_cosmicsthreshold.compare("0") == 0))
    v_cosmicsthreshold = "";
  
  
  // mandatory
  if (v_w_flatmin.isEmpty()) v_w_flatmin = "0.5";
  if (v_w_flatmax.isEmpty()) v_w_flatmax = "1.6";
  
  if (v_w_binmin.isEmpty()) v_w_binmin = "-100";
  if (v_w_binmax.isEmpty()) v_w_binmax = "500";
  
  // optional; reset to empty if information is incomplete
  if (v_w_darkmin.isEmpty() || v_w_darkmax.isEmpty()) {
    v_w_darkmin = "";
    v_w_darkmax = "";
  }
  
  if (v_bloomlowlimit.isEmpty()) v_bloomlowlimit = "20000";
  if (v_bloomminarea.isEmpty()) v_bloomminarea = "100";
  if (v_bloomwidth.isEmpty()) v_bloomwidth = "0";
  
  
  // ASTROMPHOTOM
  // maximum fgroup radius
  if (ac_sc_fgroupradiusLineEdit->text().toInt() > 180.) 
    ac_sc_fgroupradiusLineEdit->setText("180");
  
  QString v_ap_detectthresh     = ap_detectthreshLineEdit->text();
  QString v_ap_detectminarea    = ap_detectminareaLineEdit->text();
  QString v_ap_deblendmincont   = ap_deblendLineEdit->text();
  QString v_ap_lownum           = ap_lownumLineEdit->text();
  QString v_ap_sexcatminfwhm    = ap_minfwhmLineEdit->text();
  QString v_ap_sexcatflag       = ap_flagLineEdit->text();
  QString v_ap_saturation       = ap_saturationLineEdit->text();
  QString v_ap_sexbacklevel     = ap_sexbacklevelLineEdit->text();
  QString v_scamp_maxposangle   = ac_sc_maxposangleLineEdit->text();
  float maxpos = v_scamp_maxposangle.toFloat();
  if (maxpos > 180.) v_scamp_maxposangle = "180";
  QString v_scamp_maxscale      = ac_sc_maxscaleLineEdit->text();
  float maxscale = v_scamp_maxscale.toFloat();
  if (maxscale > 2.0) v_scamp_maxscale = "2.0";
  QString v_scamp_maxoffset      = ac_sc_maxoffsetLineEdit->text();
  QString v_scamp_snlow          = ac_sc_minSNLineEdit->text();
  QString v_scamp_snhigh         = ac_sc_maxSNLineEdit->text();
  QString v_scamp_distortorder   = ac_sc_distortorderLineEdit->text();
  QString v_scamp_distortorder2  = ac_sc_distortorder2LineEdit->text();
  QString v_scamp_distortgroup   = ac_sc_distortgroupsLineEdit->text();
  QString v_scamp_distortkeys    = ac_sc_distortkeysLineEdit->text();
  QString v_scamp_fgroupradius   = ac_sc_fgroupradiusLineEdit->text();
  QString v_scamp_crossidradius  = ac_sc_crossidradiusLineEdit->text();
  QString v_scamp_astrefweight   = ac_sc_astrefweightLineEdit->text();
  QString v_scamp_astrinstrukey  = ac_sc_astrinstrukeyLineEdit->text();
  QString v_scamp_photinstrukey  = ac_sc_photinstrukeyLineEdit->text();
  QString v_an_distortorder      = ac_an_distortorderLineEdit->text();
  QString v_an_maxscale          = ac_an_maxscaleLineEdit->text();
  QString v_absphot_aperture     = absphot_apertureLineEdit->text();
  QString v_absphot_filter       = absphot_filterLineEdit->text();
  QString v_absphot_colorfixed   = absphot_colorfixedLineEdit->text();
  QString v_absphot_extinction   = absphot_extinctionLineEdit->text();
  QString v_absphot_maxphoterror = absphot_maxphoterrorLineEdit->text();
  QString v_absphot_wcserror     = absphot_wcserrorLineEdit->text();
  QString v_absphot_stdminmag    = absphot_stdminmagLineEdit->text();
  QString v_absphot_zpmin        = absphot_zpminLineEdit->text();
  QString v_absphot_zpmax        = absphot_zpmaxLineEdit->text();
  QString v_absphot_kappa        = absphot_kappaLineEdit->text();
  QString v_absphot_threshold    = absphot_thresholdLineEdit->text();
  QString v_absphot_maxiter      = absphot_maxiterLineEdit->text();
  QString v_absphot_convergence  = absphot_convergenceLineEdit->text();
  float extinction = v_absphot_extinction.toFloat();
  if (extinction >= 0.) {
    extinction = -1. * extinction;
    v_absphot_extinction = v_absphot_extinction.setNum(extinction);
    absphot_extinctionLineEdit->setText(v_absphot_extinction);
  }
  
  if (v_absphot_aperture.isEmpty()) v_absphot_aperture = "10";
  if (v_absphot_filter.isEmpty()) v_absphot_filter = "B";
  if (v_absphot_colorfixed.isEmpty()) v_absphot_colorfixed = "0.0";
  if (v_absphot_extinction.isEmpty()) v_absphot_extinction = "-0.1";
  //  if (v_absphot_zpmin.isEmpty()) v_absphot_zpmin = "24";
  // if (v_absphot_zpmax.isEmpty()) v_absphot_zpmax = "25";
  if (v_absphot_kappa.isEmpty()) v_absphot_kappa = "2.0";
  if (v_absphot_threshold.isEmpty()) v_absphot_threshold = "0.15";
  if (v_absphot_maxiter.isEmpty()) v_absphot_maxiter = "10";
  if (v_absphot_convergence.isEmpty()) v_absphot_convergence = "0.01";
  if (v_absphot_stdminmag.isEmpty()) v_absphot_stdminmag = "0.0";
  if (v_absphot_maxphoterror.isEmpty()) v_absphot_maxphoterror = "0.05";
  if (v_absphot_wcserror.isEmpty()) v_absphot_wcserror = "10";
  if (v_ap_detectthresh.isEmpty()) v_ap_detectthresh = "5.0";
  if (v_ap_detectminarea.isEmpty()) v_ap_detectminarea = "5";
  if (v_ap_deblendmincont.isEmpty()) v_ap_deblendmincont = "0.0005";
  if (v_ap_lownum.isEmpty()) v_ap_lownum = "1";
  if (v_ap_sexcatminfwhm.isEmpty()) v_ap_sexcatminfwhm = "1.5";
  if (v_ap_sexcatflag.isEmpty()) v_ap_sexcatflag = "0";
  if (v_ap_saturation.isEmpty()) v_ap_saturation = "";
  if (v_scamp_maxposangle.isEmpty()) v_scamp_maxposangle = "2.0";
  if (v_scamp_maxoffset.isEmpty()) v_scamp_maxoffset = "2.0";
  if (v_scamp_maxscale.isEmpty()) v_scamp_maxscale = "1.05";
  if (v_scamp_snlow.isEmpty()) v_scamp_snlow = "5";
  if (v_scamp_snhigh.isEmpty()) v_scamp_snhigh = "20";
  if (v_scamp_fgroupradius.isEmpty()) v_scamp_fgroupradius = "1.0";
  if (v_scamp_crossidradius.isEmpty()) v_scamp_crossidradius = "2.0";
  if (v_scamp_astrefweight.isEmpty()) v_scamp_astrefweight = "1.0";
  if (v_scamp_astrinstrukey.isEmpty()) v_scamp_astrinstrukey = "FILTER";
  if (v_scamp_photinstrukey.isEmpty()) v_scamp_photinstrukey = "FILTER";
  if (v_scamp_distortorder.isEmpty()) v_scamp_distortorder = "3";
  if (v_an_maxscale.isEmpty()) v_an_maxscale = "1.05";
  if (v_an_distortorder.isEmpty()) v_an_distortorder = "3";
  
  // absolute photometry
  // Loop over the max 10 nights and select those which have valid choices
  QString v_photcalstring = "";
  QString v_photcalchoice_night_1 = "0";
  QString v_photcalchoice_night_2 = "0";
  QString v_photcalchoice_night_3 = "0";
  QString v_photcalchoice_night_4 = "0";
  QString v_photcalchoice_night_5 = "0";
  QString v_photcalchoice_night_6 = "0";
  QString v_photcalchoice_night_7 = "0";
  QString v_photcalchoice_night_8 = "0";
  QString v_photcalchoice_night_9 = "0";
  QString v_photcalchoice_night_10 = "0";
  QString night, choice;
  if (NIGHT_1_ComboBox->currentItem() != 0 && !NIGHT_1_ComboBox->isHidden()) {
    night = NIGHT_1_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_1_ComboBox->currentItem());
    v_photcalstring.append(night+"_"+choice);
    v_photcalchoice_night_1 = choice;
  }
  if (NIGHT_2_ComboBox->currentItem() != 0 && !NIGHT_2_ComboBox->isHidden()) {
    night = NIGHT_2_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_2_ComboBox->currentItem());
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_2 = choice;
  }
  if (NIGHT_3_ComboBox->currentItem() != 0 && !NIGHT_3_ComboBox->isHidden()) {
    night = NIGHT_3_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_3_ComboBox->currentItem());
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_3 = choice;
  }
  if (NIGHT_4_ComboBox->currentItem() != 0 && !NIGHT_4_ComboBox->isHidden()) {
    night = NIGHT_4_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_4_ComboBox->currentItem());
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_4 = choice;
  }
  if (NIGHT_5_ComboBox->currentItem() != 0 && !NIGHT_5_ComboBox->isHidden()) {
    night = NIGHT_5_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_5_ComboBox->currentItem());
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_5 = choice;
  }
  if (NIGHT_6_ComboBox->currentItem() != 0 && !NIGHT_6_ComboBox->isHidden()) {
    night = NIGHT_6_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_6_ComboBox->currentItem());
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_6 = choice;
  }
  if (NIGHT_7_ComboBox->currentItem() != 0 && !NIGHT_7_ComboBox->isHidden()) {
    night = NIGHT_7_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_7_ComboBox->currentItem());
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_7 = choice;
  }
  if (NIGHT_8_ComboBox->currentItem() != 0 && !NIGHT_8_ComboBox->isHidden()) {
    night = NIGHT_8_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_8_ComboBox->currentItem());
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_8 = choice;
  }
  if (NIGHT_9_ComboBox->currentItem() != 0 && !NIGHT_9_ComboBox->isHidden()) {
    night = NIGHT_9_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_9_ComboBox->currentItem());
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_9 = choice;
  }
  if (NIGHT_10_ComboBox->currentItem() != 0 && !NIGHT_10_ComboBox->isHidden()) {
    night = NIGHT_10_PushButton->text().section('_', 1, 1);
    choice = choice.setNum(NIGHT_10_ComboBox->currentItem()-1);
    v_photcalstring.append("_"+night+"_"+choice);
    v_photcalchoice_night_10 = choice;
  }

  // COADDITION
  QString v_coadd_id;
  get_coadd_id(v_coadd_id);
  QString v_refra  = coc_refraLineEdit->text();
  QString v_refdec = coc_refdecLineEdit->text();
  QString v_ident  = coc_identLineEdit->text();
  QString v_filterthreshold = coc_filterthresholdLineEdit->text();
  QString v_filterclustersize = coc_filterclustersizeLineEdit->text();
  QString v_filterborderwidth = coc_filterborderwidthLineEdit->text();
  QString v_smoothedge  = coc_smoothedgeLineEdit->text();
  QString v_skyposangle = coc_skyposangleLineEdit->text();
  QString v_propra  = coc_propmotionraLineEdit->text();
  QString v_propdec = coc_propmotiondecLineEdit->text();
  QString v_coadd_filter = "";
  if (coc_filterComboBox->count() > 0)
    v_coadd_filter = coc_filterComboBox->currentText();
  
  // do not do any filtering if the user provides only one parameter
  if (v_filterclustersize.toInt() > 9) v_filterclustersize = "9";
  
  // transform dd.ddd into hh:mm:ss (if necessary)
  QRegExp rx4( "^[-]{0,1}[0-9]*[.]{0,1}[0-9]+[^:]$" );
  if (!v_refra.isEmpty()) {
    QString refstr1 = v_refra;
    if (rx4.search(refstr1) == 0) {
      execstr = bindir;
      execstr.append("/decimaltohms ");
      execstr.append(v_refra);
      my_fscanf(execstr,v_refra);
    }
  }
  if (!v_refdec.isEmpty()) {
    QString refstr2 = v_refdec;
    if (rx4.search(refstr2) == 0) {
      execstr = bindir;
      execstr.append("/decimaltodms ");
      execstr.append(v_refdec);
      my_fscanf(execstr,v_refdec);
    }
  }
  
  // WRITE THE LOG FILE
  {
    QSettings settings (QSettings::Ini);
    QString reductionlog = lastlog;
    if (reductionlog.isEmpty()) reductionlog = "noname";
    
    settings.beginGroup("/theli_"+reductionlog+"_");
    settings.beginGroup("/PREPARATION");
    settings.writeEntry("/rename_checkbox", pre_renameCheckBox->isChecked());
    settings.writeEntry("/rename_fitskey", v_rename_fitskey);
    settings.writeEntry("/sort_fitskey", v_sort_fitskey);
    settings.writeEntry("/sort_bias", v_sort_bias);
    settings.writeEntry("/sort_dark", v_sort_dark);
    settings.writeEntry("/sort_domeflat", v_sort_domeflat);
    settings.writeEntry("/sort_skyflat", v_sort_skyflat);
    settings.writeEntry("/sort_std", v_sort_std);
    settings.beginGroup("/corrections");
    settings.writeEntry("/xtalk_norcheckbox", xtalk_norCheckBox->isChecked());
    settings.writeEntry("/xtalk_rowcheckbox", xtalk_rowCheckBox->isChecked());
    settings.writeEntry("/xtalk_multicheckbox", xtalk_multiCheckBox->isChecked());
    settings.writeEntry("/xtalk_nor_amplitude", xtalk_nor_amplitudeLineEdit->text());
    settings.writeEntry("/xtalk_row_amplitude", xtalk_row_amplitudeLineEdit->text());
    settings.writeEntry("/xtalk_nor_buttonID", xtalk_norButtonGroup->selectedId());
    settings.writeEntry("/xtalk_row_buttonID", xtalk_rowButtonGroup->selectedId());
    settings.writeEntry("/xtalk_multi_buttonID", xtalk_multiButtonGroup->selectedId());
    settings.writeEntry("/xtalk_multi_nsection", xtalk_multi_nsectionLineEdit->text());
    settings.endGroup();
    settings.endGroup();
    
    settings.beginGroup("/CALIBRATION");
    settings.writeEntry("/ovscannlow", v_cal_ovscannlow);
    settings.writeEntry("/ovscannhigh", v_cal_ovscannhigh);
    settings.writeEntry("/biasnlow", v_cal_biasnlow);
    settings.writeEntry("/biasnhigh", v_cal_biasnhigh);
    settings.writeEntry("/darknlow", v_cal_darknlow);
    settings.writeEntry("/darknhigh", v_cal_darknhigh);
    settings.writeEntry("/flatnlow", v_cal_flatnlow);
    settings.writeEntry("/flatnhigh", v_cal_flatnhigh);
    settings.endGroup();
    
    settings.beginGroup("/BACKGROUND");
    settings.beginGroup("/background");
    settings.writeEntry("/nlow1",         v_back_nlow1);
    settings.writeEntry("/nhigh1",        v_back_nhigh1);
    settings.writeEntry("/nlow2",         v_back_nlow2);
    settings.writeEntry("/nhigh2",        v_back_nhigh2);
    settings.writeEntry("/maglimit",      v_back_maglimit);
    settings.writeEntry("/distance",      v_back_distance);
    settings.writeEntry("/detectthresh",  v_back_detectthresh);
    settings.writeEntry("/detectminarea", v_back_detectminarea);
    settings.writeEntry("/maskexpand",    v_back_maskexpand);
    settings.writeEntry("/illumsmooth",   v_back_illumsmooth);
    settings.writeEntry("/fringesmooth",  v_back_fringesmooth);
    settings.writeEntry("/windowsize",    v_back_windowsize);
    settings.writeEntry("/gapsize",       v_back_gapsize);
    settings.writeEntry("/combinemethod", sf_back_combinemethodComboBox->currentItem());
    settings.writeEntry("/applymode",     sf_back_applymodeComboBox->currentItem());
    settings.writeEntry("/sexfilter",     sf_back_sexfilterCheckBox->isChecked());
    settings.writeEntry("/adjustgains",   sf_back_adjustgainsCheckBox->isChecked());
    settings.writeEntry("/fringescale",   sf_back_fringescaleCheckBox->isChecked());
    settings.writeEntry("/twopass",       sf_back_twopassCheckBox->isChecked());
    settings.endGroup();
    settings.beginGroup("/collapse");
    settings.writeEntry("/detectthresh",  v_coll_detectthresh);
    settings.writeEntry("/detectminarea", v_coll_detectminarea);
    settings.writeEntry("/maskexpand",    v_coll_maskexpand);
    settings.writeEntry("/rejectthresh",  v_coll_rejectthresh);
    settings.writeEntry("/direction",  ccc_colldirComboBox->currentItem());
    settings.writeEntry("/maskaction", ccc_maskactionCheckBox->isChecked());
    settings.writeEntry("/autothreshold", ccc_collautothresholdCheckBox->isChecked());
    settings.writeEntry("/xmin", v_coll_xmin);
    settings.writeEntry("/xmax", v_coll_xmax);
    settings.writeEntry("/ymin", v_coll_ymin);
    settings.writeEntry("/ymax", v_coll_ymax);
    settings.endGroup();
    settings.endGroup();
    
    settings.beginGroup("/WEIGHTING");
    settings.writeEntry("/w_binmin", v_w_binmin);
    settings.writeEntry("/w_binmax", v_w_binmax);
    settings.writeEntry("/w_binsize", w_binsizeSpinBox->value());
    settings.writeEntry("/w_binoutlier", w_binoutlierCheckBox->isChecked());
    settings.writeEntry("/normflatmin", v_w_flatmin);
    settings.writeEntry("/normflatmax", v_w_flatmax);
    settings.writeEntry("/darkmin", v_w_darkmin);
    settings.writeEntry("/darkmax", v_w_darkmax);
    settings.writeEntry("/defect_kernelsize", v_def_kernelsize);
    settings.writeEntry("/defect_rowtol", v_def_rowtol);
    settings.writeEntry("/defect_coltol", v_def_coltol);
    settings.writeEntry("/defect_clustol", v_def_clustol);
    settings.writeEntry("/defect_kernelsize_sf", v_def_kernelsize_sf);
    settings.writeEntry("/defect_rowtol_sf", v_def_rowtol_sf);
    settings.writeEntry("/defect_coltol_sf", v_def_coltol_sf);
    settings.writeEntry("/defect_clustol_sf", v_def_clustol_sf);
    settings.writeEntry("/weightlowthreshold", weight_threshminLineEdit->text());
    settings.writeEntry("/weighthighthreshold", weight_threshmaxLineEdit->text());
    settings.writeEntry("/cosmicthreshold", v_cosmicsthreshold);
    settings.writeEntry("/cosmicDT", v_cosmicDT);
    settings.writeEntry("/cosmicDMIN", v_cosmicDMIN);
    settings.writeEntry("/bloomlowlimit", v_bloomlowlimit);
    settings.writeEntry("/bloomminarea", v_bloomminarea);
    settings.writeEntry("/bloomwidth", v_bloomwidth);
    settings.writeEntry("/bloomdomask", weight_bloomCheckBox->isChecked());
    if (globw_usebiasCheckBox->isChecked()) settings.writeEntry("/usebias", true);
    else settings.writeEntry("/usebias", false);
    if (globw_uniformweightCheckBox->isChecked())
      settings.writeEntry("/uniformweight", true);
    else
      settings.writeEntry("/uniformweight", false);
    settings.endGroup();
    
    settings.beginGroup("/ASTROMPHOTOM");
    settings.beginGroup("/sourcecat");
    settings.writeEntry("/detectthresh", v_ap_detectthresh);
    settings.writeEntry("/detectminarea", v_ap_detectminarea);
    settings.writeEntry("/deblendmincont", v_ap_deblendmincont);
    settings.writeEntry("/lownum", v_ap_lownum);
    settings.writeEntry("/sexcatminfwhm", v_ap_sexcatminfwhm);
    settings.writeEntry("/sexcatflag", v_ap_sexcatflag);
    settings.writeEntry("/saturation", v_ap_saturation);
    settings.writeEntry("/sexbacklevel", v_ap_sexbacklevel);
    settings.writeEntry("/filter", ap_filterCheckBox->isChecked());
    settings.endGroup();
    settings.beginGroup("/scamp");
    settings.writeEntry("/maxposangle", v_scamp_maxposangle);
    settings.writeEntry("/maxoffset", v_scamp_maxoffset);
    settings.writeEntry("/maxscale", v_scamp_maxscale);
    settings.writeEntry("/minsn", v_scamp_snlow);
    settings.writeEntry("/maxsn", v_scamp_snhigh);
    settings.writeEntry("/fgroupradius", v_scamp_fgroupradius);
    settings.writeEntry("/crossidradius", v_scamp_crossidradius);
    settings.writeEntry("/astrefweight", v_scamp_astrefweight);
    settings.writeEntry("/astrinstrukey", v_scamp_astrinstrukey);
    settings.writeEntry("/photinstrukey", v_scamp_photinstrukey);
    settings.writeEntry("/distortorder", v_scamp_distortorder);
    settings.writeEntry("/distortorder2", v_scamp_distortorder2);
    settings.writeEntry("/distortkeys", v_scamp_distortkeys);
    settings.writeEntry("/distortgroup", v_scamp_distortgroup);
    settings.writeEntry("/stability", ac_sc_stabilityComboBox->currentItem());
    settings.writeEntry("/mosaictype", ac_sc_mosaictypeComboBox->currentItem());
    settings.writeEntry("/focalplane", ac_sc_FPComboBox->currentItem());
    if (ac_sc_matchflippedCheckBox->isChecked()) settings.writeEntry("/matchflipped", true);
    else settings.writeEntry("/matchflipped", false);
    if (ac_sc_matchWCSCheckBox->isChecked()) settings.writeEntry("/matchWCS", true);
    else settings.writeEntry("/matchWCS", false);
    settings.endGroup();

    settings.beginGroup("/anet");
    settings.writeEntry("/distortorder", v_an_distortorder);
    settings.writeEntry("/maxscale", v_an_maxscale);
    if (ac_an_scampdistortCheckBox->isChecked()) settings.writeEntry("/scampdistort", true);
    else settings.writeEntry("/scampdistort", false);
    settings.endGroup();

    settings.beginGroup("/absphotom");
    settings.writeEntry("/stdcat_direct", absphot_direct_stdcatComboBox->currentItem());
    settings.writeEntry("/stdcat_indirect", absphot_indirect_stdcatComboBox->currentItem());
    settings.writeEntry("/aperture", v_absphot_aperture);
    settings.writeEntry("/obsfilter", v_absphot_filter);
    settings.writeEntry("/stdfilter", absphot_stdfilterComboBox->currentItem());
    settings.writeEntry("/stdcolor", absphot_stdcolorComboBox->currentItem());
    settings.writeEntry("/colorfixed", v_absphot_colorfixed);
    settings.writeEntry("/extinction", v_absphot_extinction);
    settings.writeEntry("/zpmin", v_absphot_zpmin);
    settings.writeEntry("/zpmax", v_absphot_zpmax);
    settings.writeEntry("/kappa", v_absphot_kappa);
    settings.writeEntry("/threshold", v_absphot_threshold);
    settings.writeEntry("/maxiter", v_absphot_maxiter);
    settings.writeEntry("/convergence", v_absphot_convergence);
    settings.writeEntry("/stdminmag", v_absphot_stdminmag);
    settings.writeEntry("/maxphoterror", v_absphot_maxphoterror);
    settings.writeEntry("/stdfilterM2", absphot_stdfilterM2_ComboBox->currentItem());
    settings.writeEntry("/fittingmethodM2", absphot_fittingmethodM2_ComboBox->currentItem());
    if (absphot_wcserrorCheckBox->isChecked()) settings.writeEntry("/wcserrorCheckBox", true);
    else settings.writeEntry("/wcserrorCheckBox", false);
    settings.writeEntry("/calmode", absphot_calmodeComboBox->currentItem());
    settings.writeEntry("/wcserror", v_absphot_wcserror);
    settings.endGroup();
    settings.beginGroup("/absphotom_nightselection");
    settings.writeEntry("/nightselection", v_photcalstring);
    settings.writeEntry("/photcalchoice_night_1", v_photcalchoice_night_1);
    settings.writeEntry("/photcalchoice_night_2", v_photcalchoice_night_2);
    settings.writeEntry("/photcalchoice_night_3", v_photcalchoice_night_3);
    settings.writeEntry("/photcalchoice_night_4", v_photcalchoice_night_4);
    settings.writeEntry("/photcalchoice_night_5", v_photcalchoice_night_5);
    settings.writeEntry("/photcalchoice_night_6", v_photcalchoice_night_6);
    settings.writeEntry("/photcalchoice_night_7", v_photcalchoice_night_7);
    settings.writeEntry("/photcalchoice_night_8", v_photcalchoice_night_8);
    settings.writeEntry("/photcalchoice_night_9", v_photcalchoice_night_9);
    settings.writeEntry("/photcalchoice_night_10", v_photcalchoice_night_10);
    settings.endGroup();
    settings.endGroup();
    
    settings.beginGroup("/COADDITION");
    settings.beginGroup("/coaddition");
    settings.writeEntry("/ra", v_refra);
    settings.writeEntry("/dec", v_refdec);
    settings.writeEntry("/id", v_ident);
    settings.writeEntry("/scriptid", v_coadd_id);
    settings.writeEntry("/skyposangle", v_skyposangle);
    settings.writeEntry("/pixscale", coc_pixscaleLineEdit->text());
    settings.writeEntry("/chips", coc_chipsLineEdit->text());
    settings.writeEntry("/filterthreshold", v_filterthreshold);
    settings.writeEntry("/filterclustersize", v_filterclustersize);
    settings.writeEntry("/filterborderwidth", v_filterborderwidth);
    settings.writeEntry("/smoothedge", v_smoothedge);
    settings.writeEntry("/propermotionra", v_propra);
    settings.writeEntry("/propermotiondec", v_propdec);
    settings.writeEntry("/coaddsizex", coc_coaddsizexLineEdit->text());
    settings.writeEntry("/coaddsizey", coc_coaddsizeyLineEdit->text());
    settings.writeEntry("/seeing", coc_seeingLineEdit->text());
    settings.writeEntry("/rzp", coc_RZPLineEdit->text());
    settings.writeEntry("/speedunit", coc_speedunitComboBox->currentItem());
    settings.writeEntry("/celestialtype", coc_celestialtypeComboBox->currentItem());
    settings.writeEntry("/coaddfilteritem", coc_filterComboBox->currentItem());
    settings.writeEntry("/coaddfiltertext", v_coadd_filter);
    settings.writeEntry("/coaddfilternumb", nfiltstring);
    settings.writeEntry("/projection", coc_projectionComboBox->currentItem());
    settings.writeEntry("/celestialtype2", coc_celestialtypeComboBox->currentText());
    settings.writeEntry("/projection2", coc_projectionComboBox->currentText());
    settings.writeEntry("/kernel", coc_kernelComboBox->currentItem());
    settings.writeEntry("/combinetype", coc_combineComboBox->currentItem());
    settings.writeEntry("/clipampfrac", coc_clipampfracLineEdit->text());
    settings.writeEntry("/clipsigma", coc_clipsigmaLineEdit->text());
    settings.writeEntry("/rescaleweights", coc_rescaleweightsCheckBox->isChecked());
    settings.endGroup();
    settings.endGroup();
    
    settings.endGroup();
  }
  
  // write the parameter file
  modify_parametersini();
}


// read the parameters from the Qt file when the dialog opens
void parameters::read_parameters()
{
  QSettings settings (QSettings::Ini);
  
  QString reductionlog = lastlog;
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
  }

  settings.beginGroup("/theli_"+reductionlog+"_");
  
  // PREPARATION
  settings.beginGroup("/PREPARATION");
  pre_renameCheckBox->setChecked(settings.readBoolEntry("/rename_checkbox",false));
  pre_rename_fitskeyLineEdit->setText(settings.readEntry("/rename_fitskey", ""));
  pre_sort_fitskeyLineEdit->setText(settings.readEntry("/sort_fitskey", ""));
  pre_sort_biasLineEdit->setText(settings.readEntry("/sort_bias", ""));
  pre_sort_darkLineEdit->setText(settings.readEntry("/sort_dark", ""));
  pre_sort_domeflatLineEdit->setText(settings.readEntry("/sort_domeflat", ""));
  pre_sort_skyflatLineEdit->setText(settings.readEntry("/sort_skyflat", ""));
  pre_sort_stdLineEdit->setText(settings.readEntry("/sort_std", ""));
  settings.beginGroup("/corrections");
  xtalk_norCheckBox->setChecked(settings.readBoolEntry("/xtalk_norcheckbox",false));
  xtalk_rowCheckBox->setChecked(settings.readBoolEntry("/xtalk_rowcheckbox",false));
  xtalk_multiCheckBox->setChecked(settings.readBoolEntry("/xtalk_multicheckbox",false));
  xtalk_nor_amplitudeLineEdit->setText(settings.readEntry("/xtalk_nor_amplitude", ""));
  xtalk_row_amplitudeLineEdit->setText(settings.readEntry("/xtalk_row_amplitude", ""));
  xtalk_norButtonGroup->setButton(settings.readNumEntry("/xtalk_nor_buttonID",0));
  xtalk_rowButtonGroup->setButton(settings.readNumEntry("/xtalk_row_buttonID",0));
  xtalk_multiButtonGroup->setButton(settings.readNumEntry("/xtalk_multi_buttonID",0));
  xtalk_multi_nsectionLineEdit->setText(settings.readEntry("/xtalk_multi_nsection", ""));
  active_passive_xtalk();
  settings.endGroup();
  settings.endGroup();
  
  // CALIBRATION
  settings.beginGroup("/CALIBRATION");
  ovscan_nlowLineEdit->setText(settings.readEntry("/ovscannlow","0"));
  ovscan_nhighLineEdit->setText(settings.readEntry("/ovscannhigh","1"));
  bias_nlowLineEdit->setText(settings.readEntry("/biasnlow","0"));
  bias_nhighLineEdit->setText(settings.readEntry("/biasnhigh","1"));
  dark_nlowLineEdit->setText(settings.readEntry("/darknlow","0"));
  dark_nhighLineEdit->setText(settings.readEntry("/darknhigh","1"));
  flat_nlowLineEdit->setText(settings.readEntry("/flatnlow","0"));
  flat_nhighLineEdit->setText(settings.readEntry("/flatnhigh","1"));
  settings.endGroup();

  // BACKGROUND
  settings.beginGroup("/BACKGROUND");
  settings.beginGroup("/background");
  sf_back_nlow1LineEdit->setText(settings.readEntry("/nlow1","0"));
  sf_back_nhigh1LineEdit->setText(settings.readEntry("/nhigh1","0"));
  sf_back_nlow2LineEdit->setText(settings.readEntry("/nlow2","0"));
  sf_back_nhigh2LineEdit->setText(settings.readEntry("/nhigh2","1"));
  sf_back_maglimitLineEdit->setText(settings.readEntry("/maglimit",""));
  sf_back_distanceLineEdit->setText(settings.readEntry("/distance",""));
  sf_back_serverComboBox->setCurrentItem(settings.readNumEntry("/server",0));
  sf_back_detectthreshLineEdit->setText(settings.readEntry("/detectthresh","1.3"));
  sf_back_detectminareaLineEdit->setText(settings.readEntry("/detectminarea","5"));
  sf_back_maskexpandfactorLineEdit->setText(settings.readEntry("/maskexpand",""));
  sf_back_illumsmoothLineEdit->setText(settings.readEntry("/illumsmooth",""));
  sf_back_fringesmoothLineEdit->setText(settings.readEntry("/fringesmooth",""));
  sf_back_windowsizeLineEdit->setText(settings.readEntry("/windowsize","0"));
  sf_back_gapsizeLineEdit->setText(settings.readEntry("/gapsize",""));
  sf_back_combinemethodComboBox->setCurrentItem(settings.readNumEntry("/combinemethod",0));
  sf_back_applymodeComboBox->setCurrentItem(settings.readNumEntry("/applymode",0));
  sf_back_sexfilterCheckBox->setChecked(settings.readBoolEntry("/sexfilter",true));
  sf_back_adjustgainsCheckBox->setChecked(settings.readBoolEntry("/adjustgains",false));
  sf_back_fringescaleCheckBox->setChecked(settings.readBoolEntry("/fringescale",true));
  sf_back_twopassCheckBox->setChecked(settings.readBoolEntry("/twopass",true));
  settings.endGroup();
  settings.beginGroup("/collapse");
  ccc_colldetectthreshLineEdit->setText(settings.readEntry("/detectthresh",""));
  ccc_colldetectminareaLineEdit->setText(settings.readEntry("/detectminarea",""));
  ccc_collmaskexpandfactorLineEdit->setText(settings.readEntry("/maskexpand",""));
  ccc_collrejectthreshLineEdit->setText(settings.readEntry("/rejectthresh","1.5"));
  ccc_colldirComboBox->setCurrentItem(settings.readNumEntry("/direction",0));
  ccc_maskactionCheckBox->setChecked(settings.readBoolEntry("/maskaction",true));
  ccc_collautothresholdCheckBox->setChecked(settings.readBoolEntry("/autothreshold",false));
  ccc_collxminLineEdit->setText(settings.readEntry("/xmin",""));
  ccc_collxmaxLineEdit->setText(settings.readEntry("/xmax",""));
  ccc_collyminLineEdit->setText(settings.readEntry("/ymin",""));
  ccc_collymaxLineEdit->setText(settings.readEntry("/ymax",""));
  adjust_collcorr_direction();
  settings.endGroup();
  settings.endGroup();

  // WEIGHTING
  settings.beginGroup("/WEIGHTING");
  w_binminLineEdit->setText(settings.readEntry("/w_binmin","-100"));
  w_binmaxLineEdit->setText(settings.readEntry("/w_binmax","500"));
  w_binsizeSpinBox->setValue(settings.readNumEntry("/w_binsize",4));
  w_binoutlierCheckBox->setChecked(settings.readBoolEntry("/w_binoutlier",false));
  globw_flatminLineEdit->setText(settings.readEntry("/normflatmin","0.5"));
  globw_flatmaxLineEdit->setText(settings.readEntry("/normflatmax","1.6"));
  globw_darkminLineEdit->setText(settings.readEntry("/darkmin",""));
  globw_darkmaxLineEdit->setText(settings.readEntry("/darkmax",""));
  globw_usebiasCheckBox->setChecked(settings.readBoolEntry("/usebias",false));
  globw_uniformweightCheckBox->setChecked(settings.readBoolEntry("/uniformweight",false));
  defect_kernelsizeLineEdit->setText(settings.readEntry("/defect_kernelsize",""));
  defect_rowtolLineEdit->setText(settings.readEntry("/defect_rowtol",""));
  defect_coltolLineEdit->setText(settings.readEntry("/defect_coltol",""));
  defect_clustolLineEdit->setText(settings.readEntry("/defect_clustol",""));
  defect_kernelsize_sfLineEdit->setText(settings.readEntry("/defect_kernelsize_sf",""));
  defect_rowtol_sfLineEdit->setText(settings.readEntry("/defect_rowtol_sf",""));
  defect_coltol_sfLineEdit->setText(settings.readEntry("/defect_coltol_sf",""));
  defect_clustol_sfLineEdit->setText(settings.readEntry("/defect_clustol_sf",""));
  weight_threshminLineEdit->setText(settings.readEntry("/weightlowthreshold",""));
  weight_threshmaxLineEdit->setText(settings.readEntry("/weighthighthreshold",""));
  weight_cosmicLineEdit->setText(settings.readEntry("/cosmicthreshold","0.1"));
  weight_cosmicDTLineEdit->setText(settings.readEntry("/cosmicDT","6"));
  weight_cosmicDMINLineEdit->setText(settings.readEntry("/cosmicDMIN","1"));
  weight_bloomCheckBox->setChecked(settings.readBoolEntry("/bloomdomask",false));
  weight_bloomlowlimitLineEdit->setText(settings.readEntry("/bloomlowlimit","20000"));
  weight_bloomwidthLineEdit->setText(settings.readEntry("/bloomwidth","0"));
  weight_bloomminareaLineEdit->setText(settings.readEntry("/bloomminarea","100"));
  settings.endGroup();

  // ASTROMPHOTOM
  settings.beginGroup("/ASTROMPHOTOM");
  settings.beginGroup("/sourcecat");
  ap_detectthreshLineEdit->setText(settings.readEntry("/detectthresh","5.0"));
  ap_detectminareaLineEdit->setText(settings.readEntry("/detectminarea","5"));
  ap_deblendLineEdit->setText(settings.readEntry("/deblendmincont","0.0005"));
  ap_lownumLineEdit->setText(settings.readEntry("/lownum","1"));
  ap_minfwhmLineEdit->setText(settings.readEntry("/sexcatminfwhm","1.5"));
  ap_flagLineEdit->setText(settings.readEntry("/sexcatflag","0"));
  ap_saturationLineEdit->setText(settings.readEntry("/saturation",""));
  ap_sexbacklevelLineEdit->setText(settings.readEntry("/sexbacklevel",""));
  ap_filterCheckBox->setChecked(settings.readBoolEntry("/filter", false));
  settings.endGroup();
  settings.beginGroup("/scamp");
  ac_sc_maxposangleLineEdit->setText(settings.readEntry("/maxposangle","2.0"));
  ac_sc_maxoffsetLineEdit->setText(settings.readEntry("/maxoffset","2.0"));
  ac_sc_maxscaleLineEdit->setText(settings.readEntry("/maxscale","1.05"));
  ac_sc_minSNLineEdit->setText(settings.readEntry("/minsn","5"));
  ac_sc_maxSNLineEdit->setText(settings.readEntry("/maxsn","20"));
  ac_sc_fgroupradiusLineEdit->setText(settings.readEntry("/fgroupradius","1.0"));
  QString radius;
  get_crossidradius(radius);
  ac_sc_crossidradiusLineEdit->setText(settings.readEntry("/crossidradius",radius));
  ac_sc_astrefweightLineEdit->setText(settings.readEntry("/astrefweight","1.0"));
  ac_sc_astrinstrukeyLineEdit->setText(settings.readEntry("/astrinstrukey","FILTER"));
  ac_sc_photinstrukeyLineEdit->setText(settings.readEntry("/photinstrukey","FILTER"));
  ac_sc_distortorderLineEdit->setText(settings.readEntry("/distortorder","3"));
  ac_sc_distortorder2LineEdit->setText(settings.readEntry("/distortorder2",""));
  ac_sc_distortgroupsLineEdit->setText(settings.readEntry("/distortgroup",""));
  ac_sc_distortkeysLineEdit->setText(settings.readEntry("/distortkeys",""));
  ac_sc_stabilityComboBox->setCurrentItem(settings.readNumEntry("/stability",0));

  bool ok = 1;
  ac_sc_mosaictypeComboBox->setCurrentItem(settings.readNumEntry("/mosaictype",0,&ok));
  get_numchips();
  if (!ok && nchips_int > 1) ac_sc_mosaictypeComboBox->setCurrentItem(1);

  ac_sc_FPComboBox->setCurrentItem(settings.readNumEntry("/focalplane",0));
  ac_sc_matchflippedCheckBox->setChecked(settings.readBoolEntry("/matchflipped",false));
  ac_sc_matchWCSCheckBox->setChecked(settings.readBoolEntry("/matchWCS",true));
  settings.endGroup();

  settings.beginGroup("/anet");
  ac_an_maxscaleLineEdit->setText(settings.readEntry("/maxscale","1.05"));
  ac_an_distortorderLineEdit->setText(settings.readEntry("/distortorder",""));
  ac_an_scampdistortCheckBox->setChecked(settings.readBoolEntry("/scampdistort",false));
  settings.endGroup();

  settings.beginGroup("/absphotom");
  absphot_direct_stdcatComboBox->setCurrentItem(settings.readNumEntry("/stdcat_direct",0));
  absphot_indirect_stdcatComboBox->setCurrentItem(settings.readNumEntry("/stdcat_indirect",0));
  // fill in the sdss/mass combo box
  update_stdcolor_comboboxes();
  absphot_apertureLineEdit->setText(settings.readEntry("/aperture","10"));
  absphot_filterLineEdit->setText(settings.readEntry("/obsfilter","B"));

  // make sure combo boxes show only what they are supposed to show.
  init_stdcolor_comboboxes();

  // done separately elsewhere to avoid setting ComboBoxes with indices
  // out of range
  absphot_stdfilterComboBox->setCurrentItem(settings.readNumEntry("/stdfilter",0));
  absphot_stdcolorComboBox->setCurrentItem(settings.readNumEntry("/stdcolor",0));
  absphot_colorfixedLineEdit->setText(settings.readEntry("/colorfixed","0.0"));
  absphot_extinctionLineEdit->setText(settings.readEntry("/extinction","-0.1"));
  absphot_zpminLineEdit->setText(settings.readEntry("/zpmin"));
  absphot_zpmaxLineEdit->setText(settings.readEntry("/zpmax"));
  absphot_kappaLineEdit->setText(settings.readEntry("/kappa","2.0"));
  absphot_thresholdLineEdit->setText(settings.readEntry("/threshold","0.15"));
  absphot_maxiterLineEdit->setText(settings.readEntry("/maxiter","10"));
  absphot_convergenceLineEdit->setText(settings.readEntry("/convergence","0.01"));
  absphot_stdminmagLineEdit->setText(settings.readEntry("/stdminmag","0.0"));
  absphot_maxphoterrorLineEdit->setText(settings.readEntry("/maxphoterror","0.05"));
  absphot_stdfilterM2_ComboBox->setCurrentItem(settings.readNumEntry("/stdfilterM2",0));
  absphot_fittingmethodM2_ComboBox->setCurrentItem(settings.readNumEntry("/fittingmethodM2",0));
  absphot_calmodeComboBox->setCurrentItem(settings.readNumEntry("/calmode",0));
  absphot_wcserrorLineEdit->setText(settings.readEntry("/wcserror","10"));
  absphot_wcserrorCheckBox->setChecked(settings.readBoolEntry("/wcserrorCheckBox",true));
  settings.endGroup();
  settings.beginGroup("/absphotom_nightselection");
  NIGHT_1_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_1",0));
  NIGHT_2_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_2",0));
  NIGHT_3_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_3",0));
  NIGHT_4_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_4",0));
  NIGHT_5_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_5",0));
  NIGHT_6_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_6",0));
  NIGHT_7_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_7",0));
  NIGHT_8_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_8",0));
  NIGHT_9_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_9",0));
  NIGHT_10_ComboBox->setCurrentItem(settings.readNumEntry("/photcalchoice_night_10",0));
  settings.endGroup();
  settings.endGroup();
  
  settings.beginGroup("/COADDITION");
  settings.beginGroup("/coaddition");
  coc_refraLineEdit->setText(settings.readEntry("/ra",""));
  coc_refdecLineEdit->setText(settings.readEntry("/dec",""));
  coc_identLineEdit->setText(settings.readEntry("/id",""));
  coc_skyposangleLineEdit->setText(settings.readEntry("/skyposangle",""));
  coc_pixscaleLineEdit->setText(settings.readEntry("/pixscale",""));
  get_pixscale();

  coc_chipsLineEdit->setText(settings.readEntry("/chips",""));
  coc_seeingLineEdit->setText(settings.readEntry("/seeing",""));
  coc_RZPLineEdit->setText(settings.readEntry("/rzp",""));
  coc_filterthresholdLineEdit->setText(settings.readEntry("/filterthreshold",""));
  coc_filterclustersizeLineEdit->setText(settings.readEntry("/filterclustersize",""));
  coc_filterborderwidthLineEdit->setText(settings.readEntry("/filterborderwidth",""));
  coc_smoothedgeLineEdit->setText(settings.readEntry("/smoothedge",""));
  coc_propmotionraLineEdit->setText(settings.readEntry("/propermotionra",""));
  coc_propmotiondecLineEdit->setText(settings.readEntry("/propermotiondec",""));
  coc_coaddsizexLineEdit->setText(settings.readEntry("/coaddsizex",""));
  coc_coaddsizeyLineEdit->setText(settings.readEntry("/coaddsizey",""));
  int maxitems = coc_filterComboBox->count();
  int currentitem = settings.readNumEntry("/coaddfilteritem",0);
  if (currentitem > maxitems) currentitem = 0;
  coc_filterComboBox->setCurrentItem(currentitem);
  coc_speedunitComboBox->setCurrentItem(settings.readNumEntry("/speedunit",1));
  coc_kernelComboBox->setCurrentItem(settings.readNumEntry("/kernel",3));
  coc_projectionComboBox->setCurrentItem(settings.readNumEntry("/projection",1));
  coc_celestialtypeComboBox->setCurrentItem(settings.readNumEntry("/celestialtype",0));
  coc_combineComboBox->setCurrentItem(settings.readNumEntry("/combinetype",0));
  coc_clipampfracLineEdit->setText(settings.readEntry("/clipampfrac","0.3"));
  coc_clipsigmaLineEdit->setText(settings.readEntry("/clipsigma","4"));
  coc_rescaleweightsCheckBox->setChecked(settings.readBoolEntry("/rescaleweights",false));
  settings.endGroup();
  settings.endGroup();
  
  // copy the reference coordinates if the coaddition ra dec fields are empty
  // and if the ap ref coordinates are not empty
  QString ra  = coc_refraLineEdit->text();
  QString dec = coc_refdecLineEdit->text();
  settings.beginGroup("/ASTROMPHOTOM");
  if (ra.isEmpty()) {
    ra = settings.readEntry("/ra","");
    if (!ra.isEmpty() && ra.compare("from header") != 0)
      coc_refraLineEdit->setText(settings.readEntry("/ra",""));
  }
  if (dec.isEmpty()) {
    dec = settings.readEntry("/dec","");
    if (!dec.isEmpty() && dec.compare("from header") != 0)
      coc_refdecLineEdit->setText(settings.readEntry("/dec",""));
  }
  settings.endGroup();
  settings.endGroup();
}


// write the parameters contained in the Qt file into
// an ascii file for usage in the scripts
void parameters::modify_parametersini()
{
    FILE *of;
    bool tmp1, tmp2, tmp3;

    int speed_ind;
    double propmotionra, propmotiondec, speedcorrection;

    QString paramsinifile, paramsinifiletmp, test_ra, test_dec;

    QSettings settings(QSettings::Ini);
    QString execstr;

    //    if (!scriptdir.endsWith("/")) 
    //	scriptdir.append("/");

    check_last_char(homedir);

    paramsinifile = homedir;
    paramsinifile.append(".theli/param_set2.ini");
    paramsinifiletmp = homedir;
    paramsinifiletmp.append(".theli/param_set2.ini_tmp");

    QString reductionlog = lastlog;
    if (reductionlog.isEmpty()) {
	reductionlog = "noname";
    }

    of = fopen(paramsinifiletmp, "w");

    // read what is written in the Qt file
    settings.beginGroup("/theli_"+reductionlog+"_");

    // PREPARATION
    settings.beginGroup("/PREPARATION");
    tmp1 = settings.readBoolEntry("/rename_checkbox", false);
    if (tmp1) fprintf(of, "%s\n", "V_PRE_RENAME_CHECKED=1");
    else fprintf(of, "%s\n", "V_PRE_RENAME_CHECKED=0");
    fprintf(of, "%s%s\n", "V_RENAME_FITSKEY=",(settings.readEntry("/rename_fitskey", "")).ascii());
    fprintf(of, "%s%s\n", "V_SORT_FITSKEY=",(settings.readEntry("/sort_fitskey", "")).ascii());
    fprintf(of, "%s%s\n", "V_SORT_BIAS=",(settings.readEntry("/sort_bias", "")).ascii());
    fprintf(of, "%s%s\n", "V_SORT_DARK=",(settings.readEntry("/sort_dark", "")).ascii());
    fprintf(of, "%s%s\n", "V_SORT_DOMEFLAT=",(settings.readEntry("/sort_domeflat", "")).ascii());
    fprintf(of, "%s%s\n", "V_SORT_SKYFLAT=",(settings.readEntry("/sort_skyflat", "")).ascii());
    fprintf(of, "%s%s\n", "V_SORT_STD=",(settings.readEntry("/sort_std", "")).ascii());
    settings.beginGroup("/corrections");
    tmp1 = settings.readBoolEntry("/xtalk_norcheckbox", false);
    tmp2 = settings.readBoolEntry("/xtalk_rowcheckbox", false);
    tmp3 = settings.readBoolEntry("/xtalk_multicheckbox", false);
    if (tmp1) fprintf(of, "%s\n", "V_PRE_XTALK_NOR_CHECKED=1");
    else fprintf(of, "%s\n", "V_PRE_XTALK_NOR_CHECKED=0");
    if (tmp2) fprintf(of, "%s\n", "V_PRE_XTALK_ROW_CHECKED=1");
    else fprintf(of, "%s\n", "V_PRE_XTALK_ROW_CHECKED=0");
    if (tmp3) fprintf(of, "%s\n", "V_PRE_XTALK_MULTI_CHECKED=1");
    else fprintf(of, "%s\n", "V_PRE_XTALK_MULTI_CHECKED=0");
    fprintf(of, "%s%s\n", "V_PRE_XTALK_NOR_AMPLITUDE=",(settings.readEntry("/xtalk_nor_amplitude", "")).ascii());
    fprintf(of, "%s%s\n", "V_PRE_XTALK_ROW_AMPLITUDE=",(settings.readEntry("/xtalk_row_amplitude", "")).ascii());
    fprintf(of, "%s%d\n", "V_PRE_XTALK_NOR_BUTTONID=",settings.readNumEntry("/xtalk_nor_buttonID", 0));
    fprintf(of, "%s%d\n", "V_PRE_XTALK_ROW_BUTTONID=",settings.readNumEntry("/xtalk_row_buttonID", 0));
    fprintf(of, "%s%d\n", "V_PRE_XTALK_MULTI_BUTTONID=",settings.readNumEntry("/xtalk_multi_buttonID", 0));
    fprintf(of, "%s%s\n", "V_PRE_XTALK_MULTI_NSECTION=",(settings.readEntry("/xtalk_multi_nsection", "")).ascii());
    settings.endGroup();
    settings.endGroup();

    // CALIBRATION
    settings.beginGroup("/CALIBRATION");
    fprintf(of, "%s%s\n", "V_CAL_OVSCANNLOW=",    (settings.readEntry("/ovscannlow","0")).ascii());
    fprintf(of, "%s%s\n", "V_CAL_OVSCANNHIGH=",   (settings.readEntry("/ovscannhigh","1")).ascii());
    fprintf(of, "%s%s\n", "V_CAL_BIASNLOW=",      (settings.readEntry("/biasnlow","0")).ascii());
    fprintf(of, "%s%s\n", "V_CAL_BIASNHIGH=",     (settings.readEntry("/biasnhigh","1")).ascii());
    fprintf(of, "%s%s\n", "V_CAL_DARKNLOW=",      (settings.readEntry("/darknlow","0")).ascii());
    fprintf(of, "%s%s\n", "V_CAL_DARKNHIGH=",     (settings.readEntry("/darknhigh","1")).ascii());
    fprintf(of, "%s%s\n", "V_CAL_FLATNLOW=",      (settings.readEntry("/flatnlow","0")).ascii());
    fprintf(of, "%s%s\n", "V_CAL_FLATNHIGH=",     (settings.readEntry("/flatnhigh","1")).ascii());

    // BACKGROUND
    QString server;
    switch(sf_back_serverComboBox->currentItem()) {
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

    settings.beginGroup("/BACKGROUND");
    settings.beginGroup("/background");
    fprintf(of, "%s%s\n", "V_BACK_NLOW1=",         (settings.readEntry("/nlow1","0")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_NHIGH1=",        (settings.readEntry("/nhigh1","0")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_NLOW2=",         (settings.readEntry("/nlow2","0")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_NHIGH2=",        (settings.readEntry("/nhigh2","1")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_MAGLIMIT=",      (settings.readEntry("/maglimit","")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_DISTANCE=",      (settings.readEntry("/distance","")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_SERVER=",         server.ascii());
    fprintf(of, "%s%s\n", "V_BACK_DETECTTHRESH=",  (settings.readEntry("/detectthresh","1.3")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_DETECTMINAREA=", (settings.readEntry("/detectminarea","5")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_MASKEXPAND=",    (settings.readEntry("/maskexpand","")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_ILLUMSMOOTH=",   (settings.readEntry("/illumsmooth","")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_FRINGESMOOTH=",  (settings.readEntry("/fringesmooth","")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_WINDOWSIZE=",    (settings.readEntry("/windowsize","0")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_GAPSIZE=",       (settings.readEntry("/gapsize","")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_APPLYMODE=",     (settings.readEntry("/applymode","0")).ascii());
    fprintf(of, "%s%s\n", "V_BACK_COMBINEMETHOD=", (sf_back_combinemethodComboBox->currentText()).ascii());
    if (settings.readBoolEntry("/sexfilter",true)) fprintf(of, "%s\n", "V_BACK_SEXFILTER=Y");
    else fprintf(of, "%s\n", "V_BACK_SEXFILTER=N");
    if (settings.readBoolEntry("/adjustgains",false)) fprintf(of, "%s\n", "V_BACK_ADJUSTGAINS=Y");
    else fprintf(of, "%s\n", "V_BACK_ADJUSTGAINS=N");
    if (settings.readBoolEntry("/fringescale",true)) fprintf(of, "%s\n", "V_BACK_FRINGESCALE=Y");
    else fprintf(of, "%s\n", "V_BACK_FRINGESCALE=N");
    if (settings.readBoolEntry("/twopass",true)) fprintf(of, "%s\n", "V_BACK_TWOPASS=Y");
    else fprintf(of, "%s\n", "V_BACK_TWOPASS=N");
    settings.endGroup();
    settings.beginGroup("/collapse");
    fprintf(of, "%s%s\n", "V_COLLDETECTTHRESH=",  (settings.readEntry("/detectthresh","")).ascii());
    fprintf(of, "%s%s\n", "V_COLLDETECTMINAREA=", (settings.readEntry("/detectminarea","")).ascii());
    fprintf(of, "%s%s\n", "V_COLLMASKEXPAND=",    (settings.readEntry("/maskexpand","")).ascii());
    fprintf(of, "%s%s\n", "V_COLLREJECTTHRESH=",  (settings.readEntry("/rejectthresh","1.5")).ascii());
    if (collapseregion_groupbox->isEnabled()) {
      fprintf(of, "%s%s\n", "V_COLLXMIN=", (settings.readEntry("/xmin","")).ascii());
      fprintf(of, "%s%s\n", "V_COLLXMAX=", (settings.readEntry("/xmax","")).ascii());
      fprintf(of, "%s%s\n", "V_COLLYMIN=", (settings.readEntry("/ymin","")).ascii());
      fprintf(of, "%s%s\n", "V_COLLYMAX=", (settings.readEntry("/ymax","")).ascii());
    } else {
      fprintf(of, "%s\n", "V_COLLXMIN=");
      fprintf(of, "%s\n", "V_COLLXMAX=");
      fprintf(of, "%s\n", "V_COLLYMIN=");
      fprintf(of, "%s\n", "V_COLLYMAX=");
    }

    fprintf(of, "%s%s\n", "V_COLLDIRECTION=", (ccc_colldirComboBox->currentText()).ascii());
    if (settings.readBoolEntry("/maskaction",true))
	fprintf(of, "%s\n", "V_COLLMASKACTION=1");
    else 
	fprintf(of, "%s\n", "V_COLLMASKACTION=0");	
    if (settings.readBoolEntry("/autothreshold",false))
	fprintf(of, "%s\n", "V_COLLAUTOTHRESHOLD=1");
    else 
	fprintf(of, "%s\n", "V_COLLAUTOTHRESHOLD=0");	
    settings.endGroup();
    settings.endGroup();

    // WEIGHTING
    settings.beginGroup("/WEIGHTING");
    fprintf(of, "%s%s\n", "V_WEIGHTLOWTHRESHOLD=", (settings.readEntry("/weightlowthreshold","")).ascii());
    fprintf(of, "%s%s\n", "V_WEIGHTHIGHTHRESHOLD=", (settings.readEntry("/weighthighthreshold","")).ascii());
    fprintf(of, "%s%s\n", "V_WEIGHTBINMIN=", (settings.readEntry("/w_binmin","-100")).ascii());
    fprintf(of, "%s%s\n", "V_WEIGHTBINMAX=", (settings.readEntry("/w_binmax","500")).ascii());
    fprintf(of, "%s%s\n", "V_GLOBWFLATMIN=", (settings.readEntry("/normflatmin","0.5")).ascii());
    fprintf(of, "%s%s\n", "V_GLOBWFLATMAX=", (settings.readEntry("/normflatmax","1.6")).ascii());
    fprintf(of, "%s%s\n", "V_GLOBWDARKMIN=", (settings.readEntry("/darkmin","")).ascii());
    fprintf(of, "%s%s\n", "V_GLOBWDARKMAX=", (settings.readEntry("/darkmax","")).ascii());
    fprintf(of, "%s%s\n", "V_WEIGHTBINSIZE=", (settings.readEntry("/w_binsize","4")).ascii());
    fprintf(of, "%s%s\n", "V_DEFECT_KERNELSIZE=", (settings.readEntry("/defect_kernelsize","")).ascii());
    fprintf(of, "%s%s\n", "V_DEFECT_ROWTOL=", (settings.readEntry("/defect_rowtol","")).ascii());
    fprintf(of, "%s%s\n", "V_DEFECT_COLTOL=", (settings.readEntry("/defect_coltol","")).ascii());
    fprintf(of, "%s%s\n", "V_DEFECT_CLUSTOL=", (settings.readEntry("/defect_clustol","")).ascii());    
    fprintf(of, "%s%s\n", "V_DEFECT_KERNELSIZE_SF=", (settings.readEntry("/defect_kernelsize_sf","")).ascii());
    fprintf(of, "%s%s\n", "V_DEFECT_ROWTOL_SF=", (settings.readEntry("/defect_rowtol_sf","")).ascii());
    fprintf(of, "%s%s\n", "V_DEFECT_COLTOL_SF=", (settings.readEntry("/defect_coltol_Sf","")).ascii());
    fprintf(of, "%s%s\n", "V_DEFECT_CLUSTOL_SF=", (settings.readEntry("/defect_clustol_sf","")).ascii());    
    fprintf(of, "%s%s\n", "V_COSMICSTHRESHOLD=", (settings.readEntry("/cosmicthreshold","0.1")).ascii());
    fprintf(of, "%s%s\n", "V_COSMICDT=", (settings.readEntry("/cosmicDT","6")).ascii());
    fprintf(of, "%s%s\n", "V_COSMICDMIN=", (settings.readEntry("/cosmicDMIN","1")).ascii());
    fprintf(of, "%s%s\n", "V_BLOOMLOWLIMIT=", (settings.readEntry("/bloomlowlimit","20000")).ascii());
    fprintf(of, "%s%s\n", "V_BLOOMMINAREA=", (settings.readEntry("/bloomminarea","100")).ascii());
    fprintf(of, "%s%s\n", "V_BLOOMWIDTH=", (settings.readEntry("/bloomwidth","0")).ascii());
    if (settings.readBoolEntry("/w_binoutlier",false)) 
	fprintf(of, "%s\n", "V_WEIGHT_BINOUTLIER=TRUE");
    else
	fprintf(of, "%s\n", "V_WEIGHT_BINOUTLIER=FALSE");
    tmp1 = settings.readBoolEntry("/bloomdomask", false);
    if (tmp1) fprintf(of, "%s\n", "V_MASKBLOOMSPIKE=1");
    else fprintf(of, "%s\n", "V_MASKBLOOMSPIKE=0");
    if (settings.readBoolEntry("/uniformweight",false)) 
	fprintf(of, "%s\n", "V_GLOBW_UNIFORMWEIGHT=TRUE");
    else
	fprintf(of, "%s\n", "V_GLOBW_UNIFORMWEIGHT=FALSE");
    settings.endGroup();

    // ASTROMPHOTOM
    settings.beginGroup("/ASTROMPHOTOM");
    settings.beginGroup("/sourcecat");
    fprintf(of, "%s%s\n", "V_AP_DETECTTHRESH=", (settings.readEntry("/detectthresh","5.0")).ascii());
    fprintf(of, "%s%s\n", "V_AP_DETECTMINAREA=", (settings.readEntry("/detectminarea","5")).ascii());
    fprintf(of, "%s%s\n", "V_DEBLENDMINCONT=", (settings.readEntry("/deblendmincont","0.0005")).ascii());
    fprintf(of, "%s%s\n", "V_AP_LOWNUM=", (settings.readEntry("/lownum","1")).ascii());
    fprintf(of, "%s%s\n", "V_SEXCATMINFWHM=", (settings.readEntry("/sexcatminfwhm","1.5")).ascii());
    fprintf(of, "%s%s\n", "V_SEXCATFLAG=", (settings.readEntry("/sexcatflag","0")).ascii());
    fprintf(of, "%s%s\n", "V_SEXBACKLEVEL=", (settings.readEntry("/sexbacklevel","")).ascii());
    if (settings.readBoolEntry("/filter", false))
      fprintf(of, "%s\n", "V_AP_FILTER=Y");
    else fprintf(of, "%s\n", "V_AP_FILTER=N");
    settings.endGroup();
    settings.beginGroup("/scamp");
    fprintf(of, "%s%s\n", "V_SCAMP_MAXPOSANGLE=", (settings.readEntry("/maxposangle","2.0")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_MAXOFFSET=", (settings.readEntry("/maxoffset","2.0")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_MAXSCALE=", (settings.readEntry("/maxscale","1.05")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_SNLOW=", (settings.readEntry("/minsn","5")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_SNHIGH=", (settings.readEntry("/maxsn","20")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_POLYORDER=", (settings.readEntry("/distortorder","3")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_POLYORDER2=", (settings.readEntry("/distortorder2","")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_DISTORTGROUPS=", (settings.readEntry("/distortgroup","")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_DISTORTKEYS=", (settings.readEntry("/distortkeys","")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_FGROUPRADIUS=", (settings.readEntry("/fgroupradius","1.0")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_CROSSIDRADIUS=", (settings.readEntry("/crossidradius","2.0")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_ASTREFWEIGHT=", (settings.readEntry("/astrefweight","1.0")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_ASTRINSTRUKEY=", (settings.readEntry("/astrinstrukey","FILTER")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_PHOTINSTRUKEY=", (settings.readEntry("/photinstrukey","FILTER")).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_STABILITY=", (ac_sc_stabilityComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_SCAMP_MOSAICTYPE=", (ac_sc_mosaictypeComboBox->currentText()).ascii());
    if (ac_sc_FPComboBox->currentItem() == 0)
	fprintf(of, "%s\n", "V_SCAMP_FOCALPLANE=DEFAULT");
    if (ac_sc_FPComboBox->currentItem() == 1)
	fprintf(of, "%s\n", "V_SCAMP_FOCALPLANE=NEW");
    if (ac_sc_FPComboBox->currentItem() == 2)
	fprintf(of, "%s\n", "V_SCAMP_FOCALPLANE=NONE");
    // NOT USED, but maybe some other day when I better understand how scamp works
    //    tmp1 = settings.readBoolEntry("/useheader", false);
    // if (tmp1) fprintf(of, "%s\n", "V_SCAMP_USEHEADER=Y");
    // else fprintf(of, "%s\n", "V_SCAMP_USEHEADER=N");
    tmp1 = settings.readBoolEntry("/matchflipped", false);
    if (tmp1) fprintf(of, "%s\n", "V_SCAMP_MATCHFLIPPED=Y");
    else fprintf(of, "%s\n", "V_SCAMP_MATCHFLIPPED=N");
    tmp1 = settings.readBoolEntry("/matchWCS", true);
    if (tmp1) fprintf(of, "%s\n", "V_SCAMP_MATCHWCS=Y");
    else fprintf(of, "%s\n", "V_SCAMP_MATCHWCS=N");
    settings.endGroup();
    settings.beginGroup("/anet");
    fprintf(of, "%s%s\n", "V_ANET_MAXSCALE=", (settings.readEntry("/maxscale","1.05")).ascii());
    fprintf(of, "%s%s\n", "V_ANET_POLYORDER=", (settings.readEntry("/distortorder","")).ascii());
    tmp1 = settings.readBoolEntry("/scampdistort", false);
    if (tmp1) fprintf(of, "%s\n", "V_ANET_SCAMPDISTORT=Y");
    else fprintf(of, "%s\n", "V_ANET_SCAMPDISTORT=N");
    settings.endGroup();
    settings.beginGroup("/absphotom");
    fprintf(of, "%s%s\n", "V_ABSPHOT_STDCAT_DIRECT=",  (absphot_direct_stdcatComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_STDCAT_INDIRECT=",  (absphot_indirect_stdcatComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_APERTURE=", (settings.readEntry("/aperture","10")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_FILTER=", (settings.readEntry("/obsfilter","B")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_STDFILTER=", (absphot_stdfilterComboBox->currentText()).ascii());
    QString stdcolor = absphot_stdcolorComboBox->currentText();
    stdcolor.replace("-","m");
    fprintf(of, "%s%s\n", "V_ABSPHOT_STDCOLOR=", stdcolor.ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_COLORFIXED=", (settings.readEntry("/colorfixed","0.0")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_EXTINCTION=", (settings.readEntry("/extinction","-0.1")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_ZPMIN=", (settings.readEntry("/zpmin")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_ZPMAX=", (settings.readEntry("/zpmax")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_KAPPA=", (settings.readEntry("/kappa","2.0")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_THRESHOLD=", (settings.readEntry("/threshold","0.15")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_MAXITER=", (settings.readEntry("/maxiter","10")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_CONVERGENCE=", (settings.readEntry("/convergence","0.01")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_STDMINMAG=", (settings.readEntry("/stdminmag","0.0")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_MAXPHOTERR=", (settings.readEntry("/maxphoterror","0.05")).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_STDFILTERM2=", (absphot_stdfilterM2_ComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_CALMODE=", (absphot_calmodeComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_ABSPHOT_WCSERR=", (settings.readEntry("/wcserror","10")).ascii());
    if (absphot_fittingmethodM2_ComboBox->currentItem() == 0)
      fprintf(of, "%s\n", "V_ABSPHOT_FITTINGMETHODM2=CHIP");
    else
      fprintf(of, "%s\n", "V_ABSPHOT_FITTINGMETHODM2=MOSAIC");

    if (settings.readBoolEntry("/wcserrorCheckBox", false))
      fprintf(of, "%s\n", "V_ABSPHOT_WCSERRCHECKBOX=Y");
    else fprintf(of, "%s\n", "V_ABSPHOT_WCSERRCHECKBOX=N");

    settings.endGroup();

    settings.beginGroup("/absphotom_nightselection");
    // Loop over the max 10 nights and select those which have valid choices
    QString photcalstring = "";
    QString night, choice;
    if (NIGHT_1_ComboBox->currentItem() != 0 && !NIGHT_1_ComboBox->isHidden()) {
      night = NIGHT_1_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_1_ComboBox->currentItem());
      photcalstring.append(night+"_"+choice);
    }
    if (NIGHT_2_ComboBox->currentItem() != 0 && !NIGHT_2_ComboBox->isHidden()) {
      night = NIGHT_2_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_2_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    if (NIGHT_3_ComboBox->currentItem() != 0 && !NIGHT_3_ComboBox->isHidden()) {
      night = NIGHT_3_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_3_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    if (NIGHT_4_ComboBox->currentItem() != 0 && !NIGHT_4_ComboBox->isHidden()) {
      night = NIGHT_4_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_4_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    if (NIGHT_5_ComboBox->currentItem() != 0 && !NIGHT_5_ComboBox->isHidden()) {
      night = NIGHT_5_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_5_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    if (NIGHT_6_ComboBox->currentItem() != 0 && !NIGHT_6_ComboBox->isHidden()) {
      night = NIGHT_6_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_6_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    if (NIGHT_7_ComboBox->currentItem() != 0 && !NIGHT_7_ComboBox->isHidden()) {
      night = NIGHT_7_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_7_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    if (NIGHT_8_ComboBox->currentItem() != 0 && !NIGHT_8_ComboBox->isHidden()) {
      night = NIGHT_8_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_8_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    if (NIGHT_9_ComboBox->currentItem() != 0 && !NIGHT_9_ComboBox->isHidden()) {
      night = NIGHT_9_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_9_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    if (NIGHT_10_ComboBox->currentItem() != 0 && !NIGHT_10_ComboBox->isHidden()) {
      night = NIGHT_10_PushButton->text().section('_', 1, 1);
      choice = choice.setNum(NIGHT_10_ComboBox->currentItem());
      photcalstring.append("_"+night+"_"+choice);
    }
    fprintf(of, "%s%s\n", "V_ABSPHOT_NIGHTSELECTION=", photcalstring.ascii());

    settings.endGroup();
    settings.endGroup();

    // COADDITION
    QString v_coadd_filter = "";
    if (coc_filterComboBox->count() > 0)
      v_coadd_filter = coc_filterComboBox->currentText();
    settings.beginGroup("/COADDITION");
    settings.beginGroup("/coaddition");
    fprintf(of, "%s%s\n", "V_COADD_REFRA=",  (settings.readEntry("/ra","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_REFDEC=", (settings.readEntry("/dec","")).ascii());
    // QString v_coadd_id;
    // get_coadd_id(v_coadd_id);
    fprintf(of, "%s%s\n", "V_COADD_IDENT=",  (settings.readEntry("/scriptid","default")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_SEEING=", (settings.readEntry("/seeing","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_RZP=",    (settings.readEntry("/rzp","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_PIXSCALE=", (settings.readEntry("/pixscale","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_SIZEX=", (settings.readEntry("/coaddsizex","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_SIZEY=", (settings.readEntry("/coaddsizey","")).ascii());
    if (settings.readEntry("/skyposangle","").isEmpty())
	fprintf(of, "%s\n", "V_COADD_SKYPOSANGLE=");
    else {
     	float skypa = -1.0 * (settings.readEntry("/skyposangle","")).toFloat();
	//	float skypa = (settings.readEntry("/skyposangle","")).toFloat();
	fprintf(of, "%s%f\n", "V_COADD_SKYPOSANGLE=", skypa);
    }

    // evaluate the proper motion
    
    speed_ind = coc_speedunitComboBox->currentItem();
    if (speed_ind == 0) speedcorrection = -60.;
    if (speed_ind == 1) speedcorrection = -1.;
    if (speed_ind == 2) speedcorrection = -1/60.;
    if (speed_ind == 3) speedcorrection = -1/1440.;
    test_ra = settings.readEntry("/propermotionra","");
    test_dec = settings.readEntry("/propermotiondec","");

    if (!test_ra.isEmpty() && !test_dec.isEmpty()) {
	propmotionra  = speedcorrection * settings.readEntry("/propermotionra","").toFloat();
	propmotiondec = speedcorrection * settings.readEntry("/propermotiondec","").toFloat();
	fprintf(of, "%s%lf\n", "V_COADD_PROPMOTIONRA=", propmotionra);
	fprintf(of, "%s%lf\n", "V_COADD_PROPMOTIONDEC=", propmotiondec);
    }
    else {
	fprintf(of, "%s\n", "V_COADD_PROPMOTIONRA=");
	fprintf(of, "%s\n", "V_COADD_PROPMOTIONDEC=");
    }

    fprintf(of, "%s%s%s\n", "V_COADD_CHIPS=\"",       (settings.readEntry("/chips","")).ascii(), "\"");
    fprintf(of, "%s%s\n", "V_COADD_FILTERTHRESHOLD=", (settings.readEntry("/filterthreshold","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_FILTERCLUSTERSIZE=", (settings.readEntry("/filterclustersize","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_FILTERBORDERWIDTH=", (settings.readEntry("/filterborderwidth","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_SMOOTHEDGE=",    (settings.readEntry("/smoothedge","")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_KERNEL=",        (coc_kernelComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_COADD_FILTER=",        v_coadd_filter.ascii());
    fprintf(of, "%s%s\n", "V_COADD_PROJECTION=",    (coc_projectionComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_COADD_CELESTIALTYPE=", (coc_celestialtypeComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_COADD_COMBINETYPE=",   (coc_combineComboBox->currentText()).ascii());
    fprintf(of, "%s%s\n", "V_COADD_CLIPAMPFRAC=",   (settings.readEntry("/clipampfrac","0.3")).ascii());
    fprintf(of, "%s%s\n", "V_COADD_CLIPSIGMA=",     (settings.readEntry("/clipsigma","4")).ascii());
    if (settings.readBoolEntry("/rescaleweights", false))
      fprintf(of, "%s\n", "V_COADD_RESCALEWEIGHTS=Y");
    else fprintf(of, "%s\n", "V_COADD_RESCALEWEIGHTS=N");

    settings.endGroup();

    settings.endGroup();

    fclose(of);

    execstr = "mv ";
    execstr.append(paramsinifiletmp);
    execstr.append(" ");
    execstr.append(paramsinifile);

    my_system(execstr);
}


void parameters::get_coadd_id(QString &result)
{
  int numfilters = coc_filterComboBox->count();

  QString coadd_id = coc_identLineEdit->text();
  QString coadd_filter = coc_filterComboBox->currentText();

  if (numfilters == 1) {
    if (coadd_id.isEmpty())  result = coadd_filter;
    if (!coadd_id.isEmpty()) result = coadd_id;
  }

  if (numfilters > 1) {
    if (coadd_id.isEmpty())  result = coadd_filter;
    if (!coadd_id.isEmpty()) result = coadd_id + "_" + coadd_filter;
  }
}


void parameters::defaults()
{
  //    page = param_WidgetStack->id(this);
  
  if (page == 0) {
    pre_renameCheckBox->setChecked(false);
    pre_rename_fitskeyLineEdit->setText("ARCFILE");
    pre_sort_fitskeyLineEdit->setText("OBJECT");
    pre_sort_biasLineEdit->setText("");
    pre_sort_darkLineEdit->setText("");
    pre_sort_domeflatLineEdit->setText("");
    pre_sort_skyflatLineEdit->setText("");
    pre_sort_stdLineEdit->setText("");
    xtalk_nor_amplitudeLineEdit->clear();
    xtalk_row_amplitudeLineEdit->clear();
    xtalk_norCheckBox->setChecked(false);
    xtalk_rowCheckBox->setChecked(false);
    xtalk_multiCheckBox->setChecked(false);
    xtalk_multi_nsectionLineEdit->clear();
    untoggle_xtalkbuttons();
  }
  
  if (page == 1) {
    ovscan_nlowLineEdit->setText("0");
    ovscan_nhighLineEdit->setText("1");
    bias_nlowLineEdit->setText("0");
    bias_nhighLineEdit->setText("1");
    dark_nlowLineEdit->setText("0");
    dark_nhighLineEdit->setText("1");
    flat_nlowLineEdit->setText("0");
    flat_nhighLineEdit->setText("1");
  }
  
  if (page == 2) {
    sf_back_detectthreshLineEdit->setText("1.3");
    sf_back_detectminareaLineEdit->setText("5");
    sf_back_maskexpandfactorLineEdit->setText("");
    sf_back_combinemethodComboBox->setCurrentItem(0);
    sf_back_sexfilterCheckBox->setChecked(true);
    sf_back_nlow1LineEdit->setText("0");
    sf_back_nhigh1LineEdit->setText("0");
    sf_back_nlow2LineEdit->setText("0");
    sf_back_nhigh2LineEdit->setText("1");
    sf_back_maglimitLineEdit->setText("");
    sf_back_distanceLineEdit->setText("");
    sf_back_serverComboBox->setCurrentItem(0);
    sf_back_illumsmoothLineEdit->setText("");
    sf_back_fringesmoothLineEdit->setText("");
    sf_back_adjustgainsCheckBox->setChecked(false);
    sf_back_fringescaleCheckBox->setChecked(true);
    sf_back_twopassCheckBox->setChecked(true);
    sf_back_applymodeComboBox->setCurrentItem(0);
    sf_back_windowsizeLineEdit->setText("0");
    sf_back_gapsizeLineEdit->setText("");
  }
  
  if (page == 3) {
    ccc_colldetectthreshLineEdit->setText("");
    ccc_colldetectminareaLineEdit->setText("");
    ccc_collmaskexpandfactorLineEdit->setText("");
    ccc_collrejectthreshLineEdit->setText("1.5");
    ccc_colldirComboBox->setCurrentItem(0);
    ccc_collautothresholdCheckBox->setChecked(false);
    ccc_collxminLineEdit->clear();
    ccc_collxmaxLineEdit->clear();
    ccc_collyminLineEdit->clear();
    ccc_collymaxLineEdit->clear();
    adjust_collcorr_direction();
  }

  if (page == 4) {
    w_binminLineEdit->setText("-100");
    w_binmaxLineEdit->setText("500");
    w_binsizeSpinBox->setValue(4);
    globw_flatminLineEdit->setText("0.5");
    globw_flatmaxLineEdit->setText("1.6");
    globw_darkminLineEdit->clear();
    globw_darkmaxLineEdit->clear();
    globw_usebiasCheckBox->setChecked(false);
    globw_uniformweightCheckBox->setChecked(false);
    defect_kernelsizeLineEdit->clear();
    defect_rowtolLineEdit->clear();
    defect_coltolLineEdit->clear();
    defect_clustolLineEdit->clear();
    defect_kernelsize_sfLineEdit->clear();
    defect_rowtol_sfLineEdit->clear();
    defect_coltol_sfLineEdit->clear();
    defect_clustol_sfLineEdit->clear();
    toggle_globalweight_params();
  }
  
  if (page == 5) {
    weight_threshminLineEdit->clear();
    weight_threshmaxLineEdit->clear();
    weight_cosmicLineEdit->setText("0.1");
    weight_cosmicDTLineEdit->setText("6");
    weight_cosmicDMINLineEdit->setText("1");
    weight_bloomCheckBox->setChecked(false);
    weight_bloomlowlimitLineEdit->setText("20000");
    weight_bloomminareaLineEdit->setText("100");
    weight_bloomwidthLineEdit->setText("0");
  }
  
  if (page == 6) {
    absphot_apertureLineEdit->setText("10");
    absphot_filterLineEdit->clear();
    absphot_colorfixedLineEdit->clear();
    absphot_extinctionLineEdit->clear();
    absphot_zpminLineEdit->clear();
    absphot_zpmaxLineEdit->clear();
    absphot_kappaLineEdit->setText("2.0");
    absphot_thresholdLineEdit->setText("0.15");
    absphot_maxiterLineEdit->setText("10");
    absphot_convergenceLineEdit->setText("0.01");
    absphot_stdminmagLineEdit->clear();
    absphot_wcserrorLineEdit->setText("10");
    absphot_wcserrorCheckBox->setChecked(true);
    absphot_calmodeComboBox->setCurrentItem(0);
  }
  
  if (page == 7) {
    absphot_maxphoterrorLineEdit->setText("0.05");
  }
  
  if (page == 8) {
    ap_detectthreshLineEdit->setText("5.0");
    ap_detectminareaLineEdit->setText("5");
    ap_deblendLineEdit->setText("0.0005");
    ap_lownumLineEdit->setText("");
    ap_minfwhmLineEdit->setText("1.5");
    ap_flagLineEdit->setText("0");
    ap_saturationLineEdit->setText("");
    ap_sexbacklevelLineEdit->setText("");
    ap_filterCheckBox->setChecked(false);
  }
  
  if (page == 9) {
    ac_sc_maxposangleLineEdit->setText("2.0");
    ac_sc_maxoffsetLineEdit->setText("2.0");
    ac_sc_maxscaleLineEdit->setText("1.05");
    ac_sc_distortorderLineEdit->setText("3");
    ac_sc_distortorder2LineEdit->setText("");
    ac_sc_distortgroupsLineEdit->setText("");
    ac_sc_minSNLineEdit->setText("5");
    ac_sc_maxSNLineEdit->setText("20");
    ac_sc_fgroupradiusLineEdit->setText("1.0");
    ac_sc_astrefweightLineEdit->setText("1.0");
    ac_sc_astrinstrukeyLineEdit->setText("FILTER");
    ac_sc_photinstrukeyLineEdit->setText("FILTER");
    ac_sc_stabilityComboBox->setCurrentItem(0);
    ac_sc_mosaictypeComboBox->setCurrentItem(0);
    ac_sc_FPComboBox->setCurrentItem(0);
    ac_sc_matchflippedCheckBox->setChecked(false);
    ac_sc_matchWCSCheckBox->setChecked(true);
    ac_an_distortorderLineEdit->setText("");
    ac_an_maxscaleLineEdit->setText("1.05");
    ac_an_scampdistortCheckBox->setChecked(false);
    NIGHT_1_ComboBox->setCurrentItem(0);
    NIGHT_2_ComboBox->setCurrentItem(0);
    NIGHT_3_ComboBox->setCurrentItem(0);
    NIGHT_4_ComboBox->setCurrentItem(0);
    NIGHT_5_ComboBox->setCurrentItem(0);
    NIGHT_6_ComboBox->setCurrentItem(0);
    NIGHT_7_ComboBox->setCurrentItem(0);
    NIGHT_8_ComboBox->setCurrentItem(0);
    NIGHT_9_ComboBox->setCurrentItem(0);
    NIGHT_10_ComboBox->setCurrentItem(0);
    update_crossidradius();
  }
  
  if (page == 10) {
    coc_refraLineEdit->clear();
    coc_refdecLineEdit->clear();
    get_pixscale();
    coc_identLineEdit->setText("");
    coc_skyposangleLineEdit->clear();
    coc_chipsLineEdit->clear();
    coc_seeingLineEdit->clear();
    coc_RZPLineEdit->clear();
    coc_filterComboBox->setCurrentItem(0);
    coc_filterthresholdLineEdit->clear();
    coc_filterclustersizeLineEdit->clear();
    coc_filterborderwidthLineEdit->clear();
    coc_smoothedgeLineEdit->clear();
    coc_propmotionraLineEdit->clear();
    coc_propmotiondecLineEdit->clear();
    coc_coaddsizexLineEdit->clear();
    coc_coaddsizeyLineEdit->clear();
    coc_speedunitComboBox->setCurrentItem(1);
    coc_kernelComboBox->setCurrentItem(3);
    coc_projectionComboBox->setCurrentItem(1);
    coc_celestialtypeComboBox->setCurrentItem(0);
    coc_combineComboBox->setCurrentItem(0);
    coc_clipampfracLineEdit->setText("0.3");
    coc_clipsigmaLineEdit->setText("4.0");
    coc_rescaleweightsCheckBox->setChecked(false);
  }
}


void parameters::defaults_all()
{
  pre_renameCheckBox->setChecked(false);
  pre_rename_fitskeyLineEdit->setText("ARCFILE");
  pre_sort_fitskeyLineEdit->setText("OBJECT");
  pre_sort_biasLineEdit->setText("");
  pre_sort_darkLineEdit->setText("");
  pre_sort_domeflatLineEdit->setText("");
  pre_sort_skyflatLineEdit->setText("");
  pre_sort_stdLineEdit->setText("");
  xtalk_nor_amplitudeLineEdit->clear();
  xtalk_row_amplitudeLineEdit->clear();
  xtalk_norCheckBox->setChecked(false);
  xtalk_rowCheckBox->setChecked(false);
  xtalk_multiCheckBox->setChecked(false);
  xtalk_multi_nsectionLineEdit->clear();
  untoggle_xtalkbuttons();
  
  ovscan_nlowLineEdit->setText("0");
  ovscan_nhighLineEdit->setText("1");
  bias_nlowLineEdit->setText("0");
  bias_nhighLineEdit->setText("1");
  dark_nlowLineEdit->setText("0");
  dark_nhighLineEdit->setText("1");
  flat_nlowLineEdit->setText("0");
  flat_nhighLineEdit->setText("1");
  
  sf_back_detectthreshLineEdit->setText("1.3");
  sf_back_detectminareaLineEdit->setText("5");
  sf_back_maskexpandfactorLineEdit->setText("");
  sf_back_combinemethodComboBox->setCurrentItem(0);
  sf_back_sexfilterCheckBox->setChecked(true);
  sf_back_nlow1LineEdit->setText("0");
  sf_back_nhigh1LineEdit->setText("0");
  sf_back_nlow2LineEdit->setText("0");
  sf_back_nhigh2LineEdit->setText("1");
  sf_back_maglimitLineEdit->setText("");
  sf_back_distanceLineEdit->setText("");
  sf_back_serverComboBox->setCurrentItem(0);
  sf_back_illumsmoothLineEdit->setText("");
  sf_back_fringesmoothLineEdit->setText("");
  sf_back_adjustgainsCheckBox->setChecked(false);
  sf_back_fringescaleCheckBox->setChecked(true);
  sf_back_twopassCheckBox->setChecked(true);
  sf_back_applymodeComboBox->setCurrentItem(0);
  sf_back_windowsizeLineEdit->setText("0");
  sf_back_gapsizeLineEdit->setText("");

  ccc_colldetectthreshLineEdit->setText("");
  ccc_colldetectminareaLineEdit->setText("");
  ccc_collmaskexpandfactorLineEdit->setText("");
  ccc_collrejectthreshLineEdit->setText("1.5");
  ccc_colldirComboBox->setCurrentItem(0);
  ccc_collautothresholdCheckBox->setChecked(false);
  ccc_collxminLineEdit->clear();
  ccc_collxmaxLineEdit->clear();
  ccc_collyminLineEdit->clear();
  ccc_collymaxLineEdit->clear();
  adjust_collcorr_direction();

  w_binminLineEdit->setText("-100");
  w_binmaxLineEdit->setText("500");
  w_binsizeSpinBox->setValue(4);
  globw_flatminLineEdit->setText("0.5");
  globw_flatmaxLineEdit->setText("1.6");
  globw_darkminLineEdit->clear();
  globw_darkmaxLineEdit->clear();
  globw_usebiasCheckBox->setChecked(false);
  globw_uniformweightCheckBox->setChecked(false);
  defect_kernelsizeLineEdit->clear();
  defect_rowtolLineEdit->clear();
  defect_coltolLineEdit->clear();
  defect_clustolLineEdit->clear();
  defect_kernelsize_sfLineEdit->clear();
  defect_rowtol_sfLineEdit->clear();
  defect_coltol_sfLineEdit->clear();
  defect_clustol_sfLineEdit->clear();
  toggle_globalweight_params();
  
  weight_threshminLineEdit->clear();
  weight_threshmaxLineEdit->clear();
  weight_cosmicLineEdit->setText("0.1");
  weight_cosmicDTLineEdit->setText("6");
  weight_cosmicDMINLineEdit->setText("1");
  weight_bloomCheckBox->setChecked(false);
  weight_bloomlowlimitLineEdit->setText("20000");
  weight_bloomminareaLineEdit->setText("100");
  weight_bloomwidthLineEdit->setText("0");
  
  absphot_apertureLineEdit->setText("10");
  absphot_filterLineEdit->clear();
  absphot_colorfixedLineEdit->clear();
  absphot_extinctionLineEdit->clear();
  absphot_zpminLineEdit->clear();
  absphot_zpmaxLineEdit->clear();
  absphot_kappaLineEdit->setText("2.0");
  absphot_thresholdLineEdit->setText("0.15");
  absphot_maxiterLineEdit->setText("10");
  absphot_convergenceLineEdit->setText("0.01");
  absphot_stdminmagLineEdit->clear();
  absphot_wcserrorLineEdit->setText("10");
  absphot_wcserrorCheckBox->setChecked(true);

  absphot_maxphoterrorLineEdit->setText("0.05");
  
  ap_detectthreshLineEdit->setText("5.0");
  ap_detectminareaLineEdit->setText("5");
  ap_deblendLineEdit->setText("0.0005");
  ap_lownumLineEdit->setText("");
  ap_minfwhmLineEdit->setText("1.5");
  ap_flagLineEdit->setText("0");
  ap_saturationLineEdit->setText("");
  ap_sexbacklevelLineEdit->setText("");
  ap_filterCheckBox->setChecked(false);
  absphot_calmodeComboBox->setCurrentItem(0);

  ac_sc_maxposangleLineEdit->setText("2.0");
  ac_sc_maxoffsetLineEdit->setText("2.0");
  ac_sc_maxscaleLineEdit->setText("1.05");
  ac_sc_distortorderLineEdit->setText("3");
  ac_sc_distortorder2LineEdit->setText("");
  ac_sc_distortgroupsLineEdit->setText("");
  ac_sc_minSNLineEdit->setText("5");
  ac_sc_maxSNLineEdit->setText("20");
  ac_sc_fgroupradiusLineEdit->setText("1.0");
  ac_sc_astrefweightLineEdit->setText("1.0");
  ac_sc_astrinstrukeyLineEdit->setText("FILTER");
  ac_sc_photinstrukeyLineEdit->setText("FILTER");
  ac_sc_stabilityComboBox->setCurrentItem(0);
  ac_sc_mosaictypeComboBox->setCurrentItem(0);
  ac_sc_FPComboBox->setCurrentItem(0);
  ac_sc_matchflippedCheckBox->setChecked(false);
  ac_sc_matchWCSCheckBox->setChecked(true);
  ac_an_distortorderLineEdit->setText("");
  ac_an_maxscaleLineEdit->setText("1.05");
  ac_an_scampdistortCheckBox->setChecked(false);
  NIGHT_1_ComboBox->setCurrentItem(0);
  NIGHT_2_ComboBox->setCurrentItem(0);
  NIGHT_3_ComboBox->setCurrentItem(0);
  NIGHT_4_ComboBox->setCurrentItem(0);
  NIGHT_5_ComboBox->setCurrentItem(0);
  NIGHT_6_ComboBox->setCurrentItem(0);
  NIGHT_7_ComboBox->setCurrentItem(0);
  NIGHT_8_ComboBox->setCurrentItem(0);
  NIGHT_9_ComboBox->setCurrentItem(0);
  NIGHT_10_ComboBox->setCurrentItem(0);
  update_crossidradius();
  
  coc_refraLineEdit->clear();
  coc_refdecLineEdit->clear();
  get_pixscale();
  coc_identLineEdit->setText("");
  coc_skyposangleLineEdit->clear();
  coc_chipsLineEdit->clear();
  coc_seeingLineEdit->clear();
  coc_RZPLineEdit->clear();
  coc_filterComboBox->setCurrentItem(0);
  coc_filterthresholdLineEdit->clear();
  coc_filterclustersizeLineEdit->clear();
  coc_filterborderwidthLineEdit->clear();
  coc_smoothedgeLineEdit->clear();
  coc_propmotionraLineEdit->clear();
  coc_propmotiondecLineEdit->clear();
  coc_coaddsizexLineEdit->clear();
  coc_coaddsizeyLineEdit->clear();
  coc_speedunitComboBox->setCurrentItem(1);
  coc_kernelComboBox->setCurrentItem(3);
  coc_projectionComboBox->setCurrentItem(1);
  coc_celestialtypeComboBox->setCurrentItem(0);
  coc_combineComboBox->setCurrentItem(0);
  coc_clipampfracLineEdit->setText("0.3");
  coc_clipsigmaLineEdit->setText("4.0");
  coc_rescaleweightsCheckBox->setChecked(false);
}


void parameters::populate_xtalkbuttons()
{
  QString path;
  
  path = scriptdir + "../images/";
  xtalk_nor_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_nor_2x2p.png"));
  xtalk_nor_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_nor_1x2p.png"));
  xtalk_nor_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_nor_2x1p.png"));
  xtalk_row_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_row_2x2p.png"));
  xtalk_row_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_row_1x2p.png"));
  xtalk_row_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_row_2x1p.png"));
  xtalk_col_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_col_2x2p.png"));
  xtalk_col_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_col_1x2p.png"));
  xtalk_col_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_col_2x1p.png"));
  xtalk_multixToolButton->setIconSet(QPixmap(path+"xtalk_multi_x_p.png"));
  xtalk_multiyToolButton->setIconSet(QPixmap(path+"xtalk_multi_y_p.png"));
  
  xtalk_nor_2x2ToolButton->setUsesBigPixmap(true);
  xtalk_nor_1x2ToolButton->setUsesBigPixmap(true);
  xtalk_nor_2x1ToolButton->setUsesBigPixmap(true);
  xtalk_row_2x2ToolButton->setUsesBigPixmap(true);
  xtalk_row_1x2ToolButton->setUsesBigPixmap(true);
  xtalk_row_2x1ToolButton->setUsesBigPixmap(true);
  xtalk_col_2x2ToolButton->setUsesBigPixmap(true);
  xtalk_col_1x2ToolButton->setUsesBigPixmap(true);
  xtalk_col_2x1ToolButton->setUsesBigPixmap(true);
  xtalk_multixToolButton->setUsesBigPixmap(true);
  xtalk_multiyToolButton->setUsesBigPixmap(true);
}


void parameters::untoggle_xtalkbuttons()
{
  xtalk_nor_2x2ToolButton->setOn(false);
  xtalk_nor_1x2ToolButton->setOn(false);
  xtalk_nor_2x1ToolButton->setOn(false);
  xtalk_row_2x2ToolButton->setOn(false);
  xtalk_row_1x2ToolButton->setOn(false);
  xtalk_row_2x1ToolButton->setOn(false);
  xtalk_col_2x2ToolButton->setOn(false);
  xtalk_col_1x2ToolButton->setOn(false);
  xtalk_col_2x1ToolButton->setOn(false);
  xtalk_multixToolButton->setOn(false);
  xtalk_multiyToolButton->setOn(false);
  active_passive_xtalk();
}


void parameters::link_checkbox_xtalk()
{
  if (xtalk_norCheckBox->isChecked() &&
      xtalk_norButtonGroup->selected() == 0) {
    xtalk_nor_2x2ToolButton->setOn(true);
  }
  if (xtalk_rowCheckBox->isChecked() &&
      xtalk_rowButtonGroup->selected() == 0) {
    xtalk_row_2x2ToolButton->setOn(true);
  }
  if (xtalk_multiCheckBox->isChecked() &&
      xtalk_multiButtonGroup->selected() == 0) {
    xtalk_multixToolButton->setOn(true);
  }
  if (!xtalk_norCheckBox->isChecked()) {
    xtalk_nor_2x2ToolButton->setOn(false);
    xtalk_nor_1x2ToolButton->setOn(false);
    xtalk_nor_2x1ToolButton->setOn(false);
  }
  if (!xtalk_rowCheckBox->isChecked()) {
    xtalk_row_2x2ToolButton->setOn(false);
    xtalk_row_1x2ToolButton->setOn(false);
    xtalk_row_2x1ToolButton->setOn(false);
    xtalk_col_2x2ToolButton->setOn(false);
    xtalk_col_1x2ToolButton->setOn(false);
    xtalk_col_2x1ToolButton->setOn(false);
  }
  if (!xtalk_multiCheckBox->isChecked()) {
    xtalk_multixToolButton->setOn(false);
    xtalk_multiyToolButton->setOn(false);
  }
  
  active_passive_xtalk();
}


void parameters::link_xtalk_checkbox()
{
  if (xtalk_norButtonGroup->selected() != 0 &&
      !xtalk_norCheckBox->isChecked()) {
    xtalk_norCheckBox->setChecked(true);
  }
  if (xtalk_rowButtonGroup->selected() != 0 &&
      !xtalk_rowCheckBox->isChecked()) {
    xtalk_rowCheckBox->setChecked(true);
  }
  if (xtalk_multiButtonGroup->selected() != 0 &&
      !xtalk_multiCheckBox->isChecked()) {
    xtalk_multiCheckBox->setChecked(true);
  }
  
  active_passive_xtalk();
}


void parameters::active_passive_xtalk()
{
  QString path;
  path = scriptdir + "../images/";
  
  if (xtalk_nor_2x2ToolButton->isOn())
    xtalk_nor_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_nor_2x2a.png"));
  else xtalk_nor_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_nor_2x2p.png"));
  if (xtalk_nor_1x2ToolButton->isOn())
    xtalk_nor_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_nor_1x2a.png"));
  else xtalk_nor_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_nor_1x2p.png"));
  if (xtalk_nor_2x1ToolButton->isOn())
    xtalk_nor_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_nor_2x1a.png"));
  else xtalk_nor_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_nor_2x1p.png"));
  if (xtalk_row_2x2ToolButton->isOn())
    xtalk_row_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_row_2x2a.png"));
  else xtalk_row_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_row_2x2p.png"));
  if (xtalk_row_1x2ToolButton->isOn())
    xtalk_row_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_row_1x2a.png"));
  else xtalk_row_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_row_1x2p.png"));
  if (xtalk_row_2x1ToolButton->isOn())
    xtalk_row_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_row_2x1a.png"));
  else xtalk_row_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_row_2x1p.png"));
  if (xtalk_col_2x2ToolButton->isOn())
    xtalk_col_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_col_2x2a.png"));
  else xtalk_col_2x2ToolButton->setIconSet(QPixmap(path+"xtalk_col_2x2p.png"));
  if (xtalk_col_1x2ToolButton->isOn())
    xtalk_col_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_col_1x2a.png"));
  else xtalk_col_1x2ToolButton->setIconSet(QPixmap(path+"xtalk_col_1x2p.png"));
  if (xtalk_col_2x1ToolButton->isOn())
    xtalk_col_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_col_2x1a.png"));
  else xtalk_col_2x1ToolButton->setIconSet(QPixmap(path+"xtalk_col_2x1p.png"));
  if (xtalk_multixToolButton->isOn())
    xtalk_multixToolButton->setIconSet(QPixmap(path+"xtalk_multi_x_a.png"));
  else xtalk_multixToolButton->setIconSet(QPixmap(path+"xtalk_multi_x_p.png"));
  if (xtalk_multiyToolButton->isOn())
    xtalk_multiyToolButton->setIconSet(QPixmap(path+"xtalk_multi_y_a.png"));
  else xtalk_multiyToolButton->setIconSet(QPixmap(path+"xtalk_multi_y_p.png"));
}


void parameters::validate()
{
  QRegExp rismall( "^[1-9]" );
  QRegExp ri( "^[0-9]+" );
  QRegExp ricomma( "^[0-9,]+" );
  QRegExp rni( "^[-][0-9]+" );
  QRegExp rx( "^\\S+$" );
  QRegExp rx1( "^[A-Z0-9a-z-+._,]+$" );
  QRegExp rx2( "^[-]{0,1}[0-9.:]+" );
  QRegExp rx3( "^[A-Z0-9a-z-+._,:]+$" );
  QRegExp rf( "^[-]{0,1}[0-9]*[.]{0,1}[0-9]+" );
  QRegExp rf2( "^[0-9]*[.]{0,1}[0-9]+" );
  QRegExp rint( "^[0-9]+[0-9\\s]+$" );
  
  QValidator* validator_intblank = new QRegExpValidator( rint, this );
  QValidator* validator_intsmall = new QRegExpValidator( rismall, this );
  QValidator* validator_int = new QRegExpValidator( ri, this );
  QValidator* validator_intcomma = new QRegExpValidator( ricomma, this );
  QValidator* validator_intn = new QRegExpValidator( rni, this );
  QValidator* validator_string = new QRegExpValidator( rx, this );
  QValidator* validator_string1 = new QRegExpValidator( rx1, this );
  QValidator* validator_string2 = new QRegExpValidator( rx2, this );
  QValidator* validator_string3 = new QRegExpValidator( rx3, this );
  QValidator* validator_float = new QRegExpValidator(rf, this );
  QValidator* validator_float2 = new QRegExpValidator(rf2, this );
  
  // validate section preparation
  pre_rename_fitskeyLineEdit->setValidator( validator_string );
  pre_sort_fitskeyLineEdit->setValidator( validator_string );
  pre_sort_biasLineEdit->setValidator( validator_string );
  pre_sort_darkLineEdit->setValidator( validator_string );
  pre_sort_domeflatLineEdit->setValidator( validator_string );
  pre_sort_skyflatLineEdit->setValidator( validator_string );
  pre_sort_stdLineEdit->setValidator( validator_string );
  
  // validate section xtalk
  xtalk_nor_amplitudeLineEdit->setValidator( validator_float );
  xtalk_row_amplitudeLineEdit->setValidator( validator_float );
  xtalk_multi_nsectionLineEdit->setValidator( validator_int );
  
  // validate section calibration
  ovscan_nlowLineEdit->setValidator( validator_int );
  ovscan_nhighLineEdit->setValidator( validator_int );
  bias_nlowLineEdit->setValidator( validator_int );
  bias_nhighLineEdit->setValidator( validator_int );
  dark_nlowLineEdit->setValidator( validator_int );
  dark_nhighLineEdit->setValidator( validator_int );
  flat_nlowLineEdit->setValidator( validator_int );
  flat_nhighLineEdit->setValidator( validator_int );
  
  // validate section background model
  sf_back_nlow1LineEdit->setValidator( validator_int );
  sf_back_nhigh1LineEdit->setValidator( validator_int );
  sf_back_nlow2LineEdit->setValidator( validator_int );
  sf_back_nhigh2LineEdit->setValidator( validator_int );
  sf_back_maglimitLineEdit->setValidator( validator_float2 );
  sf_back_distanceLineEdit->setValidator( validator_float2 );
  sf_back_detectthreshLineEdit->setValidator( validator_float2 );
  sf_back_detectminareaLineEdit->setValidator( validator_int );
  sf_back_maskexpandfactorLineEdit->setValidator( validator_float2 );
  sf_back_windowsizeLineEdit->setValidator( validator_int );
  sf_back_gapsizeLineEdit->setValidator( validator_float2 );
  sf_back_illumsmoothLineEdit->setValidator( validator_int );
  sf_back_fringesmoothLineEdit->setValidator( validator_int );

  // validate section collapse correction
  ccc_colldetectthreshLineEdit->setValidator( validator_float2 );
  ccc_colldetectminareaLineEdit->setValidator( validator_int );
  ccc_collmaskexpandfactorLineEdit->setValidator( validator_float2 );
  ccc_collrejectthreshLineEdit->setValidator( validator_float2 );
  ccc_collxminLineEdit->setValidator( validator_int );
  ccc_collxmaxLineEdit->setValidator( validator_int );
  ccc_collyminLineEdit->setValidator( validator_int );
  ccc_collymaxLineEdit->setValidator( validator_int );

  // validate section weights
  w_binminLineEdit->setValidator( validator_intn );
  w_binmaxLineEdit->setValidator( validator_int );
  globw_flatminLineEdit->setValidator( validator_float2 );
  globw_flatmaxLineEdit->setValidator( validator_float2 );
  globw_darkminLineEdit->setValidator( validator_float );
  globw_darkmaxLineEdit->setValidator( validator_float );
  defect_kernelsizeLineEdit->setValidator( validator_float2 );
  defect_rowtolLineEdit->setValidator( validator_float2 );
  defect_coltolLineEdit->setValidator( validator_float2 );
  defect_clustolLineEdit->setValidator( validator_float2 );
  defect_kernelsize_sfLineEdit->setValidator( validator_float2 );
  defect_rowtol_sfLineEdit->setValidator( validator_float2 );
  defect_coltol_sfLineEdit->setValidator( validator_float2 );
  defect_clustol_sfLineEdit->setValidator( validator_float2 );
  weight_threshminLineEdit->setValidator( validator_float );
  weight_threshmaxLineEdit->setValidator( validator_float );
  weight_cosmicLineEdit->setValidator( validator_float2 );
  weight_cosmicDTLineEdit->setValidator( validator_int );
  weight_cosmicDMINLineEdit->setValidator( validator_int );
  weight_bloomlowlimitLineEdit->setValidator( validator_float2 );
  weight_bloomminareaLineEdit->setValidator( validator_int );
  weight_bloomwidthLineEdit->setValidator( validator_float2 );
  
  // validate section object detection
  ap_detectthreshLineEdit->setValidator( validator_float2 );
  ap_detectminareaLineEdit->setValidator( validator_int );
  ap_deblendLineEdit->setValidator( validator_float2 );
  ap_lownumLineEdit->setValidator( validator_int );
  ap_minfwhmLineEdit->setValidator( validator_float2 );
  ap_flagLineEdit->setValidator( validator_int );
  ap_saturationLineEdit->setValidator( validator_int );
  ap_sexbacklevelLineEdit->setValidator( validator_float );
  
  // validate section astrometry
  ac_sc_maxposangleLineEdit->setValidator( validator_float2 );
  ac_sc_maxoffsetLineEdit->setValidator( validator_float2 );
  ac_sc_maxscaleLineEdit->setValidator( validator_float2 );
  ac_sc_distortorderLineEdit->setValidator( validator_intsmall );
  ac_sc_distortorder2LineEdit->setValidator( validator_intcomma );
  ac_sc_distortgroupsLineEdit->setValidator( validator_intcomma );
  ac_sc_distortkeysLineEdit->setValidator( validator_string3 );
  ac_sc_minSNLineEdit->setValidator( validator_float2 );
  ac_sc_maxSNLineEdit->setValidator( validator_float2 );
  ac_sc_fgroupradiusLineEdit->setValidator( validator_float2 );
  ac_sc_crossidradiusLineEdit->setValidator( validator_float2 );
  ac_sc_astrefweightLineEdit->setValidator( validator_float2 );
  ac_sc_astrinstrukeyLineEdit->setValidator( validator_string1 );
  ac_sc_photinstrukeyLineEdit->setValidator( validator_string1 );
  ac_an_distortorderLineEdit->setValidator( validator_intsmall );
  ac_an_maxscaleLineEdit->setValidator( validator_float2 );
  
  // validate section absolute photometry
  absphot_apertureLineEdit->setValidator(validator_int);
  absphot_filterLineEdit->setValidator(validator_string1);
  absphot_colorfixedLineEdit->setValidator(validator_string1);
  absphot_extinctionLineEdit->setValidator(validator_float);
  absphot_zpminLineEdit->setValidator(validator_float2);
  absphot_zpmaxLineEdit->setValidator(validator_float2);
  absphot_kappaLineEdit->setValidator(validator_float2);
  absphot_thresholdLineEdit->setValidator(validator_float2);
  absphot_maxiterLineEdit->setValidator(validator_int);
  absphot_convergenceLineEdit->setValidator(validator_float2);
  absphot_stdminmagLineEdit->setValidator(validator_float);
  absphot_maxphoterrorLineEdit->setValidator(validator_float2);
  absphot_wcserrorLineEdit->setValidator(validator_int);
  
  // validate section coaddition
  coc_identLineEdit->setValidator( validator_string1 );
  coc_refraLineEdit->setValidator( validator_string2 );
  coc_refdecLineEdit->setValidator( validator_string2 );
  coc_pixscaleLineEdit->setValidator( validator_float2 );
  coc_skyposangleLineEdit->setValidator( validator_float );
  coc_filterthresholdLineEdit->setValidator( validator_float2 );    
  coc_filterclustersizeLineEdit->setValidator( validator_int );    
  coc_filterborderwidthLineEdit->setValidator( validator_int );    
  coc_smoothedgeLineEdit->setValidator( validator_int );    
  coc_chipsLineEdit->setValidator( validator_intblank );    
  coc_propmotionraLineEdit->setValidator( validator_float );
  coc_propmotiondecLineEdit->setValidator( validator_float );
  coc_coaddsizexLineEdit->setValidator( validator_int );    
  coc_coaddsizeyLineEdit->setValidator( validator_int );    
  coc_seeingLineEdit->setValidator( validator_float2 );
  coc_RZPLineEdit->setValidator( validator_float );
  coc_clipampfracLineEdit->setValidator( validator_float2 );    
  coc_clipsigmaLineEdit->setValidator( validator_float2 );    
}


void parameters::adjust_collcorr_direction()
{
  QString colldir, path;
  
  path = scriptdir + "../images/";
  
  colldir = ccc_colldirComboBox->currentText();
  
  if (colldir.compare("x") == 0)
    ccc_colldirPixmapLabel->setPixmap(QPixmap(path+"collapse_x.png"));

  if (colldir.compare("y") == 0)
    ccc_colldirPixmapLabel->setPixmap(QPixmap(path+"collapse_y.png"));

  if (colldir.compare("xy") == 0)
    ccc_colldirPixmapLabel->setPixmap(QPixmap(path+"collapse_xy.png"));

  if (colldir.compare("xyyx") == 0)
    ccc_colldirPixmapLabel->setPixmap(QPixmap(path+"collapse_xyyx.png"));

  if (colldir.compare("yxxy") == 0)
    ccc_colldirPixmapLabel->setPixmap(QPixmap(path+"collapse_yxxy.png"));

  if (colldir.compare("xxxx") == 0)
    ccc_colldirPixmapLabel->setPixmap(QPixmap(path+"collapse_xxxx.png"));

  if (colldir.compare("yyyy") == 0)
    ccc_colldirPixmapLabel->setPixmap(QPixmap(path+"collapse_yyyy.png"));
}


void parameters::showhide_bloom()
{
  if (weight_bloomCheckBox->isChecked()) {
    weight_bloomlowlimitLineEdit->show();
    weight_bloomminareaLineEdit->show();
    weight_bloomwidthLineEdit->show();
    weight_bloom1TextLabel->show();
    weight_bloom2TextLabel->show();
    weight_bloom3TextLabel->show();
  } 
  else {
    weight_bloomlowlimitLineEdit->hide();
    weight_bloomminareaLineEdit->hide();
    weight_bloomwidthLineEdit->hide();
    weight_bloom1TextLabel->hide();
    weight_bloom2TextLabel->hide();
    weight_bloom3TextLabel->hide();
  }
}


void parameters::showhide_clipped()
{
  if (coc_combineComboBox->currentText() == "CLIPPED") {
    coc_clipampfracLineEdit->show();
    coc_clipsigmaLineEdit->show();
    coc_clipampfracLabel->show();
    coc_clipsigmaLabel->show();
  } 
  else {
    coc_clipampfracLineEdit->hide();
    coc_clipsigmaLineEdit->hide();
    coc_clipampfracLabel->hide();
    coc_clipsigmaLabel->hide();
  }
}


void parameters::showhide_defectdetection()
{
  QString bayerpattern;
  QString fullinstpath;
  
  fullinstpath = get_instpath();

  get_fileparameter(fullinstpath, "BAYER", bayerpattern);

  if (bayerpattern.compare("N") == 0) {
    defectdetectionGroupBox->show();
  }
  if (bayerpattern.compare("Y") == 0) {
    defect_kernelsizeLineEdit->clear();
    defect_rowtolLineEdit->clear();
    defect_coltolLineEdit->clear();
    defect_clustolLineEdit->clear();
    defect_kernelsize_sfLineEdit->clear();
    defect_rowtol_sfLineEdit->clear();
    defect_coltol_sfLineEdit->clear();
    defect_clustol_sfLineEdit->clear();
    defectdetectionGroupBox->hide();
  }
}


void parameters::showhide_cosmicsDTDMIN()
{
  if (weight_cosmicLineEdit->text().isEmpty()) {
    weight_cosmicDTLineEdit->setDisabled(true);
    weight_cosmicDMINLineEdit->setDisabled(true);
  } 
  else {
    weight_cosmicDTLineEdit->setDisabled(false);
    weight_cosmicDMINLineEdit->setDisabled(false);
  }
}


void parameters::check_defect_kernelsize()
{
  if (!defect_rowtolLineEdit->text().isEmpty() ||
      !defect_coltolLineEdit->text().isEmpty() ||
      !defect_clustolLineEdit->text().isEmpty()) {
    if (defect_kernelsizeLineEdit->text().isEmpty())
      defect_kernelsizeLineEdit->setText("30");
  }
  if (!defect_rowtol_sfLineEdit->text().isEmpty() ||
      !defect_coltol_sfLineEdit->text().isEmpty() ||
      !defect_clustol_sfLineEdit->text().isEmpty()) {
    if (defect_kernelsize_sfLineEdit->text().isEmpty())
      defect_kernelsize_sfLineEdit->setText("30");
  }
}


void parameters::check_filterthreshold()
{
  float kappa = coc_filterthresholdLineEdit->text().toFloat();

  if (kappa < 3.0 && kappa > 0.0) {
    QMessageBox::warning( this, "Extremely low detection threshold!",
	"You chose an extremely aggressive detection threshold, which will\n"
	"result in excessive masking of pixels. Is your data properly calibrated?\n"
	"Maybe the dark frames used are wrong, or the detection of cosmics while\n"
	"creating the weights was switched off, or you screwed up the background\n"
        "subtraction (\"Subtract superflat\", near-IR data only).\n"
	"Or you did not dither your observations, in which case the spurious pixels\n"
	"cannot be removed by this outlier rejection.\n\n"
	"If you really want to keep this low threshold, then combine it at least with\n"
	"a very high cluster size, such as 5-7, depending on the size of the resampling\n"
        "kernel. Even then, expect massive masking! Check the resampled weights!\n");
    return;
  }
}
