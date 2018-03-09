#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qsettings.h>
#include <qtimer.h>
#include <qtoolbutton.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include "functions.h"

QString cni_homedir, cni_scriptdir, cni_userinstdir, cni_userscriptdir;
QString ssd_instrumentname, ssd_numchips, ssd_inst_path, cni_bindir, cni_tmpdir;
int int_ssd_numchips, int_numccd;


void cni_form::init()
{
  QString configpath, logpath, lastlog;
  QSettings settings (QSettings::Ini);
  
  // get general info
  settings.beginGroup("/theligeneral_");
  lastlog = settings.readEntry("/lastlog", "noname");
  cni_scriptdir = settings.readEntry("/mainpath", "");
  cni_homedir = settings.readEntry("/homedir", "");
  cni_bindir = settings.readEntry("/bindir", "");
  bash = settings.readEntry("/bash", "");
  settings.endGroup();
  
  cni_tmpdir = cni_homedir+".theli/tmp/";
  
  QHeader *verticalheader = ssd_transformationTable->verticalHeader();
  verticalheader->hide();
  ssd_transformationTable->setLeftMargin(0);
  
  ssd_transformationTable->setColumnWidth(0,50);
  ssd_transformationTable->setColumnWidth(1,50);
  ssd_transformationTable->setColumnWidth(2,50);
  ssd_transformationTable->setColumnWidth(3,60);
  
  ssd_transformationTable->setMaximumWidth(215);
  ssd_transformationTable->setMaximumHeight(187);
  
  ssd_transformationTable->setPaletteBackgroundColor(QColor(255,254,212));
  
  ssd_transformationTable->setText(0,0,"0 ");
  ssd_transformationTable->setText(1,0,"1 ");
  ssd_transformationTable->setText(2,0,"2 ");
  ssd_transformationTable->setText(3,0,"3 ");
  ssd_transformationTable->setText(4,0,"4 ");
  ssd_transformationTable->setText(5,0,"5 ");
  ssd_transformationTable->setText(6,0,"6 ");
  ssd_transformationTable->setText(7,0,"7 ");
  
  ssd_transformationTable->setText(0,1," 0 ");
  ssd_transformationTable->setText(1,1," 0 ");
  ssd_transformationTable->setText(2,1," 0 ");
  ssd_transformationTable->setText(3,1,"  90 ");
  ssd_transformationTable->setText(4,1,"  90 ");
  ssd_transformationTable->setText(5,1,"  90 ");
  ssd_transformationTable->setText(6,1," -90 ");
  ssd_transformationTable->setText(7,1," 180 ");
  
  ssd_transformationTable->setText(0,2," ");
  ssd_transformationTable->setText(1,2," x");
  ssd_transformationTable->setText(2,2," y");
  ssd_transformationTable->setText(3,2," ");
  ssd_transformationTable->setText(4,2," x");
  ssd_transformationTable->setText(5,2," y");
  ssd_transformationTable->setText(6,2," ");
  ssd_transformationTable->setText(7,2," ");
  
  ssd_transformationTable->setText(0,3,"");
  ssd_transformationTable->setText(1,3," 180 | y");
  ssd_transformationTable->setText(2,3," 180 | x");
  ssd_transformationTable->setText(3,3,"");
  ssd_transformationTable->setText(4,3," -90 | y");
  ssd_transformationTable->setText(5,3," -90 | x");
  ssd_transformationTable->setText(6,3,"");
  ssd_transformationTable->setText(7,3,"");
  
  // set some default keys we want to keep
  ssd_dummy1LineEdit->setText("DATE-OBS");
  ssd_rawdummy1LineEdit->setText("DATE-OBS");
  ssd_dummy2LineEdit->setText("MJD-OBS");
  ssd_rawdummy2LineEdit->setText("MJD");
  
  // set the instrument name, get the scriptdir and NCHIPS
  ssd_set_instrumentname();
  
  // handle the bayer pattern stuff
  bayer_RAW_RadioButton->setChecked(true);
  populate_bayerbuttons();
  untoggle_bayerbuttons();
  showhide_bayerpattern();
  
  monochrome_FITS_RadioButton->setChecked(true);
  bayerframe->hide();
  
  cni_userinstdir = cni_homedir + ".theli/instruments_user/";
  cni_userscriptdir = cni_homedir + ".theli/scripts/";
  
  // we want to use the default CD matrix values
  ssd_preserveCDCheckBox->setChecked(true);
}


void cni_form::populate_bayerbuttons()
{
  QString path;
  
  path = cni_scriptdir + "../images/";
  bayer_RGGB_ToolButton->setIconSet(QPixmap(path+"bayer_rggb_a.png"));
  bayer_GRBG_ToolButton->setIconSet(QPixmap(path+"bayer_grbg_a.png"));
  bayer_GBRG_ToolButton->setIconSet(QPixmap(path+"bayer_gbrg_a.png"));
  bayer_BGGR_ToolButton->setIconSet(QPixmap(path+"bayer_bggr_a.png"));
  
  bayer_RGGB_ToolButton->setUsesBigPixmap(true);
  bayer_GRBG_ToolButton->setUsesBigPixmap(true);
  bayer_GBRG_ToolButton->setUsesBigPixmap(true);
  bayer_BGGR_ToolButton->setUsesBigPixmap(true);
}


void cni_form::untoggle_bayerbuttons()
{
  bayer_RGGB_ToolButton->setOn(true);
  bayer_GRBG_ToolButton->setOn(false);
  bayer_GBRG_ToolButton->setOn(false);
  bayer_BGGR_ToolButton->setOn(false);
  active_passive_bayer();
}


void cni_form::active_passive_bayer()
{
  QString path;
  path = cni_scriptdir + "../images/";
  
  if ( bayer_RGGB_ToolButton->isOn())
    bayer_RGGB_ToolButton->setIconSet(QPixmap(path+"bayer_rggb_a.png"));
  else bayer_RGGB_ToolButton->setIconSet(QPixmap(path+"bayer_rggb_p.png"));
  if ( bayer_GRBG_ToolButton->isOn())
    bayer_GRBG_ToolButton->setIconSet(QPixmap(path+"bayer_grbg_a.png"));
  else bayer_GRBG_ToolButton->setIconSet(QPixmap(path+"bayer_grbg_p.png"));
  if ( bayer_GBRG_ToolButton->isOn())
    bayer_GBRG_ToolButton->setIconSet(QPixmap(path+"bayer_gbrg_a.png"));
  else bayer_GBRG_ToolButton->setIconSet(QPixmap(path+"bayer_gbrg_p.png"));
  if ( bayer_BGGR_ToolButton->isOn())
    bayer_BGGR_ToolButton->setIconSet(QPixmap(path+"bayer_bggr_a.png"));
  else bayer_BGGR_ToolButton->setIconSet(QPixmap(path+"bayer_bggr_p.png"));
}


void cni_form::showhide_bayerpattern()
{
  showhide_bayerframe();
  if (!bayer_FITS_RadioButton->isChecked()) {
    bayer_RGGB_ToolButton->hide();
    bayer_GRBG_ToolButton->hide();
    bayer_GBRG_ToolButton->hide();
    bayer_BGGR_ToolButton->hide();
    selectBayerPatternLabel->hide();
    bayer_ButtonGroup->hide();
  } 
  else {
    bayer_RGGB_ToolButton->show();
    bayer_GRBG_ToolButton->show();
    bayer_GBRG_ToolButton->show();
    bayer_BGGR_ToolButton->show();
    selectBayerPatternLabel->show();
    bayer_ButtonGroup->show();
  }
  active_passive_bayer();
}


void cni_form::showhide_bayerframe()
{
  if (!bayer_FITS_RadioButton->isChecked()) bayerframe->hide();
  else bayerframe->show();
}


void cni_form::destroy()
{
  // nothing for the moment
}


void cni_form::validate()
{    
  //    QRegExp rf1( "^[-]{0,1}[0-9]+[.]{0,1}[0-9]+" );
  QRegExp rf1( "[-0-9.]+" );
  QValidator* validator_float = new QRegExpValidator(rf1, this );
  //    QRegExp re( "^[-]{0,1}[0-9][.]{0,1}[0-9]+[e]{0,1}[-]{0,1}[0-9]+" );
  QRegExp re( "[-0-9e.]+" );
  //    QValidator* validator_exp = new QRegExpValidator(re, this );
  //    QRegExp rf2( "[0-9]+[.]{0,1}[0-9]+" );
  QRegExp rf2( "[0-9.]+" );
  QValidator* validator_floatpos = new QRegExpValidator(rf2, this );
  QRegExp ri3( "[0-9\\s]+" );
  QValidator* validator_intposblank = new QRegExpValidator(ri3, this );
  QRegExp ri4( "[-0-9\\s]+" );
  QValidator* validator_intblank = new QRegExpValidator(ri4, this );
  cni_geogLineEdit->setValidator( validator_float );
  cni_pixsclLineEdit->setValidator( validator_floatpos );
  cni_ovminLineEdit->setValidator( validator_intposblank );
  cni_ovmaxLineEdit->setValidator( validator_intposblank);
  cni_cutonxLineEdit->setValidator( validator_intposblank );
  cni_cutonyLineEdit->setValidator( validator_intposblank );
  cni_xsizeLineEdit->setValidator( validator_intposblank );
  cni_ysizeLineEdit->setValidator( validator_intposblank );
  cni_crpix1LineEdit->setValidator( validator_intblank );
  cni_crpix2LineEdit->setValidator( validator_intblank );

  // instrument name must not accept blank characters
  QRegExp rx2( "^\\S+$" );
  QValidator* validator_string2 = new QRegExpValidator( rx2, this );
  cni_instnameLineEdit->setValidator( validator_string2 );
}


