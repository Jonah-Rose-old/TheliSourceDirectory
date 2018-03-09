/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
/*
  Progress Monitor for some theli processes
  application form added to theliform.ui
  created with QT Designer 3.3.7
  author: Carsten Moos
  email:  cmoos@gmx.de
  date: 06.10.2012
  tested for theli 2.5.1 after 01.01.2012
  consists of: pmonitor.h, pmonitor.ui, pmonitor.ui.h (this), pmonitorfuntions.h
  anchors to theliform.ui.h / menu / Progress Monitor 
  is non modal
  */
/*Limitations:
  Preprocession: monitoring bias, dark and flat
  Calibrating  : monitoring bias, dark and flat
  this won't work with the given concept. They have the same step in theli
  and the timer has to run multiple times.
  Combining subsky and resampling would be nice, but the timer has to run two times. No idea how this could be done in similar manner.
  Sound after finishing a step : every Desktop seems to be different
*/
/*
  Changes in Version 0.35
  some cosmetic optimizations of the code
  get_buttonID, get_viewer, put_sound()

*/
/*
  Changes in Version 0.34
  new function set_reference_stamp for referencing last log file for pmon-stamp
  tooltip NOMAD added
  errlog=" >>/tmp/pmon.log "
  Stamp-button hidden  , mybe obsolete in this version
  get_paths() after Start button obligate to get current ident or paths
  */
/*
  Changes in Version 0.33
  scripts/pmon_ds9-show-RGB.sh included via btn_show-Bayer-RGB
  Btn_show_Bayer-RGB
  cosmetics in the form, texts
  pmon_protocol2.sh will not longer hang
  ds with scripts/pmon_ds9_analysis.ds9
  
  */

/*
Changes in Version 0.31
scripts/pmon_astrom_stats.sh with call pmon_getflxscale.sh
- now watching the pid and trial to include processmonitoring
- TouchStamp after successful count has moved to ??? trial at show-protocol
- Delay of 1 Interval after start
- Pre: counts science and ORIGINALS, if theli has already moved files, maybe lost of a file
- 

*/

/*
  Changes in Version 0.30
  Button correct NOMAD added and two scripts 
  scripts/pmon_get_corrected_photcat_calibr_NOMAD_blue.sh
scripts/pmon_get_corrected_photcat_calibr_NOMAD_red.sh
added
  */

/*
Changes made in Version 0.29: 06.11.11
- Blink with skycat disabled, does't work properly
- scripts rename to theli-like scheme
- scripts : apps in background 
- ds9 uses regions instead of catalogs
- all monitoring checked for cam with BAYER matrix

Changes made in Version 0.28: 19.6.11
- Result Monitor added
- Tests with
* Skycat: remote tk.h fehlt beim Kompilieren neuerer Versionen. Bei Herbert geht auch   die 3.0 nicht.
  remote calling: skycat fitsfile -remote 1
  (( skycat.sh ))
  ressources:
     . http://archive.eso.org/cms/tools-documentation/eso-skycat-download
     . 
* DS9: remote mit http://debian.ethz.ch/d-i/ds9/xpa.linux.2.1.10.tar.gz
  remote calling: xpaset -p ds9 file fitsfile
  (( ds9.sh ))
   ressources:
     . http://hea-www.harvard.edu/RD/ds9/
     . http://debian.ethz.ch/d-i/ds9/
     . http://hea-www.harvard.edu/RD/xpa/programs.html
     . http://hea-www.harvard.edu/RD/ds9/ref/ref.pdf
     . http://hea-www.harvard.edu/RD/ds9/user/user.pdf

* solved: QtSettings groups not all closed, appl crashes without message ??? Maybe QT Version conflict?
* needs external programms:
     . skycat (V 3.0 ) or ds9 (5.1 from 2008) from path 
     . eog ( or display of ImageMagic package )
     . firefox ( or link to other browsers ) in script protocol_theli2.sh
     . scripts: pmon_skycat.sh, pmon_skycat_withcat.sh, pmon_ds9.sh, pmon_ds9_withcat.sh, pmon_protocol_theli2.sh, pmon_astrom_stats.sh, pmon_astrom_showplots.sh
     .tools: cut, tail, ls, ps, grep, wc, find, 

*/
/*
Changes made in Version 0.27: 10.6.11
- counting skysub distinguishes now between auto model and manual model
- subskyautomodel has 2 runs, problem with 50% level not to be solved simply
- Note: on NFS the file timestamp has sometimes only date no time information, so
  the counter will fail at the same day
- Note using guassian smooth, counter runs twice too, even though the _skysub.fits     files are deleted right after using  and .sub.fits are created in first step .
  This is different by using sextractor, where two runs are essential. Treated same.
-
*/
#include <qapplication.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <pmonitor.h>
#include <qlistbox.h>
#include <qstring.h>
#include <stdio.h>
#include <qsettings.h>
#include "functions.h"
#include <unistd.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include "globalvariables.h"
#include "pmonitorfunctions.h"

