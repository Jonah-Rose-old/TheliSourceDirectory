#include "functions.h"
#include "cni_form.h"
#include "parameters.h"
#include "color_form.h"
#include "commhist.h"
#include "globalvariables.h"
#include "logfile.h"
#include "section_pre.h"
#include "section_cal.h"
#include "section_sf.h"
#include "section_w.h"
#include "section_ap.h"
#include "section_cc.h"
#include "combineimages.h"
#include "statistics.h"
#include "zp.h"
#include "manualsky.h"
#include "imalyzer.h"
#include "animate.h"
#include "pmonitor.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// #include "sys/sysinfo.h"   uncomment if the sysconf() function isn't available
#include <sys/statvfs.h> // for statvfs
#include <errno.h>

#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qtextstream.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qlistbox.h>
#include <qsizegrip.h>
#include <qvalidator.h>
#include <qstring.h>
#include <qprocess.h>
#include <qtextedit.h>
#include <qfont.h>
#include <qstringlist.h>
#include <qstylefactory.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include <qfontdialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmenudata.h>
#include <qaction.h>
#include <qstatusbar.h>
#include <qsize.h>
#include <qwhatsthis.h>
#include <qsettings.h>


int proc_ap_restoreheader_started, proc_ap_zeroheader_started;
QProcess *proc_ap_restoreheader, *proc_ap_zeroheader;

void theliForm::versioncheck(int showversionwarning)
{
  QString gv, sv;
  int gvint, svint, answer;

  gv = guiversion;
  sv = guiserverversion;

  gv.replace(".","");
  sv.replace(".","");

  gvint = gv.toInt();
  svint = sv.toInt();

  if (gvint < svint && showversionwarning == 1) {
    answer = QMessageBox::warning(
				  this, "Update available",
				  QString( "<b>OUTDATED version:</b> \n\nA newer THELI version than the one you are using is available\n. Please update to the latest version and retry.\n\n" ),
				  "&OK", "&Do not show this message again during this session", QString::null, 0, 1 );
  }

  if (answer == 1) showversionwarning = 1;
  else showversionwarning = 0;
}


void theliForm::changestatus_set_imextension()
{
  QString tmp, sciencestring2;
  QStringList status;
  QSettings settings (QSettings::Ini);
  
  //  get the log file name (lastlog)
  settings.beginGroup("/theligeneral_");
  QString reductionlog = settings.readEntry("/lastlog", "noname");
  settings.endGroup();
  
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  sciencestring2 = "";
  
  if (statusCPSDebiasingAction->isOn()) {
    sciencestring2.append("1");
    settings.writeEntry("/CALIBRATION/processsciencescript", 1);
    ext_img[0] = 1;
    CB_processscience = 1;
  }
  else {
    sciencestring2.append("0");
    settings.writeEntry("/CALIBRATION/processsciencescript", 0);
    ext_img[0] = 0;
    CB_processscience = 0;
  }
  if (statusCPSBackgroundAction->isOn()) {
    sciencestring2.append("1");
    settings.writeEntry("/BACKGROUND/backgroundscript", 1);
    ext_img[1] = 1;
    CB_background = 1;
  }
  else {
    sciencestring2.append("0");
    settings.writeEntry("/BACKGROUND/backgroundscript", 0);
    ext_img[1] = 0;
    CB_background = 0;
  }
  if (statusCPSChopnodAction->isOn()) {
    sciencestring2.append("1");
    settings.writeEntry("/BACKGROUND/chopnodscript", 1);
    ext_img[2] = 1;
    CB_chopnod = 1;
  }
  else {
    sciencestring2.append("0");
    settings.writeEntry("/BACKGROUND/chopnodscript", 0);
    ext_img[2] = 0;
    CB_chopnod = 0;
  }
  if (statusCPSCollapseAction->isOn()) {
    sciencestring2.append("1");
    settings.writeEntry("/BACKGROUND/collapsescript", 1);
    ext_img[3] = 1;
    CB_collapse = 1;
  }
  else {
    sciencestring2.append("0");
    settings.writeEntry("/BACKGROUND/collapsescript", 0);
    ext_img[3] = 0;
    CB_collapse = 0;
  }
  if (statusCPSDebloomingAction->isOn()) {
    sciencestring2.append("1");
    settings.writeEntry("/WEIGHTING/debloomscript", 1);
    ext_img[4] = 1;
    CB_debloom = 1;
  }
  else {
    sciencestring2.append("0");
    settings.writeEntry("/WEIGHTING/debloomscript", 0);
    ext_img[4] = 0;
    CB_debloom = 0;
  }
  if (statusCPSDirectphotAction->isOn()) {
    sciencestring2.append("1");
    settings.writeEntry("/ASTROMPHOTOM/absphotomdirectscript", 1);
    ext_img[5] = 1;
    CB_directphot = 1;
  }
  else {
    sciencestring2.append("0");
    settings.writeEntry("/ASTROMPHOTOM/absphotomdirectscript", 0);
    ext_img[5] = 0;
    CB_directphot = 0;
  }
  
  paint_checkboxes();
  
  settings.writeEntry("/PROCESSINGSTATUS/status", sciencestring2);
  
  settings.endGroup();
}


void theliForm::open_pipelinepaper()
{
  QString execstr;
  QString document;
  
  document = MAINGUIPATH + "gui/doc/pipelinepaper.pdf &";
  
  if (pdfacroread->isOn()) execstr = "acroread " + document;
  if (pdfgvv->isOn()) execstr = "gvv " + document;
  if (pdfxpdf->isOn()) execstr = "xpdf " + document;
  
  my_system(execstr);  
  
  my_system(execstr);  
}


void theliForm::changestatus_obtain_imextension()
{
  //  int extimg[8];
  int i;
  QString line, value, ident, tmp, filename;
  QStringList status;
  QSettings settings (QSettings::Ini);
  
  //  get the log file name (lastlog)
  settings.beginGroup("/theligeneral_");
  QString reductionlog = settings.readEntry("/lastlog", "noname");
  settings.endGroup();
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  value = settings.readEntry("/PROCESSINGSTATUS/status");

  status = status.split( " ", value );
  for (i = 0; i < 6; i++) {
    tmp = value.at(i);
    ext_img[i] = tmp.toInt();
    //    printf("debug: %d %d \n", extimg[i], ext_img[i]);
  }

  if (ext_img[0] == 1) statusCPSDebiasingAction->setOn(true);
  else statusCPSDebiasingAction->setOn(false);
  if (ext_img[1] == 1) statusCPSBackgroundAction->setOn(true);
  else statusCPSBackgroundAction->setOn(false);
  if (ext_img[2] == 1) statusCPSChopnodAction->setOn(true);
  else statusCPSChopnodAction->setOn(false);
  if (ext_img[3] == 1) statusCPSCollapseAction->setOn(true);
  else statusCPSCollapseAction->setOn(false);
  if (ext_img[4] == 1) statusCPSDebloomingAction->setOn(true);
  else statusCPSDebloomingAction->setOn(false);
  if (ext_img[5] == 1) statusCPSDirectphotAction->setOn(true);
  else statusCPSDirectphotAction->setOn(false);
}


void theliForm::flush_tempdir()
{
  QString delcommand;
  QDir dir2;
  
  check_last_char(homedir);
  
  delcommand = "\\rm -f ";
  delcommand += usertmpdir;
  delcommand += "*";
  dir2.setPath(usertmpdir);
  if (dir2.exists()) my_system(delcommand);
}


void theliForm::flush_tempdir_updatespace()
{
  QString execstr, menutext, tooltiptext, space3;
  QString space;
  long space2;
  
  // do nothing if the dir does not exist
  check_last_char(homedir);
  if (checkdir2(homedir) != 1) return;
  
  execstr = "du -k ";
  execstr += usertmpdir;
  execstr += "| gawk 'BEGIN {s=0} {s=$1} END {print s}'";
  execstr += " 2> /dev/null";
  
  my_fgets(execstr, space);
  
  space2 = atol(space)/1024;
  space3.setNum(space2);
  
  menutext = "Flush tmp dir (";
  menutext += space3;
  menutext += " MB)";
  deleteFlush_tmp_dirAction->setMenuText(menutext);
  
  tooltiptext = "Deletes the temporary data in ";
  tooltiptext += usertmpdir;
  deleteFlush_tmp_dirAction->setToolTip(tooltiptext);
}


void theliForm::delete_data_helper(QLineEdit *datadir,
                                   QString subdir,
                                   QString extension)
{
  QString mainpath, cleandir, deldir, call;
  QString fullpath, scripts;
  
  started_proc_del = 0;
  mainpath = in_maindirLineEdit->text();
  check_last_char(mainpath);
  deldir = datadir->text();
  cleandir = mainpath;
  cleandir.append(deldir);
  check_last_char(cleandir);
  if (!subdir.isEmpty()) cleandir.append(subdir);
  
  if (checkdir2(cleandir) == 1) {
    call = "./cleanfiles.sh ";
    call.append(cleandir);
    call.append(" ");
    call.append("\"\"");
    call.append(" ");
    call.append(extension);
    
    fullpath = userscriptdir;
    fullpath.append("/");
    fullpath.append("theli_delete_comm.sh");
    scripts = call;
    scripts.append("\n");
    if (!subdir.isEmpty()) {
      scripts.append("rm -rf ");
      scripts.append(cleandir);
    }
    // write the superscript and execute it
    process_group = "del";
    if (checkdir2(userscriptdir) == 1 && !scripts.isEmpty())
      write_superscriptbody(fullpath, scripts);
  }
}


void theliForm::delete_MASK_IMAGES()
{
  delete_data_helper(in_sciencedirLineEdit, "MASK_IMAGES", "OFC.mask.");
  delete_data_helper(in_skydirLineEdit, "MASK_IMAGES", "OFC.mask.");
  delete_data_helper(in_standarddirLineEdit, "MASK_IMAGES", "OFC.mask.");
}


void theliForm::delete_SPLIT_IMAGES()
{
  delete_data_helper(in_sciencedirLineEdit, "SPLIT_IMAGES", ".");
  delete_data_helper(in_skydirLineEdit, "SPLIT_IMAGES", ".");
  delete_data_helper(in_standarddirLineEdit, "SPLIT_IMAGES", ".");
}


void theliForm::delete_OFC_IMAGES()
{
  delete_data_helper(in_sciencedirLineEdit, "OFC_IMAGES", "OFC.");
  delete_data_helper(in_skydirLineEdit, "OFC_IMAGES", "OFC.");
  delete_data_helper(in_standarddirLineEdit, "OFC_IMAGES", "OFC.");
}


void theliForm::delete_OFCB_IMAGES()
{
  delete_data_helper(in_sciencedirLineEdit, "OFCB_IMAGES", "OFCB.");
  delete_data_helper(in_skydirLineEdit, "OFCB_IMAGES", "OFCB.");
  delete_data_helper(in_standarddirLineEdit, "OFCB_IMAGES", "OFCB.");
}


void theliForm::delete_OFCH_IMAGES()
{
  delete_data_helper(in_sciencedirLineEdit,"OFCH_IMAGES", "OFCH.");
  delete_data_helper(in_skydirLineEdit, "OFCH_IMAGES", "OFCH.");
  delete_data_helper(in_standarddirLineEdit, "OFCH_IMAGES", "OFCH.");
}


void theliForm::delete_undebloomed()
{
  QString extension;
  
  get_fitsext_fordebloom(ext_img, ext_imgch);
  extension = ext_imgch;
  extension.append("_IMAGES");
  ext_imgch.append(".");
  delete_data_helper(in_sciencedirLineEdit, extension, ext_imgch);
  delete_data_helper(in_skydirLineEdit, extension, ext_imgch);
  delete_data_helper(in_standarddirLineEdit, extension, ext_imgch);
}


void theliForm::delete_beforedirectphot()
{
  QString extension;
  get_fitsext_forweights(ext_img,ext_imgch);
  if (invoked_fromexecall == 1) get_fitsext_fromcheckboxes("forweights");
  extension = ext_imgch;
  extension.append("_IMAGES");
  ext_imgch.append(".");
  delete_data_helper(in_sciencedirLineEdit, extension, ext_imgch);
  delete_data_helper(in_skydirLineEdit, extension, ext_imgch);
  delete_data_helper(in_standarddirLineEdit, extension, ext_imgch);
}


void theliForm::delete_sub()
{
  QString deleteextension;
  get_fitsext_forastromcoadd(ext_img, ext_imgch);
  deleteextension = ext_imgch;
  deleteextension.append(".sub.");
  delete_data_helper(in_sciencedirLineEdit, "", deleteextension);
  delete_data_helper(in_skydirLineEdit, "", deleteextension);
  delete_data_helper(in_standarddirLineEdit, "", deleteextension);
}


void theliForm::delete_resampled()
{
  QString deleteextension, ident, subdir;
  
  QString reductionlog = in_reductionlogLineEdit->text();
  
  QSettings settings (QSettings::Ini);
  settings.beginGroup("/theli_"+reductionlog+"_");
  settings.beginGroup("/COADDITION");
  settings.beginGroup("/coaddition");
  ident = settings.readEntry("/scriptid", "");
  settings.endGroup();
  settings.endGroup();
  settings.endGroup();
  
  get_fitsext_forastromcoadd(ext_img, ext_imgch);
  deleteextension = ext_imgch;
  deleteextension.append(".");
  deleteextension.append(ident);
  deleteextension.append(".");
  deleteextension.append("resamp.");
  subdir = "coadd_";
  subdir.append(ident);
  delete_data_helper(in_sciencedirLineEdit, subdir, deleteextension);
  delete_data_helper(in_skydirLineEdit, subdir, deleteextension);
  delete_data_helper(in_standarddirLineEdit, subdir, deleteextension);
  
  deleteextension.append("weight.");
  delete_data_helper(in_sciencedirLineEdit, subdir, deleteextension);
  delete_data_helper(in_skydirLineEdit, subdir, deleteextension);
  delete_data_helper(in_standarddirLineEdit, subdir, deleteextension);
}


void theliForm::style_pushbuttonclicked()
{
  QSettings settings (QSettings::Ini);

  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) reductionlog = "noname";
  
  GUIstyle = 0;
  
  // cycle through the available themes
  GUIstyle = atoi( tf_styleSpinBox->cleanText());
  qApp->setStyle( styles[ GUIstyle - 1] );

  settings.beginGroup("/theli_"+reductionlog+"_");
    settings.beginGroup("/theliform_settings");
      settings.writeEntry("/TF_STYLE", tf_styleSpinBox->cleanText());
    settings.endGroup();
  settings.endGroup();

  // not sure why i needed this one ...
  //apply_logfile();
}


void theliForm::font_pushbuttonclicked()
{
  // set the font the user selected
  bool ok;
  guifont = QFontDialog::getFont( &ok, QFont( "Times", 10 ), this );
  if ( ok ) {
    qApp->setFont( guifont, true );
  }
  update_logfile();
}


void theliForm::show_software_version()
{
  QMessageBox::information( this, "About THELI",
			    "THELI GUI version: v"+guiversion+"\n"
			    "Licensed under the terms of the GNU GPL.\n\n"
			    "Author: Mischa Schirmer\n");
}


void theliForm::cleardirs_initialisepushbutton()
{
  in_maindirLineEdit->clear();
  in_biasdirLineEdit->clear();
  in_darkdirLineEdit->clear();
  in_flatdirLineEdit->clear();
  in_flatoffdirLineEdit->clear();
  in_sciencedirLineEdit->clear();
  in_skydirLineEdit->clear();
  in_standarddirLineEdit->clear();
  update_logfile();
}


// open an image upon double click or clicking the "OK" push button.
// if the image is clicked only once, show the FITS header in the 
// preview window
void theliForm::viewimageOpen()
{
  QString execstr, maindatapath;
  
  // which FITS viewer has the user selected
  
  maindatapath = in_maindirLineEdit->text();
  if (skycat->isOn()) {
    execstr = "cd ";
    execstr.append(maindatapath);
    execstr.append("; skycat ");
  }
  if (ds9->isOn()) {
    execstr = "cd ";
    execstr.append(maindatapath);
    execstr.append("; ds9 ");
  }
  
  // redefine the preview class
  class Preview : public QTextEdit, public QFilePreview
  {
  public:
  Preview( QWidget *parent=0 ) : QTextEdit( parent ) {}
    
    void previewUrl( const QUrl &u )
    {
      // that sets the characteristics of the preview window
      clear();
      fitsheaderfontsize = guifont.pointSize();
      QFont fitsheaderfont ("Unicode", fitsheaderfontsize, QFont::Normal);
      //	    fitsheaderfont.setPointSize(fitsheaderfontsize);
      fitsheaderfont.setFixedPitch(true);
      setCurrentFont(fitsheaderfont);
      setWordWrap(NoWrap);
      setReadOnly(true);
      
      // now prepare the FITS header, and show the preview
      int i;
      QString fitsheadpath, line, execstr;
      QString path = u.path();
      QFileInfo fileinfo;
      fileinfo = QFileInfo(path);
      if (!fileinfo.isDir()) {
	// write the fits header to a file in $homedir/.theli/
	execstr = bindir + "/dfits ";
	execstr.append(path);
	execstr.append(" > ");
	execstr.append(homedir);
	execstr.append(".theli/fitshead_tmp");
	my_system(execstr);	
	// the path to the header file
	fitsheadpath = homedir;
	fitsheadpath.append(".theli/fitshead_tmp");		
	// read the header file
	QFile file( fitsheadpath );
	i = 0;
	if ( file.open( IO_ReadOnly ) && checkfile( fitsheadpath ) == 1) {
	  QTextStream stream( &file );
	  while ( !stream.atEnd() ) {
	    line = stream.readLine();
	    i++;
	    if (line.compare("not a FITS file") != 0) {
	      if (i>1) {	// remove the first line that is output by dfits
				// this shows the FITS header in the preview window
		append(line);
		setCursorPosition(0,0);
	      }
	    } else return;
	  }
	  // close and remove the file
	  file.remove();    
	}		
      }
    }
  };
  
  // now show all this in the GUI
  
  Preview* p = new Preview;
  
  QFileDialog* fn = new QFileDialog(this);
  
  QString filters("All (*.*);;"
		  "Images (*.fits *.fit *.fts *.fiz *.FIT *.FITS *.FTS *.FIZ)");
  
  fn->setInfoPreviewEnabled( TRUE );
  fn->setInfoPreview( p, p );
  fn->setPreviewMode( QFileDialog::Info );
  fn->setDir(in_maindirLineEdit->text());
  fn->setFilters(filters);
  fn->setModal(true);
  fn->show();
  
  // show the image
  QString currentFileName;
  if ( fn->exec() == QDialog::Accepted ) {
    currentFileName = fn->selectedFile();
    execstr.append(currentFileName);
    execstr.append("&");
    my_system(execstr);
  }
}


void theliForm::get_lastchar(QString &string)
{
  lastchar = string.right(1);
}


void theliForm::call_parameters_helper()
{
  // clear the command list window as it might be rewritten depending
  // on the input made in the parameter fields
  pre_splitCheckBox->setChecked(false);
  cal_procscienceCheckBox->setChecked(false);
  sf_collapseCheckBox->setChecked(false);
  cc_coaddCheckBox->setChecked(false);
  pre_commandTextEdit->setText(empty);
  cal_commandTextEdit->setText(empty);
  sf_commandTextEdit->setText(empty);
  ap_commandTextEdit->setText(empty);
  cc_commandTextEdit->setText(empty);
}


void theliForm::call_parameters()
{
  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_pre()
{
  
  // THESE EXTRA { } ARE IMPORTANT, forcing the settings to be written
  
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","0");
    settings.endGroup();
    settings.endGroup();
  }
  
  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}

void theliForm::call_parameters_cal()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","1");
    settings.endGroup();
    settings.endGroup();
  }
  
  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_sf1()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","2");
    settings.endGroup();
    settings.endGroup();
  }

  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_sf2()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","3");
    settings.endGroup();
    settings.endGroup();
  }

  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_w1()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","4");
    settings.endGroup();
    settings.endGroup();
  }
  
  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_w3()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","4");
    settings.endGroup();
    settings.endGroup();
  }
  
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_w2()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","5");
    settings.endGroup();
    settings.endGroup();
  }
  
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_absphotom_indirect()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","6");
    settings.endGroup();
    settings.endGroup();
  }
  
  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_absphotom_direct()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","7");
    settings.endGroup();
    settings.endGroup();
  }
  
  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_sexcat()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","8");
    settings.endGroup();
    settings.endGroup();
  }
  
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_astrom()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","9");
    settings.endGroup();
    settings.endGroup();
  }
  
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_parameters_cc1()
{
  {
    QSettings settings(QSettings::Ini);
    settings.beginGroup("/theli_"+in_reductionlogLineEdit->text()+"_");
    settings.beginGroup("/theliform_settings");
    settings.writeEntry("/TF_PARAMETERSPAGE","10");
    settings.endGroup();
    settings.endGroup();
  }
  
  call_parameters_helper();
  parameters *param = new parameters();
  param->show();
}


