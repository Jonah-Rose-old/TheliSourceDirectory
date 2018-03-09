/****************************************************************************
** Form implementation generated from reading ui file 'zp.ui'
**
** Created: Fri Feb 2 16:30:22 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "zp.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../zp.ui.h"
/*
 *  Constructs a zp as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
zp::zp( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "zp" );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    zpLayout = new QGridLayout( this, 1, 1, 11, 6, "zpLayout"); 

    zp_abortPushButton = new QPushButton( this, "zp_abortPushButton" );
    zp_abortPushButton->setMinimumSize( QSize( 120, 0 ) );
    zp_abortPushButton->setMaximumSize( QSize( 120, 32767 ) );
    zp_abortPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );

    zpLayout->addWidget( zp_abortPushButton, 2, 2 );

    zp_closePushButton = new QPushButton( this, "zp_closePushButton" );
    zp_closePushButton->setMinimumSize( QSize( 120, 0 ) );
    zp_closePushButton->setMaximumSize( QSize( 120, 32767 ) );

    zpLayout->addWidget( zp_closePushButton, 2, 3 );
    spacer42 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed );
    zpLayout->addItem( spacer42, 1, 2 );

    zp_startPushButton = new QPushButton( this, "zp_startPushButton" );
    zp_startPushButton->setMinimumSize( QSize( 120, 0 ) );
    zp_startPushButton->setMaximumSize( QSize( 120, 32767 ) );
    zp_startPushButton->setPaletteForegroundColor( QColor( 0, 0, 0 ) );

    zpLayout->addWidget( zp_startPushButton, 2, 1 );
    spacer40 = new QSpacerItem( 68, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    zpLayout->addItem( spacer40, 2, 0 );

    groupBox20 = new QGroupBox( this, "groupBox20" );
    groupBox20->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    QFont groupBox20_font(  groupBox20->font() );
    groupBox20->setFont( groupBox20_font ); 
    groupBox20->setColumnLayout(0, Qt::Vertical );
    groupBox20->layout()->setSpacing( 6 );
    groupBox20->layout()->setMargin( 11 );
    groupBox20Layout = new QGridLayout( groupBox20->layout() );
    groupBox20Layout->setAlignment( Qt::AlignTop );

    textLabel2 = new QLabel( groupBox20, "textLabel2" );

    groupBox20Layout->addWidget( textLabel2, 2, 0 );
    spacer37 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox20Layout->addItem( spacer37, 1, 2 );

    textLabel3 = new QLabel( groupBox20, "textLabel3" );

    groupBox20Layout->addWidget( textLabel3, 4, 0 );

    textLabel1_3 = new QLabel( groupBox20, "textLabel1_3" );

    groupBox20Layout->addWidget( textLabel1_3, 3, 0 );

    layout3 = new QGridLayout( 0, 1, 1, 0, 6, "layout3"); 

    textLabel1 = new QLabel( groupBox20, "textLabel1" );

    layout3->addWidget( textLabel1, 0, 0 );

    zp_loadPushButton = new QPushButton( groupBox20, "zp_loadPushButton" );
    zp_loadPushButton->setMinimumSize( QSize( 120, 0 ) );
    zp_loadPushButton->setMaximumSize( QSize( 120, 32767 ) );

    layout3->addWidget( zp_loadPushButton, 0, 2 );

    zp_imageLineEdit = new QLineEdit( groupBox20, "zp_imageLineEdit" );
    zp_imageLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, zp_imageLineEdit->sizePolicy().hasHeightForWidth() ) );
    zp_imageLineEdit->setMinimumSize( QSize( 500, 0 ) );
    zp_imageLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout3->addMultiCellWidget( zp_imageLineEdit, 1, 1, 0, 2 );
    spacer38 = new QSpacerItem( 232, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer38, 0, 1 );

    groupBox20Layout->addMultiCellLayout( layout3, 0, 0, 0, 6 );

    textLabel4_2 = new QLabel( groupBox20, "textLabel4_2" );

    groupBox20Layout->addMultiCellWidget( textLabel4_2, 11, 11, 4, 6 );

    zp_resultLineEdit = new QLineEdit( groupBox20, "zp_resultLineEdit" );
    zp_resultLineEdit->setMinimumSize( QSize( 100, 0 ) );
    zp_resultLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    zp_resultLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    zp_resultLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    zp_resultLineEdit->setReadOnly( TRUE );

    groupBox20Layout->addMultiCellWidget( zp_resultLineEdit, 12, 12, 1, 2 );
    spacer39 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox20Layout->addItem( spacer39, 9, 0 );

    textLabel4 = new QLabel( groupBox20, "textLabel4" );

    groupBox20Layout->addWidget( textLabel4, 12, 0 );

    textLabel5_3 = new QLabel( groupBox20, "textLabel5_3" );

    groupBox20Layout->addMultiCellWidget( textLabel5_3, 14, 14, 4, 5 );

    zp_resultrmsLineEdit = new QLineEdit( groupBox20, "zp_resultrmsLineEdit" );
    zp_resultrmsLineEdit->setMinimumSize( QSize( 100, 0 ) );
    zp_resultrmsLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    zp_resultrmsLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    zp_resultrmsLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    zp_resultrmsLineEdit->setReadOnly( TRUE );

    groupBox20Layout->addMultiCellWidget( zp_resultrmsLineEdit, 14, 14, 1, 2 );
    spacer13_4 = new QSpacerItem( 30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox20Layout->addItem( spacer13_4, 15, 3 );

    textLabel1_4 = new QLabel( groupBox20, "textLabel1_4" );

    groupBox20Layout->addWidget( textLabel1_4, 10, 0 );

    textLabel5 = new QLabel( groupBox20, "textLabel5" );

    groupBox20Layout->addMultiCellWidget( textLabel5, 12, 12, 4, 6 );

    textLabel4_4 = new QLabel( groupBox20, "textLabel4_4" );

    groupBox20Layout->addWidget( textLabel4_4, 14, 0 );
    spacer17 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox20Layout->addMultiCell( spacer17, 13, 14, 6, 6 );

    zp_resulterrLineEdit = new QLineEdit( groupBox20, "zp_resulterrLineEdit" );
    zp_resulterrLineEdit->setMinimumSize( QSize( 100, 0 ) );
    zp_resulterrLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    zp_resulterrLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    zp_resulterrLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    zp_resulterrLineEdit->setReadOnly( TRUE );

    groupBox20Layout->addMultiCellWidget( zp_resulterrLineEdit, 13, 13, 1, 2 );

    textLabel4_5 = new QLabel( groupBox20, "textLabel4_5" );

    groupBox20Layout->addWidget( textLabel4_5, 13, 0 );

    textLabel2_2 = new QLabel( groupBox20, "textLabel2_2" );

    groupBox20Layout->addWidget( textLabel2_2, 11, 0 );

    textLabel5_2 = new QLabel( groupBox20, "textLabel5_2" );

    groupBox20Layout->addMultiCellWidget( textLabel5_2, 15, 15, 4, 6 );

    zp_resultnumLineEdit = new QLineEdit( groupBox20, "zp_resultnumLineEdit" );
    zp_resultnumLineEdit->setMinimumSize( QSize( 100, 0 ) );
    zp_resultnumLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    zp_resultnumLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    zp_resultnumLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    zp_resultnumLineEdit->setReadOnly( TRUE );

    groupBox20Layout->addMultiCellWidget( zp_resultnumLineEdit, 15, 15, 1, 2 );

    textLabel3_2 = new QLabel( groupBox20, "textLabel3_2" );

    groupBox20Layout->addMultiCellWidget( textLabel3_2, 11, 11, 1, 2 );
    spacer13_2 = new QSpacerItem( 30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox20Layout->addItem( spacer13_2, 13, 3 );
    spacer13_3 = new QSpacerItem( 30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox20Layout->addItem( spacer13_3, 14, 3 );
    spacer13 = new QSpacerItem( 30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox20Layout->addItem( spacer13, 12, 3 );

    textLabel5_4 = new QLabel( groupBox20, "textLabel5_4" );

    groupBox20Layout->addMultiCellWidget( textLabel5_4, 13, 13, 4, 5 );

    textLabel4_3 = new QLabel( groupBox20, "textLabel4_3" );

    groupBox20Layout->addWidget( textLabel4_3, 15, 0 );

    textLabel1_2_2 = new QLabel( groupBox20, "textLabel1_2_2" );

    groupBox20Layout->addWidget( textLabel1_2_2, 5, 0 );

    textLabel1_2_2_2_2 = new QLabel( groupBox20, "textLabel1_2_2_2_2" );

    groupBox20Layout->addWidget( textLabel1_2_2_2_2, 6, 0 );

    zp_maxphoterrorLineEdit = new QLineEdit( groupBox20, "zp_maxphoterrorLineEdit" );
    zp_maxphoterrorLineEdit->setMinimumSize( QSize( 100, 0 ) );
    zp_maxphoterrorLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    zp_maxphoterrorLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox20Layout->addMultiCellWidget( zp_maxphoterrorLineEdit, 7, 7, 2, 4 );

    zp_magtypeComboBox = new QComboBox( FALSE, groupBox20, "zp_magtypeComboBox" );

    groupBox20Layout->addMultiCellWidget( zp_magtypeComboBox, 6, 6, 2, 4 );

    zp_saturationLineEdit = new QLineEdit( groupBox20, "zp_saturationLineEdit" );
    zp_saturationLineEdit->setMinimumSize( QSize( 100, 0 ) );
    zp_saturationLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    zp_saturationLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox20Layout->addMultiCellWidget( zp_saturationLineEdit, 5, 5, 2, 4 );

    textLabel1_2 = new QLabel( groupBox20, "textLabel1_2" );

    groupBox20Layout->addMultiCellWidget( textLabel1_2, 7, 7, 0, 1 );

    zp_aperturesizeLineEdit = new QLineEdit( groupBox20, "zp_aperturesizeLineEdit" );
    zp_aperturesizeLineEdit->setMinimumSize( QSize( 100, 0 ) );
    zp_aperturesizeLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    zp_aperturesizeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox20Layout->addMultiCellWidget( zp_aperturesizeLineEdit, 8, 8, 2, 4 );

    textLabel1_2_2_2 = new QLabel( groupBox20, "textLabel1_2_2_2" );

    groupBox20Layout->addWidget( textLabel1_2_2_2, 8, 0 );
    spacer41 = new QSpacerItem( 120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox20Layout->addMultiCell( spacer41, 4, 4, 5, 6 );

    zp_refcatComboBox = new QComboBox( FALSE, groupBox20, "zp_refcatComboBox" );
    zp_refcatComboBox->setMinimumSize( QSize( 100, 0 ) );
    zp_refcatComboBox->setMaximumSize( QSize( 100, 32767 ) );

    groupBox20Layout->addMultiCellWidget( zp_refcatComboBox, 2, 2, 2, 4 );

    zp_serverComboBox = new QComboBox( FALSE, groupBox20, "zp_serverComboBox" );
    zp_serverComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, zp_serverComboBox->sizePolicy().hasHeightForWidth() ) );
    zp_serverComboBox->setMinimumSize( QSize( 100, 0 ) );
    zp_serverComboBox->setMaximumSize( QSize( 100, 32767 ) );
    zp_serverComboBox->setFocusPolicy( QComboBox::NoFocus );
    zp_serverComboBox->setSizeLimit( 11 );

    groupBox20Layout->addMultiCellWidget( zp_serverComboBox, 3, 3, 2, 4 );

    zp_filterComboBox = new QComboBox( FALSE, groupBox20, "zp_filterComboBox" );
    zp_filterComboBox->setMinimumSize( QSize( 100, 0 ) );
    zp_filterComboBox->setMaximumSize( QSize( 100, 32767 ) );

    groupBox20Layout->addMultiCellWidget( zp_filterComboBox, 4, 4, 2, 4 );

    zpLayout->addMultiCellWidget( groupBox20, 0, 0, 0, 3 );
    languageChange();
    resize( QSize(546, 570).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( zp_closePushButton, SIGNAL( clicked() ), this, SLOT( quit() ) );
    connect( zp_loadPushButton, SIGNAL( clicked() ), this, SLOT( select_image() ) );
    connect( zp_refcatComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_filter_comboboxes() ) );
    connect( zp_maxphoterrorLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( zp_imageLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( zp_abortPushButton, SIGNAL( clicked() ), this, SLOT( kill_process() ) );
    connect( zp_startPushButton, SIGNAL( clicked() ), this, SLOT( get_zp() ) );
    connect( zp_saturationLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( zp_magtypeComboBox, SIGNAL( activated(const QString&) ), this, SLOT( show_hide() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
zp::~zp()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void zp::languageChange()
{
    setCaption( tr( "Direct absolute photometric calibration" ) );
    zp_abortPushButton->setText( tr( "Abort" ) );
    zp_closePushButton->setText( tr( "Close" ) );
    zp_startPushButton->setText( tr( "Get ZP" ) );
    groupBox20->setTitle( QString::null );
    textLabel2->setText( tr( "Photometric reference catalog" ) );
    textLabel3->setText( tr( "Filter in which you observed" ) );
    textLabel1_3->setText( tr( "Download server" ) );
    textLabel1->setText( tr( "Full path to the image" ) );
    zp_loadPushButton->setText( tr( "Locate image ..." ) );
    QToolTip::add( zp_imageLineEdit, tr( "The directory with the images you want to combine" ) );
    textLabel4_2->setText( tr( "<b>FITS KEY</b>" ) );
    QToolTip::add( zp_resultLineEdit, tr( "The photometric zeropoint<br>" ) );
    textLabel4->setText( tr( "Photometric ZP" ) );
    textLabel5_3->setText( tr( "ZP_D_RMS" ) );
    QToolTip::add( zp_resultrmsLineEdit, tr( "The scattering of the magnitude differences between image and reference system" ) );
    textLabel1_4->setText( tr( "<b>Results :</b>" ) );
    textLabel5->setText( tr( "ZP_D" ) );
    textLabel4_4->setText( tr( "RMS of (MagOBJ - MagREF)" ) );
    QToolTip::add( zp_resulterrLineEdit, tr( "Uncertainty of the ZP" ) );
    textLabel4_5->setText( tr( "Zeropoint error" ) );
    textLabel2_2->setText( tr( "<b>Description</b>" ) );
    textLabel5_2->setText( tr( "ZP_D_NUM" ) );
    QToolTip::add( zp_resultnumLineEdit, tr( "The number of sources used to determine the zeropoint<br>" ) );
    textLabel3_2->setText( tr( "<b>Value</b>" ) );
    textLabel5_4->setText( tr( "ZP_D_ERR" ) );
    textLabel4_3->setText( tr( "Number of sources used" ) );
    textLabel1_2_2->setText( tr( "Saturation / non-lin. level" ) );
    textLabel1_2_2_2_2->setText( tr( "SExtractor mag type" ) );
    QToolTip::add( zp_maxphoterrorLineEdit, tr( "The maximum error allowed in the measurement of an object's magnitude if it enters the fit. If left empty, this value will be set to 0.05.<br>" ) );
    zp_magtypeComboBox->clear();
    zp_magtypeComboBox->insertItem( tr( "MAG_APER" ) );
    zp_magtypeComboBox->insertItem( tr( "MAG_AUTO" ) );
    QToolTip::add( zp_magtypeComboBox, tr( "Choose the SExtractor magnitude type" ) );
    QToolTip::add( zp_saturationLineEdit, tr( "This is the level at which saturation (or non-linearity) occurs in the coadded image. Sources with pixels brighter than this value will not be used in the analysis.<br>" ) );
    textLabel1_2->setText( tr( "SExtractor max phot error (mag)" ) );
    QToolTip::add( zp_aperturesizeLineEdit, tr( "Aperture diameter if MAG_APER is chosen" ) );
    textLabel1_2_2_2->setText( tr( "Aperture diameter (pixel)" ) );
    zp_refcatComboBox->clear();
    zp_refcatComboBox->insertItem( tr( "SDSS" ) );
    zp_refcatComboBox->insertItem( tr( "2MASS" ) );
    zp_serverComboBox->clear();
    zp_serverComboBox->insertItem( tr( "France" ) );
    zp_serverComboBox->insertItem( tr( "USA" ) );
    zp_serverComboBox->insertItem( tr( "Canada" ) );
    zp_serverComboBox->insertItem( tr( "Japan" ) );
    zp_serverComboBox->insertItem( tr( "India" ) );
    zp_serverComboBox->insertItem( tr( "China" ) );
    zp_serverComboBox->insertItem( tr( "UK" ) );
    zp_serverComboBox->insertItem( tr( "Hawaii" ) );
    zp_serverComboBox->insertItem( tr( "Russia" ) );
    QToolTip::add( zp_serverComboBox, tr( "The server from where the data is downloaded." ) );
}

