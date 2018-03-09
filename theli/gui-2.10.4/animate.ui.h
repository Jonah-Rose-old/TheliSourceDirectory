#include <qdir.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <math.h>
#include <qsettings.h>
#include <qimage.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include "functions.h"

int direxists, getstat, numberofchips, started;
int proc_col_movie_started;
QString colordir, coadddir, animate_scriptdir, animate_homedir, animate_bindir;
QString rfac, vfac, bfac, stiff_mode, luminancetiff, sciencedir, animate_pid;
QString r_image, v_image, b_image, l_image, globalmask, instrument;
QDir animate_directory, coadd_directory;
QProcess *proc_col_movie;
QFile convolve_images_file;
QStringList current_imagelist;


void animate::select_dir()
{
  QString selected = QFileDialog::getExistingDirectory(
                    sciencedir,
                    this,
                    "Get SCIENCE directory",
                    "Choose the SCIENCE directory containing coadditions",
                    TRUE );

  animate_dirLineEdit->setText(selected);

  validator_main();
}


void animate::update_chipnum()
{
  QString qnum, number;
  int i;
  
  animate_chipComboBox->clear();
  numberofchips = numchips.toInt();
  for (i=0; i<numberofchips; i++) {
    number = number.setNum(i+1);
    qnum = "CCD ";
    qnum.append(number);
    animate_chipComboBox->insertItem(qnum, i);
  }
  if (numberofchips > 1) {
    animate_chipComboBox->show();
    animate_chipTextLabel->show();
  }
  else {
    animate_chipComboBox->hide();
    animate_chipTextLabel->hide();
  }
}


void animate::fill_in_moviedivisions()
{
  QString nxstr, nystr, div_id;
  int i, j, nx, ny;
  
  nx = animate_divxSpinBox->value();
  ny = animate_divySpinBox->value();
  
  animate_divListBox->clear();
  
  for (j=0; j<ny; j++) {
    for (i=0; i<nx; i++) {
      nxstr = nxstr.setNum(i+1);
      nystr = nystr.setNum(j+1);
      div_id = nystr+"-"+nxstr;
      animate_divListBox->insertItem(div_id);
    }
  }
  
  animate_divListBox->setCurrentItem(0);
  animate_divListBox->setSelected(0,true);
}


void animate::setstate_manualthresholds()
{
  if (animate_rangeComboBox->currentItem() == 1) {
    animate_TIFFminLineEdit->hide();
    animate_TIFFmaxLineEdit->hide();
    animate_minmaxTextLabel->hide();
    animate_contrastSpinBox->show();
    animate_contrastTextLabel->show();
  }
  else {
    animate_TIFFminLineEdit->show();
    animate_TIFFmaxLineEdit->show();
    animate_minmaxTextLabel->show();
    animate_contrastSpinBox->hide();
    animate_contrastTextLabel->hide();
  }
}