void theliForm::call_cni()
{
  // first update the logfile so that the latest instrument 
  // can be found there
  update_logfile();
  
  // now show that dialogue
  cni_form* cf = new cni_form();
  cf->show();
}


void theliForm::call_color()
{
  update_lastlog();
  update_logfile();
  
  QFile file(scriptdir);
  if (file.exists() && scriptdir.length() > 0) {
    color_form* color = new color_form();
    color->show();
  }
  else {
    QMessageBox::warning( this, "Non-existing pipeline directory!",
			  "Check your installation!\n");
    return;
  }
}


void theliForm::call_animate()
{
  update_lastlog();
  update_logfile();
  
  QFile file(scriptdir);
  if (file.exists() && scriptdir.length() > 0) {
    animate* anim = new animate();
    anim->show();
  }
  else {
    QMessageBox::warning( this, "Non-existing pipeline directory!",
			  "Check your installation!\n");
    return;
  }
}


void theliForm::call_zp()
{
  update_lastlog();
  update_logfile();
  
  zp* zeropoint = new zp();
  zeropoint->show();
}


void theliForm::call_combineimages()
{
  update_lastlog();
  update_logfile();
  
  combineimages* combima = new combineimages();
  combima->show();
}


void theliForm::call_imagestatistics()
{
  update_lastlog();
  update_logfile();
  
  statistics* imagestatistics = new statistics();
  imagestatistics->show();
}


void theliForm::call_imalyzer()
{
  update_lastlog();
  update_logfile();
  
  imalyzer* imanalysis = new imalyzer();
  imanalysis->show();
}


void theliForm::call_select_astroref_image()
{
  QString currentFileName = QFileDialog::getOpenFileName(
	    homedir,
	    "Images (*.fits *.fit *.fts *.FIT *.FITS *.FTS);; All (*.*)",
	    this,
	    "open file dialog",
	    "Select an astrometrically calibrated image");

  if ( currentFileName.isEmpty() )
    return;
  
  ap_refimageLineEdit->setText(currentFileName);
}


void theliForm::call_commandhistory()
{
  commhist* ch = new commhist();
  ch->show();
}


void theliForm::edit_scripts()
{
  QString execstr;
  
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  QString fn = QFileDialog::getOpenFileName(
	    scriptdir,
	    "Pipeline scripts (*.sh);; All (*.*)",
	    this,
	    "open file dialog",
	    "Choose a script" );
    
  if ( fn.isEmpty() )
    return;
  QString currentFileName = fn;
  execstr.append(currentFileName);
  execstr.append("&");
  my_system(execstr);
}


void theliForm::edit_userinstconfigs()
{
  QString execstr;
    
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  QString fn = QFileDialog::getOpenFileName(
	    userthelidir+"/instruments_user/",
	    "Instrument configuration files (*.ini)",
	    this,
	    "open file dialog",
	    "Choose an instrument" );

  QFileInfo fileinfo = QFileInfo(fn);
  QString filename = fileinfo.fileName();
  
  if ( fn.isEmpty())
    return;
  
  if ( filename.compare("progs.ini") == 0 || 
       filename.compare("param_set1.ini") == 0 || 
       filename.compare("param_set2.ini") == 0 ||
       filename.compare("param_set3.ini") == 0) {
    QMessageBox::information( this, "THELI",
		"This is not an instrument configuration file!\n"
                "Please select a different one.");
    return;
  }

  QString currentFileName = fn;
  execstr.append(currentFileName);
  execstr.append("&");
  my_system(execstr);
}


void theliForm::edit_profinstconfigs()
{
  QString execstr;
    
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  QString fn = QFileDialog::getOpenFileName(
	    scriptdir+"/instruments_professional/",
	    "Instrument configuration files (*.ini)",
	    this,
	    "open file dialog",
	    "Choose an instrument" );

  QFileInfo fileinfo = QFileInfo(fn);
  QString filename = fileinfo.fileName();
  
  if ( fn.isEmpty())
    return;
  
  if ( filename.compare("progs.ini") == 0 || 
       filename.compare("param_set1.ini") == 0 || 
       filename.compare("param_set2.ini") == 0 ||
       filename.compare("param_set3.ini") == 0) {
    QMessageBox::information( this, "THELI",
		"This is not an instrument configuration file!\n"
                "Please select a different one.");
    return;
  }

  QString currentFileName = fn;
  execstr.append(currentFileName);
  execstr.append("&");
  my_system(execstr);
}


void theliForm::edit_comminstconfigs()
{
  QString execstr;
    
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  QString fn = QFileDialog::getOpenFileName(
	    scriptdir+"/instruments_commercial/",
	    "Instrument configuration files (*.ini)",
	    this,
	    "open file dialog",
	    "Choose an instrument" );

  QFileInfo fileinfo = QFileInfo(fn);
  QString filename = fileinfo.fileName();
  
  if ( fn.isEmpty())
    return;
  
  if ( filename.compare("progs.ini") == 0 || 
       filename.compare("param_set1.ini") == 0 || 
       filename.compare("param_set2.ini") == 0 ||
       filename.compare("param_set3.ini") == 0) {
    QMessageBox::information( this, "THELI",
		"This is not an instrument configuration file!\n"
                "Please select a different one.");
    return;
  }

  QString currentFileName = fn;
  execstr.append(currentFileName);
  execstr.append("&");
  my_system(execstr);
}


void theliForm::show_prelog()
{
  QString execstr;
  
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  execstr.append(userscriptdir);
  execstr.append("theli_pre_comm.sh.log &");
  my_system(execstr);
}


void theliForm::show_callog()
{
  QString execstr;
  
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  execstr.append(userscriptdir);
  execstr.append("theli_cal_comm.sh.log &");
  my_system(execstr);
}


void theliForm::show_sflog()
{
  QString execstr;
  
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  execstr.append(userscriptdir);
  execstr.append("theli_bg_comm.sh.log &");
  my_system(execstr);
}


void theliForm::show_wlog()
{
  QString execstr;
  
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  execstr.append(userscriptdir);
  execstr.append("theli_w_comm.sh.log &");
  my_system(execstr);
}


void theliForm::show_aplog()
{
  QString execstr;
  
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  execstr.append(userscriptdir);
  execstr.append("theli_ap_comm.sh.log &");
  my_system(execstr);
}


void theliForm::show_cclog()
{
  QString execstr;
  
  if (nano->isOn()) execstr = "nano ";
  if (emacs->isOn()) execstr = "emacs ";
  if (xemacs->isOn()) execstr = "xemacs ";
  if (nedit->isOn()) execstr = "nedit ";
  if (vi->isOn()) execstr = "xterm -e vi ";
  
  execstr.append(userscriptdir);
  execstr.append("theli_cc_comm.sh.log &");
  my_system(execstr);
}


// get the FITS extension in case the user clicks the "start all" button.
// this is needed to override the settings in the LOG file
void theliForm::get_fitsext_fromcheckboxes(QString keyword)
{
  if (keyword.compare("forchopnod") == 0) {
    ext_imgch = "OFC";
    if (sf_backgroundCheckBox->isChecked() || CB_background == 1) 
      ext_imgch.append("B");
  }
  if (keyword.compare("forcollapse") == 0) {
    ext_imgch = "OFC";
    if (sf_backgroundCheckBox->isChecked() || CB_background == 1) 
      ext_imgch.append("B");
    if (sf_chopnodCheckBox->isChecked() || CB_chopnod == 1) 
      ext_imgch.append("H");
  }
  if (keyword.compare("fordebloom") == 0) {
    ext_imgch = "OFC";
    if (sf_backgroundCheckBox->isChecked() || CB_background == 1) 
      ext_imgch.append("B");
    if (sf_chopnodCheckBox->isChecked() || CB_chopnod == 1) 
      ext_imgch.append("H");
    if (sf_collapseCheckBox->isChecked() || CB_collapse == 1) 
      ext_imgch.append("C");
  }
  if (keyword.compare("forweights") == 0) {
    ext_imgch = "OFC";
    if (sf_backgroundCheckBox->isChecked() || CB_background == 1) 
      ext_imgch.append("B");
    if (sf_chopnodCheckBox->isChecked() || CB_chopnod == 1) 
      ext_imgch.append("H");
    if (sf_collapseCheckBox->isChecked() || CB_collapse == 1) 
      ext_imgch.append("C");
    if (w_debloomCheckBox->isChecked() || CB_debloom == 1) 
      ext_imgch.append( "D");
  }
  if (keyword.compare("forastrometry") == 0) {
    ext_imgch = "OFC";
    
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
    
    if (sf_backgroundCheckBox->isChecked() || CB_background == 1) 
      ext_imgch.append("B");
    if (sf_chopnodCheckBox->isChecked() || CB_chopnod == 1) 
      ext_imgch.append("H");
    if (sf_collapseCheckBox->isChecked() || CB_collapse == 1) 
      ext_imgch.append("C");
    if (w_debloomCheckBox->isChecked() || CB_debloom == 1) 
      ext_imgch.append( "D");
    if ((ap_absphotom_direct_CheckBox->isChecked() || 
	 CB_absphotom_direct == 1) &&
	abs_fittingmethod)
      ext_imgch.append( "P");
  }
}


void theliForm::is_imextension_empty(int &result)
{
  obtain_imextension();
  result = 0;
  if (ext_img[0] == 0) {
    QMessageBox::warning( this, "Unknown reduction status",
	QString ("THELI does not know the reduction status of your data.\n"
		 "Provide the missing information in the 'Status' pull-down menu.\n"
		 "'Debiasing and flatfielding done' must be marked,\n"
		 "even if you did not apply these calibrators.\n"));
    result = 1;
  }
}


void theliForm::get_fitsext_forchopnod(int *array, QString &result)
{
  obtain_imextension();
  result = "";
  if (array[0] == 1) {
    result.append("OFC");
    if (array[1] == 1) result.append("B");
  }
}


void theliForm::get_fitsext_forcollmerge(int *array, QString &result)
{
  obtain_imextension();
  result = "";
  if (array[0] == 1) {
    result.append("OFC");
    if (array[1] == 1) result.append("B");
    if (array[2] == 1) result.append("H");
  }
}


void theliForm::get_fitsext_fordebloom(int *array, QString &result)
{
  obtain_imextension();
  result = "";
  if (array[0] == 1) {
    result.append("OFC");
    if (array[1] == 1) result.append("B");
    if (array[2] == 1) result.append("H");
    if (array[3] == 1) result.append("C");
  }
}


void theliForm::get_fitsext_forweights(int *array, QString &result)
{
  obtain_imextension();
  result = "";
  if (array[0] == 1) {
    result.append("OFC");
    if (array[1] == 1) result.append("B");
    if (array[2] == 1) result.append("H");
    if (array[3] == 1) result.append("C");
    if (array[4] == 1) result.append("D");
  }
}


void theliForm::get_fitsext_forastromcoadd(int *array, QString &result)
{
  obtain_imextension();
  result = "";
  if (array[0] == 1) {
    result.append("OFC");
    if (array[1] == 1) result.append("B");
    if (array[2] == 1) result.append("H");
    if (array[3] == 1) result.append("C");
    if (array[4] == 1) result.append("D");
    if (array[5] == 1) result.append("P");
  }
}


void theliForm::which_instlist()
{
  if (in_profinstRadioButton->isChecked()) inst_prof_index = in_instrumentListBox->currentItem();
  if (in_comminstRadioButton->isChecked()) inst_comm_index = in_instrumentListBox->currentItem();
  if (in_userinstRadioButton->isChecked()) inst_user_index = in_instrumentListBox->currentItem();
}


void theliForm::check_for_instrument_duplicates()
{
  QDir inst_prof_dir, inst_user_dir, inst_comm_dir;
  QStringList instrumentlist;
    
  inst_prof_dir.setPath(profinstdir);
  inst_comm_dir.setPath(comminstdir);
  inst_user_dir.setPath(userinstdir);

  QStringList inst_prof_list = inst_prof_dir.entryList("*.ini");
  QStringList inst_comm_list = inst_comm_dir.entryList("*.ini");
  QStringList inst_user_list = inst_user_dir.entryList("*.ini");
  
  for ( QStringList::Iterator it = inst_prof_list.begin(); it != inst_prof_list.end(); ++it )
    instrumentlist.append(*it);
  for ( QStringList::Iterator it = inst_comm_list.begin(); it != inst_comm_list.end(); ++it )
      instrumentlist.append(*it);
  for ( QStringList::Iterator it = inst_user_list.begin(); it != inst_user_list.end(); ++it )
    instrumentlist.append(*it);

  instrumentlist.sort();

  QString inst, instold;

  instold = "";
  for ( QStringList::Iterator it = instrumentlist.begin(); it != instrumentlist.end(); ++it ) {
    inst = *it;
    if (QString::compare(inst,instold) == 0) {
	QMessageBox::warning( this, "Duplicate instrument found",
			      "The following duplicate instrument was found:\n\n"
		""+*it+"\n\n"
		"The instrument name must be unique. Please check the\n"
		"following paths and remove or rename one of the copies:\n\n"
		""+profinstdir+"\n"
		""+comminstdir+"\n"
		""+userinstdir+"\n");
    }
    instold = *it;
  }
}


void theliForm::highlight_instrumentname(QString instrumentname)
{
  bool exist = false;
  QFile instfile;
  QString tmpread;

  // first, find out where the instrument is stored

  instfile.setName(profinstdir+instrumentname+".ini");
  if (instfile.exists()) {
    in_profinstRadioButton->setChecked(true);
    exist = true;
  }

  instfile.setName(comminstdir+instrumentname+".ini");
  if (instfile.exists()) {
    in_comminstRadioButton->setChecked(true);
    exist = true;
  }

  instfile.setName(userinstdir+instrumentname+".ini");
  if (instfile.exists()) {
    in_userinstRadioButton->setChecked(true);
    exist = true;
  }


  // now highlight the instrument

  int numinstruments = in_instrumentListBox->numRows();
  if (!instrumentname.isEmpty() && exist) {
    for (int i = 0; i < numinstruments; i++) {
      tmpread = in_instrumentListBox->text(i);
      if (tmpread.compare(instrumentname) == 0) {
	lastinstrument = instrumentname;
	in_instrumentListBox->setCurrentItem(i);
      }
    }
  }
  else lastinstrument = in_instrumentListBox->currentText();
}


void theliForm::refresh_instlist()
{
  QDir inst_prof_dir, inst_user_dir, inst_comm_dir;
  QString tmpread;
  
  inst_prof_dir.setPath(profinstdir);
  inst_comm_dir.setPath(comminstdir);
  inst_user_dir.setPath(userinstdir);
  
  if (!inst_prof_dir.exists() && 
      !inst_comm_dir.exists() &&
      !inst_user_dir.exists()) in_instrumentListBox->clear();
  else {
    // populate the ListBox
    in_instrumentListBox->clear();
    QStringList inst_prof_list = inst_prof_dir.entryList("*.ini");
    QStringList inst_comm_list = inst_comm_dir.entryList("*.ini");
    QStringList inst_user_list = inst_user_dir.entryList("*.ini");
    
    inst_prof_list.gres(".ini","");
    inst_comm_list.gres(".ini","");
    inst_user_list.gres(".ini","");
    if (in_profinstRadioButton->isChecked()) in_instrumentListBox->insertStringList(inst_prof_list,-1);
    if (in_comminstRadioButton->isChecked()) in_instrumentListBox->insertStringList(inst_comm_list,-1);
    if (in_userinstRadioButton->isChecked()) in_instrumentListBox->insertStringList(inst_user_list,-1);
    in_instrumentListBox->sort();

    // update the instrument list    
    //	highlight_instrumentname(lastinstrument);
    
    if (in_profinstRadioButton->isChecked()) in_instrumentListBox->setCurrentItem(inst_prof_index);
    if (in_comminstRadioButton->isChecked()) in_instrumentListBox->setCurrentItem(inst_comm_index);
    if (in_userinstRadioButton->isChecked()) in_instrumentListBox->setCurrentItem(inst_user_index);
    
    int i, numinstruments = in_instrumentListBox->numRows();
    QString instrumentname = lastinstrument;
    if (!instrumentname.isEmpty()) {
      for (i = 0; i < numinstruments; i++) {
	tmpread = in_instrumentListBox->text(i);
	if (tmpread.compare(instrumentname) == 0) {
	  lastinstrument = instrumentname;
	  in_instrumentListBox->setCurrentItem(i);
	}
      }
    }
    else lastinstrument = in_instrumentListBox->currentText();
  }
}


void theliForm::repaint_defaultpaths_helper(QString dir, QString dir2, 
					    QLineEdit *myLineEdit)
{
  if (!dir2.isEmpty()) {
    if (checkdir2(dir) == 1) 
      myLineEdit->setPaletteBackgroundColor(tfgreen);
    else myLineEdit->setPaletteBackgroundColor(tfred);
  } else myLineEdit->setPaletteBackgroundColor(tfcyan);
}


void theliForm::repaint_defaultabspaths_helper(QString dir, QString dir2, 
					    QLineEdit *myLineEdit)
{
  QDir maindir = QDir(dir);
  
  // the main dir must be an absolute path
  
  if (!dir2.isEmpty()) {	
    if (checkdir2(dir) == 1 && !maindir.isRelative()) 
      myLineEdit->setPaletteBackgroundColor(tfgreen);
    else myLineEdit->setPaletteBackgroundColor(tfred);
  } else myLineEdit->setPaletteBackgroundColor(tfcyan);
}


void theliForm::repaint_defaultpaths()
{
  // avoid empty characters in the path fields
  QRegExp rx( "^\\S+$" );
  QRegExp ry( "^[^\\s/]+$" );
  QValidator* validator_string = new QRegExpValidator( rx, this );
  QValidator* validator_noslash = new QRegExpValidator( ry, this );
  in_maindirLineEdit->setValidator( validator_string );
  in_biasdirLineEdit->setValidator( validator_noslash );
  in_darkdirLineEdit->setValidator( validator_noslash );
  in_flatdirLineEdit->setValidator( validator_noslash );
  in_flatoffdirLineEdit->setValidator( validator_noslash );
  in_sciencedirLineEdit->setValidator( validator_noslash );
  in_skydirLineEdit->setValidator( validator_noslash );
  in_standarddirLineEdit->setValidator( validator_noslash );
  
  QString main     = in_maindirLineEdit->text();
  main.append("/");
  QString bias     = main;
  QString dark     = main;
  QString flat     = main;
  QString flatoff  = main;
  QString science  = main;
  QString sky      = main;
  QString standard = main;
  
  bias.append(in_biasdirLineEdit->text());
  dark.append(in_darkdirLineEdit->text());
  flat.append(in_flatdirLineEdit->text());
  flatoff.append(in_flatoffdirLineEdit->text());
  science.append(in_sciencedirLineEdit->text());
  sky.append(in_skydirLineEdit->text());
  standard.append(in_standarddirLineEdit->text());
  
  QString main2     = in_maindirLineEdit->text();
  QString bias2     = in_biasdirLineEdit->text();
  QString dark2     = in_darkdirLineEdit->text();
  QString flat2     = in_flatdirLineEdit->text();
  QString flatoff2  = in_flatoffdirLineEdit->text();
  QString science2  = in_sciencedirLineEdit->text();
  QString sky2      = in_skydirLineEdit->text();
  QString standard2 = in_standarddirLineEdit->text();
  
  repaint_defaultabspaths_helper(main, main2, in_maindirLineEdit);
  repaint_defaultpaths_helper(bias, bias2, in_biasdirLineEdit);
  repaint_defaultpaths_helper(dark, dark2, in_darkdirLineEdit);
  repaint_defaultpaths_helper(flat, flat2, in_flatdirLineEdit);
  repaint_defaultpaths_helper(flatoff, flatoff2, in_flatoffdirLineEdit);
  repaint_defaultpaths_helper(science, science2, in_sciencedirLineEdit);
  repaint_defaultpaths_helper(sky, sky2, in_skydirLineEdit);
  repaint_defaultpaths_helper(standard, standard2, in_standarddirLineEdit);
}


