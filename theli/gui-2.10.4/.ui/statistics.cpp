/****************************************************************************
** Form implementation generated from reading ui file 'statistics.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "statistics.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../statistics.ui.h"
/*
 *  Constructs a statistics as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
statistics::statistics( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "statistics" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    setModal( FALSE );
    statisticsLayout = new QGridLayout( this, 1, 1, 11, 6, "statisticsLayout"); 

    groupBox5 = new QGroupBox( this, "groupBox5" );
    groupBox5->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    groupBox5->setColumnLayout(0, Qt::Vertical );
    groupBox5->layout()->setSpacing( 6 );
    groupBox5->layout()->setMargin( 11 );
    groupBox5Layout = new QGridLayout( groupBox5->layout() );
    groupBox5Layout->setAlignment( Qt::AlignTop );

    textLabel1 = new QLabel( groupBox5, "textLabel1" );
    textLabel1->setAlignment( int( QLabel::AlignVCenter ) );

    groupBox5Layout->addWidget( textLabel1, 0, 0 );

    textLabel2 = new QLabel( groupBox5, "textLabel2" );
    textLabel2->setAlignment( int( QLabel::AlignVCenter ) );

    groupBox5Layout->addWidget( textLabel2, 1, 0 );

    statistics_dirLineEdit = new QLineEdit( groupBox5, "statistics_dirLineEdit" );
    statistics_dirLineEdit->setMinimumSize( QSize( 380, 0 ) );
    statistics_dirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    statistics_dirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox5Layout->addMultiCellWidget( statistics_dirLineEdit, 0, 0, 1, 2 );

    statistics_filterLineEdit = new QLineEdit( groupBox5, "statistics_filterLineEdit" );
    statistics_filterLineEdit->setMinimumSize( QSize( 160, 0 ) );
    statistics_filterLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    statistics_filterLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox5Layout->addWidget( statistics_filterLineEdit, 1, 1 );

    statistics_selectPushButton = new QPushButton( groupBox5, "statistics_selectPushButton" );
    statistics_selectPushButton->setMinimumSize( QSize( 200, 0 ) );
    statistics_selectPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox5Layout->addWidget( statistics_selectPushButton, 1, 2 );

    statisticsLayout->addWidget( groupBox5, 0, 0 );

    statistics_closePushButton = new QPushButton( this, "statistics_closePushButton" );
    statistics_closePushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, statistics_closePushButton->sizePolicy().hasHeightForWidth() ) );
    statistics_closePushButton->setMinimumSize( QSize( 80, 0 ) );
    statistics_closePushButton->setMaximumSize( QSize( 80, 32767 ) );
    statistics_closePushButton->setFocusPolicy( QPushButton::NoFocus );

    statisticsLayout->addWidget( statistics_closePushButton, 2, 2 );
    spacer4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    statisticsLayout->addMultiCell( spacer4, 2, 2, 0, 1 );

    groupBox2 = new QGroupBox( this, "groupBox2" );
    groupBox2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, groupBox2->sizePolicy().hasHeightForWidth() ) );
    groupBox2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QGridLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );
    spacer11 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox2Layout->addItem( spacer11, 1, 1 );

    layout8 = new QGridLayout( 0, 1, 1, 0, 6, "layout8"); 

    textLabel4_2_2 = new QLabel( groupBox2, "textLabel4_2_2" );

    layout8->addWidget( textLabel4_2_2, 1, 3 );

    textLabel1_4 = new QLabel( groupBox2, "textLabel1_4" );

    layout8->addWidget( textLabel1_4, 1, 1 );

    textLabel4 = new QLabel( groupBox2, "textLabel4" );

    layout8->addMultiCellWidget( textLabel4, 0, 0, 0, 3 );

    textLabel4_2 = new QLabel( groupBox2, "textLabel4_2" );

    layout8->addWidget( textLabel4_2, 1, 2 );

    textLabel2_2 = new QLabel( groupBox2, "textLabel2_2" );

    layout8->addWidget( textLabel2_2, 1, 0 );

    statistics_xmaxLineEdit = new QLineEdit( groupBox2, "statistics_xmaxLineEdit" );
    statistics_xmaxLineEdit->setMinimumSize( QSize( 80, 0 ) );
    statistics_xmaxLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    statistics_xmaxLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    statistics_xmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout8->addWidget( statistics_xmaxLineEdit, 2, 1 );

    statistics_xminLineEdit = new QLineEdit( groupBox2, "statistics_xminLineEdit" );
    statistics_xminLineEdit->setMinimumSize( QSize( 80, 0 ) );
    statistics_xminLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    statistics_xminLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    statistics_xminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout8->addWidget( statistics_xminLineEdit, 2, 0 );

    statistics_yminLineEdit = new QLineEdit( groupBox2, "statistics_yminLineEdit" );
    statistics_yminLineEdit->setMinimumSize( QSize( 80, 0 ) );
    statistics_yminLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    statistics_yminLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    statistics_yminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout8->addWidget( statistics_yminLineEdit, 2, 2 );

    statistics_ymaxLineEdit = new QLineEdit( groupBox2, "statistics_ymaxLineEdit" );
    statistics_ymaxLineEdit->setMinimumSize( QSize( 80, 0 ) );
    statistics_ymaxLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    statistics_ymaxLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    statistics_ymaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout8->addWidget( statistics_ymaxLineEdit, 2, 3 );

    groupBox2Layout->addLayout( layout8, 1, 0 );

    buttonGroup1 = new QButtonGroup( groupBox2, "buttonGroup1" );
    buttonGroup1->setLineWidth( 0 );
    buttonGroup1->setProperty( "selectedId", -1 );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QGridLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    statistics_seeingarcsecRadioButton = new QRadioButton( buttonGroup1, "statistics_seeingarcsecRadioButton" );
    statistics_seeingarcsecRadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup1Layout->addWidget( statistics_seeingarcsecRadioButton, 1, 0 );

    statistics_seeingpixelRadioButton = new QRadioButton( buttonGroup1, "statistics_seeingpixelRadioButton" );
    statistics_seeingpixelRadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup1Layout->addWidget( statistics_seeingpixelRadioButton, 1, 1 );

    textLabel1_2 = new QLabel( buttonGroup1, "textLabel1_2" );

    buttonGroup1Layout->addMultiCellWidget( textLabel1_2, 0, 0, 0, 1 );

    groupBox2Layout->addWidget( buttonGroup1, 1, 2 );
    spacer9 = new QSpacerItem( 100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2Layout->addItem( spacer9, 1, 3 );

    statistics_table = new QTable( groupBox2, "statistics_table" );
    statistics_table->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, statistics_table->sizePolicy().hasHeightForWidth() ) );
    statistics_table->setMinimumSize( QSize( 700, 0 ) );
    statistics_table->setFocusPolicy( QTable::NoFocus );
    statistics_table->setNumRows( 3 );
    statistics_table->setNumCols( 8 );
    statistics_table->setRowMovingEnabled( FALSE );
    statistics_table->setColumnMovingEnabled( FALSE );
    statistics_table->setReadOnly( TRUE );
    statistics_table->setSorting( FALSE );

    groupBox2Layout->addMultiCellWidget( statistics_table, 2, 2, 0, 4 );

    textLabel1_3 = new QLabel( groupBox2, "textLabel1_3" );

    groupBox2Layout->addMultiCellWidget( textLabel1_3, 0, 0, 0, 2 );

    layout31 = new QVBoxLayout( 0, 0, 6, "layout31"); 

    statistics_clearPushButton = new QPushButton( groupBox2, "statistics_clearPushButton" );
    statistics_clearPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, statistics_clearPushButton->sizePolicy().hasHeightForWidth() ) );
    statistics_clearPushButton->setMinimumSize( QSize( 120, 0 ) );
    statistics_clearPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout31->addWidget( statistics_clearPushButton );

    statistics_getstatisticsPushButton = new QPushButton( groupBox2, "statistics_getstatisticsPushButton" );
    statistics_getstatisticsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, statistics_getstatisticsPushButton->sizePolicy().hasHeightForWidth() ) );
    statistics_getstatisticsPushButton->setMinimumSize( QSize( 120, 0 ) );
    statistics_getstatisticsPushButton->setMaximumSize( QSize( 32767, 32767 ) );
    statistics_getstatisticsPushButton->setCursor( QCursor( 0 ) );
    statistics_getstatisticsPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout31->addWidget( statistics_getstatisticsPushButton );

    statistics_abortPushButton = new QPushButton( groupBox2, "statistics_abortPushButton" );
    statistics_abortPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, statistics_abortPushButton->sizePolicy().hasHeightForWidth() ) );
    statistics_abortPushButton->setMinimumSize( QSize( 120, 0 ) );
    statistics_abortPushButton->setMaximumSize( QSize( 32767, 32767 ) );
    statistics_abortPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    statistics_abortPushButton->setCursor( QCursor( 0 ) );
    statistics_abortPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout31->addWidget( statistics_abortPushButton );

    groupBox2Layout->addMultiCellLayout( layout31, 0, 1, 4, 4 );

    statisticsLayout->addMultiCellWidget( groupBox2, 1, 1, 0, 2 );
    spacer5 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    statisticsLayout->addItem( spacer5, 0, 1 );
    languageChange();
    resize( QSize(866, 403).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( statistics_clearPushButton, SIGNAL( clicked() ), this, SLOT( clearfields() ) );
    connect( statistics_dirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( statistics_filterLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( statistics_xminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( statistics_xmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( statistics_yminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( statistics_ymaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( statistics_closePushButton, SIGNAL( clicked() ), this, SLOT( quit() ) );
    connect( statistics_getstatisticsPushButton, SIGNAL( clicked() ), this, SLOT( get_statistics() ) );
    connect( statistics_seeingarcsecRadioButton, SIGNAL( stateChanged(int) ), this, SLOT( update_statisticstable() ) );
    connect( statistics_selectPushButton, SIGNAL( clicked() ), this, SLOT( select_dirimage() ) );
    connect( statistics_abortPushButton, SIGNAL( clicked() ), this, SLOT( kill_stats() ) );

    // tab order
    setTabOrder( statistics_dirLineEdit, statistics_filterLineEdit );
    setTabOrder( statistics_filterLineEdit, statistics_xminLineEdit );
    setTabOrder( statistics_xminLineEdit, statistics_xmaxLineEdit );
    setTabOrder( statistics_xmaxLineEdit, statistics_yminLineEdit );
    setTabOrder( statistics_yminLineEdit, statistics_ymaxLineEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
statistics::~statistics()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void statistics::languageChange()
{
    setCaption( tr( "Image statistics" ) );
    QWhatsThis::add( this, tr( "<b><font color=\"#0000ff\">Image statistics</font></b>\n"
"\n"
"<p>\n"
"<b>Image directory:</b> The path to the images from which you want to obtain statistics<br>\n"
"<b>Name filter:</b> Any kind of file name filter, such as \"*[1-4]OFCS.fits\" (without the \n"
"quotes). If left empty, all images will be used (*.fits)<br>\n"
"<b>Image section:</b> If you want the statistics to be calculated based on a subsection \n"
"of the images, then enter it here. It will be the same for all images and chips. If left empty, the entire images are considered.<br>\n"
"</p>" ) );
    groupBox5->setTitle( QString::null );
    textLabel1->setText( tr( "Directory with images:" ) );
    textLabel2->setText( tr( "Name filter (e.g. *OFC.fits):" ) );
    QToolTip::add( statistics_dirLineEdit, tr( "Directory with the images from which statistics should be obtained" ) );
    QToolTip::add( statistics_filterLineEdit, tr( "A filter for the images, e.g. \"*1OFCS.fits\", or the name of an individual file. If left empty, it is set to \"*.fits\"<br>" ) );
    statistics_selectPushButton->setText( tr( "Select image" ) );
    statistics_closePushButton->setText( tr( "Close" ) );
    groupBox2->setTitle( QString::null );
    textLabel4_2_2->setText( tr( "ymax" ) );
    textLabel1_4->setText( tr( "xmax" ) );
    textLabel4->setText( tr( "Area for background estimation:" ) );
    textLabel4_2->setText( tr( "ymin" ) );
    textLabel2_2->setText( tr( "xmin" ) );
    QToolTip::add( statistics_xmaxLineEdit, tr( "The XMAX value of the statistics section" ) );
    QToolTip::add( statistics_xminLineEdit, tr( "The XMIN value of the statistics section" ) );
    QToolTip::add( statistics_yminLineEdit, tr( "The YMIN value of the statistics section" ) );
    QToolTip::add( statistics_ymaxLineEdit, tr( "The YMAX value of the statistics section" ) );
    buttonGroup1->setTitle( QString::null );
    statistics_seeingarcsecRadioButton->setText( tr( "arcsec" ) );
    statistics_seeingpixelRadioButton->setText( tr( "pixel" ) );
    textLabel1_2->setText( tr( "FWHM and HFD units:" ) );
    textLabel1_3->setText( tr( "<b>Image statistics: Background, seeing, transparency</b>" ) );
    statistics_clearPushButton->setText( tr( "Clear fields" ) );
    QToolTip::add( statistics_clearPushButton, tr( "Clears the image area and the table" ) );
    statistics_getstatisticsPushButton->setText( tr( "Get statistics" ) );
    statistics_abortPushButton->setText( tr( "Abort" ) );
}

