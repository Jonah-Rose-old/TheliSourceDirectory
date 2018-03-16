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
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qsettings.h>
#include <qprocess.h>
#include <qfileinfo.h>
#include "functions.h"

QString statistics_scriptdir, statistics_instrument, statistics_homedir;
int proc_statistics_started;
QProcess *proc_statistics;

void statistics::validate()
{
  QString directory;
  
  // integer fields
  QRegExp ri( "[0-9]+" );
  QValidator* validator_int = new QRegExpValidator( ri, this );
  statistics_xminLineEdit->setValidator( validator_int );
  statistics_xmaxLineEdit->setValidator( validator_int );
  statistics_yminLineEdit->setValidator( validator_int );
  statistics_ymaxLineEdit->setValidator( validator_int );
  
  // directories and similar ones must not accept blank characters
  QRegExp rx( "^\\S+$" );
  QValidator* validator_string = new QRegExpValidator( rx, this );
  statistics_filterLineEdit->setValidator( validator_string );
  statistics_dirLineEdit->setValidator( validator_string );
  
  // check if the image directory exists, and paint it accordingly
  directory = statistics_dirLineEdit->text();
  QDir dir = QDir(directory);
  if (dir.exists() && !directory.isEmpty()) 
    statistics_dirLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
  if (!dir.exists() && !directory.isEmpty()) 
    statistics_dirLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
  if (directory.isEmpty()) 
    statistics_dirLineEdit->setPaletteBackgroundColor(QColor(255,255,255));
  
}


void statistics::clearfields()
{
  statistics_xminLineEdit->clear();
  statistics_xmaxLineEdit->clear();
  statistics_yminLineEdit->clear();
  statistics_ymaxLineEdit->clear();
  //  statistics_filterLineEdit->clear();
  statistics_seeingarcsecRadioButton->setChecked(true);
  statistics_table->setNumRows(0);
  statistics_table->update();
}


void statistics::get_statistics()
{
  FILE *of;
  long numfiles;
  QString line, directory, filter, section;
  QString xmin, xmax, ymin, ymax, execstr;
  QDir dir;
  
  directory = statistics_dirLineEdit->text();
  filter = statistics_filterLineEdit->text();
  xmin = statistics_xminLineEdit->text();
  xmax = statistics_xmaxLineEdit->text();
  ymin = statistics_yminLineEdit->text();
  ymax = statistics_ymaxLineEdit->text();
  
  dir = QDir(directory);
  
  if (filter.isEmpty()) filter = "*.fits";

  if (!dir.exists() || directory.isEmpty()) {
    QMessageBox::warning( this, "No images found",
			  "Directory does not exist or is empty.\n");
    return;
  }
  
  numfiles = get_numfiles_indir(directory, filter);
  
  if (numfiles == 0) {
    QMessageBox::warning( this, "No images found",
			  "No images found, check the directory path\n"
			  "and, if provided, your name filter\n");
    return;
  }
  
  // clear the table
  statistics_table->setNumRows(0);
  statistics_table->erase();
  statistics_table->update();
  
  section = "";
  if (!xmin.isEmpty() && !xmax.isEmpty() && 
      !ymin.isEmpty() && !ymax.isEmpty()) {
    section.append(xmin);
    section.append(" ");
    section.append(xmax);
    section.append(" ");
    section.append(ymin);
    section.append(" ");
    section.append(ymax);
  }
  
  execstr = statistics_scriptdir;
  execstr.append("/statistics.sh ");
  execstr.append(directory);
  execstr.append(" ");
  execstr.append(statistics_instrument);
  execstr.append(" ");
  execstr.append(section);
  execstr.append(" > "+statistics_homedir+".theli/scripts/theli_statistics.sh.log");
  execstr.append(" 2>&1");
  
  of = fopen(statistics_homedir+"/.theli/scripts/theli_statistics.sh", "w");
  fprintf(of, "#!/bin/bash -xv\n\n");
  fprintf(of, "cd %s\n\n", statistics_scriptdir.ascii());
  fprintf(of, "export THELIFILTER=%s\n", filter.ascii());
  fprintf(of, "%s\n\n", execstr.ascii());
  fprintf(of, "echo THELIMESSAGE: finished\n");
  fclose(of);
  
  QString fullpath = statistics_homedir+"/.theli/scripts/theli_statistics.sh";
  
  execstr = "chmod +x "+fullpath;
  my_system(execstr);
  
  // execute as external process:
  proc_statistics = new QProcess( this );
  proc_statistics->addArgument(fullpath);
  connect(proc_statistics, SIGNAL(processExited()), this, SLOT(fill_in_table()));
  if (!proc_statistics->start()) printf("Could not start statistics process!\n");
  
  statistics_getstatisticsPushButton->setText("Running ...");
  statistics_getstatisticsPushButton->setPaletteForegroundColor(QColor(0,155,0));
  
  proc_statistics_started = 1;
}