void theliForm::get_numcpu(QString paramset1inipath)
{
  QString numcpu;
  
  long maxcpu = get_max_ncpu();
  in_numcpuSpinBox->setMaxValue(maxcpu);
  get_fileparameter(paramset1inipath, "NPARA", numcpu);
  int ncpu = numcpu.toInt();
  if (ncpu > maxcpu) ncpu = maxcpu;
  in_numcpuSpinBox->setValue(ncpu);
}


void theliForm::update_current_commandlist()
{
  if (mainmenu_TabWidget->currentPageIndex() == 1) update_pre_commandlist();
  if (mainmenu_TabWidget->currentPageIndex() == 2) update_cal_commandlist();
  if (mainmenu_TabWidget->currentPageIndex() == 3) update_sf_commandlist();
  if (mainmenu_TabWidget->currentPageIndex() == 4) update_w_commandlist();
  if (mainmenu_TabWidget->currentPageIndex() == 5) update_ap_commandlist();
  if (mainmenu_TabWidget->currentPageIndex() == 6) update_cc_commandlist();
}


void theliForm::update_tabchanged_commandlists()
{
  int index = mainmenu_TabWidget->currentPageIndex();
  QColor darkred = QColor(170,0,0);
  
  // update the number of simultaneously loaded chips
  
  // make sure the mandatory checkboxes don't change their foreground 
  // colour to black when changing other GUI elements (this is a Qt bug)
  
  //    update_nframes_instconfig();
  
  //    if (index == 0) get_exposure_number();
  if (index == 1) {
    update_pre_commandlist();
    pre_splitCheckBox->setPaletteForegroundColor(darkred);
  }
  if (index == 2) {
    update_cal_commandlist();
    if (cal_nobiasCheckBox->isChecked())
      cal_procbiasCheckBox->setPaletteForegroundColor(QColor(0,0,0));
    else
      cal_procbiasCheckBox->setPaletteForegroundColor(darkred);

    cal_procflatCheckBox->setPaletteForegroundColor(darkred);
    cal_procscienceCheckBox->setPaletteForegroundColor(darkred);
  }
  if (index == 3) update_sf_commandlist();
  if (index == 4) {
    update_w_commandlist();
    w_createglobwCheckBox->setPaletteForegroundColor(darkred);
    w_createweightsCheckBox->setPaletteForegroundColor(darkred);
  }
  if (index == 5) {
    update_ap_commandlist();
    ap_createsourcecatCheckBox->setPaletteForegroundColor(darkred);
    ap_astrometryCheckBox->setPaletteForegroundColor(darkred);
    update_ap_variousfields();
  }
  if (index == 6) {
    update_cc_commandlist();
    cc_createskysubCheckBox->setPaletteForegroundColor(darkred);
    cc_coaddCheckBox->setPaletteForegroundColor(darkred);
  }
}


void theliForm::repaint_parameter_fields_helper(QCheckBox *myCheckBox, 
						QLineEdit *myLineEdit)
{
  QString tmp = myLineEdit->text();
  if (myCheckBox->isChecked() && tmp.isEmpty())
    myLineEdit->setPaletteBackgroundColor(tfred);
  else myLineEdit->setPaletteBackgroundColor(tfwhite);
}


void theliForm::show_scriptlog(QTextEdit *text_edit, QString errormessage)
{
  QString execstr, completemessage, errorline;
  
  if (errormessage.left(11).compare("THELI_ERROR") != 0 && 
      errormessage.length() < 11 && errcount == 0) {
    text_edit->append("Done.");
    text_edit->scrollToBottom();
  }
  if (errormessage.left(11).compare("THELI_ERROR") == 0 && 
      errormessage.length() > 11 && errcount != 0) {
    errorline.setNum(errline);
    completemessage = "ERROR (line ";
    completemessage.append(errorline);
    completemessage.append("): Showing you the log ...");
    text_edit->append(completemessage);
    text_edit->scrollToBottom();
    
    // display the log file
    if (nano->isOn()) execstr = "nano +"+errorline+" ";
    if (emacs->isOn()) execstr = "emacs +"+errorline+" ";
    if (xemacs->isOn()) execstr = "xemacs ";
    if (nedit->isOn()) execstr = "nedit -line "+errorline+" ";
    if (vi->isOn()) execstr = "xterm -e vi ";
    
    versioncheck(showversionwarning);

    execstr.append(fullpathlog);
    execstr.append(" &");
    my_system(execstr);
  } 

  started_proc_del = 0;
  started_proc_pre = 0;
  started_proc_cal = 0;
  started_proc_sf = 0;
  started_proc_w = 0;
  started_proc_ap = 0;
  started_proc_cc = 0;
}


void updateError_helper(QProcess *process, QTextEdit *myTextEdit)
{
    QByteArray data = process->readStderr();
    myTextEdit->append("An error occured in the calling script:");
    myTextEdit->append(QString(data));
    myTextEdit->scrollToBottom();
}


void theliForm::updateError()
{
    if (process_group.compare("pre") == 0) 
	updateError_helper(proc_pre, pre_messagesTextEdit);
    if (process_group.compare("cal") == 0) 
	updateError_helper(proc_cal, cal_messagesTextEdit);
    if (process_group.compare("sf") == 0) 
	updateError_helper(proc_sf, sf_messagesTextEdit);
    if (process_group.compare("w") == 0) 
	updateError_helper(proc_w, w_messagesTextEdit);
    if (process_group.compare("ap") == 0) 
	updateError_helper(proc_ap, ap_messagesTextEdit);
    if (process_group.compare("cc") == 0) 
	updateError_helper(proc_cc, cc_messagesTextEdit);
}


void theliForm::updateText()
{
  QString tmp, tmp2, size, numstars, myskycatpath, execstr;
  QSettings settings(QSettings::Ini);
  QString reductionlog = in_reductionlogLineEdit->text();

  if (process_group.compare("del") == 0) {
    QByteArray data = proc_del->readStdout();
    tmp = QString(data);
    if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
      tmp2 = tmp.right( tmp.length() - 14);
      if (mainmenu_TabWidget->currentPageIndex() == 1){
	pre_messagesTextEdit->append(tmp2);
	pre_messagesTextEdit->scrollToBottom();
      }
      if (mainmenu_TabWidget->currentPageIndex() == 2){
	cal_messagesTextEdit->append(tmp2);
	cal_messagesTextEdit->scrollToBottom();
      }
      if (mainmenu_TabWidget->currentPageIndex() == 3){
	sf_messagesTextEdit->append(tmp2);
	sf_messagesTextEdit->scrollToBottom();
      }
      if (mainmenu_TabWidget->currentPageIndex() == 4){
	w_messagesTextEdit->append(tmp2);
	w_messagesTextEdit->scrollToBottom();
      }
      if (mainmenu_TabWidget->currentPageIndex() == 5){
	ap_messagesTextEdit->append(tmp2);
	ap_messagesTextEdit->scrollToBottom();
      }
      if (mainmenu_TabWidget->currentPageIndex() == 6){
	cc_messagesTextEdit->append(tmp2);
	cc_messagesTextEdit->scrollToBottom();
      }
    }
  }
  
  if (process_group.compare("pre") == 0) {
    QByteArray data = proc_pre->readStdout();
    tmp = QString(data);
    if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
      tmp2 = tmp.right( tmp.length() - 14);
      pre_messagesTextEdit->append(tmp2);
      pre_messagesTextEdit->scrollToBottom();
      if (tmp2.compare("Sorting raw data ...\n") == 0) {
	CB_sortrawdata = 1;
	pre_sortdataCheckBox->setPaletteBackgroundColor(tfgreen);
	pre_sortdataCheckBox->setChecked(false);
      }
      if (tmp2.compare("Splitting data, correcting headers ...\n") == 0) {
	CB_processsplit = 1;
	pre_splitCheckBox->setPaletteBackgroundColor(tfgreen);
	pre_splitCheckBox->setChecked(false);
      }
      if (tmp2.compare("Creating links ...\n") == 0) {
	CB_createlinks = 1;
	pre_createlinksCheckBox->setPaletteBackgroundColor(tfgreen);
	pre_createlinksCheckBox->setChecked(false);
      }	
    }
  }

  if (process_group.compare("cal") == 0) {
    QByteArray data = proc_cal->readStdout();
    tmp = QString(data);
    if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
      tmp2 = tmp.right( tmp.length() - 14);
      cal_messagesTextEdit->append(tmp2);
      cal_messagesTextEdit->scrollToBottom();
      if (tmp2.compare("Processing BIASes ...\n") == 0) {
	CB_processbias = 1;
	cal_procbiasCheckBox->setPaletteBackgroundColor(tfgreen);
	cal_procbiasCheckBox->setChecked(false);
      }
      if (tmp2.compare("Processing DARKs ...\n") == 0) {
	CB_processdark = 1;
	cal_procdarkCheckBox->setPaletteBackgroundColor(tfgreen);
	cal_procdarkCheckBox->setChecked(false);
      }
      if (tmp2.compare("Processing FLATs ...\n") == 0) {
	CB_processflat = 1;
	cal_procflatCheckBox->setPaletteBackgroundColor(tfgreen);
	cal_procflatCheckBox->setChecked(false);
      }
      if (tmp2.compare("Calibrating data ...\n") == 0) {
	CB_processscience = 1;
	cal_procscienceCheckBox->setPaletteBackgroundColor(tfgreen);
	cal_procscienceCheckBox->setChecked(false);
	statusCPSDebiasingAction->setOn(true);
	cal_calibredoPushButton->setIconSet(QPixmap(active));
	cal_calibredoPushButton->blockSignals(false);
	set_imextension(1, 0);
      }
    }
  }

  if (process_group.compare("sf") == 0) {
    QByteArray data = proc_sf->readStdout();
    tmp = QString(data);
    if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
      tmp2 = tmp.right( tmp.length() - 14);
      sf_messagesTextEdit->append(tmp2);
      sf_messagesTextEdit->scrollToBottom();
      if (tmp2.compare("Grouping images ...\n") == 0) {
	CB_spreadsequence = 1;
	sf_spreadsequenceCheckBox->setPaletteBackgroundColor(tfgreen);
	sf_spreadsequenceCheckBox->setChecked(false);
      }
      if (tmp2.compare("Background modelling ...\n") == 0) {
	CB_background = 1;
	sf_backgroundCheckBox->setPaletteBackgroundColor(tfgreen);
	sf_backgroundCheckBox->setChecked(false);
	statusCPSBackgroundAction->setOn(true);
	sf_backgroundredoPushButton->setIconSet(QPixmap(active));
	sf_backgroundredoPushButton->blockSignals(false);
	if (science_background == 1) set_imextension(1, 1);
      }
      if (tmp2.compare("Collecting images ...\n") == 0) {
	CB_mergesequence = 1;
	sf_mergesequenceCheckBox->setPaletteBackgroundColor(tfgreen);
	sf_mergesequenceCheckBox->setChecked(false);
      }
      if (tmp2.compare("Subtracting sky by chop-nod ...\n") == 0) {
	CB_chopnod = 1;
	sf_chopnodCheckBox->setPaletteBackgroundColor(tfgreen);
	sf_chopnodCheckBox->setChecked(false);
	statusCPSChopnodAction->setOn(true);
	sf_chopnodredoPushButton->setIconSet(QPixmap(active));
	sf_chopnodredoPushButton->blockSignals(false);
	if (science_chopnod == 1) set_imextension(1, 2);
      }
      if (tmp2.compare("Collapse correction ...\n") == 0) {
	CB_collapse = 1;
	sf_collapseCheckBox->setPaletteBackgroundColor(tfgreen);
	sf_collapseCheckBox->setChecked(false);
	statusCPSCollapseAction->setOn(true);
	sf_collapseredoPushButton->setIconSet(QPixmap(active));
	sf_collapseredoPushButton->blockSignals(false);
	if (science_coll == 1) set_imextension(1, 3);
      }
    }
  }

  if (process_group.compare("w") == 0) {
    QByteArray data = proc_w->readStdout();
    tmp = QString(data);
    if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
      tmp2 = tmp.right( tmp.length() - 14);
      w_messagesTextEdit->append(tmp2);
      w_messagesTextEdit->scrollToBottom();
      if (tmp2.compare("Deblooming images ...\n") == 0) {
	CB_debloom = 1;
	w_debloomCheckBox->setPaletteBackgroundColor(tfgreen);
	w_debloomCheckBox->setChecked(false);
	statusCPSDebloomingAction->setOn(true);
	w_debloomredoPushButton->setIconSet(QPixmap(active));
	w_debloomredoPushButton->blockSignals(false);
	if (science_debloom == 1) set_imextension(1, 6);
      }
      if (tmp2.compare("Creating fits preview ...\n") == 0 ||
	  tmp2.compare("Creating tiff preview ...\n") == 0) {
	CB_binnedmosaics = 1;
	w_createbinmosCheckBox->setPaletteBackgroundColor(tfgreen);
	w_createbinmosCheckBox->setChecked(false);
      }
      if (tmp2.compare("Creating global WEIGHTs ...\n") == 0) {
	CB_globalweights = 1;
	w_createglobwCheckBox->setPaletteBackgroundColor(tfgreen);
	w_createglobwCheckBox->setChecked(false);
      }
      if (tmp2.compare("Creating WEIGHTs ...\n") == 0) {
	CB_createweights = 1;
	w_createweightsCheckBox->setPaletteBackgroundColor(tfgreen);
	w_createweightsCheckBox->setChecked(false);
      }
      if (tmp2.compare("Separating different target fields ...\n") == 0) {
	CB_distributesets = 1;
	w_distsetCheckBox->setPaletteBackgroundColor(tfgreen);
	w_distsetCheckBox->setChecked(false);
      }
    }
  }

  if (process_group.compare("ap") == 0) {
    QByteArray data = proc_ap->readStdout();
    tmp = QString(data);
    //	printf("%s\n", tmp.ascii());
    // printf("%d\n", tmp.length());
    
    if (tmp.contains("COUNTSTARS") == 1) {
      myskycatpath = userscriptdir;
      myskycatpath.append("theli_mystd.skycat");
      execstr = "gawk 'END {if ($1~/---/ || $1~/ /) print 0; else print $1}' ";
      execstr.append(myskycatpath);
      my_fscanf(execstr, numstars);
      if (numstars.toInt() > 0)
	ap_messagesTextEdit->append(numstars+" reference sources retrieved.");
      else {
	warn(ap_messagesTextEdit,"WARNING: No reference sources retrieved!");
	warn(ap_messagesTextEdit,"-- try a different download server");
	warn(ap_messagesTextEdit,"-- try a different catalog");
	warn(ap_messagesTextEdit,"-- check your internet connection");
	warn(ap_messagesTextEdit,"-- adjust mag limit and radius");
      }
      ap_messagesTextEdit->scrollToBottom();
      // COUNTSTARS and the next THELIMESSAGE are contained in one
      // string including a linefeed. We must remove the first half
      // of the string as the second is otherwise not recognised.
      tmp.remove("COUNTSTARS\n");
    }
    
    if (tmp.contains("THELIMESSAGE:") == 1) {
      tmp.replace("THELIMESSAGE: ","");
      ap_messagesTextEdit->append(tmp);
      ap_messagesTextEdit->scrollToBottom();
      if (tmp.compare("Estimating direct absolute zeropoints ...\n") == 0) {
	CB_absphotom_direct = 1;
	ap_absphotom_direct_CheckBox->setPaletteBackgroundColor(tfgreen);
	ap_absphotom_direct_CheckBox->setChecked(false);

	bool abs_fittingmethod;
	QSettings settings (QSettings::Ini);
	QString reductionlog = in_reductionlogLineEdit->text();
	settings.beginGroup("/theli_"+reductionlog+"_");
	settings.beginGroup("/ASTROMPHOTOM");
	settings.beginGroup("/absphotom");
	int fittingmethodM2 = settings.readNumEntry("/fittingmethodM2",0);
	if (fittingmethodM2 == 0) abs_fittingmethod = true;
	else abs_fittingmethod = false;
	settings.endGroup();
	settings.endGroup();
	settings.endGroup();

	if (abs_fittingmethod) {
	  CB_directphot = 1;
	  statusCPSDirectphotAction->setOn(true);
	  ap_directphotredoPushButton->setIconSet(QPixmap(active));
	  ap_directphotredoPushButton->blockSignals(false);
	  if (science_directphot == 1) set_imextension(1, 7);
	}
	else {
	  CB_directphot = 0;
	  statusCPSDirectphotAction->setOn(false);
	  ap_directphotredoPushButton->setIconSet(QPixmap(passive));
	  ap_directphotredoPushButton->blockSignals(true);
	}
      }
      if (tmp.compare("Preparing photometry catalogs ...\n") == 0 ||
	  tmp.compare("Estimating indirect absolute zeropoints ...\n") == 0) {
	CB_absphotom_indirect = 1;
	ap_absphotom_indirect_CheckBox->setPaletteBackgroundColor(tfgreen);
	ap_absphotom_indirect_CheckBox->setChecked(false);
	statusCPSDirectphotAction->setOn(false);
      }
      if (tmp.compare("Detecting sources ...\n") == 0) {
	CB_singleastrom = 1;
	ap_createsourcecatCheckBox->setPaletteBackgroundColor(tfgreen);
	ap_createsourcecatCheckBox->setChecked(false);
      }
      if (tmp.compare("Calculating astrometric solution ...\n") == 0) {
	CB_astrometry = 1;
	ap_astrometryCheckBox->setPaletteBackgroundColor(tfgreen);
	ap_astrometryCheckBox->setChecked(false);
      }
    }
  }

  if (process_group.compare("cc") == 0) {
    QByteArray data = proc_cc->readStdout();
    tmp = QString(data);
    if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
      tmp2 = tmp.right( tmp.length() - 14);
      cc_messagesTextEdit->append(tmp2);
      cc_messagesTextEdit->scrollToBottom();
      if (tmp2.compare("Subtracting the sky ...\n") == 0) {
	CB_skysub = 1;
	cc_createskysubCheckBox->setPaletteBackgroundColor(tfgreen);
	cc_createskysubCheckBox->setChecked(false);
      }
      if (tmp2.compare("Coaddition: initialising ...\n") == 0 ||
	  tmp2.compare("Coaddition: smoothing overlap ...\n") == 0) {
	CB_createcoadd = 1;
	cc_coaddCheckBox->setPaletteBackgroundColor(tfgreen);
	cc_coaddCheckBox->setChecked(false);
      }
      if (tmp2.compare("Resolving link structure ...\n") == 0) {
	CB_resolvelinks = 1;
	cc_resolvelinksCheckBox->setPaletteBackgroundColor(tfgreen);
	cc_resolvelinksCheckBox->setChecked(false);
      }
    }
  }

  update_logfile();
}


