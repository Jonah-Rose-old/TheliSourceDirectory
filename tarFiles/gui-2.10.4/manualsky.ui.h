#include <qdir.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qstring.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qcheckbox.h>
#include <qsettings.h>
#include <qwidgetstack.h>
#include "functions.h"

QString manualsky_scriptdir, manualsky_instrument, manualsky_homedir, manual;
QString manualsky_pid, manualsky_numchips, manualsky_bindir, sciencedir;
int started_proc_manualsky, manualsky_errcount, mscalldialog, bgdefined;
QString manualsky_concatenate_logs, manualsky_remove_individuallogs;
QString manualsky_fullpathlog, manualsky_lastlog, statusscience, statusstring;
QHeader *manualsky_horizontal;
long allnumfiles;
QString manualsky_inst_path;
int manualsky_inst_prof_index, manualsky_inst_comm_index, manualsky_inst_user_index;
int manualsky_instprofRB=0, manualsky_instcommRB=0, manualsky_instuserRB=0;

struct statistics
{
  char name[200];
  float mode;
  float lquart;
  float median;
  float uquart;
  float mean;
  float rms;
};

struct statistics *bg;


void manualsky::validate()
{
  QString directory;
  
  // integer fields
  QRegExp ri( "[0-9]+" );
  QValidator* validator_int = new QRegExpValidator( ri, this );
  ms_skysubbacksizeLineEdit->setValidator( validator_int );
  ms_skysubdetectminareaLineEdit->setValidator( validator_int );
  ms_xminLineEdit->setValidator( validator_int );
  ms_xmaxLineEdit->setValidator( validator_int );
  ms_yminLineEdit->setValidator( validator_int );
  ms_ymaxLineEdit->setValidator( validator_int );
  
  // float fields
  QRegExp rfloat( "^[-]{0,1}[0-9]*[.]{0,1}[0-9]+" );
  QRegExp rf2( "[0-9]*[.]{0,1}[0-9]+" );
  QValidator* validator_float = new QRegExpValidator( rfloat, this );
  QValidator* validator_float2 = new QRegExpValidator(rf2, this );
  ms_fillLineEdit->setValidator( validator_float );
  ms_skysubdetectthreshLineEdit->setValidator( validator_float2 );
  ms_maskexpandfactorLineEdit->setValidator( validator_float2 );

  // ra dec
  QRegExp radec( "^[-]{0,1}[0-9.:]+" );
  QValidator* validator_radec = new QRegExpValidator( radec, this );
  ms_ra1LineEdit->setValidator( validator_radec );
  ms_ra2LineEdit->setValidator( validator_radec );
  ms_dec1LineEdit->setValidator( validator_radec );
  ms_dec2LineEdit->setValidator( validator_radec );
}


void manualsky::clearfields()
{
  int i;
  // clear the table
  for (i=0; i<manualsky_table->numRows(); i++) {
    manualsky_table->setText(i,0,"");
    manualsky_table->setText(i,1,"");
  }
  manualsky_table->setNumRows(0);
  
  manual = "";
}


void manualsky::showhide_rows()
{
  long i, k, l;
  
  if (manualsky_table->numRows() == 0) return;
  
  if (ms_ccdselectionComboBox->currentItem() == 0) {
    k = 1;
    for (i=0; i<manualsky_table->numRows(); i++) {
      l = 0;
      if (k>1 && l == 0) {
	manualsky_table->hideRow(i);
	l = 1;
      }
      if (k==1 && l == 0) {
	manualsky_table->showRow(i);
	l = 1;
      }
      if (k==manualsky_numchips.toInt()) k = 0;
      k++;
    }
  }
  
  if (ms_ccdselectionComboBox->currentItem() == 1) {
    for (i=0; i<manualsky_table->numRows(); i++) {
      manualsky_table->showRow(i);
    }
  }
}


