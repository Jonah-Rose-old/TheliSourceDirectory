/****************************************************************************
** Form implementation generated from reading ui file 'combineimages.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "combineimages.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../combineimages.ui.h"
/*
 *  Constructs a combineimages as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
combineimages::combineimages( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "combineimages" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    setModal( FALSE );
    combineimagesLayout = new QGridLayout( this, 1, 1, 11, 6, "combineimagesLayout"); 
    spacer18 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding );
    combineimagesLayout->addItem( spacer18, 1, 0 );

    groupBox2 = new QGroupBox( this, "groupBox2" );
    groupBox2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, groupBox2->sizePolicy().hasHeightForWidth() ) );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QGridLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );

    combima_closePushButton = new QPushButton( groupBox2, "combima_closePushButton" );
    combima_closePushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, combima_closePushButton->sizePolicy().hasHeightForWidth() ) );
    combima_closePushButton->setMinimumSize( QSize( 120, 0 ) );
    combima_closePushButton->setMaximumSize( QSize( 120, 32767 ) );
    combima_closePushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox2Layout->addWidget( combima_closePushButton, 1, 3 );

    combima_clearPushButton = new QPushButton( groupBox2, "combima_clearPushButton" );
    combima_clearPushButton->setMinimumSize( QSize( 120, 0 ) );
    combima_clearPushButton->setMaximumSize( QSize( 120, 32767 ) );
    combima_clearPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox2Layout->addWidget( combima_clearPushButton, 0, 3 );

    combima_abortPushButton = new QPushButton( groupBox2, "combima_abortPushButton" );
    combima_abortPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, combima_abortPushButton->sizePolicy().hasHeightForWidth() ) );
    combima_abortPushButton->setMinimumSize( QSize( 120, 0 ) );
    combima_abortPushButton->setMaximumSize( QSize( 120, 32767 ) );
    combima_abortPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    combima_abortPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox2Layout->addWidget( combima_abortPushButton, 1, 1 );

    combima_statusLineEdit = new QLineEdit( groupBox2, "combima_statusLineEdit" );
    combima_statusLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, combima_statusLineEdit->sizePolicy().hasHeightForWidth() ) );
    combima_statusLineEdit->setMinimumSize( QSize( 120, 0 ) );
    combima_statusLineEdit->setMaximumSize( QSize( 120, 32767 ) );
    combima_statusLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    combima_statusLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    combima_statusLineEdit->setAlignment( int( QLineEdit::AlignLeft ) );
    combima_statusLineEdit->setReadOnly( TRUE );

    groupBox2Layout->addWidget( combima_statusLineEdit, 0, 1 );

    combima_startPushButton = new QPushButton( groupBox2, "combima_startPushButton" );
    combima_startPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, combima_startPushButton->sizePolicy().hasHeightForWidth() ) );
    combima_startPushButton->setMinimumSize( QSize( 120, 0 ) );
    combima_startPushButton->setMaximumSize( QSize( 120, 32767 ) );
    combima_startPushButton->setPaletteForegroundColor( QColor( 0, 85, 0 ) );
    combima_startPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox2Layout->addWidget( combima_startPushButton, 1, 0 );

    textLabel3 = new QLabel( groupBox2, "textLabel3" );
    textLabel3->setAlignment( int( QLabel::AlignVCenter ) );

    groupBox2Layout->addWidget( textLabel3, 0, 0 );
    spacer21_2 = new QSpacerItem( 40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    groupBox2Layout->addItem( spacer21_2, 1, 2 );

    combineimagesLayout->addWidget( groupBox2, 2, 0 );

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, groupBox1->sizePolicy().hasHeightForWidth() ) );
    groupBox1->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QGridLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    textLabel1_2 = new QLabel( groupBox1, "textLabel1_2" );
    textLabel1_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel1_2->sizePolicy().hasHeightForWidth() ) );

    groupBox1Layout->addWidget( textLabel1_2, 2, 0 );

    textLabel1_3 = new QLabel( groupBox1, "textLabel1_3" );
    textLabel1_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel1_3->sizePolicy().hasHeightForWidth() ) );

    groupBox1Layout->addWidget( textLabel1_3, 3, 0 );

    textLabel1 = new QLabel( groupBox1, "textLabel1" );

    groupBox1Layout->addWidget( textLabel1, 0, 0 );

    combima_dirLineEdit = new QLineEdit( groupBox1, "combima_dirLineEdit" );
    combima_dirLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, combima_dirLineEdit->sizePolicy().hasHeightForWidth() ) );
    combima_dirLineEdit->setMinimumSize( QSize( 420, 0 ) );
    combima_dirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addMultiCellWidget( combima_dirLineEdit, 1, 1, 0, 4 );

    combima_filterLineEdit = new QLineEdit( groupBox1, "combima_filterLineEdit" );
    combima_filterLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, combima_filterLineEdit->sizePolicy().hasHeightForWidth() ) );
    combima_filterLineEdit->setMinimumSize( QSize( 170, 0 ) );
    combima_filterLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addMultiCellWidget( combima_filterLineEdit, 2, 2, 2, 4 );

    combima_nameLineEdit = new QLineEdit( groupBox1, "combima_nameLineEdit" );
    combima_nameLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, combima_nameLineEdit->sizePolicy().hasHeightForWidth() ) );
    combima_nameLineEdit->setMinimumSize( QSize( 170, 0 ) );
    combima_nameLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addMultiCellWidget( combima_nameLineEdit, 3, 3, 2, 4 );

    combima_dtLineEdit = new QLineEdit( groupBox1, "combima_dtLineEdit" );
    combima_dtLineEdit->setMinimumSize( QSize( 60, 0 ) );
    combima_dtLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    combima_dtLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addWidget( combima_dtLineEdit, 6, 2 );
    spacer1_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer1_2, 3, 1 );
    spacer1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer1, 2, 1 );
    spacer10 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer10, 6, 4 );

    combima_dminLineEdit = new QLineEdit( groupBox1, "combima_dminLineEdit" );
    combima_dminLineEdit->setMinimumSize( QSize( 60, 0 ) );
    combima_dminLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    combima_dminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addWidget( combima_dminLineEdit, 6, 3 );
    spacer1_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer1_3, 6, 1 );

    textLabel1_6 = new QLabel( groupBox1, "textLabel1_6" );
    textLabel1_6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel1_6->sizePolicy().hasHeightForWidth() ) );

    groupBox1Layout->addWidget( textLabel1_6, 6, 0 );

    textLabel2_2 = new QLabel( groupBox1, "textLabel2_2" );
    textLabel2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, textLabel2_2->sizePolicy().hasHeightForWidth() ) );
    textLabel2_2->setAlignment( int( QLabel::AlignVCenter ) );

    groupBox1Layout->addMultiCellWidget( textLabel2_2, 5, 5, 0, 1 );
    spacer10_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer10_3, 7, 4 );

    combima_nhighLineEdit = new QLineEdit( groupBox1, "combima_nhighLineEdit" );
    combima_nhighLineEdit->setMinimumSize( QSize( 60, 0 ) );
    combima_nhighLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    combima_nhighLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addWidget( combima_nhighLineEdit, 7, 3 );
    spacer1_5 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer1_5, 7, 1 );

    combima_nlowLineEdit = new QLineEdit( groupBox1, "combima_nlowLineEdit" );
    combima_nlowLineEdit->setMinimumSize( QSize( 60, 0 ) );
    combima_nlowLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    combima_nlowLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addWidget( combima_nlowLineEdit, 7, 2 );

    textLabel1_4 = new QLabel( groupBox1, "textLabel1_4" );
    textLabel1_4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel1_4->sizePolicy().hasHeightForWidth() ) );

    groupBox1Layout->addWidget( textLabel1_4, 7, 0 );
    spacer8_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer8_2, 4, 0 );
    spacer8_2_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer8_2_2, 8, 0 );

    textLabel1_5 = new QLabel( groupBox1, "textLabel1_5" );
    textLabel1_5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, textLabel1_5->sizePolicy().hasHeightForWidth() ) );
    textLabel1_5->setAlignment( int( QLabel::AlignVCenter ) );

    groupBox1Layout->addWidget( textLabel1_5, 9, 0 );

    layout29 = new QHBoxLayout( 0, 0, 6, "layout29"); 

    layout28 = new QGridLayout( 0, 1, 1, 0, 6, "layout28"); 
    spacer53 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout28->addItem( spacer53, 1, 0 );

    layout26 = new QHBoxLayout( 0, 0, 6, "layout26"); 

    combima_methodComboBox = new QComboBox( FALSE, groupBox1, "combima_methodComboBox" );
    combima_methodComboBox->setMinimumSize( QSize( 100, 0 ) );
    combima_methodComboBox->setFocusPolicy( QComboBox::NoFocus );
    layout26->addWidget( combima_methodComboBox );
    spacer52 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout26->addItem( spacer52 );

    combima_rescaleCheckBox = new QCheckBox( groupBox1, "combima_rescaleCheckBox" );
    combima_rescaleCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, combima_rescaleCheckBox->sizePolicy().hasHeightForWidth() ) );
    combima_rescaleCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    layout26->addWidget( combima_rescaleCheckBox );

    layout28->addMultiCellLayout( layout26, 0, 0, 0, 1 );

    combima_levelLineEdit = new QLineEdit( groupBox1, "combima_levelLineEdit" );
    combima_levelLineEdit->setMinimumSize( QSize( 100, 0 ) );
    combima_levelLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout28->addWidget( combima_levelLineEdit, 1, 1 );
    layout29->addLayout( layout28 );
    spacer54 = new QSpacerItem( 40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    layout29->addItem( spacer54 );

    groupBox1Layout->addMultiCellLayout( layout29, 10, 10, 0, 4 );

    combineimagesLayout->addWidget( groupBox1, 0, 0 );
    languageChange();
    resize( QSize(517, 511).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( combima_clearPushButton, SIGNAL( clicked() ), this, SLOT( clearfields() ) );
    connect( combima_dirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( combima_nameLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( combima_filterLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( combima_nhighLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( combima_nlowLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( combima_dtLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( combima_dminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( combima_abortPushButton, SIGNAL( clicked() ), this, SLOT( kill_combineimage() ) );
    connect( combima_startPushButton, SIGNAL( clicked() ), this, SLOT( compile_scriptsyntax() ) );
    connect( combima_closePushButton, SIGNAL( clicked() ), this, SLOT( quit() ) );
    connect( combima_rescaleCheckBox, SIGNAL( clicked() ), this, SLOT( validate() ) );
    connect( combima_levelLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );

    // tab order
    setTabOrder( combima_dirLineEdit, combima_filterLineEdit );
    setTabOrder( combima_filterLineEdit, combima_nameLineEdit );
    setTabOrder( combima_nameLineEdit, combima_dtLineEdit );
    setTabOrder( combima_dtLineEdit, combima_dminLineEdit );
    setTabOrder( combima_dminLineEdit, combima_nlowLineEdit );
    setTabOrder( combima_nlowLineEdit, combima_nhighLineEdit );
    setTabOrder( combima_nhighLineEdit, combima_levelLineEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
combineimages::~combineimages()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void combineimages::languageChange()
{
    setCaption( tr( "Combine images" ) );
    QWhatsThis::add( this, tr( "With this dialog you can combine any set of images in some directory, using the usual detection and rejection parameters.\n"
"\n"
"<p><b>Name filter:</b><br>This is the string that specifies the status of the processing on the particular set of images. It is the string between the chip number and the \".fits\" extension of the file name. For example, OFC or OFCST. If no filter is given, all \".fits\" images in the specified directory will be combined.</p>\n"
"\n"
"<p><b>Name of the combined image:</b><br>Here you can specify the name of the stacked image, without chip number and \".fits\" extension. If left empty, it will be set to the name of the folder.</p>\n"
"\n"
"<p><b>Detection thresholds (DT | DMIN):</b><br>The signal to noise ratio above sky background for each pixel of an object, and the minimum number of such pixels making up an object. All objecs defined in this way will be removed from the images before combination. If both fields are left empty, no object detection will take place. Default values are 1.0 for DT and 5 for DMIN.</p>\n"
"\n"
"<p><b>Number of pixels to reject (low | high):</b><br>The specified numbers of low and high pixels will be rejected during stacking.</p>" ) );
    groupBox2->setTitle( QString::null );
    combima_closePushButton->setText( tr( "Close" ) );
    combima_clearPushButton->setText( tr( "Clear" ) );
    combima_abortPushButton->setText( tr( "Abort" ) );
    combima_startPushButton->setText( tr( "Start" ) );
    textLabel3->setText( tr( "<b>Processing status:</b>" ) );
    groupBox1->setTitle( tr( "Combine a set of images" ) );
    textLabel1_2->setText( tr( "Name filter" ) );
    textLabel1_3->setText( tr( "Name of the combined image" ) );
    textLabel1->setText( tr( "<b>Directory with the images</b>" ) );
    QToolTip::add( combima_dirLineEdit, tr( "The directory with the images you want to combine" ) );
    QToolTip::add( combima_filterLineEdit, tr( "Put a filter string here, such as \"OFCS\". It must be the string between the chip number and the \".fits\" suffix of the file name.<br>" ) );
    QToolTip::add( combima_nameLineEdit, tr( "OPTIONAL: The name of the combined image. If left empty, it will be the name of the folder where the images are.<br>" ) );
    QToolTip::add( combima_dtLineEdit, tr( "The minimum signal-to-noise of a single pixel of an object.<br>" ) );
    QToolTip::add( combima_dminLineEdit, tr( "The minimum number of connected pixels above DT that form an object.<br>" ) );
    textLabel1_6->setText( tr( "Detection thresholds (DT | DMIN)" ) );
    textLabel2_2->setText( tr( "<b>Detection and combination parameters</b>" ) );
    QToolTip::add( combima_nhighLineEdit, tr( "When combining the images, this number of highest pixels will be excluded from the stack.<br>" ) );
    QToolTip::add( combima_nlowLineEdit, tr( "When combining the images, this number of lowest pixels will be excluded from the stack.<br>" ) );
    textLabel1_4->setText( tr( "Number of pixels to reject (low | high)" ) );
    textLabel1_5->setText( tr( "<b>Combination method</b>" ) );
    combima_methodComboBox->clear();
    combima_methodComboBox->insertItem( tr( "Median" ) );
    combima_methodComboBox->insertItem( tr( "Mean" ) );
    combima_rescaleCheckBox->setText( tr( "Rescale images to\n"
"the same level" ) );
    QToolTip::add( combima_rescaleCheckBox, tr( "Activate this CheckBox if you want the images to be rescaled to the same level (statistical mode) before combination. Optionally, you can specify this level.<br>" ) );
    QToolTip::add( combima_levelLineEdit, tr( "The level to which the combined image can be rescaled (optional).<br> " ) );
}

