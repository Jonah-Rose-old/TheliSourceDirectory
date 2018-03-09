#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qvalidator.h>
#include <qsettings.h>
#include "functions.h"

QString homedir_ch, lastlog;

void commhist::init()
{
    QString line, filename, tmp;
    QSettings settings (QSettings::Ini);

    // get general info
    settings.beginGroup("/theligeneral_");
      lastlog = settings.readEntry("/lastlog", "noname");
      homedir_ch = settings.readEntry("/homedir", "");
    settings.endGroup();

    // update the title
    tmp = "Command history for LOG :  ";
    tmp.append(lastlog);
    ch_titleTextLabel->setText(tmp);
    
    // read the command history
    filename = homedir_ch;
    filename.append("/.theli/reduction_logs/");
    filename.append(lastlog);
    filename.append(".com");
    
    QFile file( filename );
    if ( file.open( IO_ReadOnly )) {
	QTextStream stream( &file );
	while ( !stream.atEnd() ) {
	    line = stream.readLine();
	    ch_commandhistoryTextEdit->append(line);
	}	
    }
}


void commhist::clear_history()
{
    QString filename, execstr;
    QSettings settings (QSettings::Ini);
    
    // clear the text window
    ch_commandhistoryTextEdit->clear();
    
    // get the history name
    filename = homedir_ch;
    filename.append("/.theli/reduction_logs/");
    filename.append(lastlog);
    filename.append(".com");
  
    // delete the history
    QFile file (filename);
    file.remove();
  
    // touch the history
    execstr = "touch ";
    execstr.append(filename);
    my_system(execstr);
}


void commhist::save_history()
{
    FILE *of;
    
    QString line, value, ident, reductionlog, maindir;
    QString currentFileName, scripts;
    QSettings settings (QSettings::Ini);

    // get general info
    settings.beginGroup("/theli_"+lastlog+"_");
    settings.beginGroup("/INITIALISE");
    maindir = settings.readEntry("/maindir", homedir_ch);
    
    currentFileName = QFileDialog::getSaveFileName(
	    maindir,
	    "Command histories (*.com)",
	    this,
	    "save file dialog",
	    "Enter name for the command history");
    
    if ( currentFileName.isEmpty() )
	return;

    if ( !currentFileName.isEmpty()) {
	int answer = 0;
	// Print warning if an existing file is going to be overwritten.
	if ( QFile::exists( currentFileName ))
	    answer = QMessageBox::warning(
		    this, "Overwrite File",
		    QString( "<b>WARNING:</b> \n\nYou are going to overwrite\n\n\'%1\'?" ).
		    arg( currentFileName ),
		    "&OK", "&Cancel", QString::null, 1, 1 );
	// Save file otherwise
	if ( answer == 0 ) {
	    
	    // force a ".com" suffix for the history file name
	    if (!currentFileName.endsWith(".com")) 
		currentFileName.append(".com");
	    
	    if ((of = fopen(currentFileName, "w")) != NULL) {
		scripts = ch_commandhistoryTextEdit->text();
		fprintf(of, "%s\n", scripts.ascii());
		fclose(of);
	    }	
	    return;	
	}
    }
}