/*
Check image
QString GLB_image;
0:
1: 
*/

int GBL_Start_Queue=1;             // can be set by theliform.ui Start / Abort
int GBL_autoskysub=0;              // Flag for exeption automodelling with 2 runs
int GBL_AGAIN=1;                   // 0 stops Timer
QString IMG_name, IMG_path, CAT_path;

QString errlog=" >>/tmp/pmon.log ";

QString subdir1,pattern1,pattern2="",pattern3="",process="",logfile="";
int iBASICfilecounter=0;
QTimer *timer;
QString EXTstatus;
int Total=0,Intervall=1000;    // time elapsed, timer intervall
void pmonitor::p_start()
{
    int group=-1, buttonID;
    QString SEARCH_PATH_PATTERN;
    QString execstr,result,Info;
    pattern1=""; 
    pattern2="";
    pattern3="";
    logfile="";
    Info = progress_listBox->currentItem();
    progress_info->setText(Info + ";" + sciencepath +";" +mainpath);
    
    get_paths();    // for ident update
    group=progress_listBox->currentItem(); 
    if (group==0)                       // Automatik: Itemnr=0
    {
	if (process_group=="pre") progress_listBox->setCurrentItem(1);
	if (process_group=="cal") progress_listBox->setCurrentItem(2);
	if (process_group=="w")   progress_listBox->setCurrentItem(3);
	if (process_group=="ap")  progress_listBox->setCurrentItem(4);
	if (process_group=="cc") 
	    {
	  // Seperate different processes in this group
	  //ps -j x|grep skysub|grep -v grep|wc -l
	   execstr = " ps -j x|grep skysub|grep -v grep|wc -l";
               my_fscanf(execstr, result);
	   if (atoi(result) >0)   
	       progress_listBox->setCurrentItem(5);   // subskying
	   else
	       progress_listBox->setCurrentItem(6);   // resampling
		
	       // if (theliForm->cc_commandTextEdit.countains ("skysub")) progress_listBox->setCurrentItem(5);
	       // if (theliForm->cc_commandTextEdit .contains("resample_coadd")) progress_listBox->setCurrentItem(6);

	    }
    }
    group=progress_listBox->currentItem(); 
    //Automatik, Korrektur des Inkrements fuer group
    group--;		// decrementing Listorder in respect of automatic 
    if (group ==-1) {
    QMessageBox::warning( this,"Automatic", "Processing group not recognized. Theli is not working.");
    }
    else
    {
    
    EXTstatus=get_ext(); 
    GBL_AGAIN=1;                                // enable TIMER
         // count all calibrated files
    SEARCH_PATH_PATTERN=sciencepath + "/*" +EXTstatus +".fits";
	 
    switch (group)
    {
       case 0: //preprocessing
         iBASICfilecounter=count2(sciencepath);  // hier werden alle sciencefiles gezaehlt.
	 iBASICfilecounter+=count2(sciencepath + "/ORIGINALS"); // already removed files too
         subdir1="";
         pattern1="*_?.fits";  // Count subFiles CPU=1
         pattern2="*_??.fits"; // Count subFiles CPU=11
 	 IMG_path=sciencepath;
	 IMG_name=pattern1;
	 CAT_path="";
	
	 process="process_split";
         break;	
       case 1:  //calibrating all sensors
       {
         iBASICfilecounter=count3(sciencepath);  // hier werden alle preprocessed  files gezaehlt.
         subdir1="";
         pattern1="*_?"   +EXTstatus +".fits"; // Count subFiles CPU=1
         pattern2="*_??" +EXTstatus +".fits";// Count subFiles CPU=11
	 if (get_BAYER(sciencepath)) {
		iBASICfilecounter*=3;
           	//pattern3="*_[RGB]_*" +EXTstatus +".fits";// Count Bayerfiles CPU=*
	 } 
	IMG_path=sciencepath;
	IMG_name=pattern1;
	CAT_path="";
	process="process_science";
        break;
       }	
       case 2:  //weighting
       {
	iBASICfilecounter=count1(SEARCH_PATH_PATTERN);
        subdir1="/../WEIGHTS";
        pattern1="*"+EXTstatus+".weight.fits"; // Count subFiles
	IMG_path=sciencepath+subdir1;
	IMG_name=pattern1;
	CAT_path="";
	process="create_weights";
        logfile="w";
        break;
       }
       case 3:  //createcatalog
       {
	iBASICfilecounter=count1(SEARCH_PATH_PATTERN);
        subdir1="/cat";
        pattern1="*"+EXTstatus+".cat1"; // Count subFiles
	IMG_path=sciencepath;
	IMG_name="*_?"   +EXTstatus +".fits";
	CAT_path=sciencepath+subdir1;//+"/skycat";
	chk_cat->setEnabled(1);
	//process="sex";
        process="create_astromcats";
	logfile="ap";
        break;
       }

       case 4:  //subsky
       {
	iBASICfilecounter=count1(SEARCH_PATH_PATTERN);
        subdir1="";
        //pattern1="*"+EXTstatus+"*sub.fits"; // Count subFiles, model = OFC_skysub.fits
        //if (theliForm->cc_commandTextEdit.contains ("ALL")){
	buttonID=get_buttonID();

        if (buttonID==0){
		// auto modelling
		//iBASICfilecounter*=2;
		//pattern1="*"+EXTstatus+"*[.sky]sub.fits";
		pattern1="*"+EXTstatus+"_skysub.fits";
		pattern2="*"+EXTstatus+".sub.fits";
                GBL_autoskysub=1;
	}
	else{
		// no auto modelling, 
		pattern1="*"+EXTstatus+"*.sub.fits";
		GBL_autoskysub=0;
	}
	IMG_path=sciencepath;
	IMG_name="*"+EXTstatus+"*.sub.fits";
	CAT_path=sciencepath+"cat/";//skycat";
	chk_cat->setEnabled(1);
	process="create_skysub";
	logfile="cc";
        break;
       }
       case 5:  //resamp
       {
	iBASICfilecounter=count1(SEARCH_PATH_PATTERN);
	subdir1="/coadd_"+ident;
	pattern1="*"+EXTstatus+"*.resamp.fits"; // Count subFiles
 
         // Count filtered OFC
        iBASICfilecounter=count1(sciencepath +subdir1 + "/*"+EXTstatus+"*.head");
	IMG_path=sciencepath+subdir1;
	IMG_name=pattern1;
	CAT_path="";
	process="resample_coadd";
	logfile="cc";
	 break;
       }
       default:
         GBL_AGAIN=0;    // stops timer, paranoid
    }
fitsFile->setText(IMG_path+"/"+IMG_name);	 // set filename pattern for showfits
progress_startpushButton->setEnabled(FALSE);     // disable start-button
set_reference_stamp(logfile);			 // set reference time
timer->start( Intervall, FALSE );                // 1 seconds  timer
    }  // end of else
//QMessageBox::warning( this,"Debuginfo", "EXTstatus: " +EXTstatus);
}


