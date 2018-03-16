#ifndef PMONITORFUNCTIONS_H
#define PMONITORFUNCTIONS_H

#include "functions.h"
#include <qstring.h>
#include "globalvariables.h"
/*
functions for pmonitor.ui.h
author: Carsten Moos
email:  cmoos@gmx.de
pmon version: >=0.35
date:    06.10.2012
*/

QString instrument, mainpath, sciencepath,ident;
void TouchStamp(QString path)
{
QString execstr;
   // create Timestamp
      execstr = "touch ";
      execstr.append(path);
      execstr.append("/pmon-stamp");
      my_system(execstr);
}
void Info(QString *info)
{
QString i=".....";


i.append ("ext_imgch: "+ext_imgch);
i.append (" process_group:" +process_group);
/*
i.append (" started_proc_pre:" +i.number(started_proc_pre));
i.append ("started_proc_cal:" +i.number(started_proc_cal));
i.append ("started_proc_sf:" +i.number(started_proc_sf));
i.append ("started_proc_w:" +i.number(started_proc_w));
i.append ("started_proc_ap:" +i.number(started_proc_ap));
i.append ("started_proc_cc:" +i.number(started_proc_cc));
*/
/*
i.append ("started_proc_all:" +i.number(started_proc_all));
i.append ("started_proc_del:" +i.number(started_proc_del));
i.append ("ext_img:" +i.number(ext_img[0]) +i.number(ext_img[1])+i.number(ext_img[2]));
i.append ("startup:" +i.number(startup));
i.append ("GUIstyle:" +i.number(GUIstyle));
*/
/*
int proc_ap_restoreheader_started, proc_ap_zeroheader_started;
QString fullpathlog, numchips;
QString pid, homedir, startdir;
QString usertmpdir, userscriptdir, userthelidir;
QString userinstdir, profinstdir, comminstdir;
QString astrorefdir, scriptdirfromfile;
long process_id;
*/


*info=i;

}

void get_paths()
{
// Paths & Co
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
      mainpath = settings.readEntry("/maindir");
      sciencepath = settings.readEntry("/maindir");
      sciencepath.append(settings.readEntry("/sciencedir"));
      //editor = settings.readEntry("/editor");
      //if (strcmp(editor,"vi") == 0)
	//editor = "xterm -e vi ";
      settings.endGroup();
    settings.endGroup();
    // get coadd id
      settings.beginGroup("/theli_"+lastlog+"_");
      settings.beginGroup("/COADDITION");
      settings.beginGroup("/coaddition");
      ident = settings.readEntry("/scriptid", "");
      settings.endGroup();
//QMessageBox::warning( this, "Debuginfo",  "Ident: " +ident);   
}

QString get_ext(){
int col=0;
// idx: 0 1 2 3 4 5 6 7
//    OFC S F U H C D P
char CEXT[8]="       ";
QString EXT="";
if ( ext_img[0] ) EXT="OFC";
CEXT[0]='\0';
if ( ext_img[1] ) CEXT[col++]='B';
CEXT[1]='\0';
if ( ext_img[2] ) CEXT[col++]='H';
CEXT[2]='\0';
if ( ext_img[3] ) CEXT[col++]='C';
CEXT[3]='\0';
if ( ext_img[4] ) CEXT[col++]='D';
CEXT[4]='\0';
if ( ext_img[5] ) CEXT[col++]='P';
CEXT[5]='\0';
EXT.append(CEXT);
if (EXT=="" ) EXT ="OFC";   // fall back, ext_imgch not always correct???
return(EXT);
}