void cni_form::clearfields()
{
  cni_nccdSpinBox->setValue(1);
  cni_instnameLineEdit->clear();
  cni_geogLineEdit->clear();
  cni_pixsclLineEdit->clear();
  cni_ovminLineEdit->clear();
  cni_ovmaxLineEdit->clear();
  cni_cutonxLineEdit->clear();
  cni_cutonyLineEdit->clear();
  cni_xsizeLineEdit->clear();
  cni_ysizeLineEdit->clear();
  cni_crpix1LineEdit->clear();
  cni_crpix2LineEdit->clear();
}


void cni_form::count_ccds_entered()
{
  QString ccdgeometry, numccd;
  
  // check if the user enters valid characters
  validate();
  
  // Retrieve the inputs and update the fields
  ccdgeometry = cni_ovminLineEdit->text();
  get_ccdnumber(ccdgeometry, numccd);
  cni_ovminnumchkLineEdit->setText(numccd);
  
  ccdgeometry = cni_ovmaxLineEdit->text();
  get_ccdnumber(ccdgeometry, numccd);
  cni_ovmaxnumchkLineEdit->setText(numccd);
  
  ccdgeometry = cni_cutonxLineEdit->text();
  get_ccdnumber(ccdgeometry, numccd);
  cni_cutonxnumchkLineEdit->setText(numccd);
  
  ccdgeometry = cni_cutonyLineEdit->text();
  get_ccdnumber(ccdgeometry, numccd);
  cni_cutonynumchkLineEdit->setText(numccd);
  
  ccdgeometry = cni_xsizeLineEdit->text();
  get_ccdnumber(ccdgeometry, numccd);
  cni_xsizenumchkLineEdit->setText(numccd);
  
  ccdgeometry = cni_ysizeLineEdit->text();
  get_ccdnumber(ccdgeometry, numccd);
  cni_ysizenumchkLineEdit->setText(numccd);
  
  ccdgeometry = cni_crpix1LineEdit->text();
  get_ccdnumber(ccdgeometry, numccd);
  cni_crpix1numchkLineEdit->setText(numccd);
  
  ccdgeometry = cni_crpix2LineEdit->text();
  get_ccdnumber(ccdgeometry, numccd);
  cni_crpix2numchkLineEdit->setText(numccd);
  
  compare_nchips_nccd();
}


void cni_form::fill_in_presentinst()
{
  QSettings settings (QSettings::Ini);
  QString presentinst, lastlog;
  
  // get present instrument name
  settings.beginGroup("/theligeneral_");
  lastlog = settings.readEntry("/lastlog", "noname");
  settings.endGroup();
  settings.beginGroup("/theli_"+lastlog+"_");
  settings.beginGroup("/INITIALISE");
  presentinst = settings.readEntry("/instrumentname");
  if (settings.readBoolEntry("/inst_prof",true))
    ssd_inst_path = cni_scriptdir + "/instruments_professional/";
  if (settings.readBoolEntry("/inst_comm",true))
    ssd_inst_path = cni_scriptdir + "/instruments_commercial/";
  if (settings.readBoolEntry("/inst_user",true))
    ssd_inst_path = cni_homedir + "/.theli/instruments_user/";
  settings.endGroup();
  settings.endGroup();
  
  QString filename = ssd_inst_path;
  filename.append(presentinst);
  filename.append(".ini");
  read_conffile(filename);
}


void cni_form::fill_in_WFI_example()
{
  QString filename = cni_scriptdir+"/instruments_professional/";
  filename.append("WFI@MPGESO.ini");
  read_conffile(filename);
}


void cni_form::fill_in_WFC_example()
{
  QString filename = cni_scriptdir+"/instruments_professional/";
  filename.append("WFC@INT.ini");
  read_conffile(filename);
}


void cni_form::fill_in_ST10_example()
{
  QString filename = cni_scriptdir+"/instruments_commercial/";
  filename.append("ST10.ini");
  read_conffile(filename);
}


void cni_form::read_conffile( QString conffile )
{
  QFile file(conffile);
  if (file.exists()) {
    get_fileparameter( conffile, "INSTRUMENT");
    get_fileparameter( conffile, "NCHIPS");
    get_fileparameter( conffile, "OBSLAT");
    get_fileparameter( conffile, "PIXSCALE");
    get_fileparameter( conffile, "OVSCANX1");
    get_fileparameter( conffile, "OVSCANX2");
    get_fileparameter( conffile, "CUTX");
    get_fileparameter( conffile, "CUTY");
    get_fileparameter( conffile, "SIZEX");
    get_fileparameter( conffile, "SIZEY");
    get_fileparameter( conffile, "REFPIXX");
    get_fileparameter( conffile, "REFPIXY");
    get_fileparameter( conffile, "BAYER");
    get_fileparameter( conffile, "TYPE");
    
    compare_nchips_nccd();
  }
}


void cni_form::get_inst_parameter(QString file, QString key, QString *value)
{
  QString execstr, line;

  // print the entire line that contains the relevant string
  execstr = "gawk '($0~/";
  execstr.append(key);
  execstr.append("=/) {print}' ");
  execstr.append(file);
  
  my_fgets(execstr, line);
  
  if (!line.isEmpty()) {
    
    // remove the newline feed included by the fgets function
    line.setLength(line.length()-1);
    
    // get the argument only
    *value = line.section('=',1);
    
    // "value" now has either simple numeric form or 
    // "KEY=([1]=4106 [2]=10 [3]=4106 [4]=4106)"
    // remove all square brackets [...] and equal (=) signs from it
    QRegExp rx("(\\[[0-9]+\\]=)");
    (*value).replace(rx,"");
    (*value).replace("(","");
    (*value).replace(")","");
  }
  else *value = "";
}


void cni_form::get_fileparameter(QString file, QString key)
{
  QString execstr, line, value;
  
  // print the entire line that contains the relevant string
  execstr = "gawk '($0~/";
  execstr.append(key);
  execstr.append("=/) {print}' ");
  execstr.append(file);
  
  my_fgets(execstr, line);
  
  if (!line.isEmpty()) {
    
    // remove the newline feed included by the fgets function
    line.setLength(line.length()-1);
    
    // get the argument only
    value = line.section('=',1);
    
    // "value" now has either simple numeric form, "Y/N", or 
    // "KEY=([1]=4106 [2]=10 [3]=4106 [4]=4106)"
    // remove all square brackets [...] and equal (=) signs from it
    QRegExp rx("(\\[[0-9]+\\]=)");
    value.replace(rx,"");
    value.replace("(","");
    value.replace(")","");
  }
  else value = "";
  
  // populate the fields in the instconfig file dialogue
  if (key.compare("INSTRUMENT") == 0) cni_instnameLineEdit->setText(value);
  if (key.compare("NCHIPS") == 0) cni_nccdSpinBox->setValue(value.toInt());
  if (key.compare("OBSLAT") == 0) cni_geogLineEdit->setText(value);
  if (key.compare("PIXSCALE") == 0) cni_pixsclLineEdit->setText(value);
  if (key.compare("OVSCANX1") == 0) cni_ovminLineEdit->setText(value);
  if (key.compare("OVSCANX2") == 0) cni_ovmaxLineEdit->setText(value);
  if (key.compare("CUTX") == 0) cni_cutonxLineEdit->setText(value);
  if (key.compare("CUTY") == 0) cni_cutonyLineEdit->setText(value);
  if (key.compare("SIZEX") == 0) cni_xsizeLineEdit->setText(value);
  if (key.compare("SIZEY") == 0) cni_ysizeLineEdit->setText(value);
  if (key.compare("REFPIXX") == 0) cni_crpix1LineEdit->setText(value);
  if (key.compare("REFPIXY") == 0) cni_crpix2LineEdit->setText(value);
  if (key.compare("TYPE") == 0) {
    if (value.compare("OPT") == 0) cni_cameratypeComboBox->setCurrentItem(0);
    if (value.compare("NIR") == 0) cni_cameratypeComboBox->setCurrentItem(1);
    if (value.compare("MIR") == 0) cni_cameratypeComboBox->setCurrentItem(2);
  }
}


void cni_form::get_ccdnumber( QString ccdgeometry, QString &numccd)
{
  QString execstr;
  
  // create awk command
  execstr = "echo ";
  execstr.append(ccdgeometry);
  execstr.append(" | gawk '{print NF}'");
  
  my_fgets(execstr, numccd);
}