void pmonitor::init()
{
QString str,execstr,VERSION=" 0.35";
    //this->setCaption(this->caption()+ VERSION); 
    this->setCaption("Theli Monitor "+ VERSION); 
    //this->setX(theliForm->setX()+theliForm->Width); 
    this->setGeometry(800,0,360,688);
    progress_startpushButton_newstamp->setHidden(true); // no longer needed
    progress_info->setHidden(true); // only for debugging
    progress_info->setText("init");
    progress_listBox-> removeItem(0);

    progress_listBox-> insertItem(QString("Automatic          "));   
    progress_listBox-> insertItem(QString("Split Science      "));
    progress_listBox-> insertItem(QString("Calibrating        "));
    progress_listBox-> insertItem(QString("Weighting          "));
    progress_listBox-> insertItem(QString("Creating Catalogs  "));
    progress_listBox-> insertItem(QString("Subtracting sky    "));
    progress_listBox-> insertItem(QString("Resampling         "));
    progress_listBox->setCurrentItem(0);   // Automatic as default
    progress_info->setText("init + list inserted");
    
    chk_cat->setEnabled(0); 
    get_paths();

    timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(timer_done()) );
    
     // first create Timestamp
    TouchStamp(mainpath);
}
void pmonitor::destroy()
{
// disconnect( timer, SIGNAL(timeout()), this, SLOT(timer_done()) )  ;
 delete  timer;
}