void manualsky::update_table()
{
  long i, j, k, chipnumber;
  QString mean, median, mode, name, lquart;
  
  if (manualsky_table->numRows() == 0) return;
  
  // plot the table accordingly
  // show the full table
  if (ms_showchipComboBox->currentItem() == 0) {
    for (k=0; k<allnumfiles; k++) {
      name = bg[k].name;
      mode = mode.setNum(bg[k].mode);
      mean = mean.setNum(bg[k].mean);
      median = median.setNum(bg[k].median);
      lquart = lquart.setNum(bg[k].lquart);
      manualsky_table->setText(k,0,name);
      
      if (ms_showmethodComboBox->currentItem() == 0) {
	manualsky_table->setText(k,1,mode);
	manualsky_horizontal->setLabel(1,"Mode");
      }
      if (ms_showmethodComboBox->currentItem() == 1) {
	manualsky_table->setText(k,1,median);
	manualsky_horizontal->setLabel(1,"Median");
      }
      if (ms_showmethodComboBox->currentItem() == 2) {
	manualsky_table->setText(k,1,mean);
	manualsky_horizontal->setLabel(1,"Mean");
      }
      if (ms_showmethodComboBox->currentItem() == 3) {
	manualsky_table->setText(k,1,lquart);
	manualsky_horizontal->setLabel(1,"L. Quart.");
      }
    }
    showhide_rows();
  }
  
  // only the n-th values
  if (ms_showchipComboBox->currentItem() > 0) {
    chipnumber = ms_showchipComboBox->currentItem();
    i = 0;
    j = chipnumber - 1;
    for (k=0; k<allnumfiles; k++) {
      if (i>=manualsky_numchips.toInt()) {
	j = j + manualsky_numchips.toInt();
	i = 0;
      }
      name = bg[k].name;
      mode = mode.setNum(bg[j].mode);
      median = median.setNum(bg[j].median);
      mean = mean.setNum(bg[j].mean);
      lquart = lquart.setNum(bg[j].lquart);
      manualsky_table->setText(k,0,name);
      if (ms_showmethodComboBox->currentItem() == 0) {
	manualsky_table->setText(k,1,mode);
	manualsky_horizontal->setLabel(1,"Mode");
      }
      if (ms_showmethodComboBox->currentItem() == 1) {
	manualsky_table->setText(k,1,median);
	manualsky_horizontal->setLabel(1,"Median");
      }
      if (ms_showmethodComboBox->currentItem() == 2) {
	manualsky_table->setText(k,1,mean);
	manualsky_horizontal->setLabel(1,"Mean");
      }
      if (ms_showmethodComboBox->currentItem() == 3) {
	manualsky_table->setText(k,1,lquart);
	manualsky_horizontal->setLabel(1,"L. Quart.");
      }
      i++;
    }
    showhide_rows();
  }
}