void statistics::fill_in_table()
{
  int i;
  QString line, teststring,  execstr;
  
  execstr = statistics_dirLineEdit->text();
  execstr.append("/statistics.dat");
  
  // read the output
  QFile file( execstr );
  if ( file.open( IO_ReadOnly )) {
    QTextStream stream( &file );
    i = 0;
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      QStringList lst = lst.split( " ", line);
      if (!line.isEmpty()) {
	statistics_table->setNumRows(i+1);
	statistics_table->setText(i,0,lst[0]);
	statistics_table->setText(i,1,lst[1]);
	statistics_table->setText(i,2,lst[2]);
	statistics_table->setText(i,3,lst[3]);
	statistics_table->setText(i,4,lst[4]);
	statistics_table->setText(i,7,lst[9]);
	statistics_table->setText(i,8,lst[5]);
	statistics_table->setText(i,9,lst[6]);
	statistics_table->setText(i,10,lst[7]);
	statistics_table->setText(i,11,lst[8]);
	
	if ((lst[5]).compare("NA") == 0) *(lst.at(5)) = "";
	if ((lst[6]).compare("NA") == 0) *(lst.at(6)) = "";
	if ((lst[7]).compare("NA") == 0) *(lst.at(7)) = "";
	if ((lst[8]).compare("NA") == 0) *(lst.at(8)) = "";
	if ((lst[9]).compare("NA") == 0) *(lst.at(9)) = "";
	
	if (statistics_seeingarcsecRadioButton->isChecked()) {
	  statistics_table->setText(i,5,lst[6]);
	  statistics_table->setText(i,6,lst[8]);
	}
	else {
	  statistics_table->setText(i,5,lst[5]);
	  statistics_table->setText(i,6,lst[7]);
	}
	i++;
      }
    }
    file.close();
  }
  statistics_getstatisticsPushButton->setPaletteForegroundColor(QColor(0,0,0));
  statistics_getstatisticsPushButton->setText("Get statistics");
  
  // display the result
  
  my_system("display "+statistics_dirLineEdit->text()+"/statistics.png 2> /dev/null &");
}


void statistics::populate_statisticstable()
{
  QHeader *statistics_vertical = statistics_table->verticalHeader();
  QHeader *statistics_horizontal = statistics_table->horizontalHeader();
  statistics_vertical->hide();
  statistics_table->setNumCols(12);
  statistics_table->setNumRows(0);
  statistics_table->setLeftMargin(0);
  statistics_table->setColumnStretchable(0,true);
  statistics_table->setColumnStretchable(1,false);
  statistics_table->setColumnStretchable(2,false);
  statistics_table->setColumnStretchable(3,false);
  statistics_table->setColumnStretchable(4,false);
  statistics_table->setColumnStretchable(5,false);
  statistics_table->setColumnStretchable(6,false);
  statistics_table->setColumnStretchable(7,false);

  statistics_table->setColumnWidth(0,84);
  statistics_table->setColumnWidth(1,84);
  statistics_table->setColumnWidth(2,84);
  statistics_table->setColumnWidth(3,84);
  statistics_table->setColumnWidth(4,84);
  statistics_table->setColumnWidth(5,84);
  statistics_table->setColumnWidth(6,84);
  statistics_table->setColumnWidth(7,84);

  statistics_horizontal->setLabel(0,"Image");
  statistics_horizontal->setLabel(1,"Mean");
  statistics_horizontal->setLabel(2,"Mode");
  statistics_horizontal->setLabel(3,"Median");
  statistics_horizontal->setLabel(4,"rms");
  statistics_horizontal->setLabel(5,"FWHM [\"]");
  statistics_horizontal->setLabel(6,"HFD [\"]");
  statistics_horizontal->setLabel(7,"RZP");
  
  statistics_table->hideColumn(8);
  statistics_table->hideColumn(9);
  statistics_table->hideColumn(10);
  statistics_table->hideColumn(11);
}