void pmonitor::p_reset()
{
progress_info->setText("Reset");
progress_status->setText("");
progressBar->setProgress(0); // -1 waere logisch, geht aber nicht
progress_startpushButton->setEnabled(1);
GBL_AGAIN=0;
timer->stop();              //  timer off
Total=0;
get_paths();                // reread science and mainpath
progress_listBox->setCurrentItem(0);   // Automatik
//chk_cat->setEnabled(0);
}


void pmonitor::timer_done()
{
QString execstr,result;
int status=0, pid=0;
int iCURfilecounter=0; // current numer of already finished files
static int DELAY=0;   // short delay at first run until theli is actually running
static int RUN=0;     // Theli has started=
Total+= (int)(Intervall/1000.0);
if (DELAY++ >1){      // Wait for theli to initialize for 1 intervall

    if (pattern1 !="")
    {
      execstr = "{ find  ";
      execstr.append(sciencepath +subdir1 +"/");
      execstr.append(" -maxdepth 1 -newer " +mainpath +"/pmon-stamp -name ");
      execstr.append("\"" + pattern1 +"\" ");
      if (pattern2 !="") execstr.append( "; find  "+sciencepath +subdir1 +"/" +" -maxdepth 1 -newer " +mainpath +"/pmon-stamp -name \"" + pattern2 +"\"  2>/dev/null " );
      if (pattern3 !="") execstr.append( "; find  "+sciencepath +subdir1 +"/" +" -maxdepth 1 -newer " +mainpath +"/pmon-stamp -name \"" + pattern3 +"\"  2>/dev/null " );
      execstr.append( "; }");	
      execstr.append("|wc -l");

      my_fscanf(execstr, result);
      iCURfilecounter=atoi(result);
      progress_info->setText(QString::number(iCURfilecounter)+";" + QString::number(iBASICfilecounter) + ";" +pattern1+"||"+pattern2);
      if (iBASICfilecounter >0)
         status=(int)(iCURfilecounter*100.0/iBASICfilecounter);
      else
         status=100;
      if (iBASICfilecounter<iCURfilecounter) status=100; // should not occur
      progressBar->setProgress(status);
    }
  pid = get_pid(process);
  //QMessageBox::warning( this,"Debuginfo","name: " +process +" pid: " +QString::number(pid));
  if (iCURfilecounter >0 && pid>0){ 
      progress_status->setText("Counter: " + QString::number(iCURfilecounter)+" of " + QString::number(iBASICfilecounter) +", Time: " +QString::number(Total) +" s of " + QString::number((int)(Total *1.0 / iCURfilecounter*iBASICfilecounter)) +" s " +QString::number(pid));
      RUN=1;  // Theli is actually working
      }
  else
      progress_status->setText("waiting for theli ...");
  
  if (RUN && !(pid>0)) status=100;  // stop if theli has aborted
  if (!(GBL_Start_Queue && GBL_AGAIN && status <100 ))  // stop timer
    { progress_status->setText("Pid: " + QString::number(pid));
      if (GBL_autoskysub) {     // Exeption: second stage of autosubsky
           pattern1=pattern2;
           pattern2="";
	   //iBASICfilecounter/=2;
           GBL_autoskysub=0;
       }
       else{
          timer->stop();	
          progress_startpushButton->setEnabled(TRUE);
          progress_status->setText("Done in " +QString::number(Total) +" s");
	  put_sound();
      //  create timestamp not sure, if Theli already finished all
      //  TouchStamp(mainpath);
          progress_info->setText(execstr);
          Total=0;
          progress_listBox->setCurrentItem(0);   // automatic
	  status=0;
          RUN=0;
       }  // else GLB_autoskysub
    }     // if status =100
  }       // DELAY
}


