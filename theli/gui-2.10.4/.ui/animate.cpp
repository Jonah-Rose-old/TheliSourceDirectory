/****************************************************************************
** Form implementation generated from reading ui file 'animate.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "animate.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../animate.ui.h"
/*
 *  Constructs a animate as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
animate::animate( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "animate" );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    animateLayout = new QGridLayout( this, 1, 1, 11, 6, "animateLayout"); 

    groupBox5_3 = new QGroupBox( this, "groupBox5_3" );
    groupBox5_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, groupBox5_3->sizePolicy().hasHeightForWidth() ) );
    groupBox5_3->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    groupBox5_3->setColumnLayout(0, Qt::Vertical );
    groupBox5_3->layout()->setSpacing( 6 );
    groupBox5_3->layout()->setMargin( 11 );
    groupBox5_3Layout = new QGridLayout( groupBox5_3->layout() );
    groupBox5_3Layout->setAlignment( Qt::AlignTop );

    animate_overlapSpinBox = new QSpinBox( groupBox5_3, "animate_overlapSpinBox" );
    animate_overlapSpinBox->setMinimumSize( QSize( 60, 0 ) );
    animate_overlapSpinBox->setMaximumSize( QSize( 60, 32767 ) );
    animate_overlapSpinBox->setFocusPolicy( QSpinBox::NoFocus );
    animate_overlapSpinBox->setMaxValue( 1000 );
    animate_overlapSpinBox->setLineStep( 50 );
    animate_overlapSpinBox->setValue( 50 );

    groupBox5_3Layout->addWidget( animate_overlapSpinBox, 4, 2 );

    textLabel11 = new QLabel( groupBox5_3, "textLabel11" );

    groupBox5_3Layout->addMultiCellWidget( textLabel11, 4, 4, 0, 1 );

    animate_divySpinBox = new QSpinBox( groupBox5_3, "animate_divySpinBox" );
    animate_divySpinBox->setMinimumSize( QSize( 60, 0 ) );
    animate_divySpinBox->setMaximumSize( QSize( 60, 32767 ) );
    animate_divySpinBox->setFocusPolicy( QSpinBox::NoFocus );
    animate_divySpinBox->setMaxValue( 9 );
    animate_divySpinBox->setMinValue( 1 );

    groupBox5_3Layout->addWidget( animate_divySpinBox, 3, 2 );

    textLabel10_2 = new QLabel( groupBox5_3, "textLabel10_2" );
    textLabel10_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel10_2->sizePolicy().hasHeightForWidth() ) );

    groupBox5_3Layout->addWidget( textLabel10_2, 5, 0 );

    animate_coaddListBox = new QListBox( groupBox5_3, "animate_coaddListBox" );
    animate_coaddListBox->setMinimumSize( QSize( 0, 140 ) );
    animate_coaddListBox->setMaximumSize( QSize( 32767, 140 ) );
    animate_coaddListBox->setFocusPolicy( QListBox::ClickFocus );
    animate_coaddListBox->setSelectionMode( QListBox::Extended );

    groupBox5_3Layout->addMultiCellWidget( animate_coaddListBox, 1, 1, 0, 2 );

    textLabel1_7 = new QLabel( groupBox5_3, "textLabel1_7" );
    textLabel1_7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, textLabel1_7->sizePolicy().hasHeightForWidth() ) );

    groupBox5_3Layout->addMultiCellWidget( textLabel1_7, 0, 0, 0, 1 );

    animate_divxSpinBox = new QSpinBox( groupBox5_3, "animate_divxSpinBox" );
    animate_divxSpinBox->setMinimumSize( QSize( 60, 0 ) );
    animate_divxSpinBox->setMaximumSize( QSize( 60, 32767 ) );
    animate_divxSpinBox->setFocusPolicy( QSpinBox::NoFocus );
    animate_divxSpinBox->setMaxValue( 9 );
    animate_divxSpinBox->setMinValue( 1 );

    groupBox5_3Layout->addWidget( animate_divxSpinBox, 2, 2 );

    textLabel2_3 = new QLabel( groupBox5_3, "textLabel2_3" );

    groupBox5_3Layout->addMultiCellWidget( textLabel2_3, 2, 2, 0, 1 );

    textLabel3_2 = new QLabel( groupBox5_3, "textLabel3_2" );

    groupBox5_3Layout->addMultiCellWidget( textLabel3_2, 3, 3, 0, 1 );

    animate_divListBox = new QListBox( groupBox5_3, "animate_divListBox" );
    animate_divListBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, animate_divListBox->sizePolicy().hasHeightForWidth() ) );
    animate_divListBox->setMinimumSize( QSize( 100, 140 ) );
    animate_divListBox->setFocusPolicy( QListBox::ClickFocus );
    animate_divListBox->setSelectionMode( QListBox::Extended );

    groupBox5_3Layout->addMultiCellWidget( animate_divListBox, 5, 5, 1, 2 );

    animateLayout->addWidget( groupBox5_3, 1, 0 );

    groupBox3 = new QGroupBox( this, "groupBox3" );
    groupBox3->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QGridLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );
    spacer12_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox3Layout->addItem( spacer12_2, 2, 1 );

    animate_chipTextLabel = new QLabel( groupBox3, "animate_chipTextLabel" );

    groupBox3Layout->addWidget( animate_chipTextLabel, 0, 0 );

    animate_chipComboBox = new QComboBox( FALSE, groupBox3, "animate_chipComboBox" );
    animate_chipComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox3Layout->addWidget( animate_chipComboBox, 1, 0 );
    spacer28_2 = new QSpacerItem( 152, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox3Layout->addItem( spacer28_2, 1, 1 );

    animate_animatePushButton = new QPushButton( groupBox3, "animate_animatePushButton" );
    animate_animatePushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox3Layout->addWidget( animate_animatePushButton, 6, 1 );

    animate_createPushButton = new QPushButton( groupBox3, "animate_createPushButton" );
    animate_createPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox3Layout->addWidget( animate_createPushButton, 5, 1 );
    spacer14 = new QSpacerItem( 76, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox3Layout->addItem( spacer14, 5, 0 );
    spacer11 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox3Layout->addItem( spacer11, 4, 1 );

    layout1 = new QGridLayout( 0, 1, 1, 0, 6, "layout1"); 
    spacer19 = new QSpacerItem( 82, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    layout1->addItem( spacer19, 5, 2 );

    animate_delaySpinBox = new QSpinBox( groupBox3, "animate_delaySpinBox" );
    animate_delaySpinBox->setFocusPolicy( QSpinBox::NoFocus );
    animate_delaySpinBox->setMaxValue( 9 );

    layout1->addWidget( animate_delaySpinBox, 5, 1 );

    animate_rangeComboBox = new QComboBox( FALSE, groupBox3, "animate_rangeComboBox" );
    animate_rangeComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, animate_rangeComboBox->sizePolicy().hasHeightForWidth() ) );
    animate_rangeComboBox->setMinimumSize( QSize( 166, 0 ) );
    animate_rangeComboBox->setMaximumSize( QSize( 166, 32767 ) );
    animate_rangeComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout1->addMultiCellWidget( animate_rangeComboBox, 0, 0, 1, 2 );

    animate_contrastSpinBox = new QSpinBox( groupBox3, "animate_contrastSpinBox" );
    animate_contrastSpinBox->setMaxValue( 9 );
    animate_contrastSpinBox->setMinValue( -9 );

    layout1->addWidget( animate_contrastSpinBox, 2, 1 );

    animate_TIFFminLineEdit = new QLineEdit( groupBox3, "animate_TIFFminLineEdit" );
    animate_TIFFminLineEdit->setMinimumSize( QSize( 80, 0 ) );
    animate_TIFFminLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    animate_TIFFminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout1->addWidget( animate_TIFFminLineEdit, 1, 1 );

    animate_maskCheckBox = new QCheckBox( groupBox3, "animate_maskCheckBox" );
    animate_maskCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    layout1->addMultiCellWidget( animate_maskCheckBox, 4, 4, 0, 2 );

    animate_contrastTextLabel = new QLabel( groupBox3, "animate_contrastTextLabel" );

    layout1->addWidget( animate_contrastTextLabel, 2, 0 );
    spacer12 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout1->addItem( spacer12, 3, 2 );
    spacer15 = new QSpacerItem( 82, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer15, 2, 2 );

    animate_minmaxTextLabel = new QLabel( groupBox3, "animate_minmaxTextLabel" );

    layout1->addWidget( animate_minmaxTextLabel, 1, 0 );

    animate_TIFFmaxLineEdit = new QLineEdit( groupBox3, "animate_TIFFmaxLineEdit" );
    animate_TIFFmaxLineEdit->setMinimumSize( QSize( 80, 0 ) );
    animate_TIFFmaxLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    animate_TIFFmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout1->addWidget( animate_TIFFmaxLineEdit, 1, 2 );

    textLabel1_8 = new QLabel( groupBox3, "textLabel1_8" );
    textLabel1_8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, textLabel1_8->sizePolicy().hasHeightForWidth() ) );
    textLabel1_8->setFocusPolicy( QLabel::NoFocus );

    layout1->addWidget( textLabel1_8, 5, 0 );

    textLabel4 = new QLabel( groupBox3, "textLabel4" );

    layout1->addWidget( textLabel4, 0, 0 );

    groupBox3Layout->addMultiCellLayout( layout1, 3, 3, 0, 1 );

    animateLayout->addMultiCellWidget( groupBox3, 1, 1, 1, 2 );

    groupBox9 = new QGroupBox( this, "groupBox9" );
    groupBox9->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    groupBox9->setColumnLayout(0, Qt::Vertical );
    groupBox9->layout()->setSpacing( 6 );
    groupBox9->layout()->setMargin( 11 );
    groupBox9Layout = new QGridLayout( groupBox9->layout() );
    groupBox9Layout->setAlignment( Qt::AlignTop );

    animate_selectdirPushButton = new QPushButton( groupBox9, "animate_selectdirPushButton" );
    animate_selectdirPushButton->setMinimumSize( QSize( 160, 0 ) );

    groupBox9Layout->addWidget( animate_selectdirPushButton, 0, 2 );
    spacer42 = new QSpacerItem( 174, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    groupBox9Layout->addItem( spacer42, 0, 1 );

    textLabel1 = new QLabel( groupBox9, "textLabel1" );

    groupBox9Layout->addWidget( textLabel1, 0, 0 );

    animate_dirLineEdit = new QLineEdit( groupBox9, "animate_dirLineEdit" );
    animate_dirLineEdit->setMinimumSize( QSize( 0, 0 ) );
    animate_dirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox9Layout->addMultiCellWidget( animate_dirLineEdit, 1, 1, 0, 2 );

    animateLayout->addMultiCellWidget( groupBox9, 0, 0, 0, 2 );
    spacer42_2 = new QSpacerItem( 450, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    animateLayout->addMultiCell( spacer42_2, 2, 2, 0, 1 );

    closePushButton = new QPushButton( this, "closePushButton" );
    closePushButton->setMinimumSize( QSize( 80, 0 ) );
    closePushButton->setMaximumSize( QSize( 80, 32767 ) );

    animateLayout->addWidget( closePushButton, 2, 2 );
    languageChange();
    resize( QSize(591, 555).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( animate_selectdirPushButton, SIGNAL( clicked() ), this, SLOT( select_dir() ) );
    connect( animate_dirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_main() ) );
    connect( animate_divxSpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( fill_in_moviedivisions() ) );
    connect( animate_divListBox, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( update_animatebuttontext() ) );
    connect( animate_divySpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( fill_in_moviedivisions() ) );
    connect( animate_TIFFmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( animate_TIFFminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( animate_rangeComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setstate_manualthresholds() ) );
    connect( animate_createPushButton, SIGNAL( clicked() ), this, SLOT( make_movie_step1() ) );
    connect( animate_animatePushButton, SIGNAL( clicked() ), this, SLOT( animate_division() ) );
    connect( closePushButton, SIGNAL( clicked() ), this, SLOT( quit() ) );

    // tab order
    setTabOrder( animate_coaddListBox, animate_TIFFminLineEdit );
    setTabOrder( animate_TIFFminLineEdit, animate_TIFFmaxLineEdit );
    setTabOrder( animate_TIFFmaxLineEdit, animate_delaySpinBox );
    setTabOrder( animate_delaySpinBox, animate_divListBox );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
animate::~animate()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void animate::languageChange()
{
    setCaption( tr( "Animate resampled images" ) );
    groupBox5_3->setTitle( QString::null );
    QToolTip::add( animate_overlapSpinBox, tr( "Adjust the overlap between divisions" ) );
    textLabel11->setText( tr( "Field overlap [pixel]" ) );
    textLabel10_2->setText( tr( "<b>Select field(s)</b>" ) );
    QToolTip::add( animate_coaddListBox, tr( "Select the coadditions from which you want to make a movie" ) );
    textLabel1_7->setText( tr( "<b>Select coaddition(s)</b>" ) );
    textLabel2_3->setText( tr( "Number of fields along x" ) );
    textLabel3_2->setText( tr( "Number of fields along y" ) );
    groupBox3->setTitle( QString::null );
    animate_chipTextLabel->setText( tr( "Select chip" ) );
    animate_animatePushButton->setText( tr( "Animate division" ) );
    animate_createPushButton->setText( tr( "Create animation(s)" ) );
    QToolTip::add( animate_delaySpinBox, tr( "The delay of the images in the animated GIF in units of 1/10th seconds." ) );
    animate_rangeComboBox->clear();
    animate_rangeComboBox->insertItem( tr( "Manual" ) );
    animate_rangeComboBox->insertItem( tr( "Automatic" ) );
    QToolTip::add( animate_rangeComboBox, tr( "Manual or automatic (z-scale style) scaling" ) );
    QToolTip::add( animate_contrastSpinBox, tr( "Contrast range: -9 ... 9. default: 0" ) );
    animate_maskCheckBox->setText( tr( "Mask pixels with weight zero" ) );
    QToolTip::add( animate_maskCheckBox, tr( "If activated, pixel with zero weight will be set to zero in the movie" ) );
    animate_contrastTextLabel->setText( tr( "Contrast" ) );
    animate_minmaxTextLabel->setText( tr( "min | max" ) );
    textLabel1_8->setText( tr( "Frame delay:" ) );
    textLabel4->setText( tr( "Dynamic range" ) );
    groupBox9->setTitle( QString::null );
    animate_selectdirPushButton->setText( tr( "Select dir ..." ) );
    textLabel1->setText( tr( "<b>SCIENCE dir:</b>" ) );
    QToolTip::add( animate_dirLineEdit, tr( "The science directory with individual coadd directories." ) );
    closePushButton->setText( tr( "Close" ) );
}

