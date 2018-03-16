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
#include <qfile.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include "functions.h"

int direxists, getstat, numberofchips, started;
int proc_col_combine_started;
int proc_col_colcalib_started, proc_col_elum_started;
int proc_col_stiff_started, proc_col_fits2tiff_started;
int proc_col_crop_started;
bool colcalib_success = false;
QString colordir, coadddir, color_scriptdir, color_homedir, color_bindir;
QString rfac, vfac, bfac, stiff_mode, luminancetiff;
QString r_image, v_image, b_image, l_image, instrument;
QDir color_directory, coadd_directory;
QProcess *proc_col_combine, *proc_col_colcalib;
QProcess *proc_col_stiff, *proc_col_fits2tiff, *proc_col_elum;
QProcess *proc_col_crop;
QStringList current_imagelist;

typedef struct {
  QString catname;
  QString nstars;
  QString bfac;
  QString gfac;
  QString rfac;
  QString bfacerr;
  QString gfacerr;
  QString rfacerr;
} _photcatresult_;

_photcatresult_ photcatresult[4];

QString bestcat;  // the catalog yielding most phot reference sources
int bestindex = 0;

void color_form::select_dir()
{
  QFileDialog* fd = new QFileDialog( this, "Select directory", TRUE );
  fd->setMode( QFileDialog::DirectoryOnly );
  fd->setDir(color_dirLineEdit->text());
  fd->show();

  if ( fd->exec() == QDialog::Accepted ) {
    QString selected = fd->selectedFile();
    color_dirLineEdit->setText(selected);
  }
}


void color_form::get_coadddirs()
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
    
    color_coaddListBox->clear();
    color_coaddListBox->insertStringList(dirlist);
    color_coaddListBox->setCurrentItem(0);
  }
  else {
    color_coaddListBox->clear();
    color_imagesListBox->clear();
  }
}


void color_form::link_coaddimages()
{
  QStringList coaddimages;
  QString dirname, execstr, coaddimagename, coaddweightname, ident;
  QString date, colordirpath;
  int i, numimages, nimages_selected = 0;
  
  // a sanity check
  if (!color_directory.exists()) color_directory.mkdir(colordir);
  // if 'color_theli' already exists, then rename it
  else {
    my_fscanf("date +%FT%T", date);
    date.replace(":","-");
    colordirpath = color_directory.absPath();
    execstr = "mv "+colordirpath+" "+colordirpath+"_backup_"+date;
    my_system(execstr);
    color_directory.mkdir(colordir);
  }
  
  // wipe some fields and tables
  color_imagesListBox->clear();
  color_ELum_imagesListBox->clear();
  color_BV_r_ComboBox->clear();
  color_BV_v_ComboBox->clear();
  color_BV_b_ComboBox->clear();
  
  // get a list of selected images
  numimages = color_coaddListBox->numRows();
  for (i=0; i<numimages; i++) {
    if (color_coaddListBox->isSelected(i)) nimages_selected++;
  }
  
  // if no coadd directories are selected, then select all directories
  if (nimages_selected == 0) color_coaddListBox->selectAll(true);
  
  for (i=0; i<numimages; i++) {
    if (color_coaddListBox->isSelected(i)) {
      dirname = color_coaddListBox->text(i);
      ident   = color_coaddListBox->text(i);
      // remove the leading "coadd_" substring
      ident.remove(0,6);
      execstr = "ln -sf "+coadddir+"/"+dirname+"/coadd.fits "+coadddir+
	        "/color_theli/"+ident+".fits";
      my_system(execstr);
      execstr = "ln -sf "+coadddir+"/"+dirname+"/coadd.weight.fits "+coadddir+
	"/color_theli/"+ident+".weight.fits";
      my_system(execstr);
    }
  }
  
  // now fill the image list
  get_colorchannels();
  
  // the sky statistics line edits can be erased
  color_xminLineEdit->clear();
  color_xmaxLineEdit->clear();
  color_yminLineEdit->clear();
  color_ymaxLineEdit->clear();
  
  // crop the images to the same size
  get_maxoverlap();
}


void color_form::get_colorchannels()
{
  QStringList allimages, images;
  QString tmp, xmin, xmax, ymin, ymax, line, sky, rms;
  int i, numimages;
  
  if (direxists == 1) {
    // populate the image list
    color_directory.setNameFilter("*.fits;*.fit;*.fts;*.FITS;*.FIT;*.FTS");
    allimages = color_directory.entryList();
    i = 0;
    for ( QStringList::Iterator it = allimages.begin(); it != allimages.end(); ++it ) {
      tmp = *it;
      if (tmp.find("weight.fits") == -1 && tmp.find("_2tiff.fits") == -1) 
	images.append(tmp);
    }
    color_imagesListBox->clear();
    color_imagesListBox->insertStringList(images);
    color_imagesListBox->setCurrentItem(0);
    // populate the enhanced luminance list box
    color_ELum_imagesListBox->clear();
    color_ELum_imagesListBox->insertStringList(images);
    color_ELum_imagesListBox->setCurrentItem(0);
    // populate the skyresidual TextEdit
    numimages = color_imagesListBox->count();
    // shall we clear the table too?
    color_statisticsTable->setNumRows(numimages);
    // populate the combine ComboBoxes
    int flag = 0;
    if (color_bbnb_broadComboBox->count() != 0) flag = 1;
    if (flag == 0) {
      color_bbnb_broadComboBox->clear();
      color_bbnb_narrowComboBox->clear();
    }
    color_bbnb_broadComboBox->insertStringList(images);
    color_bbnb_narrowComboBox->insertStringList(images);
    if (flag == 0) {
      color_bbnb_broadComboBox->setCurrentItem(0);
      color_bbnb_narrowComboBox->setCurrentItem(0);
    }
    // populate the colour ComboBoxes
    color_BV_r_ComboBox->clear();
    color_BV_v_ComboBox->clear();
    color_BV_b_ComboBox->clear();
    color_BV_r_ComboBox->insertStringList(images);
    color_BV_v_ComboBox->insertStringList(images);
    color_BV_b_ComboBox->insertStringList(images);
    color_BV_r_ComboBox->setCurrentItem(0);
    color_BV_v_ComboBox->setCurrentItem(0);
    color_BV_b_ComboBox->setCurrentItem(0);
    // populate the STIFF images ComboBox
    color_stiffRGBLUMComboBox->clear();
    populate_stiffcombobox();
    /*
      color_stiffRGBLUMComboBox->insertItem("Make RGB image", 0);
      QStringList images2 = images;
      for ( QStringList::Iterator it2 = images2.begin(); 
      it2 != images2.end(); ++it2 ) (*it2).prepend("LUM: ");
      color_stiffRGBLUMComboBox->insertStringList(images2);
      color_stiffRGBLUMComboBox->setCurrentItem(0);
    */
    
    // if some statistics has been obtained previously, read the info
    i = 0;
    QFile file( colordir+"/stats.dat" );
    if ( file.open( IO_ReadOnly )) {
      flush_table();
      QTextStream stream( &file );
      while ( !stream.atEnd() ) {
	// read the first line
	line = stream.readLine();
	if (line.contains('#') == 1) {
	  xmin = line.section(" ", 1, 1);
	  xmax = line.section(" ", 2, 2);
	  ymin = line.section(" ", 3, 3);
	  ymax = line.section(" ", 4, 4);
	  color_xminLineEdit->setText(xmin);
	  color_xmaxLineEdit->setText(xmax);
	  color_yminLineEdit->setText(ymin);
	  color_ymaxLineEdit->setText(ymax);
	}
	if (line.contains('#') == 0) {
	  sky = line.section(" ", 0, 0);
	  rms = line.section(" ", 1, 1);
	  // sky.truncate(7);
	  // rms.truncate(7);
	  if (i<numimages) {
	    color_statisticsTable->setText(i,1,sky);
	    color_statisticsTable->setText(i,4,rms);
	  }
	  i++;
	  getstat = 1;
	}
      }
      file.close();
    }
  }
  else {
    color_imagesListBox->clear();
    color_BV_r_ComboBox->clear();
    color_BV_v_ComboBox->clear();
    color_BV_b_ComboBox->clear();
    color_bbnb_broadComboBox->clear();
    color_bbnb_narrowComboBox->clear();
    color_xminLineEdit->clear();
    color_xmaxLineEdit->clear();
    color_yminLineEdit->clear();
    color_ymaxLineEdit->clear();
    getstat = 0;
  }
  
  // fill in the rest of the table if statistics has been obtained previously
  if (getstat == 1) {
    update_tiffthresholds(numimages);
    for (i=0; i < numimages; i++) {
      color_statisticsTable->setText(i,0,color_imagesListBox->text(i));
    }
  }
}


