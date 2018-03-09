#include <qapplication.h>
#include <qpixmap.h>
#include <qsplashscreen.h>
#include <qimage.h>
#include <qsettings.h>
#include "theliform.h"
#include "functions.h"
#include "image.h"
#include "stdio.h"
#include "stdlib.h"
#include "globalvariables.h"
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    
    FILE *pipe, *stream;
    char tmp[1000];
    QString user, qtowner, result, pid, scriptdir, line, execstr;
    QSettings settings (QSettings::Ini);
    QStringList list;
    pid_t processid;

    scriptdir = MAINGUIPATH;
    scriptdir.append("gui/scripts/");

    // check if scamp is present
    QString scamp;
    scamp = bindir+"/scamp";
    QFile scampfile(scamp);
    if (!scampfile.exists()) {
      scamp = bindir+"/scamp.exe";
      QFile scampfile(scamp);
      if (scampfile.exists()) {
	// remove the windows .exe suffix some browsers append
	my_system("mv "+bindir+"/scamp.exe "+bindir+"/scamp");
	scamp = bindir+"/scamp";
      }
      else {
      	printf("\n\n");
	printf("   ===================================================================\n\n");
	printf("   Could not find the 'scamp' executable in:\n");
	printf("   %s\n\n", bindir.ascii());
	printf("   You can obtain the latest version of scamp from:\n");
	printf("   http://www.astromatic.net/software/scamp\n\n");
	printf("   ====================================================================\n\n");
	exit (1);
      }
    }

    // check if scamp is executable
    QString executable;
    execstr = "ls -l "+scamp+" | gawk '{print $1}'";
    my_fscanf(execstr, result);
    executable = result.at(3);
    if (executable.compare("x") != 0) {
      // try and make it executable
      execstr = "chmod +x "+scamp+" &> /dev/null";
      my_system(execstr);
      // check if successful
      execstr = "ls -l "+scamp+" | gawk '{print $1}'";
      my_fscanf(execstr, result);
      executable = result.at(3);
      if (executable.compare("x") != 0) {
      	printf("\n\n");
	printf("   ===================================================================\n\n");
	printf("   The 'scamp' programme in:\n");
	printf("   %s\n\n", bindir.ascii());
	printf("   is not executable, and THELI could not make it executable either.\n");
	printf("   Probably you do not have file permissions to make these changes.\n");
      	printf("   Contact your SysAdmin or the person who installed THELI.\n");
	printf("   ====================================================================\n\n");
	exit (1);
      }
    }

    // check if another instance of theli is run by the same user

    // who launches theli?
    if (( pipe = popen("id -un", "r")) == NULL) {
        perror("popen");
        exit (1);
    }
    if (fscanf(pipe, "%s", tmp) == 0) {
        printf("\tError: Could not read user name!\n");
        user = "";
    }
    else user = tmp;
    pclose(pipe);

    // is theli already running by this user?

    /* get the process id */
    if ((processid = getpid()) < 0) {
      perror("Unable to get process ID for this instance of THELI!");
      exit (1);
    }
    
    execstr = "ps -u "+user;
    stream = popen(execstr, "r");
    QTextStream ts(stream, IO_ReadOnly);
    while (! ts.atEnd()) {
      line = ts.readLine();
      if (line.endsWith(" theli")) {
	list = list.split(" ", line);
	QStringList::Iterator it = list.begin();
	pid = *it;
	if (pid.toInt() != processid) {
	  printf("You are already running THELI (process ID: %s)\n", pid.ascii());
	  printf("Only one instance of theli can be run per user at the same time.\n\n");
	  exit (0);
	}
      }
    }
    pclose(stream);


    // check if ${HOME}/.qt is writable
    QString writable;
    execstr = "ls -ld ${HOME}/.qt | gawk '{print $3}'";
    my_fscanf(execstr, qtowner);
    if (qtowner.compare(user) != 0) {
      // .qt not owned by user
      execstr = "ls -ld ${HOME}/.qt | gawk '{print $1}'";
      my_fscanf(execstr, result);
      writable = result.at(5);
      if (writable.compare("w") != 0) {
      	printf("\n\n");
	printf("   ===========================================================================\n\n");
	printf("   The .qt directory in your home directory is owned by 'root' and not by you.\n");
	printf("   You need write permission in this directory, otherwise THELI will not work\n");
	printf("   properly. To change ownership, do\n\n");
      	printf("   sudo chown %s -R ~/.qt\n", user.ascii());
	printf("   ===========================================================================\n\n");
	exit (1);
      }
    }

    // read the splashscreen image from image.h
    QImage splashimage;
    splashimage.loadFromData(image, len);

    // convert the image to a pixmap
    QPixmap splashpixmap;
    splashpixmap.convertFromImage(splashimage, 0);

    // show the splashscreen
    QSplashScreen *splash = new QSplashScreen( splashpixmap );
    splash->show();
   
    // show the main GUI window
    theliForm w;
    settings.beginGroup("/theligeneral_");
      int width = settings.readNumEntry("/guiwidth", 900);
      int height = settings.readNumEntry("/guiheight", 700);
    settings.endGroup();
    w.resize(width, height);
    w.show();

    delete splash;
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    return app.exec();
}
