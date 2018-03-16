#include <qdir.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qstring.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qapplication.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <qstringlist.h>
#include <qfiledialog.h>
#include <qradiobutton.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qsettings.h>
#include <qprocess.h>
#include <qfileinfo.h>
#include "functions.h"

QString ia_scriptdir, ia_instrument, ia_homedir;
int proc_ia_analyse_started, proc_ia_makeplot_started, makeplot_aborted;
QProcess *proc_ia_analyse, *proc_ia_makeplot;

void imalyzer::validate()
{
  QString directory;

  // integer fields
  QRegExp ri( "[0-9]+" );
  QRegExp rf( "^[0-9]*[.]{0,1}[0-9]+" );

  // directories and similar ones must not accept blank characters
  QRegExp rx( "^\\S+$" );
  QValidator* validator_string = new QRegExpValidator( rx, this );
  QValidator* validator_float = new QRegExpValidator(rf, this );
  QValidator* validator_int = new QRegExpValidator(ri, this );
  ia_filterLineEdit->setValidator( validator_string );
  ia_dirLineEdit->setValidator( validator_string );
  ia_titleLineEdit->setValidator( validator_string );
  ia_scaleminLineEdit->setValidator( validator_float );
  ia_scalemaxLineEdit->setValidator( validator_float );
  ia_scalecolorrangeLineEdit->setValidator( validator_int );

  // check if the image directory exists, and paint it accordingly
  directory = ia_dirLineEdit->text();
  QDir dir = QDir(directory);
  if (dir.exists() && !directory.isEmpty()) 
    ia_dirLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
  if (!dir.exists() && !directory.isEmpty()) 
    ia_dirLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
  if (directory.isEmpty()) 
    ia_dirLineEdit->setPaletteBackgroundColor(QColor(255,255,255));
}


void imalyzer::init()
{
  QString lastlog, sciencedir, stat_inst_path;
  QSettings settings (QSettings::Ini);

  // get general info
  settings.beginGroup("/theligeneral_");
  lastlog = settings.readEntry("/lastlog", "noname");
  ia_scriptdir = settings.readEntry("/mainpath", "");
  ia_homedir = settings.readEntry("/homedir", "");
  settings.endGroup();

  // grep the currently selected instrument and science dir
  settings.beginGroup("/theli_"+lastlog+"_");
  settings.beginGroup("/INITIALISE");
  sciencedir = settings.readEntry("/maindir");
  sciencedir.append(settings.readEntry("/sciencedir"));      
  ia_instrument = settings.readEntry ("/instrumentname");
  if (settings.readBoolEntry("/inst_prof",true))
    stat_inst_path = ia_scriptdir + "/instruments_professional/";
  if (settings.readBoolEntry("/inst_comm",true))
    stat_inst_path = ia_scriptdir + "/instruments_commercial/";
  if (settings.readBoolEntry("/inst_user",true))
    stat_inst_path = ia_homedir + "/.theli/instruments_user/";
  settings.endGroup();
  settings.endGroup();

  ia_dirLineEdit->setText(sciencedir);
  // check if the image directory exists, and paint it accordingly
  QDir dir = QDir(sciencedir);
  
  if (dir.exists() && !sciencedir.isEmpty()) 
    ia_dirLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
  if (!dir.exists() && !sciencedir.isEmpty()) 
    ia_dirLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
  if (sciencedir.isEmpty()) 
    ia_dirLineEdit->setPaletteBackgroundColor(QColor(255,255,255));

  proc_ia_analyse_started = 0;
  proc_ia_makeplot_started = 0;

  ia_filterLineEdit->setFocus();

  ia_showhide();

  ia_scalefullrangeRadioButton->setChecked(true);

  makeplot_aborted = 0;

  radioselect();

  ia_fwhmsplinesmoothCheckBox->setChecked(true);
  ia_interpolateanisoCheckBox->setChecked(true);
}