void manualsky::get_manualsky()
{
  int i, chipnumber;
  QString directory, allfilter, filter, filtertmp;
  QString execstring, section, exec, current;
  QString qs1, qs2, qs3, qs4, qs5;
  QString xmin, xmax, ymin, ymax, execstr;
  QString ra1, ra2, dec1, dec2;
  QString resultname, resultnametmp;
  QString line;
  
  QStringList allfilelist;
  QDir dir;
  
  directory = sciencedir;
  filtertmp = statusstring;
  xmin = ms_xminLineEdit->text();
  xmax = ms_xmaxLineEdit->text();
  ymin = ms_yminLineEdit->text();
  ymax = ms_ymaxLineEdit->text();
  ra1  = ms_ra1LineEdit->text();
  ra2  = ms_ra2LineEdit->text();
  dec1 = ms_dec1LineEdit->text();
  dec2 = ms_dec2LineEdit->text();
  
  // swap min and max if accidentally inverted
  if (xmax.toInt() < xmin.toInt()) {
    xmin = ms_xmaxLineEdit->text();
    xmax = ms_xminLineEdit->text();
  }
  if (ymax.toInt() < ymin.toInt()) {
    ymin = ms_ymaxLineEdit->text();
    ymax = ms_yminLineEdit->text();
  }

  dir = QDir(directory);
  
  if (!dir.exists() || directory.isEmpty()) {
    QMessageBox::warning( this, "Missing directory",
			  "The SCIENCE directory does not exist!\n");
    return;
  }
  
  allfilter = "*";
  allfilter.append(filtertmp);
  allfilter.append(".fits");
  
  filter = "*";
  if (ms_showchipComboBox->currentItem() > 0) {
    chipnumber = ms_showchipComboBox->currentItem();
    current = current.setNum(chipnumber);
    filter.append(current);
  }
  filter.append(filtertmp);
  filter.append(".fits");
  
  allfilelist = dir.entryList(allfilter);
  allnumfiles = dir.entryList(allfilter).count();
  
  free(bg);
  bg = (struct statistics *) malloc(allnumfiles*sizeof(struct statistics));
  
  if (allnumfiles == 0) {
    QMessageBox::warning( this, "No images found",
			  "No "+filtertmp+"-images found in this directory!\n");
    return;
  }
  
  // clear the table
  for (i=0; i<manualsky_table->numRows(); i++) {
    manualsky_table->setText(i,0,"");
    manualsky_table->setText(i,1,"");
  }
  manualsky_table->setNumRows(allnumfiles);
  
  // if the user specified a subregion, then make sure that only RA/DEC or 
  // x/y is specified. 
  
  int test_cartesian = 0, test_equatorial = 0;
  
  if (xmin.isEmpty() && xmax.isEmpty() && 
      ymin.isEmpty() && ymax.isEmpty()) test_cartesian = 1;
  
  if (ra1.isEmpty() && ra2.isEmpty() && 
      dec1.isEmpty() && dec2.isEmpty()) test_equatorial = 1;
  
  if (test_cartesian != 1  && test_equatorial != 1) {
    QMessageBox::warning( this, "Constant sky subtraction",
			  "The empty sky region must be defined either through 4 RA/DEC\n"
			  "or through 4 x/y coordinates. Only one of the two options can be selected.\n"
			  "" );
    return;
  }
  
  resultnametmp = "skybackground_";
  resultnametmp.append(filtertmp);
  resultnametmp.append(".stat_tmp");
  resultname = "skybackground_";
  resultname.append(filtertmp);
  resultname.append(".stat");
  
  // convert RA/DEC to x/y if needed.
  if (!ra1.isEmpty() && !ra2.isEmpty() && !dec1.isEmpty() && !dec2.isEmpty()) {
    execstr = "cd ";
    execstr.append(manualsky_scriptdir);
    execstr.append("; ");
    execstr.append("./get_constsky_helper.sh ");
    execstr.append(sciencedir);
    execstr.append(" ");
    execstr.append(statusstring);
    execstr.append(" ");
    execstr.append(manualsky_instrument);
    execstr.append(" ");
    execstr.append(ra1);
    execstr.append(" ");
    execstr.append(ra2);
    execstr.append(" ");
    execstr.append(dec1);
    execstr.append(" ");
    execstr.append(dec2);
    execstr.append(" > /dev/null 2>&1");
    my_system(execstr);
  }
  
  else {
    if (!xmin.isEmpty() && !xmax.isEmpty() && !ymin.isEmpty() && !ymax.isEmpty()) {
      section = " -s ";
      section.append(xmin);
      section.append(" ");
      section.append(xmax);
      section.append(" ");
      section.append(ymin);
      section.append(" ");
      section.append(ymax);
      section.append(" ");
    }
    else if (test_cartesian == 1 && test_equatorial == 1) section = "";
    else {
      // return if the user entered incomplete or contradictory information
      QMessageBox::warning( this, "Constant sky subtraction",
			    "The empty sky region must be defined either through 4 RA/DEC\n"
			    "or through 4 x/y coordinates. Only one of the two options can be selected.\n"
			    "");
      return;
    }
    
    // ok, region selection is useful, continue determining the sky
    execstr = "cd ";
    execstr.append(directory);
    execstr.append("; ");
    execstr.append(manualsky_bindir);
    execstr.append("/imstats ");
    execstr.append(section);
    execstr.append(" -d 6 -t -1000000 1000000 ");
    execstr.append(" -o ");
    execstr.append(directory);
    execstr.append("/");
    execstr.append(resultnametmp);
    execstr.append(" ");
    execstr.append(allfilter);
    my_system(execstr);
    
    execstr = "gawk '($1 !~ \"#\") {print $1, $2, $3, $4, $5, $6, $7}' ";
    execstr.append(directory);
    execstr.append("/");
    execstr.append(resultnametmp);
    execstr.append(" > ");
    execstr.append(directory);
    execstr.append("/");
    execstr.append(resultname);
    my_system(execstr);
    
    // delete the temporary file
    QString statfilestring = directory;
    statfilestring.append("/");
    statfilestring.append(resultnametmp);
    QFile statfile(statfilestring);
    if (statfile.exists()) statfile.remove();
  }
  
  // read the data, display and store it
  i = 0;
  execstr = directory;
  execstr.append("/");
  execstr.append(resultname);
  
  QFile file( execstr );
  if ( file.open( IO_ReadOnly )) {
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      QStringList lst = lst.split( " ", line );
      if (!line.isEmpty()) {
	strcpy(bg[i].name,lst[0]);
	bg[i].mode = lst[1].toFloat();
	bg[i].lquart = lst[2].toFloat();
	bg[i].median = lst[3].toFloat();
	bg[i].uquart = lst[4].toFloat();
	bg[i].mean = lst[5].toFloat();
	bg[i].rms = lst[6].toFloat();
	i++;
      }
    }
    file.close();
  }
  
  update_table();
  
  mscalldialog = 1;
  save_fullstatistics();
  mscalldialog = 0;
  
  bgdefined = 1;
}


void manualsky::load_manualsky()
{
  QString execstr, line, nrow, type;
  int i;
  
  QString directory;
  
  directory = sciencedir;
  
  QString currentFileName = QFileDialog::getOpenFileName(
	directory,
	"Statistics tables (skybackground*.stat)",
	this,
	"open file dialog",
	"Load a manual sky estimate");
    
  if ( currentFileName.isEmpty() ) return;
  
  // get the number of columns and rows
  execstr = "gawk 'END {print NR}' ";
  execstr.append(currentFileName);
  
  my_fscanf(execstr, nrow);
  
  allnumfiles = nrow.toInt();
  
  // clear the table
  for (i=0; i<manualsky_table->numRows(); i++) {
    manualsky_table->setText(i,0,"");
    manualsky_table->setText(i,1,"");
  }
  manualsky_table->setNumRows(allnumfiles);
  
  free(bg);
  bg = (struct statistics *) malloc(allnumfiles*sizeof(struct statistics));
  
  i = 0;
  
  // read the file
  QFile file( currentFileName );
  if ( file.open( IO_ReadOnly )) {
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      QStringList lst = lst.split( " ", line );
      if (!line.isEmpty()) {
	strcpy(bg[i].name,lst[0]);
	bg[i].mode = lst[1].toFloat();
	bg[i].lquart = lst[2].toFloat();
	bg[i].median = lst[3].toFloat();
	bg[i].uquart = lst[4].toFloat();
	bg[i].mean = lst[5].toFloat();
	bg[i].rms = lst[6].toFloat();
	i++;
      }
    }
    file.close();
  }
  
  update_table();
}