void color_form::validator_main()
{
  QColor cfred = QColor(255,187,155);
  QColor cfgreen = QColor(155,255,187);
  QColor cwhite = QColor(255,255,255);
  
  // directories must not accept blank characters
  QRegExp rx( "^\\S+$" );
  QValidator* validator_string = new QRegExpValidator( rx, this );
  color_dirLineEdit->setValidator( validator_string );
  
  // repaint the background colour
  coadddir = color_dirLineEdit->text();
  coadd_directory.setPath(coadddir);
  direxists = 0;
  if (coadd_directory.exists() && coadddir.length() > 0 ) {
    direxists = 1;
    color_dirLineEdit->setPaletteBackgroundColor(cfgreen);
  }
  if (!coadd_directory.exists() && coadddir.length() > 0 ) {
    direxists = 0;
    color_dirLineEdit->setPaletteBackgroundColor(cfred);
  }
  if (coadddir.length() == 0 ) {
    direxists = 0;
    color_dirLineEdit->setPaletteBackgroundColor(cwhite);
  }
  
  //    get_colorchannels();
  get_coadddirs();
}


void color_form::validator_sec()
{
  // make that numeric fields only accept numeric input
  QRegExp ri( "^[0-9]+" );
  QValidator* validator_int = new QRegExpValidator( ri, this );
  color_xminLineEdit->setValidator( validator_int );
  color_xmaxLineEdit->setValidator( validator_int );
  color_yminLineEdit->setValidator( validator_int );
  color_ymaxLineEdit->setValidator( validator_int );
  
  // thresholds and color tolerance accept floats
  QRegExp rf( "^[-]{0,1}[0-9]+[.]{0,1}[0-9]+" );
  QValidator* validator_float = new QRegExpValidator( rf, this );
  color_factorRLineEdit->setValidator( validator_float );
  color_factorGLineEdit->setValidator( validator_float );
  color_factorBLineEdit->setValidator( validator_float );
  color_stiffmaxvalLineEdit->setValidator( validator_float );
  color_stiffminvalLineEdit->setValidator( validator_float );
  color_stiffsatvalLineEdit->setValidator( validator_float );
  color_stiffgammaLineEdit->setValidator( validator_float );
  color_bbnb_broadwidthLineEdit->setValidator( validator_float );
  color_bbnb_narrowwidthLineEdit->setValidator( validator_float );
  color_bbnb_weightLineEdit->setValidator( validator_float );
}


void color_form::remove_imagefromlist()
{
  int i, numimages;
  QStringList unimage;
  QString unwantedimage, execstr;
  
  // delete highlighted images backwards to avoid problems with 
  // the indexing of a shrinking list
  numimages = color_imagesListBox->count();
  for (i=numimages-1; i>=0; i--) {
    if (color_imagesListBox->isSelected(i)) {
      color_imagesListBox->removeItem(i);
      
      // remove the entry from the other widgets as well
      color_BV_r_ComboBox->removeItem(i);
      color_BV_v_ComboBox->removeItem(i);
      color_BV_b_ComboBox->removeItem(i);
      color_bbnb_broadComboBox->removeItem(i);
      color_bbnb_narrowComboBox->removeItem(i);
      color_ELum_imagesListBox->removeItem(i);
      // if statistics has been done, remove the corresponding line
      if (getstat == 1) color_statisticsTable->removeRow(i);
    }
  }
  
  color_imagesListBox->setCurrentItem(0);
  color_imagesListBox->setSelected(0,true);
  // update the tiff thresholds
  numimages = color_imagesListBox->count();
  update_tiffthresholds(numimages);
  
  // clean up the remaining fields if all images are removed
  if (numimages == 0) {
    color_xminLineEdit->clear();
    color_xmaxLineEdit->clear();
    color_yminLineEdit->clear();
    color_ymaxLineEdit->clear();
  }
}


void color_form::flush_table()
{
  int numimages = color_imagesListBox->count();
  color_statisticsTable->setNumCols(5);
  color_statisticsTable->setNumRows(0);
  color_statisticsTable->setNumRows(numimages);
}


void color_form::get_statistics()
{
  FILE *of;
  int i, numimages;
  float low, lowthresh, high;
  QString xmin, xmax, ymin, ymax, line;
  QString filename, tmp, execstr;
  QString lowstr, highstr, rms, sky;
  
  QDir dir = QDir(colordir);
  if (!dir.exists()) return;
  
  filename = color_homedir;
  if (!filename.endsWith("/")) filename.append("/");    
  filename.append(".theli/scripts/theli_color_getstatistics.sh");
  
  xmin = color_xminLineEdit->text();
  xmax = color_xmaxLineEdit->text();
  ymin = color_yminLineEdit->text();
  ymax = color_ymaxLineEdit->text();
  
  // swap values if accidentally inverted:
  if (xmax.toInt() < xmin.toInt()) {
    xmin = color_xmaxLineEdit->text();
    xmax = color_xminLineEdit->text();
  }
  if (ymax.toInt() < ymin.toInt()) {
    ymin = color_ymaxLineEdit->text();
    ymax = color_yminLineEdit->text();
  }

  // if one of those fields is empty, do nothing.
  if (xmin.isEmpty() || xmax.isEmpty() || 
      ymin.isEmpty() || ymax.isEmpty()) {
    QMessageBox::warning( this, "No statistics region defined",
			  "You must first select an area of empty sky and \n"
			  "define it in the \"Coadditions\" section!\n");
    return;
  }
  
  // write the script
  
  // write everything into the file
  of = fopen(filename, "w");
  
  fprintf(of, "%s%s\n\n", "#!",bash.ascii());
  fprintf(of, "# ******************************************\n");
  fprintf(of, "# ***** Create color image: statistics *****\n");
  fprintf(of, "# *****        created by THELI        *****\n");
  fprintf(of, "# ******************************************\n\n");
  fprintf(of, "dir=`pwd`\n\n");
  fprintf(of, "cd %s\n\n",color_scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  
  fprintf(of, "cd %s\n\n",colordir.ascii());
  fprintf(of, "imagelist=\"\"\n\n");
  
  // run fitscut to get the small image sections
  i = 0;
  numimages = color_imagesListBox->count();
  for (i=0; i<numimages; i++) {
    fprintf(of, "imagelist=\"${imagelist} %s\"\n", color_imagesListBox->text(i).ascii());
  }
  fprintf(of, "echo \"#\" %s %s %s %s > stats.dat\n", xmin.ascii(), xmax.ascii(), ymin.ascii(), ymax.ascii());
  fprintf(of, "${P_IMSTATS} -d 6 ${imagelist} -s %s %s %s %s |\\\n", 
	  xmin.ascii(), xmax.ascii(), ymin.ascii(), ymax.ascii());
  fprintf(of, "      ${P_GAWK} '($0 !~ /#/) {print $2, $7}' >> stats.dat\n\n");
  fprintf(of, "cd ${dir}\n");
  fclose(of);
  
  // flush the table
  flush_table();
  
  // make the file executable
  execstr = "chmod +x ";
  execstr.append(filename);
  my_system(execstr);
  
  // execute the script
  my_system(filename);
  
  // obtain and display the results
  i = 0;
  QFile file( colordir+"/stats.dat" );
  if ( file.open( IO_ReadOnly )) {
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      if (line.contains('#') == 0) {
	sky = line.section(" ", 0, 0);
	rms = line.section(" ", 1, 1);
	//		sky.truncate(7);
	//		rms.truncate(7);
	if (i<numimages) {
	  color_statisticsTable->setText(i,1,sky);
	  color_statisticsTable->setText(i,4,rms);
	}
	i++;
      }
    }
    file.close();
  }
  
  // get the lower and upper TIFF thresholds
  i = 0;
  low = color_statisticsTable->text(0,4).toFloat();
  for (i=0; i<numimages; i++) {
    lowthresh = color_statisticsTable->text(i,4).toFloat();
    if (lowthresh >= low) low = lowthresh;
  }
  low = low * 6.0;
  if (low > 0) low = low * (-1.);
  high = fabs(low) * 500.;
  
  lowstr = lowstr.setNum(low);
  highstr = highstr.setNum(high);
  lowstr.truncate(8);
  highstr.truncate(6);
  
  
  // fill the table with the thresholds
  for (i=0; i < numimages; i++) {
    color_statisticsTable->setText(i,2,lowstr);
    color_statisticsTable->setText(i,3,highstr);
  }
  
  // fill the names into the table
  for (i=0; i < numimages; i++) {
    color_statisticsTable->setText(i,0,color_imagesListBox->text(i));
  }
  
  // the statistics has been obtained
  getstat = 1;
}