void cni_form::compare_nchips_nccd()
{
  int nccds, nfields1, nfields2, nfields3, nfields4;
  int nfields5, nfields6, nfields7, nfields8;
  
  nccds = cni_nccdSpinBox->value();
  nfields1 = cni_ovminnumchkLineEdit->text().toInt();
  nfields2 = cni_ovmaxnumchkLineEdit->text().toInt();
  nfields3 = cni_cutonxnumchkLineEdit->text().toInt();
  nfields4 = cni_cutonynumchkLineEdit->text().toInt();
  nfields5 = cni_xsizenumchkLineEdit->text().toInt();
  nfields6 = cni_ysizenumchkLineEdit->text().toInt();
  nfields7 = cni_crpix1numchkLineEdit->text().toInt();
  nfields8 = cni_crpix2numchkLineEdit->text().toInt();
  
  QColor cnigreen = QColor( 155, 255, 187);
  QColor cnired = QColor( 255, 187, 155);
  
  if (nccds != nfields1) cni_ovminnumchkLineEdit->setPaletteBackgroundColor( cnired);
  else cni_ovminnumchkLineEdit->setPaletteBackgroundColor( cnigreen);
  if (nccds != nfields2) cni_ovmaxnumchkLineEdit->setPaletteBackgroundColor( cnired);
  else cni_ovmaxnumchkLineEdit->setPaletteBackgroundColor( cnigreen);
  if (nccds != nfields3) cni_cutonxnumchkLineEdit->setPaletteBackgroundColor( cnired);
  else cni_cutonxnumchkLineEdit->setPaletteBackgroundColor( cnigreen);
  if (nccds != nfields4) cni_cutonynumchkLineEdit->setPaletteBackgroundColor( cnired);
  else cni_cutonynumchkLineEdit->setPaletteBackgroundColor( cnigreen);
  if (nccds != nfields5) cni_xsizenumchkLineEdit->setPaletteBackgroundColor( cnired);
  else cni_xsizenumchkLineEdit->setPaletteBackgroundColor( cnigreen);
  if (nccds != nfields6) cni_ysizenumchkLineEdit->setPaletteBackgroundColor( cnired);
  else cni_ysizenumchkLineEdit->setPaletteBackgroundColor( cnigreen);
  if (nccds != nfields7) cni_crpix1numchkLineEdit->setPaletteBackgroundColor( cnired);
  else cni_crpix1numchkLineEdit->setPaletteBackgroundColor( cnigreen);
  if (nccds != nfields8) cni_crpix2numchkLineEdit->setPaletteBackgroundColor( cnired);
  else cni_crpix2numchkLineEdit->setPaletteBackgroundColor( cnigreen);
}


void cni_form::load_instconfig()
{
  QString filename, progsini;
  
  QString currentFileName = QFileDialog::getOpenFileName(
	ssd_inst_path,
	"Instrument config files (*.ini)",
	this,
	"open file dialog",
	"Choose an instrument config file");

  if ( currentFileName.isEmpty() )
    return;
  
  read_conffile(currentFileName);
  
  // set the cursor to the home position for all line edits
  cni_ovminLineEdit->home(false);
  cni_ovmaxLineEdit->home(false);
  cni_cutonxLineEdit->home(false);
  cni_cutonyLineEdit->home(false);
  cni_xsizeLineEdit->home(false);
  cni_ysizeLineEdit->home(false);
  cni_crpix1LineEdit->home(false);
  cni_crpix2LineEdit->home(false);
}


void cni_form::save_instconfig()
{
  FILE *of;
  int length, i, check;
  float cdelt1, cdelt2;
  QString suggestedfilename, currentFileName, progsini;
  QString lead, execstr, cutonx, cutony, checkstring, cameratype;
  
  // open the save dialogue only if all information has been entered
  // and if the chip numbers match
  int nccds = cni_nccdSpinBox->value();
  int nf1 = cni_ovminnumchkLineEdit->text().toInt();
  int nf2 = cni_ovmaxnumchkLineEdit->text().toInt();
  int nf3 = cni_cutonxnumchkLineEdit->text().toInt();
  int nf4 = cni_cutonynumchkLineEdit->text().toInt();
  int nf5 = cni_xsizenumchkLineEdit->text().toInt();
  int nf6 = cni_ysizenumchkLineEdit->text().toInt();
  int nf7 = cni_crpix1numchkLineEdit->text().toInt();
  int nf8 = cni_crpix2numchkLineEdit->text().toInt();
  if ( nccds != nf1 || nccds != nf2 || nccds != nf3 || nccds != nf4 || 
       nccds != nf5 || nccds != nf6 || nccds != nf7 || nccds != nf8) {
    QMessageBox::warning( this, "Save instrument configuration file",
			  "Mismatch between number of elements entered\n"
			  "and the number of detectors in the instrument.\n"
			  "Complete the input before the file can be saved.");
    return;
  }
  
  // the camera type used
  if (cni_cameratypeComboBox->currentText() == "OPTICAL") cameratype = "OPT";
  if (cni_cameratypeComboBox->currentText() == "NEAR-IR") cameratype = "NIR";
  if (cni_cameratypeComboBox->currentText() == "MID-IR") cameratype  = "MIR";

  // make sure the cut-on x|y values make sense (must not be zero)
  check = 1;
  cutonx = cni_cutonxLineEdit->text();
  cutony = cni_cutonyLineEdit->text();
  for (i=0; i<nccds; i++) {
    checkstring = cutonx.section(" ", i, i);
    if (checkstring.compare("0") == 0) check *= 0;
    else check *= 1;
    checkstring = cutony.section(" ", i, i);
    if (checkstring.compare("0") == 0) check *= 0;
    else check *= 1;
  }
  if (check == 0) {
    QMessageBox::warning( this, "Save instrument configuration file",
			  "Cut-on x|y: Values must NOT be zero!\n"
			  "Minimum value allowed: 1\n"
			  "The configuration file will not be saved unless this is fixed.");
    return;
  }

  suggestedfilename = cni_userinstdir;
  suggestedfilename.append(cni_instnameLineEdit->text());
  suggestedfilename.append(".ini");
  
  currentFileName = suggestedfilename;
  
  if ( !currentFileName.isEmpty()) {
    int answer = 0;
    progsini = cni_scriptdir;
    progsini.append("progs.ini");
    // Print a note when the user tries to overwrite progs.ini. 
    // Do nothing then.
    if (currentFileName.compare(progsini) == 0) {
      QMessageBox::warning( this, "Save instrument configuration file",
			    "progs.ini: \n"
			    "You are not allowed to overwrite this file.\n"
			    "Select a different name for your instrument configuration file." );
      answer = 1;
    }
    // Print warning if an existing file is going to be overwritten.
    if ( QFile::exists(currentFileName) && 
	 currentFileName.compare(progsini) != 0)
      answer = QMessageBox::warning(
		    this, "Overwrite File",
		    QString( "<b>WARNING:</b> \n\nYou are going to overwrite\n\n\'%1\'?" ).
		    arg( currentFileName ),
		    "&OK", "&Cancel", QString::null, 0, 1 );
    // Save file otherwise
    if ( answer == 0 ) {
      
      // force a ".ini" suffix for the instrument config file name
      if (!currentFileName.endsWith(".ini")) currentFileName.append(".ini");
      
      // give the instrument name field the name that was chosen for the config file
      QFileInfo fileinfo = QFileInfo(currentFileName);
      QString lineeditname = fileinfo.fileName();
      length = lineeditname.length() - 4;
      lineeditname = lineeditname.left(length);
      cni_instnameLineEdit->setText(lineeditname);
      
      if ((of = fopen(currentFileName, "w")) != NULL) {
	
	fprintf(of, "# **************************************************************\n");
	fprintf(of, "#            Instrument config file created by THELI\n");
	fprintf(of, "# **************************************************************\n\n");
	
	fprintf(of, "# Name of the instrument (must be unique!)\n");
	lead = cni_instnameLineEdit->text();
	fprintf(of, "INSTRUMENT=%s\n\n", lead.ascii());
	
	fprintf(of, "# Number of chips\n");
	fprintf(of, "NCHIPS=%d\n\n", cni_nccdSpinBox->value());
	
	fprintf(of, "# We need to export these\n");
	fprintf(of, "export INSTRUMENT\n");
	fprintf(of, "export NCHIPS\n\n");
	
	fprintf(of, "# Load directories for programs\n");
	fprintf(of, ". progs.ini\n\n");
	
	lead = cni_geogLineEdit->text();
	fprintf(of, "# Geographical latitude of the observatory\n");
	fprintf(of, "OBSLAT=%s\n\n", lead.ascii());
	
	fprintf(of, "# Pixel scale in x and y [degrees] (CDELT1/2 keywords)\n");
	lead = cni_pixsclLineEdit->text();
	cdelt1 = lead.toFloat() / 3600. * (-1);
	cdelt2 = lead.toFloat() / 3600.;
	fprintf(of, "PIXSCX=%g\n", cdelt1);
	fprintf(of, "PIXSCY=%g\n\n", cdelt2);
	
	fprintf(of, "# Pixel scale in arcsec\n");
	fprintf(of, "PIXSCALE=%s\n\n", lead.ascii());
	
	fprintf(of, "# Detector gain\n");
	fprintf(of, "GAIN=1.0\n\n");
	
	fprintf(of, "# Overscan regions. The values are the x-range.\n");    
	fprintf(of, "# The min is given by OVSCANX1, the max by OVSCANX2.\n");    
	fprintf(of, "# We need separate regions for each chip.\n\n");
	save_instconfig_helper( of, cni_ovminLineEdit, "OVSCANX1=(");
	save_instconfig_helper( of, cni_ovmaxLineEdit, "OVSCANX2=(");
	fprintf(of, "\n");
	
	fprintf(of, "# Sections for the cutting. Select those pixels that receive light.\n");
	fprintf(of, "# Give min values for x and y, and the extent in x and y.\n\n");
	save_instconfig_helper( of, cni_cutonxLineEdit, "CUTX=(");
	save_instconfig_helper( of, cni_cutonyLineEdit, "CUTY=(");
	save_instconfig_helper( of, cni_xsizeLineEdit, "SIZEX=(");
	save_instconfig_helper( of, cni_ysizeLineEdit, "SIZEY=(");
	fprintf(of, "\n");
	
	fprintf(of, "# Reference pixels for each chip.\n\n");    
	save_instconfig_helper( of, cni_crpix1LineEdit, "REFPIXX=(");
	save_instconfig_helper( of, cni_crpix2LineEdit, "REFPIXY=(");
	fprintf(of, "\n");

	fprintf(of, "# Camera type\n");
	fprintf(of, "TYPE=%s\n\n", cameratype.ascii());
	
	fclose(of);
      }
      // change the text label of the pushbutton for a short while
      cni_savePushButton->setPaletteForegroundColor(QColor(0,155,0));
      cni_savePushButton->setText("Done");
      QTimer *timer = new QTimer();
      connect( timer, SIGNAL(timeout()), SLOT(timerSavebuttonDone()) );
      timer->start( 1000, TRUE );
      
      return;	
    }
  }
}