void animate::make_movie_step1()
{
  QStringList coaddlist, divlist;
  QString currentcoadddir, xmin, xmax, ymin, ymax, croppedsizes;
  QString execstr, id, division, mask, nax1, nax2;
  QString croppeddirstring, chip, command;
  QDir moviedir, croppeddir;
  int i, numimages, num_coadds;
  int chipindex;
  
  command = "";
  
  // list with the coadditions for which a movie is desired
  num_coadds = 0;
  numimages = animate_coaddListBox->numRows();
  
  if (numimages == 0) return;
  
  for (i=0; i<numimages; i++) {
    if (animate_coaddListBox->isSelected(i)) {
      currentcoadddir = animate_coaddListBox->text(i);
      coaddlist.append(currentcoadddir);
      num_coadds++;
      // make movie dir if it does not exist yet
      moviedir.setPath(coadddir+"/"+currentcoadddir+"/movie");
      if (!moviedir.exists()) my_system("mkdir "+coadddir+"/"+currentcoadddir+"/movie");
    }
  }
  
  // if no coadditions are selected, then select all
  if (num_coadds == 0) {
    animate_coaddListBox->selectAll(true);
    for (i=0; i<numimages; i++) {
      currentcoadddir = animate_coaddListBox->text(i);
      coaddlist.append(currentcoadddir);
      num_coadds++;
      // make movie dir if it does not exist yet
      moviedir.setPath(coadddir+"/"+currentcoadddir+"/movie");
      if (!moviedir.exists()) my_system("mkdir "+coadddir+"/"+currentcoadddir+"/movie");
    }
  }
  
  if (animate_maskCheckBox->isOn()) mask = "mask";
  else mask = "nomask";
  
  if (numberofchips == 1) chip = "1";
  if (numberofchips > 1) {
    chipindex = animate_chipComboBox->currentItem();
    chip = chip.setNum(chipindex+1);
  }
  
  // launch the first step

  // iterate over the coadditions
  for ( QStringList::Iterator it = coaddlist.begin(); it != coaddlist.end(); ++it ) {
    currentcoadddir = *it;
    // if the masking parameter has changed, delete the "cropped" directory
    if (mask.compare(globalmask) != 0) {
      croppeddirstring = coadddir+"/"+currentcoadddir+"/movie/cropped_"+chip;
      my_system("rm -rf "+croppeddirstring);
    }

    command += "./makemovie_step1.sh " +
      animate_scriptdir + " " + coadddir+ "/" + currentcoadddir + 
      " " + mask + " " + chip + "\n";
  }
  
  proc_col_movie_started = 0;
  write_superscriptbody_step1(homedir+".theli/scripts/theli_makemovie.sh", 
			command);  
  globalmask = mask;
}


