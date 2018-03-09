#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <qapplication.h>
#include <qtimer.h>
#include <qprocess.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextedit.h>
#include <qsettings.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <unistd.h>
#include "globalvariables.h"


void remove_fits(QString directory)
{
  QDir dir = QDir(directory);
  QStringList fitslist = dir.entryList("*.fits");

  for ( QStringList::Iterator it = fitslist.begin(); it != fitslist.end(); ++it ) {
    QFile image(directory+"/"+(*it));
    image.remove();
  }
}


QString get_instpath()
{
  QSettings settings(QSettings::Ini);
  QString instrument, instpath, lastlogfile;
  
  settings.beginGroup("/theligeneral_");
    lastlogfile = settings.readEntry("/lastlog", "noname");
  settings.endGroup();

  settings.beginGroup("/theli_"+lastlogfile+"_");
    settings.beginGroup("/INITIALISE");
    instrument = settings.readEntry ("/instrumentname");
    if (settings.readBoolEntry("/inst_prof",true)) {
      instpath = scriptdir + "/instruments_professional/";
    }
    if (settings.readBoolEntry("/inst_comm",true)) {
      instpath = scriptdir + "/instruments_commercial/";
    }
    if (settings.readBoolEntry("/inst_user",true)) {
      instpath = homedir + "/.theli/instruments_user/";
    }
    settings.endGroup();
  settings.endGroup();
  
  return instpath+"/"+instrument+".ini";
}


QString get_processingstatus()
{
  QString status = "";
  if (ext_img[0] == 1) {
    status.append("OFC");
    if (ext_img[1] == 1) status.append("B");
    if (ext_img[2] == 1) status.append("H");
    if (ext_img[3] == 1) status.append("C");
    if (ext_img[4] == 1) status.append("D");
    if (ext_img[5] == 1) status.append("P");
  }

  return status;
}


int get_max_ncpu() {
  long nprocs = 1;
  nprocs = sysconf(_SC_NPROCESSORS_ONLN);
  if (nprocs < 1) nprocs = 1;
  return (nprocs);
}


void warn(QTextEdit *commandwindow, QString warning)
{
  commandwindow->append("<font color=\"red\">"+warning+"</font>");
  commandwindow->setReadOnly(true);
}


void note(QTextEdit *messagewindow, QString note)
{
  messagewindow->append("<font color=#005522>"+note+"</font>");
}


void notered(QTextEdit *messagewindow, QString note)
{
  messagewindow->append("<font color=#cc0000>"+note+"</font>");
}


void my_fscanf(QString shellcommand, QString &result)
{
  FILE *pipe;
  char tmp[1000]; 
  
  result = "";

  if (( pipe = popen(shellcommand, "r")) == NULL) {
    perror("popen");
    exit (1);
  }	
  if (fscanf(pipe, "%s", tmp) == 0) {
    printf("\tError: Could not read value from stdin!\n");
    result = "";
  }
  else result = tmp;
  pclose(pipe);
}


void my_fscanf2(QString shellcommand, QString &result1, QString &result2)
{
  FILE *pipe;
  char tmp[1000], tmp2[1000]; 
  
  if (( pipe = popen(shellcommand, "r")) == NULL) {
    perror("popen");
    exit (1);
  }	
  if (fscanf(pipe, "%s %s", tmp, tmp2) == 0) {
    result1 = "";
    result2 = "";
  }
  else {
    result1 = tmp;
    result2 = tmp2;
  }
  pclose(pipe);
}


void my_fscanf3(QString shellcommand, QString &result1, QString &result2, QString &result3)
{
  FILE *pipe;
  char tmp[1000], tmp2[1000], tmp3[1000]; 
  
  if (( pipe = popen(shellcommand, "r")) == NULL) {
    perror("popen");
    exit (1);
  }	
  if (fscanf(pipe, "%s %s %s", tmp, tmp2, tmp3) == 0) {
    result1 = "";
    result2 = "";
    result3 = "";
  }
  else {
    result1 = tmp;
    result2 = tmp2;
    result3 = tmp3;
  }
  pclose(pipe);
}


int count_files(QString dirname, QString extension)
{
  QDir dir = QDir(dirname);
  QStringList list;
  
  list = dir.entryList("*_1"+extension+".fits");
  
  return (list.count());
}