void color_form::update_tiffthresholds(int numimages)
{
  int i;
  float low, lowthresh, high;
  QString lowstr, highstr;
  
  
  // get the lower and upper TIFF thresholds
  i = 0;
  low = color_statisticsTable->text(0,4).toFloat();
  for (i=0; i<numimages; i++) {
    lowthresh = color_statisticsTable->text(i,4).toFloat();
    if (lowthresh >= low) low = lowthresh;
  }
  low = low * 2.2;
  if (low > 0) low = low * (-1.);
  high = fabs(low) * 500.;
  
  lowstr = lowstr.setNum(low);
  highstr = highstr.setNum(high);
  lowstr.truncate(8);
  highstr.truncate(6);
  
  // fill the table with the thresholds
  for (i=0; i < numimages; i++) {
    color_statisticsTable->setText(i,2,lowstr);
    color_statisticsTable->setText(i,3,highstr);
  }
}


void color_form::stiff_defaults()
{
  color_stiffminvalLineEdit->setText("-0.1");
  color_stiffmaxvalLineEdit->setText("2");
  color_stiffsatvalLineEdit->setText("1.0");
  color_stiffgammaLineEdit->setText("1.0");
}


void color_form::enhance_luminance()
{
  QStringList images;
  QString execstr, filelist, command;
  QString xmin, xmax, ymin, ymax, mode, mask, line;
  QString naxis1, naxis2, naxis1old, naxis2old;
  int i, numimages, nimages_selected = 0, check;
  
  
  // get a list of selected images
  numimages = color_ELum_imagesListBox->numRows();
  
  if (numimages == 0) return;
  
  for (i=0; i<numimages; i++) {
    if (color_ELum_imagesListBox->isSelected(i)) {
      images.append(color_ELum_imagesListBox->text(i));
      nimages_selected++;
    }
  }
  
  // if no image is selected, select all images
  if (nimages_selected == 0) {
    color_ELum_imagesListBox->selectAll(true);
    for (i=0; i<numimages; i++) {
      if (color_ELum_imagesListBox->isSelected(i)) {
	images.append(color_ELum_imagesListBox->text(i));
	nimages_selected++;
      }
    }
  }
  
  // dump a file with the image selected
  QFile file( colordir+"/chisquarelist" );
  if ( file.open( IO_WriteOnly )) {
    QTextStream stream( &file );
    for ( QStringList::Iterator it = images.begin(); it != images.end(); ++it ) {
      stream << *it << "\n";
    }
    file.close();
  }
  
  // get some parameters
  xmin = color_xminLineEdit->text();
  xmax = color_xmaxLineEdit->text();
  ymin = color_yminLineEdit->text();
  ymax = color_ymaxLineEdit->text();
  mask = "nomask";
  mode = "chisquare";
  if (color_ELum_maskCheckBox->isChecked()) mask = "mask";
  if (color_ELum_modeComboBox->currentItem() == 1) mode = "mean";
  
  // print a warning if no sky region is selected and chi-square combination
  // should be done
  if (color_ELum_modeComboBox->currentItem() == 0 && 
      (xmin.isEmpty() || xmax.isEmpty() || ymin.isEmpty() || ymax.isEmpty())) {
    QMessageBox::warning( this, "No statistics region defined",
			  "For a chi-square image, you must first select an area of\n"
			  "empty sky and define it in the \"Coadditions\" section!\n");
    return;
  } 
  
  // check that images have the same file size, otherwise return
  filelist = "";
  for (i=0; i<numimages; i++) {
    filelist.append(colordir+"/"+color_imagesListBox->text(i)+" ");
  }
  execstr = bindir + "/dfits "+filelist+" | " + bindir + "/fitsort NAXIS1 NAXIS2 | gawk '(NR>1) {print $2, $3}' > ";
  execstr.append(colordir+"/filesizes 2>&1");
  my_system(execstr);
  
  i = 0;
  check = 0;
  file.setName( colordir+"/filesizes" );
  if ( file.open( IO_ReadOnly )) {
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      naxis1 = line.section(" ", 0, 0);
      naxis2 = line.section(" ", 1, 1);
      if (i == 0) {
	naxis1old = naxis1;
	naxis2old = naxis2;
      }
      if (naxis1.compare(naxis1old) != 0 || naxis2.compare(naxis2old) !=0 ) check = 1;
      i++;
    }
    file.close();
    file.remove();
  }
  
  if (check != 0) {
    QMessageBox::information( this, "Incompatible image sizes",
		     "The images do not have the same size. Run them\n"
	             "through the \"Crop maximum overlap\" step in the\n"
		     "\"Coadditions\" section first!.\n");
    return;
  }
  
  // launch 
  proc_col_elum_started = 0;
  command = "./create_chisquare_image.sh " + 
    color_scriptdir + " " + colordir + " " + xmin + " " + xmax + " " + 
    ymin + " " + ymax + " " + mode + " " + mask;
  
  write_superscriptbody(homedir+".theli/scripts/theli_create_chisquare_image.sh", 
			command, "elum");
}


void color_form::fill_in_elum()
{
  if (color_ELum_modeComboBox->currentItem() == 1)
    color_imagesListBox->insertItem("elum_mean.fits");
  else color_imagesListBox->insertItem("elum_chisquare.fits");
  populate_stiffcombobox();
}


void color_form::get_current_imagelist()
{
  QStringList allimages;
  QString tmp;
  
  // get the images
  if (direxists == 1) {
    current_imagelist.clear();
    color_directory.setNameFilter("*.fits;*.fit;*.fts;*.FITS;*.FIT;*.FTS");
    allimages = color_directory.entryList();
    for ( QStringList::Iterator it = allimages.begin(); it != allimages.end(); ++it ) {
      tmp = *it;
      if (tmp.find("weight.fits") == -1 && 
	  tmp.find("_2tiff.fits") == -1) 
	current_imagelist.append(tmp);
    }
  }
}


void color_form::populate_stiffcombobox()
{
  get_current_imagelist();
  color_stiffRGBLUMComboBox->clear();
  color_stiffRGBLUMComboBox->insertItem("Make RGB image", 0);
  QStringList images2 = current_imagelist;
  for ( QStringList::Iterator it2 = images2.begin(); 
	it2 != images2.end(); ++it2 ) (*it2).prepend("LUM: ");
  color_stiffRGBLUMComboBox->insertStringList(images2);
  color_stiffRGBLUMComboBox->setCurrentItem(0);
}


