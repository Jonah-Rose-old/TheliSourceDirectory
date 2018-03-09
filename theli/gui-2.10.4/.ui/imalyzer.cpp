/****************************************************************************
** Form implementation generated from reading ui file 'imalyzer.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "imalyzer.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../imalyzer.ui.h"
/*
 *  Constructs a imalyzer as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
imalyzer::imalyzer( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "imalyzer" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    imalyzerLayout = new QGridLayout( this, 1, 1, 11, 6, "imalyzerLayout"); 

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

    ia_filterLineEdit = new QLineEdit( groupBox5, "ia_filterLineEdit" );
    ia_filterLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, ia_filterLineEdit->sizePolicy().hasHeightForWidth() ) );
    ia_filterLineEdit->setMinimumSize( QSize( 160, 0 ) );
    ia_filterLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ia_filterLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox5Layout->addWidget( ia_filterLineEdit, 1, 1 );

    ia_selectPushButton = new QPushButton( groupBox5, "ia_selectPushButton" );
    ia_selectPushButton->setMinimumSize( QSize( 200, 0 ) );
    ia_selectPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox5Layout->addMultiCellWidget( ia_selectPushButton, 1, 1, 2, 3 );

    ia_dirLineEdit = new QLineEdit( groupBox5, "ia_dirLineEdit" );
    ia_dirLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, ia_dirLineEdit->sizePolicy().hasHeightForWidth() ) );
    ia_dirLineEdit->setMinimumSize( QSize( 380, 0 ) );
    ia_dirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ia_dirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox5Layout->addMultiCellWidget( ia_dirLineEdit, 0, 0, 1, 3 );

    ia_analysePushButton = new QPushButton( groupBox5, "ia_analysePushButton" );
    ia_analysePushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, ia_analysePushButton->sizePolicy().hasHeightForWidth() ) );
    ia_analysePushButton->setPaletteForegroundColor( QColor( 0, 0, 255 ) );
    ia_analysePushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox5Layout->addMultiCellWidget( ia_analysePushButton, 2, 2, 1, 2 );

    ia_analyseabortPushButton = new QPushButton( groupBox5, "ia_analyseabortPushButton" );
    ia_analyseabortPushButton->setMinimumSize( QSize( 120, 0 ) );
    ia_analyseabortPushButton->setMaximumSize( QSize( 120, 32767 ) );
    ia_analyseabortPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    ia_analyseabortPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox5Layout->addWidget( ia_analyseabortPushButton, 2, 3 );

    imalyzerLayout->addMultiCellWidget( groupBox5, 0, 0, 0, 1 );

    groupBox22 = new QGroupBox( this, "groupBox22" );
    groupBox22->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, groupBox22->sizePolicy().hasHeightForWidth() ) );
    groupBox22->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    groupBox22->setColumnLayout(0, Qt::Vertical );
    groupBox22->layout()->setSpacing( 6 );
    groupBox22->layout()->setMargin( 11 );
    groupBox22Layout = new QGridLayout( groupBox22->layout() );
    groupBox22Layout->setAlignment( Qt::AlignTop );

    ia_analysis1TextLabel = new QLabel( groupBox22, "ia_analysis1TextLabel" );

    groupBox22Layout->addMultiCellWidget( ia_analysis1TextLabel, 0, 0, 0, 1 );

    textLabel1_2 = new QLabel( groupBox22, "textLabel1_2" );

    groupBox22Layout->addMultiCellWidget( textLabel1_2, 2, 2, 0, 1 );

    ia_makeplotsPushButton = new QPushButton( groupBox22, "ia_makeplotsPushButton" );
    ia_makeplotsPushButton->setMinimumSize( QSize( 120, 0 ) );
    ia_makeplotsPushButton->setMaximumSize( QSize( 120, 32767 ) );
    ia_makeplotsPushButton->setPaletteForegroundColor( QColor( 0, 0, 255 ) );
    ia_makeplotsPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox22Layout->addWidget( ia_makeplotsPushButton, 4, 2 );

    ia_makeplotabortPushButton = new QPushButton( groupBox22, "ia_makeplotabortPushButton" );
    ia_makeplotabortPushButton->setMinimumSize( QSize( 120, 0 ) );
    ia_makeplotabortPushButton->setMaximumSize( QSize( 120, 32767 ) );
    ia_makeplotabortPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    ia_makeplotabortPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox22Layout->addWidget( ia_makeplotabortPushButton, 4, 3 );
    spacer7_2 = new QSpacerItem( 376, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    groupBox22Layout->addMultiCell( spacer7_2, 4, 4, 0, 1 );

    groupBox3 = new QGroupBox( groupBox22, "groupBox3" );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QGridLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );

    ia_gridCheckBox = new QCheckBox( groupBox3, "ia_gridCheckBox" );
    ia_gridCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox3Layout->addWidget( ia_gridCheckBox, 4, 0 );

    ia_mediancontourlineCheckBox = new QCheckBox( groupBox3, "ia_mediancontourlineCheckBox" );
    ia_mediancontourlineCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox3Layout->addMultiCellWidget( ia_mediancontourlineCheckBox, 3, 3, 0, 1 );

    textLabel1_6 = new QLabel( groupBox3, "textLabel1_6" );

    groupBox3Layout->addWidget( textLabel1_6, 2, 0 );

    ia_unitsComboBox = new QComboBox( FALSE, groupBox3, "ia_unitsComboBox" );
    ia_unitsComboBox->setMinimumSize( QSize( 120, 0 ) );
    ia_unitsComboBox->setMaximumSize( QSize( 120, 32767 ) );
    ia_unitsComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox3Layout->addWidget( ia_unitsComboBox, 2, 1 );

    ia_titleLineEdit = new QLineEdit( groupBox3, "ia_titleLineEdit" );
    ia_titleLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addMultiCellWidget( ia_titleLineEdit, 1, 1, 0, 1 );

    textLabel11 = new QLabel( groupBox3, "textLabel11" );

    groupBox3Layout->addMultiCellWidget( textLabel11, 0, 0, 0, 1 );

    groupBox22Layout->addWidget( groupBox3, 3, 0 );
    spacer5_2 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox22Layout->addItem( spacer5_2, 3, 1 );

    buttonGroup1 = new QButtonGroup( groupBox22, "buttonGroup1" );
    buttonGroup1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, buttonGroup1->sizePolicy().hasHeightForWidth() ) );
    buttonGroup1->setProperty( "selectedId", -1 );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QGridLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    ia_scalemaxTextLabel = new QLabel( buttonGroup1, "ia_scalemaxTextLabel" );

    buttonGroup1Layout->addWidget( ia_scalemaxTextLabel, 3, 2 );

    ia_scaleminTextLabel = new QLabel( buttonGroup1, "ia_scaleminTextLabel" );

    buttonGroup1Layout->addWidget( ia_scaleminTextLabel, 3, 1 );

    ia_scalemaxLineEdit = new QLineEdit( buttonGroup1, "ia_scalemaxLineEdit" );
    ia_scalemaxLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ia_scalemaxLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ia_scalemaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    buttonGroup1Layout->addWidget( ia_scalemaxLineEdit, 2, 2 );

    ia_scaleabsoluteRadioButton = new QRadioButton( buttonGroup1, "ia_scaleabsoluteRadioButton" );
    ia_scaleabsoluteRadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup1Layout->addWidget( ia_scaleabsoluteRadioButton, 2, 0 );

    ia_scaleminLineEdit = new QLineEdit( buttonGroup1, "ia_scaleminLineEdit" );
    ia_scaleminLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ia_scaleminLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ia_scaleminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    buttonGroup1Layout->addWidget( ia_scaleminLineEdit, 2, 1 );

    ia_scalefullrangeRadioButton = new QRadioButton( buttonGroup1, "ia_scalefullrangeRadioButton" );
    ia_scalefullrangeRadioButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ia_scalefullrangeRadioButton->sizePolicy().hasHeightForWidth() ) );
    ia_scalefullrangeRadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup1Layout->addMultiCellWidget( ia_scalefullrangeRadioButton, 0, 0, 0, 2 );

    ia_scalemaxRadioButton = new QRadioButton( buttonGroup1, "ia_scalemaxRadioButton" );
    ia_scalemaxRadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup1Layout->addMultiCellWidget( ia_scalemaxRadioButton, 1, 1, 0, 1 );

    ia_scalecolorrangeLineEdit = new QLineEdit( buttonGroup1, "ia_scalecolorrangeLineEdit" );
    ia_scalecolorrangeLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ia_scalecolorrangeLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ia_scalecolorrangeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    buttonGroup1Layout->addWidget( ia_scalecolorrangeLineEdit, 1, 2 );

    groupBox22Layout->addMultiCellWidget( buttonGroup1, 3, 3, 2, 3 );

    groupBox4 = new QGroupBox( groupBox22, "groupBox4" );

    ia_fwhmsplinesmoothCheckBox = new QCheckBox( groupBox4, "ia_fwhmsplinesmoothCheckBox" );
    ia_fwhmsplinesmoothCheckBox->setGeometry( QRect( 10, 10, 310, 20 ) );

    ia_interpolateanisoCheckBox = new QCheckBox( groupBox4, "ia_interpolateanisoCheckBox" );
    ia_interpolateanisoCheckBox->setGeometry( QRect( 10, 40, 310, 20 ) );

    groupBox22Layout->addWidget( groupBox4, 1, 0 );

    imalyzerLayout->addMultiCellWidget( groupBox22, 1, 1, 0, 1 );
    spacer5 = new QSpacerItem( 510, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    imalyzerLayout->addItem( spacer5, 2, 0 );

    ia_closePushButton = new QPushButton( this, "ia_closePushButton" );
    ia_closePushButton->setFocusPolicy( QPushButton::NoFocus );

    imalyzerLayout->addWidget( ia_closePushButton, 2, 1 );
    spacer4 = new QSpacerItem( 20, 60, QSizePolicy::Minimum, QSizePolicy::Fixed );
    imalyzerLayout->addItem( spacer4, 3, 1 );
    languageChange();
    resize( QSize(672, 494).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ia_closePushButton, SIGNAL( clicked() ), this, SLOT( quit() ) );
    connect( ia_dirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ia_filterLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ia_selectPushButton, SIGNAL( clicked() ), this, SLOT( select_dirimage() ) );
    connect( ia_titleLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ia_analysePushButton, SIGNAL( clicked() ), this, SLOT( analyse_images() ) );
    connect( ia_makeplotsPushButton, SIGNAL( clicked() ), this, SLOT( makeplot_images() ) );
    connect( ia_scaleminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ia_scalemaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ia_unitsComboBox, SIGNAL( activated(const QString&) ), this, SLOT( ia_showhide() ) );
    connect( ia_scalecolorrangeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ia_analyseabortPushButton, SIGNAL( clicked() ), this, SLOT( kill_analyse() ) );
    connect( ia_makeplotabortPushButton, SIGNAL( clicked() ), this, SLOT( kill_makeplot() ) );
    connect( ia_scalefullrangeRadioButton, SIGNAL( clicked() ), this, SLOT( radioselect() ) );
    connect( ia_scalemaxRadioButton, SIGNAL( clicked() ), this, SLOT( radioselect() ) );
    connect( ia_scaleabsoluteRadioButton, SIGNAL( clicked() ), this, SLOT( radioselect() ) );

    // tab order
    setTabOrder( ia_dirLineEdit, ia_filterLineEdit );
    setTabOrder( ia_filterLineEdit, ia_titleLineEdit );
    setTabOrder( ia_titleLineEdit, ia_scalecolorrangeLineEdit );
    setTabOrder( ia_scalecolorrangeLineEdit, ia_scaleminLineEdit );
    setTabOrder( ia_scaleminLineEdit, ia_scalemaxLineEdit );
    setTabOrder( ia_scalemaxLineEdit, ia_analyseabortPushButton );
    setTabOrder( ia_analyseabortPushButton, ia_unitsComboBox );
    setTabOrder( ia_unitsComboBox, ia_scaleabsoluteRadioButton );
    setTabOrder( ia_scaleabsoluteRadioButton, ia_scalefullrangeRadioButton );
    setTabOrder( ia_scalefullrangeRadioButton, ia_scalemaxRadioButton );
    setTabOrder( ia_scalemaxRadioButton, ia_makeplotabortPushButton );
    setTabOrder( ia_makeplotabortPushButton, ia_closePushButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
imalyzer::~imalyzer()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void imalyzer::languageChange()
{
    setCaption( tr( "Imalyzer" ) );
    groupBox5->setTitle( QString::null );
    textLabel1->setText( tr( "Directory with images:" ) );
    textLabel2->setText( tr( "Name filter (e.g. *OFC.fits):" ) );
    QToolTip::add( ia_filterLineEdit, tr( "A filter for the images, e.g. \"*1OFCS.fits\", or the name of an individual file. If left empty, it is set to \"*.fits\"<br>" ) );
    ia_selectPushButton->setText( tr( "Select image" ) );
    QToolTip::add( ia_dirLineEdit, tr( "Directory with the images that should be analysed." ) );
    ia_analysePushButton->setText( tr( "Analyse PSF (once per name filter)" ) );
    QToolTip::add( ia_analysePushButton, tr( "Performs an extensive PSF analysis of the images" ) );
    ia_analyseabortPushButton->setText( tr( "Abort" ) );
    QToolTip::add( ia_analyseabortPushButton, tr( "Aborts the PSF analysis." ) );
    groupBox22->setTitle( QString::null );
    ia_analysis1TextLabel->setText( tr( "<b>Analysis options</b>" ) );
    textLabel1_2->setText( tr( "<b>Plotting options</b>" ) );
    ia_makeplotsPushButton->setText( tr( "Make plots" ) );
    ia_makeplotabortPushButton->setText( tr( "Abort" ) );
    QToolTip::add( ia_makeplotabortPushButton, tr( "Aborts the creation of image quality plots." ) );
    groupBox3->setTitle( QString::null );
    ia_gridCheckBox->setText( tr( "Show data on a regular grid" ) );
    QToolTip::add( ia_gridCheckBox, tr( "Plots a white contour line at the median FWHM level." ) );
    ia_mediancontourlineCheckBox->setText( tr( "Show median FWHM contour line" ) );
    QToolTip::add( ia_mediancontourlineCheckBox, tr( "Plots a white contour line at the median FWHM level." ) );
    textLabel1_6->setText( tr( "FWHM units:" ) );
    ia_unitsComboBox->clear();
    ia_unitsComboBox->insertItem( tr( "arcsec" ) );
    ia_unitsComboBox->insertItem( tr( "pixel" ) );
    ia_unitsComboBox->insertItem( tr( "minFWHM" ) );
    QToolTip::add( ia_titleLineEdit, tr( "Unambiguous title of the analysis" ) );
    textLabel11->setText( tr( "Title of the analysis:" ) );
    buttonGroup1->setTitle( tr( "FWHM colour scaling" ) );
    ia_scalemaxTextLabel->setText( tr( "max" ) );
    ia_scaleminTextLabel->setText( tr( "min" ) );
    QToolTip::add( ia_scalemaxLineEdit, tr( "The largest FWHM shown. Same for all images.<br>" ) );
    ia_scaleabsoluteRadioButton->setText( tr( "absolute:" ) );
    QToolTip::add( ia_scaleabsoluteRadioButton, tr( "Use identical FWHM ranges for all images." ) );
    QToolTip::add( ia_scaleminLineEdit, tr( "The lowest FWHM shown. Same for all images.<br>" ) );
    ia_scalefullrangeRadioButton->setText( tr( "full range (min max automatic)" ) );
    QToolTip::add( ia_scalefullrangeRadioButton, tr( "Use all available colours. Images will be treated independently from each other.<br>" ) );
    ia_scalemaxRadioButton->setText( tr( "max (in % of min):" ) );
    QToolTip::add( ia_scalemaxRadioButton, tr( "If set to e.g. 50%, the upper FWHM limit shown will be 50% larger than the smallest FWHM.<br>" ) );
    QToolTip::add( ia_scalecolorrangeLineEdit, tr( "If set to e.g. 50%, the upper FWHM limit shown will be 50% larger than the smallest FWHM.<br>" ) );
    groupBox4->setTitle( QString::null );
    ia_fwhmsplinesmoothCheckBox->setText( tr( "Spline interpolation for FWHM map: smooth" ) );
    QToolTip::add( ia_fwhmsplinesmoothCheckBox, tr( "Check if you want a smoothed spline interpolation, as compared to an unsmoothed interpolation." ) );
    ia_interpolateanisoCheckBox->setText( tr( "Interpolate PSF anisotropy" ) );
    QToolTip::add( ia_interpolateanisoCheckBox, tr( "Check if the PSF anisotropy sticks represent actual measured values, or a smoothed polynomial" ) );
    ia_closePushButton->setText( tr( "Close" ) );
}