void theliForm::create_thelimessage(QString command, QString &reply)
{
    reply = "THELIMESSAGE: ";
    if (command.find("sort_rawdata.sh") != -1) 
	reply.append("Sorting raw data ...");
    if (command.find("process_split_") != -1) 
	reply.append("Splitting data, correcting headers ...");
    if (command.find("correct_xtalk.sh") != -1) 
	reply.append("Correcting for crosstalk ...");
    if (command.find("createlinks.sh") != -1) 
	reply.append("Creating links ...");
    if (command.find("process_bias_") != -1) 
	reply.append("Processing BIASes ...");
    if (command.find("process_dark_") != -1) 
	reply.append("Processing DARKs ...");
    if (command.find("process_flat_") != -1) 
	reply.append("Processing FLATs ...");
    if (command.find("create_flat_ratio.sh") != -1) 
	reply.append("Measuring gain ratios ...");
    if (command.find("subtract_flat_flatoff_") != -1) 
	reply.append("Subtracting dark flat from bright flat ...");
    if (command.find("check_files_para.sh") != -1) 
	reply.append("Checking brightness levels ...");
    if (command.find("spread_sequence.sh") != -1) 
	reply.append("Grouping images ...");
    if (command.find("create_norm_para.sh") != -1) 
	reply.append("Normalising FLAT ...");
    if (command.find("check_rawfilenames.sh") != -1) 
	reply.append("Checking filename consistency ...");
    if (command.find("process_science_para.sh") != -1) 
	reply.append("Calibrating data ...");
    if (command.find("process_background_para.sh") != -1)
	reply.append("Background modelling ...");
    if (command.find("id_bright_objects.sh") != -1)
        reply.append("Identifying chips with bright stars ...");
    if (command.find("process_science_chopnod_") != -1) 
	reply.append("Subtracting sky by chop-nod ...");
    if (command.find("cleanfiles.sh") != -1) 
	reply.append("Deleting temporary data ...");
    if (command.find("merge_sequence.sh") != -1) 
	reply.append("Collecting images ...");
    if (command.find("process_collapsecorr_para.sh") != -1) 
	reply.append("Collapse correction ...");
    if (command.find("create_debloomedimages_para.sh") != -1) 
	reply.append("Deblooming images ...");
    if (command.find("create_global_weights_para.sh") != -1) 
	reply.append("Creating global WEIGHTs ...");
    if (command.find("transform_ds9_reg.sh") != -1) 
	reply.append("Transforming DS9 masks ...");
    if (command.find("make_album_") != -1) 
	reply.append("Creating fits preview ...");
    if (command.find("create_tiff.sh") != -1) 
	reply.append("Creating tiff preview ...");
    if (command.find("create_weights_para.sh") != -1) 
	reply.append("Creating WEIGHTs ...");
    if (command.find("link_globalweights_para.sh") != -1) 
	reply.append("Creating WEIGHTs ...");
    if (command.find("distribute_sets.sh") != -1) 
	reply.append("Separating different target fields ...");
    if (command.find("transform_images_para.sh") != -1) 
	reply.append("Transforming images and weights ...");
    if (command.find("create_astrorefcat_") != -1) 
	reply.append("Retrieving astrometric reference catalog ...");
    if (command.find("correct_crval_para.sh") != -1) 
	reply.append("Adjusting CRVAL1/2 key in header ...");
    if (command.find("create_astromcats_para.sh") != -1) 
	reply.append("Detecting sources ...");
    if (command.find("create_astromcats_phot_para.sh") != -1) 
	reply.append("Detecting sources for abs photometry ...");
    if (command.find("create_photorefcat_fromWEB.sh") != -1) 
	reply.append("Retrieving photometric reference catalog ...");
    if (command.find("create_photillcorr_corrcat_para.sh") != -1) 
	reply.append("Correcting astrometry in catalogs ...");
    if (command.find("create_photillcorr_getZP.sh") != -1) 
	reply.append("Estimating direct absolute zeropoints ...");
    if (command.find("create_scampcats.sh") != -1) 
	reply.append("Merging multi-chip object catalogs ...");
    if (command.find("create_stdphotom_prepare.sh") != -1) 
	reply.append("Preparing photometry catalogs ...");
    if (command.find("create_abs_photo_info.sh") != -1) 
	reply.append("Estimating indirect absolute zeropoints ...");
    if (command.find("create_zeroorderastrom.sh") != -1) 
	reply.append("Calculating astrometric solution ...");
    if (command.find("create_xcorrastrom.sh") != -1) 
	reply.append("Calculating astrometric solution ...");
    if (command.find("create_headerastrom.sh") != -1) 
	reply.append("Calculating astrometric solution ...");
    if (command.find("create_scamp.sh") != -1) 
	reply.append("Calculating astrometric solution ...");
    if (command.find("create_astrometrynet.sh") != -1) 
	reply.append("Calculating astrometric solution ...");
    if (command.find("create_astrometrynet_photom.sh") != -1) 
	reply.append("Calculating photometric solution ...");
    if (command.find("create_scamp_phot.sh") != -1) 
	reply.append("Calculating 1st pass astrometric solution ...");
    if (command.find("create_stats_table.sh") != -1) 
	reply.append("Collecting image statistics ...");
    if (command.find("create_absphotom_coadd.sh") != -1) 
	reply.append("Collecting information for coaddition ...");
    if (command.find("create_skysub_para.sh") != -1) 
	reply.append("Subtracting the sky ...");
    if (command.find("create_skysubconst_para.sh") != -1) 
	reply.append("Subtracting the sky ...");
    if (command.find("create_smoothedge_para.sh") != -1) 
	reply.append("Coaddition: smoothing overlap ...");
    if (command.find("prepare_coadd_swarp.sh") != -1) 
	reply.append("Coaddition: initialising ...");
    if (command.find("resample_coadd_swarp_para.sh") != -1) 
	reply.append("Coaddition: resampling images ...");
    if (command.find("resample_filtercosmics.sh") != -1) 
	reply.append("Coaddition: rejecting outliers ...");
    if (command.find("perform_coadd_swarp.sh") != -1) 
	reply.append("Coaddition: coadding images ...");
    if (command.find("update_coadd_header.sh") != -1) 
	reply.append("Coaddition: updating header ...");
    if (command.find("resolvelinks.sh") != -1) 
	reply.append("Resolving link structure ...");
}


void theliForm::updateExit()
{
  int falseerrcount;
  QString line, errormessage, errorcode, falseerrorcode;
  QStringList errorlist, falseerrorlist;
  
  // scan for these error messages
  errorlist << "error";
  errorlist << "Illegal instruction";
  errorlist << "not found !";
  errorlist << ": not found";
  errorlist << "cholsolve()";
  errorlist << "inaccuracies likely to occur";
  errorlist << "WARNING: Not enough matched detections in ";
  errorlist << "too long to connect ";
  errorlist << "Segmentation fault";
  errorlist << "segmentation fault";
  errorlist << "Permission denied";
  errorlist << "command not found";
  errorlist << "Cannot map ";
  errorlist << " 0 astrometric references loaded from theli_mystd_scamp.cat";
  errorlist << "fatal: cannot open file";
  errorlist << "WARNING: Significant inaccuracy likely to occur in projection";
  errorlist << "WARNING: Null or negative global weighting factor";
  errorlist << "list has not enough members";  
  errorlist << "Network is unreachable";
  errorlist << "fatal: division by zero attempted";
  errorlist << "head not present; Aborting";
  errorlist << "no match with reference catalog !";
  errorlist << "integer expression expected";
  errorlist << "keyword out of range";
  errorlist << "keyword unknown";
  errorlist << "too many arguments";
  errorlist << "cannot execute binary file";
  errorlist << "Cannot decode file";
  errorlist << "Not enough memory";
  errorlist << "Operation not supported";
  errorlist << "buffer overflow detected";
  errorlist << "Not enough memory";
  errorlist << "did not solve";
  errorlist << "(core dumped)";
  errorlist << "CCfits::FITS::CantOpen";
  errorlist << "has flux scale = 0: I will take 1 instead";
  errorlist << "unbound variable";
  errorlist << "Could not allocate memory";
  errorlist << ".maxlevel.dat: No such file or directory";

  //    errorlist << "PREPROCESS_FAILURE";
  
  // ignore these error messages
  falseerrorlist << "found no such key: e1";
  falseerrorlist << "found no such key: ZPCHOICE";
  falseerrorlist << "found no such key: SEEING";
  falseerrorlist << "found no such key: ZP";
  falseerrorlist << "found no such key: COEFF";
  falseerrorlist << "error in filtering list catalog; exiting";
  falseerrorlist << "not recreating links led to errors when";
  falseerrorlist << "astr_interror2d";
  falseerrorlist << "astr_interror1d";
  falseerrorlist << "astr_referror2d";
  falseerrorlist << "astr_referror1d";
  falseerrorlist << "phot_error";
  falseerrorlist << "ZP_ERROR";
  falseerrorlist << "echo ERROR: ";
  falseerrorlist << "Zeropoint error";
  falseerrorlist << "seeing error";
  falseerrorlist << "WARNING: FLAGS parameter not found in catalog theli_mystd_scamp.cat";
  falseerrorlist << "will produce a mmep runtime error";
  falseerrorlist << "echo \"/${MAIN}/${SCIENCE}/headers/${HEADBASE}.head not present; Aborting\"";
  falseerrorlist << "theli_error";
  falseerrorlist << "raises a KeyError just like a dict would. Please update your code so that KeyErrors";
  falseerrorlist << "REF_PORT keyword unknown";
  falseerrorlist << "CDSCLIENT_EXEC keyword unknown";

  my_system(concatenate_logs);

  my_system(remove_individuallogs);

  // error handling
  QFile file(fullpathlog);
  
  if ( file.open( IO_ReadOnly ) && file.exists()) {
    errormessage = "";
    errcount = 0;
    errline = 0;
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      line.stripWhiteSpace();
      errline++;
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
	  errcount = line.contains(errorcode, false);
	  if (errcount > 0) {
	    errormessage = "THELI_ERROR";
	    errormessage.append(line);
	    break;
	  }
	  else errcount = 0;
	}
	if (errcount > 0) break;
      }
    }
    file.close();
    
    if (process_group.compare("del") == 0) {
      if (mainmenu_TabWidget->currentPageIndex() == 1){
	if (proc_del->normalExit()) 
	  show_scriptlog(pre_messagesTextEdit, errormessage);
	pre_messagesTextEdit->append("\n");
      }
      if (mainmenu_TabWidget->currentPageIndex() == 2){
	if (proc_del->normalExit()) 
	  show_scriptlog(cal_messagesTextEdit, errormessage);
	cal_messagesTextEdit->append("\n");
      }
      if (mainmenu_TabWidget->currentPageIndex() == 3){
	if (proc_del->normalExit()) 
	  show_scriptlog(sf_messagesTextEdit, errormessage);
	sf_messagesTextEdit->append("\n");
      }
      if (mainmenu_TabWidget->currentPageIndex() == 4){
	if (proc_del->normalExit()) 
	  show_scriptlog(w_messagesTextEdit, errormessage);
	w_messagesTextEdit->append("\n");
      }
      if (mainmenu_TabWidget->currentPageIndex() == 5){
	if (proc_del->normalExit()) 
	  show_scriptlog(ap_messagesTextEdit, errormessage);
	ap_messagesTextEdit->append("\n");
      }
      if (mainmenu_TabWidget->currentPageIndex() == 6){
	if (proc_del->normalExit()) 
	  show_scriptlog(cc_messagesTextEdit, errormessage);
	cc_messagesTextEdit->append("\n");
      }
    } 
    
    if (process_group.compare("pre") == 0) {
      if (proc_pre->normalExit()) 
	show_scriptlog(pre_messagesTextEdit, errormessage);
      else {
	pre_messagesTextEdit->append("Aborted.");
	pre_messagesTextEdit->scrollToBottom();
      } 
      pre_messagesTextEdit->append("\n");
    } 
    
    if (process_group.compare("cal") == 0) {
      if (proc_cal->normalExit()) 
	show_scriptlog(cal_messagesTextEdit, errormessage);
      else {
	cal_messagesTextEdit->append("Aborted.");
	cal_messagesTextEdit->scrollToBottom();
      } 
      cal_messagesTextEdit->append("\n");
    } 
    
    if (process_group.compare("sf") == 0) {
      if (proc_sf->normalExit()) {
	show_scriptlog(sf_messagesTextEdit, errormessage);

	// If 2-IR background subtraction was done, remove the collapse correction status
	/*
	QSettings settings (QSettings::Ini);
	QString reductionlog = in_reductionlogLineEdit->text();
	if (reductionlog.isEmpty()) {
	  reductionlog = "noname";
	}
	settings.beginGroup("/theli_"+reductionlog+"_");
	settings.beginGroup("/BACKGROUND");
	settings.beginGroup("/2pass");
	int action = settings.readNumEntry("/maskaction",0);
	settings.endGroup();
	settings.endGroup();
	settings.endGroup();
	*/
	//	if (action == 0) {

	/*
	if (science_2pass == 1) {
	  statusCPSBackgroundAction->setOn(false);
	  statusCPSChopnodAction->setOn(false);
	  statusCPSCollapseAction->setOn(false);
	  changestatus_set_imextension();
	  science_2pass = 0;
	  paint_checkboxes_helper(0, sf_2passCheckBox);
	}
	*/
      }
      else {
	sf_messagesTextEdit->append("Aborted.");
	sf_messagesTextEdit->scrollToBottom();
      } 
      sf_messagesTextEdit->append("\n");
    }
    
    if (process_group.compare("w") == 0) {
      if (proc_w->normalExit()) 
	show_scriptlog(w_messagesTextEdit, errormessage);
      else {
	w_messagesTextEdit->append("Aborted.");
	w_messagesTextEdit->scrollToBottom();
      } 
      w_messagesTextEdit->append("\n");
    } 
    
    if (process_group.compare("ap") == 0) {
      if (proc_ap->normalExit()) 
	show_scriptlog(ap_messagesTextEdit, errormessage);
      else {
	ap_messagesTextEdit->append("Aborted.");
	ap_messagesTextEdit->scrollToBottom();
      } 
      ap_messagesTextEdit->append("\n");
    } 
    
    if (process_group.compare("cc") == 0) {
      if (proc_cc->normalExit()) 
	show_scriptlog(cc_messagesTextEdit, errormessage);
      else {
	cc_messagesTextEdit->append("Aborted.");
	cc_messagesTextEdit->scrollToBottom();
      } 
      cc_messagesTextEdit->append("\n");
    } 
  }    
}