void color_form::write_superscriptbody(QString superscript, QString command, QString type)
{
  FILE *of;
  QString logfile, execstr, tmp, buttontext;
  
  // leave if the process is already running!
  
  if (type.compare("combine") == 0 && proc_col_combine_started == 1) 
    return;
  if (type.compare("colcalib") == 0 && proc_col_colcalib_started == 1) 
    return;
  if (type.compare("stiff") == 0 && proc_col_stiff_started == 1)
    return;
  if (type.compare("fits2tiff") == 0 && proc_col_fits2tiff_started == 1)
    return;
  if (type.compare("elum") == 0 && proc_col_elum_started == 1)
    return;
  if (type.compare("crop") == 0 && proc_col_crop_started == 1) 
    return;
  
  logfile = superscript+".log";
  
  // write the superscript
  of = fopen(superscript, "w");
  fprintf(of, "#!%s\n\n", bash.ascii());
  fprintf(of, "# ******************************************\n");
  fprintf(of, "# *****  Created on the fly by THELI   *****\n");
  fprintf(of, "# ******************************************\n\n");
  fprintf(of, "export INSTRUMENT=%s\n", instrument.ascii());
  if (type.compare("stiff") != 0 && 
      type.compare("fits2tiff") != 0 &&
      type.compare("crop") != 0)
    fprintf(of, "cd %s\n\n", color_scriptdir.ascii());
  else
    fprintf(of, "cd %s%s\n\n", homedir.ascii(), ".theli/scripts/");      
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
  if (type.compare("combine") == 0) {
    proc_col_combine = new QProcess( this );
    proc_col_combine->addArgument( superscript );
    connect(proc_col_combine, SIGNAL(readyReadStdout()), this, SLOT(update_combineText()));
    connect(proc_col_combine, SIGNAL(processExited()), this, SLOT(update_combine_elements()));
    if (proc_col_combine_started == 0) {
      while( !proc_col_combine->isRunning() ) proc_col_combine->launch("");
      proc_col_combine_started = 1;
      color_bbnb_combinePushButton->setText("Combining ...");
      color_bbnb_combinePushButton->setPaletteForegroundColor(QColor(0,155,0));
    }
  }
  
  if (type.compare("colcalib") == 0) {
    proc_col_colcalib = new QProcess( this );
    proc_col_colcalib->addArgument( superscript );
    connect(proc_col_colcalib, SIGNAL(readyReadStdout()), this, SLOT(update_colcalibText()));
    connect(proc_col_colcalib, SIGNAL(processExited()), this, SLOT(fill_in_colcalib()));
    if (proc_col_colcalib_started == 0) {
      while( !proc_col_colcalib->isRunning() ) proc_col_colcalib->launch("");
      proc_col_colcalib_started = 1;
      color_getBVcalPushButton->setText("Calibrating ...");
      color_getBVcalPushButton->setPaletteForegroundColor(QColor(0,155,0));
    }
  }
  
  if (type.compare("stiff") == 0) {
    proc_col_stiff = new QProcess( this );
    proc_col_stiff->addArgument( superscript );
    connect(proc_col_stiff, SIGNAL(readyReadStdout()), this, SLOT(update_stiffText()));
    connect(proc_col_stiff, SIGNAL(processExited()), this, SLOT(show_preview()));
    if (proc_col_stiff_started == 0) {
      if (color_stiffRGBLUMComboBox->currentItem() == 0) {
	r_image = color_BV_r_ComboBox->currentText();
	v_image = color_BV_v_ComboBox->currentText();
	b_image = color_BV_b_ComboBox->currentText();
	if (strcmp(r_image,v_image) == 0 &&
	    strcmp(r_image,b_image) == 0) {
	  int answer = QMessageBox::warning(
		     this, "Identical colour channels",
		     QString( "The images selected for R, G and B in the colour\n"
			      "calibration menu are identical. This will result in\n"
			      "a black&white image. Continue?\n\n" ),
		     "Yes", "No", 0, 2);
	  if (answer != 0) return;
	}
      }
      
      while( !proc_col_stiff->isRunning() ) proc_col_stiff->launch("");
      proc_col_stiff_started = 1;
      
      buttontext = "Running ...";
      color_stiffPushButton->setText(buttontext);
      color_stiffPushButton->setPaletteForegroundColor(QColor(0,155,0));
      color_stiffpreviewPushButton->setText(buttontext);
      color_stiffpreviewPushButton->setPaletteForegroundColor(QColor(0,155,0));
    }
  }
  
  if (type.compare("fits2tiff") == 0) {
    proc_col_fits2tiff = new QProcess( this );
    proc_col_fits2tiff->addArgument( superscript );
    connect(proc_col_fits2tiff, SIGNAL(readyReadStdout()), this, SLOT(update_fits2tiffText()));
    if (proc_col_fits2tiff_started == 0) {
      while( !proc_col_fits2tiff->isRunning() ) proc_col_fits2tiff->launch("");
      proc_col_fits2tiff_started = 1;
      color_fits2tiffPushButton->setText("Converting ...");
      color_fits2tiffPushButton->setPaletteForegroundColor(QColor(0,155,0));
    }
  }
  
  if (type.compare("elum") == 0) {
    proc_col_elum = new QProcess( this );
    proc_col_elum->addArgument( superscript );
    connect(proc_col_elum, SIGNAL(readyReadStdout()), this, SLOT(update_elumText()));
    connect(proc_col_elum, SIGNAL(processExited()), this, SLOT(fill_in_elum()));
    if (proc_col_elum_started == 0) {
      while( !proc_col_elum->isRunning() ) proc_col_elum->launch("");
      proc_col_elum_started = 1;
      color_ELum_doitPushButton->setText("Creating ELum ...");
      color_ELum_doitPushButton->setPaletteForegroundColor(QColor(0,155,0));
    }
  }
  
  if (type.compare("crop") == 0) {
    proc_col_crop = new QProcess( this );
    proc_col_crop->addArgument( superscript );
    connect(proc_col_crop, SIGNAL(readyReadStdout()), this, SLOT(update_cropText()));
    connect(proc_col_crop, SIGNAL(processExited()), this, SLOT(get_colorchannels()));
    if (proc_col_crop_started == 0) {
      while( !proc_col_crop->isRunning() ) proc_col_crop->launch("");
      proc_col_crop_started = 1;
      color_getcoaddedimagesPushButton->setText("Please wait ...");
      color_getcoaddedimagesPushButton->setPaletteForegroundColor(QColor(0,155,0));
    }
  }
}


void color_form::update_cropText()
{
  QString tmp, tmp2;
  QByteArray data = proc_col_crop->readStdout();
  tmp = QString(data);
  if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
    tmp2 = tmp.right( tmp.length() - 14);
    if (tmp2.compare("finished\n") == 0) {
      color_getcoaddedimagesPushButton->setPaletteForegroundColor(QColor(0,0,0));
      color_getcoaddedimagesPushButton->setText("Get coadd images");
    }
  }
}


void color_form::update_combineText()
{
  QString tmp, tmp2;
  QByteArray data = proc_col_combine->readStdout();
  tmp = QString(data);
  if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
    tmp2 = tmp.right( tmp.length() - 14);
    if (tmp2.compare("finished\n") == 0) {
      color_bbnb_combinePushButton->setPaletteForegroundColor(QColor(0,0,0));
      color_bbnb_combinePushButton->setText("Combine");
    }
  }
}


void color_form::update_colcalibText()
{
  QString tmp, tmp2;
  QByteArray data = proc_col_colcalib->readStdout();
  tmp = QString(data);
  if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
    tmp2 = tmp.right( tmp.length() - 14);
    if (tmp2.compare("finished\n") == 0) {
      color_getBVcalPushButton->setPaletteForegroundColor(QColor(0,0,0));
      color_getBVcalPushButton->setText("Calibrate");
    }
  }
}


void color_form::update_stiffText()
{
  QString tmp, tmp2;
  QByteArray data = proc_col_stiff->readStdout();
  tmp = QString(data);
  if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
    tmp2 = tmp.right( tmp.length() - 14);
    if (tmp2.compare("finished\n") == 0) {
      color_stiffPushButton->setPaletteForegroundColor(QColor(0,0,0));
      color_stiffPushButton->setText("Convert FITS to TIFF");
      color_stiffpreviewPushButton->setPaletteForegroundColor(QColor(0,0,0));
      color_stiffpreviewPushButton->setText("Preview");
    }
  }
}


void color_form::update_fits2tiffText()
{
  QString tmp, tmp2;
  QByteArray data = proc_col_fits2tiff->readStdout();
  tmp = QString(data);
  if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
    tmp2 = tmp.right( tmp.length() - 14);
    if (tmp2.compare("finished\n") == 0) {
      color_fits2tiffPushButton->setPaletteForegroundColor(QColor(0,0,0));
      color_fits2tiffPushButton->setText("Convert FITS to TIFF");
    }
  }
}


void color_form::update_elumText()
{
  QString tmp, tmp2;
  QByteArray data = proc_col_elum->readStdout();
  tmp = QString(data);
  if (tmp.left(13).compare("THELIMESSAGE:") == 0) {
    tmp2 = tmp.right( tmp.length() - 14);
    if (tmp2.compare("finished\n") == 0) {
      color_ELum_doitPushButton->setPaletteForegroundColor(QColor(0,0,0));
      color_ELum_doitPushButton->setText("Create ELum");
    }
  }
}