void pmonitor::show_fits()
{
QString execstr,result, check="",cat="", viewer="skycat";

viewer=get_viewer();
if (viewer=="skycat") chk_blink->setEnabled(false);  // skycat not supported
else
chk_blink->setEnabled(true);
//QMessageBox::warning( this,"Debuginfo", "scriptdir: +viewer" +scriptdir +";" +viewer);
if (chk_blink->isChecked()) check="1";
if (chk_cat->isChecked() && CAT_path!=""){
  if (viewer=="skycat"){
    execstr = scriptdir+"/pmon_skycat_withcat.sh '"+fitsFile->text()+"'  '"+CAT_path+"/skycat/ "+check +" " +cat +"'" +errlog;
	
  }
  if (viewer=="ds9"){
     execstr = scriptdir+"/pmon_ds9_withcat.sh '"+fitsFile->text()+"' "+CAT_path+"/ds9cat/ " +check +errlog;
  }
}
else{
/*
  if (viewer=="skycat"){
    execstr = scriptdir+"/pmon_skycat.sh '"+fitsFile->text()+"' "+check +" " +cat +errlog;
  }
  if (viewer=="ds9"){
     execstr = scriptdir+"/pmon_ds9.sh '"+fitsFile->text()+"' "+check +errlog;
  }
*/
   execstr = scriptdir+"/pmon_"+viewer +".sh '"+fitsFile->text()+"' "+check +errlog;

}
my_fscanf(execstr, result);

}
void pmonitor::show_plots()
{
QString execstr,result;
//execstr="eog " +sciencepath +"/plots/fgroups_1.png";
execstr=scriptdir + "pmon_astrom_showplots.sh " +sciencepath +"/plots/fgroups_1.png " +errlog;
my_fscanf(execstr, result);

}


void pmonitor::show_stats()
{
QString execstr,result;
btn_stats->setEnabled(false);  // during this script
//execstr="tail -n1 -q "+sciencepath +"/cat/skycat/*.skycat |cut -f1,4";
execstr=scriptdir +"pmon_astrom_stats.sh " + sciencepath +errlog;
my_fscanf(execstr, result);
btn_stats->setEnabled(true);
}


void pmonitor::show_protocol()
{
QString execstr,result;
// uses firefox as Browser
execstr=scriptdir +"/pmon_protocol_theli2.sh '"+ projectname->text() +"'" +errlog;
//QMessageBox::warning( this,"Debug-Info","Execstr: " +execstr );
my_fscanf(execstr, result); 
// reset Stamp for next run
//TouchStamp(mainpath);
}


void pmonitor::btn_cor_NOMAD_clicked()
{
QString execstr,result="",RED="",BLUE="";
execstr=scriptdir + "/pmon_get_corrected_photcat_calibr_NOMAD_red.sh " +sciencepath+"/color_theli/PHOTCAT_calibration/PHOTCAT_calibration.dat";
my_fscanf(execstr, result); 
//if (atoi(result) >0 && atoi(result) <5 ) RED=result;
RED=result;
execstr=scriptdir + "/pmon_get_corrected_photcat_calibr_NOMAD_blue.sh "+sciencepath+"/color_theli/PHOTCAT_calibration/PHOTCAT_calibration.dat";
my_fscanf(execstr, result); 
BLUE=result;
progress_status->setText(" Red: " +RED +" : Blue: " +BLUE +"(Corrected Nomad Colors)");
}

void pmonitor::newStamp()
{
// V 0.34: maybe obsolete after using set_reference-stamp(QString LOG)
    TouchStamp(mainpath);
    progress_info->setText("New start time set, repeat Theli process now!");
}


void pmonitor::btn_service_clicked()
{
QString execstr,result;
execstr=scriptdir +"pmon_service.sh " + sciencepath +errlog;
my_fscanf(execstr, result);
}


void pmonitor::btn_show_Bayer_clicked()
{
QString execstr,result;
execstr=scriptdir +"pmon_ds9-show-RGB.sh " + sciencepath +errlog;
my_fscanf(execstr, result);

}