void manualsky::fill_manualvalue()
{
  int i;
  QString directory, filter, filtertmp, value;
  QString resultname, resultnametmp;
  QDir dir;
  QStringList filelist;
  
  value = ms_fillLineEdit->text();
  
  directory = sciencedir;
  filtertmp = statusstring;
  
  dir = QDir(directory);
  
  if (!dir.exists() || directory.isEmpty() || value.isEmpty()) {
    QMessageBox::warning( this, "Missing directory",
			  "The SCIENCE directory does not exist!\n");
    return;
  }
  
  filter = "*";
  filter.append(filtertmp);
  filter.append(".fits");
  
  allnumfiles = dir.entryList(filter).count();
  filelist = dir.entryList(filter);
  
  manualsky_table->setNumRows(allnumfiles);
  
  if (bgdefined == 0) {
    free(bg);
    bg = (struct statistics *) malloc(allnumfiles*sizeof(struct statistics));
  }
  
  // update the values
  for (i=0; i<allnumfiles; i++) {
    manualsky_table->setText(i,0,filelist[i]);
    manualsky_table->setText(i,1,value);
    if (bgdefined == 0) {
      strcpy(bg[i].name,filelist[i]);
      bg[i].mode = value.toFloat();
      bg[i].lquart = value.toFloat();
      bg[i].median = value.toFloat();
      bg[i].uquart = value.toFloat();
      bg[i].mean = value.toFloat();
      bg[i].rms = value.toFloat();
    }
  }
  
  showhide_rows();
}


void manualsky::save_quit()
{
  // check that there is no contradicion in 
  // the definition of the empty sky region 
  
  QString v_sky_ra1, v_sky_ra2, v_sky_dec1, v_sky_dec2,
    v_sky_xmin, v_sky_xmax, v_sky_ymin, v_sky_ymax;
  
  v_sky_ra1 = ms_ra1LineEdit->text();
  v_sky_ra2 = ms_ra2LineEdit->text();
  v_sky_dec1 = ms_dec1LineEdit->text();
  v_sky_dec2 = ms_dec2LineEdit->text();
  v_sky_xmin = ms_xminLineEdit->text();
  v_sky_xmax = ms_xmaxLineEdit->text();
  v_sky_ymin = ms_yminLineEdit->text();
  v_sky_ymax = ms_ymaxLineEdit->text();
  
  if (!v_sky_ra1.isEmpty()  && !v_sky_ra2.isEmpty() && 
      !v_sky_dec1.isEmpty() && !v_sky_dec1.isEmpty() &&
      !v_sky_xmin.isEmpty() && !v_sky_xmax.isEmpty() && 
      !v_sky_ymin.isEmpty() && !v_sky_ymax.isEmpty()) {
    if (ms_buttonGroup->selectedId() == 1) {
      QMessageBox::warning( this, "Constant sky subtraction",
			    "The empty sky region must be defined either through 4 RA/DEC\n"
			    "or through 4 x/y coordinates. Only one of the two options can be selected.\n"
			    "");
      return;
    }
    else {
      ms_xminLineEdit->clear();
      ms_xmaxLineEdit->clear();
      ms_yminLineEdit->clear();
      ms_ymaxLineEdit->clear();
    }
  }
  
  save_wysiwyg();
  free(bg);
  manualsky::close();
}


void manualsky::save_fullstatistics()
{
  FILE *pipe;
  QString qs1, qs2, qs3, qs4, qs5, qs6, qs7, currentFileName; 
  QString directory, filename, filter, filterreplace, resultname;
  int i;
  
  if (manualsky_table->numRows() == 0) return;
  
  directory = sciencedir;
  filter = statusstring;
  
  resultname = directory;
  resultname.append("/skybackground_");
  resultname.append(filter);
  resultname.append(".stat");
  
  if (mscalldialog == 0) {
    currentFileName = QFileDialog::getSaveFileName(
	    resultname,
	    "Manual/constant sky estimate (skybackground*.stat)",
	    this,
	    "save file dialog",
	    "Select a name for the manual sky estimate");
	
    if ( currentFileName.isEmpty() ) return;
  }
  else currentFileName = resultname;
  
  int answer = 0;
  // Print warning if an existing file is going to be overwritten.
  if ( QFile::exists( currentFileName ) && mscalldialog == 0)
    answer = QMessageBox::warning(
	    this, "Overwrite File",
	    QString( "<b>WARNING:</b> \n\nYou are going to overwrite\n\n\'%1\'?" ).
	    arg( currentFileName ),
	    "&OK", "&Cancel", QString::null, 1, 1 );

  // Save file otherwise
  if ( (answer == 0 && mscalldialog == 0) || mscalldialog == 1) {
    i = 0;
    pipe = fopen(currentFileName, "w");
    for (i=0; i<manualsky_table->numRows(); i++) {
      qs1 = bg[i].name;
      qs2 = qs2.setNum(bg[i].mode);
      qs3 = qs3.setNum(bg[i].lquart);
      qs4 = qs4.setNum(bg[i].median);
      qs5 = qs5.setNum(bg[i].uquart);
      qs6 = qs6.setNum(bg[i].mean);
      qs7 = qs7.setNum(bg[i].rms);
      fprintf(pipe, "%s %s %s %s %s %s %s\n", qs1.ascii(), qs2.ascii(), 
	      qs3.ascii(), qs4.ascii(), qs5.ascii(), qs6.ascii(), 
	      qs7.ascii()); 
    }
    fclose(pipe);
  }
}