void cni_form::timerSavebuttonDone()
{
  cni_savePushButton->setPaletteForegroundColor(QColor(0,0,0));
  cni_savePushButton->setText("Save");
}


void cni_form::save_instconfig_helper( FILE *of, QLineEdit * le, QString lead)
{
  FILE *stream;
  int i;
  int nchips = cni_nccdSpinBox->value();
  long conf_arr[nchips];
  char helper[1000];
  
  QString execstr = "echo ";
  execstr.append(le->text());
  stream = popen(execstr, "r");
  for (i=0; i<nchips; i++) {
    if (fscanf(stream, "%s", helper) == 0) {
      printf("\tError: Could not read from stdin!\n");
      strcpy(helper,"0");
    }
    conf_arr[i]=atol(helper);
  }
  pclose(stream);
  
  fprintf(of, "%s", lead.ascii());
  for (i=0; i<nchips; i++) {
    if (i<nchips-1) {
      fprintf(of, "%s%d%s", "[", i+1, "]="); 
      fprintf(of, "%ld%s", conf_arr[i], " ");
    }
    if (i==nchips-1) {
      fprintf(of, "%s%d%s", "[", i+1, "]=");
      fprintf(of, "%ld%s", conf_arr[i], ")\n");
    }	    
  }   
}


void cni_form::reset_nccdspinbox()
{
  cni_nccdSpinBox->setValue(1);
}


// set the instrument name, get the scriptdir and NCHIPS
void cni_form::ssd_set_instrumentname()
{
  QString execstr, lastlog, ssd_inst_currentname;
  QString tmp1, tmp2, tmp3, transformationcode;
  QSettings settings (QSettings::Ini);
  QFile tmpinst1, tmpinst2, tmpinst3;
  int i;
  
  ssd_inst_currentname = cni_instnameLineEdit->text();
  if (ssd_inst_currentname.isEmpty()) return;
  
  tmpinst1.setName(cni_userinstdir+"/"+ssd_inst_currentname+".ini");
  if (tmpinst1.exists()) ssd_instrumentname = ssd_inst_currentname;
  else {
    QMessageBox::warning( this, "Could not locate instrument.ini",
			  "<b>Warning:</b><br> The instrument could not be identified correctly in an automatic way.<br>\n"
			  "Please select it manually from the following dialog.\n");
    ssd_instfiledialog();
  }
  if (tmpinst1.exists()) ssd_instrumentname = ssd_inst_currentname;
  else return;
  
  // get general info
  settings.beginGroup("/theligeneral_");
  lastlog = settings.readEntry("/lastlog", "noname");
  cni_scriptdir = settings.readEntry("/mainpath", "");
  cni_homedir = settings.readEntry("/homedir", "");
  settings.endGroup();
  settings.beginGroup("/theli_"+lastlog+"_");
  settings.beginGroup("/INITIALISE");
  //    if (!tmpinst1.exists()) 
  //      ssd_instrumentname = settings.readEntry ("/instrumentname");
  
  tmp1 = cni_scriptdir + "/instruments_professional/"+ssd_instrumentname+".ini";
  tmp2 = cni_scriptdir + "/instruments_commercial/"+ssd_instrumentname+".ini";
  tmp3 = cni_homedir + "/.theli/instruments_user/"+ssd_instrumentname+".ini";
  tmpinst1.setName(tmp1);
  tmpinst2.setName(tmp2);
  tmpinst3.setName(tmp3);
  if (tmpinst1.exists()) ssd_inst_path = cni_scriptdir + "/instruments_professional/";
  else if (tmpinst2.exists()) ssd_inst_path = cni_scriptdir + "/instruments_commercial/";
  else if (tmpinst3.exists()) ssd_inst_path = cni_homedir + "/.theli/instruments_user/";
  
  settings.endGroup();
  settings.endGroup();
  
  // get the number of chips of the instrument
  execstr = "grep NCHIPS= ";
  execstr.append(ssd_inst_path+ssd_instrumentname+".ini");
  execstr.append("| gawk 'BEGIN {FS=\"=\"} {print $2}'");
  
  my_fscanf(execstr, ssd_numchips);
  
  // for later use
  int_ssd_numchips = ssd_numchips.toInt();
  
  // set the instrument name in the dialog
  ssd_instrumentLineEdit->setText(ssd_instrumentname);
  
  // paint the instrument line edit green if it exists
  ssd_paint_instrumentlineedit();
  
  // fill the transformation code with default zeros
  transformationcode="";
  for (i=0; i<int_ssd_numchips; i++) {
    transformationcode.append("0 ");
  }
  ssd_transformationcodesLineEdit->setText(transformationcode);
}


void cni_form::ssd_paint_instrumentlineedit()
{
  QString instrumentname, instpath;
  instrumentname = ssd_instrumentLineEdit->text();
  instrumentname.append(".ini");
  instpath = ssd_inst_path;
  
  instpath.append(instrumentname);
  
  QFile instconfigfile(instpath);
  if (instconfigfile.exists()) 
    ssd_instrumentLineEdit->setPaletteBackgroundColor(QColor(150,255,187));
  else 
    ssd_instrumentLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
}


void cni_form::ssd_instfiledialog()
{
  int length;
  QString execstr;
  
  QString currentFileName = QFileDialog::getOpenFileName(
	    cni_userinstdir,
	    "Instruments (*.ini)",
	    this,
	    "open file dialog",
	    "Select an instrument");

  if ( currentFileName.isEmpty() || currentFileName.length() <= 4)
    return;
  
  // cut the path and the .ini extension
  QFileInfo fileinfo = QFileInfo(currentFileName);
  QString fullinstname = fileinfo.fileName();
  length = fullinstname.length() - 4;
  fullinstname = fullinstname.left(length);
  
  ssd_instrumentLineEdit->setText(fullinstname);
  
  ssd_paint_instrumentlineedit();
  
  // get the number of chips of the instrument
  execstr = "grep NCHIPS= ";
  execstr.append(ssd_inst_path);
  execstr.append("/");
  execstr.append(fullinstname);
  execstr.append(".ini");
  execstr.append("| gawk 'BEGIN {FS=\"=\"} {print $2}'");
  my_fscanf(execstr, ssd_numchips);
  
  ssd_countccds();
}


void cni_form::ssd_verify_input()
{    
  QRegExp ri3( "([0-7]+\\s+)+" );
  QValidator* validator_intposblank = new QRegExpValidator(ri3, this );
  ssd_transformationcodesLineEdit->setValidator( validator_intposblank );
}


void cni_form::ssd_countccds()
{
  QString execstr, ccdgeometry, numccd;
  
  // Retrieve the input and update the field
  ccdgeometry = ssd_transformationcodesLineEdit->text();
  
  // get the number of elements entered
  execstr = "echo ";
  execstr.append(ccdgeometry);
  execstr.append("| gawk '{print NF}'");
  my_fgets(execstr, numccd);
  ssd_numccdsLineEdit->setText(numccd);
  
  // paint the number field accordingly
  int_ssd_numchips = ssd_numchips.toInt();
  int_numccd = numccd.toInt();
  
  if (int_ssd_numchips != int_numccd) 
    ssd_numccdsLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
  else ssd_numccdsLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
}


