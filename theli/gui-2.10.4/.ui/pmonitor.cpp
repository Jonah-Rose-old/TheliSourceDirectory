/****************************************************************************
** Form implementation generated from reading ui file 'pmonitor.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "pmonitor.h"

#include <qvariant.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qprogressbar.h>
#include <qframe.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../pmonitor.ui.h"
/*
 *  Constructs a pmonitor as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
pmonitor::pmonitor( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "pmonitor" );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );

    progress_listBox = new QListBox( this, "progress_listBox" );
    progress_listBox->setGeometry( QRect( 0, 10, 151, 170 ) );
    QFont progress_listBox_font(  progress_listBox->font() );
    progress_listBox_font.setPointSize( 11 );
    progress_listBox->setFont( progress_listBox_font ); 

    progress_resetpushButton = new QPushButton( this, "progress_resetpushButton" );
    progress_resetpushButton->setGeometry( QRect( 160, 50, 100, 30 ) );
    progress_resetpushButton->setPaletteForegroundColor( QColor( 207, 0, 0 ) );
    QFont progress_resetpushButton_font(  progress_resetpushButton->font() );
    progress_resetpushButton_font.setPointSize( 11 );
    progress_resetpushButton->setFont( progress_resetpushButton_font ); 

    progress_startpushButton = new QPushButton( this, "progress_startpushButton" );
    progress_startpushButton->setGeometry( QRect( 160, 10, 100, 30 ) );
    progress_startpushButton->setPaletteForegroundColor( QColor( 0, 138, 0 ) );
    QFont progress_startpushButton_font(  progress_startpushButton->font() );
    progress_startpushButton_font.setPointSize( 11 );
    progress_startpushButton->setFont( progress_startpushButton_font ); 

    progress_startpushButton_newstamp = new QPushButton( this, "progress_startpushButton_newstamp" );
    progress_startpushButton_newstamp->setGeometry( QRect( 280, 50, 30, 30 ) );
    progress_startpushButton_newstamp->setPaletteForegroundColor( QColor( 0, 138, 0 ) );
    QFont progress_startpushButton_newstamp_font(  progress_startpushButton_newstamp->font() );
    progress_startpushButton_newstamp_font.setPointSize( 11 );
    progress_startpushButton_newstamp->setFont( progress_startpushButton_newstamp_font ); 
    progress_startpushButton_newstamp->setPixmap( QPixmap::fromMimeSource( "undo" ) );

    btn_service = new QPushButton( this, "btn_service" );
    btn_service->setGeometry( QRect( 10, 610, 140, 31 ) );
    btn_service->setPaletteForegroundColor( QColor( 0, 138, 0 ) );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 1, 300, 330, 20 ) );
    textLabel1_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    textLabel1_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    projectname = new QLineEdit( this, "projectname" );
    projectname->setGeometry( QRect( 120, 330, 210, 31 ) );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 10, 330, 100, 31 ) );
    textLabel1->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );

    progress_status = new QLineEdit( this, "progress_status" );
    progress_status->setGeometry( QRect( 0, 210, 340, 31 ) );
    QFont progress_status_font(  progress_status->font() );
    progress_status_font.setPointSize( 11 );
    progress_status->setFont( progress_status_font ); 
    progress_status->setReadOnly( TRUE );

    progressBar = new QProgressBar( this, "progressBar" );
    progressBar->setGeometry( QRect( 0, 250, 340, 40 ) );

    progress_info = new QLineEdit( this, "progress_info" );
    progress_info->setGeometry( QRect( 161, 90, 180, 31 ) );
    QFont progress_info_font(  progress_info->font() );
    progress_info_font.setPointSize( 11 );
    progress_info->setFont( progress_info_font ); 
    progress_info->setReadOnly( FALSE );

    btn_plots = new QPushButton( this, "btn_plots" );
    btn_plots->setGeometry( QRect( 180, 530, 160, 31 ) );
    btn_plots->setPaletteForegroundColor( QColor( 0, 138, 0 ) );

    btn_protocol = new QPushButton( this, "btn_protocol" );
    btn_protocol->setGeometry( QRect( 180, 610, 160, 31 ) );
    btn_protocol->setPaletteForegroundColor( QColor( 0, 138, 0 ) );

    frame3 = new QFrame( this, "frame3" );
    frame3->setGeometry( QRect( 0, 370, 340, 140 ) );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );
    frame3->setLineWidth( 3 );

    chk_blink = new QCheckBox( frame3, "chk_blink" );
    chk_blink->setGeometry( QRect( 10, 100, 80, 20 ) );

    fitsFile = new QLineEdit( frame3, "fitsFile" );
    fitsFile->setGeometry( QRect( 120, 20, 210, 30 ) );

    textLabel2 = new QLabel( frame3, "textLabel2" );
    textLabel2->setGeometry( QRect( 10, 20, 100, 31 ) );
    textLabel2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );

    blinkInterval = new QSpinBox( frame3, "blinkInterval" );
    blinkInterval->setEnabled( FALSE );
    blinkInterval->setGeometry( QRect( 110, 100, 70, 20 ) );
    blinkInterval->setMaximumSize( QSize( 1000, 1000 ) );
    blinkInterval->setMaxValue( 10 );
    blinkInterval->setMinValue( 1 );

    chk_cat = new QCheckBox( frame3, "chk_cat" );
    chk_cat->setGeometry( QRect( 200, 100, 130, 20 ) );

    btn_fits = new QPushButton( frame3, "btn_fits" );
    btn_fits->setGeometry( QRect( 120, 60, 210, 31 ) );
    btn_fits->setPaletteForegroundColor( QColor( 0, 138, 0 ) );

    btn_show_Bayer = new QPushButton( this, "btn_show_Bayer" );
    btn_show_Bayer->setGeometry( QRect( 10, 530, 140, 31 ) );
    btn_show_Bayer->setPaletteForegroundColor( QColor( 0, 138, 0 ) );

    btn_cor_NOMAD = new QPushButton( this, "btn_cor_NOMAD" );
    btn_cor_NOMAD->setGeometry( QRect( 10, 570, 140, 31 ) );
    btn_cor_NOMAD->setPaletteForegroundColor( QColor( 0, 138, 0 ) );

    btn_stats = new QPushButton( this, "btn_stats" );
    btn_stats->setGeometry( QRect( 180, 570, 160, 30 ) );
    btn_stats->setPaletteForegroundColor( QColor( 0, 138, 0 ) );
    languageChange();
    resize( QSize(347, 682).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( progress_startpushButton, SIGNAL( clicked() ), this, SLOT( p_start() ) );
    connect( progress_resetpushButton, SIGNAL( clicked() ), this, SLOT( p_reset() ) );
    connect( btn_fits, SIGNAL( clicked() ), this, SLOT( show_fits() ) );
    connect( btn_plots, SIGNAL( clicked() ), this, SLOT( show_plots() ) );
    connect( btn_stats, SIGNAL( clicked() ), this, SLOT( show_stats() ) );
    connect( btn_protocol, SIGNAL( clicked() ), this, SLOT( show_protocol() ) );
    connect( btn_cor_NOMAD, SIGNAL( clicked() ), this, SLOT( btn_cor_NOMAD_clicked() ) );
    connect( progress_startpushButton_newstamp, SIGNAL( clicked() ), this, SLOT( newStamp() ) );
    connect( btn_service, SIGNAL( clicked() ), this, SLOT( btn_service_clicked() ) );
    connect( btn_show_Bayer, SIGNAL( clicked() ), this, SLOT( btn_show_Bayer_clicked() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
pmonitor::~pmonitor()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void pmonitor::languageChange()
{
    setCaption( tr( "Progress Monitor" ) );
    progress_listBox->clear();
    progress_listBox->insertItem( tr( "New Item" ) );
    QToolTip::add( progress_listBox, tr( "Process list: Automatic= looks for running process group. <BR>  or manually select progress to be monitored, then use start" ) );
    progress_resetpushButton->setText( tr( "&Reset" ) );
    progress_resetpushButton->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    QToolTip::add( progress_resetpushButton, tr( "stops monitoring, sets automatic, reads paths" ) );
    progress_startpushButton->setText( tr( "&Start" ) );
    progress_startpushButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    QToolTip::add( progress_startpushButton, tr( "reads process list, sets new timestamp, starts monitoring" ) );
    progress_startpushButton_newstamp->setText( QString::null );
    progress_startpushButton_newstamp->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( progress_startpushButton_newstamp, tr( "For new Timestamp if theli repeats a process" ) );
    btn_service->setText( tr( "export results" ) );
    QToolTip::add( btn_service, tr( "call service script for export tiffs" ) );
    textLabel1_2->setText( tr( "   Result Monitor" ) );
    projectname->setText( tr( "projectname" ) );
    QToolTip::add( projectname, tr( "type in your projectname for the protocol" ) );
    textLabel1->setText( tr( "project title:" ) );
    QToolTip::add( progress_status, tr( "Counter: current / total, Time: running /estimated total" ) );
    QWhatsThis::add( progress_status, QString::null );
    QToolTip::add( progressBar, tr( "actually progress" ) );
    QToolTip::add( progress_info, tr( "Status" ) );
    btn_plots->setText( tr( "show plots" ) );
    QToolTip::add( btn_plots, tr( "call imageviewer to show the astrometry plots" ) );
    btn_protocol->setText( tr( "show protocol" ) );
    QToolTip::add( btn_protocol, tr( "shows a processing protocol in your browser" ) );
    chk_blink->setText( tr( "blink all" ) );
    QToolTip::add( chk_blink, tr( "Unchecked: single Checked all images" ) );
    QToolTip::add( fitsFile, tr( "fitsfile with path to show" ) );
    textLabel2->setText( tr( "selected fits:" ) );
    QToolTip::add( blinkInterval, tr( "Interval for blinking in seconds" ) );
    chk_cat->setText( tr( "with catalogue" ) );
    QToolTip::add( chk_cat, tr( "show fits with catalog" ) );
    btn_fits->setText( tr( "show fits" ) );
    QToolTip::add( btn_fits, tr( "call fitsviewer" ) );
    btn_show_Bayer->setText( tr( "show Bayer-RGB" ) );
    QToolTip::add( btn_show_Bayer, tr( "call DS9 with first RGB image in science" ) );
    btn_cor_NOMAD->setText( tr( "correct NOMAD" ) );
    QToolTip::add( btn_cor_NOMAD, tr( "calculates +5%(red) -10% (blue) from NOMAD color calibration" ) );
    btn_stats->setText( tr( "show source stats" ) );
    QToolTip::add( btn_stats, tr( "show a small statistic of found sources" ) );
}