void manualsky::save_wysiwyg()
{
  FILE *pipe;
  QString qs1, qs2, directory, resultname, check;
  int i, j, k, nrows;

  // all the following stuff is for the "manual sky" option
  directory = sciencedir;
  
  resultname = directory;
  resultname.append("/skybackground.use");
  
  // if the table appears to be empty and the user has selected
  // "manuals sky", let's not save anything
  check = manualsky_table->text(0,0);
  
  if (ms_buttonGroup->selectedId() == 1 && check.compare("") != 0 &&
      manualsky_table->numRows() != 0) {
    nrows = manualsky_table->numRows();
    manual = "MANUAL";
    pipe = fopen(resultname, "w");
    
    // if the table shows only the exposures
    if (ms_ccdselectionComboBox->currentItem() == 0) {
      i = 0;
      k = 0;
      j = 0;
      for (k=0; k<nrows; k++) {
	if (i>=manualsky_numchips.toInt()) {
	  j = j + manualsky_numchips.toInt();
	  i = 0;
	}
	qs1 = manualsky_table->text(k,0);
	qs2 = manualsky_table->text(j,1);
	fprintf(pipe, "%s %s\n", qs1.ascii(), qs2.ascii()); 
	i++;
      }
    }
    
    // if the table shows individual images
    if (ms_ccdselectionComboBox->currentItem() == 1) {
      k = 0;
      for (k=0; k<nrows; k++) {
	qs1 = manualsky_table->text(k,0);
	qs2 = manualsky_table->text(k,1);
	fprintf(pipe, "%s %s\n", qs1.ascii(), qs2.ascii()); 
      }
    }
    fclose(pipe);
  }
  else manual = "";
  
  // write whatever has been entered into the settings file
  write_parameters();
}


void manualsky::populate_manualskytable()
{
  QHeader *manualsky_vertical = manualsky_table->verticalHeader();
  manualsky_horizontal = manualsky_table->horizontalHeader();
  manualsky_vertical->hide();
  manualsky_table->setNumCols(2);
  manualsky_table->setNumRows(0);
  manualsky_table->setLeftMargin(0);
  manualsky_table->setColumnStretchable(0,true);
  manualsky_table->setColumnStretchable(1,false);
  manualsky_table->setColumnReadOnly(0,true);
  manualsky_table->setColumnWidth(1,74);
  
  manualsky_horizontal->setLabel(0,"Image name");
  manualsky_horizontal->setLabel(1,"Mode");
}