void cni_form::write_splitscript()
{
  FILE *of;
  int i, check2;
  QString execstr, splitscriptfilename;
  QString ra, dec, lst, mjd, exptime, object, airmass, filter, equinox, getkey, getkeyc;
  QString m11, m12, m21, m22, ra_convert, dec_convert, lst_convert;
  QString crpix1, crpix2, cd11, cd12, cd21, cd22, filter1, filter2;
  QString bayerpattern, dit, ndit;
  QString rawdummy1, rawdummy2, rawdummy3, rawdummy4;
  QString rawdummy5, rawdummy6, rawdummy7, rawdummy8;
  QString dummy1, dummy2, dummy3, dummy4, dummy5, dummy6;
  QString dummy7, dummy8;
  QString code;
  
  // do nothing if the number of CCDs in the instconfigfile
  // does not match the number of entered transformation codes
  if (int_ssd_numchips != int_numccd) {
    QMessageBox::warning( this, "Mismatch in detector numbers",
			  "<b>Warning:</b><br> The number of transformation codes you entered\n"
			  "does not match the number of chips in the camera.<br>\n"
			  "The splitting script will not be written.\n");
    return;
  }
  // do nothing if the transformation code does not consist of numbers 0-7 only
  check2 = 0;
  code = ssd_transformationcodesLineEdit->text();
  for (i=0; i<int_ssd_numchips; i++) {
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() > 7) check2 = 1;
  }
  if (check2 == 1) {
    QMessageBox::warning( this, "Invalid transformation code.",
			  "<b>Warning:</b><br> You entered a nonvalid transformation code,\n"
			      "at least one of the values is larger than 7.<br>\n"
			  "The splitting script will not be written.\n");
    return;
  }
  
  ssd_instrumentname = ssd_instrumentLineEdit->text();
  
  if (ssd_instrumentname.isEmpty()) {
    QMessageBox::warning( this, "Missing instrument",
			  "No instrument has been selected!<br>\n"
			  "Please choose one by clicking on 'Change instrument'\n");
    return;
  }
  
  // get the main FITS keywords
  ra = ssd_rakeyLineEdit->text();
  dec = ssd_deckeyLineEdit->text();
  lst = ssd_lstkeyLineEdit->text();
  mjd = ssd_mjdkeyLineEdit->text();
  exptime = ssd_exptimekeyLineEdit->text();
  airmass = ssd_airmasskeyLineEdit->text();
  filter = ssd_filterkeyLineEdit->text();
  equinox = ssd_equinoxLineEdit->text();
  object = "OBJECT";
  
  // checking the format; if not in decimal, convert
  if (ssd_raComboBox->currentItem() != 0)
    ra_convert = "  RA=`${P_HMSTODECIMAL} ${RA}`";
  else ra_convert = "";
  if (ssd_decComboBox->currentItem() != 0)
    dec_convert = "  DEC=`${P_DMSTODECIMAL} ${DEC}`";
  else dec_convert = "";
  if (ssd_lstComboBox->currentItem() == 1)
    lst_convert = "  LST=`${P_HMSTODECIMAL} ${LST} | ${P_GAWK} '{print $1*3600}'`";
  else lst_convert = "";
  
  splitscriptfilename = cni_userscriptdir;
  splitscriptfilename.append("/process_split_");
  splitscriptfilename.append(ssd_instrumentname);
  splitscriptfilename.append(".sh");
  
  // check if the user provided two filter names:
  if (filter.contains(",") == 1) {
    filter1 = filter.section(',',0,0);
    filter2 = filter.section(',',1,1);
    filter  = filter1;
  }
  else if (filter.contains(",") > 1) {
    QMessageBox::information( this, "Max filters",
			      "A maximum of two comma-separated filters is allowed.\n"
			      "Please adjust your selection of FILTER keywords accordingly.\n");
    return;
  } else {
    filter1 = "";
    filter2 = "";
  }

  // check if the camera has DITs and NDITs
  if (exptime.contains(",") == 1) {
    dit  = exptime.section(',',0,0);
    ndit = exptime.section(',',1,1);
  }
  else if (exptime.contains(",") > 1) {
    QMessageBox::information( this, "Max DITs",
			      "A maximum of two comma-separated exposure keywords is allowed.\n"
			      "Please adjust your selection of the EXPTIME keyword accordingly.\n");
    return;
  } else {
    dit  = "";
    ndit = "";
  }

  // check if this file already exists. If yes, print a warning message
  int answer = 0;
  QFile script;
  script.setName(splitscriptfilename);
  QFileInfo fileinfo = QFileInfo(splitscriptfilename);
  QString filename = fileinfo.fileName();
  QString message = QString("<b>WARNING:</b><br>You are going to overwrite<br>\n");
  message.append(filename);
  message.append("  !\n");
  if ( script.exists())
    answer = QMessageBox::warning(
				  this, "Overwrite warning", message,
				  "&OK", "&Cancel", QString::null, 0, 1 );
  
  // Skip when the user cancels;
  if ( answer == 1 ) return;
  
  getkey = "`get_key ${FILE} \"";
  getkeyc = "`${P_GETKEY} ${FILE} \"";
  
  // create the CRPIX and CDij strings
  crpix1 = "";
  crpix2 = "";
  cd11 = "";
  cd12 = "";
  cd21 = "";
  cd22 = "";
  QString index = QString("");
  for (i=0; i<int_ssd_numchips;i++) {
    index = index.setNum(i+1);
    crpix1.append("${REFPIXX[");
    crpix1.append(index);
    crpix1.append("]}");
    crpix2.append("${REFPIXY[");
    crpix2.append(index);
    crpix2.append("]}");
    cd11.append("${PIXSCX}");
    cd22.append("${PIXSCY}");
    cd12.append("0");
    cd21.append("0");
    if (i<int_ssd_numchips-1) {
      crpix1.append(",");
      crpix2.append(",");
      cd11.append(",");
      cd12.append(",");
      cd21.append(",");
      cd22.append(",");
    }
  }
  
  // create the Mij matrix
  m11 = "";
  m12 = "";
  m21 = "";
  m22 = "";
  code = ssd_transformationcodesLineEdit->text();
  for (i=0; i<int_ssd_numchips; i++) {
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() == 0) {
      m11.append("1");
      m12.append("0");
      m21.append("0");
      m22.append("1");
    }
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() == 1) {
      m11.append("-1");
      m12.append("0");
      m21.append("0");
      m22.append("1");
    }
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() == 2) {
      m11.append("1");
      m12.append("0");
      m21.append("0");
      m22.append("-1");
    }
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() == 3) {
      m11.append("0");
      m12.append("-1");
      m21.append("1");
      m22.append("0");
    }
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() == 4) {
      m11.append("0");
      m12.append("1");
      m21.append("1");
      m22.append("0");
    }
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() == 5) {
      m11.append("0");
      m12.append("-1");
      m21.append("-1");
      m22.append("0");
    }
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() == 6) {
      m11.append("0");
      m12.append("1");
      m21.append("-1");
      m22.append("0");
    }
    if (code.section(" ",i,i,QString::SectionSkipEmpty).toInt() == 7) {
      m11.append("-1");
      m12.append("0");
      m21.append("0");
      m22.append("-1");
    }
    if (i<int_ssd_numchips-1) {
      m11.append(",");
      m12.append(",");
      m21.append(",");
      m22.append(",");
    }	    
  }    
  
  
  // write everything into the file
  of = fopen(splitscriptfilename, "w");
  
  fprintf(of, "%s%s%s\n\n", "#!",bash.ascii()," -xv");
  fprintf(of, "# ***************************************\n");
  fprintf(of, "# ***** INSTRUMENT splitting script *****\n");
  fprintf(of, "# *****      created by THELI       *****\n");
  fprintf(of, "# ***************************************\n\n");
  
  fprintf(of, "# splits %s images\n\n", ssd_instrumentname.ascii());
  
  fprintf(of, "# $1: main directory\n");
  fprintf(of, "# $2: science directory\n\n");
  fprintf(of, "INSTDIR=instruments_professional\n");
  fprintf(of, "if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then\n");
  fprintf(of, "    INSTDIR=instruments_commercial\n");
  fprintf(of, "fi\n");
  fprintf(of, "if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then\n");
  fprintf(of, "    INSTDIR=~/.theli/instruments_user/\n");
  fprintf(of, "fi\n");
  fprintf(of, ". ${INSTDIR}/${INSTRUMENT:?}.ini\n\n");
  fprintf(of, ". bash.include\n\n");
  
  // rename the FITS file if requested by the user
  if (monochrome_FITS_RadioButton->isChecked() || bayer_FITS_RadioButton->isChecked()) {
    fprintf(of, "\n");
    fprintf(of, "# rename the images to the value of a FITS key (if requested)\n");
    fprintf(of, "\n");
    fprintf(of, "if [ \"${V_PRE_RENAME_CHECKED}\" = \"1\" ]; then\n");
    fprintf(of, "\n");
    fprintf(of, "  cd /$1/$2\n");
    fprintf(of, "  mkdir ORIGINALS\n\n");
    fprintf(of, "\n");
    fprintf(of, "  p_ls > ${TEMPDIR}/fitskey_$$\n");
    fprintf(of, "\n");
    fprintf(of, "  cat ${TEMPDIR}/fitskey_$$ |\\\n");
    fprintf(of, "  {\n");
    fprintf(of, "    while read FILE\n");
    fprintf(of, "    do\n");
    fprintf(of, "      newname=`get_key ${FILE} ${V_RENAME_FITSKEY}`\n");
    fprintf(of, "      if [ \"${newname}\" != \"\" ]; then\n");
    fprintf(of, "         mv ${FILE} ${newname}\n");
    fprintf(of, "      else\n");
    fprintf(of, "         echo ERROR: Could not find ${V_RENAME_FITSKEY} in the FITS header\n");
    fprintf(of, "         mv ${FILE} ORIGINALS/\n");
    fprintf(of, "      fi\n");
    fprintf(of, "    done\n");
    fprintf(of, "  }\n");
    fprintf(of, "fi\n");
    fprintf(of, "\n\n");
  }

  fprintf(of, "cd /$1/$2\n\n");
  fprintf(of, "FILES=`p_ls`\n\n");
  fprintf(of, "mkdir ORIGINALS\n\n");
  
  // data in RAW format
  if (monochrome_RAW_RadioButton->isChecked() || bayer_RAW_RadioButton->isChecked() ) {
    fprintf(of, "# convert the RAW images to FITS\n\n");
    fprintf(of, "for FILE in ${FILES}\n");
    fprintf(of, "do\n");
    fprintf(of, "  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS=\".\"} {print \".\"$NF}'`\n");
    fprintf(of, "  BASE=`basename $FILE ${SUFFIX}`\n");
    fprintf(of, "  EXPTIME=`${P_DCRAW} -i -v ${FILE} | ${P_GAWK} '($1~/Shutter/) {print $2}' |\\\n");
    fprintf(of, "           ${P_GAWK} 'BEGIN{FS=\"/\"} {if (NF==2) print $1 / $2; else print $1}'`\n\n");
    fprintf(of, "  ${P_DCRAW} -c -v -4 -k 0 -S 65535 -D -t 0 -T ${FILE} > ${BASE}.tif\n\n");
    
    fprintf(of, "  ER1=ER\n");
    fprintf(of, "  ER2=ROR\n");
    fprintf(of, "  if [ ! -f ${BASE}.tif ]; then\n");
    fprintf(of, "      echo \"   ${ER1}${ER2}: Could not create TIFF from RAW!\"\n");
    fprintf(of, "      if [ ! -f /usr/lib/libtiff.so.3 ]; then\n");
    fprintf(of, "           echo \"     Possible reason: /usr/lib/libtiff.so.3 not found.\"\n");
    fprintf(of, "           echo \"     Please link /usr/lib/libtiff.so.4.x.x to /usr/lib/libtiff.so.3 with\"\n");
    fprintf(of, "  	      echo \"     sudo ln -s /usr/lib/libtiff.so.4.x.x /usr/lib/libtiff.so.3\"\n");
    fprintf(of, "      fi\n");
    fprintf(of, "      exit\n");
    fprintf(of, "  fi\n");
    
    fprintf(of, "  ${P_TIFF2FITS} -i ${BASE}.tif -o ${BASE}.fits\n");
    fprintf(of, "  rm ${BASE}.tif\n\n");
    fprintf(of, "  ${P_REPLACEKEY} ${BASE}.fits \"EXPTIME = ${EXPTIME}\" EXPTIME\n");
    fprintf(of, "  ${P_REPLACEKEY} ${BASE}.fits \"ORIGFILE= ${FILE}\" ORIGFILE\n");
    fprintf(of, "  mv ${FILE} ORIGINALS\n");
    fprintf(of, "  FILE=${BASE}.fits\n\n");
    fprintf(of, "  RA=0.001\n");
    fprintf(of, "  DEC=0.0\n");
    fprintf(of, "  EQUINOX=2000.0\n");
    fprintf(of, "  OBJECT=UNKNOWN\n");
    fprintf(of, "  MJD=52000.0\n");
    if (bayer_RAW_RadioButton->isChecked()) fprintf(of, "  FILTER=RGB\n");
    if (monochrome_RAW_RadioButton->isChecked()) fprintf(of, "  FILTER=UNKNOWN\n");
    fprintf(of, "  LST=0.0\n");
    fprintf(of, "  AIRMASS=1.0\n\n");
    fprintf(of, "  EXPTIME=%sEXPTIME\"`\n", getkey.ascii());
    fprintf(of, "  if [ \"${EXPTIME}\" = \"\" ]; then\n");
    fprintf(of, "      EXPTIME=%sEXPOSURE\"`\n", getkey.ascii());
    fprintf(of, "  fi\n");
    fprintf(of, "  if [ \"${EXPTIME}\" = \"\" ]; then\n");
    fprintf(of, "      EXPTIME=1.0\n");
    fprintf(of, "  fi\n\n");
  }
    
  // color data in FITS format: user has to define the bayer matrix type
  if (bayer_FITS_RadioButton->isChecked()) {
    if (bayer_RGGB_ToolButton->isOn()) bayerpattern = "RGGB";
    if (bayer_GBRG_ToolButton->isOn()) bayerpattern = "GBRG";
    if (bayer_GRBG_ToolButton->isOn()) bayerpattern = "GRBG";
    if (bayer_BGGR_ToolButton->isOn()) bayerpattern = "BGGR";
  }

  // data in FITS format
  if (monochrome_FITS_RadioButton->isChecked() || bayer_FITS_RadioButton->isChecked()) {
    fprintf(of, "for FILE in ${FILES}\n");
    fprintf(of, "do\n");
    fprintf(of, "  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS=\".\"} {print \".\"$NF}'`\n");
    fprintf(of, "  BASE=`basename $FILE ${SUFFIX}`\n");
    fprintf(of, "  mv ${FILE} ${BASE}.fits\n");
    fprintf(of, "  FILE=${BASE}.fits\n\n");
    
    // ra
    if (ra.isEmpty()) fprintf(of, "  RA=0.001\n");
    else {
      if (ssd_raComboBox->currentItem() <= 1)
	fprintf(of, "  RA=%s%s\"`\n", getkey.ascii(), ra.ascii());
      if (ssd_raComboBox->currentItem() == 2)
	fprintf(of, "  RA=%s%s\" %s`\n", getkeyc.ascii(), ra.ascii(), "cleancolon");
      if (!ra_convert.isEmpty()) {
	fprintf(of, "  if [ \"${RA}\" != \"\" ]; then\n");
	fprintf(of, "    %s\n", ra_convert.ascii());
	fprintf(of, "  fi\n");
      }
      fprintf(of, "  if [ \"${RA}\" = \"\" ]; then\n");
      fprintf(of, "      RA=0.001\n");
      fprintf(of, "  fi\n\n");
    }
    
    // dec
    if (dec.isEmpty()) fprintf(of, "  DEC=0.0\n");
    else {
      if (ssd_decComboBox->currentItem() <= 1)
	fprintf(of, "  DEC=%s%s\"`\n", getkey.ascii(), dec.ascii());
      if (ssd_decComboBox->currentItem() == 2)
	fprintf(of, "  DEC=%s%s\" %s`\n", getkeyc.ascii(), dec.ascii(), "cleancolon");
      if (!dec_convert.isEmpty()) {
	fprintf(of, "  if [ \"${DEC}\" != \"\" ]; then\n");
	fprintf(of, "    %s\n", dec_convert.ascii());
	fprintf(of, "  fi\n");
      }
      fprintf(of, "  if [ \"${DEC}\" = \"\" ]; then\n");
      fprintf(of, "      DEC=0.0\n");
      fprintf(of, "  fi\n\n");
    }
    
    // exptime
    if (exptime.isEmpty()) fprintf(of, "  EXPTIME=1.0\n");
    else {
      // only EXPTIME keyword is present, no DIT/NDIT
      if (dit.isEmpty()) {
	fprintf(of, "  EXPTIME=%s%s\"`\n", getkey.ascii(), exptime.ascii());
	fprintf(of, "  if [ \"${EXPTIME}\" = \"\" ]; then\n");
	if (exptime.compare("EXPTIME") == 0)
	  fprintf(of, "      EXPTIME=%sEXPOSURE\"`\n", getkey.ascii());
	if (exptime.compare("EXPOSURE") == 0)
	  fprintf(of, "      EXPTIME=%sEXPTIME\"`\n", getkey.ascii());
	fprintf(of, "  fi\n");
	fprintf(of, "  if [ \"${EXPTIME}\" = \"\" ]; then\n");
	fprintf(of, "      EXPTIME=1.0\n");
	fprintf(of, "  fi\n\n");
      }
      // DIT and NDITs were given
      else {
	fprintf(of, "  DIT=%s%s\"`\n",  getkey.ascii(),  dit.ascii());
	fprintf(of, "  NDIT=%s%s\"`\n", getkey.ascii(), ndit.ascii());
	fprintf(of, "  EXPTIME=`echo ${DIT} ${NDIT} | ${P_GAWK} '{print $1*$2}'`\n");
	fprintf(of, "  if [ \"${EXPTIME}\" = \"\" ]; then\n");
	fprintf(of, "      EXPTIME=1.0\n");
	fprintf(of, "  fi\n\n");
      }
    }
    
    // object
    fprintf(of, "  OBJECT=%s%s\"`\n", getkey.ascii(), object.ascii());
    fprintf(of, "  if [ \"${OBJECT}\" = \"\" ]; then\n");
    fprintf(of, "      OBJECT=UNKNOWN\n");
    fprintf(of, "  fi\n\n");
    
    // equinox
    if (equinox.isEmpty()) fprintf(of, "  EQUINOX=2000.0\n");
    else {
      fprintf(of, "  EQUINOX=%s%s\" | sed 's/[^0-9.]//g'`\n", getkey.ascii(), equinox.ascii());
      fprintf(of, "  if [ \"${EQUINOX}\" = \"\" ]; then\n");
      fprintf(of, "      EQUINOX=2000.0\n");
      fprintf(of, "  fi\n\n");
    }	
    
    // mjd
    if (mjd.isEmpty()) fprintf(of, "  MJD=52000.0\n");
    else {
      fprintf(of, "  MJD=%s%s\"`\n", getkey.ascii(), mjd.ascii());
      fprintf(of, "  if [ \"${MJD}\" = \"\" ]; then\n");
      fprintf(of, "      MJD=52000.0\n");
      fprintf(of, "  fi\n\n");
    }
    
    // filter
    if (monochrome_FITS_RadioButton->isChecked()) {
      if (filter.isEmpty()) fprintf(of, "  FILTER=UNKNOWN\n");
      else {
	if (filter1.isEmpty() && filter2.isEmpty()) {
	  fprintf(of, "  FILTER=%s%s\" | ${P_CLEANSTRING}`\n", getkey.ascii(), filter.ascii());
	  fprintf(of, "  if [ \"${FILTER}\" = \"\" ]; then\n");
	  fprintf(of, "      FILTER=UNKNOWN\n");
	  fprintf(of, "  fi\n\n");
	}
	else {
	  fprintf(of, "  FILTER1=%s%s\" | ${P_CLEANSTRING}`\n", getkey.ascii(), filter1.ascii());
	  fprintf(of, "  FILTER2=%s%s\" | ${P_CLEANSTRING}`\n", getkey.ascii(), filter2.ascii());
	  fprintf(of, "  FILTER=${FILTER1}+${FILTER2}\n");
	  fprintf(of, "  if [ \"${FILTER}\" = \"\" ]; then\n");
	  fprintf(of, "      FILTER=UNKNOWN\n");
	  fprintf(of, "  fi\n\n");
	}
      }
    }
    else {
      if (filter.isEmpty()) fprintf(of, "  FILTER=RGB\n");
      else {
	fprintf(of, "  FILTER=%s%s\" | ${P_CLEANSTRING}`\n", getkey.ascii(), filter.ascii());
	fprintf(of, "  if [ \"${FILTER}\" = \"\" ]; then\n");
	fprintf(of, "      FILTER=RGB\n");
	fprintf(of, "  else\n");
	fprintf(of, "      FILTER=RGB_${FILTER}\n");
	fprintf(of, "  fi\n\n");
      }
    }
    
    // airmass + lst
    if (airmass.isEmpty() && lst.isEmpty()) {
      fprintf(of, "  LST=0.0\n");
      fprintf(of, "  AIRMASS=1.0\n");
    }
    if (airmass.isEmpty() && !lst.isEmpty()) {
      fprintf(of, "  LST=%s%s\"`\n", getkey.ascii(), lst.ascii());
      fprintf(of, "  if [ \"${LST}\" = \"\" ]; then\n");
      fprintf(of, "      LST=0.0\n");
      fprintf(of, "  fi\n\n");
      if (!lst_convert.isEmpty()) fprintf(of, "%s\n\n", lst_convert.ascii());
      fprintf(of, "  AIRMASS=`${P_AIRMASS} -t ${LST} -e ${EXPTIME} -r ${RA} -d ${DEC} -l ${OBSLAT}`\n");
    }
    if (!airmass.isEmpty() && !lst.isEmpty()) {
      fprintf(of, "  LST=%s%s\"`\n", getkey.ascii(), lst.ascii());
      fprintf(of, "  if [ \"${LST}\" = \"\" ]; then\n");
      fprintf(of, "      LST=0.0\n");
      fprintf(of, "  fi\n\n");
      fprintf(of, "  AIRMASS=%s%s\"`\n", getkey.ascii(), airmass.ascii());
    }
    if (!airmass.isEmpty() && lst.isEmpty()) {
      fprintf(of, "  LST=0.0\n");
      fprintf(of, "  AIRMASS=%s%s\" | ${P_GAWK} '($1!=\"FILE\") {print $2}'`\n", 
	      getkey.ascii(), airmass.ascii());
    }
    fprintf(of, "  if [ \"${AIRMASS}\" = \"\" ]; then\n");
    fprintf(of, "      AIRMASS=1.0\n");
    fprintf(of, "  fi\n\n");
  }	
  
  // these lines are always printed no matter if a normal ccd or a colour ccd is used
  fprintf(of, "  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\\\n");
  fprintf(of, "      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS=\".\"} {print $1}'`\n\n");
  
  // preserve the CD matrix if present
  if (ssd_preserveCDCheckBox->isChecked()) {
    fprintf(of, "  # Preserve the CD matrix if present.\n");
    fprintf(of, "  # If this does not work, replace the 'get_wcs' function below\n");
    fprintf(of, "  # with the explicit code in 'bash.include', and then modify it\n");
    fprintf(of, "  # according to your needs.\n");
    fprintf(of, "  get_wcs\n\n");
    fprintf(of, "  ${P_FITSSPLIT_ECL} \\\n");
    fprintf(of, "     -CRPIX1 \"${cpx1}\" \\\n");
    fprintf(of, "     -CRPIX2 \"${cpx2}\" \\\n");
    fprintf(of, "     -CD11 \"${cd11}\" \\\n");
    fprintf(of, "     -CD12 \"${cd12}\" \\\n");
    fprintf(of, "     -CD21 \"${cd21}\" \\\n");
    fprintf(of, "     -CD22 \"${cd22}\" \\\n");
    fprintf(of, "     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \\\n");
    fprintf(of, "     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \\\n");
    fprintf(of, "     -M11 \"%s\" \\\n", m11.ascii());
    fprintf(of, "     -M12 \"%s\" \\\n", m12.ascii());
    fprintf(of, "     -M21 \"%s\" \\\n", m21.ascii());
    fprintf(of, "     -M22 \"%s\" \\\n", m22.ascii());
    fprintf(of, "     ${FILE}\n\n");
  }
  else {
    fprintf(of, "  ${P_FITSSPLIT_ECL} \\\n");
    fprintf(of, "     -CRPIX1 \"%s\" \\\n", crpix1.ascii());
    fprintf(of, "     -CRPIX2 \"%s\" \\\n", crpix2.ascii());
    fprintf(of, "     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \\\n");
    fprintf(of, "     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \\\n");
    fprintf(of, "     -CD11 \"%s\" \\\n", cd11.ascii());
    fprintf(of, "     -CD12 \"%s\" \\\n", cd12.ascii());
    fprintf(of, "     -CD21 \"%s\" \\\n", cd21.ascii());
    fprintf(of, "     -CD22 \"%s\" \\\n", cd22.ascii());
    fprintf(of, "     -M11 \"%s\" \\\n", m11.ascii());
    fprintf(of, "     -M12 \"%s\" \\\n", m12.ascii());
    fprintf(of, "     -M21 \"%s\" \\\n", m21.ascii());
    fprintf(of, "     -M22 \"%s\" \\\n", m22.ascii());
    fprintf(of, "     ${FILE}\n\n");
  }
  
  // get and write optional FITS keywords
  // the THELI names
  dummy1 = ssd_dummy1LineEdit->text();
  dummy2 = ssd_dummy2LineEdit->text();
  dummy3 = ssd_dummy3LineEdit->text();
  dummy4 = ssd_dummy4LineEdit->text();
  dummy5 = ssd_dummy5LineEdit->text();
  dummy6 = ssd_dummy6LineEdit->text();
  dummy7 = ssd_dummy7LineEdit->text();
  dummy8 = ssd_dummy8LineEdit->text();
  // the raw names 
  rawdummy1 = ssd_rawdummy1LineEdit->text();
  rawdummy2 = ssd_rawdummy2LineEdit->text();
  rawdummy3 = ssd_rawdummy3LineEdit->text();
  rawdummy4 = ssd_rawdummy4LineEdit->text();
  rawdummy5 = ssd_rawdummy5LineEdit->text();
  rawdummy6 = ssd_rawdummy6LineEdit->text();
  rawdummy7 = ssd_rawdummy7LineEdit->text();
  rawdummy8 = ssd_rawdummy8LineEdit->text();
  
  if (!rawdummy1.isEmpty() || !rawdummy2.isEmpty() || !rawdummy3.isEmpty() ||
      !rawdummy4.isEmpty() || !rawdummy5.isEmpty() || !rawdummy6.isEmpty() ||
      !rawdummy7.isEmpty() || !rawdummy8.isEmpty()) {
    
    fprintf(of, "  k=1\n");
    fprintf(of, "  while [ \"${k}\" -le \"${NCHIPS}\" ]\n");
    fprintf(of, "  do\n");
    insert_replacekey(of, dummy1, rawdummy1, "DUMMY1");
    insert_replacekey(of, dummy2, rawdummy2, "DUMMY2");
    insert_replacekey(of, dummy3, rawdummy3, "DUMMY3");
    insert_replacekey(of, dummy4, rawdummy4, "DUMMY4");
    insert_replacekey(of, dummy5, rawdummy5, "DUMMY5");
    insert_replacekey(of, dummy6, rawdummy6, "DUMMY6");
    insert_replacekey(of, dummy7, rawdummy7, "DUMMY7");
    insert_replacekey(of, dummy8, rawdummy8, "DUMMY8");

    // if NDIT > 1
    if (!ndit.isEmpty()) {
      fprintf(of, "\n");
      fprintf(of, "    # scale the images such that they reflect the total exposure time.\n");
      fprintf(of, "    # otherwise we get problems with the weights when combining images\n");
      fprintf(of, "    # with different DIT/NDIT settings\n");
      fprintf(of, "    if [ \"${NDIT}\" -gt \"1\" ]; then\n");
      fprintf(of, "        ${P_IC} '%c1 '${NDIT}' *' ${BASE}_${k}.fits > ${BASE}_${k}.fits2\n", 37);
      fprintf(of, "        mv ${BASE}_${k}.fits2 ${BASE}_${k}.fits\n");
      fprintf(of, "    fi\n");
    }
    fprintf(of, "    k=$(( $k + 1 ))\n");
    fprintf(of, "  done\n\n");
  }
  
  // if RAW data is present, get the ISO speed and the date
  if (monochrome_RAW_RadioButton->isChecked() || bayer_RAW_RadioButton->isChecked()) {
    fprintf(of, "  ORIGFILE=%sORIGFILE\"`\n", getkey.ascii());
    fprintf(of, "  ISOSPEED=`${P_DCRAW} -i -v ORIGINALS/${ORIGFILE} | \\\n");
    fprintf(of, "            ${P_GAWK} 'BEGIN {FS=\":\"} {if ($1~/ISO/) print $2}'`\n");
    if (bayer_RAW_RadioButton->isChecked()) {
      fprintf(of, "  BAYERPAT=`${P_DCRAW} -i -v ORIGINALS/${ORIGFILE} |\n");
      fprintf(of, "            ${P_GAWK} '($0~/Filter pattern/) {print $3}' | sed 's/\\///g'`\n");
    }
    fprintf(of, "  DATEOBS=`${P_DCRAW} -i -v ORIGINALS/${ORIGFILE} | ${P_GAWK} '($1~/Timestamp/) {\n");
    fprintf(of, "      if ($3==\"Jan\") month=\"01\"\n");
    fprintf(of, "      if ($3==\"Feb\") month=\"02\"\n");
    fprintf(of, "      if ($3==\"Mar\") month=\"03\"\n");
    fprintf(of, "      if ($3==\"Apr\") month=\"04\"\n");
    fprintf(of, "      if ($3==\"May\") month=\"05\"\n");
    fprintf(of, "      if ($3==\"Jun\") month=\"06\"\n");
    fprintf(of, "      if ($3==\"Jul\") month=\"07\"\n");
    fprintf(of, "      if ($3==\"Aug\") month=\"08\"\n");
    fprintf(of, "      if ($3==\"Sep\") month=\"09\"\n");
    fprintf(of, "      if ($3==\"Oct\") month=\"10\"\n");
    fprintf(of, "      if ($3==\"Nov\") month=\"11\"\n");
    fprintf(of, "      if ($3==\"Dec\") month=\"12\"\n");
    fprintf(of, "      print $6\"-\"month\"-\"$4\"T\"$5}'`\n\n");
    fprintf(of, "  k=1\n");
    fprintf(of, "  while [ \"${k}\" -le \"${NCHIPS}\" ]\n");
    fprintf(of, "  do\n");
    fprintf(of, "    ${P_REPLACEKEY} ${BASE}_${k}.fits \"ISOSPEED= ${ISOSPEED}\" DUMMY9\n");
    fprintf(of, "    ${P_REPLACEKEY} ${BASE}_${k}.fits \"DATE-OBS= \'${DATEOBS}\'\" DATE-OBS\n");
    // if a Bayer matrix is present:
    if (bayer_RAW_RadioButton->isChecked()) { 
      fprintf(of, "    ${P_REPLACEKEY} ${BASE}_${k}.fits \"BAYERPAT= \'${BAYERPAT}\'\" DUMMY10\n");
    }
    fprintf(of, "    k=$(( $k + 1 ))\n");
    fprintf(of, "  done\n\n");
  }
  
  if (monochrome_FITS_RadioButton->isChecked()) {	
    fprintf(of, "  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}\n\n");
  }
  
  if (monochrome_RAW_RadioButton->isChecked() || bayer_RAW_RadioButton->isChecked()) {	
    fprintf(of, "  \\rm ${FILE}\n\n");
  }
  
  if (bayer_FITS_RadioButton->isChecked()) {	
    fprintf(of, "  k=1\n");
    fprintf(of, "  while [ \"${k}\" -le \"${NCHIPS}\" ]\n");
    fprintf(of, "  do\n");
    fprintf(of, "    ${P_REPLACEKEY} ${BASE}_${k}.fits \"BAYERPAT= \'%s\'\" DUMMY10\n", bayerpattern.ascii());
    fprintf(of, "    k=$(( $k + 1 ))\n");
    fprintf(of, "  done\n\n");
    fprintf(of, "  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}\n\n");
  }
  
  fprintf(of, "done\n\n");
  
  fclose(of);
  
  // make the file executable
  execstr = "chmod +x ";
  execstr.append(splitscriptfilename);
  my_system(execstr);
  
  // change the text label of the pushbutton for a short while
  ssd_createscriptPushButton->setPaletteForegroundColor(QColor(0,155,0));
  ssd_createscriptPushButton->setText("Done");
  QTimer *timer = new QTimer();
  connect( timer, SIGNAL(timeout()), SLOT(timerDone()) );
  timer->start( 1000, TRUE );
}