int  get_BAYER(QString path){
int mask=0;
      QString execstr, result="";
      execstr = "dfits ";
      execstr.append(path);
      execstr.append("/*_?.fits 2>/dev/null | grep BAYERPAT |head -n1|cut -d= -f1");
      my_fscanf(execstr,result);
// liefert nur einen Teilstring, das RGGB fehlt immer
      //execstr = "getkey `ls ";
      //execstr.append(path);
      //execstr.append("/*_?.fits 2>/dev/null |head -n1` BAYERPAT");
      //my_fscanf(execstr,result);
      if (result =="BAYERPAT" ) mask=1;  // no empty mask => RGB
      return(mask);
}
// Count all calibrated files
int count1(QString search){
QString result="", execstr;
//SEARCH_PATH_PATTERN=sciencepath + "/*" +EXTstatus +".fits";
execstr = "ls ";
execstr.append(search);
execstr.append(" 2>/dev/null|wc -l");
my_fscanf(execstr, result);
return (atoi(result));
}
// Count all original files
int count2(QString path){
QString result="", execstr;
//path=sciencepath
execstr = "{ ls " +path +"/*.* 2>/dev/null|wc -l; ls " +path +"/*_?.fits 2>/dev/null|wc -l; ";
execstr.append( "ls " +path +"/*_??.fits 2>/dev/null|wc -l;  ls " +path +"/*.tif 2>/dev/null|wc -l; } | paste -s|gawk '{print $1-$2-$3-$4}'");  //z.B. file1_1.fits, file27_12.fits
my_fscanf(execstr, result);
return (atoi(result));
}
// Count all preprocessed files
int count3(QString path){
QString result="", execstr;
// remaining ORIGINALS WITH _?.fits  and _??.fits are contained
execstr = "{ ls " +path +"/*_?.fits 2>/dev/null; ";
execstr.append( "ls " +path +"/*_??.fits 2>/dev/null; }");  //z.B. file1_1.fits, file27_12.fits
execstr.append(" |wc -l");
my_fscanf(execstr, result);
return (atoi(result));
}

int get_pid(QString processname)
{
int pid;
QString execstr,result="";
/*
basics for processmonitoring
processid=`pgrep -U $USER $process|head -n1`

process="process_split"
#process="process_bias"
#process="process_flat"
process="process_science"
process="create_weights"
#process="sex"
process="create_astromcats_para"
#process="create_scamp"
process="create_skysub"
process="resample_coadd"
#process="resample_filtercosmics"
#process="swarp"
*/
if ( processname != "" ){
  //execstr="if pgrep -U $USER -f " + processname +">/dev/null; then pgrep -U $USER -f " + processname +"|grep -v pgrep|head -n1;else echo 0; fi";
  //execstr="pgrep -U $USER " + processname +"|head -n1";
  execstr=scriptdir +"pmon_getpid.sh " + processname;
  my_fscanf(execstr, result);
}

if (result != "") 
    pid=atoi(result);
else
    pid=0;

return (pid);
}

void set_reference_stamp(QString LOG)
{
// sets the timestamp of ref-file to pmon-stamp
/*
useful to synchronize for with log-file
   1. weighting			.theli/scripts/theli_w_comm.sh.log
   2. create source cat		.theli/scripts/theli_ap_comm.sh.log
   3. skysub			.theli/scripts/theli_cc_comm.sh.log
   4. resamp			.theli/scripts/theli_cc_comm.sh.log
log file is updated by theli *AFTER* job is done,
sh  file is craeted by theli *BEFORE* job is started
so we set this refrence stamp once by monitor start watching one this 4 jobs
*/
QString execstr,result="";
if (LOG!=""){
  execstr="touch -r $HOME/.theli/scripts/theli_" + LOG +"_comm.sh.log " + mainpath +"/pmon-stamp 2>/dev/null";
  my_fscanf(execstr, result);
}
}

int get_buttonID(){
int ID=0;
QSettings settings (QSettings::Ini);
QString cc_reductionlog;

  	settings.beginGroup("/theligeneral_");
  	cc_reductionlog = settings.readEntry("/lastlog", "noname");
  	settings.endGroup();
	settings.beginGroup("/theli_"+cc_reductionlog+"_");
	settings.beginGroup("/COADDITION");
	settings.beginGroup("/skysub");
	//modelID = settings.readNumEntry("/modelradiobuttonID", 0);
	ID=settings.readNumEntry("/radiobuttonID", 0);
        /*manual = settings.readEntry("/manual", "");
        skychipnum = settings.readNumEntry("/chip", 0);
        */
 	settings.endGroup();
	settings.endGroup();
return (ID);

}

QString get_viewer(){
QSettings settings (QSettings::Ini);
QString cc_reductionlog;
QString fitsviewer="skycat";
settings.beginGroup("/theligeneral_");
cc_reductionlog = settings.readEntry("/lastlog", "noname");
settings.endGroup();
settings.beginGroup("/theli_"+cc_reductionlog+"_");
settings.beginGroup("/INITIALISE");
fitsviewer = settings.readEntry("/viewer", "skycat");
settings.endGroup();
settings.endGroup();
return (fitsviewer);
}

void put_sound(){
QString execstr,result="";
  execstr=scriptdir +"pmon_putsound.sh ";
  my_fscanf(execstr, result);
}

#endif