void manualsky::init()
{
    int array[6], i;
    QString logpath, qtmp, lastlog, execstr, opsys, result;
    QString sciencedirtmp, qnum;
    QString number;
    QStringList status;
    QSettings settings (QSettings::Ini);

    // get general info
    settings.beginGroup("/theligeneral_");
      manualsky_lastlog = settings.readEntry("/lastlog", "noname");
      manualsky_scriptdir = settings.readEntry("/mainpath", "");
      manualsky_homedir = settings.readEntry("/homedir", "");
      manualsky_bindir = settings.readEntry("/bindir", "");
      opsys = settings.readEntry("/opsys", "");
    settings.endGroup();

    settings.beginGroup("/theli_"+manualsky_lastlog+"_");
      settings.beginGroup("/COADDITION");
        settings.beginGroup("/skysub");
	  ms_buttonGroup->setButton(settings.readNumEntry("/radiobuttonID",0));
	  ms_saveskymodelCheckBox->setChecked(settings.readBoolEntry("/saveskymodel",false));
	settings.endGroup();
      settings.endGroup();

      started_proc_manualsky = 0;

      // grep the current science directory
      settings.beginGroup("/INITIALISE");
        sciencedir = settings.readEntry("/maindir");
	sciencedir.append(settings.readEntry("/sciencedir"));
	manualsky_instrument = settings.readEntry("/instrumentname");
	manualsky_inst_prof_index = settings.readNumEntry("/inst_prof_index");
	manualsky_inst_comm_index = settings.readNumEntry("/inst_comm_index");
	manualsky_inst_user_index = settings.readNumEntry("/inst_user_index");
	if (settings.readBoolEntry("/inst_prof",true)) {
	  manualsky_instprofRB = 1;
	  manualsky_inst_path = manualsky_scriptdir + "/instruments_professional/";
	}
	if (settings.readBoolEntry("/inst_comm",true)) {
	  manualsky_instcommRB = 1;
	  manualsky_inst_path = manualsky_scriptdir + "/instruments_commercial/";
	}
	if (settings.readBoolEntry("/inst_user",true)) {
	  manualsky_instuserRB = 1;
	  manualsky_inst_path = manualsky_homedir + "/.theli/instruments_user/";
	}
      settings.endGroup();
      statusscience = settings.readEntry("/PROCESSINGSTATUS/status");
      status = status.split( " ", statusscience );
      for (i = 0; i < 6; i++) {
	  qtmp=statusscience.at(i);
	  array[i] = qtmp.toInt();
      }
      result = "";
      if (array[0] == 1) {
	  result.append("OFC");
	  if (array[1] == 1) result.append("B");
	  if (array[2] == 1) result.append("H");
	  if (array[3] == 1) result.append("C");
	  if (array[4] == 1) result.append("D");
	  if (array[5] == 1) result.append("P");
      }
      statusstring = result;

    settings.endGroup();

    // get the number of chips
    execstr = "grep NCHIPS= ";
    execstr.append(manualsky_inst_path);
    execstr.append("/");
    execstr.append(manualsky_instrument);
    execstr.append(".ini");
    execstr.append("| gawk 'BEGIN {FS=\"=\"} {print $2}'");
    my_fscanf(execstr, manualsky_numchips);

    populate_manualskytable();

    manual = "";

    // populate the chip ComboBoxes
    ms_chipComboBox->clear();
    ms_chipComboBox->insertItem("From each chip", 0);
    for (i=1; i<=manualsky_numchips.toInt(); i++) {
	number = number.setNum(i);
	qnum = "From chip ";
	qnum.append(number);
	ms_chipComboBox->insertItem(qnum, i);
    }

    ms_showchipComboBox->clear();
    ms_showchipComboBox->insertItem("Use all chips", 0);
    for (i=1; i<=manualsky_numchips.toInt(); i++) {
	number = number.setNum(i);
	qnum = "Use chip ";
	qnum.append(number);
	ms_showchipComboBox->insertItem(qnum, i);
    }

    // fill in the remaining fields
    read_parameters();

    mscalldialog = 0;

    ms_closePushButton->setFocus();

    bgdefined = 0;

    // initialise the structure
    bg = (struct statistics *) malloc(sizeof(struct statistics));

    showhide_ccdselection();
}


// write the parameters into a Qt file for memory
void manualsky::write_parameters()
{
  QString execstr;
    
  QString v_sky_ra1, v_sky_ra2, v_sky_dec1, v_sky_dec2,
    v_detectthresh, v_detectminarea, v_backsize, v_maskexpand, 
  	v_sky_xmin, v_sky_xmax, v_sky_ymin, v_sky_ymax;
  
  // catch all nonsense the user might enter and replace it by
  // default values

  // only those values where we need to check something are
  // put into v_xxxxx variables. The rest is written directly
  // into the Qt file

  v_sky_ra1 = ms_ra1LineEdit->text();
  v_sky_ra2 = ms_ra2LineEdit->text();
  v_sky_dec1 = ms_dec1LineEdit->text();
  v_sky_dec2 = ms_dec2LineEdit->text();
  if (v_sky_ra1.isEmpty() || v_sky_dec1.isEmpty() || 
  	v_sky_ra2.isEmpty() || v_sky_dec2.isEmpty()) {
    v_sky_ra1 = "";
    v_sky_ra2 = "";
    v_sky_dec1 = "";
    v_sky_dec2 = "";
    ms_ra1LineEdit->clear();
    ms_ra2LineEdit->clear();
    ms_dec1LineEdit->clear();
    ms_dec2LineEdit->clear();
  }
  
  v_sky_xmin = ms_xminLineEdit->text();
  v_sky_xmax = ms_xmaxLineEdit->text();
  v_sky_ymin = ms_yminLineEdit->text();
  v_sky_ymax = ms_ymaxLineEdit->text();
  if (v_sky_xmin.isEmpty() || v_sky_ymin.isEmpty() || 
  	v_sky_xmax.isEmpty() || v_sky_ymax.isEmpty()) {
    v_sky_xmin = "";
    v_sky_xmax = "";
    v_sky_ymin = "";
    v_sky_ymax = "";
    ms_xminLineEdit->clear();
    ms_xmaxLineEdit->clear();
    ms_yminLineEdit->clear();
    ms_ymaxLineEdit->clear();
  }
  
  v_detectthresh  = ms_skysubdetectthreshLineEdit->text();
  v_detectminarea = ms_skysubdetectminareaLineEdit->text();
  v_backsize      = ms_skysubbacksizeLineEdit->text();
  v_maskexpand    = ms_maskexpandfactorLineEdit->text();

  if (ms_buttonGroup->selectedId() == 0) {
    if (v_detectthresh.isEmpty())  {
  	v_detectthresh = "1.5";
  	ms_skysubdetectthreshLineEdit->setText("1.5");
    }
    if (v_detectminarea.isEmpty()) {
  	v_detectminarea = "5";
  	ms_skysubdetectminareaLineEdit->setText("5");
    }
    if (v_backsize.isEmpty()) {
  	v_backsize = "256";
  	ms_skysubbacksizeLineEdit->setText("256");
    }
  }
  else {
    if (v_detectthresh.isEmpty())  v_detectthresh = "";
    if (v_detectminarea.isEmpty()) v_detectminarea = "";
    if (v_backsize.isEmpty())      v_backsize = "";
  }

  // WRITE THE LOG FILE
  {
  QSettings settings (QSettings::Ini);
  QString reductionlog = manualsky_lastlog;
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
  }
  settings.beginGroup("/theli_"+reductionlog+"_");
    settings.beginGroup("/COADDITION");
      settings.beginGroup("/skysub");
        settings.writeEntry("/radiobuttonID", ms_buttonGroup->selectedId());
	settings.writeEntry("/saveskymodel", ms_saveskymodelCheckBox->isChecked());
	settings.writeEntry("/detectthresh", v_detectthresh);
	settings.writeEntry("/detectminarea", v_detectminarea);
	settings.writeEntry("/maskexpand", v_maskexpand);
	settings.writeEntry("/backsize", v_backsize);
	settings.writeEntry("/cskyra1", v_sky_ra1);
	settings.writeEntry("/cskyra2", v_sky_ra2);
	settings.writeEntry("/cskydec1", v_sky_dec1);
	settings.writeEntry("/cskydec2", v_sky_dec2);
	settings.writeEntry("/cskyxmin", v_sky_xmin);
	settings.writeEntry("/cskyxmax", v_sky_xmax);
	settings.writeEntry("/cskyymin", v_sky_ymin);
	settings.writeEntry("/cskyymax", v_sky_ymax);
	settings.writeEntry("/fill", ms_fillLineEdit->text());
	settings.writeEntry("/method",ms_methodComboBox->currentItem());
	settings.writeEntry("/chip",ms_chipComboBox->currentItem());
	settings.writeEntry("/showmethod",ms_showmethodComboBox->currentItem());
	settings.writeEntry("/showchip",ms_showchipComboBox->currentItem());
	settings.writeEntry("/ccdselection",ms_ccdselectionComboBox->currentItem());
	settings.writeEntry("/manual",manual);
      settings.endGroup();
    settings.endGroup();
  settings.endGroup();
  }
  
  // write the parameter file
  modify_parametersini();
  
  manualsky::close();
}