void animate::make_movie_step2()
{
  QStringList coaddlist, divlist;
  QString currentcoadddir, xmin, xmax, ymin, ymax, croppedsizes;
  QString TIFFmin, TIFFmax, execstr, id, range, division, mask, nax1, nax2;
  QString croppeddirstring, contrast, delay, chip, command;
  QDir moviedir, croppeddir;
  int i, numimages, num_coadds, numdivtotal, isec, jsec, overlap;
  int x1, x2, y1, y2, sizex, sizey, naxis1, naxis2, nx, ny, tdelay, chipindex;
  float ccontrast;
  
  command = "";

  // list with the coadditions for which a movie is desired
  numimages = animate_coaddListBox->numRows();
  
  num_coadds = 0;

  if (numimages == 0) return;
  
  for (i=0; i<numimages; i++) {
    if (animate_coaddListBox->isSelected(i)) {
      currentcoadddir = animate_coaddListBox->text(i);
      coaddlist.append(currentcoadddir);
      num_coadds++;
    }
  }
  
  // if no coadditions are selected, then select all
  if (num_coadds == 0) {
    animate_coaddListBox->selectAll(true);
    for (i=0; i<numimages; i++) {
      currentcoadddir = animate_coaddListBox->text(i);
      coaddlist.append(currentcoadddir);
      num_coadds++;
    }
  }

  // list with the divisions for which a movie is desired
  numdivtotal = animate_divListBox->numRows();
  for (i=0; i<numdivtotal; i++) {
    if (animate_divListBox->isSelected(i)) {
      division = animate_divListBox->text(i);
      divlist.append(division);
    }
  }

  // get some parameters
  TIFFmin = animate_TIFFminLineEdit->text();
  TIFFmax = animate_TIFFmaxLineEdit->text();
  if (TIFFmin.isEmpty() || (TIFFmax.isEmpty() && 
			    animate_rangeComboBox->currentItem() == 0)) {
    TIFFmin = "-100";
    TIFFmax = "500";
    animate_TIFFminLineEdit->setText(TIFFmin);
    animate_TIFFmaxLineEdit->setText(TIFFmax);
  }
  if (TIFFmin.isEmpty() || (TIFFmax.isEmpty() && 
			    animate_rangeComboBox->currentItem() == 1)) {
    TIFFmin = "null";
    TIFFmax = "null";
  }
  
  if (animate_rangeComboBox->currentItem() == 1) range = "automatic";
  else range = "manual";
  if (animate_maskCheckBox->isOn()) mask = "mask";
  else mask = "nomask";
  
  tdelay = animate_delaySpinBox->value()*10;
  delay  = delay.setNum(tdelay);
  
  ccontrast = (float) animate_contrastSpinBox->value();
  ccontrast = (ccontrast + 10.)/10.;
  contrast = contrast.setNum(ccontrast);
  
  if (numberofchips == 1) chip = "1";
  if (numberofchips > 1) {
    chipindex = animate_chipComboBox->currentItem();
    chip = chip.setNum(chipindex+1);
  }
  
  // iterate over the coadditions
  for ( QStringList::Iterator it = coaddlist.begin(); it != coaddlist.end(); ++it ) {
    currentcoadddir = *it;

    // calculate the image size of the cropped images
    execstr = animate_scriptdir;
    if (!execstr.endsWith("/")) execstr.append("/");    
    execstr.append("makemovie_step2.sh ");
    execstr.append(animate_scriptdir + " " + coadddir+"/"+currentcoadddir+"/movie/cropped_"+chip);
    my_fscanf2(execstr, nax1, nax2);
    
    naxis1 = nax1.toInt();
    naxis2 = nax2.toInt();
    nx = animate_divxSpinBox->value();
    ny = animate_divySpinBox->value();
    sizex = naxis1 / nx;
    sizey = naxis2 / ny;
    overlap = animate_overlapSpinBox->value();

    // if the masking parameter has changed, delete the "cropped" directory
    if (mask.compare(globalmask) != 0) {
      croppeddirstring = coadddir+"/"+currentcoadddir+"/movie/cropped_"+chip;
      my_system("rm -rf "+croppeddirstring);
    }

    //iterate over the divisions
    for ( QStringList::Iterator divit = divlist.begin(); divit != divlist.end(); ++divit ) {
      id = *divit;
      // calculate the image section for the movie (encoded "j-i")
      // i counts the sections along x
      // j counts the sections along x
      isec = (id.section("-", 1, 1)).toInt();
      jsec = (id.section("-", 0, 0)).toInt();
      
      x1 = (isec - 1) * sizex - overlap;
      y1 = (jsec - 1) * sizey - overlap;
      x2 =  isec * sizex + overlap;
      y2 =  jsec * sizey + overlap;

      if (x1<1) x1 = 1;
      if (x2>naxis1) x2 = naxis1;
      if (y1<1) y1 = 1;
      if (y2>naxis2) y2 = naxis2;
      
      xmin = xmin.setNum(x1);
      xmax = xmax.setNum(x2);
      ymin = ymin.setNum(y1);
      ymax = ymax.setNum(y2);
      
      command += "./makemovie_step3.sh " +
	animate_scriptdir + " " + coadddir+ "/" + currentcoadddir + 
	" " + xmin + " " + xmax + " " + ymin + " " + ymax + " " + 
	TIFFmin + " " + TIFFmax + " " + range + " " + id + " " + 
	contrast + " " + delay + " " + chip + "\n";
    }
  }
  
  proc_col_movie_started = 0;
  write_superscriptbody_step2(homedir+".theli/scripts/theli_makemovie.sh", 
			command);
  
  globalmask = mask;
}