void theliForm::write_superscriptbody(QString fullpath, QString scripts)
{
  FILE *of, *of2;
  QString reply, command;
  int i;
  
  // leave if the process is already running!
  
  if (
      (process_group.compare("pre") == 0 && started_proc_pre == 1) ||
      (process_group.compare("cal") == 0 && started_proc_cal == 1) ||
      (process_group.compare("sf")  == 0 && started_proc_sf  == 1) ||
      (process_group.compare("w")   == 0 && started_proc_w   == 1) ||
      (process_group.compare("ap")  == 0 && started_proc_ap  == 1) ||
      (process_group.compare("cc")  == 0 && started_proc_cc  == 1) ||
      (process_group.compare("del") == 0 && started_proc_del == 1))
    return;
  
  
  // get the instrument name
  QString instrument = in_instrumentListBox->currentText();
  
  QString historypath = homedir;
  historypath.append("/.theli/reduction_logs/");
  QString tmp = in_reductionlogLineEdit->text();
  historypath.append(tmp);
  historypath.append(".com");
  
  // write everything into the file
  of = fopen(fullpath, "w");
  
  fprintf(of, "#!%s\n\n", bash.ascii());
  fprintf(of, "export INSTRUMENT=%s\n", instrument.ascii());
  fprintf(of, "cd %s\n\n", scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  
  tmp = fullpath;
  tmp.append("_tmp");
  of2 = fopen(tmp, "w");
  fprintf(of2,"%s\n",scripts.ascii());
  fclose(of2);
  
  fullpathlog = fullpath;
  fullpathlog.append(".log");
  
  // append the commands to the command history
  QString execstr = "more ";
  execstr.append(tmp);
  execstr.append(" >> ");
  execstr.append(historypath);
  my_system(execstr);
  
  // get the number of commands issued
  execstr = "gawk 'END {print NR}' ";
  execstr.append(tmp);
  
  my_fgets(execstr, command);
  
  int numcommands = atoi(command);
  
  if (scripts.length() > 0) {
    
    // now write each command to a script file and execute
    concatenate_logs = "cat ";
    remove_individuallogs = "\\rm -f ";
    for (i=1; i<=numcommands; i++) {
      QString i_string;
      i_string = i_string.setNum( i );
      execstr = "gawk '(NR == ";
      execstr.append(i_string);
      execstr.append(") {print $0}' ");
      execstr.append(tmp);
      
      my_fgets(execstr, command);
      
      int length = command.length() - 1;
      command.truncate(length);
      
      if (!command.isEmpty() ) {
	// get the execution directory
	QString execdir = scriptdir;
	QString path = userinstdir+instrument+".ini";
	QFile inst(path);
	if (inst.exists() && 
	    (command.contains("process_split_") || 
	     command.contains("make_album_"))) {
	  execdir = userscriptdir;
	}
	
	// get the command history file
	create_thelimessage(command,reply);
	fprintf(of, "echo %s\n", reply.ascii());
	fprintf(of, "\ncd %s\n", execdir.ascii());
	fprintf(of, "%s%s%s%d%s\n", command.ascii(), " > ", fullpathlog.ascii(), i, " 2>&1");
	fprintf(of, "\n\n");
	
	if (command.find("create_astrorefcat_") != -1) {
	  fprintf(of, "echo COUNTSTARS\n\n");
	}
	
	// prepare the concatenation of the individual log files
	concatenate_logs.append(fullpathlog);
	concatenate_logs.append(i_string);
	concatenate_logs.append(" ");
	// prepare the deletion of the individual log files
	remove_individuallogs.append(fullpathlog);
	remove_individuallogs.append(i_string);
	remove_individuallogs.append(" ");
      }
    }
    
    concatenate_logs.append(" > ");
    concatenate_logs.append(fullpathlog);
    concatenate_logs.append(" 2>&1");
    
    //	fprintf(of, "cd %s\n\n",startdir.ascii());
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
    if (process_group.compare("pre") == 0) {
      proc_pre = new QProcess( this );
      proc_pre->addArgument( fullpath );
      connect(proc_pre, SIGNAL(readyReadStderr()), this, SLOT(updateError()));
      connect(proc_pre, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
      connect(proc_pre, SIGNAL(processExited()), this, SLOT(updateExit()));
      if (started_proc_pre == 0) {
	while( !proc_pre->isRunning() ) proc_pre->launch("");
	started_proc_pre = 1;
      }
    }
    
    if (process_group.compare("cal") == 0) {
      proc_cal = new QProcess( this );
      proc_cal->addArgument( fullpath );
      connect(proc_cal, SIGNAL(readyReadStderr()), this, SLOT(updateError()));
      connect(proc_cal, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
      connect(proc_cal, SIGNAL(processExited()), this, SLOT(updateExit()));
      if (started_proc_cal == 0) {
	while( !proc_cal->isRunning() ) proc_cal->launch("");
	started_proc_cal = 1;
      }
    }
    
    if (process_group.compare("sf") == 0) {
      proc_sf = new QProcess( this );
      proc_sf->addArgument( fullpath );
      connect(proc_sf, SIGNAL(readyReadStderr()), this, SLOT(updateError()));
      connect(proc_sf, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
      connect(proc_sf, SIGNAL(processExited()), this, SLOT(updateExit()));
      if (started_proc_sf == 0) {
	while( !proc_sf->isRunning() ) proc_sf->launch("");
	started_proc_sf = 1;
      }
    }
    
    if (process_group.compare("w") == 0) {
      proc_w = new QProcess( this );
      proc_w->addArgument( fullpath );
      connect(proc_w, SIGNAL(readyReadStderr()), this, SLOT(updateError()));
      connect(proc_w, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
      connect(proc_w, SIGNAL(processExited()), this, SLOT(updateExit()));
      if (started_proc_w == 0) {
	while( !proc_w->isRunning() ) proc_w->launch("");
	started_proc_w = 1;
      }
    }

    if (process_group.compare("ap") == 0) {
      proc_ap = new QProcess( this );
      proc_ap->addArgument( fullpath );
      connect(proc_ap, SIGNAL(readyReadStderr()), this, SLOT(updateError()));
      connect(proc_ap, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
      connect(proc_ap, SIGNAL(processExited()), this, SLOT(updateExit()));
      if (started_proc_ap == 0) {
	while( !proc_ap->isRunning() ) proc_ap->launch("");
	started_proc_ap = 1;
      }
    }
    
    if (process_group.compare("cc") == 0) {
      proc_cc = new QProcess( this );
      proc_cc->addArgument( fullpath );
      connect(proc_cc, SIGNAL(readyReadStderr()), this, SLOT(updateError()));
      connect(proc_cc, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
      connect(proc_cc, SIGNAL(processExited()), this, SLOT(updateExit()));
      if (started_proc_cc == 0) {
	while( !proc_cc->isRunning() ) proc_cc->launch("");
	started_proc_cc = 1;
      }   
    }
    
    if (process_group.compare("del") == 0) {
      proc_del = new QProcess( this );
      proc_del->addArgument( fullpath );
      connect(proc_del, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
      connect(proc_del, SIGNAL(processExited()), this, SLOT(updateExit()));
      if (started_proc_del == 0) {
	while( !proc_del->isRunning() ) proc_del->launch("");
	started_proc_del = 1;
      }   
    }
  }
}


void theliForm::update_presinst()
{
  int numinstruments = in_instrumentListBox->numRows();
  
  if (checkdir2(scriptdir) == 1 && numinstruments > 0) {
    QString presinst = in_instrumentListBox->currentText();
    tf_instrumentLineEdit->setText(presinst);
    tf_instrumentLineEdit->setPaletteBackgroundColor(tfgreen);
  }
  else {
    tf_instrumentLineEdit->setText("");
    tf_instrumentLineEdit->setPaletteBackgroundColor(tfred);
  }
  update_instrument_inlogfile();
  
  // check the instrument file has consistent INSTRUMENT keyword
  check_instname_consistency();
}


void theliForm::get_numchips()
{
  QString instname_prof = profinstdir;
  QString instname_comm = comminstdir;
  QString instname_user = userinstdir;
  
  int numinst = in_instrumentListBox->numRows();
  
  if (numinst > 0 && in_instrumentListBox->currentItem() != -1) {
    instname_prof.append(in_instrumentListBox->currentText());
    instname_comm.append(in_instrumentListBox->currentText());
    instname_user.append(in_instrumentListBox->currentText());
    instname_prof.append(".ini");
    instname_comm.append(".ini");
    instname_user.append(".ini");
    QFile camera_prof(instname_prof);
    QFile camera_comm(instname_comm);
    QFile camera_user(instname_user);
    
    if (camera_prof.exists()) get_fileparameter(instname_prof, "NCHIPS", numchips);
    if (camera_comm.exists()) get_fileparameter(instname_comm, "NCHIPS", numchips);
    if (camera_user.exists()) get_fileparameter(instname_user, "NCHIPS", numchips);
  }
  else numchips = "1";
}

void theliForm::get_cameratype()
{
  QString instname_prof = profinstdir;
  QString instname_comm = comminstdir;
  QString instname_user = userinstdir;
  
  int numinst = in_instrumentListBox->numRows();
  
  if (numinst > 0 && in_instrumentListBox->currentItem() != -1) {
    instname_prof.append(in_instrumentListBox->currentText());
    instname_comm.append(in_instrumentListBox->currentText());
    instname_user.append(in_instrumentListBox->currentText());
    instname_prof.append(".ini");
    instname_comm.append(".ini");
    instname_user.append(".ini");
    QFile camera_prof(instname_prof);
    QFile camera_comm(instname_comm);
    QFile camera_user(instname_user);
    
    if (camera_prof.exists()) get_fileparameter(instname_prof, "TYPE", cameratype);
    if (camera_comm.exists()) get_fileparameter(instname_comm, "TYPE", cameratype);
    if (camera_user.exists()) get_fileparameter(instname_user, "TYPE", cameratype);
  }
  else cameratype = "OPT";

  // show or hide corresponding elements

  if (cameratype.compare("MIR") == 0) {
    pre_splitMIRCheckBox->show();
    sf_chopnodredoPushButton->show();
    sf_chopnodCheckBox->setEnabled(true);
    sf_patternTextLabel->setEnabled(true);
    sf_chopnodpatternComboBox->setEnabled(true);
    sf_chopnodinvertCheckBox->setEnabled(true);
  }
  else {
    pre_splitMIRCheckBox->hide();
    sf_chopnodredoPushButton->hide();
    sf_chopnodCheckBox->setEnabled(false);
    sf_patternTextLabel->setEnabled(false);
    sf_chopnodpatternComboBox->setEnabled(false);
    sf_chopnodinvertCheckBox->setEnabled(false);
  }

  if (cameratype.compare("NIR") == 0) {
    sf_spreadsequenceCheckBox->setEnabled(true);
    sf_mergesequenceCheckBox->setEnabled(true);
    sf_spread2TextLabel->setEnabled(true);
    sf_spread2TextLabel->setEnabled(true);
    sf_spread_numbergroupsLineEdit->setEnabled(true);
    sf_spread_lengthsequenceLineEdit->setEnabled(true);
  }
  else {
    sf_spreadsequenceCheckBox->setEnabled(false);
    sf_mergesequenceCheckBox->setEnabled(false);
    sf_spread2TextLabel->setEnabled(false);
    sf_spread2TextLabel->setEnabled(false);
    sf_spread_numbergroupsLineEdit->setEnabled(false);
    sf_spread_lengthsequenceLineEdit->setEnabled(false);
  }

  if (cameratype.compare("OPT") == 0) {
      w_debloomCheckBox->setEnabled(true);
      w_bloomthreshTextLabel->setEnabled(true);
      w_bloomthreshLineEdit->setEnabled(true);
  }
  else {
      w_debloomCheckBox->setEnabled(false);
      w_bloomthreshTextLabel->setEnabled(false);
      w_bloomthreshLineEdit->setEnabled(false);
  }

}


void theliForm::modify_npara()
{
  FILE *of;
  QString line, ident, value, execstr;
  
  check_last_char(homedir);
  QString paramset1inifile = homedir;
  paramset1inifile.append(".theli/param_set1.ini");
  QString paramset1inifiletmp = homedir;
  paramset1inifiletmp.append(".theli/param_set1.ini_tmp");
  
  QString numcpu = in_numcpuSpinBox->cleanText();
  QString nframes = in_nframesSpinBox->cleanText();
  
  QFile file( paramset1inifile );
  if ( file.open( IO_ReadOnly )) {
    of = fopen(paramset1inifiletmp, "w");
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      QStringList lst = lst.split( "=", line );
      if (!line.isEmpty()) {
	ident = line.section('=',0,0);
	if (lst.count()>1) value = line.section('=',1,1);
	else value = "";

	// write the number of processors and nframes
	if (ident.compare("NPARA") != 0 && 
	    ident.compare("NFRAMES") != 0)
	  fprintf(of, "%s\n", line.ascii());
	else {
	  if (ident.compare("NPARA") == 0)
	    fprintf(of, "%s%s\n", "NPARA=", numcpu.ascii());
	  if (ident.compare("NFRAMES") == 0)
	    fprintf(of, "%s%s\n", "NFRAMES=", nframes.ascii());
	}
      }
    }
    file.close();
    fclose(of);
  }
  
  if (file.exists()) { // suppress a warning if theli is 
    // run the very first time
    execstr = "mv ";
    execstr.append(paramset1inifiletmp);
    execstr.append(" ");
    execstr.append(paramset1inifile);
    my_system(execstr);
  }
}


// this is to write default parameters into param_set2.ini if the
// the user did not call the configure dialog in a new log file.
// otherwise we just read what has been saved and rewrite 
// param_set2.ini from the QSettings (redundant)
void theliForm::modify_paramset2()
{
  FILE *of;

  QSettings settings(QSettings::Ini);
  QString execstr;

  //    if (!homedir.endsWith("/")) 
  //	homedir.append("/");

  check_last_char(homedir);

  QString paramsinifile = homedir;
  paramsinifile.append(".theli/param_set2.ini");
  QString paramsinifiletmp = homedir;
  paramsinifiletmp.append(".theli/param_set2.ini_tmp");

  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
  }

  of = fopen(paramsinifiletmp, "w");

  // read what is written in the Qt file
  settings.beginGroup("/theli_"+reductionlog+"_");

  // PREPARATION
  settings.beginGroup("/PREPARATION");
  bool tmp0 = settings.readBoolEntry("/rename_checkbox", false);
  if (tmp0) fprintf(of, "%s\n", "V_PRE_RENAME_CHECKED=1");
  else fprintf(of, "%s\n", "V_PRE_RENAME_CHECKED=0");
  fprintf(of, "%s%s\n", "V_RENAME_FITSKEY=",(settings.readEntry("/rename_fitskey", "").ascii()));
  fprintf(of, "%s%s\n", "V_SORT_FITSKEY=",(settings.readEntry("/sort_fitskey", "").ascii()));
  fprintf(of, "%s%s\n", "V_SORT_BIAS=",(settings.readEntry("/sort_bias", "").ascii()));
  fprintf(of, "%s%s\n", "V_SORT_DARK=",(settings.readEntry("/sort_dark", "").ascii()));
  fprintf(of, "%s%s\n", "V_SORT_DOMEFLAT=",(settings.readEntry("/sort_domeflat", "").ascii()));
  fprintf(of, "%s%s\n", "V_SORT_SKYFLAT=",(settings.readEntry("/sort_skyflat", "").ascii()));
  fprintf(of, "%s%s\n", "V_SORT_STD=",(settings.readEntry("/sort_std", "").ascii()));
  settings.beginGroup("/corrections");
  bool tmp1 = settings.readBoolEntry("/xtalk_norcheckbox", false);
  bool tmp2 = settings.readBoolEntry("/xtalk_rowcheckbox", false);
  bool tmp3 = settings.readBoolEntry("/xtalk_multicheckbox", false);
  if (tmp1) fprintf(of, "%s\n", "V_PRE_XTALK_NOR_CHECKED=1");
  else fprintf(of, "%s\n", "V_PRE_XTALK_NOR_CHECKED=0");
  if (tmp2) fprintf(of, "%s\n", "V_PRE_XTALK_ROW_CHECKED=1");
  else fprintf(of, "%s\n", "V_PRE_XTALK_ROW_CHECKED=0");
  if (tmp3) fprintf(of, "%s\n", "V_PRE_XTALK_MULTI_CHECKED=1");
  else fprintf(of, "%s\n", "V_PRE_XTALK_MULTI_CHECKED=0");
  fprintf(of, "%s%s\n", "V_PRE_XTALK_NOR_AMPLITUDE=",(settings.readEntry("/xtalk_nor_amplitude", "").ascii()));
  fprintf(of, "%s%s\n", "V_PRE_XTALK_ROW_AMPLITUDE=",(settings.readEntry("/xtalk_row_amplitude", "").ascii()));
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
  if (settings.readBoolEntry("/usedark",false)) 
    fprintf(of, "%s\n", "V_CAL_USEDARK=TRUE");
  else
    fprintf(of, "%s\n", "V_CAL_USEDARK=FALSE");
  settings.endGroup();

  // BACKGROUND
  settings.beginGroup("/BACKGROUND");
  settings.beginGroup("/background");
  QString server;
  int server_choice = settings.readNumEntry("/server",0);
  switch(server_choice) {
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
  fprintf(of, "%s%s\n", "V_BACK_COMBINEMETHOD=", (settings.readEntry("/combinemethod","MEDIAN").ascii()));
  /*
  int cal_combinemethod = settings.readNumEntry("/combinemethod",0);
  if (cal_combinemethod == 0)	fprintf(of, "%s\n", "V_CAL_COMBINEMETHOD=MEDIAN");
  if (cal_combinemethod == 1)	fprintf(of, "%s\n", "V_CAL_COMBINEMETHOD=MEAN");
  */
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
  int coll_direction = settings.readNumEntry("/direction",0);
  if (coll_direction == 0) fprintf(of, "%s\n", "V_COLLDIRECTION=x");
  if (coll_direction == 1) fprintf(of, "%s\n", "V_COLLDIRECTION=y");
  if (coll_direction == 2) fprintf(of, "%s\n", "V_COLLDIRECTION=xy");
  if (coll_direction == 3) fprintf(of, "%s\n", "V_COLLDIRECTION=xyyx");
  if (coll_direction == 4) fprintf(of, "%s\n", "V_COLLDIRECTION=yxxy");
  if (coll_direction == 5) fprintf(of, "%s\n", "V_COLLDIRECTION=xxxx");
  if (coll_direction == 6) fprintf(of, "%s\n", "V_COLLDIRECTION=yyyy");
  fprintf(of, "%s%s\n", "V_COLLXMIN=", (settings.readEntry("/xmin","")).ascii());
  fprintf(of, "%s%s\n", "V_COLLXMAX=", (settings.readEntry("/xmax","")).ascii());
  fprintf(of, "%s%s\n", "V_COLLYMIN=", (settings.readEntry("/ymin","")).ascii());
  fprintf(of, "%s%s\n", "V_COLLYMAX=", (settings.readEntry("/ymax","")).ascii());

  if (settings.readBoolEntry("/maskaction", true))
    fprintf(of, "%s\n", "V_COLLMASKACTION=1");
  else 
    fprintf(of, "%s\n", "V_COLLMASKACTION=0");	

  if (settings.readBoolEntry("/autothreshold", false))
    fprintf(of, "%s\n", "V_COLLAUTOTHRESHOLD=1");
  else
    fprintf(of, "%s\n", "V_COLLAUTOTHRESHOLD=0");
  settings.endGroup();
  settings.endGroup();

  // WEIGHTING
  settings.beginGroup("/WEIGHTING");
  QString darkmin, darkmax, flatmin, flatmax;
  flatmin = settings.readEntry("/normflatmin","0.5");
  flatmax = settings.readEntry("/normflatmax","1.6");
  darkmin = settings.readEntry("/darkmin","");
  darkmax = settings.readEntry("/darkmax","");

  fprintf(of, "%s%s\n", "V_WEIGHTBINMIN=", (settings.readEntry("/w_binmin","-100")).ascii());
  fprintf(of, "%s%s\n", "V_WEIGHTBINMAX=", (settings.readEntry("/w_binmax","500")).ascii());
  fprintf(of, "%s%s\n", "V_WEIGHTBINSIZE=", (settings.readEntry("/w_binsize","4")).ascii());
  fprintf(of, "%s%s\n", "V_WEIGHTLOWTHRESHOLD=", (settings.readEntry("/weightlowthreshold","")).ascii());
  fprintf(of, "%s%s\n", "V_WEIGHTHIGHTHRESHOLD=", (settings.readEntry("/weighthighthreshold","")).ascii());

  fprintf(of, "%s%s\n", "V_GLOBWFLATMIN=", flatmin.ascii());
  fprintf(of, "%s%s\n", "V_GLOBWFLATMAX=", flatmax.ascii());
  fprintf(of, "%s%s\n", "V_GLOBWDARKMIN=", darkmin.ascii());
  fprintf(of, "%s%s\n", "V_GLOBWDARKMAX=", darkmax.ascii());
  if (!darkmin.isEmpty() && !darkmax.isEmpty()) {
    if (settings.readBoolEntry("/usebias","false"))
      fprintf(of, "%s%s\n", "V_GLOBWDARKDIR=", (in_biasdirLineEdit->text()).ascii());
    else
      fprintf(of, "%s%s\n", "V_GLOBWDARKDIR=", (in_darkdirLineEdit->text()).ascii());
  }
  else
    fprintf(of, "%s\n", "V_GLOBWDARKDIR=");

  fprintf(of, "%s%s\n", "V_COSMICSTHRESHOLD=", (settings.readEntry("/cosmicthreshold","0.1")).ascii());
  fprintf(of, "%s%s\n", "V_COSMICDT=", (settings.readEntry("/cosmicDT","6")).ascii());
  fprintf(of, "%s%s\n", "V_COSMICDMIN=", (settings.readEntry("/cosmicDMIN","1")).ascii());
  fprintf(of, "%s%s\n", "V_DEFECT_KERNELSIZE=", (settings.readEntry("/defect_kernelsize","")).ascii());
  fprintf(of, "%s%s\n", "V_DEFECT_ROWTOL=", (settings.readEntry("/defect_rowtol","")).ascii());
  fprintf(of, "%s%s\n", "V_DEFECT_COLTOL=", (settings.readEntry("/defect_coltol","")).ascii());
  fprintf(of, "%s%s\n", "V_DEFECT_CLUSTOL=", (settings.readEntry("/defect_clustol","")).ascii());
  fprintf(of, "%s%s\n", "V_DEFECT_KERNELSIZE_SF=", (settings.readEntry("/defect_kernelsize_sf","")).ascii());
  fprintf(of, "%s%s\n", "V_DEFECT_ROWTOL_SF=", (settings.readEntry("/defect_rowtol_sf","")).ascii());
  fprintf(of, "%s%s\n", "V_DEFECT_COLTOL_SF=", (settings.readEntry("/defect_coltol_sf","")).ascii());
  fprintf(of, "%s%s\n", "V_DEFECT_CLUSTOL_SF=", (settings.readEntry("/defect_clustol_sf","")).ascii());
  fprintf(of, "%s%s\n", "V_BLOOMLOWLIMIT=", (settings.readEntry("/bloomlowlimit","20000")).ascii());
  fprintf(of, "%s%s\n", "V_BLOOMMINAREA=", (settings.readEntry("/bloomminarea","100")).ascii());
  fprintf(of, "%s%s\n", "V_BLOOMWIDTH=", (settings.readEntry("/bloomwidth","5000")).ascii());
  if (settings.readBoolEntry("/w_binoutlier",false)) 
    fprintf(of, "%s\n", "V_WEIGHT_BINOUTLIER=TRUE");
  else
    fprintf(of, "%s\n", "V_WEIGHT_BINOUTLIER=FALSE");
  if (settings.readBoolEntry("/uniformweight", false))
    fprintf(of, "%s\n", "V_GLOBW_UNIFORMWEIGHT=TRUE");
  else
    fprintf(of, "%s\n", "V_GLOBW_UNIFORMWEIGHT=FALSE");
  tmp1 = settings.readBoolEntry("/bloomdomask", false);
  if (tmp1) fprintf(of, "%s\n", "V_MASKBLOOMSPIKE=1");
  else fprintf(of, "%s\n", "V_MASKBLOOMSPIKE=0");
  settings.endGroup();

  // ASTROMPHOTOM
  settings.beginGroup("/ASTROMPHOTOM");
  settings.beginGroup("/sourcecat");
  fprintf(of, "%s%s\n", "V_AP_DETECTTHRESH=", (settings.readEntry("/detectthresh","10.0")).ascii());
  fprintf(of, "%s%s\n", "V_AP_DETECTMINAREA=", (settings.readEntry("/detectminarea","10")).ascii());
  fprintf(of, "%s%s\n", "V_DEBLENDMINCONT=", (settings.readEntry("/deblendmincont","0.0005")).ascii());
  fprintf(of, "%s%s\n", "V_AP_LOWNUM=", (settings.readEntry("/lownum","1")).ascii());
  fprintf(of, "%s%s\n", "V_SEXCATMINFWHM=", (settings.readEntry("/sexcatminfwhm","1.5")).ascii());
  fprintf(of, "%s%s\n", "V_SEXCATFLAG=", (settings.readEntry("/sexcatflag","0")).ascii());
  fprintf(of, "%s%s\n", "V_SATURATION=", (settings.readEntry("/saturation","")).ascii());
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
  fprintf(of, "%s%s\n", "V_SCAMP_FGROUPRADIUS=", (settings.readEntry("/fgroupradius","")).ascii());
  fprintf(of, "%s%s\n", "V_SCAMP_CROSSIDRADIUS=", (settings.readEntry("/crossidradius","")).ascii());
  fprintf(of, "%s%s\n", "V_SCAMP_ASTREFWEIGHT=", (settings.readEntry("/astrefweight","")).ascii());
  fprintf(of, "%s%s\n", "V_SCAMP_ASTRINSTRUKEY=", (settings.readEntry("/astrinstrukey","")).ascii());
  fprintf(of, "%s%s\n", "V_SCAMP_PHOTINSTRUKEY=", (settings.readEntry("/photinstrukey","")).ascii());
  fprintf(of, "%s%s\n", "V_SCAMP_POLYORDER=", (settings.readEntry("/distortorder","3")).ascii());
  fprintf(of, "%s%s\n", "V_SCAMP_POLYORDER2=", (settings.readEntry("/distortorder2","")).ascii());
  fprintf(of, "%s%s\n", "V_SCAMP_DISTORTGROUPS=", (settings.readEntry("/distortgroup","")).ascii());
  fprintf(of, "%s%s\n", "V_SCAMP_DISTORTKEYS=", (settings.readEntry("/distortkeys","")).ascii());
  int scamp_stability = settings.readNumEntry("/stability",0);
  int scamp_mosaictype = settings.readNumEntry("/mosaictype",0);
  int scamp_focalplane = settings.readNumEntry("/focalplane",0);
  if (scamp_stability == 0) fprintf(of, "%s\n", "V_SCAMP_STABILITY=INSTRUMENT");
  if (scamp_stability == 1) fprintf(of, "%s\n", "V_SCAMP_STABILITY=EXPOSURE");
  if (scamp_mosaictype == 0) fprintf(of, "%s\n", "V_SCAMP_MOSAICTYPE=UNCHANGED");
  if (scamp_mosaictype == 1) fprintf(of, "%s\n", "V_SCAMP_MOSAICTYPE=SAME_CRVAL");
  if (scamp_mosaictype == 2) fprintf(of, "%s\n", "V_SCAMP_MOSAICTYPE=SHARE_PROJAXIS");
  if (scamp_mosaictype == 3) fprintf(of, "%s\n", "V_SCAMP_MOSAICTYPE=FIX_FOCALPLANE");
  if (scamp_mosaictype == 4) fprintf(of, "%s\n", "V_SCAMP_MOSAICTYPE=LOOSE");
  if (scamp_focalplane == 0) fprintf(of, "%s\n", "V_SCAMP_FOCALPLANE=DEFAULT");
  if (scamp_focalplane == 1) fprintf(of, "%s\n", "V_SCAMP_FOCALPLANE=NEW");
  if (scamp_focalplane == 2) fprintf(of, "%s\n", "V_SCAMP_FOCALPLANE=NONE");
  if (settings.readBoolEntry("/matchflipped",true)) 
    fprintf(of, "%s\n", "V_SCAMP_MATCHFLIPPED=Y");
  else
    fprintf(of, "%s\n", "V_SCAMP_MATCHFLIPPED=N");
  if (settings.readBoolEntry("/matchWCS",true)) 
    fprintf(of, "%s\n", "V_SCAMP_MATCHWCS=Y");
  else
    fprintf(of, "%s\n", "V_SCAMP_MATCHWCS=N");
  settings.endGroup();

  settings.beginGroup("/anet");
  fprintf(of, "%s%s\n", "V_ANET_MAXSCALE=", (settings.readEntry("/maxscale","1.05")).ascii());
  fprintf(of, "%s%s\n", "V_ANET_POLYORDER=", (settings.readEntry("/distortorder","3")).ascii());
  if (settings.readBoolEntry("/scampdistort",true)) 
    fprintf(of, "%s\n", "V_ANET_SCAMPDISTORT=Y");
  else
    fprintf(of, "%s\n", "V_ANET_SCAMPDISTORT=N");
  settings.endGroup();

  settings.beginGroup("/absphotom");
  fprintf(of, "%s%s\n", "V_ABSPHOT_APERTURE=",    (settings.readEntry("/aperture","10")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_FILTER=",      (settings.readEntry("/obsfilter","B")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_COLORFIXED=",  (settings.readEntry("/colorfixed","0.0")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_EXTINCTION=",  (settings.readEntry("/extinction","-0.1")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_ZPMIN=",       (settings.readEntry("/zpmin")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_ZPMAX=",       (settings.readEntry("/zpmax")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_KAPPA=",       (settings.readEntry("/kappa","2.0")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_THRESHOLD=",   (settings.readEntry("/threshold","0.15")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_MAXITER=",     (settings.readEntry("/maxiter","10")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_CONVERGENCE=", (settings.readEntry("/convergence","0.01")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_STDMINMAG=",   (settings.readEntry("/stdminmag","0.0")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_MAXPHOTERR=",  (settings.readEntry("/maxphoterror","0.05")).ascii());
  fprintf(of, "%s%s\n", "V_ABSPHOT_WCSERR=",      (settings.readEntry("/wcserror","10")).ascii());
  if (settings.readBoolEntry("/wcserrorCheckBox", false))
    fprintf(of, "%s\n", "V_ABSPHOT_WCSERRCHECKBOX=Y");
  else fprintf(of, "%s\n", "V_ABSPHOT_WCSERRCHECKBOX=N");

  int stdcat_direct = settings.readNumEntry("/stdcat_direct",0);
  int stdcat_indirect = settings.readNumEntry("/stdcat_indirect",0);
  int stdfilterM2 = settings.readNumEntry("/stdfilterM2",0);
  int stdfilter = settings.readNumEntry("/stdfilter",0);
  int stdcolor = settings.readNumEntry("/stdcolor",0);
  
  // DIRECT: SDSS
  if (stdcat_direct == 0) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_DIRECT=SDSS");
    if (stdfilterM2 == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTERM2=u");
    if (stdfilterM2 == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTERM2=g");
    if (stdfilterM2 == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTERM2=r");
    if (stdfilterM2 == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTERM2=i");
    if (stdfilterM2 == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTERM2=z");
  }

  // DIRECT: 2MASS
  if (stdcat_direct == 1) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_DIRECT=2MASS");
    if (stdfilterM2 == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTERM2=J");
    if (stdfilterM2 == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTERM2=H");
    if (stdfilterM2 == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTERM2=K");
  }

  // INDIRECT: LANDOLT
  if (stdcat_indirect == 0) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=LANDOLT_UBVRI");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=U");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=B");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=V");
    if (stdfilter == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=R");
    if (stdfilter == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=I");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=UmB");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=BmV");
    if (stdcolor == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=VmR");
    if (stdcolor == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=RmI");
    if (stdcolor == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=VmI");
  }

  // INDIRECT: STETSON
  if (stdcat_indirect == 1) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=STETSON_UBVRI");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=U");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=B");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=V");
    if (stdfilter == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=R");
    if (stdfilter == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=I");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=UmB");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=BmV");
    if (stdcolor == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=VmR");
    if (stdcolor == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=RmI");
    if (stdcolor == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=VmI");
  }

  // INDIRECT: STRIPE82_ugriz
  if (stdcat_indirect == 2) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=STRIPE82_ugriz");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=u");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=g");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=r");
    if (stdfilter == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=i");
    if (stdfilter == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=z");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=umg");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=gmr");
    if (stdcolor == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=rmi");
    if (stdcolor == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=imz");
  }

  // INDIRECT: STRIPE82_u'g'r'i'z'
  if (stdcat_indirect == 3) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=STRIPE82_ugriz_primed");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=u");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=g");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=r");
    if (stdfilter == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=i");
    if (stdfilter == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=z");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=umg");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=gmr");
    if (stdcolor == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=rmi");
    if (stdcolor == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=imz");
  }

  // INDIRECT: SMITH_u'g'r'i'z'
  if (stdcat_indirect == 4) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=SMITH_ugriz_primed");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=u");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=g");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=r");
    if (stdfilter == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=i");
    if (stdfilter == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=z");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=umg");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=gmr");
    if (stdcolor == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=rmi");
    if (stdcolor == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=imz");
  }

  // INDIRECT: MKO_JHK
  if (stdcat_indirect == 5) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=MKO_JHK");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=J");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=H");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=K");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=JmH");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=HmK");
  }

  // INDIRECT: HUNT_JHK
  if (stdcat_indirect == 6) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=HUNT_JHK");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=J");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=H");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=K");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=JmH");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=HmK");
  }

  // INDIRECT: 2MASSfaint_JHK
  if (stdcat_indirect == 7) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=2MASSfaint_JHK");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=J");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=H");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=K");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=JmH");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=HmK");
  }

  // INDIRECT: PERSSON_JHKKs
  if (stdcat_indirect == 8) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=PERSSON_JHKKs");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=J");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=H");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=K");
    if (stdfilter == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=Ks");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=JmH");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=HmK");
    if (stdcolor == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=HmKs");
  }

  // INDIRECT: JAC_YJHKLM
  if (stdcat_indirect == 9) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=JAC_YJHKLM");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=Y");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=J");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=H");
    if (stdfilter == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=K");
    if (stdfilter == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=L");
    if (stdfilter == 5) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=M");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=YmJ");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=JmH");
    if (stdcolor == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=HmK");
    if (stdcolor == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=KmL");
    if (stdcolor == 4) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=LmM");
  }

  // INDIRECT: MKO_LM
  if (stdcat_indirect == 10) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=MKO_LM");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=L");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=M");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=LmM");
  }

  // INDIRECT: WASHINGTON
  if (stdcat_indirect == 11) {
    fprintf(of, "%s\n", "V_ABSPHOT_STDCAT_INDIRECT=WASHINGTON");
    if (stdfilter == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=C");
    if (stdfilter == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=M");
    if (stdfilter == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=T1");
    if (stdfilter == 3) fprintf(of, "%s\n", "V_ABSPHOT_STDFILTER=T2");
    if (stdcolor == 0) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=CmM");
    if (stdcolor == 1) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=MmT1");
    if (stdcolor == 2) fprintf(of, "%s\n", "V_ABSPHOT_STDCOLOR=T1mT2");
  }

  int calmode = settings.readNumEntry("/calmode",0);
  if (calmode == 0) fprintf(of, "%s\n", "V_ABSPHOT_CALMODE=RUNCALIB");
  if (calmode == 1) fprintf(of, "%s\n", "V_ABSPHOT_CALMODE=NIGHTCALIB");

  int fittingmethodM2 = settings.readNumEntry("/fittingmethodM2",0);
  if (fittingmethodM2 == 0) fprintf(of, "%s\n", "V_ABSPHOT_FITTINGMETHODM2=CHIP");
  if (fittingmethodM2 == 1) fprintf(of, "%s\n", "V_ABSPHOT_FITTINGMETHODM2=MOSAIC");

  settings.endGroup();

  settings.beginGroup("/absphotom_nightselection");
  QString photcalstring = settings.readEntry("/nightselection","");
  fprintf(of, "%s%s\n", "V_ABSPHOT_NIGHTSELECTION=", photcalstring.ascii());  
  settings.endGroup();
  
  settings.endGroup();

  // COADDITION
  settings.beginGroup("/COADDITION");
  settings.beginGroup("/coaddition");
  fprintf(of, "%s%s\n", "V_COADD_REFRA=",    (settings.readEntry("/ra","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_REFDEC=",   (settings.readEntry("/dec","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_IDENT=",    (settings.readEntry("/scriptid","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_SEEING=",   (settings.readEntry("/seeing","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_RZP=",      (settings.readEntry("/rzp","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_PIXSCALE=", (settings.readEntry("/pixscale","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_SIZEX=", (settings.readEntry("/coaddsizex","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_SIZEY=", (settings.readEntry("/coaddsizey","")).ascii());
  if (settings.readEntry("/skyposangle","").isEmpty())
  	fprintf(of, "%s\n", "V_COADD_SKYPOSANGLE=");
  else {
    float skypa = -1.0 * (settings.readEntry("/skyposangle","")).toFloat();
    //float skypa = (settings.readEntry("/skyposangle","")).toFloat();
    fprintf(of, "%s%f\n", "V_COADD_SKYPOSANGLE=", skypa);
  }
  fprintf(of, "%s%s%s\n", "V_COADD_CHIPS=\"",    (settings.readEntry("/chips","")).ascii(), "\"");
  fprintf(of, "%s%s\n", "V_COADD_FILTERTHRESHOLD=", (settings.readEntry("/filterthreshold","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_FILTERCLUSTERSIZE=", (settings.readEntry("/filterclustersize","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_FILTERBORDERWIDTH=", (settings.readEntry("/filterborderwidth","")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_SMOOTHEDGE=", (settings.readEntry("/smoothedge","")).ascii());


  // evaluate the proper motion
  double speedcorrection;

  int speed_ind = settings.readNumEntry("/speedunit",1);
  if (speed_ind == 0) speedcorrection = -60.;
  if (speed_ind == 1) speedcorrection = -1.;
  if (speed_ind == 2) speedcorrection = -1/60.;
  if (speed_ind == 3) speedcorrection = -1/1440.;
  QString test_ra = settings.readEntry("/propermotionra","");
  QString test_dec = settings.readEntry("/propermotiondec","");

  if (!test_ra.isEmpty() && !test_dec.isEmpty()) {
    double propmotionra  = speedcorrection * settings.readEntry("/propermotionra","").toFloat();
    double propmotiondec = speedcorrection * settings.readEntry("/propermotiondec","").toFloat();
    fprintf(of, "%s%lf\n", "V_COADD_PROPMOTIONRA=", propmotionra);
    fprintf(of, "%s%lf\n", "V_COADD_PROPMOTIONDEC=", propmotiondec);
  }
  else {
    fprintf(of, "%s\n", "V_COADD_PROPMOTIONRA=");
    fprintf(of, "%s\n", "V_COADD_PROPMOTIONDEC=");
  }

  int kernel = settings.readNumEntry("/kernel",3);
  int combinetype = settings.readNumEntry("/combinetype",0);
  if (kernel == 0) fprintf(of, "%s\n", "V_COADD_KERNEL=NEAREST");
  if (kernel == 1) fprintf(of, "%s\n", "V_COADD_KERNEL=BILINEAR");
  if (kernel == 2) fprintf(of, "%s\n", "V_COADD_KERNEL=LANCZOS2");
  if (kernel == 3) fprintf(of, "%s\n", "V_COADD_KERNEL=LANCZOS3");
  if (kernel == 4) fprintf(of, "%s\n", "V_COADD_KERNEL=LANCZOS4");
  fprintf(of, "%s%s\n", "V_COADD_FILTER=",        (settings.readEntry("/coaddfiltertext","All")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_PROJECTION=",    (settings.readEntry("/projection2","TAN")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_CELESTIALTYPE=", (settings.readEntry("/celestialtype2", "EQUATORIAL")).ascii());
  if (combinetype == 0) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=WEIGHTED");
  if (combinetype == 1) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=MEDIAN");
  if (combinetype == 2) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=AVERAGE");
  if (combinetype == 3) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=MIN");
  if (combinetype == 4) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=MAX");
  if (combinetype == 5) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=CHI2");
  /*
  if (combinetype == 1) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=CLIPPED");
  if (combinetype == 2) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=MEDIAN");
  if (combinetype == 3) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=AVERAGE");
  if (combinetype == 4) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=MIN");
  if (combinetype == 5) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=MAX");
  if (combinetype == 6) fprintf(of, "%s\n", "V_COADD_COMBINETYPE=CHI2");
  */

  if (settings.readBoolEntry("/rescaleweights", false))
    fprintf(of, "%s\n", "V_COADD_RESCALEWEIGHTS=Y");
  else fprintf(of, "%s\n", "V_COADD_RESCALEWEIGHTS=N");

  fprintf(of, "%s%s\n", "V_COADD_CLIPAMPFRAC=", (settings.readEntry("/clipampfrac","0.3")).ascii());
  fprintf(of, "%s%s\n", "V_COADD_CLIPSIGMA=",   (settings.readEntry("/clipsigma","4")).ascii());

  settings.endGroup();
  settings.endGroup();

  settings.endGroup();

  fclose(of);

  execstr = "mv ";
  execstr.append(paramsinifiletmp);
  execstr.append(" ");
  execstr.append(paramsinifile);

  my_system(execstr);    
}


// this is to write default parameters into param_set3.ini if the
// the user did not call the configure dialog in a new log file.
// otherwise we just read what has been saved and rewrite 
// param_set3.ini from the QSettings (redundant)
void theliForm::modify_paramset3()
{
  FILE *of;

  QSettings settings(QSettings::Ini);
  QString execstr;

  //    if (!scriptdir.endsWith("/")) 
  //	scriptdir.append("/");

  check_last_char(homedir);

  QString paramsinifile = homedir;
  paramsinifile.append(".theli/param_set3.ini");
  QString paramsinifiletmp = homedir;
  paramsinifiletmp.append(".theli/param_set3.ini_tmp");

  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
  	reductionlog = "noname";
  }

  of = fopen(paramsinifiletmp, "w");

  // read what is written in the Qt file
  settings.beginGroup("/theli_"+reductionlog+"_");

  // COADDITION
  settings.beginGroup("/COADDITION");
  settings.beginGroup("/skysub");
  fprintf(of, "%s%s\n", "V_SKYSUBDETECTTHRESH=",  (settings.readEntry("/detectthresh","1.5")).ascii());
  fprintf(of, "%s%s\n", "V_SKYSUBDETECTMINAREA=", (settings.readEntry("/detectminarea","5")).ascii());
  fprintf(of, "%s%s\n", "V_SKYSUBBACKSIZE=",      (settings.readEntry("/backsize","256")).ascii());
  fprintf(of, "%s%s\n", "V_SKYSUBMASKEXPAND=",    (settings.readEntry("/maskexpand","")).ascii());
  if (settings.readBoolEntry("/saveskymodel",false)) fprintf(of, "%s\n", "V_SAVESKYMODEL=Y");
  else fprintf(of, "%s\n", "V_SAVESKYMODEL=N");
  fprintf(of, "%s%s\n", "V_CSKY_XMIN=",  (settings.readEntry("/cskyxmin","")).ascii());
  fprintf(of, "%s%s\n", "V_CSKY_XMAX=",  (settings.readEntry("/cskyxmax","")).ascii());
  fprintf(of, "%s%s\n", "V_CSKY_YMIN=",  (settings.readEntry("/cskyymin","")).ascii());
  fprintf(of, "%s%s\n", "V_CSKY_YMAX=",  (settings.readEntry("/cskyymax","")).ascii());
  fprintf(of, "%s%s\n", "V_CSKY_RA1=",   (settings.readEntry("/cskyra1","")).ascii());
  fprintf(of, "%s%s\n", "V_CSKY_RA2=",   (settings.readEntry("/cskyra2","")).ascii());
  fprintf(of, "%s%s\n", "V_CSKY_DEC1=",  (settings.readEntry("/cskydec1","")).ascii());
  fprintf(of, "%s%s\n", "V_CSKY_DEC2=",  (settings.readEntry("/cskydec2","")).ascii());
  fprintf(of, "%s%s\n", "V_CSKYMANUAL=", (settings.readEntry("/manual","")).ascii());
  fprintf(of, "%s%d\n", "V_CSKYMETHOD=", settings.readNumEntry("/method",0));
  settings.endGroup();
  settings.endGroup();

  settings.endGroup();

  fclose(of);

  execstr = "mv ";
  execstr.append(paramsinifiletmp);
  execstr.append(" ");
  execstr.append(paramsinifile);

  my_system(execstr);    
}


void theliForm::modify_paramsini()
{
  FILE *of;
    
  QString tmp, execstr, server; 
  QString v_do_bias, v_do_flat, v_nonlincorr, v_splitMIR;

  check_last_char(homedir);
  QString paramsinifile = homedir;
  paramsinifile.append(".theli/param_set1.ini");
  QString paramsinifiletmp = homedir;
  paramsinifiletmp.append(".theli/param_set1.ini_tmp");

  QString numcpu = in_numcpuSpinBox->cleanText();
  QString nframes = in_nframesSpinBox->cleanText();
  if (cal_nobiasCheckBox->isChecked()) v_do_bias = "N";
  else v_do_bias = "Y";
  if (cal_noflatCheckBox->isChecked()) v_do_flat = "N";
  else v_do_flat = "Y";
  if (cal_nonlinCheckBox->isChecked()) v_nonlincorr = "Y";
  else v_nonlincorr = "N";
  if (pre_splitMIRCheckBox->isChecked()) v_splitMIR = "Y";
  else v_splitMIR = "N";

  // if the user enters non-numeric characters or just empty spaces
  QString v_ap_radius = ap_radiusLineEdit->text();
  QString v_ap_maglim = ap_maglimLineEdit->text();
  QString v_ap_refra  = ap_refraLineEdit->text();
  QString v_ap_refdec = ap_refdecLineEdit->text();

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

  if (v_ap_refra.isEmpty() || v_ap_refra.compare("from header") == 0) 
    v_ap_refra="header";
  if (v_ap_refdec.isEmpty() || v_ap_refdec.compare("from header") == 0) 
    v_ap_refdec="header";

  of = fopen(paramsinifiletmp, "w");

  QString linuxkernel;
  my_fscanf("uname -a", linuxkernel);

  fprintf(of, "%s%s\n", "PROJECTNAME=", (in_reductionlogLineEdit->text()).ascii());
  fprintf(of, "%s%s\n", "NPARA=", numcpu.ascii());
  fprintf(of, "%s%s\n", "GUIVERSION=", guiversion.ascii());
  fprintf(of, "%s%s\n", "KERNEL=", linuxkernel.ascii());
  fprintf(of, "%s%s\n", "NFRAMES=", nframes.ascii());
  fprintf(of, "%s%s\n", "V_PRE_SPLITMIRCUBE=", v_splitMIR.ascii());
  fprintf(of, "%s%s\n", "V_DO_BIAS=", v_do_bias.ascii());
  fprintf(of, "%s%s\n", "V_DO_FLAT=", v_do_flat.ascii());
  fprintf(of, "%s%s\n", "V_NONLINCORR=", v_nonlincorr.ascii());
  fprintf(of, "%s%s\n", "V_AP_SERVER=", server.ascii());
  fprintf(of, "%s%s\n", "V_AP_MAGLIM=", v_ap_maglim.ascii());
  fprintf(of, "%s%s\n", "V_AP_RADIUS=", v_ap_radius.ascii());
  fprintf(of, "%s%s\n", "V_AP_REFRA=", v_ap_refra.ascii());
  fprintf(of, "%s%s\n", "V_AP_REFDEC=", v_ap_refdec.ascii());
  fclose(of);
  
  execstr = "mv ";
  execstr.append(paramsinifiletmp);
  execstr.append(" ");
  execstr.append(paramsinifile);
  my_system(execstr);
}


void theliForm::restore_BIAS()
{
  QString datasubdir = in_biasdirLineEdit->text();
  restore_subdirs(datasubdir);
}


void theliForm::restore_DARK()
{
  QString datasubdir = in_darkdirLineEdit->text();
  restore_subdirs(datasubdir);
}


void theliForm::restore_FLAT()
{
  QString datasubdir = in_flatdirLineEdit->text();
  restore_subdirs(datasubdir);
}


void theliForm::restore_FLATOFF()
{
  QString datasubdir = in_flatoffdirLineEdit->text();
  restore_subdirs(datasubdir);
}


void theliForm::restore_SCIENCE()
{
  QString datasubdir = in_sciencedirLineEdit->text();
  restore_subdirs(datasubdir);
  
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
  
  // force the update; this creates the log file in case it doesn't yet exist
  update_logfile();
  
  // get the log file name
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
  
  QString filename_in = homedir;
  filename_in.append("/.theli/reduction_logs/");
  QString tmp = in_reductionlogLineEdit->text();
  filename_in.append(tmp);
  QString history = filename_in;
  history.append(".com");
  
  // update the logfile    
  settings.writeEntry("/PROCESSINGSTATUS/status", "000000");
  settings.beginGroup("/CALIBRATION");
    settings.writeEntry("/processsciencescript", 0);
  settings.endGroup();
    settings.beginGroup("/BACKGROUND");
      settings.writeEntry("/spreadsequencescript", 0);
      settings.writeEntry("/backgroundscript", 0);
      settings.writeEntry("/mergesequencescript", 0);
      settings.writeEntry("/chopnodscript", 0);
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
  QString execstr = "touch ";
  execstr.append(history);
  my_system(execstr);
  
  // reset the fields
  resetfields_cal_pushbutton();
  resetfields_sf_pushbutton();
  resetfields_w_pushbutton();
  resetfields_ap_pushbutton();
  resetfields_cc_pushbutton();
  
  // clear all the scripts
  clearscripts_cal_pushbutton();
  clearscripts_sf_pushbutton();
  clearscripts_w_pushbutton();
  clearscripts_ap_pushbutton();
  clearscripts_cc_pushbutton();
  
  // apply the changes to the GUI
  apply_logfile();
  
  // the menubar status has to be cleared separately
  statusCPSDebiasingAction->setOn(false);
  statusCPSBackgroundAction->setOn(false);
  statusCPSChopnodAction->setOn(false);
  statusCPSCollapseAction->setOn(false);
  statusCPSDebloomingAction->setOn(false);
  statusCPSDirectphotAction->setOn(false);
  
  // clear the checkboxes
  paint_checkboxes();
  
  // reset the image extension
  // for (i=0; i<6; i++) ext_img[i] = 0;
}


void theliForm::restore_SKY()
{
  QString datasubdir = in_skydirLineEdit->text();
  restore_subdirs(datasubdir);
}


void theliForm::restore_STANDARD()
{
  QString datasubdir = in_standarddirLineEdit->text();
  restore_subdirs(datasubdir);
}


void theliForm::restore_ALL()
{
  restore_BIAS();
  restore_DARK();
  restore_FLAT();
  restore_FLATOFF();
  restore_SCIENCE();
  restore_SKY();
  restore_STANDARD();
}


void theliForm::restore_subdirs(QString datasubdir)
{
  QString execstr, rmfiledir;
  
  QString maindir = in_maindirLineEdit->text();
  check_last_char(maindir);
  QString subdir = maindir;
  subdir.append(datasubdir);
  QString subdirORIG = subdir;
  check_last_char(subdirORIG);
  subdirORIG.append("ORIGINALS");
  QString subdirORIGhidden = subdir;
  check_last_char(subdirORIGhidden);
  subdirORIGhidden.append(".ORIGINALS");
  
  QString subdirREG = subdir;
  check_last_char(subdirREG);
  subdirREG.append("reg");
  QString subdirREGhidden = subdir;
  check_last_char(subdirREGhidden);
  subdirREGhidden.append(".reg");
  
  QFile truefile;
  QDir truedir;
  QFileInfo dirinfo;
  QFileInfo fileinfo = QFileInfo(subdir);
  QFileInfo originfo = QFileInfo(subdirORIG);
  QFileInfo reginfo = QFileInfo(subdirREG);

  int hidemasks = 0;

  // if the directory exists and contains an ORIGINALS subdirectory, 
  // then delete the images therein and play back the files that 
  // are in ORIGINALS
  if (fileinfo.isDir() && originfo.exists() && 
      originfo.isDir()) {
    // hide the ORIGINALS directory
    execstr = "\\mv ";
    execstr.append(subdirORIG);
    execstr.append(" ");
    execstr.append(subdirORIGhidden);
    my_system(execstr);

    // hide the reg directory if it exists
    QDir regdir;
    regdir.setPath(subdir);
    QStringList reglist = regdir.entryList( "*.reg" ); 
    if ((reginfo.exists() && reginfo.isDir()) || !reglist.isEmpty()) {
      hidemasks = 1;
      execstr = "mkdir ";
      execstr.append(subdirREGhidden);
      my_system(execstr);

      if (reginfo.exists() && reginfo.isDir()) {
	execstr = "\\mv ";
	execstr.append(subdirREG);
	execstr.append(" ");
	execstr.append(subdirREGhidden);
	my_system(execstr);
      }
     
      if (!reglist.isEmpty()) {
	execstr = "\\mv "+subdir+"/*.reg "+subdirREGhidden;
	my_system(execstr);
      }
    }

    QDir srcdir;
    QString qsrcdir = subdir;
    srcdir.setPath(subdir);
    // the list that contains all files and dirs in subdir
    QStringList lst = srcdir.entryList( "*" );
    // loop over all list entries, in order to find links
    for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it ) {
      QString file_or_dir = ( qsrcdir + "/" + *it );
      QFileInfo fileinfo2 = QFileInfo(file_or_dir);
      // check if the file / dir is a symbolic link
      if (fileinfo2.isSymLink()) {
	QString true_file_position = fileinfo2.readLink();
	QFileInfo fileinfo3 = QFileInfo(true_file_position);
	// if the file / dir is a link, then delete the 
	// file / dir to which the link points
	if (fileinfo3.exists()) {
	  rmfiledir = "\\rm -rf ";
	  rmfiledir.append(true_file_position);
	  my_system(rmfiledir);
	}
      }
    }
    // delete all data in the directory (the ORIGINALS and the masks are saved 
    // since they are hidden)
    rmfiledir = "\\rm -rf ";
    check_last_char(subdir);
    rmfiledir.append(subdir);
    rmfiledir.append("*");
    my_system(rmfiledir);
    
    // move back the images that are in the hidden ORIGINALS directory
    subdirORIG = subdir;
    check_last_char(subdirORIG);
    subdirORIG.append(".ORIGINALS/*");
    execstr = "\\mv ";
    execstr.append(subdirORIG);
    execstr.append(" ");
    execstr.append(subdir);
    execstr.append(" > /dev/null 2>&1");
    my_system(execstr);

    // delete the hidden ORIGINALS directory
    subdirORIG = subdir;
    check_last_char(subdirORIG);
    subdirORIG.append(".ORIGINALS");
    execstr = "\\rm -rf ";
    execstr.append(subdirORIG);
    my_system(execstr);

    // move back the masks that are in the hidden .reg directory
    if (hidemasks == 1) {
      subdirREG = subdir;
      check_last_char(subdirREG);
      subdirREG.append(".reg/*");
      execstr = "\\mv ";
      execstr.append(subdirREG);
      execstr.append(" ");
      execstr.append(subdir);
      execstr.append(" > /dev/null 2>&1");
      my_system(execstr);
    
      // delete the hidden .reg directory
      subdirREG = subdir;
      check_last_char(subdirREG);
      subdirREG.append(".reg");
      execstr = "\\rm -rf ";
      execstr.append(subdirREG);
      my_system(execstr);
    }
  }
}


void theliForm::paint_checkboxes_helper(int key, QCheckBox *myCheckBox)
{
  if (key == 1) myCheckBox->setPaletteBackgroundColor(tfgreen);
  else myCheckBox->setPaletteBackgroundColor(QColor(216,216,216));
}


void theliForm::paint_checkboxes()
{
  // preparation
  paint_checkboxes_helper(CB_sortrawdata, pre_sortdataCheckBox);
  paint_checkboxes_helper(CB_processsplit, pre_splitCheckBox);
  paint_checkboxes_helper(CB_createlinks, pre_createlinksCheckBox);
  // calibration
  paint_checkboxes_helper(CB_processbias, cal_procbiasCheckBox);
  paint_checkboxes_helper(CB_processdark, cal_procdarkCheckBox);
  paint_checkboxes_helper(CB_processflat, cal_procflatCheckBox);
  paint_checkboxes_helper(CB_processscience, cal_procscienceCheckBox);
  // superflatting
  paint_checkboxes_helper(CB_spreadsequence, sf_spreadsequenceCheckBox);
  paint_checkboxes_helper(CB_background, sf_backgroundCheckBox);
  paint_checkboxes_helper(CB_mergesequence, sf_mergesequenceCheckBox);
  paint_checkboxes_helper(CB_chopnod, sf_chopnodCheckBox);
  paint_checkboxes_helper(CB_collapse, sf_collapseCheckBox);
  // weighting
  paint_checkboxes_helper(CB_debloom, w_debloomCheckBox);
  paint_checkboxes_helper(CB_globalweights, w_createglobwCheckBox);
  paint_checkboxes_helper(CB_binnedmosaics, w_createbinmosCheckBox);
  paint_checkboxes_helper(CB_createweights, w_createweightsCheckBox);
  paint_checkboxes_helper(CB_distributesets, w_distsetCheckBox);
  // astrom / photom
  paint_checkboxes_helper(CB_absphotom_direct, ap_absphotom_direct_CheckBox);
  paint_checkboxes_helper(CB_directphot, ap_absphotom_direct_CheckBox);
  paint_checkboxes_helper(CB_absphotom_indirect, ap_absphotom_indirect_CheckBox);
  paint_checkboxes_helper(CB_singleastrom, ap_createsourcecatCheckBox);
  paint_checkboxes_helper(CB_astrometry, ap_astrometryCheckBox);
  // coadd
  paint_checkboxes_helper(CB_skysub, cc_createskysubCheckBox);
  paint_checkboxes_helper(CB_createcoadd, cc_coaddCheckBox);
  paint_checkboxes_helper(CB_resolvelinks, cc_resolvelinksCheckBox);
}


void theliForm::destroy()
{
  QSettings settings (QSettings::Ini);
  
  //  obtain_imextension();
  write_logfile();
  
  // the size of the GUI
  int height = mainmenu_TabWidget->height()+75;
  int width  = mainmenu_TabWidget->width()+22;
  
  // update the number of frames
  // update_nframes_instconfig();
  
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) reductionlog = "noname";
  
  settings.beginGroup("/theligeneral_");
  settings.writeEntry("/lastlog", reductionlog);
  settings.writeEntry("/guiwidth", width);
  settings.writeEntry("/guiheight", height);
  settings.endGroup();
}


void theliForm::update_lastlog()
{
  QSettings settings (QSettings::Ini);
  
  // get the log file name
  QString reductionlog = in_reductionlogLineEdit->text();
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
    in_reductionlogLineEdit->setText("noname");
  }
  
  settings.beginGroup("/theligeneral_");
  settings.writeEntry("/lastlog", reductionlog);
  settings.endGroup();
  
  // update_logfile();
}


// get the currently used memory (linux only)
void theliForm::get_memory()
{
  FILE *pipe;
  char sub1[100], sub2[100], sub3[100], sub4[100];
  
  // read /proc/meminfo
  QString execstr = "cat /proc/meminfo | gawk 'BEGIN{mt=0;mf=0;st=0;sf=0} {if ($1 ~ /MemTotal/) mt=$2; if($1 ~ /MemFree/) mf=$2; if ($1 ~ /SwapTotal/) st=$2; if ($1  ~ /SwapFree/) sf=$2} END {print mt, mf, st, sf}'";
  
  pipe = popen(execstr, "r");
  while ( !feof(pipe )) {
    if (fscanf(pipe, "%s %s %s %s", sub1, sub2, sub3, sub4) == 0) {
      printf("\tError: Could not read from stdin!\n");
      strcpy(sub1,"0");
      strcpy(sub2,"0");
      strcpy(sub3,"0");
      strcpy(sub4,"0");
    }
  }
  pclose(pipe);
  
  QString totalphysmem = sub1;
  
  RAM = totalphysmem.toFloat() * 1024.;
}


void theliForm::get_diskspace()
{
  int ratio = 0;
  float fdatadiskfree = .0, fdatadisktotal = .0, fdatadiskwarn = .0;
  float fhomediskwarn = .0, fhomediskfree = .0;
  struct statvfs sStatVfs;
  unsigned long datadiskid = 0, homediskid = 0;
  QString datadiskfree, ho, homediskfree;
  QString execstr, progress;
  
  // get the main data directory.
  // if it does not exist, use the local dir
  QString maindir = in_maindirLineEdit->text();
  if (checkdir2(maindir) != 1) maindir = ".";
  
  QString tempdir = QDir::homeDirPath();
  tempdir.append("/.theli/tmp/");
  
  // the data disk usage
  if (statvfs(maindir, &sStatVfs) < 0 ) {
    //printf("%s\n", strerror(errno));
    // if disk space cannot be determined, we simply do not update the
    // progress bar (it has indicative meaning, only).
    return;
  } else {
    fdatadiskfree = (float) sStatVfs.f_bavail * sStatVfs.f_bsize / 1024 / 1024;
    fdatadisktotal = (float) sStatVfs.f_blocks * sStatVfs.f_bsize / 1024 / 1024;
    datadiskid = sStatVfs.f_fsid;
  }
  ratio = 100 - (int) (fdatadiskfree / fdatadisktotal * 100.);
  progress.setNum(ratio);
  QString datadiskstring = "Disk:   ";
  datadiskstring.append(progress);
  datadiskstring.append("%  (");
  datadiskstring.append(QString("%1").arg((double) fdatadiskfree, 0, 'f', 0));
  datadiskstring.append(" MB left)");
  
  // the homedisk usage
  if (statvfs(tempdir, &sStatVfs) < 0 ) {
    //printf("%s\n", strerror(errno));
    // TODO add error handling
    return;
  } else {
    fhomediskfree = (float) sStatVfs.f_bavail * sStatVfs.f_bsize / 1024 / 1024;;
    homediskid = sStatVfs.f_fsid;
  }
  
  // check if the data disk warning should be activated
  fdatadiskwarn = (in_datadiskwarnSpinBox->cleanText()).toFloat();
  if (fdatadiskfree <= fdatadiskwarn) {
    if (!datadiskspace_warned) {
      datadiskspace_warned = true;
      QMessageBox::warning( this, "THELI: DATA DISK SPACE LOW",
			    "THELI: You are running low on disk space in\n"+
			    maindir+" !");
    }
    diskspaceProgressBar->setPaletteBackgroundColor(red);
    diskspaceProgressBar->setPaletteForegroundColor(red);
  }
  else {
    datadiskspace_warned = false;
    diskspaceProgressBar->setPaletteBackgroundColor(progressbarcolour);
    diskspaceProgressBar->setPaletteForegroundColor(tfwhite);
  }
  
  // check if the tempdir disk warning should be shown
  // do that only if it is not identical to the data disk
  fhomediskwarn = (in_homediskwarnSpinBox->cleanText()).toFloat();
  if (fhomediskfree <= fhomediskwarn) {
    if (!homediskspace_warned && (datadiskid != homediskid)) {
      homediskspace_warned = true;
      QMessageBox::warning( this, "THELI: HOME DISK SPACE LOW",
			    "THELI: You are running low on disk space in your home directory!\n"
			    "THELI writes temporary files to ~/.theli/tmp/\n."
			    "Data in this directory may safely be deleted\n"
			    "when no processing is done.");
    }
  }
  else {
    homediskspace_warned = false;
  }
  
  diskspaceProgressBar->setProgress(ratio);
  diskspaceProgressBar->repaint();
  diskspaceProgressBar->drawText(10, 13, datadiskstring);
  
  // update the disk spaced used up by the tmp dir as well
  flush_tempdir_updatespace();
}


void theliForm::check_instname_consistency()
{
  QString fullinstname;
  QString instname;      // name of the instrument in camera.ini
  QString instfilename;  // name of the instrument as deduced from camera.ini filename

  QString instname_prof = profinstdir;
  QString instname_comm = comminstdir;
  QString instname_user = userinstdir;
  int numinst = in_instrumentListBox->numRows();
  if (numinst > 0 && in_instrumentListBox->currentItem() != -1) {
    instname_prof.append(in_instrumentListBox->currentText());
    instname_comm.append(in_instrumentListBox->currentText());
    instname_user.append(in_instrumentListBox->currentText());
    instname_prof.append(".ini");
    instname_comm.append(".ini");
    instname_user.append(".ini");
    QFile camera_prof(instname_prof);
    QFile camera_comm(instname_comm);
    QFile camera_user(instname_user);
    if (camera_prof.exists()) fullinstname = instname_prof;
    if (camera_comm.exists()) fullinstname = instname_comm;
    if (camera_user.exists()) fullinstname = instname_user;
  }

  QFileInfo fi(fullinstname);
  
  if (!fi.exists()) return;

  get_fileparameter(fullinstname, "INSTRUMENT", instname);
  instfilename = fi.fileName();
  
  int length = instfilename.length();
  instfilename.truncate(length-4);   // subtract ".ini"

  if (instname.compare(instfilename) == 0) {
    return;
  }
  else {
    set_fileparameter(fullinstname, "INSTRUMENT", instfilename);
  }
}


// get the NFRAMES value
void theliForm::get_nframes()
{
  QString sizex, sizey;
  QDir scriptpath;

  check_last_char(scriptdir);
  check_last_char(userinstdir);
  
  QString instname_prof = profinstdir;
  QString instname_comm = comminstdir;
  QString instname_user = userinstdir;

  scriptpath.setPath(scriptdir);

  if (scriptpath.exists()) {
    if (lastinstrument.isEmpty()) {
      in_instrumentListBox->setCurrentItem(0);
      // probably SUPERFLUOUS since setSelected() calls setCurrentItem()
      in_instrumentListBox->setSelected(0,true);
    }
    int numinstruments = in_instrumentListBox->numRows();  
    if (numinstruments > 0) {
      instname_prof.append(in_instrumentListBox->currentText());
      instname_prof.append(".ini");
      instname_comm.append(in_instrumentListBox->currentText());
      instname_comm.append(".ini");
      instname_user.append(in_instrumentListBox->currentText());
      instname_user.append(".ini");
    }
    
    // get the number of CPUs
    int ncpu = in_numcpuSpinBox->value();
    
    // get the image size (assuming that all chips cover the same area)
    QFile camera_prof(instname_prof);
    QFile camera_comm(instname_comm);
    QFile camera_user(instname_user);
    
    if (camera_prof.exists() && numinstruments>0) {
      get_fileparameter2(instname_prof, "SIZEX", sizex);
      get_fileparameter2(instname_prof, "SIZEY", sizey);
    }
    else if (camera_comm.exists() && numinstruments>0) {
      get_fileparameter2(instname_comm, "SIZEX", sizex);
      get_fileparameter2(instname_comm, "SIZEY", sizey);
    }
    else if (camera_user.exists() && numinstruments>0) {
      get_fileparameter2(instname_user, "SIZEX", sizex);
      get_fileparameter2(instname_user, "SIZEY", sizey);
    }
    
    // extract the size of one image
    sizex = sizex.simplifyWhiteSpace();
    sizey = sizey.simplifyWhiteSpace();
    sizex = sizex.remove(')');
    sizey = sizey.remove(')');
    
    get_memory();
    float bytes = sizex.toFloat() * sizey.toFloat() * 4.;
    long nframes = (floor) (0.4 * RAM / bytes / (float) ncpu);

    // obey upper limit
    if (nframes > 1000) nframes = 1000;
    in_nframesSpinBox->setMaxValue(nframes);
    in_nframesSpinBox->setValue(nframes);
    in_nframesSpinBox->update();
  }
  else in_nframesSpinBox->setValue(5);

  // update param_set1.ini
  modify_npara();
}


// show the dialog for the manual sky subtraction configuration
void theliForm::call_skysubmethod()
{
  cc_createskysubCheckBox->setChecked(false);
  cc_commandTextEdit->setText(empty);
  update_lastlog();
  
  manualsky* ms = new manualsky();
  ms->show();
}


void theliForm::init()
{
  QString execstr2;
  QFile paramset1ini;
  int i;
  QDir initdir;
  QSettings settings (QSettings::Ini);
  
  guiversion = "2.10.4";
  
  progressbarcolour = diskspaceProgressBar->paletteBackgroundColor();
  
  // initialise FITS endings (will be overwritten by read_lastlog() 
  //   further below)
  for (i=0; i<6; i++) {
    ext_img[i] = 0;
  }
  
  // get the directory from which the user launched THELI
  my_fscanf("pwd", startdir);
  
  // get the user's home directory
  my_fscanf("echo $HOME", homedir);
  check_last_char(homedir);
  
  // get the operating system
  my_fscanf("uname", opsys);
  
  // get the bash path
  my_fscanf("which bash", bash);
  
  // create a ${HOME}/.qt directory if it doesn't already exist
  QString execstr = homedir;
  execstr.append("/.qt");
  initdir.setPath(execstr);
  if (!initdir.exists()) {
    execstr2 = "mkdir ";
    execstr2.append(execstr);
    my_system(execstr2);
  }
  
  // create a ${HOME}/.theli directory if it doesn't already exist
  execstr = homedir;
  execstr.append("/.theli");
  initdir.setPath(execstr);
  if (!initdir.exists()) {
    execstr2 = "mkdir ";
    execstr2.append(execstr);
    my_system(execstr2);
  }
  
  userthelidir = homedir+"/.theli/";
  
  // create a ${HOME}/.theli/reduction_logs directory 
  // if it doesn't already exist
  execstr = userthelidir;
  execstr.append("reduction_logs");
  initdir.setPath(execstr);
  if (!initdir.exists()) {
    execstr2 = "mkdir ";
    execstr2.append(execstr);
    my_system(execstr2);
  }
  
  // create a ${HOME}/.theli/tmp directory 
  // if it doesn't already exist
  execstr = userthelidir;
  execstr.append("tmp");
  initdir.setPath(execstr);
  if (!initdir.exists()) {
    execstr2 = "mkdir ";
    execstr2.append(execstr);
    my_system(execstr2);
  }
  
  // create a ${HOME}/.theli/scripts directory 
  // if it doesn't already exist
  execstr = userthelidir;
  execstr.append("scripts");
  initdir.setPath(execstr);
  if (!initdir.exists()) {
    execstr2 = "mkdir ";
    execstr2.append(execstr);
    my_system(execstr2);
  }
  // make sure these links exist!
  execstr2 = "ln -s -f "+MAINGUIPATH+"/gui/scripts/progs.ini "+execstr;
  my_system(execstr2);
  execstr2 = "ln -s -f "+MAINGUIPATH+"/gui/scripts/bash.include "+execstr;
  my_system(execstr2);
  execstr2 = "ln -s -f "+MAINGUIPATH+"/gui/scripts/bash_functions.include "+execstr;
  my_system(execstr2);
  
  // create a ${HOME}/.theli/userinstruments directory 
  // if it doesn't already exist
  execstr = userthelidir;
  execstr.append("instruments_user");
  initdir.setPath(execstr);
  if (!initdir.exists()) {
    execstr2 = "mkdir ";
    execstr2.append(execstr);
    my_system(execstr2);
  }
  
  usertmpdir = userthelidir + "/tmp/";
  userscriptdir = userthelidir + "/scripts/";
  
  // create the param_seti.ini files if they don't exist yet
  my_system("touch ~/.theli/param_set1.ini");
  my_system("touch ~/.theli/param_set2.ini");
  my_system("touch ~/.theli/param_set3.ini");
  
  // set the editor and viewer to some default 
  // value before reading the log
  nano->setOn(true);
  skycat->setOn(true);
  pdfacroread->setOn(true);
  
  // the pipeline and GUI paths
  scriptdir = MAINGUIPATH;
  scriptdir.append("gui/scripts/");
  
  userinstdir = userthelidir + "/instruments_user/";
  profinstdir = scriptdir + "/instruments_professional/";
  comminstdir = scriptdir + "/instruments_commercial/";
  
  // write general info
  settings.beginGroup("/theligeneral_");
  settings.writeEntry("/mainpath", scriptdir);
  settings.writeEntry("/homedir", homedir);
  settings.writeEntry("/bindir", bindir);
  settings.writeEntry("/startdir", startdir);
  settings.writeEntry("/opsys", opsys);
  settings.writeEntry("/bash", bash);
  settings.endGroup();
  
  check_for_instrument_duplicates();
  
  // populate the instrument list, but do not
  // yet write something into the log file
  // (the LINEEDITS are empty since they are not yet initialised
  // by reading the logfile)
  
  startup = 0;
  refresh_instlist();
  startup = 1;
  
  // load the log file that was used last, if any
  QString tmpstr = in_reductionlogLineEdit->text();
  read_lastlog();

  refresh_instlist();

  tmpstr = in_reductionlogLineEdit->text();

  theliForm::setCaption("THELI GUI "+guiversion+" --- "+tmpstr);
  
  highlight_instrumentname(lastinstrument);
  
  // get the number of CPUs
  QString paramset1inifile = homedir;
  check_last_char(paramset1inifile);
  if (!paramset1inifile.isEmpty()) {
    paramset1inifile.append(".theli/param_set1.ini");
    paramset1ini.setName(paramset1inifile);
    if (paramset1ini.exists()) get_numcpu(paramset1inifile);
    else in_numcpuSpinBox->setValue(1);
  }
  
  // update the color of the data directory fields
  repaint_defaultpaths();
  // get the number of chips
  get_numchips();
  // update the selected instrument
  update_presinst();
  // update the command list windows according to the check boxes
  update_pre_commandlist();
  update_cal_commandlist();
  update_sf_commandlist();
  update_w_commandlist();
  update_ap_commandlist();
  update_cc_commandlist();
  // update the appearance of astromphotom section
  update_ap_variousfields();
  
  // none of the processes has yet been started.
  // we need this since if no process has yet been started 
  // and the kill button is pressed, a segfault appears otherwise.
  started_proc_del = 0;
  started_proc_pre = 0;
  started_proc_cal = 0;
  started_proc_sf = 0;
  started_proc_w = 0;
  started_proc_ap = 0;
  started_proc_cc = 0;
  started_proc_all = 0;
  
  update_logfile();
  
  // update the statusCPS actions
  changestatus_obtain_imextension();
  
  tf_instrumentLineEdit->setMinimumWidth(300);
  tf_instrumentLineEdit->setMaximumHeight(20);
  statusBar()->addWidget(tf_instrumentLineEdit, 1, true);
  
  get_diskspace();
  
  diskspaceProgressBar->setMinimumWidth(300);
  diskspaceProgressBar->setMaximumHeight(20);
  statusBar()->addWidget(diskspaceProgressBar, 33, true);
  
  QTimer *t2 = new QTimer( this );
  connect( t2, SIGNAL(timeout()), SLOT(get_diskspace()) );
  
  t2->start( 5000, FALSE );
  
  connect( helpWhats_thisAction, SIGNAL(activated()), SLOT(whatsThis()) );
  
  //  paint the redo PushButtons
  active  = scriptdir + "../images/redoarrow.png";
  passive = scriptdir + "../images/redoarrow_inactive.png";
  
  cal_calibredoPushButton->setIconSet(QPixmap(active));
  sf_backgroundredoPushButton->setIconSet(QPixmap(active));
  sf_chopnodredoPushButton->setIconSet(QPixmap(active));
  sf_collapseredoPushButton->setIconSet(QPixmap(active));
  w_debloomredoPushButton->setIconSet(QPixmap(active));
  ap_directphotredoPushButton->setIconSet(QPixmap(active));
  
  // update the NFRAMES parameter for the selected instrument
  get_nframes();
  
  lastchar = "";
  
  // make sure the instrument.ini has a consistent INSTRUMENT keyword
  check_instname_consistency();

  // make sure the 'use dark' checkbox is shown if the 'do not use bias/dark'
  // checkbox is deactivated (sometimes this is not initialised properly)
  toggle_darkinsteadbiascheckbox();

  // enable/disable the webserver for the reference catalog 
  // (ap_arcserverComboBox)
  enable_server();

  // clean up leftovers from last run
  flush_tempdir();

  flush_tempdir_updatespace();

  // what's the latest version online
  getguiversion();

  remove_fits("/home/mischa/hawki/TESTDIR");
}


void theliForm::redo_calib()
{
  QString result = "";
  obtain_imextension();
  if (ext_img[0] == 1) {
    result.append("OFC");
  }
  redo_helper(cal_calibredoPushButton, statusCPSDebiasingAction, 
	      cal_procscienceCheckBox, result, 1);
  
  clearscripts_sf_pushbutton();
  clearscripts_w_pushbutton();
  clearscripts_ap_pushbutton();
  clearscripts_cc_pushbutton();
  
  changestatus_set_imextension();
  update_cal_commandlist();
}


void theliForm::redo_background()
{
  QString result = "";
  obtain_imextension();
  if (ext_img[0] == 1) {
    result.append("OFC");
  }
  result.append("B");
  redo_helper(sf_backgroundredoPushButton, statusCPSBackgroundAction, 
	      sf_backgroundCheckBox, result, 0);

  // the next command is somewhat redundant as it repeats
  // stuff from redo_helper(), but it saves a lot of other stuff.
  clearscripts_sf_pushbutton();
  clearscripts_w_pushbutton();
  clearscripts_ap_pushbutton();
  clearscripts_cc_pushbutton();
  
  changestatus_set_imextension();
  update_sf_commandlist();
}


void theliForm::redo_chopnod()
{
  QString result = "";
  obtain_imextension();
  if (ext_img[0] == 1) {
    result.append("OFC");
    if (ext_img[1] == 1) result.append("B");
  }
  result.append("H");
  redo_helper(sf_chopnodredoPushButton, statusCPSChopnodAction, 
	      sf_chopnodCheckBox, result, 0);
  
  // do we need the following 7 lines?
  CB_chopnod = 0;
  science_coll = 0;
  science_chopnod = 0;
  sky_coll = 0;
  sky_chopnod = 0;
  update_logfile();
  statusCPSCollapseAction->setOn(false);
  
  clearscripts_w_pushbutton();
  clearscripts_ap_pushbutton();
  clearscripts_cc_pushbutton();

  changestatus_set_imextension();
  update_sf_commandlist();
}


void theliForm::redo_collapse()
{
  QString result = "";
  obtain_imextension();
  if (ext_img[0] == 1) {
    result.append("OFC");
    if (ext_img[1] == 1) result.append("B");
    if (ext_img[2] == 1) result.append("H");
  }
  result.append("C");
  redo_helper(sf_collapseredoPushButton, statusCPSCollapseAction, 
	      sf_collapseCheckBox, result, 0);
  
  clearscripts_w_pushbutton();
  clearscripts_ap_pushbutton();
  clearscripts_cc_pushbutton();

  changestatus_set_imextension();
  update_sf_commandlist();
}


/*
void theliForm::redo_2pass()
{
  QString result = "";
  obtain_imextension();
  if (ext_img[0] == 1) {
    result.append("OFC");
    if (ext_img[1] == 1) result.append("B");
    if (ext_img[2] == 1) result.append("H");
    if (ext_img[3] == 1) result.append("C");
  }

  redo_helper(sf_2passredoPushButton, statusCPSCollapseAction, 
	      sf_2passCheckBox, result, 0);
  
  clearscripts_w_pushbutton();
  clearscripts_ap_pushbutton();
  clearscripts_cc_pushbutton();

  changestatus_set_imextension();
  update_sf_commandlist();
}
*/


void theliForm::redo_debloom()
{
  QString result = "";
  obtain_imextension();
  if (ext_img[0] == 1) {
    result.append("OFC");
    if (ext_img[1] == 1) result.append("B");
    if (ext_img[2] == 1) result.append("H");
    if (ext_img[3] == 1) result.append("C");
  }
  result.append("D");
  redo_helper(w_debloomredoPushButton, statusCPSDebloomingAction, 
	      w_debloomCheckBox, result, 0);
  
  clearscripts_ap_pushbutton();
  clearscripts_cc_pushbutton();

  changestatus_set_imextension();
  update_w_commandlist();
}


void theliForm::redo_directphot()
{
  QString result = "";
  obtain_imextension();
  if (ext_img[0] == 1) {
    result.append("OFC");
    if (ext_img[1] == 1) result.append("B");
    if (ext_img[2] == 1) result.append("H");
    if (ext_img[3] == 1) result.append("C");
    if (ext_img[4] == 1) result.append("D");
  }
  
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
  
  if (abs_fittingmethod) result.append("P");
  redo_helper(ap_directphotredoPushButton, statusCPSDirectphotAction, 
	      ap_absphotom_direct_CheckBox, result, 0);
  
  do_directphot = false;
  CB_directphot = 0;
  CB_absphotom_direct = 0;
  science_directphot = 0;
  sky_directphot = 0;
  
  clearscripts_cc_pushbutton();

  changestatus_set_imextension();
  update_ap_commandlist();
}


void theliForm::redo_helper(QPushButton *pushbutton, QAction *action, 
			    QCheckBox *checkbox, QString result, int flag)
{
  QString mainpath = in_maindirLineEdit->text();
  QString science = in_sciencedirLineEdit->text();
  QDir dir_current;

  QString current_status = "";
  if (ext_img[0] == 1) {
    current_status.append("OFC");
    if (ext_img[1] == 1) current_status.append("B");
    if (ext_img[2] == 1) current_status.append("H");
    if (ext_img[3] == 1) current_status.append("C");
    if (ext_img[4] == 1) current_status.append("D");
    if (ext_img[5] == 1) current_status.append("P");
  }

  QString path = mainpath + science;
  QString result_prev = result;
  drop_last_char(result_prev);
  QString path_prev = mainpath+science+"/"+result_prev+"_IMAGES";
  QString path_current = mainpath+science+"/"+current_status+"_IMAGES";
  if (flag == 1) {
    result_prev = "";
    path_prev = mainpath+science+"/SPLIT_IMAGES";
  }
  
  QDir dir = QDir(path);
  QDir dir_prev = QDir(path_prev);
  dir_current.mkdir(path_current);
  QStringList images = dir.entryList("*"+result+".fits");
  QStringList prev_images = dir_prev.entryList("*"+result_prev+".fits");
  int nimages = dir_prev.entryList("*"+result_prev+".fits").count();
  
  if (nimages > 0) {
    // delete the new files
    //    for ( QStringList::Iterator it1 = images.begin(); it1 != images.end(); ++it1 ) {
    //  dir.remove(*it1);
    //}
    // backup the current files
    QString execstr = "mv "+path+"/*"+current_status+".fits "+path_current+"/ > /dev/null 2>&1" ;
    my_system(execstr);
    
    // restore the previous files
    execstr = "mv "+path_prev+"/*"+result_prev+".fits "+path+"/";
    my_system(execstr);
    
    action->setOn(false);
    pushbutton->setIconSet(QPixmap(passive));
    pushbutton->blockSignals(true);
    checkbox->setPaletteBackgroundColor(QColor(216,216,216));
  }
  else {
    if (flag == 1) result_prev = "SPLIT";
    QMessageBox::warning( this, "Redo task",
			  "Could not find images in "+science+"/"+result_prev+"_IMAGES !");
  }
}


void theliForm::update_zeroheader()
{
  QString result;

  obtain_imextension();
  get_fitsext(ext_img, result);

  QString command = "./copy_zeroheader.sh ";
  command += in_maindirLineEdit->text();
  command += " ";
  command += in_sciencedirLineEdit->text();
  command += " ";
  command += result;

  // launch the header update script
  proc_ap_zeroheader_started = 0;
  write_zerohead_superscriptbody(homedir+".theli/scripts/theli_copy_zeroheader.sh", 
			command, "zeroheader");
}


void theliForm::restore_header()
{
  QString result;

  obtain_imextension();
  get_fitsext(ext_img, result);

  QString command = "./restore_header.sh ";
  command += in_maindirLineEdit->text();
  command += " ";
  command += in_sciencedirLineEdit->text();
  command += " ";
  command += result;

  // launch the header update script
  proc_ap_restoreheader_started = 0;
  write_zerohead_superscriptbody(homedir+".theli/scripts/theli_restore_header.sh", 
			command, "restoreheader");
}


void theliForm::write_zerohead_superscriptbody(QString superscript, QString command, QString type)
{
  FILE *of;
  
  // leave if the process is already running!
  
  if ( (type.compare("zeroheader") == 0 && proc_ap_zeroheader_started == 1) ||
       (type.compare("restoreheader") == 0 && proc_ap_restoreheader_started == 1)) 
    return;
  
  QString logfile = superscript+".log";
  
  // write the superscript
  of = fopen(superscript, "w");
  fprintf(of, "#!%s\n\n", bash.ascii());
  fprintf(of, "# ******************************************\n");
  fprintf(of, "# *****  Created on the fly by THELI   *****\n");
  fprintf(of, "# ******************************************\n\n");
  fprintf(of, "cd %s\n\n", scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  fprintf(of, "{\n");
  fprintf(of, "%s\n", command.ascii());
  fprintf(of, "} > %s%s\n", logfile.ascii(), " 2>&1");
  fprintf(of, "\n\n");
  fprintf(of, "echo THELIMESSAGE: finished\n");
  fclose(of);
  
  // make the superscript executable
  QString execstr = "chmod +x ";
  execstr.append(superscript);
  my_system(execstr);
  
  // execute
  
  if (type.compare("zeroheader") == 0) {
    proc_ap_zeroheader = new QProcess( this );
    proc_ap_zeroheader->addArgument( superscript );
    connect(proc_ap_zeroheader, SIGNAL(processExited()), this, SLOT(update_zeroheaderText()));
    if (proc_ap_zeroheader_started == 0) {
      while( !proc_ap_zeroheader->isRunning() ) proc_ap_zeroheader->launch("");
      proc_ap_zeroheader_started = 1;
      ap_updateheaderPushButton->setText("Running ...");
      ap_updateheaderPushButton->setPaletteForegroundColor(QColor(0,155,0));
    }
  }
  
  if (type.compare("restoreheader") == 0) {
    proc_ap_restoreheader = new QProcess( this );
    proc_ap_restoreheader->addArgument( superscript );
    connect(proc_ap_restoreheader, SIGNAL(processExited()), this, SLOT(update_restoreheaderText()));
    if (proc_ap_restoreheader_started == 0) {
      while( !proc_ap_restoreheader->isRunning() ) proc_ap_restoreheader->launch("");
      proc_ap_restoreheader_started = 1;
      ap_restoreheaderPushButton->setText("Running ...");
      ap_restoreheaderPushButton->setPaletteForegroundColor(QColor(0,155,0));
    }
  }
}


void theliForm::update_zeroheaderText()
{
  ap_updateheaderPushButton->setText("Update header");
  ap_updateheaderPushButton->setPaletteForegroundColor(QColor(0,0,0));
}


void theliForm::update_restoreheaderText()
{
  ap_restoreheaderPushButton->setText("Restore header");
  ap_restoreheaderPushButton->setPaletteForegroundColor(QColor(0,0,0));
}


void theliForm::check_filepresence(QTextEdit *textedit, QString datadir, 
				   QString extension, int &flag, int &flag_ini)
{
  // do not do this check if it was run previously
  // we need to exit as some tasks change the status string
  // and if the user selected several tasks at the same time,
  // the repeated calls to this function would trigger the STOP message below
  if (flag_ini == 1) return;

  if (extension.compare("") == 0) extension = "OFC";

  QString maindir = in_maindirLineEdit->text();
  QString path = maindir + "/" + datadir;
  QDir dir = QDir(path);

  QStringList list;

  get_numchips();

  int nchips = 1;
  QString chipstring;
  while (nchips <= numchips.toInt()) {
    chipstring = chipstring.setNum(nchips);
    list = dir.entryList("*_"+chipstring+extension+".fits");
    // if images are found we can leave
    if (!list.isEmpty()) {
      flag_ini = 1;
      return;
    }
    else nchips++;
  }

  // if the code gets to this line, then no images
  // were found and we print a STOP message:
  
  flag = 1;
  warn(textedit,"STOP: No "+extension+" images found in "+path+"!");
  warn(textedit,"Check this directory, and if necessary adjust the");
  warn(textedit,"processing status in the STATUS pulldown menu.");
  flag_ini = 1;
}


void theliForm::call_monitor()
{
  update_lastlog();
  update_logfile();
  pmonitor* monitor = new pmonitor();
  monitor->show();
}


void theliForm::update_mainwindowtitle()
{
  QString tmpstr = in_reductionlogLineEdit->text();
  theliForm::setCaption("THELI GUI "+guiversion+" --- "+tmpstr);
}