// read the parameters from the Qt file when the dialog opens
void manualsky::read_parameters()
{
  QSettings settings (QSettings::Ini);
  
  QString reductionlog = manualsky_lastlog;
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
  }
  
  settings.beginGroup("/theli_"+reductionlog+"_");
    settings.beginGroup("/COADDITION");
      settings.beginGroup("/skysub");
	ms_buttonGroup->setButton(settings.readNumEntry("/radiobuttonID",0));
	ms_saveskymodelCheckBox->setChecked(settings.readBoolEntry("/saveskymodel",false));
	ms_skysubdetectthreshLineEdit->setText(settings.readEntry("/detectthresh","1.5"));
	ms_skysubdetectminareaLineEdit->setText(settings.readEntry("/detectminarea","5"));
	ms_skysubbacksizeLineEdit->setText(settings.readEntry("/backsize","256"));
	ms_maskexpandfactorLineEdit->setText(settings.readEntry("/maskexpand",""));
        ms_ra1LineEdit->setText(settings.readEntry("/cskyra1",""));
        ms_ra2LineEdit->setText(settings.readEntry("/cskyra2",""));
        ms_dec1LineEdit->setText(settings.readEntry("/cskydec1",""));
        ms_dec2LineEdit->setText(settings.readEntry("/cskydec2",""));
        ms_xminLineEdit->setText(settings.readEntry("/cskyxmin",""));
        ms_xmaxLineEdit->setText(settings.readEntry("/cskyxmax",""));
        ms_yminLineEdit->setText(settings.readEntry("/cskyymin",""));
        ms_ymaxLineEdit->setText(settings.readEntry("/cskyymax",""));
	ms_fillLineEdit->setText(settings.readEntry("/fill",""));
	ms_methodComboBox->setCurrentItem(settings.readNumEntry("/method",0));
	ms_chipComboBox->setCurrentItem(settings.readNumEntry("/chip",0));
	ms_showmethodComboBox->setCurrentItem(settings.readNumEntry("/showmethod",0));
	ms_showchipComboBox->setCurrentItem(settings.readNumEntry("/showchip",0));
	ms_ccdselectionComboBox->setCurrentItem(settings.readNumEntry("/ccdselection",0));
      settings.endGroup();
    settings.endGroup();
  settings.endGroup();
}