void imalyzer::select_dirimage()
{
  QFileDialog* fd = new QFileDialog( this, "Select directory or image", TRUE );
  fd->setMode( QFileDialog::ExistingFile );
  fd->setDir(ia_dirLineEdit->text());
  fd->show();

  if ( fd->exec() == QDialog::Accepted ) {
    QString selected = fd->selectedFile();
    QFileInfo fi = QFileInfo(selected);
    if (fi.isFile()) {
      ia_dirLineEdit->setText(fi.dirPath());
      ia_filterLineEdit->setText(fi.fileName());
    }
  }
}


void imalyzer::analyse_images()
{
  FILE *of;
  long numfiles;
  QString line, directory, filter, section;
  QString execstr;
  QDir dir, catdir;
  
  directory = ia_dirLineEdit->text();
  filter = ia_filterLineEdit->text();
  
  dir = QDir(directory);
  catdir = QDir(directory+"/cat");
  
  if (!dir.exists() || directory.isEmpty()) {
    QMessageBox::warning( this, "No images found",
			  "Directory does not exist or is empty.\n");
    return;
  }
  
  QStringList catlist = catdir.entryList("*.cat");
  if (catlist.isEmpty() || !catdir.exists()) {
    QMessageBox::warning( this, "No catalogs found",
			  "No object catalogs were found. You must run the\n"
			  "'create source cat' step first.\n");
    return;
  }
  
  if (filter.isEmpty()) filter = "*.fits";
  numfiles = dir.entryList(filter).count();
  
  if (numfiles == 0) {
    QMessageBox::warning( this, "No images found",
			  "No images found, check the directory path\n"
			  "and, if provided, your name filter.\n");
    return;
  }
  
  execstr.append("./parallel_manager.sh ");
  execstr.append("imalyzer_analysis_para.sh ");
  execstr.append(directory);
  execstr.append(" ");
  execstr.append(ia_instrument);
  execstr.append(" &> "+ia_homedir+".theli/scripts/theli_imalyzer_analysis.sh.log");
  
  of = fopen(ia_homedir+"/.theli/scripts/theli_imalyzer_analysis.sh", "w");
  fprintf(of, "#!/bin/bash -xv\n\n");
  fprintf(of, "export INSTRUMENT=%s\n", ia_instrument.ascii());
  fprintf(of, "cd %s\n\n", ia_scriptdir.ascii());
  fprintf(of, "export THELIFILTER=%s\n", filter.ascii());
  fprintf(of, "%s\n\n", execstr.ascii());
  fprintf(of, "echo THELIMESSAGE: finished\n");
  fclose(of);
  
  QString fullpath = ia_homedir+"/.theli/scripts/theli_imalyzer_analysis.sh";
  
  execstr = "chmod +x "+fullpath;
  my_system(execstr);
  
  // execute as external process:
  proc_ia_analyse = new QProcess( this );
  proc_ia_analyse->addArgument(fullpath);
  connect(proc_ia_analyse, SIGNAL(processExited()), this, SLOT(ia_analyse_done()));
  if (!proc_ia_analyse->start()) printf("Could not start image analysis process!\n");
  proc_ia_analyse_started = 1;
  
  ia_analysePushButton->setText("Running ...");
  ia_analysePushButton->setPaletteForegroundColor(QColor(0,155,0));
}