void statistics::update_statisticstable()
{
  int i, numrows;
  QString fwhm_i, fwhm_w, hfd_i, hfd_w;

  QHeader *statistics_horizontal = statistics_table->horizontalHeader();

  if (statistics_seeingarcsecRadioButton->isChecked()) {
    statistics_horizontal->setLabel(5,"FWHM [\"]");
    statistics_horizontal->setLabel(6,"HFD [\"]");
  }
  else {
    statistics_horizontal->setLabel(5,"FWHM [pix]");
    statistics_horizontal->setLabel(6,"HFD [pix]");
  }

  numrows = statistics_table->numRows();
  if (numrows == 0) return; 

  if (statistics_seeingarcsecRadioButton->isChecked()) {
    for (i=0; i<numrows; i++) {
      fwhm_w = statistics_table->text(i,9);
      hfd_w  = statistics_table->text(i,11);
      statistics_table->setText(i,5,fwhm_w);
      statistics_table->setText(i,6,hfd_w);
    }
  }
  else {
    for (i=0; i<numrows; i++) {
      fwhm_i = statistics_table->text(i,8);
      hfd_i  = statistics_table->text(i,10);
      statistics_table->setText(i,5,fwhm_i);
      statistics_table->setText(i,6,hfd_i);
    }
  }
}


void statistics::init()
{
  QString lastlog, sciencedir, stat_inst_path;
  QSettings settings (QSettings::Ini);
  
  // get general info
  settings.beginGroup("/theligeneral_");
    lastlog = settings.readEntry("/lastlog", "noname");
    statistics_scriptdir = settings.readEntry("/mainpath", "");
    statistics_homedir = settings.readEntry("/homedir", "");
  settings.endGroup();
  
  // grep the currently selected instrument and science dir
  settings.beginGroup("/theli_"+lastlog+"_");
    settings.beginGroup("/INITIALISE");
      sciencedir = settings.readEntry("/maindir");
      sciencedir.append(settings.readEntry("/sciencedir"));      
      statistics_instrument = settings.readEntry ("/instrumentname");
      if (settings.readBoolEntry("/inst_prof",true))
	stat_inst_path = statistics_scriptdir + "/instruments_professional/";
      if (settings.readBoolEntry("/inst_comm",true))
	stat_inst_path = statistics_scriptdir + "/instruments_commercial/";
      if (settings.readBoolEntry("/inst_user",true))
	stat_inst_path = statistics_homedir + "/.theli/instruments_user/";

    settings.endGroup();
  settings.endGroup();

  statistics_dirLineEdit->setText(sciencedir);
  // check if the image directory exists, and paint it accordingly
  QDir dir = QDir(sciencedir);
  
  if (dir.exists() && !sciencedir.isEmpty()) 
    statistics_dirLineEdit->setPaletteBackgroundColor(QColor(155,255,187));
  if (!dir.exists() && !sciencedir.isEmpty()) 
    statistics_dirLineEdit->setPaletteBackgroundColor(QColor(255,187,155));
  if (sciencedir.isEmpty()) 
    statistics_dirLineEdit->setPaletteBackgroundColor(QColor(255,255,255));
  
  statistics_seeingarcsecRadioButton->setChecked(true);
  
  proc_statistics_started = 0;
  
  populate_statisticstable();
  
  statistics_filterLineEdit->setFocus();
}


void statistics::select_dirimage()
{
  QFileDialog* fd = new QFileDialog( this, "Select directory or image", TRUE );
  fd->setMode( QFileDialog::ExistingFile );
  fd->setDir(statistics_dirLineEdit->text());
  fd->show();

  if ( fd->exec() == QDialog::Accepted ) {
    QString selected = fd->selectedFile();
    QFileInfo fi = QFileInfo(selected);
    if (fi.isFile()) {
      statistics_dirLineEdit->setText(fi.dirPath());
      statistics_filterLineEdit->setText(fi.fileName());
    }
  }
}


void statistics::kill_stats()
{
  QString execstr, pid;
  long process_id;
  
  if (proc_statistics_started == 1 && proc_statistics->isRunning()) {
    process_id = proc_statistics->processIdentifier();
    pid = pid.setNum(process_id);
    execstr = scriptdir;
    execstr.append("/parakill.sh ");
    execstr.append(pid);
    execstr.append(" > /dev/null 2>&1");
    my_system(execstr);
    statistics_getstatisticsPushButton->setPaletteForegroundColor(QColor(0,0,0));
    statistics_getstatisticsPushButton->setText("Get statistics");
  }
}


void statistics::quit()
{
  // only close the dialog if no processes are running
  if (proc_statistics_started == 0) 
    statistics::close();
  else {
    int answer = QMessageBox::warning(this, "Process still running",
				  QString( "A process is still running!\n If you Quit, it will be aborted.\n\n" ),
				  "&Cancel", "&Quit", QString::null, 0, 1 );

    if (answer == 0) return;
    else {
      kill_stats();
      statistics::close();
    }
  }
}