void color_form::update_combine_elements()
{
  QString image_broad, image_narrow, width_broad, width_narrow, weight;
  QString filt1, filt2, newname;

  image_broad  = color_bbnb_broadComboBox->currentText();
  image_narrow = color_bbnb_narrowComboBox->currentText();
  width_broad  = color_bbnb_broadwidthLineEdit->text();
  width_narrow = color_bbnb_narrowwidthLineEdit->text();

  // reconstruct the name of the combined image
  filt1 = image_broad.remove("_cropped.fits");
  filt2 = image_narrow.remove("_cropped.fits");
  newname = filt1+"+"+filt2+"_cropped.fits";

  // Enter the new name in all relevant fields
  get_colorchannels();
}


void color_form::combine_images()
{
  QString execstr;
  QString image_broad, image_narrow, width_broad, width_narrow, weight;

  image_broad  = color_bbnb_broadComboBox->currentText();
  image_narrow = color_bbnb_narrowComboBox->currentText();
  width_broad  = color_bbnb_broadwidthLineEdit->text();
  width_narrow = color_bbnb_narrowwidthLineEdit->text();
  weight       = color_bbnb_weightLineEdit->text();

  if (weight.isEmpty()) weight = "1.0";

  // leave conditions
  if (image_broad.compare(image_narrow) == 0) {
    QMessageBox::warning( this, "Identical images chosen",
			  "You must choose different images to combine.\n");
    return;
  }

  // launch the convolution script
  proc_col_combine_started = 0;
  write_superscriptbody(homedir+".theli/scripts/theli_color_combine.sh", 
			"./color_combine.sh "+colordir+" "+image_broad+
			" "+image_narrow+" "+width_broad+" "+width_narrow+" "+weight, "combine");
}


void color_form::show_preview()
{
  QString result, stiff_filename;
  
  if (stiff_mode.compare("rgb") == 0)
    stiff_filename = colordir+"/rgb_stiff.tif";
  else
    stiff_filename = colordir+"/"+luminancetiff;
  
  QFile preview( stiff_filename );
  
  // check if the display programme has been installed
  my_system("which display > "+colordir+"log 2>&1");
  QFile displaycheck( colordir+"log");
  if ( displaycheck.open( IO_ReadOnly )) {
    QTextStream stream( &displaycheck );
    while ( !stream.atEnd() ) {
      result = stream.readLine();
      if (result.contains("not found", FALSE) > 0 ||
	  result.contains("which: no ", FALSE) || 
	  result.isEmpty() || result.length() < 4) {
	QMessageBox::warning( this, "Command not found",
			      "The 'display' programme does not exist on your machine.\n"
			      "Please install the ImageMagick programme suite.\n");
	return;
      }
    }
    displaycheck.close();
    displaycheck.remove();
  }
  
  my_system("display "+stiff_filename+" &");
}


void color_form::reset_BVcorr()
{
  color_factorRLineEdit->setText("1.000");
  color_factorGLineEdit->setText("1.000");
  color_factorBLineEdit->setText("1.000");
  color_errorRLineEdit->setText("0.000");
  color_errorGLineEdit->setText("0.000");
  color_errorBLineEdit->setText("0.000");

  color_SDSSPushButton->setOn(false);
  color_UCAC4PushButton->setOn(false);
  color_NOMADPushButton->setOn(false);
  color_AVGWHITEPushButton->setOn(false);

  color_nstarsSDSSTextLabel  -> setText("0 stars");
  color_nstarsUCAC4TextLabel -> setText("0 stars");
  color_nstarsNOMADTextLabel -> setText("0 stars");
  color_nstarsAVGWHITETextLabel -> setText("0 stars");

  int i;
  for (i=0; i<4; i++) {
    photcatresult[i].bfac    = "1.000";
    photcatresult[i].bfacerr = "0.000";
    photcatresult[i].gfac    = "1.000";
    photcatresult[i].gfacerr = "0.000";
    photcatresult[i].rfac    = "1.000";
    photcatresult[i].rfacerr = "0.000";
  }


}