// write the parameters contained in the Qt file into
// an ascii file for usage in the scripts
void manualsky::modify_parametersini()
{
  FILE *of;
  
  QString paramsinifile, paramsinifiletmp;
  
  QSettings settings(QSettings::Ini);
  QString execstr;
  
  //    if (!manualsky_scriptdir.endsWith("/")) 
  //	manualsky_scriptdir.append("/");
  
  check_last_char(manualsky_homedir);
  
  paramsinifile = manualsky_homedir;
  paramsinifile.append(".theli/param_set3.ini");
  paramsinifiletmp = manualsky_homedir;
  paramsinifiletmp.append(".theli/param_set3.ini_tmp");
  
  QString reductionlog = manualsky_lastlog;
  if (reductionlog.isEmpty()) {
    reductionlog = "noname";
  }
  
  of = fopen(paramsinifiletmp, "w");
  
  // read what is written in the Qt file
  settings.beginGroup("/theli_"+reductionlog+"_");
    settings.beginGroup("/COADDITION");
      settings.beginGroup("/skysub");
        ms_buttonGroup->setButton(settings.readNumEntry("/radiobuttonID",0));
	fprintf(of, "%s%s\n", "V_SKYSUBDETECTTHRESH=",  (settings.readEntry("/detectthresh","")).ascii());
	fprintf(of, "%s%s\n", "V_SKYSUBDETECTMINAREA=", (settings.readEntry("/detectminarea","")).ascii());
	fprintf(of, "%s%s\n", "V_SKYSUBBACKSIZE=",      (settings.readEntry("/backsize","")).ascii());
	fprintf(of, "%s%s\n", "V_SKYSUBMASKEXPAND=",    (settings.readEntry("/maskexpand","")).ascii());
	if (settings.readBoolEntry("/saveskymodel",false))
	  fprintf(of, "%s\n", "V_SAVESKYMODEL=Y");
	else 
	  fprintf(of, "%s\n", "V_SAVESKYMODEL=N");
	fprintf(of, "%s%s\n", "V_CSKY_RA1=",   (settings.readEntry("/cskyra1","")).ascii());
	fprintf(of, "%s%s\n", "V_CSKY_RA2=",   (settings.readEntry("/cskyra2","")).ascii());
	fprintf(of, "%s%s\n", "V_CSKY_DEC1=",   (settings.readEntry("/cskydec1","")).ascii());
	fprintf(of, "%s%s\n", "V_CSKY_DEC2=",   (settings.readEntry("/cskydec2","")).ascii());
	fprintf(of, "%s%s\n", "V_CSKY_XMIN=",   (settings.readEntry("/cskyxmin","")).ascii());
	fprintf(of, "%s%s\n", "V_CSKY_XMAX=",   (settings.readEntry("/cskyxmax","")).ascii());
	fprintf(of, "%s%s\n", "V_CSKY_YMIN=",   (settings.readEntry("/cskyymin","")).ascii());
	fprintf(of, "%s%s\n", "V_CSKY_YMAX=",   (settings.readEntry("/cskyymax","")).ascii());
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


void manualsky::defaults()
{
  clearfields();
  ms_fillLineEdit->clear();
  ms_skysubbacksizeLineEdit->setText("256");
  ms_skysubdetectminareaLineEdit->setText("5");
  ms_skysubdetectthreshLineEdit->setText("1.5");
  ms_maskexpandfactorLineEdit->clear();
  ms_ccdselectionComboBox->setCurrentItem(0);
  ms_showmethodComboBox->setCurrentItem(0);
  ms_showchipComboBox->setCurrentItem(0);
  ms_methodComboBox->setCurrentItem(0);
  ms_chipComboBox->setCurrentItem(0);
  ms_buttonGroup->setButton(0);
  ms_saveskymodelCheckBox->setChecked(false);
  ms_ra1LineEdit->clear();
  ms_ra2LineEdit->clear();
  ms_dec1LineEdit->clear();
  ms_dec2LineEdit->clear();
  ms_xminLineEdit->clear();
  ms_xmaxLineEdit->clear();
  ms_yminLineEdit->clear();
  ms_ymaxLineEdit->clear();
}


void manualsky::showhide_ccdselection()
{
  if (manualsky_numchips.toInt() == 1) {
    ms_ccdselectionComboBox->setCurrentItem(0);
    ms_showchipComboBox->setCurrentItem(0);
    ms_chipComboBox->setCurrentItem(0);
    ms_ccdselectionComboBox->setDisabled(true);
    ms_showchipComboBox->setDisabled(true);
    ms_chipComboBox->setDisabled(true);
  }
  if (manualsky_numchips.toInt() > 1) {
    ms_ccdselectionComboBox->setDisabled(false);
    ms_showchipComboBox->setDisabled(false);
    ms_chipComboBox->setDisabled(false);
  }
}


void manualsky::switch_selection()
{
  if (ms_buttonGroup->selectedId() == 0) {
    ms_1GroupBox->setEnabled(false);
    ms_frame2->setEnabled(false);
    ms_frame3->setEnabled(false);
  }
  if (ms_buttonGroup->selectedId() == 1) {
    ms_1GroupBox->setEnabled(true);
    ms_frame2->setEnabled(true);
    ms_frame3->setEnabled(true);
  }
}


void manualsky::show_info()
{
  QMessageBox::information( this, "Important note",
	"Note that for the estimation of a constant sky background objects \n"
	"will still be detected and removed from the data. The thresholds \n"
        "given in the sky modelling section will be used. Only if you manually \n"
        "override the estimates (in the right half of this dialogue), \n"
        "no object subtraction will be performed.\n");
}