void my_fgets(QString shellcommand, QString &result)
{
  FILE *pipe;
  char tmp[1000]; 
  
  if (( pipe = popen(shellcommand, "r")) == NULL) {
    perror("popen");
    exit (1);
  }	
  if (fgets(tmp, 1000, pipe) == NULL) {
    result = "";
  }
  else result = tmp;
  pclose(pipe);
}


void my_system(QString execstr)
{
  if (system(execstr) == -1)
    printf("\tError: Could not execute shell command!\n");
}


void get_fileparameter(QString file, QString keyname, QString &value)
{
  QString execstr = "gawk 'BEGIN{FS=\"=\"} ($1==\"";
  execstr.append(keyname);
  execstr.append("\") {print $2}' ");
  execstr.append(file);
  
  my_fscanf(execstr, value);
}


void set_fileparameter(QString paramfile, QString parameter, QString value)
{
  FILE *of;
  QString filetmp = paramfile+"_tmp";
  QString lineini, line, ident, execstr;
  
  QFile file( paramfile );
  if ( file.open( IO_ReadOnly )) {
    of = fopen(filetmp, "w");
    QTextStream stream( &file );
    while ( !stream.atEnd() ) {
      lineini = stream.readLine();
      line=lineini.simplifyWhiteSpace();
      if (!line.isEmpty()) {
	ident = line.section('=',0,0);
	if (ident.compare(parameter) != 0)
	  fprintf(of, "%s\n", line.ascii());
	else {
	  fprintf(of, "%s=%s\n", ident.ascii(), value.ascii());
	}
      } else
	fprintf(of, "\n");
    }
    file.close();
    fclose(of);
  }
  
  if (file.exists()) { 
    // suppress a warning if theli is run the very first time
    execstr = "mv ";
    execstr.append(filetmp);
    execstr.append(" ");
    execstr.append(paramfile);
    my_system(execstr);
  }
}


void get_fileparameter2(QString file, QString keyname, QString &value)
{
  QString execstr = "gawk 'BEGIN{FS=\"=\"} ($1==\"";
  execstr.append(keyname);
  execstr.append("\") {print $3}' ");
  execstr.append(file);
  execstr.append(" | gawk '{print $1}'");
  
  my_fscanf(execstr, value);
}


void get_fitsext(int *array, QString &result)
{
  result = "";
  if (array[0] == 1) result.append("OFC");
  if (array[1] == 1) result.append("B");
  if (array[2] == 1) result.append("H");
  if (array[3] == 1) result.append("C");
  if (array[4] == 1) result.append("D");
  if (array[5] == 1) result.append("P");
}


void check_last_char(QString &string)
{
  if (!string.endsWith("/")) string.append("/");
}


void drop_last_char(QString &string)
{
  int len;
  
  if (!string.isEmpty()) {
    len = string.length() - 1;
    string.truncate(len);
  }
}


long get_numfiles_indir(QString directory, QString filter)
{
  QDir dir;
  
  dir = QDir(directory);
  
  if (!dir.exists() || directory.isEmpty()) 
    return 0;

  if (filter.isEmpty()) 
    filter = "*.fits";

  return dir.entryList(filter).count();
}


int checkdir(QString path1, QString path2)
{
  QDir e;
  int result;
  
  check_last_char(path1);
  QString existdirectory = path1;
  existdirectory.append(path2);
  e.setPath(existdirectory);
  if (e.exists() && !path2.isEmpty()) result = 1;
  else result = 0;
  
  return (result);  
}


int checkdir2(QString path)
{
  QDir e;
  int result;
  
  if (!path.isEmpty()) { 
    e.setPath(path);
    if (e.exists()) result = 1;
    else result = 0;
  } else result = 0;
  
  return (result);  
}


int checkfile(QString pathfile)
{
  QFile f;
  
  f.setName(pathfile);
  if (f.exists() && !pathfile.isEmpty()) return 1;
  else return 0;
}


void create_scriptcall_0(QTextEdit *textedit, int para, QString task, QString path)
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  textedit->append(call);
}


void create_scriptcall_1(QTextEdit *textedit, int para, QString task, QString path, 
			 QString ext1 )
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  textedit->append(call);
}


void create_scriptcall_split(QTextEdit *textedit, int para, QString task, QString path, 
			 QString ext1 )
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  textedit->append(call);
}


void create_scriptcall_2(QTextEdit *textedit, int para, QString task, QString path, 
			 QString ext1, QString ext2 )
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh "; 
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  call.append(" ");
  call.append(ext2);
  textedit->append(call);
}