void cni_form::timerDone()
{
  ssd_createscriptPushButton->setPaletteForegroundColor(QColor(0,0,0));
  ssd_createscriptPushButton->setText("Create script");
}


void cni_form::insert_replacekey(FILE *output, QString theliheader, 
					  QString rawdummy, QString dummykey)
{
  int rawlen, thelilen;
  QString getkey;

  getkey = "`get_key ${FILE} \"";
  
  if (!rawdummy.isEmpty()) {
    // copy (and truncate) if necessary
    rawlen = rawdummy.length();
    // if no theliheader is given
    if (rawlen >= 9 && theliheader.isEmpty()) {
      theliheader = rawdummy.left(8);
      theliheader.append("=");
    }
    if (rawlen < 9 && theliheader.isEmpty()) {
      theliheader = rawdummy;
      thelilen = theliheader.length();
      while (thelilen < 8) {
	theliheader.append(" ");
	thelilen = theliheader.length();
      }
      if (theliheader.length() == 8) theliheader.append("=");
    }
    // if a theliheader is given
    if (!theliheader.isEmpty()) {
      thelilen = theliheader.length();
      while (thelilen < 8) {
	theliheader.append(" ");
	thelilen = theliheader.length();
      }
      if (theliheader.length() == 8) theliheader.append("=");
    }
    
    // write it
    fprintf(output, "    tmp=%s%s\"`\n", getkey.ascii(), rawdummy.ascii());
    fprintf(output, "    ${P_REPLACEKEY} ${BASE}_${k}.fits \"%s ${tmp}\" %s\n", 
	    theliheader.ascii(), dummykey.ascii());
  }
}