void animate::animate_division()
{
  QStringList tifflist;
  QString coadd, div, moviedirstring, result;
  QString execstr;
  QDir moviedir;
  int i, numimages, num_coadds, numdivtotal, num_divs;
  
  numimages = animate_coaddListBox->numRows();
  numdivtotal = animate_divListBox->numRows();
  
  // check number of coadditions selected
  num_coadds = 0;
  for (i=0; i<numimages; i++) {
    if (animate_coaddListBox->isSelected(i)) num_coadds++;
  }
  
  // check number of divisions selected
  num_divs = 0;
  for (i=0; i<numdivtotal; i++) {
    if (animate_divListBox->isSelected(i)) num_divs++;
  }
  
  if (num_coadds != 1 || num_divs != 1) {
    QMessageBox::warning( this, "Invalid selection",
	 "Please select one coaddition and one division only.\n");
    return;
  }
  
  // identify the coaddition and division to be animated
  coadd = animate_coaddListBox->currentText();
  div = animate_divListBox->currentText();
  
  moviedirstring = coadddir+"/"+coadd+"/movie/";
  moviedir.setPath(moviedirstring);
  if (!moviedir.exists()) {
    QMessageBox::warning( this, "Data not found",
	 "ERROR: directory with the images for the animation not found!\n");
    return;
  }
  
  // check if any division TIFF files are present
  moviedir.setNameFilter(div+"_*.tif");
  tifflist = moviedir.entryList();
  if (tifflist.isEmpty()) {
    QMessageBox::warning( this, "Data not found",
         "ERROR: no TIFF images found for the animation.\n"
	 "Did you create the animation for the division selected?\n");
    return;
  }
  
  // check if the 'animate' programme has been installed
  my_system("which animate > "+moviedirstring+"/log"+" 2>&1");
  QFile animatecheck( moviedirstring+"/log");
  if ( animatecheck.open( IO_ReadOnly )) {
    QTextStream stream( &animatecheck );
    while ( !stream.atEnd() ) {
      result = stream.readLine();
      if (result.contains("not found", FALSE) > 0 ||
	  result.contains("which: no ", FALSE) || 
	  result.isEmpty() || result.length() < 4) {
	QMessageBox::warning( this, "Command not found",
           "The 'animate' programme does not exist on your machine.\n"
	   "Please install the ImageMagick programme suite.\n");
	return;
      }
    }
    animatecheck.close();
    animatecheck.remove();
  }
  
  execstr = "animate "+moviedirstring+"/"+div+"_*.tif";
  my_system(execstr);
}


void animate::update_animatebuttontext()
{
  QString div;
  int i, numdivisions, selected = 0;
  
  numdivisions = animate_divListBox->numRows();
  
  // get the first division selected
  for (i=0; i<numdivisions; i++) {
    if (animate_divListBox->isSelected(i)) {
      selected = i;
      break;
    }
    }
  
  div = animate_divListBox->text(selected);
  
  animate_animatePushButton->setText("Animate division "+div);
}


void animate::get_coadddirs()
{
  QStringList allcoadds, dirlist;
  QFileInfo fileinfo;
  QString tmp;
  
  if (coadd_directory.exists()) {
    // populate the coadd list with directories only
    coadd_directory.setNameFilter("coadd_*");
    allcoadds = coadd_directory.entryList();
    for ( QStringList::Iterator it = allcoadds.begin(); it != allcoadds.end(); ++it ) {
      tmp = *it;
      fileinfo.setFile(coadddir+"/"+tmp);
      if (fileinfo.isDir()) dirlist.append(tmp);
    }
    
    animate_coaddListBox->clear();
    animate_coaddListBox->insertStringList(dirlist);
    animate_coaddListBox->setCurrentItem(0);
  }
  else {
    animate_coaddListBox->clear();
  }
}


void animate::validator_main()
{
  QColor cfred = QColor(255,187,155);
  QColor cfgreen = QColor(155,255,187);
  QColor cwhite = QColor(255,255,255);
  
  // directories must not accept blank characters
  QRegExp rx( "^\\S+$" );
  QValidator* validator_string = new QRegExpValidator( rx, this );
  animate_dirLineEdit->setValidator( validator_string );
  
  // repaint the background colour
  coadddir = animate_dirLineEdit->text();
  coadd_directory.setPath(coadddir);
  direxists = 0;
  if (coadd_directory.exists() && coadddir.length() > 0 ) {
    direxists = 1;
    animate_dirLineEdit->setPaletteBackgroundColor(cfgreen);
  }
  if (!coadd_directory.exists() && coadddir.length() > 0 ) {
    direxists = 0;
    animate_dirLineEdit->setPaletteBackgroundColor(cfred);
  }
  if (coadddir.length() == 0 ) {
    direxists = 0;
    animate_dirLineEdit->setPaletteBackgroundColor(cwhite);
  }
  
  //    get_colorchannels();
  get_coadddirs();
}