void create_scriptcall_3ap(QString &call, int para, QString task, QString path, 
			   QString ext1, QString ext2, QString catpath)
{
  call = "";
  if (para == 1) call = "./parallel_manager.sh "; 
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  call.append(" ");
  call.append(ext2);
  call.append(" ");
  call.append(catpath);
}


void create_scriptcall_3(QTextEdit *textedit, int para, QString task, 
			  QString path, QString ext1, QString ext2, 
			  QString ext3 )
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  call.append(" ");
  call.append(ext2);
  call.append(" ");
  call.append(ext3);
  textedit->append(call);
}


void create_scriptcall_4(QTextEdit *textedit, int para, QString task, QString path, 
			 QString ext1, QString ext2, QString ext3, 
			 QString ext4 )
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  call.append(" ");
  call.append(ext2);
  call.append(" ");
  call.append(ext3);
  call.append(" ");
  call.append(ext4);
  textedit->append(call);
}


void create_scriptcall_5(QTextEdit *textedit, int para, QString task, QString path, 
			 QString ext1, QString ext2, QString ext3, 
			 QString ext4, QString ext5 )
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  call.append(" ");
  call.append(ext2);
  call.append(" ");
  call.append(ext3);
  call.append(" ");
  call.append(ext4);
  call.append(" ");
  call.append(ext5);
  textedit->append(call);
}


void create_scriptcall_6(QTextEdit *textedit, int para, QString task, QString path, 
			 QString ext1, QString ext2, QString ext3, 
			 QString ext4, QString ext5, QString ext6 )
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  call.append(" ");
  call.append(ext2);
  call.append(" ");
  call.append(ext3);
  call.append(" ");
  call.append(ext4);
  call.append(" ");
  call.append(ext5);
  call.append(" ");
  call.append(ext6);
  textedit->append(call);
}


void create_scriptcall_8(QTextEdit *textedit, int para, QString task, QString path, 
			 QString ext1, QString ext2, QString ext3, 
			 QString ext4, QString ext5, QString ext6, 
			 QString ext7, QString ext8)
{
  QString call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  call.append(" ");
  call.append(ext2);
  call.append(" ");
  call.append(ext3);
  call.append(" ");
  call.append(ext4);
  call.append(" ");
  call.append(ext5);
  call.append(" ");
  call.append(ext6);
  call.append(" ");
  call.append(ext7);
  call.append(" ");
  call.append(ext8);
  textedit->append(call);
}


void create_scriptcall_4ap(QString &call, int para, QString task, QString path, 
			 QString ext1, QString ext2, QString ext3, QString ext4)
{
  call = "";
  if (para == 1) call = "./parallel_manager.sh ";
  else call = "./";
  call.append(task);
  call.append(" ");
  call.append(path);
  call.append(" ");
  call.append(ext1);
  call.append(" ");
  call.append(ext2);
  call.append(" ");
  call.append(ext3);
  call.append(" ");
  call.append(ext4);
}


int sum(int *array, int dim) {
  int i, total;
  
  total = 0;
  for (i=0;i<dim;i++) total += array[i];
  
  return (total);
}


bool test_network()
{

  return(true);
  
  // this should be replaced with something that returns more quickly.
  
  QString line;
  QString command = bindir+"/aclient 130.79.129.161 1660 find2m -help";
  command.append(" > "+usertmpdir+"/networktest 2>&1");
  my_system(command);
  
  QFile network(usertmpdir+"/networktest");
  if ( network.open( IO_ReadOnly )) {
    QTextStream stream( &network );
    while ( !stream.atEnd() ) {
      line = stream.readLine();
      if (line.contains("Network is unreachable") > 0) {
	network.close();
	return(false);
      }
    }
  }		
  return(true);
}


void getguiversion()
{
  QString log;

  QString execstr = "wget -T 3 -t 1 -O ~/.theli/scripts/VERSION http://www.astro.uni-bonn.de/~theli/gui/download/VERSION -o ~/.theli/scripts/wget.log";
  my_system(execstr);

  QFile file (homedir+"/.theli/scripts/VERSION");
  if (file.size() == 0) {
    guiserverversion = "";
  }
  else {
    execstr = "grep \".theli/scripts/VERSION' saved\" ~/.theli/scripts/wget.log";
    my_fscanf(execstr, log);
    
    if (!log.isEmpty()) {
      execstr = "cat ~/.theli/scripts/VERSION";
      my_fscanf(execstr, guiserverversion);
    }
    else guiserverversion = "";
  }
}

#endif