void imalyzer::makeplot_images()
{
  FILE *of;
  long numfiles;
  QString line, directory, filter, filterstarcat, section, units;
  QString execstr, title, colorrange, smooth, contour, grid, psf;
  QString scalemin, scalemax, min, max, scaling;
  QDir dir, ksbdir;
  
  colorrange = "0";
  directory  = ia_dirLineEdit->text();
  filter     = ia_filterLineEdit->text();
  title      = ia_titleLineEdit->text();
  units      = ia_unitsComboBox->currentText();
  
  if ( ia_scalefullrangeRadioButton->isOn()) {
    scalemin = "undef";
    scalemax = "undef";
    scaling = "fullrange";
  }
  else if (ia_scalemaxRadioButton->isOn()) {
    scalemin = "undef";
    scalemax = "undef";
    scaling = "max";
    colorrange = ia_scalecolorrangeLineEdit->text();
  }
  else {
    scalemin = ia_scaleminLineEdit->text();
    scalemax = ia_scalemaxLineEdit->text();
    scaling = "absolute";
    if (scalemin.isEmpty() || scalemax.isEmpty()) {
      QMessageBox::warning( this, "Missing parameters",
			    "You must provide both a lower and an upper limit!\n");
      return;
    }
  }
  
  dir = QDir(directory);
  
  if (!dir.exists() || directory.isEmpty()) {
    QMessageBox::warning( this, "No images found",
			  "Directory does not exist or is empty.\n");
    return;
  }
  
  if (filter.isEmpty()) filter = "*.fits";
  numfiles = dir.entryList(filter).count();
  
  if (numfiles == 0) {
    QMessageBox::warning( this, "No images found",
			  "No images found, check the directory path\n"
			  "and, if provided, your name filter\n");
    return;
  }
  
  // check if catalogs are present
  filterstarcat = filter;
  filterstarcat.replace(".fits","_stars.cat");
  ksbdir = QDir(directory+"/imalyzer/cat/");
  numfiles = ksbdir.entryList(filterstarcat).count();
  if (!ksbdir.exists() || numfiles == 0) {
    QMessageBox::warning( this, "Missing data",
			  "You must run the \"Analyse PSF\" task first!\n");
    return;
  }
  
  if (title.isEmpty()) {
    title="imagequality";
    ia_titleLineEdit->setText("imagequality");
  }
  
  if (ia_fwhmsplinesmoothCheckBox->isChecked()) smooth = "smoothed";
  else smooth = "unsmoothed";
  
  if (ia_interpolateanisoCheckBox->isChecked()) psf = "polynomial";
  else psf = "data";
  
  if (ia_mediancontourlineCheckBox->isChecked()) contour = "yes";
  else contour = "no";
  
  if (ia_gridCheckBox->isChecked()) grid = "grid";
  else grid = "nogrid";
  
  execstr.append("./parallel_manager.sh ");
  execstr.append("imalyzer_plot_para.sh ");
  execstr.append(directory);
  execstr.append(" ");
  execstr.append(ia_instrument);
  execstr.append(" ");
  execstr.append(title);
  execstr.append(" ");
  execstr.append(colorrange);
  execstr.append(" ");
  execstr.append(units);
  execstr.append(" ");
  execstr.append(scalemin);
  execstr.append(" ");
  execstr.append(scalemax);
  execstr.append(" ");
  execstr.append(scaling);
  execstr.append(" ");
  execstr.append(smooth);
  execstr.append(" ");
  execstr.append(contour);
  execstr.append(" ");
  execstr.append(grid);
  execstr.append(" ");
  execstr.append(psf);
  execstr.append(" &> "+ia_homedir+".theli/scripts/theli_imalyzer_plot.sh.log");

  of = fopen(ia_homedir+"/.theli/scripts/theli_imalyzer_plot.sh", "w");
  fprintf(of, "#!/bin/bash -xv\n\n");
  fprintf(of, "export INSTRUMENT=%s\n", ia_instrument.ascii());
  fprintf(of, "cd %s\n\n", ia_scriptdir.ascii());
  fprintf(of, "export THELIFILTER=%s\n", filter.ascii());
  fprintf(of, "%s\n\n", execstr.ascii());
  
  execstr = "./make_slideshow.sh " + directory + "/imalyzer " + title;

  fprintf(of, "%s\n\n", execstr.ascii());

  fprintf(of, "echo THELIMESSAGE: finished\n");
  fclose(of);

  QString fullpath = ia_homedir+"/.theli/scripts/theli_imalyzer_plot.sh";

  execstr = "chmod +x "+fullpath;
  my_system(execstr);

  // execute as external process:
  proc_ia_makeplot = new QProcess( this );
  proc_ia_makeplot->addArgument(fullpath);
  connect(proc_ia_makeplot, SIGNAL(processExited()), this, SLOT(ia_makeplot_done()));
  if (!proc_ia_makeplot->start()) printf("Could not start makeplot process!\n");

  proc_ia_makeplot_started = 1;
  ia_makeplotsPushButton->setText("Running ...");
  ia_makeplotsPushButton->setPaletteForegroundColor(QColor(0,155,0));
}


void imalyzer::ia_analyse_done()
{
  ia_analysePushButton->setText("Analyse PSF (once per name filter)");
  ia_analysePushButton->setPaletteForegroundColor(QColor(0,0,255));
  proc_ia_analyse_started = 0;
}


