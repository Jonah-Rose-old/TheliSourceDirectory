#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <qapplication.h>
#include <qstring.h>
#include <qprocess.h>
#include <qprogressbar.h>
#include <qfont.h>
#include <qtimer.h>


// general
QString MAINGUIPATH="/home/jonah/theli/theli//";
QString bindir="/home/jonah/theli/theli/bin/Linux_64//";
QString scriptdir, guiversion, guiserverversion;
QString active, passive;

int ext_img[6];
int startup, unresolved, showversionwarning = 0;

QString ext_imgch;
QString process_group;
QString lastinstrument, lastchar;
QString cameratype="OPT";

int errcount, inst_prof_index, inst_comm_index, inst_user_index;
long errline;
float RAM;
int started_proc_pre, started_proc_cal, started_proc_sf, started_proc_w;
int started_proc_ap, started_proc_cc, started_proc_all, started_proc_del;
//int proc_ap_restoreheader_started, proc_ap_zeroheader_started;
int GUIstyle, invoked_fromexecall, fitsheaderfontsize;
QProcess *proc_pre, *proc_cal, *proc_sf, *proc_w, *proc_ap;
QProcess *proc_cc, *proc_all, *proc_del, *proc_progressbar;
QProcess *proc_resolve, *proc_dither;
//QProcess *proc_ap_restoreheader, proc_ap_zeroheader;
QStringList styles;
QString concatenate_logs, remove_individuallogs, opsys, bash;
QString qnumstyles, qcurrentstyle;
QString fullpathlog, numchips;
QString pid, homedir, startdir;
QString usertmpdir, userscriptdir, userthelidir;
QString userinstdir, profinstdir, comminstdir;
// QString astrorefdir, scriptdirfromfile;
QProgressBar *pb;
long process_id;
const QString empty = "";
QFont guifont, fitsheaderfont;
QTimer *t_cpumem;
QColor tfred = QColor(255,187,155);
QColor tfgreen = QColor(155,255,187);
QColor tfcyan = QColor(220,255,255);
QColor tfwhite = QColor(255,255,255);
QColor progressbarcolour;

bool datadiskspace_warned = false, homediskspace_warned = false;

// pre
int CB_sortrawdata, CB_processsplit, CB_createlinks;

// cal
int CB_processbias, CB_processdark, CB_processflat, CB_processflatoff;
int CB_processscience;
int science_ofc, sky_ofc;

// sf
int CB_spreadsequence, CB_background, CB_mergesequence;
int CB_collapse, CB_chopnod;
int science_background, sky_background;
int science_chopnod, sky_chopnod;
int science_coll, sky_coll;

// w
int CB_debloom, CB_globalweights, CB_binnedmosaics, CB_createweights;
int science_debloom, sky_debloom;
int sciencedirectphot, skydirectphot, CB_distributesets;

// ap
int CB_singleastrom, CB_applybestshift, CB_directphot;
int science_directphot, sky_directphot;
int CB_skysub, CB_astrometry, CB_photometry;
int CB_absphotom_direct, CB_absphotom_indirect;
bool do_directphot;

// cc
int CB_createcoadd, CB_createsexcat, CB_resolvelinks;

#endif
