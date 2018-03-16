#include <qdir.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qstring.h>
#include <qprocess.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include "functions.h"

QString zp_pid, instrument, sciencepath, editor;
int started_proc_zp, zp_errcount, zp_errline;
QProcess *proc_zp;

void zp::validate()
{
    QString filename;

    // magnitude error
    QRegExp rf( "[0-9]*[.]{0,1}[0-9]+" );
    QValidator* validator_float = new QRegExpValidator( rf, this );
    zp_aperturesizeLineEdit->setValidator( validator_float );
    zp_maxphoterrorLineEdit->setValidator( validator_float );
    zp_saturationLineEdit->setValidator( validator_float );

    // directory must not accept blank characters
    QRegExp rx( "^\\S+$" );
    QValidator* validator_string = new QRegExpValidator( rx, this );
    zp_imageLineEdit->setValidator( validator_string );

    // check if the image entered exists and paint it accordingly
    filename = zp_imageLineEdit->text();
    QFile file(filename);
    
    if (file.exists() && !filename.isEmpty()) 
	zp_imageLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
    if (!file.exists() && !filename.isEmpty()) 
	zp_imageLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
    if (filename.isEmpty()) 
	zp_imageLineEdit->setPaletteBackgroundColor(QColor(255,255,255));
}


void zp::get_zp()
{
    FILE *of;
    QString filter, execstr, refcat, image, maxerr, server, saturation, magtype, aperture;

    zp_resultLineEdit->clear();
    zp_resulterrLineEdit->clear();
    zp_resultrmsLineEdit->clear();
    zp_resultnumLineEdit->clear();

    image  = zp_imageLineEdit->text();
    filter = zp_filterComboBox->currentText();
    refcat = zp_refcatComboBox->currentText();
    maxerr = zp_maxphoterrorLineEdit->text();
    saturation = zp_saturationLineEdit->text();
    magtype = zp_magtypeComboBox->currentText();
    aperture = zp_aperturesizeLineEdit->text();

    switch(zp_serverComboBox->currentItem()) {
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

    // 2MASS vizier mag key is called "K"
    if (filter.compare("Ks") == 0) filter = "K";

    QFile file(image);
    
    if (!file.exists()) {
      QMessageBox::warning( this, "Image not found.",
	   "The image you selected does not exist!\n");
      return;
    }

    if (maxerr.isEmpty()) {
      maxerr = "0.05";
      zp_maxphoterrorLineEdit->setText("0.05");
    }

    if (saturation.isEmpty()) {
      saturation = "10000";
      zp_saturationLineEdit->setText("10000");
    }

    if (aperture.isEmpty()) {
      aperture = "10";
      zp_aperturesizeLineEdit->setText("10");
    }

    execstr = scriptdir;
    execstr.append("/get_coadd_zp.sh ");
    execstr.append(image);
    execstr.append(" ");
    execstr.append(refcat);
    execstr.append(" ");
    execstr.append(filter);
    execstr.append(" ");
    execstr.append(maxerr);
    execstr.append(" ");
    execstr.append(saturation);
    execstr.append(" ");
    execstr.append(magtype);
    execstr.append(" ");
    execstr.append(aperture);
    execstr.append(" ");
    execstr.append(server);
    execstr.append(" &> "+homedir+".theli/scripts/theli_get_coadd_zp.log");

    of = fopen(homedir+"/.theli/scripts/theli_getcoaddzp.sh", "w");
    fprintf(of, "#!/bin/bash -xv\n\n");
    fprintf(of, "cd %s\n\n", scriptdir.ascii());
    fprintf(of, "%s\n\n", execstr.ascii());
    fprintf(of, "echo THELIMESSAGE: finished\n");
    fclose(of);

    QString fullpath = homedir+"/.theli/scripts/theli_getcoaddzp.sh";

    execstr = "chmod +x "+fullpath;
    my_system(execstr);

    zp_resultLineEdit->clear();
    zp_resulterrLineEdit->clear();
    zp_resultrmsLineEdit->clear();
    zp_resultnumLineEdit->clear();

    // execute as external process:
    proc_zp = new QProcess( this );
    proc_zp->addArgument(fullpath);
    connect(proc_zp, SIGNAL(processExited()), this, SLOT(fill_in_zp()));
    if (!proc_zp->start()) 
      printf("Could not start zeropoint process!\n");
    else {
      zp_startPushButton->setText("Running ...");
      zp_startPushButton->setPaletteForegroundColor(QColor(0,155,0));
      started_proc_zp = 1;
    }
}


void zp::show_hide()
{
  if (zp_magtypeComboBox->currentText() == "MAG_AUTO") 
    zp_aperturesizeLineEdit->setEnabled(false);
  else
    zp_aperturesizeLineEdit->setEnabled(true);
}


void zp::select_image()
{
  QString path = sciencepath;
  QString image = zp_imageLineEdit->text();

  QFileInfo fileinfo;
  fileinfo = QFileInfo(image);
  
  if (!image.isEmpty()) path = fileinfo.dirPath();

  QString currentFileName = QFileDialog::getOpenFileName(
	    path,
	    "Images (*.fits *.fit *.fts *.FIT *.FITS *.FTS);; All (*.*)",
	    this,
	    "open file dialog",
	    "Select the image you want to calibrate");

    if ( currentFileName.isEmpty() )
	return;
  
    zp_imageLineEdit->setText(currentFileName);
}


void zp::update_filter_comboboxes()
{
  QStringList filters;

  if (zp_refcatComboBox->currentText() == "SDSS") {
    filters += "u";
    filters += "g";
    filters += "r";
    filters += "i";
    filters += "z";
  }
  if (zp_refcatComboBox->currentText() == "2MASS") {
    filters += "J";
    filters += "H";
    filters += "Ks";
  }

  // populate the comboboxes:
  zp_filterComboBox->clear();
  zp_filterComboBox->insertStringList(filters);
}


void zp::fill_in_zp()
{
  QString zp, execstr, image;

  zp_startPushButton->setText("Get ZP");
  zp_startPushButton->setPaletteForegroundColor(QColor(0,0,0));

  if (scan_for_errors() == 1 || started_proc_zp == 0)
    return;

  image = zp_imageLineEdit->text();

  execstr = bindir+"/getkey " + image + " ZP_D";
  my_fscanf(execstr, zp);
  zp_resultLineEdit->setText(zp);

  execstr = bindir+"/getkey " + image + " ZP_D_ERR";
  my_fscanf(execstr, zp);
  zp_resulterrLineEdit->setText(zp);

  execstr = bindir+"/getkey " + image + " ZP_D_RMS";
  my_fscanf(execstr, zp);
  zp_resultrmsLineEdit->setText(zp);

  execstr = bindir+"/getkey " + image + " ZP_D_NUM";
  my_fscanf(execstr, zp);
  zp_resultnumLineEdit->setText(zp);

  started_proc_zp = 0;
}


void zp::kill_process()
{
  QString execstr;
  long process_id;
  
  if (started_proc_zp == 1 && proc_zp->isRunning()) {
    started_proc_zp = 0;
    process_id = proc_zp->processIdentifier();
    zp_pid = zp_pid.setNum(process_id);
    execstr = scriptdir;
    execstr.append("/parakill.sh ");
    execstr.append(zp_pid);
    execstr.append(" > /dev/null 2>&1");
    my_system(execstr);
    zp_startPushButton->setText("Get ZP");
    zp_startPushButton->setPaletteForegroundColor(QColor(0,0,0));
  }
}


void zp::init()
{
  QString lastlog;
  QSettings settings (QSettings::Ini);
  
  // get general info
  settings.beginGroup("/theligeneral_");
    lastlog = settings.readEntry("/lastlog", "noname");
    homedir = settings.readEntry("/homedir", "");
    startdir = settings.readEntry("/startdir", "");
    bash = settings.readEntry("/bash", "");
  settings.endGroup();

  // grep the currently selected instrument and sciencedir
  settings.beginGroup("/theli_"+lastlog+"_");
    settings.beginGroup("/INITIALISE");
      sciencepath = settings.readEntry("/maindir");
      sciencepath.append(settings.readEntry("/sciencedir"));
      editor = settings.readEntry("/editor");
      if (strcmp(editor,"vi") == 0)
	editor = "xterm -e vi ";
    settings.endGroup();
  settings.endGroup();

  started_proc_zp = 0;
  
  update_filter_comboboxes();
  
  zp_maxphoterrorLineEdit->setText("0.05");
  zp_saturationLineEdit->setText("100");
  zp_aperturesizeLineEdit->setText("10");
}


int zp::scan_for_errors()
{
  int falseerrcount;
  QString zp_fullpathlog, execstr;
  QString line, errormessage, errorcode, falseerrorcode;
  QStringList errorlist, falseerrorlist;
  
  zp_fullpathlog = homedir + "/.theli/scripts/theli_get_coadd_zp.log";
  
  // scan for these error messages
  errorlist << "error";
  errorlist << "ERROR";
  errorlist << "No photometric reference sources retrieved!";

  // ignore these error messages
  falseerrorlist << "max phot error";
  falseerrorlist << "theli_error";
  falseerrorlist << "V_ABSPHOT_MAXPHOTERR";
  
  // error handling
  QFile file(zp_fullpathlog);
  
  if ( file.open( IO_ReadOnly ) && file.exists()) {
    errormessage = "";
    zp_errcount = 0;
    zp_errline = 0;
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      line.stripWhiteSpace();
      zp_errline++;
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
	  zp_errcount = line.contains(errorcode, false);

	  if (line.contains("No photometric reference sources retrieved!", false)) {
	          QMessageBox::information( this, "No catalog overlap",
		     "No photometric reference sources found for your field!\n"
		      "Try a different catalog or download server,\n"
		      "and check your internet connectivity!\n");
		  file.close();
		  return (1);
	  }

	  if (zp_errcount > 0) {
	    errormessage = "THELI_ERROR";
	    errormessage.append(line);
	    break;
	  }
	  else zp_errcount = 0;
	}
	if (zp_errcount > 0) break;
      }
    }
    file.close();
    
    if (proc_zp->normalExit()) {
      if (errormessage.left(11).compare("THELI_ERROR") == 0 && 
	  errormessage.length() > 11 && zp_errcount != 0) {
	
	// display the log file
	execstr = editor + " ";
	execstr.append(zp_fullpathlog);
	execstr.append(" &");
	my_system(execstr);	    
	return (1);
      }
    }
  }
  return (0);
}


void zp::quit()
{
  // only close the dialog if no processes are running
  if (started_proc_zp == 0) 
    zp::close();
  else {
    int answer = QMessageBox::warning(this, "Process still running",
				  QString( "A process is still running!\n If you Quit, it will be aborted.\n\n" ),
				  "&Cancel", "&Quit", QString::null, 0, 1 );

    if (answer == 0) return;
    else {
      kill_process();
      zp::close();
    }
  }
}