void color_form::stiff()
{
  FILE *of;
  int i, numimages;
  float tmpfloat;
  
  QString filename, execstr, file, sky, outfilename;
  QString tmp, bfac2, rfac2;
  QString stiff_minval, stiff_maxval, stiff_satval, stiff_gamma;
  QString r_imagetiff, v_imagetiff, b_imagetiff, l_imagetiff;
  QStringList lst;
  
  filename = color_homedir;
  if (!filename.endsWith("/")) filename.append("/");    
  filename.append(".theli/scripts/gettiffpreview.sh");
  
  if (color_stiffRGBLUMComboBox->currentItem() == 0) {
    stiff_mode = "rgb";
    outfilename = "rgb_stiff.tif";
  } 
  else {
    stiff_mode = "lum";
    l_image = color_stiffRGBLUMComboBox->currentText();
    l_image.remove(0,5);
    outfilename = l_image;
    outfilename.replace(".fits","_stiff.tif");
    luminancetiff = outfilename;
  }
  
  r_image = color_BV_r_ComboBox->currentText();
  v_image = color_BV_v_ComboBox->currentText();
  b_image = color_BV_b_ComboBox->currentText();
  
  // write the script
  of = fopen(filename, "w");
  
  fprintf(of, "%s%s\n\n", "#!",bash.ascii());
  fprintf(of, "# ****************************************\n");
  fprintf(of, "# *****       Make STIFF image       *****\n");
  fprintf(of, "# *****       created by THELI       *****\n");
  fprintf(of, "# ****************************************\n\n");
  fprintf(of, "cd %s\n\n", color_scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  fprintf(of, ". bash_functions.include\n\n");
  
  fprintf(of, "cd %s\n\n",colordir.ascii());
  
  i = 0;
  numimages = color_imagesListBox->count();
  
  // the RGB image
  if (stiff_mode.compare("rgb") == 0) {
    for (i=0; i<numimages; i++) {
      file = color_imagesListBox->text(i);
      tmp = color_statisticsTable->text(i,1);
      if (tmp.isEmpty()) tmp = "0.0";
      tmpfloat = tmp.stripWhiteSpace().toFloat();
      sky.setNum(tmpfloat);
      
      bfac2 = color_factorBLineEdit->text();
      rfac2 = color_factorRLineEdit->text();
      if (bfac2.isEmpty()) bfac2 = "1.0";
      if (rfac2.isEmpty()) rfac2 = "1.0";
      if (QString::compare(b_image,file) == 0) {
	fprintf(of, "${P_IC} '%s1 %s -' %s > %s_tmp\n", 
		"%", sky.ascii(), file.ascii(), file.ascii());
	fprintf(of, "${P_IC} '%s1 %s *' %s_tmp > %s_tmprescaled\n", 
		"%", bfac2.ascii(), b_image.ascii(), b_image.ascii());
	fprintf(of, "mv %s_tmprescaled %s_tmp\n", b_image.ascii(), b_image.ascii());
      }
      if (QString::compare(r_image,file) == 0) {
	fprintf(of, "${P_IC} '%s1 %s -' %s > %s_tmp\n", 
		"%", sky.ascii(), file.ascii(), file.ascii());
	fprintf(of, "${P_IC} '%s1 %s *' %s_tmp > %s_tmprescaled\n", 
		"%", rfac2.ascii(), r_image.ascii(), r_image.ascii());
	fprintf(of, "mv %s_tmprescaled %s_tmp\n", r_image.ascii(),  r_image.ascii());
      }
      if (QString::compare(v_image,file) == 0) {
	fprintf(of, "${P_IC} '%s1 %s -' %s > %s_tmp\n", 
		"%", sky.ascii(), file.ascii(), file.ascii());
      }
    }
  }
  
  // the LUM image
  if (stiff_mode.compare("lum") == 0) {
    for (i=0; i<numimages; i++) {
      file = color_imagesListBox->text(i);
      if (file.compare(l_image) == 0) {
	tmp = color_statisticsTable->text(i,1);
	if (tmp.isEmpty()) tmp = "0.0";
	tmpfloat = tmp.stripWhiteSpace().toFloat();
	sky.setNum(tmpfloat);
      }
    }
    fprintf(of, "${P_IC} '%s1 %s -' %s > %s_tmp\n", 
	    "%", sky.ascii(), l_image.ascii(), l_image.ascii());
  }
  
  stiff_minval = color_stiffminvalLineEdit->text();
  stiff_maxval = color_stiffmaxvalLineEdit->text();
  stiff_satval = color_stiffsatvalLineEdit->text();
  stiff_gamma  = color_stiffgammaLineEdit->text();
  
  fprintf(of, "${P_STIFF} -c ${CONF}/stiff.conf\\\n");
  fprintf(of, "           -OUTFILE_NAME %s\\\n", outfilename.ascii());
  fprintf(of, "           -GAMMA_FAC %s\\\n", stiff_gamma.ascii());
  fprintf(of, "           -COLOUR_SAT %s\\\n", stiff_satval.ascii()); 
  
  // for a colour image
  if (stiff_mode.compare("rgb") == 0) {
    fprintf(of, "           %s_tmp %s_tmp %s_tmp\\\n", 
	    r_image.ascii(), v_image.ascii(), b_image.ascii());
    fprintf(of, "           -MIN_LEVEL %s,%s,%s\\\n", 
	    stiff_minval.ascii(), stiff_minval.ascii(), stiff_minval.ascii()); 
    fprintf(of, "           -MAX_LEVEL %s,%s,%s\n", 
	    stiff_maxval.ascii(), stiff_maxval.ascii(), stiff_maxval.ascii());
    
    r_imagetiff = r_image;
    b_imagetiff = b_image;
    v_imagetiff = v_image;
    r_imagetiff.replace(".fits","_2tiff.fits");
    v_imagetiff.replace(".fits","_2tiff.fits");
    b_imagetiff.replace(".fits","_2tiff.fits");
    fprintf(of, "mv %s_tmp %s\n", r_image.ascii(), r_imagetiff.ascii());
    fprintf(of, "mv %s_tmp %s\n", v_image.ascii(), v_imagetiff.ascii());
    fprintf(of, "mv %s_tmp %s\n", b_image.ascii(), b_imagetiff.ascii());
    
    //new
    fprintf(of, "value %s\n", bfac2.ascii());
    fprintf(of, "writekey %s COLORFAC \"${VALUE} / Colour calibration factor\" REPLACE\n", 
	    b_imagetiff.ascii());
    fprintf(of, "value %s\n", rfac2.ascii());
    fprintf(of, "writekey %s COLORFAC \"${VALUE} / Colour calibration factor\" REPLACE\n", 
	    r_imagetiff.ascii());
    fprintf(of, "value %s\n", "1.000");
    fprintf(of, "writekey %s COLORFAC \"${VALUE} / Colour calibration factor\" REPLACE\n", 
	    v_imagetiff.ascii());
  }
  
  // for a luminance/bw image
  else {
    l_imagetiff = v_image;
    l_imagetiff.replace(".fits","_2tiff.fits");
    fprintf(of, "           %s_tmp\\\n", l_image.ascii());
    fprintf(of, "           -MIN_LEVEL %s\\\n", stiff_minval.ascii()); 
    fprintf(of, "           -MAX_LEVEL %s\n",   stiff_maxval.ascii()); 
    fprintf(of, "mv %s_tmp %s\n", l_image.ascii(), l_imagetiff.ascii());
  }
  
  fclose(of);
  
  // make the file executable
  execstr = "chmod +x ";
  execstr.append(filename);
  my_system(execstr);
  
  proc_col_stiff_started = 0;
  write_superscriptbody(homedir+".theli/scripts/theli_gettiffpreview.sh", 
			"./gettiffpreview.sh", "stiff");
}


void color_form::get_color_correction()
{
  QDir bvdir;
  QString filename, execstr, command;
  
  photcatresult[0].catname = "SDSS";
  photcatresult[1].catname = "UCAC4";
  photcatresult[2].catname = "NOMAD";
  photcatresult[3].catname = "AVGWHITE";

  color_factorRLineEdit->setText("");
  color_factorGLineEdit->setText("1.000");
  color_factorBLineEdit->setText("");
  color_errorRLineEdit->setText("");
  color_errorGLineEdit->setText("0.000");
  color_errorBLineEdit->setText("");
  
  // first of all, check if the network connection can be established
  if (!test_network()) {
    QMessageBox::warning( this, "Network connection failed!",
			  "Could not connect to the CDS-server!\n"
			  "Either your network connection or the CDS-server is down.");
    return;
  }
  
  bvdir.setPath(colordir + "/PHOTCAT_calibration/");
  if (! bvdir.exists()) bvdir.mkdir(colordir + "/PHOTCAT_calibration/");
  
  r_image = color_BV_r_ComboBox->currentText();
  v_image = color_BV_v_ComboBox->currentText();
  b_image = color_BV_b_ComboBox->currentText();
  
  filename = color_scriptdir;
  if (!filename.endsWith("/")) filename.append("/");    
  
  // leave if the directory does not exist
  if (!bvdir.exists()) return;
  
  // launch the colour correction script
  proc_col_colcalib_started = 0;

  QString server;
  switch(color_serverComboBox->currentItem()) {
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
  command = "./color_correction_photcat.sh " + 
    color_scriptdir + " " + colordir + " " + b_image + " " + v_image + " " + r_image + " " + server;
  
  write_superscriptbody(homedir+".theli/scripts/theli_color_correction_photcat.sh", 
			command, "colcalib");
}


void color_form::fill_in_colcalib()
{
  QStringList lst, photcatresults;

  // check the network connection status
  QFile connectionstatus(colordir+"/PHOTCAT_calibration/unconnected");
  if (connectionstatus.exists()) {
    QMessageBox::information( this, "Network unreachable!",
			      "No calibration will be obtained based on reference catalogs.\n");
  }

  // The best catalog
  QFile bestcatfile(colordir+"/PHOTCAT_calibration/PHOTCAT_bestcat");
  if ( bestcatfile.open( IO_ReadOnly )) {
    QTextStream stream( &bestcatfile );
    bestcat = stream.readLine();
  }

  // read all the photcat results from the files into a struct
  photcatresults.append(colordir+"/PHOTCAT_calibration/PHOTCAT_calibration_SDSS.dat");
  photcatresults.append(colordir+"/PHOTCAT_calibration/PHOTCAT_calibration_UCAC4.dat");
  photcatresults.append(colordir+"/PHOTCAT_calibration/PHOTCAT_calibration_NOMAD.dat");
  photcatresults.append(colordir+"/PHOTCAT_calibration/PHOTCAT_calibration_AVGWHITE.dat");

  int j = 0;
  for ( QStringList::Iterator it = photcatresults.begin(); it != photcatresults.end(); ++it ) {
    // read the data into the structure;
    read_into_struct(*it, j);
    if (bestcat.compare(photcatresult[j].catname) == 0) bestindex = j;
    j++;
  }
  
  // show how many stars were retrieved
  color_nstarsSDSSTextLabel  -> setText(photcatresult[0].nstars+" stars");
  color_nstarsUCAC4TextLabel -> setText(photcatresult[1].nstars+" stars");
  color_nstarsNOMADTextLabel -> setText(photcatresult[2].nstars+" stars");
  color_nstarsAVGWHITETextLabel -> setText(photcatresult[3].nstars+" stars");
  
  // Toggle the best catalog (ignore AVGWHITE)
  if (bestindex == 0) color_SDSSPushButton->setOn(true);
  else if (bestindex == 1) color_UCAC4PushButton->setOn(true);
  else color_NOMADPushButton->setOn(true);
  
  // show the corresponding correction factors
  set_toggle_button();
  
  // Complain if no reference stars were retrieved
  if (photcatresult[bestindex].nstars.toInt() == 0) {
    QMessageBox::information( this, "No G2-type reference found!\n", 
			      "No G2-type reference sources could be matched in this field!\n"
			      "Either there are none, or your exposure is not deep enough\n"
			      "to match fainter sources. Use the 'average white' instead?\n");
  }
}


void color_form::read_into_struct(QString filename, int index)
{
  QFile file(filename);
  QStringList lst;
  int i = 0;

  if ( file.open( IO_ReadOnly )) {
    QTextStream stream( &file );

    while ( !stream.atEnd() ) {
      if (i==0) photcatresult[index].nstars = stream.readLine();
      if (i==1) {
	lst = lst.split( " ", stream.readLine() );
	photcatresult[index].bfac    = lst[0];
	photcatresult[index].bfacerr = lst[1];
      }
      if (i==2) {
	lst = lst.split( " ", stream.readLine() );
	photcatresult[index].gfac    = lst[0];
	photcatresult[index].gfacerr = lst[1];
      }
      if (i==3) {
	lst = lst.split( " ", stream.readLine() );
	photcatresult[index].rfac    = lst[0];
	photcatresult[index].rfacerr = lst[1];
      }
      i++;
    }
  }
  file.close();
}


void color_form::set_toggle_button()
{
  int index = 0;
  if (color_SDSSPushButton->isOn())  index = 0;
  if (color_UCAC4PushButton->isOn()) index = 1;
  if (color_NOMADPushButton->isOn()) index = 2;
  if (color_AVGWHITEPushButton->isOn()) index = 3;

  // Fill in the best values, toggle the corresponding button
  color_factorRLineEdit->setText(photcatresult[index].rfac);
  color_factorGLineEdit->setText(photcatresult[index].gfac);
  color_factorBLineEdit->setText(photcatresult[index].bfac);
  color_errorRLineEdit->setText(photcatresult[index].rfacerr);
  color_errorGLineEdit->setText(photcatresult[index].gfacerr);
  color_errorBLineEdit->setText(photcatresult[index].bfacerr);
}


void color_form::fits2tiff()
{
  FILE *of;
  int i, numimages, suffixpos;

  QString filename, execstr, basename, low, high, file, file2, sky;
  QStringList lst;

  filename = color_homedir;
  if (!filename.endsWith("/")) filename.append("/");    
  filename.append(".theli/scripts/fits2tiff.sh");

  // new
  r_image = color_BV_r_ComboBox->currentText();
  v_image = color_BV_v_ComboBox->currentText();
  b_image = color_BV_b_ComboBox->currentText();

  // write the script
  of = fopen(filename, "w");
  
  fprintf(of, "%s%s\n\n", "#!",bash.ascii());
  fprintf(of, "# *****************************************\n");
  fprintf(of, "# ***** Create color image: FITS2TIFF *****\n");
  fprintf(of, "# *****        created by THELI       *****\n");
  fprintf(of, "# *****************************************\n\n");
  fprintf(of, "cd %s\n\n", color_scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  fprintf(of, ". bash_functions.include\n\n");

  fprintf(of, "cd %s\n\n",colordir.ascii());

  i = 0;
  numimages = color_imagesListBox->count();

  for (i=0; i<numimages; i++) {
    file = color_imagesListBox->text(i);
    suffixpos = file.findRev('.');
    basename = file.left(suffixpos);
    QString tmp = color_statisticsTable->text(i,1);
    if (tmp.isEmpty()) tmp = "0.0";
    float tmpfloat = tmp.stripWhiteSpace().toFloat();
    sky.setNum(tmpfloat);
    
    // first, subtract the sky
    fprintf(of, "${P_IC} '%s1 %s -' %s > %s_tmp\n", 
	    "%", sky.ascii(), file.ascii(), file.ascii());
    
    // apply the B-V correction (if present)
    bfac = color_factorBLineEdit->text();
    rfac = color_factorRLineEdit->text();
    if (bfac.isEmpty()) bfac = "1.0";
    if (rfac.isEmpty()) rfac = "1.0";
    
    if (QString::compare(b_image,file) == 0) {
      fprintf(of, "${P_IC} '%s1 %s *' %s_tmp > %s_tmprescaled\n", 
	      "%", bfac.ascii(), b_image.ascii(), b_image.ascii());
      fprintf(of, "mv %s_tmprescaled %s_tmp\n", b_image.ascii(), b_image.ascii());
    }
    if (QString::compare(r_image,file) == 0) {
      fprintf(of, "${P_IC} '%s1 %s *' %s_tmp > %s_tmprescaled\n", 
	      "%", rfac.ascii(), r_image.ascii(), r_image.ascii());
      fprintf(of, "mv %s_tmprescaled %s_tmp\n", r_image.ascii(),  r_image.ascii());
    }
    
    // new
    fprintf(of, "value %s\n", bfac.ascii());
    fprintf(of, "writekey %s_tmp COLORFAC \"${VALUE} / Colour calibration factor\" REPLACE\n", 
	    b_image.ascii());
    fprintf(of, "value %s\n", rfac.ascii());
    fprintf(of, "writekey %s_tmp COLORFAC \"${VALUE} / Colour calibration factor\" REPLACE\n", 
	    r_image.ascii());
    fprintf(of, "value %s\n", "1.000");
    fprintf(of, "writekey %s COLORFAC \"${VALUE} / Colour calibration factor\" REPLACE\n", 
	    v_image.ascii());
    
    // do the FITS2TIFF conversion
    low  = color_statisticsTable->text(i,2);
    high = color_statisticsTable->text(i,3);
    fprintf(of, "${P_FITS2TIFF} -i %s_tmp -o %s.tif -r %s %s\n", 
	    file.ascii(), basename.ascii(), low.ascii(), high.ascii());
  }
  
  for (i=0; i<numimages; i++) {
    file = color_imagesListBox->text(i);
    file2 = file;
    file2.replace(".fits","_2tiff.fits");
    fprintf(of, "mv %s_tmp %s\n", file.ascii(), file2.ascii());
  }
  
  fclose(of);
  // make the file executable
  execstr = "chmod +x ";
  execstr.append(filename);
  my_system(execstr);
  
  // execute the script
  proc_col_fits2tiff_started = 0;
  write_superscriptbody(homedir+".theli/scripts/theli_fits2tiff.sh", 
			"./fits2tiff.sh", "fits2tiff");
}


void color_form::get_maxoverlap()
{
  FILE *of;
  int i, numimages;
  QString filename, execstr, imagelist = "", imagepath;
  QString str_crval1, str_crval2;
  
  numimages = color_imagesListBox->count();
  
  double crval1[numimages], crval2[numimages];
  
  // do nothing if no images are selected
  if (numimages == 0) return;
  
  for (i=0; i<numimages; i++) {
    imagelist.append(color_imagesListBox->text(i));
    imagelist.append(" ");
  }
  
  // check if the images have identical CRVAL1 CRVAL2 header keys;
  // if not, complain
  
  for (i=0; i<numimages; i++) {
    imagepath=colordir+"/"+color_imagesListBox->text(i);
    execstr=color_bindir+"/getkey "+imagepath+" CRVAL1";
    my_fscanf(execstr, str_crval1);
    execstr=color_bindir+"/getkey "+imagepath+" CRVAL2";
    my_fscanf(execstr, str_crval2);
    crval1[i] = str_crval1.toDouble();
    crval2[i] = str_crval2.toDouble();
  }
  
  for (i=1; i<numimages; i++) {
    if (crval1[0] != crval1[i] || crval2[0] != crval2[i]) {
      QMessageBox::critical( this, "Inconsistent astrometric headers",
	      "The images do not have identical CRVAL1 and CRVAL2 header keywords.\n"
	      "You must re-run the coaddition for all filters with identical values\n"
	      "for the reference RA and DEC. Otherwise the coadded images cannot be\n"
	      "registered, and the colour calibration will fail.\n");
	return;
      }
    }
  
  filename = color_homedir;
  if (!filename.endsWith("/")) filename.append("/");
  filename.append(".theli/scripts/getmaxoverlap.sh");
  
  // write the script
  of = fopen(filename, "w");
  
  fprintf(of, "%s%s\n\n", "#!",bash.ascii());
  fprintf(of, "# ******************************************\n");
  fprintf(of, "# ***** Create color image: maxoverlap *****\n");
  fprintf(of, "# *****        created by THELI        *****\n");
  fprintf(of, "# ******************************************\n\n");
  fprintf(of, "cd %s\n\n", color_scriptdir.ascii());
  fprintf(of, ". progs.ini\n\n");
  fprintf(of, ". bash.include\n\n");
  
  fprintf(of, "cd %s\n\n",colordir.ascii());
  
  fprintf(of, "${P_DFITS} %s | ${P_FITSORT} CRPIX1 CRPIX2 NAXIS1 NAXIS2 \\\n", imagelist.ascii());
  fprintf(of, " | ${P_GAWK} '($1 !~ /FILE/) {print $2, $3, $4, $5}' > tmp_$$\n\n");
  
  fprintf(of, "xlow=`${P_GAWK} 'BEGIN {min = 1000000} {if ($1<min) min = $1} END {print (-1)*min}' tmp_$$`\n");
  fprintf(of, "ylow=`${P_GAWK} 'BEGIN {min = 1000000} {if ($2<min) min = $2} END {print (-1)*min}' tmp_$$`\n");
  fprintf(of, "xhigh=`${P_GAWK} 'BEGIN {min = 1000000} {if (($3-$1)<min) min = $3-$1} END {print min}' tmp_$$`\n");
  fprintf(of, "yhigh=`${P_GAWK} 'BEGIN {min = 1000000} {if (($4-$2)<min) min = $4-$2} END {print min}' tmp_$$`\n\n");
  
  fprintf(of, "ls -1 %s > tmp_$$\n", imagelist.ascii());
  
  fprintf(of, "cat tmp_$$ |\\\n");
  fprintf(of, "{\n");
  fprintf(of, "    while read file\n");
  fprintf(of, "    do\n");
  fprintf(of, "      crpix1=`get_key ${file} CRPIX1`\n");
  fprintf(of, "      crpix2=`get_key ${file} CRPIX2`\n");
  fprintf(of, "      BASE=`basename ${file} .fits`\n");
  fprintf(of, "      xlownew=`echo ${xlow} ${crpix1} | ${P_GAWK} '{print $2+$1+1}'`\n");
  fprintf(of, "      ylownew=`echo ${ylow} ${crpix2} | ${P_GAWK} '{print $2+$1+1}'`\n");
  fprintf(of, "      xhighnew=`echo ${xhigh} ${crpix1} | ${P_GAWK} '{print $2+$1}'`\n");
  fprintf(of, "      yhighnew=`echo ${yhigh} ${crpix2} | ${P_GAWK} '{print $2+$1}'`\n");
  fprintf(of, "      ${P_FITSCUT} -i ${file} -o ${BASE}_cropped.fits \\\n");
  fprintf(of, "               -x ${xlownew} ${xhighnew} ${ylownew} ${yhighnew}\n");
  fprintf(of, "      if [ -f ${BASE}.weight.fits ]; then\n");
  fprintf(of, "         ${P_FITSCUT} -i ${BASE}.weight.fits -o ${BASE}_cropped.weight.fits \\\n");
  fprintf(of, "                  -x ${xlownew} ${xhighnew} ${ylownew} ${yhighnew}\n");
  fprintf(of, "         rm ${BASE}.weight.fits\n");
  fprintf(of, "      fi\n");
  fprintf(of, "      rm ${file}\n");
  fprintf(of, "    done\n");
  fprintf(of, "}\n\n");
  fprintf(of, "\\rm tmp_$$\n");
  fclose(of);

  // make the file executable
  execstr = "chmod +x ";
  execstr.append(filename);
  my_system(execstr);

  // execute the script
  proc_col_crop_started = 0;
  write_superscriptbody(homedir+".theli/scripts/theli_getmaxoverlap.sh", 
			"./getmaxoverlap.sh", "crop");
}


void color_form::update_colordirpath()
{
  colordir = color_dirLineEdit->text();
  coadddir = color_dirLineEdit->text();
  colordir.append("/color_theli/");
  coadd_directory.setPath(coadddir);
  color_directory.setPath(colordir);
}


void color_form::init()
{
  QString lastlog, execstr, tmp, sciencedir, color_inst_path;
  QSettings settings (QSettings::Ini);
  QColor cfred = QColor(255,187,155);
  QColor cfgreen = QColor(155,255,187);
  QColor cwhite = QColor(255,255,255);

  proc_col_combine_started = 0;
  proc_col_colcalib_started = 0;
  proc_col_stiff_started = 0;
  proc_col_fits2tiff_started = 0;
  proc_col_elum_started = 0;
  proc_col_crop_started = 0;

  started = 0;

  // get general info
  settings.beginGroup("/theligeneral_");
    lastlog = settings.readEntry("/lastlog", "noname");
    color_scriptdir = settings.readEntry("/mainpath", "");
    color_homedir = settings.readEntry("/homedir", "");
    color_bindir = settings.readEntry("/bindir", "");
    bash = settings.readEntry("/bash", "");
  settings.endGroup();

  // grep the currently selected instrument and science dir
  settings.beginGroup("/theli_"+lastlog+"_");
    settings.beginGroup("/INITIALISE");
      sciencedir = settings.readEntry("/maindir");
      sciencedir.append(settings.readEntry("/sciencedir"));
      instrument = settings.readEntry("/instrumentname");
      if (settings.readBoolEntry("/inst_prof",true)) {
	color_inst_path = color_scriptdir + "/instruments_professional/";
      }
      if (settings.readBoolEntry("/inst_comm",true)) {
	color_inst_path = color_scriptdir + "/instruments_commercial/";
      }
      if (settings.readBoolEntry("/inst_user",true)) {
	color_inst_path = color_homedir + "/.theli/instruments_user/";
      }
    settings.endGroup();
  settings.endGroup();

  // get the number of chips
  execstr = "grep NCHIPS= ";
  execstr.append(color_inst_path);
  execstr.append("/");
  execstr.append(instrument);
  execstr.append(".ini");
  execstr.append("| gawk 'BEGIN {FS=\"=\"} {print $2}'");
  my_fscanf(execstr, numchips);

  //  initialise the dialog
  color_imagesListBox->clear();
  color_BV_r_ComboBox->clear();
  color_BV_v_ComboBox->clear();
  color_BV_b_ComboBox->clear();
  color_bbnb_broadComboBox->clear();
  color_bbnb_narrowComboBox->clear();
  color_bbnb_broadwidthLineEdit->setText("100");
  color_bbnb_narrowwidthLineEdit->setText("10");
  color_bbnb_weightLineEdit->setText("1.0");
  color_factorRLineEdit->clear();
  color_factorGLineEdit->setText("1.000");
  color_factorBLineEdit->clear();
  color_errorRLineEdit->clear();
  color_errorGLineEdit->setText("0.000");
  color_errorBLineEdit->clear();
  color_stiffminvalLineEdit->setText("-0.1");
  color_stiffmaxvalLineEdit->setText("2");
  color_stiffsatvalLineEdit->setText("1.0");
  color_stiffgammaLineEdit->setText("1.0");

  QHeader *vertical = color_statisticsTable->verticalHeader();
  vertical->hide();
  color_statisticsTable->setLeftMargin(0);
  color_statisticsTable->setColumnWidth(0,160);
  color_statisticsTable->setColumnWidth(1,100);
  color_statisticsTable->setColumnWidth(2,100);
  color_statisticsTable->setColumnWidth(3,100);
  color_statisticsTable->setColumnWidth(4,100);
  color_statisticsTable->setNumCols(5);
  color_statisticsTable->setNumRows(0);
  color_statisticsTable->setColumnStretchable(0,true);

  //  no statistics done yet...
  getstat = 0;

  // set the directory line edit and repaint its background
  color_dirLineEdit->setText(sciencedir);
  color_imagesListBox->clear();

  // create the color_theli directory if it does not exist yet
  colordir = color_dirLineEdit->text();
  coadddir = color_dirLineEdit->text();
  colordir.append("/color_theli/");

  coadd_directory.setPath(coadddir);
  color_directory.setPath(colordir);
  if (!color_directory.exists()) color_directory.mkdir(colordir);

  direxists = 0;
  if (coadd_directory.exists() && sciencedir.length() > 0 ) {
    direxists = 1;
    color_dirLineEdit->setPaletteBackgroundColor(cfgreen);
  }
  if (!coadd_directory.exists() && sciencedir.length() > 0 ) {
    direxists = 0;
    color_dirLineEdit->setPaletteBackgroundColor(cfred);
  }
  if (sciencedir.length() == 0 ) {
    direxists = 0;
    color_dirLineEdit->setPaletteBackgroundColor(cwhite);
  }
  
  // populate the image ListBox
  get_colorchannels();

  started = 1;
}