void animate::validator_sec()
{
  QRegExp ri_neg( "^[-]{0,1}[0-9]+" );
  QValidator* validator_negint = new QRegExpValidator( ri_neg, this );
  animate_TIFFminLineEdit->setValidator( validator_negint );
  animate_TIFFmaxLineEdit->setValidator( validator_negint );
}


void animate::write_superscriptbody_step1(QString superscript, QString command)
{
  FILE *of;
  QString logfile, execstr, tmp, buttontext;
  
  // leave if the process is already running!
  
  if (proc_col_movie_started == 1)
    return;
  
  logfile = superscript+".log1";
  
  // write the superscript
  of = fopen(superscript, "w");
  fprintf(of, "#!%s\n\n", bash.ascii());
  fprintf(of, "# ******************************************\n");
  fprintf(of, "# *****  Created on the fly by THELI   *****\n");
  fprintf(of, "# ******************************************\n\n");
  fprintf(of, "export INSTRUMENT=%s\n", instrument.ascii());
  fprintf(of, "cd %s\n\n", animate_scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  fprintf(of, "{\n");
  fprintf(of, "%s\n", command.ascii());
  fprintf(of, "} > %s%s\n", logfile.ascii(), " 2>&1");
  fprintf(of, "\n\n");
  fprintf(of, "echo THELIMESSAGE: finished\n");
  fclose(of);
  
  // make the superscript executable
  execstr = "chmod +x ";
  execstr.append(superscript);
  my_system(execstr);
  
  // execute
  proc_col_movie = new QProcess( this );
  proc_col_movie->addArgument( superscript );
  connect(proc_col_movie, SIGNAL(readyReadStdout()), this, SLOT(make_movie_step2()));
  if (proc_col_movie_started == 0) {
    if (!proc_col_movie->start()) {
      printf("Could not start movie process!\n");
      return;
    }
    proc_col_movie_started = 1;
    animate_createPushButton->setText("Creating animation(s) ...");
    animate_createPushButton->setPaletteForegroundColor(QColor(0,155,0));
  }
}


void animate::write_superscriptbody_step2(QString superscript, QString command)
{
  FILE *of;
  QString logfile, execstr, tmp, buttontext;
  
  // leave if the process is already running!
  
  if (proc_col_movie_started == 1)
    return;
  
  logfile = superscript+".log2";
  
  // write the superscript
  of = fopen(superscript, "w");
  fprintf(of, "#!%s\n\n", bash.ascii());
  fprintf(of, "# ******************************************\n");
  fprintf(of, "# *****  Created on the fly by THELI   *****\n");
  fprintf(of, "# ******************************************\n\n");
  fprintf(of, "export INSTRUMENT=%s\n", instrument.ascii());
  fprintf(of, "cd %s\n\n", animate_scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  fprintf(of, "{\n");
  fprintf(of, "%s\n", command.ascii());
  fprintf(of, "} > %s%s\n", logfile.ascii(), " 2>&1");
  fprintf(of, "\n\n");
  fprintf(of, "echo THELIMESSAGE: finished\n");
  fclose(of);
  
  // make the superscript executable
  execstr = "chmod +x ";
  execstr.append(superscript);
  my_system(execstr);
  
  // execute
  
  proc_col_movie = new QProcess( this );
  proc_col_movie->addArgument( superscript );
  connect(proc_col_movie, SIGNAL(readyReadStdout()), this, SLOT(update_movieText()));
  if (proc_col_movie_started == 0) {
    while( !proc_col_movie->isRunning() ) proc_col_movie->launch("");
    proc_col_movie_started = 1;
    animate_createPushButton->setText("Creating animation(s) ...");
    animate_createPushButton->setPaletteForegroundColor(QColor(0,155,0));
  }
}


void animate::update_movieText()
{
  QString tmp, tmp2;
  QByteArray data = proc_col_movie->readStdout();
  tmp = QString(data);
  if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
    tmp2 = tmp.right( tmp.length() - 14);
    if (tmp2.compare("finished\n") == 0) {
      animate_createPushButton->setPaletteForegroundColor(QColor(0,0,0));
      animate_createPushButton->setText("Create animation(s)");
    }
  }
  proc_col_movie_started = 0;
}


void animate::init()
{
  QString lastlog, execstr, tmp, animate_inst_path;
  QSettings settings (QSettings::Ini);
  QColor cfred = QColor(255,187,155);
  QColor cfgreen = QColor(155,255,187);
  QColor cwhite = QColor(255,255,255);
  
  proc_col_movie_started = 0;
  
  started = 0;
  
  // get general info
  settings.beginGroup("/theligeneral_");
  lastlog = settings.readEntry("/lastlog", "noname");
  animate_scriptdir = settings.readEntry("/mainpath", "");
  animate_homedir = settings.readEntry("/homedir", "");
  animate_bindir = settings.readEntry("/bindir", "");
  bash = settings.readEntry("/bash", "");
  settings.endGroup();
  
  // grep the currently selected instrument and science dir
  settings.beginGroup("/theli_"+lastlog+"_");
  settings.beginGroup("/INITIALISE");
  sciencedir = settings.readEntry("/maindir");
  sciencedir.append(settings.readEntry("/sciencedir"));
  instrument = settings.readEntry("/instrumentname");
  if (settings.readBoolEntry("/inst_prof",true)) {
    animate_inst_path = animate_scriptdir + "/instruments_professional/";
  }
  if (settings.readBoolEntry("/inst_comm",true)) {
    animate_inst_path = animate_scriptdir + "/instruments_commercial/";
  }
  if (settings.readBoolEntry("/inst_user",true)) {
    animate_inst_path = animate_homedir + "/.theli/instruments_user/";
  }
  settings.endGroup();
  settings.endGroup();
  
  // get the number of chips
  execstr = "grep NCHIPS= ";
  execstr.append(animate_inst_path);
  execstr.append("/");
  execstr.append(instrument);
  execstr.append(".ini");
  execstr.append("| gawk 'BEGIN {FS=\"=\"} {print $2}'");
  my_fscanf(execstr, numchips);
  
  //  initialise the dialog
  
  // set the directory line edit and repaint its background
  animate_dirLineEdit->setText(sciencedir);
  
  coadd_directory.setPath(coadddir);
  animate_directory.setPath(colordir);
  if (!animate_directory.exists()) animate_directory.mkdir(colordir);
  
  direxists = 0;
  if (coadd_directory.exists() && sciencedir.length() > 0 ) {
    direxists = 1;
    animate_dirLineEdit->setPaletteBackgroundColor(cfgreen);
  }
  if (!coadd_directory.exists() && sciencedir.length() > 0 ) {
    direxists = 0;
    animate_dirLineEdit->setPaletteBackgroundColor(cfred);
  }
  if (sciencedir.length() == 0 ) {
    direxists = 0;
    animate_dirLineEdit->setPaletteBackgroundColor(cwhite);
  }
  
  get_coadddirs();
  update_chipnum();
  setstate_manualthresholds();
  
  fill_in_moviedivisions();
  update_animatebuttontext();
  animate_contrastSpinBox->setValue(0);
  globalmask = "";
  
  started = 1;
}


void animate::kill_process()
{
  QString execstr;
  long process_id;
  
  if (proc_col_movie_started == 1 && proc_col_movie->isRunning()) {
    proc_col_movie_started = 0;
    process_id = proc_col_movie->processIdentifier();
    animate_pid = animate_pid.setNum(process_id);
    execstr = scriptdir;
    execstr.append("/parakill.sh ");
    execstr.append(animate_pid);
    execstr.append(" > /dev/null 2>&1");
    my_system(execstr);
  }
}

void animate::quit()
{
  // only close the dialog if no processes are running
  if (proc_col_movie_started == 0) 
    animate::close();
  else {
    int answer = QMessageBox::warning(this, "Process still running",
				  QString( "A process is still running!\n If you Quit, it will be aborted.\n\n" ),
				  "&Cancel", "&Quit", QString::null, 0, 1 );

    if (answer == 0) return;
    else {
      kill_process();
      animate::close();
    }
  }
}