void cni_form::guess_camerageometry()
{
  // guess the camera geometry based on a flatfield with medium
  // saturation level

  FILE *pipe;
  int nchips_int, loop, i;
  QString flatfield, execstr, nchips, line;
  QString naxis1, naxis2;
  QString cutonx, cutony, sizex, sizey, crpix1, crpix2, ovscanx1, ovscanx2;
  QString fcutonx, fcutony, fsizex, fsizey, fcrpix1, fcrpix2, fovscanx1, fovscanx2;
  char s1[10], s2[10], s3[10], s4[10], s5[10], s6[10], s7[10], s8[10];

  flatfield = QFileDialog::getOpenFileName(
	  cni_userinstdir,
	  "FITS (*.FITS *.FIT *.FTS *.fits *.fit *.fts);; All (*.*)",
	  this,
	  "open file dialog",
	  "Select a flat field");

  if ( flatfield.isEmpty() || flatfield.length() <= 4)
    return;


  // check if the image is in DSLR RAW format

  QString format;

  execstr = cni_bindir+"/dcraw -i " + flatfield + " > " + cni_tmpdir + "/fileformat 2>&1";
  my_system(execstr); 

  execstr = "cat " + cni_tmpdir + "/fileformat";
  my_fscanf(execstr, format);

  if (format.contains("Cannot") > 0) {
    // assume we have a fits file

    // the number of chips in the camera
    execstr = cni_bindir + "/fitsguessgeom -i " + flatfield + " -n";
    my_fscanf(execstr, nchips);
    if (nchips.compare("") == 0 || nchips.contains("ERROR") > 0) {
      QMessageBox::warning( this, "Guessing chip geometries",
			    "The image selected does not appear to be a valid FITS file.\n");
      return;
    }
    nchips_int = nchips.toInt();
    if (nchips_int == 0) cni_nccdSpinBox->setValue(1);
    else cni_nccdSpinBox->setValue(nchips_int);
    
    // the remaining camera parameters
    loop = nchips_int;
    if (loop == 0) loop = 1;
    
    fovscanx1 = ""; 
    fovscanx2 = "";
    fcutonx   = ""; 
    fcutony   = ""; 
    fsizex    = ""; 
    fsizey    = ""; 
    fcrpix1   = ""; 
    fcrpix2   = ""; 
    
    for (i=1; i<=loop; i++) {
      // single-chip camera
      if (nchips_int == 0)
	execstr = cni_bindir + "/fitsguessgeom -i " + flatfield;
      // multi-chip camera
      else {
	nchips.setNum(i);
	execstr = cni_bindir + "/fitsguessgeom -i " + flatfield + " -x " + nchips;
      }
      
      pipe = popen(execstr, "r");
      while ( !feof(pipe )) {
	if (fscanf(pipe, "%s %s %s %s %s %s %s %s", s1, s2, s3, s4, s5, s6, s7, s8) == 0) {
	  QMessageBox::warning( this, "Guessing chip geometries",
				"The image selected does not appear to be a valid FITS file.\n");
	  return;
	}
      }
      pclose(pipe);
    
      cutonx = s1;
      sizex  = s2;
      cutony = s3;
      sizey  = s4;
      naxis1 = s5;
      naxis2 = s6;
      crpix1 = s7;
      crpix2 = s8;
      if (cutonx.toInt() > 5) {
	ovscanx1 = "2";
	ovscanx2 = ovscanx2.setNum(cutonx.toInt()-2);
      }
      else {
	ovscanx1 = "0";
	ovscanx2 = "0";
      }
      
      // prepare the rest
      fovscanx1 = fovscanx1 + ovscanx1 + " "; 
      fovscanx2 = fovscanx2 + ovscanx2 + " ";
      fcutonx   = fcutonx + cutonx + " "; 
      fcutony   = fcutony + cutony + " "; 
      fsizex    = fsizex + sizex + " "; 
      fsizey    = fsizey + sizey + " "; 
      fcrpix1   = fcrpix1 + crpix1 +" "; 
      fcrpix2   = fcrpix2 + crpix2 + " "; 
    }
    // fill in the rest
    cni_ovminLineEdit->setText(fovscanx1);
    cni_ovmaxLineEdit->setText(fovscanx2);
    cni_cutonxLineEdit->setText(fcutonx);
    cni_cutonyLineEdit->setText(fcutony);
    cni_xsizeLineEdit->setText(fsizex);
    cni_ysizeLineEdit->setText(fsizey); 
    cni_crpix1LineEdit->setText(fcrpix1);
    cni_crpix2LineEdit->setText(fcrpix2);
  }
  else {
    // must be a RAW file
    execstr = cni_bindir + "/dcraw -i -v " + flatfield + " | gawk '($0~/Image size:/) {print $3, $5}'";
    my_fscanf2(execstr, naxis1, naxis2);

    cni_nccdSpinBox->setValue(1);    

    crpix1 = crpix1.setNum(naxis1.toInt()/2);
    crpix2 = crpix2.setNum(naxis2.toInt()/2);

    cni_ovminLineEdit->setText("0");
    cni_ovmaxLineEdit->setText("0");
    cni_cutonxLineEdit->setText("1");
    cni_cutonyLineEdit->setText("1");
    cni_xsizeLineEdit->setText(naxis1);
    cni_ysizeLineEdit->setText(naxis2); 
    cni_crpix1LineEdit->setText(crpix1);
    cni_crpix2LineEdit->setText(crpix2);
  }
}