void imalyzer::ia_makeplot_done()
{
  ia_makeplotsPushButton->setText("Make plots");
  ia_makeplotsPushButton->setPaletteForegroundColor(QColor(0,0,255));

  // show the result in firefox
  if (makeplot_aborted == 0)
    my_system("firefox "+ia_dirLineEdit->text()+
	      "/imalyzer/"+ia_titleLineEdit->text()+".html 2> /dev/null &");

  proc_ia_makeplot_started = 0;
  makeplot_aborted = 0;
}


void imalyzer::ia_showhide()
{
  if (ia_unitsComboBox->currentItem() == 2) {
    if (ia_scaleabsoluteRadioButton->isOn()) ia_scalefullrangeRadioButton->setChecked(true);
    ia_scaleminLineEdit->setEnabled(false);
    ia_scalemaxLineEdit->setEnabled(false);
    ia_scaleminTextLabel->setEnabled(false);
    ia_scalemaxTextLabel->setEnabled(false);
    ia_scaleabsoluteRadioButton->setEnabled(false);
  }
  else {
    ia_scaleminLineEdit->setEnabled(true);
    ia_scalemaxLineEdit->setEnabled(true);
    ia_scaleminTextLabel->setEnabled(true);
    ia_scalemaxTextLabel->setEnabled(true);
    ia_scaleabsoluteRadioButton->setEnabled(true);
  }

  radioselect();

}


void imalyzer::radioselect()
{
  if (ia_scalefullrangeRadioButton->isOn()) {
    ia_scaleminLineEdit->setEnabled(false);
    ia_scalemaxLineEdit->setEnabled(false);
    ia_scalecolorrangeLineEdit->setEnabled(false);
  }
  if (ia_scalemaxRadioButton->isOn()) {
    ia_scaleminLineEdit->setEnabled(false);
    ia_scalemaxLineEdit->setEnabled(false);
    ia_scalecolorrangeLineEdit->setEnabled(true);
  }
  if (ia_scaleabsoluteRadioButton->isOn()) {
    ia_scaleminLineEdit->setEnabled(true);
    ia_scalemaxLineEdit->setEnabled(true);
    ia_scalecolorrangeLineEdit->setEnabled(false);
  }
}


void imalyzer::kill_analyse()
{
  QString execstr, pid;
  long process_id;
  
  if (proc_ia_analyse_started == 1 && proc_ia_analyse->isRunning()) {
    process_id = proc_ia_analyse->processIdentifier();
    pid = pid.setNum(process_id);
    execstr = scriptdir;
    execstr.append("/parakill.sh ");
    execstr.append(pid);
    execstr.append(" > /dev/null 2>&1");
    my_system(execstr);
    ia_analyse_done();
  }
}


void imalyzer::kill_makeplot()
{
  QString execstr, pid;
  long process_id;
  
  if (proc_ia_makeplot_started == 1 && proc_ia_makeplot->isRunning()) {
    process_id = proc_ia_makeplot->processIdentifier();
    pid = pid.setNum(process_id);
    execstr = scriptdir;
    execstr.append("/parakill.sh ");
    execstr.append(pid);
    execstr.append(" > /dev/null 2>&1");
    my_system(execstr);
    ia_makeplotsPushButton->setText("Make plots");
    ia_makeplotsPushButton->setPaletteForegroundColor(QColor(0,0,255));
    proc_ia_makeplot_started = 0;
    makeplot_aborted = 1;
  }
}


void imalyzer::quit()
{
  // only close the dialog if no processes are running
  if (proc_ia_analyse_started == 0 && proc_ia_makeplot_started == 0) 
    imalyzer::close();
  else {
    int answer = QMessageBox::warning(this, "Process still running",
				  QString( "A process is still running!\n If you Quit, it will be aborted.\n\n" ),
				  "&Cancel", "&Quit", QString::null, 0, 1 );

    if (answer == 0) return;
    else {
      if (proc_ia_analyse_started == 1) kill_analyse();
      if (proc_ia_makeplot_started == 1) kill_makeplot();
      imalyzer::close();
    }
  }
}
