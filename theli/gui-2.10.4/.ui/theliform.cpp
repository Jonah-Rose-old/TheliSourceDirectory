/****************************************************************************
** Form implementation generated from reading ui file 'theliform.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "theliform.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlistbox.h>
#include <qprogressbar.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qtextedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../theliform.ui.h"
/*
 *  Constructs a theliForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
theliForm::theliForm( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "theliForm" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setPaletteBackgroundColor( QColor( 255, 244, 230 ) );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );
    theliFormLayout = new QGridLayout( centralWidget(), 1, 1, 11, 6, "theliFormLayout"); 

    mainmenu_TabWidget = new QTabWidget( centralWidget(), "mainmenu_TabWidget" );
    mainmenu_TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)3, 0, 0, mainmenu_TabWidget->sizePolicy().hasHeightForWidth() ) );
    mainmenu_TabWidget->setPaletteBackgroundColor( QColor( 135, 135, 135 ) );
    mainmenu_TabWidget->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    mainmenu_TabWidget->setFocusPolicy( QTabWidget::NoFocus );

    PageInitialise = new QWidget( mainmenu_TabWidget, "PageInitialise" );
    PageInitialiseLayout = new QGridLayout( PageInitialise, 1, 1, 11, 6, "PageInitialiseLayout"); 

    in_instselectGroupBox = new QGroupBox( PageInitialise, "in_instselectGroupBox" );
    in_instselectGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, in_instselectGroupBox->sizePolicy().hasHeightForWidth() ) );
    in_instselectGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    in_instselectGroupBox->setFrameShadow( QGroupBox::Sunken );
    in_instselectGroupBox->setLineWidth( 1 );
    in_instselectGroupBox->setColumnLayout(0, Qt::Vertical );
    in_instselectGroupBox->layout()->setSpacing( 6 );
    in_instselectGroupBox->layout()->setMargin( 11 );
    in_instselectGroupBoxLayout = new QGridLayout( in_instselectGroupBox->layout() );
    in_instselectGroupBoxLayout->setAlignment( Qt::AlignTop );

    in_createselfPushButton = new QPushButton( in_instselectGroupBox, "in_createselfPushButton" );
    in_createselfPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, in_createselfPushButton->sizePolicy().hasHeightForWidth() ) );
    in_createselfPushButton->setMinimumSize( QSize( 80, 0 ) );
    in_createselfPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_instselectGroupBoxLayout->addWidget( in_createselfPushButton, 0, 0 );

    in_instButtonGroup = new QButtonGroup( in_instselectGroupBox, "in_instButtonGroup" );
    in_instButtonGroup->setFocusPolicy( QButtonGroup::ClickFocus );
    in_instButtonGroup->setLineWidth( 0 );
    in_instButtonGroup->setExclusive( FALSE );
    in_instButtonGroup->setProperty( "selectedId", -1 );
    in_instButtonGroup->setColumnLayout(0, Qt::Vertical );
    in_instButtonGroup->layout()->setSpacing( 6 );
    in_instButtonGroup->layout()->setMargin( 11 );
    in_instButtonGroupLayout = new QGridLayout( in_instButtonGroup->layout() );
    in_instButtonGroupLayout->setAlignment( Qt::AlignTop );

    in_profinstRadioButton = new QRadioButton( in_instButtonGroup, "in_profinstRadioButton" );
    in_profinstRadioButton->setFocusPolicy( QRadioButton::ClickFocus );

    in_instButtonGroupLayout->addWidget( in_profinstRadioButton, 0, 0 );

    in_comminstRadioButton = new QRadioButton( in_instButtonGroup, "in_comminstRadioButton" );
    in_comminstRadioButton->setFocusPolicy( QRadioButton::ClickFocus );

    in_instButtonGroupLayout->addWidget( in_comminstRadioButton, 1, 0 );

    in_userinstRadioButton = new QRadioButton( in_instButtonGroup, "in_userinstRadioButton" );
    in_userinstRadioButton->setFocusPolicy( QRadioButton::ClickFocus );

    in_instButtonGroupLayout->addWidget( in_userinstRadioButton, 2, 0 );

    in_instselectGroupBoxLayout->addWidget( in_instButtonGroup, 0, 1 );

    in_instrumentListBox = new QListBox( in_instselectGroupBox, "in_instrumentListBox" );
    in_instrumentListBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, in_instrumentListBox->sizePolicy().hasHeightForWidth() ) );
    in_instrumentListBox->setFocusPolicy( QListBox::NoFocus );

    in_instselectGroupBoxLayout->addMultiCellWidget( in_instrumentListBox, 1, 1, 0, 1 );

    PageInitialiseLayout->addMultiCellWidget( in_instselectGroupBox, 0, 1, 1, 1 );

    layout24 = new QHBoxLayout( 0, 0, 6, "layout24"); 

    diskspaceProgressBar = new QProgressBar( PageInitialise, "diskspaceProgressBar" );
    diskspaceProgressBar->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, diskspaceProgressBar->sizePolicy().hasHeightForWidth() ) );
    diskspaceProgressBar->setPaletteBackgroundColor( QColor( 148, 148, 148 ) );
    diskspaceProgressBar->setFrameShadow( QProgressBar::MShadow );
    diskspaceProgressBar->setPercentageVisible( FALSE );
    layout24->addWidget( diskspaceProgressBar );
    spacer182_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout24->addItem( spacer182_2 );

    tf_instrumentLineEdit = new QLineEdit( PageInitialise, "tf_instrumentLineEdit" );
    tf_instrumentLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tf_instrumentLineEdit->sizePolicy().hasHeightForWidth() ) );
    tf_instrumentLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    tf_instrumentLineEdit->setReadOnly( TRUE );
    layout24->addWidget( tf_instrumentLineEdit );

    PageInitialiseLayout->addMultiCellLayout( layout24, 2, 2, 0, 1 );
    spacer31_2 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Minimum );
    PageInitialiseLayout->addItem( spacer31_2, 1, 0 );

    layout21 = new QVBoxLayout( 0, 0, 6, "layout21"); 

    createsaveresumeGroupBox = new QGroupBox( PageInitialise, "createsaveresumeGroupBox" );
    createsaveresumeGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, createsaveresumeGroupBox->sizePolicy().hasHeightForWidth() ) );
    createsaveresumeGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    createsaveresumeGroupBox->setColumnLayout(0, Qt::Vertical );
    createsaveresumeGroupBox->layout()->setSpacing( 6 );
    createsaveresumeGroupBox->layout()->setMargin( 11 );
    createsaveresumeGroupBoxLayout = new QGridLayout( createsaveresumeGroupBox->layout() );
    createsaveresumeGroupBoxLayout->setAlignment( Qt::AlignTop );

    in_loadlogPushButton = new QPushButton( createsaveresumeGroupBox, "in_loadlogPushButton" );
    in_loadlogPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, in_loadlogPushButton->sizePolicy().hasHeightForWidth() ) );
    in_loadlogPushButton->setMinimumSize( QSize( 80, 0 ) );
    in_loadlogPushButton->setFocusPolicy( QPushButton::NoFocus );

    createsaveresumeGroupBoxLayout->addWidget( in_loadlogPushButton, 0, 2 );

    in_reductionlogLineEdit = new QLineEdit( createsaveresumeGroupBox, "in_reductionlogLineEdit" );
    in_reductionlogLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, in_reductionlogLineEdit->sizePolicy().hasHeightForWidth() ) );
    in_reductionlogLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    in_reductionlogLineEdit->setMaxLength( 99 );
    in_reductionlogLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    createsaveresumeGroupBoxLayout->addMultiCellWidget( in_reductionlogLineEdit, 1, 1, 0, 1 );

    in_clearprocstatusPushButton = new QPushButton( createsaveresumeGroupBox, "in_clearprocstatusPushButton" );
    in_clearprocstatusPushButton->setFocusPolicy( QPushButton::NoFocus );

    createsaveresumeGroupBoxLayout->addWidget( in_clearprocstatusPushButton, 1, 2 );

    in_homediskwarnSpinBox = new QSpinBox( createsaveresumeGroupBox, "in_homediskwarnSpinBox" );
    in_homediskwarnSpinBox->setMinimumSize( QSize( 90, 0 ) );
    in_homediskwarnSpinBox->setMaximumSize( QSize( 90, 32767 ) );
    in_homediskwarnSpinBox->setMaxValue( 99900 );
    in_homediskwarnSpinBox->setMinValue( 100 );
    in_homediskwarnSpinBox->setLineStep( 100 );
    in_homediskwarnSpinBox->setValue( 100 );

    createsaveresumeGroupBoxLayout->addWidget( in_homediskwarnSpinBox, 3, 2 );

    in_datadiskwarnSpinBox = new QSpinBox( createsaveresumeGroupBox, "in_datadiskwarnSpinBox" );
    in_datadiskwarnSpinBox->setMinimumSize( QSize( 90, 0 ) );
    in_datadiskwarnSpinBox->setMaximumSize( QSize( 90, 32767 ) );
    in_datadiskwarnSpinBox->setMaxValue( 99900 );
    in_datadiskwarnSpinBox->setMinValue( 100 );
    in_datadiskwarnSpinBox->setLineStep( 100 );
    in_datadiskwarnSpinBox->setValue( 100 );

    createsaveresumeGroupBoxLayout->addWidget( in_datadiskwarnSpinBox, 2, 2 );

    textLabel1_6_2 = new QLabel( createsaveresumeGroupBox, "textLabel1_6_2" );

    createsaveresumeGroupBoxLayout->addMultiCellWidget( textLabel1_6_2, 3, 3, 0, 1 );

    textLabel1_6 = new QLabel( createsaveresumeGroupBox, "textLabel1_6" );

    createsaveresumeGroupBoxLayout->addMultiCellWidget( textLabel1_6, 2, 2, 0, 1 );

    in_nframesTextLabel = new QLabel( createsaveresumeGroupBox, "in_nframesTextLabel" );
    in_nframesTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, in_nframesTextLabel->sizePolicy().hasHeightForWidth() ) );

    createsaveresumeGroupBoxLayout->addWidget( in_nframesTextLabel, 3, 3 );

    in_nframesSpinBox = new QSpinBox( createsaveresumeGroupBox, "in_nframesSpinBox" );
    in_nframesSpinBox->setMinimumSize( QSize( 70, 0 ) );
    in_nframesSpinBox->setMaximumSize( QSize( 70, 32767 ) );
    in_nframesSpinBox->setFocusPolicy( QSpinBox::StrongFocus );
    in_nframesSpinBox->setButtonSymbols( QSpinBox::UpDownArrows );
    in_nframesSpinBox->setMaxValue( 1000 );
    in_nframesSpinBox->setMinValue( 1 );
    in_nframesSpinBox->setValue( 5 );

    createsaveresumeGroupBoxLayout->addWidget( in_nframesSpinBox, 3, 4 );

    in_TextLabel3_2 = new QLabel( createsaveresumeGroupBox, "in_TextLabel3_2" );
    in_TextLabel3_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, in_TextLabel3_2->sizePolicy().hasHeightForWidth() ) );

    createsaveresumeGroupBoxLayout->addWidget( in_TextLabel3_2, 2, 3 );

    tf_styleSpinBox = new QSpinBox( createsaveresumeGroupBox, "tf_styleSpinBox" );
    tf_styleSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, tf_styleSpinBox->sizePolicy().hasHeightForWidth() ) );
    tf_styleSpinBox->setMinimumSize( QSize( 70, 0 ) );
    tf_styleSpinBox->setMaximumSize( QSize( 70, 32767 ) );
    tf_styleSpinBox->setFocusPolicy( QSpinBox::StrongFocus );
    tf_styleSpinBox->setWrapping( FALSE );
    tf_styleSpinBox->setMaxValue( 1000 );
    tf_styleSpinBox->setMinValue( 1 );

    createsaveresumeGroupBoxLayout->addWidget( tf_styleSpinBox, 2, 4 );

    in_numcpuSpinBox = new QSpinBox( createsaveresumeGroupBox, "in_numcpuSpinBox" );
    in_numcpuSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, in_numcpuSpinBox->sizePolicy().hasHeightForWidth() ) );
    in_numcpuSpinBox->setMinimumSize( QSize( 70, 0 ) );
    in_numcpuSpinBox->setMaximumSize( QSize( 70, 32767 ) );
    in_numcpuSpinBox->setFocusPolicy( QSpinBox::StrongFocus );
    in_numcpuSpinBox->setButtonSymbols( QSpinBox::UpDownArrows );
    in_numcpuSpinBox->setMaxValue( 999 );
    in_numcpuSpinBox->setMinValue( 1 );
    in_numcpuSpinBox->setValue( 1 );

    createsaveresumeGroupBoxLayout->addWidget( in_numcpuSpinBox, 1, 4 );

    in_TextLabel3 = new QLabel( createsaveresumeGroupBox, "in_TextLabel3" );
    in_TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, in_TextLabel3->sizePolicy().hasHeightForWidth() ) );

    createsaveresumeGroupBoxLayout->addWidget( in_TextLabel3, 1, 3 );
    spacer37_3_2 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    createsaveresumeGroupBoxLayout->addItem( spacer37_3_2, 0, 1 );

    textLabel2_3 = new QLabel( createsaveresumeGroupBox, "textLabel2_3" );

    createsaveresumeGroupBoxLayout->addWidget( textLabel2_3, 0, 0 );
    layout21->addWidget( createsaveresumeGroupBox );

    in_dirsettingGroupBox = new QGroupBox( PageInitialise, "in_dirsettingGroupBox" );
    in_dirsettingGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, in_dirsettingGroupBox->sizePolicy().hasHeightForWidth() ) );
    in_dirsettingGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    in_dirsettingGroupBox->setColumnLayout(0, Qt::Vertical );
    in_dirsettingGroupBox->layout()->setSpacing( 6 );
    in_dirsettingGroupBox->layout()->setMargin( 11 );
    in_dirsettingGroupBoxLayout = new QGridLayout( in_dirsettingGroupBox->layout() );
    in_dirsettingGroupBoxLayout->setAlignment( Qt::AlignTop );

    in_biasdirLineEdit = new QLineEdit( in_dirsettingGroupBox, "in_biasdirLineEdit" );
    in_biasdirLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, in_biasdirLineEdit->sizePolicy().hasHeightForWidth() ) );
    in_biasdirLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    in_biasdirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    in_biasdirLineEdit->setMaxLength( 99 );
    in_biasdirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    in_dirsettingGroupBoxLayout->addWidget( in_biasdirLineEdit, 1, 1 );

    in_maindirTextLabel = new QLabel( in_dirsettingGroupBox, "in_maindirTextLabel" );

    in_dirsettingGroupBoxLayout->addWidget( in_maindirTextLabel, 0, 0 );

    in_darkdirTextLabel = new QLabel( in_dirsettingGroupBox, "in_darkdirTextLabel" );

    in_dirsettingGroupBoxLayout->addWidget( in_darkdirTextLabel, 2, 0 );

    in_biasdirTextLabel = new QLabel( in_dirsettingGroupBox, "in_biasdirTextLabel" );

    in_dirsettingGroupBoxLayout->addWidget( in_biasdirTextLabel, 1, 0 );

    in_restoreallPushButton = new QPushButton( in_dirsettingGroupBox, "in_restoreallPushButton" );
    in_restoreallPushButton->setMinimumSize( QSize( 100, 0 ) );
    in_restoreallPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    in_restoreallPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_restoreallPushButton, 0, 2 );

    in_restoredarkPushButton = new QPushButton( in_dirsettingGroupBox, "in_restoredarkPushButton" );
    in_restoredarkPushButton->setMinimumSize( QSize( 100, 0 ) );
    in_restoredarkPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    in_restoredarkPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_restoredarkPushButton, 2, 2 );

    in_restorebiasPushButton = new QPushButton( in_dirsettingGroupBox, "in_restorebiasPushButton" );
    in_restorebiasPushButton->setMinimumSize( QSize( 100, 0 ) );
    in_restorebiasPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    in_restorebiasPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_restorebiasPushButton, 1, 2 );

    in_darkdirLineEdit = new QLineEdit( in_dirsettingGroupBox, "in_darkdirLineEdit" );
    in_darkdirLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    in_darkdirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    in_darkdirLineEdit->setMaxLength( 99 );
    in_darkdirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    in_dirsettingGroupBoxLayout->addWidget( in_darkdirLineEdit, 2, 1 );

    in_maindirLineEdit = new QLineEdit( in_dirsettingGroupBox, "in_maindirLineEdit" );
    in_maindirLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    in_maindirLineEdit->setMaxLength( 900 );
    in_maindirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    in_dirsettingGroupBoxLayout->addWidget( in_maindirLineEdit, 0, 1 );

    in_flatdirTextLabel_2_2 = new QLabel( in_dirsettingGroupBox, "in_flatdirTextLabel_2_2" );

    in_dirsettingGroupBoxLayout->addWidget( in_flatdirTextLabel_2_2, 7, 0 );

    in_flatdirTextLabel_2 = new QLabel( in_dirsettingGroupBox, "in_flatdirTextLabel_2" );

    in_dirsettingGroupBoxLayout->addWidget( in_flatdirTextLabel_2, 6, 0 );

    in_sciencedirTextLabel = new QLabel( in_dirsettingGroupBox, "in_sciencedirTextLabel" );

    in_dirsettingGroupBoxLayout->addWidget( in_sciencedirTextLabel, 5, 0 );

    in_processskyCheckBox = new QCheckBox( in_dirsettingGroupBox, "in_processskyCheckBox" );
    in_processskyCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_processskyCheckBox, 8, 1 );

    in_clearPushButton = new QPushButton( in_dirsettingGroupBox, "in_clearPushButton" );
    in_clearPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, in_clearPushButton->sizePolicy().hasHeightForWidth() ) );
    in_clearPushButton->setMinimumSize( QSize( 100, 0 ) );
    in_clearPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_clearPushButton, 8, 2 );

    in_sciencedirLineEdit = new QLineEdit( in_dirsettingGroupBox, "in_sciencedirLineEdit" );
    in_sciencedirLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    in_sciencedirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    in_sciencedirLineEdit->setMaxLength( 99 );
    in_sciencedirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    in_dirsettingGroupBoxLayout->addWidget( in_sciencedirLineEdit, 5, 1 );

    in_skydirLineEdit = new QLineEdit( in_dirsettingGroupBox, "in_skydirLineEdit" );
    in_skydirLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    in_skydirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    in_skydirLineEdit->setMaxLength( 99 );
    in_skydirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    in_dirsettingGroupBoxLayout->addWidget( in_skydirLineEdit, 6, 1 );

    in_restoreskyPushButton = new QPushButton( in_dirsettingGroupBox, "in_restoreskyPushButton" );
    in_restoreskyPushButton->setMinimumSize( QSize( 100, 0 ) );
    in_restoreskyPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    in_restoreskyPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_restoreskyPushButton, 6, 2 );

    in_restoresciencePushButton = new QPushButton( in_dirsettingGroupBox, "in_restoresciencePushButton" );
    in_restoresciencePushButton->setMinimumSize( QSize( 100, 0 ) );
    in_restoresciencePushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    in_restoresciencePushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_restoresciencePushButton, 5, 2 );

    in_restorestandardPushButton = new QPushButton( in_dirsettingGroupBox, "in_restorestandardPushButton" );
    in_restorestandardPushButton->setMinimumSize( QSize( 100, 0 ) );
    in_restorestandardPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    in_restorestandardPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_restorestandardPushButton, 7, 2 );

    in_standarddirLineEdit = new QLineEdit( in_dirsettingGroupBox, "in_standarddirLineEdit" );
    in_standarddirLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    in_standarddirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    in_standarddirLineEdit->setMaxLength( 99 );
    in_standarddirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    in_dirsettingGroupBoxLayout->addWidget( in_standarddirLineEdit, 7, 1 );

    in_flatdirTextLabel = new QLabel( in_dirsettingGroupBox, "in_flatdirTextLabel" );

    in_dirsettingGroupBoxLayout->addWidget( in_flatdirTextLabel, 3, 0 );

    in_flatdirLineEdit = new QLineEdit( in_dirsettingGroupBox, "in_flatdirLineEdit" );
    in_flatdirLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    in_flatdirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    in_flatdirLineEdit->setMaxLength( 99 );
    in_flatdirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    in_dirsettingGroupBoxLayout->addWidget( in_flatdirLineEdit, 3, 1 );

    in_restoreflatPushButton = new QPushButton( in_dirsettingGroupBox, "in_restoreflatPushButton" );
    in_restoreflatPushButton->setMinimumSize( QSize( 100, 0 ) );
    in_restoreflatPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    in_restoreflatPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_restoreflatPushButton, 3, 2 );

    in_flatdirTextLabel_3 = new QLabel( in_dirsettingGroupBox, "in_flatdirTextLabel_3" );

    in_dirsettingGroupBoxLayout->addWidget( in_flatdirTextLabel_3, 4, 0 );

    in_flatoffdirLineEdit = new QLineEdit( in_dirsettingGroupBox, "in_flatoffdirLineEdit" );
    in_flatoffdirLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    in_flatoffdirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    in_flatoffdirLineEdit->setMaxLength( 99 );
    in_flatoffdirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    in_dirsettingGroupBoxLayout->addWidget( in_flatoffdirLineEdit, 4, 1 );

    in_restoreflatoffPushButton = new QPushButton( in_dirsettingGroupBox, "in_restoreflatoffPushButton" );
    in_restoreflatoffPushButton->setMinimumSize( QSize( 100, 0 ) );
    in_restoreflatoffPushButton->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    in_restoreflatoffPushButton->setFocusPolicy( QPushButton::NoFocus );

    in_dirsettingGroupBoxLayout->addWidget( in_restoreflatoffPushButton, 4, 2 );
    layout21->addWidget( in_dirsettingGroupBox );

    PageInitialiseLayout->addLayout( layout21, 0, 0 );
    mainmenu_TabWidget->insertTab( PageInitialise, QString::fromLatin1("") );

    PageCalibration = new QWidget( mainmenu_TabWidget, "PageCalibration" );
    PageCalibrationLayout = new QGridLayout( PageCalibration, 1, 1, 11, 6, "PageCalibrationLayout"); 

    layout23 = new QGridLayout( 0, 1, 1, 0, 6, "layout23"); 

    groupBox18 = new QGroupBox( PageCalibration, "groupBox18" );
    groupBox18->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, groupBox18->sizePolicy().hasHeightForWidth() ) );
    groupBox18->setMinimumSize( QSize( 520, 350 ) );
    groupBox18->setMaximumSize( QSize( 520, 350 ) );
    groupBox18->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );

    line3 = new QFrame( groupBox18, "line3" );
    line3->setGeometry( QRect( 20, 100, 470, 20 ) );
    line3->setFrameShape( QFrame::HLine );
    line3->setFrameShadow( QFrame::Sunken );
    line3->setFrameShape( QFrame::HLine );

    pre_TextLabel12_2 = new QLabel( groupBox18, "pre_TextLabel12_2" );
    pre_TextLabel12_2->setGeometry( QRect( 220, 230, 220, 20 ) );
    pre_TextLabel12_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_TextLabel12_2->sizePolicy().hasHeightForWidth() ) );

    pre_TextLabel12_2_2 = new QLabel( groupBox18, "pre_TextLabel12_2_2" );
    pre_TextLabel12_2_2->setGeometry( QRect( 220, 280, 270, 20 ) );
    pre_TextLabel12_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_TextLabel12_2_2->sizePolicy().hasHeightForWidth() ) );

    pre_linkchipsLineEdit = new QLineEdit( groupBox18, "pre_linkchipsLineEdit" );
    pre_linkchipsLineEdit->setGeometry( QRect( 220, 250, 270, 24 ) );
    pre_linkchipsLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pre_linkchipsLineEdit->sizePolicy().hasHeightForWidth() ) );
    pre_linkchipsLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    pre_linkchipsLineEdit->setMaxLength( 99 );
    pre_linkchipsLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    pre_scratchdirLineEdit = new QLineEdit( groupBox18, "pre_scratchdirLineEdit" );
    pre_scratchdirLineEdit->setGeometry( QRect( 220, 300, 270, 24 ) );
    pre_scratchdirLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_scratchdirLineEdit->sizePolicy().hasHeightForWidth() ) );
    pre_scratchdirLineEdit->setPaletteBackgroundColor( QColor( 220, 255, 255 ) );
    pre_scratchdirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    pre_scratchdirLineEdit->setMaxLength( 999 );
    pre_scratchdirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    pre_splitCheckBox = new QCheckBox( groupBox18, "pre_splitCheckBox" );
    pre_splitCheckBox->setGeometry( QRect( 20, 190, 280, 22 ) );
    pre_splitCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_splitCheckBox->sizePolicy().hasHeightForWidth() ) );
    pre_splitCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont pre_splitCheckBox_font(  pre_splitCheckBox->font() );
    pre_splitCheckBox_font.setBold( TRUE );
    pre_splitCheckBox->setFont( pre_splitCheckBox_font ); 
    pre_splitCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    pre_splitCheckBox->setChecked( TRUE );

    pre_sortdataCheckBox = new QCheckBox( groupBox18, "pre_sortdataCheckBox" );
    pre_sortdataCheckBox->setGeometry( QRect( 20, 70, 280, 22 ) );
    pre_sortdataCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_sortdataCheckBox->sizePolicy().hasHeightForWidth() ) );
    QFont pre_sortdataCheckBox_font(  pre_sortdataCheckBox->font() );
    pre_sortdataCheckBox_font.setBold( TRUE );
    pre_sortdataCheckBox->setFont( pre_sortdataCheckBox_font ); 
    pre_sortdataCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    pre_createlinksCheckBox = new QCheckBox( groupBox18, "pre_createlinksCheckBox" );
    pre_createlinksCheckBox->setGeometry( QRect( 20, 250, 174, 22 ) );
    pre_createlinksCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_createlinksCheckBox->sizePolicy().hasHeightForWidth() ) );
    QFont pre_createlinksCheckBox_font(  pre_createlinksCheckBox->font() );
    pre_createlinksCheckBox_font.setBold( TRUE );
    pre_createlinksCheckBox->setFont( pre_createlinksCheckBox_font ); 
    pre_createlinksCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    pre_createlinksCheckBox->setChecked( FALSE );

    pre_splitMIRCheckBox = new QCheckBox( groupBox18, "pre_splitMIRCheckBox" );
    pre_splitMIRCheckBox->setGeometry( QRect( 340, 190, 150, 20 ) );

    pre_TextLabel1 = new QLabel( groupBox18, "pre_TextLabel1" );
    pre_TextLabel1->setGeometry( QRect( 20, 30, 220, 24 ) );
    pre_TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, pre_TextLabel1->sizePolicy().hasHeightForWidth() ) );

    pre_TextLabel13 = new QLabel( groupBox18, "pre_TextLabel13" );
    pre_TextLabel13->setGeometry( QRect( 20, 130, 460, 40 ) );

    layout23->addWidget( groupBox18, 0, 0 );

    pre_commandGroupBox = new QGroupBox( PageCalibration, "pre_commandGroupBox" );
    pre_commandGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, pre_commandGroupBox->sizePolicy().hasHeightForWidth() ) );
    pre_commandGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    pre_commandGroupBox->setColumnLayout(0, Qt::Vertical );
    pre_commandGroupBox->layout()->setSpacing( 6 );
    pre_commandGroupBox->layout()->setMargin( 11 );
    pre_commandGroupBoxLayout = new QGridLayout( pre_commandGroupBox->layout() );
    pre_commandGroupBoxLayout->setAlignment( Qt::AlignTop );

    pre_commandTextEdit = new QTextEdit( pre_commandGroupBox, "pre_commandTextEdit" );
    pre_commandTextEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, pre_commandTextEdit->sizePolicy().hasHeightForWidth() ) );
    pre_commandTextEdit->setFocusPolicy( QTextEdit::ClickFocus );
    pre_commandTextEdit->setWordWrap( QTextEdit::NoWrap );

    pre_commandGroupBoxLayout->addWidget( pre_commandTextEdit, 0, 0 );

    layout23->addMultiCellWidget( pre_commandGroupBox, 1, 1, 0, 1 );

    layout22 = new QVBoxLayout( 0, 0, 6, "layout22"); 
    spacer37_3 = new QSpacerItem( 20, 36, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout22->addItem( spacer37_3 );

    layout21_2 = new QGridLayout( 0, 1, 1, 0, 6, "layout21_2"); 

    pre_killPushButton = new QPushButton( PageCalibration, "pre_killPushButton" );
    pre_killPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_killPushButton->sizePolicy().hasHeightForWidth() ) );
    pre_killPushButton->setMinimumSize( QSize( 110, 0 ) );
    pre_killPushButton->setMaximumSize( QSize( 110, 32767 ) );
    pre_killPushButton->setPaletteForegroundColor( QColor( 185, 0, 0 ) );
    pre_killPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout21_2->addWidget( pre_killPushButton, 1, 1 );

    pre_configurePushButton = new QPushButton( PageCalibration, "pre_configurePushButton" );
    pre_configurePushButton->setMinimumSize( QSize( 110, 0 ) );
    pre_configurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    pre_configurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont pre_configurePushButton_font(  pre_configurePushButton->font() );
    pre_configurePushButton_font.setBold( TRUE );
    pre_configurePushButton->setFont( pre_configurePushButton_font ); 
    pre_configurePushButton->setFocusPolicy( QPushButton::NoFocus );

    layout21_2->addWidget( pre_configurePushButton, 1, 0 );

    pre_clearscriptsPushButton = new QPushButton( PageCalibration, "pre_clearscriptsPushButton" );
    pre_clearscriptsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_clearscriptsPushButton->sizePolicy().hasHeightForWidth() ) );
    pre_clearscriptsPushButton->setMinimumSize( QSize( 110, 0 ) );
    pre_clearscriptsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    pre_clearscriptsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout21_2->addWidget( pre_clearscriptsPushButton, 0, 0 );

    pre_startPushButton = new QPushButton( PageCalibration, "pre_startPushButton" );
    pre_startPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, pre_startPushButton->sizePolicy().hasHeightForWidth() ) );
    pre_startPushButton->setMinimumSize( QSize( 110, 0 ) );
    pre_startPushButton->setMaximumSize( QSize( 110, 32767 ) );
    pre_startPushButton->setPaletteForegroundColor( QColor( 0, 118, 0 ) );
    pre_startPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout21_2->addWidget( pre_startPushButton, 0, 1 );
    spacer42 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout21_2->addItem( spacer42, 2, 1 );
    spacer35_4 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout21_2->addMultiCell( spacer35_4, 0, 1, 2, 2 );
    layout22->addLayout( layout21_2 );

    pre_messagesGroupBox = new QGroupBox( PageCalibration, "pre_messagesGroupBox" );
    pre_messagesGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, pre_messagesGroupBox->sizePolicy().hasHeightForWidth() ) );
    pre_messagesGroupBox->setMinimumSize( QSize( 0, 220 ) );
    pre_messagesGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    pre_messagesGroupBox->setColumnLayout(0, Qt::Vertical );
    pre_messagesGroupBox->layout()->setSpacing( 6 );
    pre_messagesGroupBox->layout()->setMargin( 11 );
    pre_messagesGroupBoxLayout = new QGridLayout( pre_messagesGroupBox->layout() );
    pre_messagesGroupBoxLayout->setAlignment( Qt::AlignTop );

    pre_messagesTextEdit = new QTextEdit( pre_messagesGroupBox, "pre_messagesTextEdit" );
    pre_messagesTextEdit->setMinimumSize( QSize( 0, 0 ) );
    pre_messagesTextEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    pre_messagesTextEdit->setFocusPolicy( QTextEdit::NoFocus );
    pre_messagesTextEdit->setFrameShadow( QTextEdit::Sunken );
    pre_messagesTextEdit->setWordWrap( QTextEdit::NoWrap );
    pre_messagesTextEdit->setReadOnly( TRUE );

    pre_messagesGroupBoxLayout->addWidget( pre_messagesTextEdit, 0, 0 );
    layout22->addWidget( pre_messagesGroupBox );

    layout23->addLayout( layout22, 0, 1 );

    PageCalibrationLayout->addLayout( layout23, 0, 0 );
    mainmenu_TabWidget->insertTab( PageCalibration, QString::fromLatin1("") );

    TabPage = new QWidget( mainmenu_TabWidget, "TabPage" );
    TabPageLayout = new QGridLayout( TabPage, 1, 1, 11, 6, "TabPageLayout"); 

    layout46 = new QGridLayout( 0, 1, 1, 0, 6, "layout46"); 

    cal_commandGroupBox = new QGroupBox( TabPage, "cal_commandGroupBox" );
    cal_commandGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, cal_commandGroupBox->sizePolicy().hasHeightForWidth() ) );
    cal_commandGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    cal_commandGroupBox->setColumnLayout(0, Qt::Vertical );
    cal_commandGroupBox->layout()->setSpacing( 6 );
    cal_commandGroupBox->layout()->setMargin( 11 );
    cal_commandGroupBoxLayout = new QGridLayout( cal_commandGroupBox->layout() );
    cal_commandGroupBoxLayout->setAlignment( Qt::AlignTop );

    cal_commandTextEdit = new QTextEdit( cal_commandGroupBox, "cal_commandTextEdit" );
    cal_commandTextEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, cal_commandTextEdit->sizePolicy().hasHeightForWidth() ) );
    cal_commandTextEdit->setFocusPolicy( QTextEdit::ClickFocus );
    cal_commandTextEdit->setWordWrap( QTextEdit::NoWrap );

    cal_commandGroupBoxLayout->addWidget( cal_commandTextEdit, 0, 0 );

    layout46->addMultiCellWidget( cal_commandGroupBox, 1, 1, 0, 1 );

    layout45 = new QVBoxLayout( 0, 0, 6, "layout45"); 
    spacer38_2 = new QSpacerItem( 20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer38_2 );

    layout27 = new QGridLayout( 0, 1, 1, 0, 6, "layout27"); 

    cal_configurePushButton = new QPushButton( TabPage, "cal_configurePushButton" );
    cal_configurePushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, cal_configurePushButton->sizePolicy().hasHeightForWidth() ) );
    cal_configurePushButton->setMinimumSize( QSize( 110, 0 ) );
    cal_configurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    cal_configurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont cal_configurePushButton_font(  cal_configurePushButton->font() );
    cal_configurePushButton_font.setBold( TRUE );
    cal_configurePushButton->setFont( cal_configurePushButton_font ); 
    cal_configurePushButton->setFocusPolicy( QPushButton::NoFocus );
    cal_configurePushButton->setToggleButton( FALSE );

    layout27->addWidget( cal_configurePushButton, 0, 0 );
    spacer34_2 = new QSpacerItem( 121, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout27->addItem( spacer34_2, 1, 2 );

    cal_clearscriptsPushButton = new QPushButton( TabPage, "cal_clearscriptsPushButton" );
    cal_clearscriptsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cal_clearscriptsPushButton->sizePolicy().hasHeightForWidth() ) );
    cal_clearscriptsPushButton->setMinimumSize( QSize( 110, 0 ) );
    cal_clearscriptsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    cal_clearscriptsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout27->addWidget( cal_clearscriptsPushButton, 1, 0 );

    cal_killPushButton = new QPushButton( TabPage, "cal_killPushButton" );
    cal_killPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cal_killPushButton->sizePolicy().hasHeightForWidth() ) );
    cal_killPushButton->setMinimumSize( QSize( 110, 0 ) );
    cal_killPushButton->setMaximumSize( QSize( 110, 32767 ) );
    cal_killPushButton->setPaletteForegroundColor( QColor( 185, 0, 0 ) );
    cal_killPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout27->addWidget( cal_killPushButton, 2, 1 );

    cal_startPushButton = new QPushButton( TabPage, "cal_startPushButton" );
    cal_startPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cal_startPushButton->sizePolicy().hasHeightForWidth() ) );
    cal_startPushButton->setMinimumSize( QSize( 110, 0 ) );
    cal_startPushButton->setMaximumSize( QSize( 110, 32767 ) );
    cal_startPushButton->setPaletteForegroundColor( QColor( 0, 118, 0 ) );
    cal_startPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout27->addWidget( cal_startPushButton, 1, 1 );
    spacer41 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout27->addItem( spacer41, 3, 0 );

    cal_resetfieldsPushButton = new QPushButton( TabPage, "cal_resetfieldsPushButton" );
    cal_resetfieldsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cal_resetfieldsPushButton->sizePolicy().hasHeightForWidth() ) );
    cal_resetfieldsPushButton->setMinimumSize( QSize( 110, 0 ) );
    cal_resetfieldsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    cal_resetfieldsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout27->addWidget( cal_resetfieldsPushButton, 2, 0 );
    layout45->addLayout( layout27 );

    cal_messagesGroupBox = new QGroupBox( TabPage, "cal_messagesGroupBox" );
    cal_messagesGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, cal_messagesGroupBox->sizePolicy().hasHeightForWidth() ) );
    cal_messagesGroupBox->setMinimumSize( QSize( 0, 220 ) );
    cal_messagesGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    cal_messagesGroupBox->setColumnLayout(0, Qt::Vertical );
    cal_messagesGroupBox->layout()->setSpacing( 6 );
    cal_messagesGroupBox->layout()->setMargin( 11 );
    cal_messagesGroupBoxLayout = new QGridLayout( cal_messagesGroupBox->layout() );
    cal_messagesGroupBoxLayout->setAlignment( Qt::AlignTop );

    cal_messagesTextEdit = new QTextEdit( cal_messagesGroupBox, "cal_messagesTextEdit" );
    cal_messagesTextEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    cal_messagesTextEdit->setFocusPolicy( QTextEdit::NoFocus );
    cal_messagesTextEdit->setFrameShadow( QTextEdit::Sunken );
    cal_messagesTextEdit->setWordWrap( QTextEdit::NoWrap );
    cal_messagesTextEdit->setReadOnly( TRUE );

    cal_messagesGroupBoxLayout->addWidget( cal_messagesTextEdit, 0, 0 );
    layout45->addWidget( cal_messagesGroupBox );

    layout46->addLayout( layout45, 0, 1 );

    cal_GroupBox = new QGroupBox( TabPage, "cal_GroupBox" );
    cal_GroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, cal_GroupBox->sizePolicy().hasHeightForWidth() ) );
    cal_GroupBox->setMinimumSize( QSize( 520, 350 ) );
    cal_GroupBox->setMaximumSize( QSize( 520, 350 ) );
    cal_GroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );

    cal_darkinsteadbiasCheckBox = new QCheckBox( cal_GroupBox, "cal_darkinsteadbiasCheckBox" );
    cal_darkinsteadbiasCheckBox->setGeometry( QRect( 320, 310, 100, 20 ) );
    cal_darkinsteadbiasCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cal_darkinsteadbiasCheckBox->sizePolicy().hasHeightForWidth() ) );
    cal_darkinsteadbiasCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    cal_darkinsteadbiasCheckBox->setChecked( FALSE );

    cal_procscienceCheckBox = new QCheckBox( cal_GroupBox, "cal_procscienceCheckBox" );
    cal_procscienceCheckBox->setGeometry( QRect( 40, 280, 210, 20 ) );
    cal_procscienceCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont cal_procscienceCheckBox_font(  cal_procscienceCheckBox->font() );
    cal_procscienceCheckBox_font.setBold( TRUE );
    cal_procscienceCheckBox->setFont( cal_procscienceCheckBox_font ); 
    cal_procscienceCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    cal_procscienceCheckBox->setChecked( TRUE );

    cal_scienceminLineEdit = new QLineEdit( cal_GroupBox, "cal_scienceminLineEdit" );
    cal_scienceminLineEdit->setGeometry( QRect( 320, 280, 60, 22 ) );
    cal_scienceminLineEdit->setMinimumSize( QSize( 60, 0 ) );
    cal_scienceminLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    cal_scienceminLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    cal_scienceminLineEdit->setMaxLength( 9 );
    cal_scienceminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cal_TextLabel3 = new QLabel( cal_GroupBox, "cal_TextLabel3" );
    cal_TextLabel3->setGeometry( QRect( 390, 110, 50, 16 ) );
    cal_TextLabel3->setMinimumSize( QSize( 50, 0 ) );

    cal_sciencemaxLineEdit = new QLineEdit( cal_GroupBox, "cal_sciencemaxLineEdit" );
    cal_sciencemaxLineEdit->setGeometry( QRect( 390, 280, 60, 22 ) );
    cal_sciencemaxLineEdit->setMinimumSize( QSize( 60, 0 ) );
    cal_sciencemaxLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    cal_sciencemaxLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    cal_sciencemaxLineEdit->setMaxLength( 9 );
    cal_sciencemaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cal_procbiasCheckBox = new QCheckBox( cal_GroupBox, "cal_procbiasCheckBox" );
    cal_procbiasCheckBox->setGeometry( QRect( 40, 130, 210, 20 ) );
    cal_procbiasCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont cal_procbiasCheckBox_font(  cal_procbiasCheckBox->font() );
    cal_procbiasCheckBox_font.setBold( TRUE );
    cal_procbiasCheckBox->setFont( cal_procbiasCheckBox_font ); 
    cal_procbiasCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    cal_procbiasCheckBox->setChecked( TRUE );

    cal_biasminLineEdit = new QLineEdit( cal_GroupBox, "cal_biasminLineEdit" );
    cal_biasminLineEdit->setGeometry( QRect( 320, 130, 60, 22 ) );
    cal_biasminLineEdit->setMinimumSize( QSize( 60, 0 ) );
    cal_biasminLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    cal_biasminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cal_calibredoPushButton = new QPushButton( cal_GroupBox, "cal_calibredoPushButton" );
    cal_calibredoPushButton->setGeometry( QRect( 10, 280, 20, 20 ) );
    cal_calibredoPushButton->setFocusPolicy( QPushButton::NoFocus );

    cal_darkminLineEdit = new QLineEdit( cal_GroupBox, "cal_darkminLineEdit" );
    cal_darkminLineEdit->setGeometry( QRect( 320, 180, 60, 22 ) );
    cal_darkminLineEdit->setMinimumSize( QSize( 60, 0 ) );
    cal_darkminLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    cal_darkminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cal_TextLabel2 = new QLabel( cal_GroupBox, "cal_TextLabel2" );
    cal_TextLabel2->setGeometry( QRect( 320, 110, 50, 16 ) );
    cal_TextLabel2->setMinimumSize( QSize( 50, 0 ) );

    cal_flatmaxLineEdit = new QLineEdit( cal_GroupBox, "cal_flatmaxLineEdit" );
    cal_flatmaxLineEdit->setGeometry( QRect( 390, 230, 60, 22 ) );
    cal_flatmaxLineEdit->setMinimumSize( QSize( 60, 0 ) );
    cal_flatmaxLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    cal_flatmaxLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    cal_flatmaxLineEdit->setMaxLength( 9 );
    cal_flatmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cal_procdarkCheckBox = new QCheckBox( cal_GroupBox, "cal_procdarkCheckBox" );
    cal_procdarkCheckBox->setGeometry( QRect( 40, 180, 210, 20 ) );
    cal_procdarkCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont cal_procdarkCheckBox_font(  cal_procdarkCheckBox->font() );
    cal_procdarkCheckBox_font.setBold( TRUE );
    cal_procdarkCheckBox->setFont( cal_procdarkCheckBox_font ); 
    cal_procdarkCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    cal_procdarkCheckBox->setChecked( TRUE );

    cal_flatminLineEdit = new QLineEdit( cal_GroupBox, "cal_flatminLineEdit" );
    cal_flatminLineEdit->setGeometry( QRect( 320, 230, 60, 22 ) );
    cal_flatminLineEdit->setMinimumSize( QSize( 60, 0 ) );
    cal_flatminLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    cal_flatminLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    cal_flatminLineEdit->setMaxLength( 9 );
    cal_flatminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cal_procflatCheckBox = new QCheckBox( cal_GroupBox, "cal_procflatCheckBox" );
    cal_procflatCheckBox->setGeometry( QRect( 40, 230, 210, 20 ) );
    cal_procflatCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont cal_procflatCheckBox_font(  cal_procflatCheckBox->font() );
    cal_procflatCheckBox_font.setBold( TRUE );
    cal_procflatCheckBox->setFont( cal_procflatCheckBox_font ); 
    cal_procflatCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    cal_procflatCheckBox->setChecked( TRUE );

    cal_biasmaxLineEdit = new QLineEdit( cal_GroupBox, "cal_biasmaxLineEdit" );
    cal_biasmaxLineEdit->setGeometry( QRect( 390, 130, 60, 22 ) );
    cal_biasmaxLineEdit->setMinimumSize( QSize( 60, 0 ) );
    cal_biasmaxLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    cal_biasmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cal_darkmaxLineEdit = new QLineEdit( cal_GroupBox, "cal_darkmaxLineEdit" );
    cal_darkmaxLineEdit->setGeometry( QRect( 390, 180, 60, 22 ) );
    cal_darkmaxLineEdit->setMinimumSize( QSize( 60, 0 ) );
    cal_darkmaxLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    cal_darkmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4 = new QFrame( cal_GroupBox, "frame4" );
    frame4->setGeometry( QRect( 40, 20, 280, 80 ) );
    frame4->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    frame4->setFrameShape( QFrame::StyledPanel );
    frame4->setFrameShadow( QFrame::Plain );

    cal_nobiasCheckBox = new QCheckBox( frame4, "cal_nobiasCheckBox" );
    cal_nobiasCheckBox->setGeometry( QRect( 10, 10, 200, 20 ) );
    cal_nobiasCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    cal_noflatCheckBox = new QCheckBox( frame4, "cal_noflatCheckBox" );
    cal_noflatCheckBox->setGeometry( QRect( 10, 30, 200, 20 ) );
    cal_noflatCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    cal_nonlinCheckBox = new QCheckBox( frame4, "cal_nonlinCheckBox" );
    cal_nonlinCheckBox->setGeometry( QRect( 10, 50, 240, 20 ) );
    cal_nonlinCheckBox->setFocusPolicy( QCheckBox::ClickFocus );

    layout46->addWidget( cal_GroupBox, 0, 0 );

    TabPageLayout->addLayout( layout46, 0, 0 );
    mainmenu_TabWidget->insertTab( TabPage, QString::fromLatin1("") );

    PagePrereducion = new QWidget( mainmenu_TabWidget, "PagePrereducion" );
    PagePrereducionLayout = new QGridLayout( PagePrereducion, 1, 1, 11, 6, "PagePrereducionLayout"); 

    sf_commandGroupBox = new QGroupBox( PagePrereducion, "sf_commandGroupBox" );
    sf_commandGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, sf_commandGroupBox->sizePolicy().hasHeightForWidth() ) );
    sf_commandGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    sf_commandGroupBox->setColumnLayout(0, Qt::Vertical );
    sf_commandGroupBox->layout()->setSpacing( 6 );
    sf_commandGroupBox->layout()->setMargin( 11 );
    sf_commandGroupBoxLayout = new QGridLayout( sf_commandGroupBox->layout() );
    sf_commandGroupBoxLayout->setAlignment( Qt::AlignTop );

    sf_commandTextEdit = new QTextEdit( sf_commandGroupBox, "sf_commandTextEdit" );
    sf_commandTextEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, sf_commandTextEdit->sizePolicy().hasHeightForWidth() ) );
    sf_commandTextEdit->setFocusPolicy( QTextEdit::ClickFocus );
    sf_commandTextEdit->setWordWrap( QTextEdit::NoWrap );

    sf_commandGroupBoxLayout->addWidget( sf_commandTextEdit, 0, 0 );

    PagePrereducionLayout->addMultiCellWidget( sf_commandGroupBox, 1, 1, 0, 1 );

    layout41 = new QVBoxLayout( 0, 0, 6, "layout41"); 
    spacer42_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout41->addItem( spacer42_2 );

    layout21_3 = new QGridLayout( 0, 1, 1, 0, 6, "layout21_3"); 
    spacer39_2 = new QSpacerItem( 119, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout21_3->addMultiCell( spacer39_2, 0, 1, 2, 2 );
    spacer40 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout21_3->addMultiCell( spacer40, 2, 2, 0, 1 );

    sf_startPushButton = new QPushButton( PagePrereducion, "sf_startPushButton" );
    sf_startPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, sf_startPushButton->sizePolicy().hasHeightForWidth() ) );
    sf_startPushButton->setMinimumSize( QSize( 110, 0 ) );
    sf_startPushButton->setMaximumSize( QSize( 110, 32767 ) );
    sf_startPushButton->setPaletteForegroundColor( QColor( 0, 118, 0 ) );
    sf_startPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout21_3->addWidget( sf_startPushButton, 0, 1 );

    sf_clearscriptsPushButton = new QPushButton( PagePrereducion, "sf_clearscriptsPushButton" );
    sf_clearscriptsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, sf_clearscriptsPushButton->sizePolicy().hasHeightForWidth() ) );
    sf_clearscriptsPushButton->setMinimumSize( QSize( 110, 0 ) );
    sf_clearscriptsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    sf_clearscriptsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout21_3->addWidget( sf_clearscriptsPushButton, 0, 0 );

    sf_resetsfieldsPushButton = new QPushButton( PagePrereducion, "sf_resetsfieldsPushButton" );
    sf_resetsfieldsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, sf_resetsfieldsPushButton->sizePolicy().hasHeightForWidth() ) );
    sf_resetsfieldsPushButton->setMinimumSize( QSize( 110, 0 ) );
    sf_resetsfieldsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    sf_resetsfieldsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout21_3->addWidget( sf_resetsfieldsPushButton, 1, 0 );

    sf_killPushButton = new QPushButton( PagePrereducion, "sf_killPushButton" );
    sf_killPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, sf_killPushButton->sizePolicy().hasHeightForWidth() ) );
    sf_killPushButton->setMinimumSize( QSize( 110, 0 ) );
    sf_killPushButton->setMaximumSize( QSize( 110, 32767 ) );
    sf_killPushButton->setPaletteForegroundColor( QColor( 185, 0, 0 ) );
    sf_killPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout21_3->addWidget( sf_killPushButton, 1, 1 );
    layout41->addLayout( layout21_3 );

    sf_messagesGroupBox = new QGroupBox( PagePrereducion, "sf_messagesGroupBox" );
    sf_messagesGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, sf_messagesGroupBox->sizePolicy().hasHeightForWidth() ) );
    sf_messagesGroupBox->setMinimumSize( QSize( 0, 220 ) );
    sf_messagesGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    sf_messagesGroupBox->setColumnLayout(0, Qt::Vertical );
    sf_messagesGroupBox->layout()->setSpacing( 6 );
    sf_messagesGroupBox->layout()->setMargin( 11 );
    sf_messagesGroupBoxLayout = new QGridLayout( sf_messagesGroupBox->layout() );
    sf_messagesGroupBoxLayout->setAlignment( Qt::AlignTop );

    sf_messagesTextEdit = new QTextEdit( sf_messagesGroupBox, "sf_messagesTextEdit" );
    sf_messagesTextEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    sf_messagesTextEdit->setFocusPolicy( QTextEdit::NoFocus );
    sf_messagesTextEdit->setFrameShadow( QTextEdit::Sunken );
    sf_messagesTextEdit->setWordWrap( QTextEdit::NoWrap );
    sf_messagesTextEdit->setReadOnly( TRUE );

    sf_messagesGroupBoxLayout->addWidget( sf_messagesTextEdit, 0, 0 );
    layout41->addWidget( sf_messagesGroupBox );

    PagePrereducionLayout->addLayout( layout41, 0, 1 );

    sf_ButtonGroup = new QButtonGroup( PagePrereducion, "sf_ButtonGroup" );
    sf_ButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sf_ButtonGroup->sizePolicy().hasHeightForWidth() ) );
    sf_ButtonGroup->setMinimumSize( QSize( 520, 350 ) );
    sf_ButtonGroup->setMaximumSize( QSize( 520, 350 ) );
    sf_ButtonGroup->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );

    sf_backgroundconfigurePushButton = new QPushButton( sf_ButtonGroup, "sf_backgroundconfigurePushButton" );
    sf_backgroundconfigurePushButton->setGeometry( QRect( 380, 90, 110, 24 ) );
    sf_backgroundconfigurePushButton->setMinimumSize( QSize( 110, 0 ) );
    sf_backgroundconfigurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    sf_backgroundconfigurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont sf_backgroundconfigurePushButton_font(  sf_backgroundconfigurePushButton->font() );
    sf_backgroundconfigurePushButton_font.setBold( TRUE );
    sf_backgroundconfigurePushButton->setFont( sf_backgroundconfigurePushButton_font ); 
    sf_backgroundconfigurePushButton->setFocusPolicy( QPushButton::NoFocus );

    sf_backgroundredoPushButton = new QPushButton( sf_ButtonGroup, "sf_backgroundredoPushButton" );
    sf_backgroundredoPushButton->setGeometry( QRect( 10, 90, 20, 20 ) );
    sf_backgroundredoPushButton->setFocusPolicy( QPushButton::NoFocus );

    sf_collapseredoPushButton = new QPushButton( sf_ButtonGroup, "sf_collapseredoPushButton" );
    sf_collapseredoPushButton->setGeometry( QRect( 10, 290, 20, 20 ) );
    sf_collapseredoPushButton->setFocusPolicy( QPushButton::NoFocus );

    sf_collconfigurePushButton = new QPushButton( sf_ButtonGroup, "sf_collconfigurePushButton" );
    sf_collconfigurePushButton->setGeometry( QRect( 380, 290, 110, 24 ) );
    sf_collconfigurePushButton->setMinimumSize( QSize( 110, 0 ) );
    sf_collconfigurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    sf_collconfigurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont sf_collconfigurePushButton_font(  sf_collconfigurePushButton->font() );
    sf_collconfigurePushButton_font.setBold( TRUE );
    sf_collconfigurePushButton->setFont( sf_collconfigurePushButton_font ); 
    sf_collconfigurePushButton->setFocusPolicy( QPushButton::NoFocus );

    sf_collapseCheckBox = new QCheckBox( sf_ButtonGroup, "sf_collapseCheckBox" );
    sf_collapseCheckBox->setGeometry( QRect( 40, 290, 220, 20 ) );
    QFont sf_collapseCheckBox_font(  sf_collapseCheckBox->font() );
    sf_collapseCheckBox_font.setBold( TRUE );
    sf_collapseCheckBox->setFont( sf_collapseCheckBox_font ); 
    sf_collapseCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    sf_chopnodredoPushButton = new QPushButton( sf_ButtonGroup, "sf_chopnodredoPushButton" );
    sf_chopnodredoPushButton->setGeometry( QRect( 10, 210, 20, 20 ) );
    sf_chopnodredoPushButton->setFocusPolicy( QPushButton::NoFocus );

    sf_patternTextLabel = new QLabel( sf_ButtonGroup, "sf_patternTextLabel" );
    sf_patternTextLabel->setGeometry( QRect( 270, 210, 69, 22 ) );

    sf_chopnodpatternComboBox = new QComboBox( FALSE, sf_ButtonGroup, "sf_chopnodpatternComboBox" );
    sf_chopnodpatternComboBox->setGeometry( QRect( 360, 210, 68, 22 ) );
    sf_chopnodpatternComboBox->setFocusPolicy( QComboBox::NoFocus );

    sf_chopnodinvertCheckBox = new QCheckBox( sf_ButtonGroup, "sf_chopnodinvertCheckBox" );
    sf_chopnodinvertCheckBox->setGeometry( QRect( 440, 210, 69, 20 ) );
    sf_chopnodinvertCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    sf_spreadsequenceCheckBox = new QCheckBox( sf_ButtonGroup, "sf_spreadsequenceCheckBox" );
    sf_spreadsequenceCheckBox->setGeometry( QRect( 40, 50, 210, 20 ) );
    QFont sf_spreadsequenceCheckBox_font(  sf_spreadsequenceCheckBox->font() );
    sf_spreadsequenceCheckBox_font.setBold( TRUE );
    sf_spreadsequenceCheckBox->setFont( sf_spreadsequenceCheckBox_font ); 
    sf_spreadsequenceCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    sf_spreadsequenceCheckBox->setChecked( FALSE );

    sf_mergesequenceCheckBox = new QCheckBox( sf_ButtonGroup, "sf_mergesequenceCheckBox" );
    sf_mergesequenceCheckBox->setGeometry( QRect( 40, 130, 220, 20 ) );
    QFont sf_mergesequenceCheckBox_font(  sf_mergesequenceCheckBox->font() );
    sf_mergesequenceCheckBox_font.setBold( TRUE );
    sf_mergesequenceCheckBox->setFont( sf_mergesequenceCheckBox_font ); 
    sf_mergesequenceCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    sf_spread1TextLabel = new QLabel( sf_ButtonGroup, "sf_spread1TextLabel" );
    sf_spread1TextLabel->setGeometry( QRect( 330, 30, 80, 20 ) );

    sf_spread2TextLabel = new QLabel( sf_ButtonGroup, "sf_spread2TextLabel" );
    sf_spread2TextLabel->setGeometry( QRect( 420, 30, 58, 20 ) );

    sf_spread_numbergroupsLineEdit = new QLineEdit( sf_ButtonGroup, "sf_spread_numbergroupsLineEdit" );
    sf_spread_numbergroupsLineEdit->setGeometry( QRect( 330, 50, 70, 23 ) );
    sf_spread_numbergroupsLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    sf_spread_numbergroupsLineEdit->setMaxLength( 9 );
    sf_spread_numbergroupsLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    sf_spread_lengthsequenceLineEdit = new QLineEdit( sf_ButtonGroup, "sf_spread_lengthsequenceLineEdit" );
    sf_spread_lengthsequenceLineEdit->setGeometry( QRect( 420, 50, 70, 23 ) );
    sf_spread_lengthsequenceLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    sf_spread_lengthsequenceLineEdit->setMaxLength( 9 );
    sf_spread_lengthsequenceLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    sf_chopnodCheckBox = new QCheckBox( sf_ButtonGroup, "sf_chopnodCheckBox" );
    sf_chopnodCheckBox->setGeometry( QRect( 40, 210, 220, 20 ) );
    QFont sf_chopnodCheckBox_font(  sf_chopnodCheckBox->font() );
    sf_chopnodCheckBox_font.setBold( TRUE );
    sf_chopnodCheckBox->setFont( sf_chopnodCheckBox_font ); 
    sf_chopnodCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    sf_backgroundCheckBox = new QCheckBox( sf_ButtonGroup, "sf_backgroundCheckBox" );
    sf_backgroundCheckBox->setGeometry( QRect( 40, 90, 290, 20 ) );
    QFont sf_backgroundCheckBox_font(  sf_backgroundCheckBox->font() );
    sf_backgroundCheckBox_font.setBold( TRUE );
    sf_backgroundCheckBox->setFont( sf_backgroundCheckBox_font ); 
    sf_backgroundCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    sf_backgroundCheckBox->setChecked( FALSE );

    PagePrereducionLayout->addWidget( sf_ButtonGroup, 0, 0 );
    mainmenu_TabWidget->insertTab( PagePrereducion, QString::fromLatin1("") );

    TabPage_2 = new QWidget( mainmenu_TabWidget, "TabPage_2" );
    TabPageLayout_2 = new QGridLayout( TabPage_2, 1, 1, 11, 6, "TabPageLayout_2"); 

    layout36 = new QGridLayout( 0, 1, 1, 0, 6, "layout36"); 

    layout35 = new QVBoxLayout( 0, 0, 6, "layout35"); 
    spacer43 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout35->addItem( spacer43 );

    layout29 = new QGridLayout( 0, 1, 1, 0, 6, "layout29"); 

    w_clearscriptsPushButton = new QPushButton( TabPage_2, "w_clearscriptsPushButton" );
    w_clearscriptsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, w_clearscriptsPushButton->sizePolicy().hasHeightForWidth() ) );
    w_clearscriptsPushButton->setMinimumSize( QSize( 110, 0 ) );
    w_clearscriptsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    w_clearscriptsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout29->addWidget( w_clearscriptsPushButton, 1, 0 );

    w_resetfieldsPushButton = new QPushButton( TabPage_2, "w_resetfieldsPushButton" );
    w_resetfieldsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, w_resetfieldsPushButton->sizePolicy().hasHeightForWidth() ) );
    w_resetfieldsPushButton->setMinimumSize( QSize( 110, 0 ) );
    w_resetfieldsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    w_resetfieldsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout29->addWidget( w_resetfieldsPushButton, 2, 0 );
    spacer40_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout29->addItem( spacer40_2, 1, 2 );
    spacer39 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout29->addItem( spacer39, 3, 1 );

    w_startPushButton = new QPushButton( TabPage_2, "w_startPushButton" );
    w_startPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, w_startPushButton->sizePolicy().hasHeightForWidth() ) );
    w_startPushButton->setMinimumSize( QSize( 110, 0 ) );
    w_startPushButton->setMaximumSize( QSize( 110, 32767 ) );
    w_startPushButton->setPaletteForegroundColor( QColor( 0, 118, 0 ) );
    w_startPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout29->addWidget( w_startPushButton, 1, 1 );

    w_killPushButton = new QPushButton( TabPage_2, "w_killPushButton" );
    w_killPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, w_killPushButton->sizePolicy().hasHeightForWidth() ) );
    w_killPushButton->setMinimumSize( QSize( 110, 0 ) );
    w_killPushButton->setMaximumSize( QSize( 110, 32767 ) );
    w_killPushButton->setPaletteForegroundColor( QColor( 185, 0, 0 ) );
    w_killPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout29->addWidget( w_killPushButton, 2, 1 );
    layout35->addLayout( layout29 );

    w_messagesGroupBox = new QGroupBox( TabPage_2, "w_messagesGroupBox" );
    w_messagesGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, w_messagesGroupBox->sizePolicy().hasHeightForWidth() ) );
    w_messagesGroupBox->setMinimumSize( QSize( 0, 220 ) );
    w_messagesGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    w_messagesGroupBox->setColumnLayout(0, Qt::Vertical );
    w_messagesGroupBox->layout()->setSpacing( 6 );
    w_messagesGroupBox->layout()->setMargin( 11 );
    w_messagesGroupBoxLayout = new QGridLayout( w_messagesGroupBox->layout() );
    w_messagesGroupBoxLayout->setAlignment( Qt::AlignTop );

    w_messagesTextEdit = new QTextEdit( w_messagesGroupBox, "w_messagesTextEdit" );
    w_messagesTextEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    w_messagesTextEdit->setFocusPolicy( QTextEdit::NoFocus );
    w_messagesTextEdit->setFrameShadow( QTextEdit::Sunken );
    w_messagesTextEdit->setWordWrap( QTextEdit::NoWrap );
    w_messagesTextEdit->setReadOnly( TRUE );

    w_messagesGroupBoxLayout->addWidget( w_messagesTextEdit, 0, 0 );
    layout35->addWidget( w_messagesGroupBox );

    layout36->addLayout( layout35, 0, 1 );

    w_commandGroupBox = new QGroupBox( TabPage_2, "w_commandGroupBox" );
    w_commandGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, w_commandGroupBox->sizePolicy().hasHeightForWidth() ) );
    w_commandGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    w_commandGroupBox->setColumnLayout(0, Qt::Vertical );
    w_commandGroupBox->layout()->setSpacing( 6 );
    w_commandGroupBox->layout()->setMargin( 11 );
    w_commandGroupBoxLayout = new QGridLayout( w_commandGroupBox->layout() );
    w_commandGroupBoxLayout->setAlignment( Qt::AlignTop );

    w_commandTextEdit = new QTextEdit( w_commandGroupBox, "w_commandTextEdit" );
    w_commandTextEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, w_commandTextEdit->sizePolicy().hasHeightForWidth() ) );
    w_commandTextEdit->setFocusPolicy( QTextEdit::ClickFocus );
    w_commandTextEdit->setWordWrap( QTextEdit::NoWrap );

    w_commandGroupBoxLayout->addWidget( w_commandTextEdit, 0, 0 );

    layout36->addMultiCellWidget( w_commandGroupBox, 1, 1, 0, 1 );

    w_weightButtonGroup = new QButtonGroup( TabPage_2, "w_weightButtonGroup" );
    w_weightButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, w_weightButtonGroup->sizePolicy().hasHeightForWidth() ) );
    w_weightButtonGroup->setMinimumSize( QSize( 520, 350 ) );
    w_weightButtonGroup->setMaximumSize( QSize( 520, 350 ) );
    w_weightButtonGroup->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );

    w_configure3PushButton = new QPushButton( w_weightButtonGroup, "w_configure3PushButton" );
    w_configure3PushButton->setGeometry( QRect( 270, 110, 110, 23 ) );
    w_configure3PushButton->setMinimumSize( QSize( 110, 0 ) );
    w_configure3PushButton->setMaximumSize( QSize( 110, 32767 ) );
    w_configure3PushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont w_configure3PushButton_font(  w_configure3PushButton->font() );
    w_configure3PushButton_font.setBold( TRUE );
    w_configure3PushButton->setFont( w_configure3PushButton_font ); 
    w_configure3PushButton->setFocusPolicy( QPushButton::NoFocus );
    w_configure3PushButton->setToggleButton( FALSE );

    w_createbinmosCheckBox = new QCheckBox( w_weightButtonGroup, "w_createbinmosCheckBox" );
    w_createbinmosCheckBox->setGeometry( QRect( 40, 110, 220, 20 ) );
    QFont w_createbinmosCheckBox_font(  w_createbinmosCheckBox->font() );
    w_createbinmosCheckBox_font.setBold( TRUE );
    w_createbinmosCheckBox->setFont( w_createbinmosCheckBox_font ); 
    w_createbinmosCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    w_distsetCheckBox = new QCheckBox( w_weightButtonGroup, "w_distsetCheckBox" );
    w_distsetCheckBox->setGeometry( QRect( 40, 290, 220, 20 ) );
    QFont w_distsetCheckBox_font(  w_distsetCheckBox->font() );
    w_distsetCheckBox_font.setBold( TRUE );
    w_distsetCheckBox->setFont( w_distsetCheckBox_font ); 
    w_distsetCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    w_distsetCheckBox->setChecked( FALSE );

    w_createweightsCheckBox = new QCheckBox( w_weightButtonGroup, "w_createweightsCheckBox" );
    w_createweightsCheckBox->setGeometry( QRect( 40, 230, 220, 20 ) );
    w_createweightsCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont w_createweightsCheckBox_font(  w_createweightsCheckBox->font() );
    w_createweightsCheckBox_font.setBold( TRUE );
    w_createweightsCheckBox->setFont( w_createweightsCheckBox_font ); 
    w_createweightsCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    w_createweightsCheckBox->setChecked( TRUE );

    w_debloomredoPushButton = new QPushButton( w_weightButtonGroup, "w_debloomredoPushButton" );
    w_debloomredoPushButton->setGeometry( QRect( 10, 50, 20, 20 ) );
    w_debloomredoPushButton->setFocusPolicy( QPushButton::NoFocus );

    w_configure2PushButton = new QPushButton( w_weightButtonGroup, "w_configure2PushButton" );
    w_configure2PushButton->setGeometry( QRect( 270, 230, 110, 23 ) );
    w_configure2PushButton->setMinimumSize( QSize( 110, 0 ) );
    w_configure2PushButton->setMaximumSize( QSize( 110, 32767 ) );
    w_configure2PushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont w_configure2PushButton_font(  w_configure2PushButton->font() );
    w_configure2PushButton_font.setBold( TRUE );
    w_configure2PushButton->setFont( w_configure2PushButton_font ); 
    w_configure2PushButton->setFocusPolicy( QPushButton::NoFocus );
    w_configure2PushButton->setToggleButton( FALSE );

    w_configure1PushButton = new QPushButton( w_weightButtonGroup, "w_configure1PushButton" );
    w_configure1PushButton->setGeometry( QRect( 270, 170, 110, 23 ) );
    w_configure1PushButton->setMinimumSize( QSize( 110, 0 ) );
    w_configure1PushButton->setMaximumSize( QSize( 110, 32767 ) );
    w_configure1PushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont w_configure1PushButton_font(  w_configure1PushButton->font() );
    w_configure1PushButton_font.setBold( TRUE );
    w_configure1PushButton->setFont( w_configure1PushButton_font ); 
    w_configure1PushButton->setFocusPolicy( QPushButton::NoFocus );
    w_configure1PushButton->setToggleButton( FALSE );

    w_minoverlapTextLabel = new QLabel( w_weightButtonGroup, "w_minoverlapTextLabel" );
    w_minoverlapTextLabel->setGeometry( QRect( 270, 290, 150, 20 ) );

    w_minoverlapLineEdit = new QLineEdit( w_weightButtonGroup, "w_minoverlapLineEdit" );
    w_minoverlapLineEdit->setGeometry( QRect( 430, 290, 60, 23 ) );
    w_minoverlapLineEdit->setMinimumSize( QSize( 0, 0 ) );
    w_minoverlapLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    w_minoverlapLineEdit->setMaxLength( 9 );
    w_minoverlapLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    w_createglobwCheckBox = new QCheckBox( w_weightButtonGroup, "w_createglobwCheckBox" );
    w_createglobwCheckBox->setGeometry( QRect( 40, 170, 220, 20 ) );
    w_createglobwCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont w_createglobwCheckBox_font(  w_createglobwCheckBox->font() );
    w_createglobwCheckBox_font.setBold( TRUE );
    w_createglobwCheckBox->setFont( w_createglobwCheckBox_font ); 
    w_createglobwCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    w_createglobwCheckBox->setChecked( TRUE );

    w_debloomCheckBox = new QCheckBox( w_weightButtonGroup, "w_debloomCheckBox" );
    w_debloomCheckBox->setGeometry( QRect( 40, 50, 220, 20 ) );
    QFont w_debloomCheckBox_font(  w_debloomCheckBox->font() );
    w_debloomCheckBox_font.setBold( TRUE );
    w_debloomCheckBox->setFont( w_debloomCheckBox_font ); 
    w_debloomCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    w_bloomthreshTextLabel = new QLabel( w_weightButtonGroup, "w_bloomthreshTextLabel" );
    w_bloomthreshTextLabel->setGeometry( QRect( 270, 50, 160, 20 ) );

    w_bloomthreshLineEdit = new QLineEdit( w_weightButtonGroup, "w_bloomthreshLineEdit" );
    w_bloomthreshLineEdit->setGeometry( QRect( 430, 50, 60, 23 ) );
    w_bloomthreshLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    w_bloomthreshLineEdit->setMaxLength( 9 );
    w_bloomthreshLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout36->addWidget( w_weightButtonGroup, 0, 0 );

    TabPageLayout_2->addLayout( layout36, 0, 0 );
    mainmenu_TabWidget->insertTab( TabPage_2, QString::fromLatin1("") );

    PageAstromPhotom = new QWidget( mainmenu_TabWidget, "PageAstromPhotom" );
    PageAstromPhotomLayout = new QGridLayout( PageAstromPhotom, 1, 1, 11, 6, "PageAstromPhotomLayout"); 

    layout38 = new QGridLayout( 0, 1, 1, 0, 6, "layout38"); 

    ap_astphotButtonGroup = new QButtonGroup( PageAstromPhotom, "ap_astphotButtonGroup" );
    ap_astphotButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, ap_astphotButtonGroup->sizePolicy().hasHeightForWidth() ) );
    ap_astphotButtonGroup->setMinimumSize( QSize( 520, 350 ) );
    ap_astphotButtonGroup->setMaximumSize( QSize( 520, 350 ) );
    ap_astphotButtonGroup->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );

    ap_astrommethodconfigurePushButton = new QPushButton( ap_astphotButtonGroup, "ap_astrommethodconfigurePushButton" );
    ap_astrommethodconfigurePushButton->setGeometry( QRect( 390, 290, 110, 23 ) );
    ap_astrommethodconfigurePushButton->setMinimumSize( QSize( 110, 0 ) );
    ap_astrommethodconfigurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    ap_astrommethodconfigurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont ap_astrommethodconfigurePushButton_font(  ap_astrommethodconfigurePushButton->font() );
    ap_astrommethodconfigurePushButton_font.setBold( TRUE );
    ap_astrommethodconfigurePushButton->setFont( ap_astrommethodconfigurePushButton_font ); 
    ap_astrommethodconfigurePushButton->setFocusPolicy( QPushButton::NoFocus );

    ap_sexcatconfigurePushButton = new QPushButton( ap_astphotButtonGroup, "ap_sexcatconfigurePushButton" );
    ap_sexcatconfigurePushButton->setGeometry( QRect( 390, 260, 110, 23 ) );
    ap_sexcatconfigurePushButton->setMinimumSize( QSize( 110, 0 ) );
    ap_sexcatconfigurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    ap_sexcatconfigurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont ap_sexcatconfigurePushButton_font(  ap_sexcatconfigurePushButton->font() );
    ap_sexcatconfigurePushButton_font.setBold( TRUE );
    ap_sexcatconfigurePushButton->setFont( ap_sexcatconfigurePushButton_font ); 
    ap_sexcatconfigurePushButton->setFocusPolicy( QPushButton::NoFocus );

    ap_directphotredoPushButton = new QPushButton( ap_astphotButtonGroup, "ap_directphotredoPushButton" );
    ap_directphotredoPushButton->setGeometry( QRect( 10, 230, 20, 20 ) );
    ap_directphotredoPushButton->setFocusPolicy( QPushButton::NoFocus );

    ap_absphotom_indirect_configurePushButton = new QPushButton( ap_astphotButtonGroup, "ap_absphotom_indirect_configurePushButton" );
    ap_absphotom_indirect_configurePushButton->setGeometry( QRect( 390, 200, 110, 23 ) );
    ap_absphotom_indirect_configurePushButton->setMinimumSize( QSize( 110, 0 ) );
    ap_absphotom_indirect_configurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    ap_absphotom_indirect_configurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont ap_absphotom_indirect_configurePushButton_font(  ap_absphotom_indirect_configurePushButton->font() );
    ap_absphotom_indirect_configurePushButton_font.setBold( TRUE );
    ap_absphotom_indirect_configurePushButton->setFont( ap_absphotom_indirect_configurePushButton_font ); 
    ap_absphotom_indirect_configurePushButton->setFocusPolicy( QPushButton::NoFocus );

    ap_absphotom_direct_configurePushButton = new QPushButton( ap_astphotButtonGroup, "ap_absphotom_direct_configurePushButton" );
    ap_absphotom_direct_configurePushButton->setGeometry( QRect( 390, 230, 110, 23 ) );
    ap_absphotom_direct_configurePushButton->setMinimumSize( QSize( 110, 0 ) );
    ap_absphotom_direct_configurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    ap_absphotom_direct_configurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont ap_absphotom_direct_configurePushButton_font(  ap_absphotom_direct_configurePushButton->font() );
    ap_absphotom_direct_configurePushButton_font.setBold( TRUE );
    ap_absphotom_direct_configurePushButton->setFont( ap_absphotom_direct_configurePushButton_font ); 
    ap_absphotom_direct_configurePushButton->setFocusPolicy( QPushButton::NoFocus );

    ap_absphotom_indirect_CheckBox = new QCheckBox( ap_astphotButtonGroup, "ap_absphotom_indirect_CheckBox" );
    ap_absphotom_indirect_CheckBox->setGeometry( QRect( 40, 200, 260, 20 ) );
    QFont ap_absphotom_indirect_CheckBox_font(  ap_absphotom_indirect_CheckBox->font() );
    ap_absphotom_indirect_CheckBox_font.setBold( TRUE );
    ap_absphotom_indirect_CheckBox->setFont( ap_absphotom_indirect_CheckBox_font ); 
    ap_absphotom_indirect_CheckBox->setFocusPolicy( QCheckBox::NoFocus );
    ap_absphotom_indirect_CheckBox->setChecked( FALSE );

    frame3 = new QFrame( ap_astphotButtonGroup, "frame3" );
    frame3->setGeometry( QRect( 20, 30, 480, 160 ) );
    frame3->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    frame3->setFrameShape( QFrame::Panel );
    frame3->setFrameShadow( QFrame::Sunken );
    frame3->setLineWidth( 1 );

    ap_maglimTextLabel = new QLabel( frame3, "ap_maglimTextLabel" );
    ap_maglimTextLabel->setGeometry( QRect( 333, 11, 61, 22 ) );

    ap_radiusTextLabel = new QLabel( frame3, "ap_radiusTextLabel" );
    ap_radiusTextLabel->setGeometry( QRect( 333, 39, 61, 22 ) );

    ap_refraTextLabel = new QLabel( frame3, "ap_refraTextLabel" );
    ap_refraTextLabel->setGeometry( QRect( 151, 11, 40, 22 ) );

    ap_refdecLineEdit = new QLineEdit( frame3, "ap_refdecLineEdit" );
    ap_refdecLineEdit->setGeometry( QRect( 199, 39, 126, 22 ) );
    ap_refdecLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ap_refdecLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ap_dtdminTextLabel = new QLabel( frame3, "ap_dtdminTextLabel" );
    ap_dtdminTextLabel->setGeometry( QRect( 247, 67, 78, 22 ) );

    ap_refimagedetectthreshLineEdit = new QLineEdit( frame3, "ap_refimagedetectthreshLineEdit" );
    ap_refimagedetectthreshLineEdit->setGeometry( QRect( 333, 67, 60, 22 ) );
    ap_refimagedetectthreshLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ap_refimagedetectthreshLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ap_refimagedetectthreshLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ap_refimagedetectthreshLineEdit->setMaxLength( 9 );
    ap_refimagedetectthreshLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ap_refimagedetectminareaLineEdit = new QLineEdit( frame3, "ap_refimagedetectminareaLineEdit" );
    ap_refimagedetectminareaLineEdit->setGeometry( QRect( 402, 67, 60, 22 ) );
    ap_refimagedetectminareaLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ap_refimagedetectminareaLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ap_refimagedetectminareaLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ap_refimagedetectminareaLineEdit->setMaxLength( 9 );
    ap_refimagedetectminareaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ap_refdecTextLabel = new QLabel( frame3, "ap_refdecTextLabel" );
    ap_refdecTextLabel->setGeometry( QRect( 151, 39, 40, 22 ) );

    ap_selectimagePushButton = new QPushButton( frame3, "ap_selectimagePushButton" );
    ap_selectimagePushButton->setGeometry( QRect( 247, 95, 215, 24 ) );
    ap_selectimagePushButton->setMinimumSize( QSize( 130, 0 ) );
    ap_selectimagePushButton->setFocusPolicy( QPushButton::NoFocus );

    ap_refimageLineEdit = new QLineEdit( frame3, "ap_refimageLineEdit" );
    ap_refimageLineEdit->setGeometry( QRect( 151, 126, 311, 22 ) );
    ap_refimageLineEdit->setMaxLength( 999 );
    ap_refimageLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ap_getastrefcatalogPushButton = new QPushButton( frame3, "ap_getastrefcatalogPushButton" );
    ap_getastrefcatalogPushButton->setGeometry( QRect( 13, 125, 130, 24 ) );

    textLabel1_7 = new QLabel( frame3, "textLabel1_7" );
    textLabel1_7->setGeometry( QRect( 13, 11, 130, 50 ) );

    ap_arcserverComboBox = new QComboBox( FALSE, frame3, "ap_arcserverComboBox" );
    ap_arcserverComboBox->setGeometry( QRect( 13, 67, 130, 22 ) );
    ap_arcserverComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, ap_arcserverComboBox->sizePolicy().hasHeightForWidth() ) );
    ap_arcserverComboBox->setMinimumSize( QSize( 130, 0 ) );
    ap_arcserverComboBox->setMaximumSize( QSize( 130, 32767 ) );
    ap_arcserverComboBox->setFocusPolicy( QComboBox::NoFocus );
    ap_arcserverComboBox->setSizeLimit( 11 );

    ap_maglimLineEdit = new QLineEdit( frame3, "ap_maglimLineEdit" );
    ap_maglimLineEdit->setGeometry( QRect( 402, 11, 60, 22 ) );
    ap_maglimLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ap_maglimLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ap_maglimLineEdit->setMaxLength( 9 );
    ap_maglimLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ap_refraLineEdit = new QLineEdit( frame3, "ap_refraLineEdit" );
    ap_refraLineEdit->setGeometry( QRect( 199, 11, 126, 22 ) );
    ap_refraLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ap_refraLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ap_resolveLineEdit = new QLineEdit( frame3, "ap_resolveLineEdit" );
    ap_resolveLineEdit->setGeometry( QRect( 210, 67, 220, 22 ) );
    ap_resolveLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ap_resolveLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ap_resolveTextLabel = new QLabel( frame3, "ap_resolveTextLabel" );
    ap_resolveTextLabel->setGeometry( QRect( 150, 68, 50, 20 ) );

    ap_resolvePushButton = new QPushButton( frame3, "ap_resolvePushButton" );
    ap_resolvePushButton->setGeometry( QRect( 436, 65, 26, 26 ) );
    ap_resolvePushButton->setPixmap( QPixmap::fromMimeSource( "magnifyer.png" ) );

    ap_radiusLineEdit = new QLineEdit( frame3, "ap_radiusLineEdit" );
    ap_radiusLineEdit->setGeometry( QRect( 402, 39, 60, 22 ) );
    ap_radiusLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ap_radiusLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ap_radiusLineEdit->setMaxLength( 9 );
    ap_radiusLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ap_arccatComboBox = new QComboBox( FALSE, frame3, "ap_arccatComboBox" );
    ap_arccatComboBox->setGeometry( QRect( 13, 96, 130, 22 ) );
    ap_arccatComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, ap_arccatComboBox->sizePolicy().hasHeightForWidth() ) );
    ap_arccatComboBox->setMinimumSize( QSize( 130, 0 ) );
    ap_arccatComboBox->setMaximumSize( QSize( 130, 32767 ) );
    ap_arccatComboBox->setFocusPolicy( QComboBox::NoFocus );
    ap_arccatComboBox->setSizeLimit( 15 );

    ap_createsourcecatCheckBox = new QCheckBox( ap_astphotButtonGroup, "ap_createsourcecatCheckBox" );
    ap_createsourcecatCheckBox->setGeometry( QRect( 40, 260, 180, 20 ) );
    ap_createsourcecatCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont ap_createsourcecatCheckBox_font(  ap_createsourcecatCheckBox->font() );
    ap_createsourcecatCheckBox_font.setBold( TRUE );
    ap_createsourcecatCheckBox->setFont( ap_createsourcecatCheckBox_font ); 
    ap_createsourcecatCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    ap_createsourcecatCheckBox->setChecked( TRUE );

    ap_updateheaderPushButton = new QPushButton( ap_astphotButtonGroup, "ap_updateheaderPushButton" );
    ap_updateheaderPushButton->setGeometry( QRect( 270, 320, 110, 24 ) );

    ap_restoreheaderPushButton = new QPushButton( ap_astphotButtonGroup, "ap_restoreheaderPushButton" );
    ap_restoreheaderPushButton->setGeometry( QRect( 390, 320, 110, 24 ) );

    ap_astrommethodComboBox = new QComboBox( FALSE, ap_astphotButtonGroup, "ap_astrommethodComboBox" );
    ap_astrommethodComboBox->setGeometry( QRect( 240, 290, 140, 22 ) );
    ap_astrommethodComboBox->setFocusPolicy( QComboBox::NoFocus );

    ap_astrometryCheckBox = new QCheckBox( ap_astphotButtonGroup, "ap_astrometryCheckBox" );
    ap_astrometryCheckBox->setGeometry( QRect( 40, 290, 180, 20 ) );
    ap_astrometryCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont ap_astrometryCheckBox_font(  ap_astrometryCheckBox->font() );
    ap_astrometryCheckBox_font.setBold( TRUE );
    ap_astrometryCheckBox->setFont( ap_astrometryCheckBox_font ); 
    ap_astrometryCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    ap_astrometryCheckBox->setChecked( TRUE );

    ap_absphotom_direct_CheckBox = new QCheckBox( ap_astphotButtonGroup, "ap_absphotom_direct_CheckBox" );
    ap_absphotom_direct_CheckBox->setGeometry( QRect( 40, 230, 260, 20 ) );
    QFont ap_absphotom_direct_CheckBox_font(  ap_absphotom_direct_CheckBox->font() );
    ap_absphotom_direct_CheckBox_font.setBold( TRUE );
    ap_absphotom_direct_CheckBox->setFont( ap_absphotom_direct_CheckBox_font ); 
    ap_absphotom_direct_CheckBox->setFocusPolicy( QCheckBox::NoFocus );
    ap_absphotom_direct_CheckBox->setChecked( FALSE );

    layout38->addWidget( ap_astphotButtonGroup, 0, 0 );

    layout37 = new QVBoxLayout( 0, 0, 6, "layout37"); 
    spacer44 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout37->addItem( spacer44 );

    layout30 = new QGridLayout( 0, 1, 1, 0, 6, "layout30"); 

    ap_resetfieldsPushButton = new QPushButton( PageAstromPhotom, "ap_resetfieldsPushButton" );
    ap_resetfieldsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ap_resetfieldsPushButton->sizePolicy().hasHeightForWidth() ) );
    ap_resetfieldsPushButton->setMinimumSize( QSize( 110, 0 ) );
    ap_resetfieldsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    ap_resetfieldsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout30->addWidget( ap_resetfieldsPushButton, 1, 0 );
    spacer41_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout30->addItem( spacer41_2, 0, 2 );

    ap_killPushButton = new QPushButton( PageAstromPhotom, "ap_killPushButton" );
    ap_killPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ap_killPushButton->sizePolicy().hasHeightForWidth() ) );
    ap_killPushButton->setMinimumSize( QSize( 110, 0 ) );
    ap_killPushButton->setMaximumSize( QSize( 110, 32767 ) );
    ap_killPushButton->setPaletteForegroundColor( QColor( 185, 0, 0 ) );
    ap_killPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout30->addWidget( ap_killPushButton, 1, 1 );

    ap_startPushButton = new QPushButton( PageAstromPhotom, "ap_startPushButton" );
    ap_startPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ap_startPushButton->sizePolicy().hasHeightForWidth() ) );
    ap_startPushButton->setMinimumSize( QSize( 110, 0 ) );
    ap_startPushButton->setMaximumSize( QSize( 110, 32767 ) );
    ap_startPushButton->setPaletteForegroundColor( QColor( 0, 118, 0 ) );
    ap_startPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout30->addWidget( ap_startPushButton, 0, 1 );

    ap_clearscriptsPushButton = new QPushButton( PageAstromPhotom, "ap_clearscriptsPushButton" );
    ap_clearscriptsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ap_clearscriptsPushButton->sizePolicy().hasHeightForWidth() ) );
    ap_clearscriptsPushButton->setMinimumSize( QSize( 110, 0 ) );
    ap_clearscriptsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    ap_clearscriptsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout30->addWidget( ap_clearscriptsPushButton, 0, 0 );
    spacer38 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout30->addItem( spacer38, 2, 0 );
    layout37->addLayout( layout30 );

    ap_messagesGroupBox = new QGroupBox( PageAstromPhotom, "ap_messagesGroupBox" );
    ap_messagesGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, ap_messagesGroupBox->sizePolicy().hasHeightForWidth() ) );
    ap_messagesGroupBox->setMinimumSize( QSize( 0, 220 ) );
    ap_messagesGroupBox->setMaximumSize( QSize( 32767, 32767 ) );
    ap_messagesGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    ap_messagesGroupBox->setColumnLayout(0, Qt::Vertical );
    ap_messagesGroupBox->layout()->setSpacing( 6 );
    ap_messagesGroupBox->layout()->setMargin( 11 );
    ap_messagesGroupBoxLayout = new QGridLayout( ap_messagesGroupBox->layout() );
    ap_messagesGroupBoxLayout->setAlignment( Qt::AlignTop );

    ap_messagesTextEdit = new QTextEdit( ap_messagesGroupBox, "ap_messagesTextEdit" );
    ap_messagesTextEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    ap_messagesTextEdit->setFocusPolicy( QTextEdit::NoFocus );
    ap_messagesTextEdit->setFrameShadow( QTextEdit::Sunken );
    ap_messagesTextEdit->setWordWrap( QTextEdit::NoWrap );
    ap_messagesTextEdit->setReadOnly( TRUE );

    ap_messagesGroupBoxLayout->addWidget( ap_messagesTextEdit, 0, 0 );
    layout37->addWidget( ap_messagesGroupBox );

    layout38->addLayout( layout37, 0, 1 );

    ap_commandGroupBox = new QGroupBox( PageAstromPhotom, "ap_commandGroupBox" );
    ap_commandGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, ap_commandGroupBox->sizePolicy().hasHeightForWidth() ) );
    ap_commandGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    ap_commandGroupBox->setColumnLayout(0, Qt::Vertical );
    ap_commandGroupBox->layout()->setSpacing( 6 );
    ap_commandGroupBox->layout()->setMargin( 11 );
    ap_commandGroupBoxLayout = new QGridLayout( ap_commandGroupBox->layout() );
    ap_commandGroupBoxLayout->setAlignment( Qt::AlignTop );

    ap_commandTextEdit = new QTextEdit( ap_commandGroupBox, "ap_commandTextEdit" );
    ap_commandTextEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, ap_commandTextEdit->sizePolicy().hasHeightForWidth() ) );
    ap_commandTextEdit->setFocusPolicy( QTextEdit::ClickFocus );
    ap_commandTextEdit->setWordWrap( QTextEdit::NoWrap );

    ap_commandGroupBoxLayout->addWidget( ap_commandTextEdit, 0, 0 );

    layout38->addMultiCellWidget( ap_commandGroupBox, 1, 1, 0, 1 );

    PageAstromPhotomLayout->addLayout( layout38, 0, 0 );
    mainmenu_TabWidget->insertTab( PageAstromPhotom, QString::fromLatin1("") );

    PageCoadd = new QWidget( mainmenu_TabWidget, "PageCoadd" );
    PageCoaddLayout = new QGridLayout( PageCoadd, 1, 1, 11, 6, "PageCoaddLayout"); 

    layout40 = new QGridLayout( 0, 1, 1, 0, 6, "layout40"); 

    layout39 = new QVBoxLayout( 0, 0, 6, "layout39"); 
    spacer45 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout39->addItem( spacer45 );

    layout31 = new QGridLayout( 0, 1, 1, 0, 6, "layout31"); 

    cc_killPushButton = new QPushButton( PageCoadd, "cc_killPushButton" );
    cc_killPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cc_killPushButton->sizePolicy().hasHeightForWidth() ) );
    cc_killPushButton->setMinimumSize( QSize( 110, 0 ) );
    cc_killPushButton->setMaximumSize( QSize( 110, 32767 ) );
    cc_killPushButton->setPaletteForegroundColor( QColor( 185, 0, 0 ) );
    cc_killPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout31->addWidget( cc_killPushButton, 1, 1 );

    cc_clearscriptsPushButton = new QPushButton( PageCoadd, "cc_clearscriptsPushButton" );
    cc_clearscriptsPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cc_clearscriptsPushButton->sizePolicy().hasHeightForWidth() ) );
    cc_clearscriptsPushButton->setMinimumSize( QSize( 110, 0 ) );
    cc_clearscriptsPushButton->setMaximumSize( QSize( 110, 32767 ) );
    cc_clearscriptsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout31->addWidget( cc_clearscriptsPushButton, 0, 0 );
    spacer42_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout31->addMultiCell( spacer42_3, 0, 1, 2, 2 );

    cc_startPushButton = new QPushButton( PageCoadd, "cc_startPushButton" );
    cc_startPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cc_startPushButton->sizePolicy().hasHeightForWidth() ) );
    cc_startPushButton->setMinimumSize( QSize( 110, 0 ) );
    cc_startPushButton->setMaximumSize( QSize( 110, 32767 ) );
    cc_startPushButton->setPaletteForegroundColor( QColor( 0, 118, 0 ) );
    cc_startPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout31->addWidget( cc_startPushButton, 0, 1 );
    spacer37 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout31->addItem( spacer37, 2, 1 );
    layout39->addLayout( layout31 );

    cc_messagesGroupBox = new QGroupBox( PageCoadd, "cc_messagesGroupBox" );
    cc_messagesGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, cc_messagesGroupBox->sizePolicy().hasHeightForWidth() ) );
    cc_messagesGroupBox->setMinimumSize( QSize( 0, 220 ) );
    cc_messagesGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    cc_messagesGroupBox->setColumnLayout(0, Qt::Vertical );
    cc_messagesGroupBox->layout()->setSpacing( 6 );
    cc_messagesGroupBox->layout()->setMargin( 11 );
    cc_messagesGroupBoxLayout = new QGridLayout( cc_messagesGroupBox->layout() );
    cc_messagesGroupBoxLayout->setAlignment( Qt::AlignTop );

    cc_messagesTextEdit = new QTextEdit( cc_messagesGroupBox, "cc_messagesTextEdit" );
    cc_messagesTextEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    cc_messagesTextEdit->setFocusPolicy( QTextEdit::NoFocus );
    cc_messagesTextEdit->setFrameShadow( QTextEdit::Sunken );
    cc_messagesTextEdit->setWordWrap( QTextEdit::NoWrap );
    cc_messagesTextEdit->setReadOnly( TRUE );

    cc_messagesGroupBoxLayout->addWidget( cc_messagesTextEdit, 0, 0 );
    layout39->addWidget( cc_messagesGroupBox );

    layout40->addLayout( layout39, 0, 1 );

    cc_commandGroupBox = new QGroupBox( PageCoadd, "cc_commandGroupBox" );
    cc_commandGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, cc_commandGroupBox->sizePolicy().hasHeightForWidth() ) );
    cc_commandGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    cc_commandGroupBox->setColumnLayout(0, Qt::Vertical );
    cc_commandGroupBox->layout()->setSpacing( 6 );
    cc_commandGroupBox->layout()->setMargin( 11 );
    cc_commandGroupBoxLayout = new QGridLayout( cc_commandGroupBox->layout() );
    cc_commandGroupBoxLayout->setAlignment( Qt::AlignTop );

    cc_commandTextEdit = new QTextEdit( cc_commandGroupBox, "cc_commandTextEdit" );
    cc_commandTextEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, cc_commandTextEdit->sizePolicy().hasHeightForWidth() ) );
    cc_commandTextEdit->setFocusPolicy( QTextEdit::ClickFocus );
    cc_commandTextEdit->setHScrollBarMode( QTextEdit::Auto );
    cc_commandTextEdit->setWordWrap( QTextEdit::NoWrap );

    cc_commandGroupBoxLayout->addWidget( cc_commandTextEdit, 0, 0 );

    layout40->addMultiCellWidget( cc_commandGroupBox, 1, 1, 0, 1 );

    cc_coaddButtonGroup = new QButtonGroup( PageCoadd, "cc_coaddButtonGroup" );
    cc_coaddButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, cc_coaddButtonGroup->sizePolicy().hasHeightForWidth() ) );
    cc_coaddButtonGroup->setMinimumSize( QSize( 520, 350 ) );
    cc_coaddButtonGroup->setMaximumSize( QSize( 520, 350 ) );
    cc_coaddButtonGroup->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );

    cc_skysubconfigurePushButton = new QPushButton( cc_coaddButtonGroup, "cc_skysubconfigurePushButton" );
    cc_skysubconfigurePushButton->setGeometry( QRect( 230, 80, 110, 24 ) );
    cc_skysubconfigurePushButton->setMinimumSize( QSize( 110, 0 ) );
    cc_skysubconfigurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    cc_skysubconfigurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont cc_skysubconfigurePushButton_font(  cc_skysubconfigurePushButton->font() );
    cc_skysubconfigurePushButton_font.setBold( TRUE );
    cc_skysubconfigurePushButton->setFont( cc_skysubconfigurePushButton_font ); 
    cc_skysubconfigurePushButton->setFocusPolicy( QPushButton::NoFocus );

    cc_coaddconfigurePushButton = new QPushButton( cc_coaddButtonGroup, "cc_coaddconfigurePushButton" );
    cc_coaddconfigurePushButton->setGeometry( QRect( 230, 150, 110, 24 ) );
    cc_coaddconfigurePushButton->setMinimumSize( QSize( 110, 0 ) );
    cc_coaddconfigurePushButton->setMaximumSize( QSize( 110, 32767 ) );
    cc_coaddconfigurePushButton->setPaletteForegroundColor( QColor( 235, 106, 20 ) );
    QFont cc_coaddconfigurePushButton_font(  cc_coaddconfigurePushButton->font() );
    cc_coaddconfigurePushButton_font.setBold( TRUE );
    cc_coaddconfigurePushButton->setFont( cc_coaddconfigurePushButton_font ); 
    cc_coaddconfigurePushButton->setFocusPolicy( QPushButton::NoFocus );

    cc_resolvedirLineEdit = new QLineEdit( cc_coaddButtonGroup, "cc_resolvedirLineEdit" );
    cc_resolvedirLineEdit->setGeometry( QRect( 230, 240, 270, 23 ) );
    cc_resolvedirLineEdit->setPaletteBackgroundColor( QColor( 220, 255, 255 ) );
    cc_resolvedirLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    cc_resolvedirLineEdit->setMaxLength( 99 );
    cc_resolvedirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cc_resolvelinksCheckBox = new QCheckBox( cc_coaddButtonGroup, "cc_resolvelinksCheckBox" );
    cc_resolvelinksCheckBox->setGeometry( QRect( 20, 230, 180, 20 ) );
    QFont cc_resolvelinksCheckBox_font(  cc_resolvelinksCheckBox->font() );
    cc_resolvelinksCheckBox_font.setBold( TRUE );
    cc_resolvelinksCheckBox->setFont( cc_resolvelinksCheckBox_font ); 
    cc_resolvelinksCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    cc_textLabel2_2 = new QLabel( cc_coaddButtonGroup, "cc_textLabel2_2" );
    cc_textLabel2_2->setGeometry( QRect( 230, 220, 230, 20 ) );

    cc_createskysubCheckBox = new QCheckBox( cc_coaddButtonGroup, "cc_createskysubCheckBox" );
    cc_createskysubCheckBox->setGeometry( QRect( 20, 80, 180, 20 ) );
    cc_createskysubCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont cc_createskysubCheckBox_font(  cc_createskysubCheckBox->font() );
    cc_createskysubCheckBox_font.setBold( TRUE );
    cc_createskysubCheckBox->setFont( cc_createskysubCheckBox_font ); 
    cc_createskysubCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    cc_createskysubCheckBox->setChecked( TRUE );

    cc_coaddCheckBox = new QCheckBox( cc_coaddButtonGroup, "cc_coaddCheckBox" );
    cc_coaddCheckBox->setGeometry( QRect( 20, 150, 180, 20 ) );
    cc_coaddCheckBox->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    QFont cc_coaddCheckBox_font(  cc_coaddCheckBox->font() );
    cc_coaddCheckBox_font.setBold( TRUE );
    cc_coaddCheckBox->setFont( cc_coaddCheckBox_font ); 
    cc_coaddCheckBox->setFocusPolicy( QCheckBox::NoFocus );
    cc_coaddCheckBox->setChecked( TRUE );

    layout40->addWidget( cc_coaddButtonGroup, 0, 0 );

    PageCoaddLayout->addLayout( layout40, 0, 0 );
    mainmenu_TabWidget->insertTab( PageCoadd, QString::fromLatin1("") );

    theliFormLayout->addWidget( mainmenu_TabWidget, 0, 0 );

    // actions
    settingsGUIfontAction = new QAction( this, "settingsGUIfontAction" );
    viewCommandhistoryAction = new QAction( this, "viewCommandhistoryAction" );
    helpTechpaperAction = new QAction( this, "helpTechpaperAction" );
    helpTechpaperAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );
    helpVersionAction = new QAction( this, "helpVersionAction" );
    editScriptsAction = new QAction( this, "editScriptsAction" );
    ActionGroupEditors = new QActionGroup( this, "ActionGroupEditors" );
    ActionGroupEditors->setOn( FALSE );
    ActionGroupEditors->setUsesDropDown( TRUE );
    nano = new QAction( ActionGroupEditors, "nano" );
    nano->setToggleAction( TRUE );
    emacs = new QAction( ActionGroupEditors, "emacs" );
    emacs->setToggleAction( TRUE );
    xemacs = new QAction( ActionGroupEditors, "xemacs" );
    xemacs->setToggleAction( TRUE );
    nedit = new QAction( ActionGroupEditors, "nedit" );
    nedit->setToggleAction( TRUE );
    vi = new QAction( ActionGroupEditors, "vi" );
    vi->setToggleAction( TRUE );
    ActionGroupImageViewers = new QActionGroup( this, "ActionGroupImageViewers" );
    ActionGroupImageViewers->setUsesDropDown( TRUE );
    skycat = new QAction( ActionGroupImageViewers, "skycat" );
    skycat->setToggleAction( TRUE );
    ds9 = new QAction( ActionGroupImageViewers, "ds9" );
    ds9->setToggleAction( TRUE );
    ActionGroupPDFViewers = new QActionGroup( this, "ActionGroupPDFViewers" );
    ActionGroupPDFViewers->setUsesDropDown( TRUE );
    pdfacroread = new QAction( ActionGroupPDFViewers, "pdfacroread" );
    pdfacroread->setToggleAction( TRUE );
    pdfgvv = new QAction( ActionGroupPDFViewers, "pdfgvv" );
    pdfgvv->setToggleAction( TRUE );
    pdfxpdf = new QAction( ActionGroupPDFViewers, "pdfxpdf" );
    pdfxpdf->setToggleAction( TRUE );
    viewDatadirFITSAction = new QAction( this, "viewDatadirFITSAction" );
    viewProcessinglogsPreparation_logAction = new QAction( this, "viewProcessinglogsPreparation_logAction" );
    viewProcessinglogsCalibration_logAction = new QAction( this, "viewProcessinglogsCalibration_logAction" );
    viewProcessinglogsBackground_logAction = new QAction( this, "viewProcessinglogsBackground_logAction" );
    viewProcessinglogsWeighting_logAction = new QAction( this, "viewProcessinglogsWeighting_logAction" );
    viewProcessinglogsAstromPhotom_logAction = new QAction( this, "viewProcessinglogsAstromPhotom_logAction" );
    viewProcessinglogsCoaddition_logAction = new QAction( this, "viewProcessinglogsCoaddition_logAction" );
    deleteSPLIT_IMAGESAction = new QAction( this, "deleteSPLIT_IMAGESAction" );
    deleteMASK_IMAGESAction = new QAction( this, "deleteMASK_IMAGESAction" );
    deleteOFC_IMAGESAction = new QAction( this, "deleteOFC_IMAGESAction" );
    deleteOFCB_IMAGESAction = new QAction( this, "deleteOFCB_IMAGESAction" );
    deleteOFCH_IMAGESAction = new QAction( this, "deleteOFCH_IMAGESAction" );
    deleteundebloomedAction = new QAction( this, "deleteundebloomedAction" );
    deletebeforedirectphotAction = new QAction( this, "deletebeforedirectphotAction" );
    deleteresampledAction = new QAction( this, "deleteresampledAction" );
    helpWhats_thisAction = new QAction( this, "helpWhats_thisAction" );
    deleteFlush_tmp_dirAction = new QAction( this, "deleteFlush_tmp_dirAction" );
    statusCPSDebiasingAction = new QAction( this, "statusCPSDebiasingAction" );
    statusCPSDebiasingAction->setToggleAction( TRUE );
    statusCPSBackgroundAction = new QAction( this, "statusCPSBackgroundAction" );
    statusCPSBackgroundAction->setToggleAction( TRUE );
    statusCPSChopnodAction = new QAction( this, "statusCPSChopnodAction" );
    statusCPSChopnodAction->setToggleAction( TRUE );
    statusCPSCollapseAction = new QAction( this, "statusCPSCollapseAction" );
    statusCPSCollapseAction->setToggleAction( TRUE );
    statusCPSDebloomingAction = new QAction( this, "statusCPSDebloomingAction" );
    statusCPSDebloomingAction->setToggleAction( TRUE );
    statusCPSDirectphotAction = new QAction( this, "statusCPSDirectphotAction" );
    statusCPSDirectphotAction->setToggleAction( TRUE );
    miscellaneousMake_colour_picture_Action = new QAction( this, "miscellaneousMake_colour_picture_Action" );
    editProfessional_instrumentsAction = new QAction( this, "editProfessional_instrumentsAction" );
    miscellaneousCombineImagesAction = new QAction( this, "miscellaneousCombineImagesAction" );
    miscellaneousImage_statisticsAction = new QAction( this, "miscellaneousImage_statisticsAction" );
    miscellaneousImalyzerAction = new QAction( this, "miscellaneousImalyzerAction" );
    miscellaneousGetZP_Action = new QAction( this, "miscellaneousGetZP_Action" );
    miscellaneousAnimateAction = new QAction( this, "miscellaneousAnimateAction" );
    miscellaneousProgress_MonitorAction = new QAction( this, "miscellaneousProgress_MonitorAction" );
    editCommercial_instrumentsAction = new QAction( this, "editCommercial_instrumentsAction" );
    editUser_defined_instrumentsAction = new QAction( this, "editUser_defined_instrumentsAction" );


    // toolbars


    // menubar
    theliMenuBar = new QMenuBar( this, "theliMenuBar" );


    Edit = new QPopupMenu( this );
    editScriptsAction->addTo( Edit );
    editProfessional_instrumentsAction->addTo( Edit );
    editCommercial_instrumentsAction->addTo( Edit );
    editUser_defined_instrumentsAction->addTo( Edit );
    theliMenuBar->insertItem( QString(""), Edit, 1 );

    Settings = new QPopupMenu( this );
    settingsGUIfontAction->addTo( Settings );
    Settings->insertSeparator();
    popupMenu_7 = new QPopupMenu( this );
    Settings->insertItem( ActionGroupEditors->iconSet(), tr( "Editor" ), popupMenu_7 );
    nano->addTo( popupMenu_7 );
    emacs->addTo( popupMenu_7 );
    xemacs->addTo( popupMenu_7 );
    nedit->addTo( popupMenu_7 );
    vi->addTo( popupMenu_7 );
    popupMenu_13 = new QPopupMenu( this );
    Settings->insertItem( ActionGroupImageViewers->iconSet(), tr( "Image viewer" ), popupMenu_13 );
    skycat->addTo( popupMenu_13 );
    ds9->addTo( popupMenu_13 );
    popupMenu_16 = new QPopupMenu( this );
    Settings->insertItem( ActionGroupPDFViewers->iconSet(), tr( "PDF viewer" ), popupMenu_16 );
    pdfacroread->addTo( popupMenu_16 );
    pdfgvv->addTo( popupMenu_16 );
    pdfxpdf->addTo( popupMenu_16 );
    theliMenuBar->insertItem( QString(""), Settings, 2 );

    View = new QPopupMenu( this );
    viewDatadirFITSAction->addTo( View );
    viewCommandhistoryAction->addTo( View );
    View->insertSeparator();
    viewProcessinglogsPreparation_logAction->addTo( View );
    viewProcessinglogsCalibration_logAction->addTo( View );
    viewProcessinglogsBackground_logAction->addTo( View );
    viewProcessinglogsWeighting_logAction->addTo( View );
    viewProcessinglogsAstromPhotom_logAction->addTo( View );
    viewProcessinglogsCoaddition_logAction->addTo( View );
    theliMenuBar->insertItem( QString(""), View, 3 );

    Status = new QPopupMenu( this );
    statusCPSDebiasingAction->addTo( Status );
    statusCPSBackgroundAction->addTo( Status );
    statusCPSChopnodAction->addTo( Status );
    statusCPSCollapseAction->addTo( Status );
    statusCPSDebloomingAction->addTo( Status );
    statusCPSDirectphotAction->addTo( Status );
    theliMenuBar->insertItem( QString(""), Status, 4 );

    Miscellaneous = new QPopupMenu( this );
    miscellaneousCombineImagesAction->addTo( Miscellaneous );
    miscellaneousImalyzerAction->addTo( Miscellaneous );
    miscellaneousImage_statisticsAction->addTo( Miscellaneous );
    miscellaneousGetZP_Action->addTo( Miscellaneous );
    miscellaneousAnimateAction->addTo( Miscellaneous );
    miscellaneousMake_colour_picture_Action->addTo( Miscellaneous );
    miscellaneousProgress_MonitorAction->addTo( Miscellaneous );
    theliMenuBar->insertItem( QString(""), Miscellaneous, 5 );

    Delete = new QPopupMenu( this );
    deleteSPLIT_IMAGESAction->addTo( Delete );
    deleteMASK_IMAGESAction->addTo( Delete );
    deleteOFC_IMAGESAction->addTo( Delete );
    deleteOFCB_IMAGESAction->addTo( Delete );
    deleteOFCH_IMAGESAction->addTo( Delete );
    deleteundebloomedAction->addTo( Delete );
    deletebeforedirectphotAction->addTo( Delete );
    deleteresampledAction->addTo( Delete );
    Delete->insertSeparator();
    deleteFlush_tmp_dirAction->addTo( Delete );
    theliMenuBar->insertItem( QString(""), Delete, 6 );

    Help = new QPopupMenu( this );
    helpWhats_thisAction->addTo( Help );
    Help->insertSeparator();
    helpTechpaperAction->addTo( Help );
    Help->insertSeparator();
    helpVersionAction->addTo( Help );
    theliMenuBar->insertItem( QString(""), Help, 7 );

    languageChange();
    resize( QSize(813, 664).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( miscellaneousAnimateAction, SIGNAL( activated() ), this, SLOT( call_animate() ) );
    connect( in_createselfPushButton, SIGNAL( clicked() ), this, SLOT( call_cni() ) );
    connect( miscellaneousMake_colour_picture_Action, SIGNAL( activated() ), this, SLOT( call_color() ) );
    connect( miscellaneousCombineImagesAction, SIGNAL( activated() ), this, SLOT( call_combineimages() ) );
    connect( viewCommandhistoryAction, SIGNAL( activated() ), this, SLOT( call_commandhistory() ) );
    connect( miscellaneousImage_statisticsAction, SIGNAL( activated() ), this, SLOT( call_imagestatistics() ) );
    connect( miscellaneousImalyzerAction, SIGNAL( activated() ), this, SLOT( call_imalyzer() ) );
    connect( miscellaneousProgress_MonitorAction, SIGNAL( activated() ), this, SLOT( call_monitor() ) );
    connect( ap_absphotom_direct_configurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_absphotom_direct() ) );
    connect( ap_absphotom_indirect_configurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_absphotom_indirect() ) );
    connect( ap_astrommethodconfigurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_astrom() ) );
    connect( cal_configurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_cal() ) );
    connect( cc_coaddconfigurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_cc1() ) );
    connect( pre_configurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_pre() ) );
    connect( ap_sexcatconfigurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_sexcat() ) );
    connect( sf_backgroundconfigurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_sf1() ) );
    connect( sf_collconfigurePushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_sf2() ) );
    connect( w_configure1PushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_w1() ) );
    connect( w_configure2PushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_w2() ) );
    connect( w_configure3PushButton, SIGNAL( clicked() ), this, SLOT( call_parameters_w3() ) );
    connect( ap_selectimagePushButton, SIGNAL( clicked() ), this, SLOT( call_select_astroref_image() ) );
    connect( cc_skysubconfigurePushButton, SIGNAL( clicked() ), this, SLOT( call_skysubmethod() ) );
    connect( miscellaneousGetZP_Action, SIGNAL( activated() ), this, SLOT( call_zp() ) );
    connect( statusCPSChopnodAction, SIGNAL( activated() ), this, SLOT( changestatus_set_imextension() ) );
    connect( statusCPSCollapseAction, SIGNAL( activated() ), this, SLOT( changestatus_set_imextension() ) );
    connect( statusCPSDebiasingAction, SIGNAL( activated() ), this, SLOT( changestatus_set_imextension() ) );
    connect( statusCPSDebloomingAction, SIGNAL( activated() ), this, SLOT( changestatus_set_imextension() ) );
    connect( statusCPSDirectphotAction, SIGNAL( activated() ), this, SLOT( changestatus_set_imextension() ) );
    connect( statusCPSBackgroundAction, SIGNAL( activated() ), this, SLOT( changestatus_set_imextension() ) );
    connect( cal_nonlinCheckBox, SIGNAL( clicked() ), this, SLOT( check_nonlinearity() ) );
    connect( ap_refdecLineEdit, SIGNAL( selectionChanged() ), this, SLOT( clean_refcoordsLineEdit() ) );
    connect( ap_refraLineEdit, SIGNAL( selectionChanged() ), this, SLOT( clean_refcoordsLineEdit() ) );
    connect( in_clearPushButton, SIGNAL( clicked() ), this, SLOT( cleardirs_initialisepushbutton() ) );
    connect( in_clearprocstatusPushButton, SIGNAL( clicked() ), this, SLOT( clear_processingstatus_in_logfile() ) );
    connect( in_restoreallPushButton, SIGNAL( clicked() ), this, SLOT( clear_processingstatus_in_logfile() ) );
    connect( ap_clearscriptsPushButton, SIGNAL( clicked() ), this, SLOT( clearscripts_ap_pushbutton() ) );
    connect( cal_clearscriptsPushButton, SIGNAL( clicked() ), this, SLOT( clearscripts_cal_pushbutton() ) );
    connect( cc_clearscriptsPushButton, SIGNAL( clicked() ), this, SLOT( clearscripts_cc_pushbutton() ) );
    connect( pre_clearscriptsPushButton, SIGNAL( clicked() ), this, SLOT( clearscripts_pre_pushbutton() ) );
    connect( sf_clearscriptsPushButton, SIGNAL( clicked() ), this, SLOT( clearscripts_sf_pushbutton() ) );
    connect( w_clearscriptsPushButton, SIGNAL( clicked() ), this, SLOT( clearscripts_w_pushbutton() ) );
    connect( deletebeforedirectphotAction, SIGNAL( activated() ), this, SLOT( delete_beforedirectphot() ) );
    connect( deleteMASK_IMAGESAction, SIGNAL( activated() ), this, SLOT( delete_MASK_IMAGES() ) );
    connect( deleteOFCB_IMAGESAction, SIGNAL( activated() ), this, SLOT( delete_OFCB_IMAGES() ) );
    connect( deleteOFCH_IMAGESAction, SIGNAL( activated() ), this, SLOT( delete_OFCH_IMAGES() ) );
    connect( deleteOFC_IMAGESAction, SIGNAL( activated() ), this, SLOT( delete_OFC_IMAGES() ) );
    connect( deleteresampledAction, SIGNAL( activated() ), this, SLOT( delete_resampled() ) );
    connect( deleteSPLIT_IMAGESAction, SIGNAL( activated() ), this, SLOT( delete_SPLIT_IMAGES() ) );
    connect( deleteundebloomedAction, SIGNAL( activated() ), this, SLOT( delete_undebloomed() ) );
    connect( editCommercial_instrumentsAction, SIGNAL( activated() ), this, SLOT( edit_comminstconfigs() ) );
    connect( editProfessional_instrumentsAction, SIGNAL( activated() ), this, SLOT( edit_profinstconfigs() ) );
    connect( editScriptsAction, SIGNAL( activated() ), this, SLOT( edit_scripts() ) );
    connect( editUser_defined_instrumentsAction, SIGNAL( activated() ), this, SLOT( edit_userinstconfigs() ) );
    connect( ap_arccatComboBox, SIGNAL( activated(const QString&) ), this, SLOT( enable_server() ) );
    connect( ap_startPushButton, SIGNAL( clicked() ), this, SLOT( exec_ap() ) );
    connect( cal_startPushButton, SIGNAL( clicked() ), this, SLOT( exec_cal() ) );
    connect( cc_startPushButton, SIGNAL( clicked() ), this, SLOT( exec_cc() ) );
    connect( pre_startPushButton, SIGNAL( clicked() ), this, SLOT( exec_pre() ) );
    connect( sf_startPushButton, SIGNAL( clicked() ), this, SLOT( exec_sf() ) );
    connect( w_startPushButton, SIGNAL( clicked() ), this, SLOT( exec_w() ) );
    connect( deleteFlush_tmp_dirAction, SIGNAL( activated() ), this, SLOT( flush_tempdir() ) );
    connect( settingsGUIfontAction, SIGNAL( activated() ), this, SLOT( font_pushbuttonclicked() ) );
    connect( ap_getastrefcatalogPushButton, SIGNAL( clicked() ), this, SLOT( get_astref_catalog_button() ) );
    connect( in_instrumentListBox, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( get_cameratype() ) );
    connect( in_instrumentListBox, SIGNAL( currentChanged(QListBoxItem*) ), this, SLOT( get_cameratype() ) );
    connect( in_instrumentListBox, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( get_nframes() ) );
    connect( in_instrumentListBox, SIGNAL( currentChanged(QListBoxItem*) ), this, SLOT( get_nframes() ) );
    connect( in_numcpuSpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( get_nframes() ) );
    connect( ap_killPushButton, SIGNAL( clicked() ), this, SLOT( kill_ap() ) );
    connect( cal_killPushButton, SIGNAL( clicked() ), this, SLOT( kill_cal() ) );
    connect( cc_killPushButton, SIGNAL( clicked() ), this, SLOT( kill_cc() ) );
    connect( pre_killPushButton, SIGNAL( clicked() ), this, SLOT( kill_pre() ) );
    connect( sf_killPushButton, SIGNAL( clicked() ), this, SLOT( kill_sf() ) );
    connect( w_killPushButton, SIGNAL( clicked() ), this, SLOT( kill_w() ) );
    connect( in_loadlogPushButton, SIGNAL( clicked() ), this, SLOT( load_reductionlog() ) );
    connect( in_numcpuSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( modify_npara() ) );
    connect( helpTechpaperAction, SIGNAL( activated() ), this, SLOT( open_pipelinepaper() ) );
    connect( sf_backgroundredoPushButton, SIGNAL( clicked() ), this, SLOT( redo_background() ) );
    connect( cal_calibredoPushButton, SIGNAL( clicked() ), this, SLOT( redo_calib() ) );
    connect( sf_chopnodredoPushButton, SIGNAL( clicked() ), this, SLOT( redo_chopnod() ) );
    connect( sf_collapseredoPushButton, SIGNAL( clicked() ), this, SLOT( redo_collapse() ) );
    connect( w_debloomredoPushButton, SIGNAL( clicked() ), this, SLOT( redo_debloom() ) );
    connect( ap_directphotredoPushButton, SIGNAL( clicked() ), this, SLOT( redo_directphot() ) );
    connect( in_comminstRadioButton, SIGNAL( clicked() ), this, SLOT( refresh_instlist() ) );
    connect( in_profinstRadioButton, SIGNAL( clicked() ), this, SLOT( refresh_instlist() ) );
    connect( in_userinstRadioButton, SIGNAL( clicked() ), this, SLOT( refresh_instlist() ) );
    connect( in_biasdirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( repaint_defaultpaths() ) );
    connect( in_darkdirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( repaint_defaultpaths() ) );
    connect( in_flatdirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( repaint_defaultpaths() ) );
    connect( in_maindirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( repaint_defaultpaths() ) );
    connect( in_skydirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( repaint_defaultpaths() ) );
    connect( in_sciencedirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( repaint_defaultpaths() ) );
    connect( in_standarddirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( repaint_defaultpaths() ) );
    connect( mainmenu_TabWidget, SIGNAL( currentChanged(QWidget*) ), this, SLOT( repaint_defaultpaths() ) );
    connect( in_flatoffdirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( repaint_defaultpaths() ) );
    connect( ap_resetfieldsPushButton, SIGNAL( clicked() ), this, SLOT( resetfields_ap_pushbutton() ) );
    connect( cal_resetfieldsPushButton, SIGNAL( clicked() ), this, SLOT( resetfields_cal_pushbutton() ) );
    connect( sf_resetsfieldsPushButton, SIGNAL( clicked() ), this, SLOT( resetfields_sf_pushbutton() ) );
    connect( w_resetfieldsPushButton, SIGNAL( clicked() ), this, SLOT( resetfields_w_pushbutton() ) );
    connect( ap_resolvePushButton, SIGNAL( clicked() ), this, SLOT( resolve_target() ) );
    connect( in_restoreallPushButton, SIGNAL( clicked() ), this, SLOT( restore_ALL() ) );
    connect( in_restorebiasPushButton, SIGNAL( clicked() ), this, SLOT( restore_BIAS() ) );
    connect( in_restoredarkPushButton, SIGNAL( clicked() ), this, SLOT( restore_DARK() ) );
    connect( in_restoreflatPushButton, SIGNAL( clicked() ), this, SLOT( restore_FLAT() ) );
    connect( in_restoreflatoffPushButton, SIGNAL( clicked() ), this, SLOT( restore_FLATOFF() ) );
    connect( ap_restoreheaderPushButton, SIGNAL( clicked() ), this, SLOT( restore_header() ) );
    connect( in_restoresciencePushButton, SIGNAL( clicked() ), this, SLOT( restore_SCIENCE() ) );
    connect( in_restoreskyPushButton, SIGNAL( clicked() ), this, SLOT( restore_SKY() ) );
    connect( in_restorestandardPushButton, SIGNAL( clicked() ), this, SLOT( restore_STANDARD() ) );
    connect( viewProcessinglogsAstromPhotom_logAction, SIGNAL( activated() ), this, SLOT( show_aplog() ) );
    connect( viewProcessinglogsCalibration_logAction, SIGNAL( activated() ), this, SLOT( show_callog() ) );
    connect( viewProcessinglogsCoaddition_logAction, SIGNAL( activated() ), this, SLOT( show_cclog() ) );
    connect( viewProcessinglogsPreparation_logAction, SIGNAL( activated() ), this, SLOT( show_prelog() ) );
    connect( viewProcessinglogsBackground_logAction, SIGNAL( activated() ), this, SLOT( show_sflog() ) );
    connect( helpVersionAction, SIGNAL( activated() ), this, SLOT( show_software_version() ) );
    connect( viewProcessinglogsWeighting_logAction, SIGNAL( activated() ), this, SLOT( show_wlog() ) );
    connect( tf_styleSpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( style_pushbuttonclicked() ) );
    connect( cal_nobiasCheckBox, SIGNAL( clicked() ), this, SLOT( toggle_darkinsteadbiascheckbox() ) );
    connect( ap_absphotom_direct_CheckBox, SIGNAL( clicked() ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_absphotom_indirect_CheckBox, SIGNAL( clicked() ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_arccatComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_arcserverComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_astrometryCheckBox, SIGNAL( clicked() ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_astrommethodComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_createsourcecatCheckBox, SIGNAL( clicked() ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_refdecLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_refimagedetectminareaLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_refimagedetectthreshLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_refimageLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_refraLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_commandlist() ) );
    connect( ap_arcserverComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_ap_variousfields() ) );
    connect( ap_maglimLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_variousfields() ) );
    connect( ap_radiusLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_variousfields() ) );
    connect( ap_refimagedetectminareaLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_variousfields() ) );
    connect( ap_refimagedetectthreshLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_variousfields() ) );
    connect( ap_refimageLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_ap_variousfields() ) );
    connect( cal_nobiasCheckBox, SIGNAL( clicked() ), this, SLOT( update_biascheckboxcolor() ) );
    connect( cal_noflatCheckBox, SIGNAL( clicked() ), this, SLOT( update_biascheckboxcolor() ) );
    connect( cal_biasmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_biasminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_darkinsteadbiasCheckBox, SIGNAL( clicked() ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_flatmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_flatminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_nobiasCheckBox, SIGNAL( clicked() ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_noflatCheckBox, SIGNAL( clicked() ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_procbiasCheckBox, SIGNAL( clicked() ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_procdarkCheckBox, SIGNAL( clicked() ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_procflatCheckBox, SIGNAL( clicked() ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_procscienceCheckBox, SIGNAL( clicked() ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_sciencemaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_cal_commandlist() ) );
    connect( cal_scienceminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_cal_commandlist() ) );
    connect( cc_coaddCheckBox, SIGNAL( clicked() ), this, SLOT( update_cc_commandlist() ) );
    connect( cc_createskysubCheckBox, SIGNAL( clicked() ), this, SLOT( update_cc_commandlist() ) );
    connect( cc_resolvedirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_cc_commandlist() ) );
    connect( cc_resolvelinksCheckBox, SIGNAL( clicked() ), this, SLOT( update_cc_commandlist() ) );
    connect( statusCPSChopnodAction, SIGNAL( activated() ), this, SLOT( update_current_commandlist() ) );
    connect( statusCPSCollapseAction, SIGNAL( activated() ), this, SLOT( update_current_commandlist() ) );
    connect( statusCPSDebiasingAction, SIGNAL( activated() ), this, SLOT( update_current_commandlist() ) );
    connect( statusCPSDebloomingAction, SIGNAL( activated() ), this, SLOT( update_current_commandlist() ) );
    connect( statusCPSDirectphotAction, SIGNAL( activated() ), this, SLOT( update_current_commandlist() ) );
    connect( statusCPSBackgroundAction, SIGNAL( activated() ), this, SLOT( update_current_commandlist() ) );
    connect( mainmenu_TabWidget, SIGNAL( currentChanged(QWidget*) ), this, SLOT( update_logfile() ) );
    connect( in_reductionlogLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_mainwindowtitle() ) );
    connect( in_instrumentListBox, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( update_pre_commandlist() ) );
    connect( pre_createlinksCheckBox, SIGNAL( clicked() ), this, SLOT( update_pre_commandlist() ) );
    connect( pre_linkchipsLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_pre_commandlist() ) );
    connect( pre_scratchdirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_pre_commandlist() ) );
    connect( pre_sortdataCheckBox, SIGNAL( clicked() ), this, SLOT( update_pre_commandlist() ) );
    connect( pre_splitCheckBox, SIGNAL( clicked() ), this, SLOT( update_pre_commandlist() ) );
    connect( in_instrumentListBox, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( update_presinst() ) );
    connect( in_instrumentListBox, SIGNAL( currentChanged(QListBoxItem*) ), this, SLOT( update_presinst() ) );
    connect( sf_chopnodCheckBox, SIGNAL( clicked() ), this, SLOT( update_sf_commandlist() ) );
    connect( sf_chopnodinvertCheckBox, SIGNAL( clicked() ), this, SLOT( update_sf_commandlist() ) );
    connect( sf_chopnodpatternComboBox, SIGNAL( textChanged(const QString&) ), this, SLOT( update_sf_commandlist() ) );
    connect( sf_collapseCheckBox, SIGNAL( clicked() ), this, SLOT( update_sf_commandlist() ) );
    connect( sf_mergesequenceCheckBox, SIGNAL( clicked() ), this, SLOT( update_sf_commandlist() ) );
    connect( sf_backgroundCheckBox, SIGNAL( clicked() ), this, SLOT( update_sf_commandlist() ) );
    connect( mainmenu_TabWidget, SIGNAL( currentChanged(QWidget*) ), this, SLOT( update_tabchanged_commandlists() ) );
    connect( in_instrumentListBox, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( update_w_commandlist() ) );
    connect( w_bloomthreshLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_w_commandlist() ) );
    connect( w_createbinmosCheckBox, SIGNAL( clicked() ), this, SLOT( update_w_commandlist() ) );
    connect( w_createglobwCheckBox, SIGNAL( clicked() ), this, SLOT( update_w_commandlist() ) );
    connect( w_createweightsCheckBox, SIGNAL( clicked() ), this, SLOT( update_w_commandlist() ) );
    connect( w_debloomCheckBox, SIGNAL( clicked() ), this, SLOT( update_w_commandlist() ) );
    connect( w_distsetCheckBox, SIGNAL( clicked() ), this, SLOT( update_w_commandlist() ) );
    connect( w_minoverlapLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_w_commandlist() ) );
    connect( ap_updateheaderPushButton, SIGNAL( clicked() ), this, SLOT( update_zeroheader() ) );
    connect( ap_refdecLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate_refcoords() ) );
    connect( ap_refraLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate_refcoords() ) );
    connect( viewDatadirFITSAction, SIGNAL( activated() ), this, SLOT( viewimageOpen() ) );
    connect( in_instrumentListBox, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( which_instlist() ) );
    connect( sf_spreadsequenceCheckBox, SIGNAL( clicked() ), this, SLOT( update_sf_commandlist() ) );

    // tab order
    setTabOrder( in_reductionlogLineEdit, in_datadiskwarnSpinBox );
    setTabOrder( in_datadiskwarnSpinBox, in_homediskwarnSpinBox );
    setTabOrder( in_homediskwarnSpinBox, in_numcpuSpinBox );
    setTabOrder( in_numcpuSpinBox, tf_styleSpinBox );
    setTabOrder( tf_styleSpinBox, in_nframesSpinBox );
    setTabOrder( in_nframesSpinBox, in_maindirLineEdit );
    setTabOrder( in_maindirLineEdit, in_biasdirLineEdit );
    setTabOrder( in_biasdirLineEdit, in_darkdirLineEdit );
    setTabOrder( in_darkdirLineEdit, in_flatdirLineEdit );
    setTabOrder( in_flatdirLineEdit, in_flatoffdirLineEdit );
    setTabOrder( in_flatoffdirLineEdit, in_sciencedirLineEdit );
    setTabOrder( in_sciencedirLineEdit, in_skydirLineEdit );
    setTabOrder( in_skydirLineEdit, in_standarddirLineEdit );
    setTabOrder( in_standarddirLineEdit, pre_linkchipsLineEdit );
    setTabOrder( pre_linkchipsLineEdit, pre_scratchdirLineEdit );
    setTabOrder( pre_scratchdirLineEdit, cal_biasminLineEdit );
    setTabOrder( cal_biasminLineEdit, cal_biasmaxLineEdit );
    setTabOrder( cal_biasmaxLineEdit, cal_flatminLineEdit );
    setTabOrder( cal_flatminLineEdit, cal_flatmaxLineEdit );
    setTabOrder( cal_flatmaxLineEdit, cal_scienceminLineEdit );
    setTabOrder( cal_scienceminLineEdit, cal_sciencemaxLineEdit );
    setTabOrder( cal_sciencemaxLineEdit, w_bloomthreshLineEdit );
    setTabOrder( w_bloomthreshLineEdit, w_minoverlapLineEdit );
    setTabOrder( w_minoverlapLineEdit, ap_refraLineEdit );
    setTabOrder( ap_refraLineEdit, ap_refdecLineEdit );
    setTabOrder( ap_refdecLineEdit, ap_maglimLineEdit );
    setTabOrder( ap_maglimLineEdit, ap_radiusLineEdit );
    setTabOrder( ap_radiusLineEdit, ap_refimagedetectthreshLineEdit );
    setTabOrder( ap_refimagedetectthreshLineEdit, ap_refimagedetectminareaLineEdit );
    setTabOrder( ap_refimagedetectminareaLineEdit, ap_refimageLineEdit );
    setTabOrder( ap_refimageLineEdit, cc_resolvedirLineEdit );

    // buddies
    in_nframesTextLabel->setBuddy( in_nframesSpinBox );
    in_TextLabel3_2->setBuddy( tf_styleSpinBox );
    in_TextLabel3->setBuddy( in_numcpuSpinBox );
    textLabel2_3->setBuddy( in_reductionlogLineEdit );
    in_maindirTextLabel->setBuddy( in_maindirLineEdit );
    in_darkdirTextLabel->setBuddy( in_darkdirLineEdit );
    in_biasdirTextLabel->setBuddy( in_biasdirLineEdit );
    in_flatdirTextLabel_2_2->setBuddy( in_skydirLineEdit );
    in_flatdirTextLabel_2->setBuddy( in_skydirLineEdit );
    in_sciencedirTextLabel->setBuddy( in_sciencedirLineEdit );
    in_flatdirTextLabel->setBuddy( in_flatdirLineEdit );
    in_flatdirTextLabel_3->setBuddy( in_flatdirLineEdit );
    pre_TextLabel12_2->setBuddy( pre_linkchipsLineEdit );
    pre_TextLabel12_2_2->setBuddy( pre_scratchdirLineEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
theliForm::~theliForm()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void theliForm::languageChange()
{
    setCaption( tr( "THELI GUI v.2.10.4" ) );
    QWhatsThis::add( this, QString::null );
    QToolTip::add( mainmenu_TabWidget, QString::null );
    QWhatsThis::add( mainmenu_TabWidget, QString::null );
    in_instselectGroupBox->setTitle( tr( "Select instrument" ) );
    QWhatsThis::add( in_instselectGroupBox, tr( "<p><font color=\"#aa0000\" size=\"+1\">Select instrument</font><br>\n"
"Here you can select from a range of predefined instruments. You can also create your own instrument which is then permanently added to the already existing ones.</p>\n"
"\n"
"<p>If you think a commonly used instrument is missing from this list (certainly the case), then please drop us the configuration file you prepared for it (<b><font color=\"#0000ff\">Create new instrument</font></b>) so that we can include it. Please provide us also with all necessary information concerning peculiarities of this instrument, such as orientation, flipping, etc...</p>" ) );
    in_createselfPushButton->setText( tr( "Create instrument\n"
"(user defined)" ) );
    QToolTip::add( in_createselfPushButton, tr( "Create a new instrument which will appear in the user-defined section" ) );
    in_instButtonGroup->setTitle( QString::null );
    in_profinstRadioButton->setText( tr( "Professional" ) );
    QToolTip::add( in_profinstRadioButton, tr( "Select from a list of pre-defined cameras at various telescopes" ) );
    in_comminstRadioButton->setText( tr( "Commercial" ) );
    QToolTip::add( in_comminstRadioButton, tr( "Commercially available camera types" ) );
    in_userinstRadioButton->setText( tr( "User defined" ) );
    QToolTip::add( in_userinstRadioButton, tr( "User-defined cameras" ) );
    QToolTip::add( in_instrumentListBox, tr( "Select the instrument you used." ) );
    QToolTip::add( diskspaceProgressBar, tr( "Disk space" ) );
    QToolTip::add( tf_instrumentLineEdit, tr( "The currently selected instrument" ) );
    createsaveresumeGroupBox->setTitle( tr( "Project and global settings" ) );
    QWhatsThis::add( createsaveresumeGroupBox, tr( "<font color=\"#aa0000\" size=\"+1\">Load / reset a Project</font><br>\n"
"For each data reduction Project a log file is created. It remembers the current status of the reduction and configures the GUI accordingly upon relaunch.\n"
"\n"
"<p>You can:<br>\n"
"<b>Load ... :</b> Load the Project from a previous reduction session and continue reducing the data. \n"
"<b>Reset</b> Sets all parameters and fields to default values. <br>\n"
"</p>\n"
"\n"
"If you forget to enter a name for the Project at the beginning of the reduction, everything will be stored in aProject called <tt>noname</tt>, even if this is not immediately visible to you." ) );
    in_loadlogPushButton->setText( tr( "Load ..." ) );
    in_loadlogPushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_loadlogPushButton, tr( "Continue with a previous session" ) );
    in_reductionlogLineEdit->setText( QString::null );
    QToolTip::add( in_reductionlogLineEdit, tr( "The name of the current project" ) );
    in_clearprocstatusPushButton->setText( tr( "Reset" ) );
    QToolTip::add( in_clearprocstatusPushButton, tr( "Deletes all information in the Log file" ) );
    in_homediskwarnSpinBox->setSuffix( tr( " MB" ) );
    QToolTip::add( in_homediskwarnSpinBox, tr( "A warning will be shown if the free space on the disk containing your home directory (and temporary data files) drops below this threshold.<br>" ) );
    in_datadiskwarnSpinBox->setSuffix( tr( " MB" ) );
    QToolTip::add( in_datadiskwarnSpinBox, tr( "A warning will be shown if the free space on the disk where you keep the data drops below this threshold.<br>" ) );
    textLabel1_6_2->setText( tr( "Home diskspace warning:" ) );
    textLabel1_6->setText( tr( "Data diskspace warning:" ) );
    in_nframesTextLabel->setText( tr( "NFRAMES :" ) );
    in_nframesSpinBox->setPrefix( QString::null );
    QToolTip::add( in_nframesSpinBox, tr( "The number of individual chips you can process in memory without swapping. It is determined automatically. If you encounter a slow-down due to swapping, reduce this value.<br>" ) );
    QWhatsThis::add( in_nframesSpinBox, QString::null );
    in_TextLabel3_2->setText( tr( "GUI style:" ) );
    QWhatsThis::add( in_TextLabel3_2, tr( "<p><font color=\"#aa0000\" size=\"+1\">Number of CPUs</font><br>\n"
"There are two kinds of multi-processor machines. On the one side there are those that \n"
"incorporate two or more CPUs in a single computer, such as a dual processor PC or a \n"
"multi-processor SUN workstation. On the other side there are cluster architectures, where the \n"
"CPUs are spread over several independent machines. THELI itself supports full parallelisation \n"
"for both types, however the graphical user interface you are looking at the very moment \n"
"DOES NOT support cluster architectures. This is because parallelisation on the latter (via \n"
"<i>PVM</i>) is very different and much more involved than the one for multi-processor \n"
"machines. The THELI documentation has the details.<br>\n"
"Users of normal multi-processor machines simply select the number of CPUs they want to use. \n"
"Note that only instruments with several CCDs benefit from parallelisation. In the optimal case one \n"
"has as many CPUs as CCDs.\n"
"<br>Changing the number of CPUs in the GUI updates the file <i>progs.ini</i>.</p>" ) );
    tf_styleSpinBox->setPrefix( QString::null );
    QWhatsThis::add( tf_styleSpinBox, tr( "Choose a style that pleases your eye" ) );
    in_numcpuSpinBox->setPrefix( QString::null );
    QToolTip::add( in_numcpuSpinBox, tr( "The number of available processors on your machine." ) );
    QWhatsThis::add( in_numcpuSpinBox, tr( "<p><font color=\"#aa0000\" size=\"+1\">Number of CPUs</font><br>\n"
"There are two kinds of multi-processor machines. On the one side there are those that \n"
"incorporate two or more CPUs in a single computer, such as a dual processor PC or a \n"
"multi-processor SUN workstation. On the other side there are cluster architectures, where the \n"
"CPUs are spread over several independent machines. THELI itself supports full parallelisation \n"
"for both types, however the graphical user interface you are looking at the very moment \n"
"DOES NOT support cluster architectures. This is because parallelisation on the latter (via \n"
"<i>PVM</i>) is very different and much more involved than the one for multi-processor \n"
"machines. The THELI documentation has the details.<br>\n"
"Users of normal multi-processor machines simply select the number of CPUs they want to use. \n"
"Note that only instruments with several CCDs benefit from parallelisation. In the optimal case one \n"
"has as many CPUs as CCDs.\n"
"<br>Changing the number of CPUs in the GUI updates the file <i>progs.ini</i>.</p>" ) );
    in_TextLabel3->setText( tr( "# of CPUs:" ) );
    QWhatsThis::add( in_TextLabel3, tr( "<p><font color=\"#aa0000\" size=\"+1\">Number of CPUs</font><br>\n"
"There are two kinds of multi-processor machines. On the one side there are those that \n"
"incorporate two or more CPUs in a single computer, such as a dual processor PC or a \n"
"multi-processor SUN workstation. On the other side there are cluster architectures, where the \n"
"CPUs are spread over several independent machines. THELI itself supports full parallelisation \n"
"for both types, however the graphical user interface you are looking at the very moment \n"
"DOES NOT support cluster architectures. This is because parallelisation on the latter (via \n"
"<i>PVM</i>) is very different and much more involved than the one for multi-processor \n"
"machines. The THELI documentation has the details.<br>\n"
"Users of normal multi-processor machines simply select the number of CPUs they want to use. \n"
"Note that only instruments with several CCDs benefit from parallelisation. In the optimal case one \n"
"has as many CPUs as CCDs.\n"
"<br>Changing the number of CPUs in the GUI updates the file <i>progs.ini</i>.</p>" ) );
    textLabel2_3->setText( tr( "Project:" ) );
    in_dirsettingGroupBox->setTitle( tr( "Data directories" ) );
    QWhatsThis::add( in_dirsettingGroupBox, tr( "<font color=\"#aa0000\" size=\"+1\">Data directories</font><br>\n"
"Here you let THELI know the main path to your data, and in which subdirectories of it \n"
"you put the BIAS, DARK, FLAT and SCIENCE exposures.\n"
"\n"
"<p> The SKY directory is basically a second SCIENCE \n"
"directory that contains the exposures of an \"empty\" field used for sky background estimation. This is a concept often used in near-IR observations. But it is as useful in the optical, e.g. for the calculation of a SUPERFLAT when very extended objects have been observed. If the SKY directory is present, the data therein is automatically reduced in parallel to the SCIENCE data and - if you wish - used for the calibration of the latter.\n"
"</p>" ) );
    in_biasdirLineEdit->setText( QString::null );
    QToolTip::add( in_biasdirLineEdit, tr( "The BIAS subdirectory" ) );
    in_maindirTextLabel->setText( tr( "Main path:" ) );
    QToolTip::add( in_maindirTextLabel, tr( "The main directory of your data" ) );
    in_darkdirTextLabel->setText( tr( "Dark:" ) );
    QToolTip::add( in_darkdirTextLabel, tr( "The DARK subdirectory" ) );
    in_biasdirTextLabel->setText( tr( "Bias:" ) );
    QToolTip::add( in_biasdirTextLabel, tr( "The BIAS subdirectory" ) );
    in_restoreallPushButton->setText( tr( "Restore ALL" ) );
    in_restoreallPushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_restoreallPushButton, tr( "DELETES all images in these  directories if ORIGINALS directories exist in them.<br>Moves back the images from the ORIGINALS directories." ) );
    in_restoredarkPushButton->setText( tr( "Restore ORIG" ) );
    in_restoredarkPushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_restoredarkPushButton, tr( "DELETES all images in the corresponding directory if an ORIGINALS directory exists.<br>Moves back the images from the ORIGINALS directory." ) );
    in_restorebiasPushButton->setText( tr( "Restore ORIG" ) );
    in_restorebiasPushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_restorebiasPushButton, tr( "DELETES all images in the corresponding directory if an ORIGINALS directory exists.<br>Moves back the images from the ORIGINALS directory." ) );
    in_darkdirLineEdit->setText( QString::null );
    QToolTip::add( in_darkdirLineEdit, tr( "The DARK subdirectory" ) );
    in_maindirLineEdit->setText( QString::null );
    QToolTip::add( in_maindirLineEdit, tr( "The main directory of your data" ) );
    in_flatdirTextLabel_2_2->setText( tr( "Standard:" ) );
    QToolTip::add( in_flatdirTextLabel_2_2, tr( "The subdirectory where you put STANDARD observations (if any)" ) );
    in_flatdirTextLabel_2->setText( tr( "Sky:" ) );
    QToolTip::add( in_flatdirTextLabel_2, tr( "The subdirectory where you put SKY observations (blank field, if any)" ) );
    in_sciencedirTextLabel->setText( tr( "Science:" ) );
    QToolTip::add( in_sciencedirTextLabel, tr( "The subdirectory with your SCIENCE observations" ) );
    in_processskyCheckBox->setText( tr( "Fully process SKY data as well" ) );
    QToolTip::add( in_processskyCheckBox, tr( "Set this switch if you want to process the SKY data as well. Every step you do with the SCIENCE data will then be repeatd for the SKY data, too.<br>" ) );
    in_clearPushButton->setText( tr( "&Clear dirs" ) );
    in_clearPushButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    QToolTip::add( in_clearPushButton, tr( "Clears all data directories entered" ) );
    in_sciencedirLineEdit->setText( QString::null );
    QToolTip::add( in_sciencedirLineEdit, tr( "The subdirectory with your SCIENCE observations" ) );
    in_skydirLineEdit->setText( QString::null );
    QToolTip::add( in_skydirLineEdit, tr( "The subdirectory where you put SKY observations (blank field, if any)" ) );
    in_restoreskyPushButton->setText( tr( "Restore ORIG" ) );
    in_restoreskyPushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_restoreskyPushButton, tr( "DELETES all images in the corresponding directory if an ORIGINALS directory exists.<br>Moves back the images from the ORIGINALS directory." ) );
    in_restoresciencePushButton->setText( tr( "Restore ORIG" ) );
    in_restoresciencePushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_restoresciencePushButton, tr( "DELETES all images in the corresponding directory if an ORIGINALS directory exists.<br>Moves back the images from the ORIGINALS directory." ) );
    in_restorestandardPushButton->setText( tr( "Restore ORIG" ) );
    in_restorestandardPushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_restorestandardPushButton, tr( "DELETES all images in the corresponding directory if an ORIGINALS directory exists.<br>Moves back the images from the ORIGINALS directory." ) );
    in_standarddirLineEdit->setText( QString::null );
    QToolTip::add( in_standarddirLineEdit, tr( "The subdirectory where you put STANDARD observations (if any)" ) );
    in_flatdirTextLabel->setText( tr( "Flat:" ) );
    QToolTip::add( in_flatdirTextLabel, tr( "The FLAT subdirectory" ) );
    in_flatdirLineEdit->setText( QString::null );
    QToolTip::add( in_flatdirLineEdit, tr( "The FLAT subdirectory" ) );
    in_restoreflatPushButton->setText( tr( "Restore ORIG" ) );
    in_restoreflatPushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_restoreflatPushButton, tr( "DELETES all images in the corresponding directory if an ORIGINALS directory exists.<br>Moves back the images from the ORIGINALS directory." ) );
    in_flatdirTextLabel_3->setText( tr( "Flat off:" ) );
    QToolTip::add( in_flatdirTextLabel_3, tr( "The FLAT subdirectory" ) );
    in_flatoffdirLineEdit->setText( QString::null );
    QToolTip::add( in_flatoffdirLineEdit, tr( "The FLAT OFF subdirectory (\"dark\" flats for near-IR data)" ) );
    in_restoreflatoffPushButton->setText( tr( "Restore ORIG" ) );
    in_restoreflatoffPushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( in_restoreflatoffPushButton, tr( "DELETES all images in the corresponding directory if an ORIGINALS directory exists.<br>Moves back the images from the ORIGINALS directory." ) );
    mainmenu_TabWidget->changeTab( PageInitialise, tr( "Initialise" ) );
    groupBox18->setTitle( tr( "Tasks" ) );
    QWhatsThis::add( groupBox18, tr( "<p><font color=\"#aa0000\" size=\"+1\">Sorting, splitting, linking</font><br>\n"
"In this processing group the data is prepared to be run through the THELI pipeline.<br>\n"
"<b>Sort data using FITS key:</b> If your images have a FITS keyword that identifies the image as a BIAS, FLAT etc, you can use this key to automatically sort the data.<br>\n"
"<b>Split FITS / correct header:</b> Splits multi-extension FITS files (usually from multi-chip cameras) into individual FITS files. At the same time a new header is created, so that the image is suitable to be run through the THELI pipeline. This script must also be applied to single-chip cameras.<br>\n"
"<b>Create links:</b> If you work on a multi-processor machine, you can spread your data over different hard disks and thus take full advantage of the processing. A link structure is created that makes the data appear to be in one directory on one disk, but actually it is spread to different ones.\n"
"</p>\n"
"<p>Right click each of the tasks to get more detailed information on what is going on.</p>" ) );
    pre_TextLabel12_2->setText( tr( "Chips going to scratch:" ) );
    pre_TextLabel12_2_2->setText( tr( "Scratch directory:" ) );
    pre_linkchipsLineEdit->setText( QString::null );
    QToolTip::add( pre_linkchipsLineEdit, tr( "Blank-separated list of the chips you want to move. For example:  1 3 5 7" ) );
    QToolTip::add( pre_scratchdirLineEdit, tr( "Full path to the scratch directory where you want to have the chips." ) );
    pre_splitCheckBox->setText( tr( "Split FITS / correct header" ) );
    pre_splitCheckBox->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( pre_splitCheckBox, tr( "Splits multi-extension FITS files, and writes the THELI standard FITS header" ) );
    QWhatsThis::add( pre_splitCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Split FITS / correct header</b></font></p>\n"
"\n"
"<p><b>Status: </b><font color=\"#ff0000\"><b>MANDATORY</b></font></p>\n"
"\n"
"<p><b>User action required: </b><font color=\"#aa0000\"><br>\n"
"-- Configure the dialog (optional)</font>\n"
"</p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"FITS images come in a huge variety. THELI, however, can only handle images with a certain format and certain headers. Therefore, the script that is invoked  by this task is mostly instrument dependent. It splits multi-extension FITS files into the single extensions, changing the ending of their file names into <i>_n.fits</i>, where <i>n</i> is the number of the extension.<br>\n"
"In addition, an entirely new FITS header is written that conforms with our standards.<br>\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"The script reads in FITS data and splits it into the individual extensions. If no extensions are found, the data is left as is, but gets a new file name from which it is clear that a single-chip camera was used. If necessary, images are flipped/rotated such that North is up and East is left. This assumes of course that the orientation of the camera is always the same, which is the case for many instruments, but not for all.<br>\n"
"A new header is written to the FITS file that conforms to our standards. In particular for the astrometric and photometric parts of THELI it is essential that the correct header keywords are found.\n"
"</p>\n"
"\n"
"<p><b>ATTENTION:</b><br>\n"
"Every time you define a new instrument, you have to create a corresponding splitting script and test it thoroughly. In principle it is enough if you select a similar instrument from the list and see how the corresponding script works. Copy it and adjust it to your needs. The script you create must have the following name so that it can be uniquely linked to your instrument:<br>\n"
"process_split_YOURINSTRUMENT_eclipse.sh<br>\n"
"where YOURINSTRUMENT is precisely the same name under which your instrument appears in the <b><font color=\"#0000ff\">Instrument selection list box</font></b> in the <b><font color=\"#0000ff\">Initialise</font></b> processing group.</p>" ) );
    pre_sortdataCheckBox->setText( tr( "Sort data using FITS key" ) );
    pre_sortdataCheckBox->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( pre_sortdataCheckBox, tr( "Sorts data based on the identifier given in this FITS keyword" ) );
    QWhatsThis::add( pre_sortdataCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Sort data using FITS key</b></font></p>\n"
"\n"
"<p><b>Status: </b><font color=\"#00aa00\"><b>OPTIONAL</b></font></p>\n"
"\n"
"<p><b>User action required: </b><font color=\"#aa0000\"><br>\n"
"-- Specify the FITS keyword that identifies the exposure type, and its according values for BIASes, DARKs, DOMEFLATs, SKYFLATs and STANDARD exposures</font>\n"
"</p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"At the beginning of the data reduction all sorts of exposures are often mixed together. They must be separated into BIASes, FLATs, SCIENCE exposures etc before the reduction can start. If the images have a header keyword that specifies the exposure type, then you can use this task to automatically disentangle them. \n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"The script reads data present in the <font color=\"#0000ff\"><b>Main data directory</b></font> (as specified in the <font color=\"#0000ff\"><b>Initialise</b></font> processing group) and creates corresponding sub-directories for the various data types found. The data is redistributed to these directories. All remaining images go to the SCIENCE directory (and require further manual splitting).</p>\n"
"\n"
"<p><b>ATTENTION:</b><br>\n"
"You can not trust the outcome of this script since FITS headers are not necessarily reliable. Always check manually that the data ended up in the correct place. This script does not discriminate between different filters. If you have different filters mixed, you have to recollect them manually, and put them e.g. into FLAT_B, FLAT_V, and FLAT_R.</p>" ) );
    pre_createlinksCheckBox->setText( tr( "Create links" ) );
    pre_createlinksCheckBox->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( pre_createlinksCheckBox, tr( "Distributes the data over several hard disks" ) );
    QWhatsThis::add( pre_createlinksCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Create links</b></font></p>\n"
"\n"
"<p><b>Status: </b><font color=\"#00aa00\"><b>OPTIONAL</b></font></p>\n"
"\n"
"<p><b>User action required: </b><font color=\"#aa0000\"><br>\n"
"-- Specify the scratch directory<br>\n"
"-- Specify the chips you want to link</font>\n"
"</p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"This is only interesting for you if you work with multi-chip cameras on a \n"
"multi-processor computer. Users of single chip cameras can ignore all this.<br>\n"
"In order to take full advantage of parallel processing, the data of a multi-chip camera \n"
"should reside on different hard disks. In the ideal case we have as many hard disks \n"
"and processors as we have CCDs in the camera. The data on the different hard disks \n"
"is linked to the main directory, though, so the data appears as if everything resides in \n"
"the same place. If all this is the case, then with a computer having n processors and \n"
"disks you can reduce the data of a multi-chip camera in the 1/n-th time.</p>\n"
"<p>\n"
"The way you redistribute your data depends on the number of CCDs and of the \n"
"number of CPUs you have. For example, you worked with an 8-chip camera, and want \n"
"to process the data on a 2 processor machine. Then you specify under \n"
"<b><font color=\"#0000ff\">Chips that go to scratch</font></b> the blank separated list of the CCDs,<br>\n"
"<i>1 3 5 7</i>, and under \n"
"<b><font color=\"#0000ff\">Scratch directory</font></b> the full path to where you \n"
"want the data, e.g. <i><font color=\"#00a700\">\n"
"/scratchdisk1/user/data/</font></i>. Once finished, you repeat this step with<br>\n"
"<i>2 4 6 8</i> and <i><font color=\"#00a700\">/scratchdisk2/user/data/</font></i>.\n"
"<br>\n"
"On a 4-processor machine it would look like that:<br>\n"
"<i>1 5</i> and <i><font color=\"#00a700\">/scratchdisk1/...</font></i><br>\n"
"<i>2 6</i> and <i><font color=\"#00a700\">/scratchdisk2/...</font></i><br>\n"
"<i>3 7</i> and <i><font color=\"#00a700\">/scratchdisk3/...</font></i><br>\n"
"<i>4 8</i> and <i><font color=\"#00a700\">/scratchdisk4/...</font></i><br>\n"
"Thus, processor 1 works on chips 1 and then 5, processor 2 works on chips 2 and then \n"
"6 etc. <br>\n"
"You have to stick to this scheme since this is the way the tasks are distributed to the \n"
"various CPUs by the parallel manager script used.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"The script moves the chips you specified to a scratch direcory you specified, and \n"
"creates links back to the position where the images have been. If your data is already \n"
"split into various directories, such as BIAS, FLAT, SCIENCE, you have to repeat this \n"
"script for each of them.\n"
"</p>\n"
"\n"
"<p><b>ATTENTION:</b><br>\n"
"You must add your scratch directories to the <i>progs.ini</i> file, otherwise THELI \n"
"doesn not know where the data is.\n"
"</p>" ) );
    pre_splitMIRCheckBox->setText( tr( "Split MIR cubes" ) );
    QToolTip::add( pre_splitMIRCheckBox, tr( "Whether the MIR cubes should be split into individual chop-nod corrected exposures, or whether the entire stacked cube should be kept.<br>" ) );
    pre_TextLabel1->setText( tr( "Data are in the main directory." ) );
    pre_TextLabel13->setText( tr( "Data are in the individual directories.\n"
"Only raw data are allowed in the directories for the splitting." ) );
    pre_commandGroupBox->setTitle( tr( "Commands" ) );
    pre_killPushButton->setText( tr( "&Abort" ) );
    pre_killPushButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    QToolTip::add( pre_killPushButton, tr( "Be careful with this button!<br>You will have to manually clean the directory from left over stuff, and put the data back into its previous position!" ) );
    pre_configurePushButton->setText( tr( "Configure" ) );
    pre_configurePushButton->setAccel( QKeySequence( QString::null ) );
    pre_clearscriptsPushButton->setText( tr( "Clea&r scripts" ) );
    pre_clearscriptsPushButton->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    pre_startPushButton->setText( tr( "&Start" ) );
    pre_startPushButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    pre_messagesGroupBox->setTitle( tr( "Messages" ) );
    pre_messagesTextEdit->setText( QString::null );
    mainmenu_TabWidget->changeTab( PageCalibration, tr( "Preparation" ) );
    cal_commandGroupBox->setTitle( tr( "Commands" ) );
    cal_configurePushButton->setText( tr( "Configure" ) );
    cal_configurePushButton->setAccel( QKeySequence( QString::null ) );
    cal_clearscriptsPushButton->setText( tr( "Clea&r scripts" ) );
    cal_clearscriptsPushButton->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    cal_killPushButton->setText( tr( "&Abort" ) );
    cal_killPushButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    QToolTip::add( cal_killPushButton, tr( "Be careful with this button!<br>You will have to manually clean the directory from left over stuff, and put the data back into its previous position!" ) );
    cal_startPushButton->setText( tr( "&Start" ) );
    cal_startPushButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    cal_resetfieldsPushButton->setText( tr( "&Defaults" ) );
    cal_resetfieldsPushButton->setAccel( QKeySequence( tr( "Alt+D" ) ) );
    cal_messagesGroupBox->setTitle( tr( "Messages" ) );
    cal_GroupBox->setTitle( tr( "Tasks" ) );
    QWhatsThis::add( cal_GroupBox, tr( "<p><font color=\"#aa0000\" size=\"+1\">Debiasing, flatfielding, creation of superflat</font><br>\n"
"Here we do the basic pre-processing of the data. An overscan correction is done as well if overscan regions are specified in the corresponding instrument configuration file.<br>\n"
"<b>Process biases:</b> Calculates a master BIAS from a set of BIAS exposures.<br>\n"
"<b>Process darks:</b> Calculates a master DARK from a set of DARK exposures.<br>\n"
"<b>Process flats:</b> Calculates a master FLAT from a set of BIAS and FLAT \n"
"exposures.<br>\n"
"<b>Spread sequence (IR):</b> IR detectors often take a while until they settled into equilibrium. Say, you make 10 images at a given dither position and then move to another dither position, the detector has to settle into its equilibrium again. This can take between one and five exposures in the sequence. These images have certain characteristics in the background, which are the same for the <i>n</i>-th exposures in the <i>k</i>-th sequence. Thus, up to the defringing step they have to be processed separately from the others. This script handles the splitting.<br>\n"
"<b>Calibrate data:</b> The SCIENCE data itself is corrected with the master BIAS and FLAT frames. Cameras having a significant dark current need to be corrected with the master DARK instead of the master BIAS. If SKY (i.e. blank sky fields) are present too, then these are reduced as well with the same settings as the SCIENCE data. For near-IR data you might not want to apply a BIAS to the FLAT or SCIENCE data, for mid-IR even flat fields might not be very useful (mark the corresponding CheckBoxes).\n"
"</p>\n"
"<p>Right click each of the tasks to get more detailed information on what is going on.</p>" ) );
    cal_darkinsteadbiasCheckBox->setText( tr( "Use DARK" ) );
    QToolTip::add( cal_darkinsteadbiasCheckBox, tr( "Use a DARK instead of the BIAS" ) );
    cal_procscienceCheckBox->setText( tr( "Calibrate data" ) );
    QToolTip::add( cal_procscienceCheckBox, tr( "Applies the master bias and master flat to the data, and optionally calculates a superflat<br>" ) );
    QWhatsThis::add( cal_procscienceCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Calibrate data</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify if you want to calculate a SUPERFLAT from this data<br>\n"
"-- Specify if you want to correct with a DARK instead of a BIAS<br>\n"
"-- Provide detection thresholds for the masking of the objects for the optional    SUPERFLAT calculation, together with the background smoothing scale,  combination method, and window size.</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Debiases and flatfields the data, and optionally creates a SUPERFLAT. If an SKY directory is present, it will be reduced, too. From the SKY data a SUPERFLAT will always be calculated, even if you uncheck the according CheckBox.<br>\n"
"For the calculation of the SUPERFLAT one must adjust various parameters, depending on the type of target, the dither strategy, and the filter used. Refer to the THELI documentation for the extensive details. If you know that instead<br> \n"
"-- your FLAT works very well<br>\n"
"-- you do not suffer from fringing<br> \n"
"-- you do not gain anything by the application of a SUPERFLAT<br>\n"
"then you can ignore all that follows and jump immediately to the \n"
"<font color=\"#0000ff\"><b>Weighting</b> </font>processing group.<br>\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"All images are overscan corrected (if specified in the instrument configuration), debiased and flat-fielded (if requested). The overscan region is removed. Gain differences of multi-chip cameras are adjusted based on the FLAT.<br>\n"
"Optionally, a SUPERFLAT is calculated. To this end all objects above the specified detection thresholds are masked. For images without fringing you could use values (1.0 | 20) for (DT | DMIN), and for images with strong fringing values (7.0 | 5.0) are better suited. If the detection thresholds are left empty, no object masking takes place.<br>\n"
"The masked images are then rescaled to the same mode, median or mean combined using outlier rejection, and then moved to a MASK_IMAGES folder.<br>\n"
"If the <b><font color=\"#0000ff\">Window size</font></b> parameter has been set to zero (or left empty), a static SUPERFLAT is calculated from all images in the directory. If that parameter is set to a non-zero value <i>n</i>, a dynamic SUPERFLAT is calculated from the <i>n</i> exposures around the current exposure, and applied right away.<br>\n"
"A normalised copy of the SUPERFLAT is stored separately in a SCIENCE_norm directory. It can provide additional bad pixel contraints during the creation of the WEIGHTs later on.<br>\n"
"Once an image went through this task, it gets a <i><font color=\"#00aa00\">file name extension</font></i> assigned from which the user can see through which processing steps it already went. In this case the file name extension is usually <i>OFC</i>, which indicates that the image was overscan corrected and flatfielded.<br>\n"
"File name example: <i>WFI123_1OFC.fits</i>\n"
"</p>" ) );
    cal_scienceminLineEdit->setText( QString::null );
    QToolTip::add( cal_scienceminLineEdit, tr( "Exposures with a mode lower than this value are rejected.<br>" ) );
    cal_TextLabel3->setText( tr( "max" ) );
    QToolTip::add( cal_TextLabel3, tr( "Flat exposures with a mode higher than this value are rejected from the calculation of the master FLAT.<br>" ) );
    cal_sciencemaxLineEdit->setText( QString::null );
    QToolTip::add( cal_sciencemaxLineEdit, tr( "Exposures with a mode higher than this value are rejected.<br>" ) );
    cal_procbiasCheckBox->setText( tr( "Process biases" ) );
    QToolTip::add( cal_procbiasCheckBox, tr( "Creates a master bias from a series of bias exposures" ) );
    QWhatsThis::add( cal_procbiasCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Process biases / darks</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"> None</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Combines all BIASes in the <b><font color=\"#0000ff\">BIAS</font> </b>directory set in the <b><font color=\"#0000ff\">Initialise</font></b> group. You can override this by entering a different subdirectory. The more BIASes you have, the better the master BIAS will be and the smaller the calibration noise you introduce in your SCIENCE images. If a DARK directory was provided as well, a master DARK will be created in the same way, too. We recommend at least 10 BIAS or DARK exposures.</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"All BIAS frames found in the <b><font color=\"#0000ff\">BIAS</font> </b>directory are overscan corrected (if specified in the instrument configuration) and median combined using outlier rejection. The resulting master BIAS adopts the name of the directory it resides in. Overscan regions are trimmed from the images.\n"
"</p>" ) );
    cal_biasminLineEdit->setText( QString::null );
    QToolTip::add( cal_biasminLineEdit, tr( "Bias exposures with a mode lower than this value are rejected from the calculation of the master BIAS.<br>" ) );
    cal_calibredoPushButton->setText( QString::null );
    QToolTip::add( cal_calibredoPushButton, tr( "Click here before repeating this task." ) );
    cal_darkminLineEdit->setText( QString::null );
    QToolTip::add( cal_darkminLineEdit, tr( "Dark exposures with a mode lower than this value are rejected from the calculation of the master DARK.<br>" ) );
    cal_TextLabel2->setText( tr( "min" ) );
    cal_flatmaxLineEdit->setText( QString::null );
    QToolTip::add( cal_flatmaxLineEdit, tr( "Flat exposures with a mode higher than this value are rejected from the calculation of the master FLAT.<br>" ) );
    cal_procdarkCheckBox->setText( tr( "Process darks" ) );
    QToolTip::add( cal_procdarkCheckBox, tr( "Creates a master dark from a series of dark exposures" ) );
    QWhatsThis::add( cal_procdarkCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Process biases / darks</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"> None</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Combines all BIASes in the <b><font color=\"#0000ff\">BIAS</font> </b>directory set in the <b><font color=\"#0000ff\">Initialise</font></b> group. You can override this by entering a different subdirectory. The more BIASes you have, the better the master BIAS will be and the smaller the calibration noise you introduce in your SCIENCE images. If a DARK directory was provided as well, a master DARK will be created in the same way, too. We recommend at least 10 BIAS or DARK exposures.</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"All BIAS frames found in the <b><font color=\"#0000ff\">BIAS</font> </b>directory are overscan corrected (if specified in the instrument configuration) and median combined using outlier rejection. The resulting master BIAS adopts the name of the directory it resides in. Overscan regions are trimmed from the images.\n"
"</p>" ) );
    cal_flatminLineEdit->setText( QString::null );
    QToolTip::add( cal_flatminLineEdit, tr( "Flat exposures with a mode lower than this value are rejected from the calculation of the master FLAT.<br>" ) );
    cal_procflatCheckBox->setText( tr( "Process flats" ) );
    QToolTip::add( cal_procflatCheckBox, tr( "Creates a master flat from a series of flat field exposures" ) );
    QWhatsThis::add( cal_procflatCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Process flats</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify a minimum and maximum threshold for the individual FLAT exposures (optional)</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Combines all FLATs in the <b><font color=\"#0000ff\">FLAT</font> </b>directory set in the <b><font color=\"#0000ff\">Initialise</font></b> group. You can override this by entering a different subdirectory.<br>\n"
"A FLAT correction is very useful even if the camera appears to be illuminated very homogeneously, since the FLAT corrects also for pixel response non-uniformity. The more FLATs you have, the better the master FLAT will be and the smaller the calibration noise you introduce in your SCIENCE images. We recommend at least 5 FLAT exposures.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"All FLAT frames found in the <b><font color=\"#0000ff\">FLAT</font> </b>directory are filtered with respect to the optionally given thresholds. Outliers are moved to a BADMODE directory within the FLAT directory. Remaining exposures are overscan corrected (if specified in the instrument configuration) and have the overscan trimmed away. \n"
"Thereafter, the master BIAS is subtracted, unless the <b><font color=\"#0000ff\">o not apply BIAS / DARK</font></b> CheckBox has been activated. Subsequently, the FLAT exposures are rescaled to the highest mode in the stack. It follows a median combination using outlier rejection. The resulting master FLAT adopts the same name as its directory.\n"
"<br>\n"
"The master FLAT gets normalised so that its mode is equal to 1. The normalised FLAT is saved in a FLAT_norm directory under the main path. It is used later on for the creation of the WEIGHTs.\n"
"</p>" ) );
    cal_biasmaxLineEdit->setText( QString::null );
    QToolTip::add( cal_biasmaxLineEdit, tr( "Bias exposures with a mode higher than this value are rejected from the calculation of the master BIAS.<br>" ) );
    cal_darkmaxLineEdit->setText( QString::null );
    QToolTip::add( cal_darkmaxLineEdit, tr( "Dark exposures with a mode higher than this value are rejected from the calculation of the master DARK.<br>" ) );
    cal_nobiasCheckBox->setText( tr( "Do not apply BIAS / DARK" ) );
    QToolTip::add( cal_nobiasCheckBox, tr( "Mark this CheckBox if you do not want a BIAS / DARK subtracted from your FLAT / SCIENCE, or if you don't have a BIAS / DARK." ) );
    cal_noflatCheckBox->setText( tr( "Do not apply FLAT" ) );
    QToolTip::add( cal_noflatCheckBox, tr( "Mark this CheckBox if you do not want a FLAT applied to your SCIENCE data, or if you don't have a FLAT." ) );
    cal_nonlinCheckBox->setText( tr( "Correct for non-linearity" ) );
    QToolTip::add( cal_nonlinCheckBox, tr( "Mark this CheckBox if you want to correct for non-linearity in your data. This requires that the non-linearity coefficients have been measured and provided in a separate configuration file. <br>" ) );
    mainmenu_TabWidget->changeTab( TabPage, tr( "Calibration" ) );
    sf_commandGroupBox->setTitle( tr( "Commands" ) );
    sf_startPushButton->setText( tr( "&Start" ) );
    sf_startPushButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    sf_clearscriptsPushButton->setText( tr( "Clea&r scripts" ) );
    sf_clearscriptsPushButton->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    sf_resetsfieldsPushButton->setText( tr( "&Defaults" ) );
    sf_resetsfieldsPushButton->setAccel( QKeySequence( tr( "Alt+D" ) ) );
    sf_killPushButton->setText( tr( "&Abort" ) );
    sf_killPushButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    QToolTip::add( sf_killPushButton, tr( "Be careful with this button!<br>You will have to manually clean the directory from left over stuff, and put the data back into its previous position!" ) );
    sf_messagesGroupBox->setTitle( tr( "Messages" ) );
    sf_ButtonGroup->setTitle( tr( "Tasks" ) );
    QWhatsThis::add( sf_ButtonGroup, tr( "<p><font color=\"#aa0000\" size=\"+1\">Background</font><br><br>\n"
"All scripts herein are OPTIONAL. If SKY observations are present, they will be reduced automatically in the same way as the SCIENCE data.<br>\n"
"<b>Spread sequence (IR):</b> IR detectors often take a while until they settled into equilibrium. Say, you make 10 images at a given dither position and then move to another dither position, then detector has to settle into its equilibrium again. This can take between one and five exposures in the sequence. These images have certain characteristics in the background, which are the same for the <i>n</i>-th exposures in the <i>k</i>-th sequence. Thus, they have to be processed separately from the others. This script handles the splitting.<br>\n"
"<b>Background model correction:</b> This is a complex task that can be configured very specifically. Essentially, it allows you to calculate various types of background models and apply them to the data in different manners.<br>\n"
"<b>Merge sequence (IR):</b> Moves the images that were spread by <b>\n"
"<font color=\"#0000ff\">Spread sequence</font></b> back to the SCIENCE (SKY) directory. <br>\n"
"<b>Chop/Nod sky subtraction:</b> If you observed in the mid-IR, you usually have to subtract two subsequent images from each other in order to get rid of the sky background. It is assumed that you have on-target and off-target images in the same directory.<br>\n"
"<b>Collapse correction:</b> Detectors with multiple readout ports (mostly IR, but also in the optical) sometimes show jumps across the different quadrant borders. You can collapse an image along the x- or y-direction, then expand it again and subtract it from the original to correct for this. <br>\n"
"</p>\n"
"<p>Right click each of the tasks to get more detailed information on what is going on.</p>" ) );
    sf_backgroundconfigurePushButton->setText( tr( "Configure" ) );
    sf_backgroundredoPushButton->setText( QString::null );
    QToolTip::add( sf_backgroundredoPushButton, tr( "Click here before repeating this task." ) );
    sf_collapseredoPushButton->setText( QString::null );
    QToolTip::add( sf_collapseredoPushButton, tr( "Click here before repeating this task." ) );
    sf_collconfigurePushButton->setText( tr( "Configure" ) );
    sf_collapseCheckBox->setText( tr( "Collapse correction" ) );
    QToolTip::add( sf_collapseCheckBox, tr( "Does a collapse correction to remove horizontal or vertical gradients" ) );
    QWhatsThis::add( sf_collapseCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Collapse correction</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify the collapse direction and the two SExtractor detection thresholds.<br>\n"
"-- Optionally, a region containing a large object can be excluded when creating the collapse model.</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Some instruments, in particular IR detectors, can still show horizontal or vertical gradients after full flat-fielding and sky subtraction. Usually, these gradients span the entire image.<br>\n"
"A simple correction for this is to collapse the image parallel to the brighter (or darker) region, i.e. to calculate  an average profile of the sky background perpendicular to it. This profile is then expanded again to the original image size and subtracted, thus removing the gradient and most of the sky background.<br>\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"All objects in the  exposure are detected and removed. The image is then collapsed along the specified direction, using an averaging process including outlier rejection. The profile obtained is then expanded in the same direction to the original size of the image and subtracted from it.<br>\n"
"Images have another <i>C</i> appended to their file name extension, i.e. \n"
"<i>OFCC</i>.\n"
"<br>File name example: <i>WFI123_1OFCC.fits</i>\n"
"</p>" ) );
    sf_chopnodredoPushButton->setText( QString::null );
    QToolTip::add( sf_chopnodredoPushButton, tr( "Click here before repeating this task." ) );
    sf_patternTextLabel->setText( tr( "Pattern:" ) );
    sf_chopnodpatternComboBox->clear();
    sf_chopnodpatternComboBox->insertItem( tr( "0110" ) );
    sf_chopnodpatternComboBox->insertItem( tr( "1001" ) );
    sf_chopnodpatternComboBox->insertItem( tr( "0101" ) );
    sf_chopnodpatternComboBox->insertItem( tr( "1010" ) );
    QToolTip::add( sf_chopnodpatternComboBox, tr( "Select the Chop-Nod pattern (0: sky, 1: ontarget)" ) );
    sf_chopnodinvertCheckBox->setText( tr( "Invert" ) );
    QToolTip::add( sf_chopnodinvertCheckBox, tr( "Activate this CheckBox if you want the pattern inverted for every second group, i.e. 0110-1001-0110-1001<br>" ) );
    sf_spreadsequenceCheckBox->setText( tr( "Spread sequence (NIR)" ) );
    QToolTip::add( sf_spreadsequenceCheckBox, tr( "Spreads a sequence of IR exposures in a certain way" ) );
    QWhatsThis::add( sf_spreadsequenceCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Spread sequence (IR)</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify the length of the sequence (number of exposures per dither point)<br>\n"
"-- Specify the number of groups into which you want to split the sequence</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"IR detectors often take a while until they settled into equilibrium. Say, you make 10 \n"
"images at a given dither position and then move to another dither position, then the \n"
"detector has to settle into equilibrium again. This can take between one and five \n"
"exposures in the sequence, depending on various factors. These images have certain \n"
"characteristics in the background, which are the same for the <i>n</i>-th exposures in the <i>k</i>-th sequence. Thus, up to the defringing step they have to be processed \n"
"separately from the others. This script handles the splitting.<br>\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"Example: You have 12 images per dither point, and you know that the detector settled \n"
"into its equilibrium at the third exposure, then you choose \"3\" for the number of \n"
"groups. The script then creates three directories, SCIENCE_S1, SCIENCE_S2 and \n"
"SCIENCE_S3, all on the same level as the SCIENCE directory. The first image of every \n"
"sequence (i.e. numbers 1, 13, 25, etc) are then moved into SCIENCE_S1. The second \n"
"images of the series (2, 14, 26, etc) go to SCIENCE_S2, and the rest to SCIENCE_S3.<br>You then have to process these directories separately up to the defringing step. After \n"
"that you merge the images again in SCIENCE with <b><font color=\"#0000ff\">Merge \n"
"sequence (IR)</font></b> .<br>\n"
"The same happens automatically for an SKY field.\n"
"</p>\n"
"\n"
"<p><b>ATTENTION:</b><br>\n"
"It is important that you do not mix sequences with different lengths in the SCIENCE \n"
"directory. We assume that only COMPLETE sequences are in the SCIENCE directory." ) );
    sf_mergesequenceCheckBox->setText( tr( "Merge sequence (NIR)" ) );
    QToolTip::add( sf_mergesequenceCheckBox, tr( "Merges  a sequence of previously spread IR exposures" ) );
    QWhatsThis::add( sf_mergesequenceCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Merge sequence (IR)</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify the number of groups</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"This script moves back the images that were spread previously using <b>\n"
"<font color=\"#0000ff\">Spread sequence</font></b> in the <b>\n"
"<font color=\"#0000ff\">Calibration</font></b> processing group. The number of groups for both tasks must be the same.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"Moves back the images to the SCIENCE (SKY) directory for further processing.\n"
"</p>" ) );
    sf_spread1TextLabel->setText( tr( "# groups" ) );
    sf_spread2TextLabel->setText( tr( "Length" ) );
    sf_spread_numbergroupsLineEdit->setText( tr( "3" ) );
    QToolTip::add( sf_spread_numbergroupsLineEdit, tr( "Number of groups into which you want to split a sequence" ) );
    sf_spread_lengthsequenceLineEdit->setText( tr( "12" ) );
    QToolTip::add( sf_spread_lengthsequenceLineEdit, tr( "Number of exposures in a sequence" ) );
    sf_chopnodCheckBox->setText( tr( "Chop/Nod skysub (MIR)" ) );
    QToolTip::add( sf_chopnodCheckBox, tr( "Does a chop-nod sky subtraction for mid-IR data" ) );
    QWhatsThis::add( sf_chopnodCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Chop/Nod sky subtraction</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br> \n"
"-- Choose a chop-nod pattern. \"0\" means off-object, \"1\" means on-object<br>\n"
"-- If necessary, you can invert the pattern for every second group, i.e. for example 0110-1001-0110-1001 etc<br>\n"
"</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"This script does a pairwise subtraction of subsequent images to remove the sky background. It is assumed that images on-target and off-target are mixed in a repeated certain manner in the same directory. If for example the pattern \"0110\" is selected, then the first image is subtracted from the second one, and the fourth image from the third one. Such a sequence is typically the result of repeated chopped-nodded exposures. This technique is most often used for mid-IR observations.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"Subsequent images are subtracted from each other in the manner specified by the user.<br>\n"
"Images have a <i>H</i> appended to their file name extension, i.e. <i>OFCH</i>.\n"
"<br>File name example: <i>WFI123_1OFCH.fits</i>\n"
"</p>" ) );
    sf_backgroundCheckBox->setText( tr( "Background model correction" ) );
    QToolTip::add( sf_backgroundCheckBox, tr( "Applies a background correction (e.g. superflat, fringe model, NIR sky)" ) );
    QWhatsThis::add( sf_backgroundCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Background model correction</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"This is a very complex task that has to be configured properly according to your needs. It will either superflat your data, subtract a fringe model, or do a 1- or 2-pass near-IR background subtraction.\n"
"\n"
"Please see the online user manual and Schirmer 2013 for more details." ) );
    mainmenu_TabWidget->changeTab( PagePrereducion, tr( "Background" ) );
    w_clearscriptsPushButton->setText( tr( "Clea&r scripts" ) );
    w_clearscriptsPushButton->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    w_resetfieldsPushButton->setText( tr( "&Defaults" ) );
    w_resetfieldsPushButton->setAccel( QKeySequence( tr( "Alt+D" ) ) );
    w_startPushButton->setText( tr( "&Start" ) );
    w_startPushButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    w_killPushButton->setText( tr( "&Abort" ) );
    w_killPushButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    QToolTip::add( w_killPushButton, tr( "Be careful with this button!<br>You will have to manually clean the directory from left over stuff, and put the data back into its previous position!" ) );
    w_messagesGroupBox->setTitle( tr( "Messages" ) );
    w_commandGroupBox->setTitle( tr( "Commands" ) );
    w_weightButtonGroup->setTitle( tr( "Tasks" ) );
    QWhatsThis::add( w_weightButtonGroup, tr( "<p><font color=\"#aa0000\" size=\"+1\">Weighting and cosmetics</font><br>\n"
"The main tasks in this processing group deal with the creation of WEIGHT images for each exposure.<br>\n"
"<p>\n"
"<b>Debloom images:</b> Before the WEIGHT creation takes place, you have the opportunity to clean the images from blooming spikes. Be aware that this is a purely cosmetic task. You should not apply it if you want to analyse your images in a scientific way afterwards, since a lot of cosmetics is happening around very bright sources.<br>\n"
"<b>Create binned preview:</b> Images are binned and also converted to TIFF format for easier eyeballing. Mosaic cameras have their chips grouped together as well such that the entire exposure can be seen. Use DS9 for masking of satellite tracks and other unwanted features (see the THELI documentation of how to do this).<br>\n"
"<b>Create global weights:</b> This task takes a normalised FLAT as the basis for the WEIGHTs. Various ways of bad pixel detection are offered.<br>\n"
"<b>Create WEIGHTs:</b> Detects bad pixels and cosmics in the individul exposures. It also reads DS9 masks and creates an individual WEIGHT for each exposure.\n"
"<b>Distribute target sets:</b> If you imaged different targets during your observing run, then you can automatically split the data with this task into the according sets (if the FITS headers carry astrometric information). Otherwise you have to do it manually, i.e. create different SCIENCE directories (SCIENCE_SET_1, SCIENCE_SET_2, ...) and move the data there.\n"
"</p>\n"
"<p>Right click each of the tasks to get more detailed information on what is going on.</p>" ) );
    w_configure3PushButton->setText( tr( "Configure" ) );
    w_configure3PushButton->setAccel( QKeySequence( QString::null ) );
    w_createbinmosCheckBox->setText( tr( "Create binned preview" ) );
    QToolTip::add( w_createbinmosCheckBox, tr( "Creates a binned overview image for each exposure of a multi-chip camera, and a TIFF image." ) );
    QWhatsThis::add( w_createbinmosCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Create binned preview</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\">None</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Eyeballing multi-chip camera data is easiest if one can see all CCDs of an exposure at the same time in correct order. This task creates such overview images from multi-chip data. If you define a new multi-chip instrument not included in the present release of THELI, then you have to write your own according configuration file for the mosaicing. See the documentation of how to do this.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"The script reads a small configuration file that tells it the relative positioning of the CCDs of a multi-chip camera with respect to each other. The images are then shrunk by a specified binning factor in the script and pasted together into a single FITS file. The same happens for single chip cameras. All images are also converted to TIFF.\n"
"These <i>binned mosaics</i> are stored in BINNED_[FITS | TIFF] directories within the SCIENCE directory.\n"
"</p>" ) );
    w_distsetCheckBox->setText( tr( "Distribute target sets" ) );
    QToolTip::add( w_distsetCheckBox, tr( "Identifies image associations on the sky. The SCIENCE directory field in the Initialise section will then point to the first association found, SCIENCE_set_1.<br>" ) );
    QWhatsThis::add( w_distsetCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Distribute target sets</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify a minimum overlap area</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Possibly you have observed several targets at different positions in the sky. For the astrometric and photometric calibration we must split the images in the SCIENCE (and, if applicable, the SKY directory) into various sets containing only images of a particular target. Since the file names of your images do not necessarily reflect the target itself, this script tries to identify image associations based on the astrometric information in the FITS header. Images with the minimum overlap specified are associated and moved to a dedicated directory. <br>\n"
"If your images don't yet have astrometric information, you have to distribute them manually into new directories at the same level as the SCIENCE directory and change the corresponding entry in the Initialise section accordingly. The names of these directories are irrelevant.<br>\n"
"If you have only a single target anyway in your SCIENCE directory (and possibly a single SKY field in the SKY directory), then you can leave the data where it is. The SKY field will again be processed automatically together with the SCIENCE.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"The script reads the CRVAL1/2 keywords as well as the pixel scale of your images. Based on the minimum overlap given, it decides whether two or more images belong to the same target or not. For each association found a new directory <i>SCIENCE_set_n</i> is created, where <i>n</i> counts the associations. The SCIENCE_set_n directories are on the same level as the SCIENCE directory, i.e. right below the <b><font color=\"#0000ff\">Main data directory</font></b>.\n"
"</p>" ) );
    w_createweightsCheckBox->setText( tr( "Create WEIGHTs" ) );
    QToolTip::add( w_createweightsCheckBox, tr( "Creates the individual weight maps for each image" ) );
    QWhatsThis::add( w_createweightsCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Create WEIGHTs</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Mask anything you want in the images before running this task.<br>\n"
"-- Select a threshold for cosmics</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Before running this task, you have to eyeball each of your exposures for satellite tracks or unwanted reflections. For multi-chip cameras, look at the binned mosaics you created previously. Use DS9 to do the masking (use POLYGONs, exclusively). See the THELI documentation for examples of how to do that. <br>\n"
"Once done, start this task. It will detect all cosmics and dead pixels in your image (including permanent defects found before) and creates an individual WEIGHT map based on this and the global weight map made before.<br>\n"
"If your data is strongly undersampled, stars might be mistaken as cosmics. In that case leave the <i>cosmics threshold</i> blank. You must use the outlier rejection in the coaddition to get rid of cosmics and other dirt effects.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"The polygon masks created with DS9 are modified such that they become readable to the weighting routines used. Thereafter, <i>SExtractor</i> and <i>Eye</i> are run in conjunction with each other over every individual image. Cosmics, hot and dead pixels are detected and included in the WEIGHT map. Any mask  found for an image is taken into account too, with the offending region being set to zero in the WEIGHT. Each image in the SCIENCE and the SKY directory (if present) gets its own WEIGHT map assigned. All WEIGHTs end up in the WEIGHTS \n"
"directory below the <b><font color=\"#0000ff\">Main data directory</font></b>.<br>\n"
"The WEIGHT images adopt the same file name extension as the images for which they were created.<br>\n"
"File name example: <i>WFI123_1OFCSD.fits and WFI123_1OFCSD.weight.fits</i>.\n"
"</p>" ) );
    w_debloomredoPushButton->setText( QString::null );
    QToolTip::add( w_debloomredoPushButton, tr( "Click here before repeating this task." ) );
    w_configure2PushButton->setText( tr( "Configure" ) );
    w_configure2PushButton->setAccel( QKeySequence( QString::null ) );
    w_configure1PushButton->setText( tr( "Configure" ) );
    w_configure1PushButton->setAccel( QKeySequence( QString::null ) );
    w_minoverlapTextLabel->setText( tr( "Minimum overlap:" ) );
    QToolTip::add( w_minoverlapTextLabel, tr( "Images with an overlap larger than that are regarded to belong to the same target." ) );
    w_minoverlapLineEdit->setText( tr( "1000" ) );
    QToolTip::add( w_minoverlapLineEdit, tr( "Images with an overlap larger than this value are considered to belong to the same target. The maximum allowed value is 1024.<br>" ) );
    w_createglobwCheckBox->setText( tr( "Create global weights" ) );
    QToolTip::add( w_createglobwCheckBox, tr( "Creates the basic weight map for the individual weight images" ) );
    QWhatsThis::add( w_createglobwCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Create global weights</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Provide lower and upper thresholds for the normalised FLAT in the configure dialogue.<br>\n"
"-- Optionally, specify if you want to use further bad pixel constraints from the \n"
"   SUPERFLAT (or the SKY SUPERFLAT), and from a DARK (or BIAS).<br>\n"
"</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"The FLAT field carries information on how sensitive a particular pixel is with respect to \n"
"all other pixels. Thus, it serves as a natural choice for a WEIGHT map. Pixels lower or \n"
"higher than the corresponding thresholds for the normalised FLAT are set to zero in \n"
"the WEIGHT map. A DARK and a normalised SUPERFLAT are well suited for the \n"
"detection of permanent pixel defects. Optionally, pixels with values outside the \n"
"specified ranges in these two images are set to zero as well.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"The normalised FLAT is taken as the basis of the WEIGHT map. All pixels with values \n"
"outside the specified ranges are set to zero.\n"
"</p>\n"
"\n"
"<p><b>ATTENTION:</b><br>\n"
"Eyeball the global weight images created in the WEIGHTS subdirectory. If you see a \n"
"very large number of pixels masked, or entire regions missing, then relax your \n"
"thresholds a bit (unless you deliberately wanted these regions to be masked).\n"
"</p>" ) );
    w_debloomCheckBox->setText( tr( "Debloom images" ) );
    QToolTip::add( w_debloomCheckBox, tr( "Removes blooming spikes in the images (for the preparation of colour pictures)" ) );
    QWhatsThis::add( w_debloomCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Debloom images</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify the blooming threshold</font></p>\n"
"\n"
"<p><b>Description:</b><br>This scripts interpolates pixels affected by blooming \n"
"spikes based on the pixels surrounding them. This is a pure cosmetic task you might \n"
"want to apply if you are interested in pretty pictures. For scientific applications you \n"
"should not perform deblooming, since no real information is recovered by the \n"
"removal of the spikes.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"Pixels values above the specified threshold are replaced by interpolated values.<br>\n"
" A <i>D</i> is appended to the file name extension, indicating that an image has \n"
"been debloomed. For example, after superflatting the extension would then read \n"
"<i>OFCD</i>.<br>\n"
"File name example: <i>WFI123_1OFCD.fits</i>\n"
"</p>" ) );
    w_bloomthreshTextLabel->setText( tr( "Saturation threshold:" ) );
    w_bloomthreshLineEdit->setText( tr( "55000" ) );
    QToolTip::add( w_bloomthreshLineEdit, tr( "Pixels with values higher than that are assumed <br>to be affected by blooming." ) );
    mainmenu_TabWidget->changeTab( TabPage_2, tr( "Weighting" ) );
    ap_astphotButtonGroup->setTitle( tr( "Tasks" ) );
    QWhatsThis::add( ap_astphotButtonGroup, tr( "<p><font color=\"#aa0000\" size=\"+1\">Astrometric and photometric calibration</font><br>\n"
"<p>Orange box: Create a specific astrometric standard star catalogue. Enter the RA and DEC of your targets (in degrees or hh:mm:ss format) if not already in the FITS headers. If you leave this field empty, the original SCIENCE (and if present, SKY) directories are processed. For more information, see the \"What's this?\" help for this box.</p>\n"
"\n"
"<p><b>Absolute photometry (indirect):</b> Determines absolute photometric zeropoints for each exposure based on standard star field observations.<br>\n"
"<p><b>Absolute photometry (direct):</b> Determines absolute photometric zeropoints for each exposure based on direct overlap with either SDSS or 2MASS.<br>\n"
"<p><b>Create source cat:</b> Extracts source catalogues from the images.<br>\n"
"<b>Astro+photometry:</b> Calculates an astrometric solution, including distortion correction and automatic mosaicing for multi-chip cameras if <b>Scamp</b> is selected from the ComboBox. <b>Shift (float | int)</b> determine only the relative offsets without respect to sky coordinates. You can choose between float and integer pixel shifts. All three methods calculate relative photometric zeropoints.<br>\n"
"</p>\n"
"<p>Right click each of the tasks to get more detailed information on what is going on.</p>" ) );
    ap_astrommethodconfigurePushButton->setText( tr( "Configure" ) );
    ap_astrommethodconfigurePushButton->setAccel( QKeySequence( QString::null ) );
    ap_sexcatconfigurePushButton->setText( tr( "Configure" ) );
    ap_sexcatconfigurePushButton->setAccel( QKeySequence( QString::null ) );
    ap_directphotredoPushButton->setText( QString::null );
    QToolTip::add( ap_directphotredoPushButton, tr( "Click here before repeating this task." ) );
    ap_absphotom_indirect_configurePushButton->setText( tr( "Configure" ) );
    ap_absphotom_indirect_configurePushButton->setAccel( QKeySequence( QString::null ) );
    ap_absphotom_direct_configurePushButton->setText( tr( "Configure" ) );
    ap_absphotom_direct_configurePushButton->setAccel( QKeySequence( QString::null ) );
    ap_absphotom_indirect_CheckBox->setText( tr( "Absolute photometry (indirect)" ) );
    QToolTip::add( ap_absphotom_indirect_CheckBox, tr( "Attempts to derive absolute photometric zeropoints for each exposure." ) );
    QWhatsThis::add( ap_absphotom_indirect_CheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Absolute photometry</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"This task determines a first-order astrometric solution for exposures of a standard star field. The latter must be contained in either the Landoldt- or the Stetson-catalogue. After images have been calibrated astrometrically, they are matched with photometric standard sources, and a 1-, 2 or 3-parameter fit is obtained for the zeropoint of each SCIENCE exposure. The ZP is written into the FITS header of the SCIENCE exposure. If no ZP could be found, e.g. because no standard stars were observed in that night, the ZP is set to -1.\n"
"</p>" ) );
    QWhatsThis::add( frame3, tr( "<b>Astrometric reference catalog</b><br>\n"
"Using this dialog you can create your own astrometric reference catalog that \n"
"matches your needs. You can obtain your catalog<br>\n"
"-- from the web, choosing e.g. GSC2, USNO-B, 2MASS or SDSS<br>\n"
"-- from an astrometrically calibrated image<br>\n"
"-- from a local binary copy of a reference catalog (format described in the pipeline \n"
"documentation, i.e. the one used for the USNO distribution)<br><br>\n"
"\n"
"If you selected the web download or a local copy, then the catalog will be retrieved around the specified coordinates within the given radius and down to the given magnitude. If the coordinates are not provided they will be determined based on the FITS headers of the data.<br><br>\n"
"\n"
"<b>NOTE:</b> If you enter coordinates in the RA and DEC field,\n"
"then these coordinates will be written into the FITS headers. This allows you\n"
"to overwrite the old values if they are very unreliable or not yet present. A warning will be shown upon clicking the Start button. If you do not want to modify the FITS headers, then <i>leave these fields empty</i>.<br><br>" ) );
    ap_maglimTextLabel->setText( tr( "Mag limit:" ) );
    ap_radiusTextLabel->setText( tr( "Radius :" ) );
    ap_refraTextLabel->setText( tr( "RA:" ) );
    QToolTip::add( ap_refdecLineEdit, tr( "OPTIONAL: The declination (deg or dd:mm:ss) of your target, if not already present (or wrong) in the FITS header. <br>" ) );
    ap_dtdminTextLabel->setText( tr( "DT  |  DMIN :" ) );
    QToolTip::add( ap_dtdminTextLabel, tr( "Size of the convolution kernel used for sky background modelling." ) );
    ap_refimagedetectthreshLineEdit->setText( tr( "10.0" ) );
    QToolTip::add( ap_refimagedetectthreshLineEdit, tr( "Detection threshold per pixel for an object" ) );
    ap_refimagedetectminareaLineEdit->setText( tr( "10" ) );
    QToolTip::add( ap_refimagedetectminareaLineEdit, tr( "Minimum number of connected pixels for an object" ) );
    ap_refdecTextLabel->setText( tr( "DEC:" ) );
    ap_selectimagePushButton->setText( tr( "Select image ..." ) );
    QToolTip::add( ap_refimageLineEdit, tr( "The full path to the astrometrically calibrated image." ) );
    ap_getastrefcatalogPushButton->setText( tr( "Get catalog" ) );
    textLabel1_7->setText( tr( "Astrometric\n"
"reference\n"
"catalog from:" ) );
    ap_arcserverComboBox->clear();
    ap_arcserverComboBox->insertItem( tr( "WEB (France)" ) );
    ap_arcserverComboBox->insertItem( tr( "WEB (USA)" ) );
    ap_arcserverComboBox->insertItem( tr( "WEB (Canada)" ) );
    ap_arcserverComboBox->insertItem( tr( "WEB (Japan)" ) );
    ap_arcserverComboBox->insertItem( tr( "WEB (India)" ) );
    ap_arcserverComboBox->insertItem( tr( "WEB (China)" ) );
    ap_arcserverComboBox->insertItem( tr( "WEB (UK)" ) );
    ap_arcserverComboBox->insertItem( tr( "WEB (Hawaii)" ) );
    ap_arcserverComboBox->insertItem( tr( "WEB (Russia)" ) );
    ap_arcserverComboBox->insertItem( tr( "Image" ) );
    QToolTip::add( ap_maglimLineEdit, tr( "The faint magnitude limit for the astrometric reference catalogue." ) );
    QToolTip::add( ap_refraLineEdit, tr( "OPTIONAL: The right ascension (deg or hh:mm:ss) of your target, if not already present (or wrong) in the FITS header. <br>" ) );
    QToolTip::add( ap_resolveLineEdit, tr( "OPTIONAL: The target name for which you want to retrieve coordinates<br>" ) );
    ap_resolveTextLabel->setText( tr( "Target:" ) );
    ap_resolvePushButton->setText( QString::null );
    QToolTip::add( ap_resolvePushButton, tr( "Find your target in the Simbad, NED and VizieR databases, and retrieve its coordinates.<br>" ) );
    QToolTip::add( ap_radiusLineEdit, tr( "The radius [in arcmin] within which reference sources are retrieved. This value is determined automatically, taking dither sizes into account. The field can be left empty.<br>" ) );
    ap_arccatComboBox->clear();
    ap_arccatComboBox->insertItem( tr( "SDSS-DR9" ) );
    ap_arccatComboBox->insertItem( tr( "GAIA-DR1" ) );
    ap_arccatComboBox->insertItem( tr( "IGSL" ) );
    ap_arccatComboBox->insertItem( tr( "PPMXL" ) );
    ap_arccatComboBox->insertItem( tr( "USNO-B1" ) );
    ap_arccatComboBox->insertItem( tr( "2MASS" ) );
    ap_arccatComboBox->insertItem( tr( "URAT1" ) );
    ap_arccatComboBox->insertItem( tr( "SPM4" ) );
    ap_arccatComboBox->insertItem( tr( "UCAC4" ) );
    ap_arccatComboBox->insertItem( tr( "GSC-2.3" ) );
    ap_arccatComboBox->insertItem( tr( "TYC" ) );
    ap_arccatComboBox->insertItem( tr( "ALLSKY" ) );
    ap_createsourcecatCheckBox->setText( tr( "Create source cat" ) );
    QToolTip::add( ap_createsourcecatCheckBox, tr( "Creates a source catalogue for each image for later astrometry and photometry" ) );
    QWhatsThis::add( ap_createsourcecatCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Create source cat</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Provide the approximate RA and DEC of  the center of your exposures in the orange box if this information is not yet present in the FITS headers.<br>\n"
"-- Provide the two SExtractor detection thresholds; further parameters are available for fine-tuning.</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"Extracts source catalogues for each image, using <i>SExtractor</i>. These are used thereafter for the astrometric and photometric calibration. You need to specify the SExtractor detection threshold and the minimum number of connected pixels above this threshold. Increasing the values might be useful if you have crowded fields. If your fields are rather empty, you might lower both values to e.g. 5 or less.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"First, some adjustments of the header have to be made before the images can run through the astrometric pipeline. If RA and DEC values are explicitly provided by the user, then these are written to the FITS headers.<br>\n"
"SExtractor is used to detect all objects in the image matching the specified detection thresholds. This process makes use of the WEIGHT images created previously (if available), thus the corresponding object catalogues are very clean. The results are written to a <tt>cat</tt> subdirectory.</p>" ) );
    ap_updateheaderPushButton->setText( tr( "Update header" ) );
    QToolTip::add( ap_updateheaderPushButton, tr( "Writes the zero-order astrometric solution (CRVAL, CRPIX, CD-matrix) into the FITS headers" ) );
    QWhatsThis::add( ap_updateheaderPushButton, QString::null );
    ap_restoreheaderPushButton->setText( tr( "Restore header" ) );
    QToolTip::add( ap_restoreheaderPushButton, tr( "Restores the original (raw data) zero-order astrometric information in the header (undo \"update header\")" ) );
    ap_astrommethodComboBox->clear();
    ap_astrommethodComboBox->insertItem( tr( "Scamp" ) );
    ap_astrommethodComboBox->insertItem( tr( "Astrometry.net" ) );
    ap_astrommethodComboBox->insertItem( tr( "Shift (float)" ) );
    ap_astrommethodComboBox->insertItem( tr( "Shift (int)" ) );
    ap_astrommethodComboBox->insertItem( tr( "Xcorr" ) );
    ap_astrommethodComboBox->insertItem( tr( "Header" ) );
    QToolTip::add( ap_astrommethodComboBox, tr( "Select a method for astrometric calibration. \"Shift only\" does not care about sky angle and absolute coordinates.<br>" ) );
    ap_astrometryCheckBox->setText( tr( "Astro+photometry" ) );
    QToolTip::add( ap_astrometryCheckBox, tr( "Calculates astrometric and photometric solutions" ) );
    QWhatsThis::add( ap_astrometryCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Astro+photometry</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#aa0000\">Mandatory</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Choose the astrometry method and configure it<br>\n"
"</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"<i>Scamp and astrometry.net</i> deliver full distortion correction for large mosaics with many hundreds of exposures. Execution time can become large for crowded fields.<br>\n"
"The <i>Shift only</i> and <i>Cross-corrrelation</i> approaches work only for single-chip cameras and are meant for exposures with only one or very few sources in the field (e.g. mid-IR data). The overlap between exposures should be at least half the field of view. Note that these solutions do not register the images with respect to absolute sky coordinates.<br>\n"
"The <i>Shift only</i> and <i>Cross-corrrelation</i> approaches assume that the WCS information in the FITS data is correct, and THELI will use the zero order solution (CRVAL, CRPIX, CD matrix). Higher order terms, such as for distortion, are not used (as they are likely in a format THELI does not understand).\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"<i>Astro+photometry</i> uses the previously created object catalogues for the astrometric calibration. Relative photometric zeropoints are calculated as well. The result is stored in a <tt>headers</tt> subdirectory.</p>\n"
"</p>" ) );
    ap_absphotom_direct_CheckBox->setText( tr( "Absolute photometry (direct)" ) );
    QToolTip::add( ap_absphotom_direct_CheckBox, tr( "Attempts to derive absolute photometric zeropoints for each exposure." ) );
    QWhatsThis::add( ap_absphotom_direct_CheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Absolute photometry</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"This task determines a first-order astrometric solution based on direct comparison with known SDSS or 2MASS magnitudes in the field. Optionally, residual gain differences between chips can be corrected for in this process.\n"
"</p>" ) );
    ap_resetfieldsPushButton->setText( tr( "&Defaults" ) );
    ap_resetfieldsPushButton->setAccel( QKeySequence( tr( "Alt+D" ) ) );
    ap_killPushButton->setText( tr( "&Abort" ) );
    ap_killPushButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    QToolTip::add( ap_killPushButton, tr( "Be careful with this button!<br>You will have to manually clean the directory from left over stuff, and put the data back into its previous position!" ) );
    ap_startPushButton->setText( tr( "&Start" ) );
    ap_startPushButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    ap_clearscriptsPushButton->setText( tr( "Clea&r scripts" ) );
    ap_clearscriptsPushButton->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    ap_messagesGroupBox->setTitle( tr( "Messages" ) );
    ap_commandGroupBox->setTitle( tr( "Commands" ) );
    mainmenu_TabWidget->changeTab( PageAstromPhotom, tr( "Astrom / Photom" ) );
    cc_killPushButton->setText( tr( "&Abort" ) );
    cc_killPushButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    QToolTip::add( cc_killPushButton, tr( "Be careful with this button!<br>You will have to manually clean the directory from left over stuff, and put the data back into its previous position!" ) );
    cc_clearscriptsPushButton->setText( tr( "Clea&r scripts" ) );
    cc_clearscriptsPushButton->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    cc_startPushButton->setText( tr( "&Start" ) );
    cc_startPushButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    cc_messagesGroupBox->setTitle( tr( "Messages" ) );
    cc_commandGroupBox->setTitle( tr( "Commands" ) );
    cc_coaddButtonGroup->setTitle( tr( "Tasks" ) );
    QWhatsThis::add( cc_coaddButtonGroup, tr( "<p><font color=\"#aa0000\" size=\"+1\">Sky subtraction and Coaddition</font><br>\n"
"<b>Sky subtraction:</b> Subtracts the sky in order to achieve a seamless coadded image. You can choose between automatic modelling, or the subtraction of a constant value. If you do not do the task, then the coaddition will be done without sky subtraction.<br>\n"
"<b>Coaddition:</b>The images and their weights are resampled to the sky coordinates you specify (otherwise a reference coordinate is chosen automatically). You can also specify the pixel scale you want for the coadded image, optionally filter for outliers, and select a resampling kernel.<br>\n"
"<b>Resolve link structure:</b> If you have a multi-processor machine, and processed with it data from a multi-chip camera, and if you had distributed the data over different hard disks using <b><font color=\"#0000ff\">Create links</font></b>, then you can resolve this link structure with this script for the given directory.\n"
"</p>\n"
"<p>Right click each of the tasks to get more detailed information on what is going on.</p>" ) );
    cc_skysubconfigurePushButton->setText( tr( "Configure" ) );
    cc_skysubconfigurePushButton->setAccel( QKeySequence( QString::null ) );
    cc_coaddconfigurePushButton->setText( tr( "Configure" ) );
    cc_coaddconfigurePushButton->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( cc_resolvedirLineEdit, tr( "Directory for which you want to resolve the link structure." ) );
    cc_resolvelinksCheckBox->setText( tr( "Resolve links" ) );
    cc_resolvelinksCheckBox->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( cc_resolvelinksCheckBox, tr( "Recollects the data spread over several hard disks in the beginning" ) );
    QWhatsThis::add( cc_resolvelinksCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Resolve link structure</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#00aa00\">OPTIONAL</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify the directory that should be resolved</font></p>\n"
"\n"
"<p><b>Description:</b><br>\n"
"If you used the <b><font color=\"#0000ff\">Create links</font></b> task in the \n"
"preparation processing group, then you can resolve the link structure created with this \n"
"task. The data is copied back to its corresponding position in the main path.<br>\n"
"You only need this task when you processed multi-chip camera data and created a \n"
"link structure for it.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"Resolves and delete links. The data is copied back into the main path.\n"
"</p>" ) );
    cc_textLabel2_2->setText( tr( "in this directory (full path)" ) );
    cc_createskysubCheckBox->setText( tr( "Sky subtraction" ) );
    cc_createskysubCheckBox->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( cc_createskysubCheckBox, tr( "Subtracts the sky from the images" ) );
    QWhatsThis::add( cc_createskysubCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Sky subtraction</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY for multi-chip cmeras</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify the FWHM of the Gaussian convolution kernel and the detection thresholds<br>\n"
"-- If you want to subtract a constant as the sky estimate, choose from which chip and with what method. Additionally, you can define a smaller region of the sky for an unbiased estimate of a constant sky.\n"
"</font></p>\n"
"<p><b>Description:</b><br>\n"
"Sky subtraction is essential to get seamless images in mosaiced data. This holds for dithered images taken with single-chip cameras as well. If you have very extended nebulosity in your image, try to subtract a constant.\n"
"</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"All objects above the detection thresholds are removed from the image. The remaining image is smoothed with a Gaussian kernel, yielding an estimate of the sky background. This image is then subtracted from the originals. Alternatively, a constant sky background is estimated from one chip representative for the others, or from each chip individually.<br>\n"
"Sky subtracted images have the string \"<i>.sub</i>\" inserted into their file name.<br>Example: <i>WFI123_1OFC.sub.fits</i>\n"
"</p>" ) );
    cc_coaddCheckBox->setText( tr( "Coaddition" ) );
    cc_coaddCheckBox->setAccel( QKeySequence( QString::null ) );
    QToolTip::add( cc_coaddCheckBox, tr( "Coadds the data" ) );
    QWhatsThis::add( cc_coaddCheckBox, tr( "<p><font color=\"#00007f\" size=\"+1\"><b>Coaddition</b></font></p>\n"
"\n"
"<p><b>Status: <font color=\"#ff0000\">MANDATORY</font></b></p>\n"
"\n"
"<p><b>User action required:</b><font color=\"#aa0000\"><br>\n"
"-- Specify the directory you want to coadd (if other than SCIENCE)<br>\n"
"-- Provide RA, DEC, pixel scale and an ID string (all optional)<br>\n"
"-- Select outlier rejection and resampling kernel (optional)</font></p>\n"
"<p><b>Description:</b><br>\n"
"Coadds the images in a directory using <i>SWarp</i>. You can specify a reference coordinate (RA and DEC) as well as the output pixel scale and a unique identifying string. The latter allows you to create several coadditions of the same data set without overwriting the previous one. You can also choose a resampling kernel, and switch on outlier rejection (usually unnecessary due to the use of WEIGHTs).</p>\n"
"\n"
"<p><b>Script action:</b><br>\n"
"First, there is a preparation step at which the output image is initialised. It is followed by the resampling of the individual exposures and by the coaddition itself.\n"
"</p>" ) );
    mainmenu_TabWidget->changeTab( PageCoadd, tr( "Coaddition" ) );
    settingsGUIfontAction->setText( tr( "GUI font" ) );
    settingsGUIfontAction->setMenuText( tr( "GUI font" ) );
    viewCommandhistoryAction->setText( tr( "Command history" ) );
    viewCommandhistoryAction->setMenuText( tr( "Command history" ) );
    helpTechpaperAction->setText( tr( "Technical paper" ) );
    helpTechpaperAction->setMenuText( tr( "Technical paper" ) );
    helpVersionAction->setText( tr( "About THELI" ) );
    helpVersionAction->setMenuText( tr( "About THELI" ) );
    editScriptsAction->setText( tr( "Scripts" ) );
    editScriptsAction->setMenuText( tr( "Scripts" ) );
    ActionGroupEditors->setText( tr( "Editor" ) );
    nano->setText( tr( "nano" ) );
    emacs->setText( tr( "emacs" ) );
    xemacs->setText( tr( "xemacs" ) );
    nedit->setText( tr( "nedit" ) );
    vi->setText( tr( "vi" ) );
    ActionGroupImageViewers->setText( tr( "Image viewer" ) );
    skycat->setText( tr( "skycat" ) );
    ds9->setText( tr( "ds9" ) );
    ActionGroupPDFViewers->setText( tr( "PDF viewer" ) );
    pdfacroread->setText( tr( "acroread" ) );
    pdfgvv->setText( tr( "gvv" ) );
    pdfxpdf->setText( tr( "xpdf" ) );
    pdfxpdf->setMenuText( tr( "xpdf" ) );
    viewDatadirFITSAction->setText( tr( "FITS image+header / data directory" ) );
    viewDatadirFITSAction->setMenuText( tr( "FITS image+header / data directory" ) );
    viewProcessinglogsPreparation_logAction->setText( tr( "Preparation log" ) );
    viewProcessinglogsPreparation_logAction->setMenuText( tr( "Preparation log" ) );
    viewProcessinglogsCalibration_logAction->setText( tr( "Calibration log" ) );
    viewProcessinglogsCalibration_logAction->setMenuText( tr( "Calibration log" ) );
    viewProcessinglogsBackground_logAction->setText( tr( "Background log" ) );
    viewProcessinglogsBackground_logAction->setMenuText( tr( "Background log" ) );
    viewProcessinglogsWeighting_logAction->setText( tr( "Weighting log" ) );
    viewProcessinglogsWeighting_logAction->setMenuText( tr( "Weighting log" ) );
    viewProcessinglogsAstromPhotom_logAction->setText( tr( "Astrom / Photom log" ) );
    viewProcessinglogsAstromPhotom_logAction->setMenuText( tr( "Astrom / Photom log" ) );
    viewProcessinglogsCoaddition_logAction->setText( tr( "Coaddition log" ) );
    viewProcessinglogsCoaddition_logAction->setMenuText( tr( "Coaddition log" ) );
    deleteSPLIT_IMAGESAction->setText( tr( "SPLIT_IMAGES" ) );
    deleteSPLIT_IMAGESAction->setMenuText( tr( "SPLIT_IMAGES" ) );
    deleteMASK_IMAGESAction->setText( tr( "MASK_IMAGES" ) );
    deleteMASK_IMAGESAction->setMenuText( tr( "MASK_IMAGES" ) );
    deleteOFC_IMAGESAction->setText( tr( "OFC_IMAGES" ) );
    deleteOFC_IMAGESAction->setMenuText( tr( "OFC_IMAGES" ) );
    deleteOFCB_IMAGESAction->setText( tr( "OFCB_IMAGES" ) );
    deleteOFCB_IMAGESAction->setMenuText( tr( "OFCB_IMAGES" ) );
    deleteOFCH_IMAGESAction->setText( tr( "OFCH_IMAGES" ) );
    deleteOFCH_IMAGESAction->setMenuText( tr( "OFCH_IMAGES" ) );
    deleteundebloomedAction->setText( tr( "undebloomed" ) );
    deleteundebloomedAction->setMenuText( tr( "undebloomed" ) );
    deletebeforedirectphotAction->setText( tr( "before abs. photometry" ) );
    deletebeforedirectphotAction->setMenuText( tr( "before abs. photometry" ) );
    deleteresampledAction->setText( tr( "resampled" ) );
    deleteresampledAction->setMenuText( tr( "resampled" ) );
    helpWhats_thisAction->setText( tr( "What's this?" ) );
    helpWhats_thisAction->setMenuText( tr( "What's this?" ) );
    deleteFlush_tmp_dirAction->setText( tr( "Flush tmp dir" ) );
    deleteFlush_tmp_dirAction->setMenuText( tr( "Flush tmp dir" ) );
    deleteFlush_tmp_dirAction->setToolTip( tr( "Flush tmp dir" ) );
    statusCPSDebiasingAction->setText( tr( "Debiasing and flatfielding done" ) );
    statusCPSDebiasingAction->setMenuText( tr( "Debiasing and flatfielding done" ) );
    statusCPSBackgroundAction->setText( tr( "Background modeling done" ) );
    statusCPSBackgroundAction->setMenuText( tr( "Background modeling done" ) );
    statusCPSChopnodAction->setText( tr( "Chop-nod skysubtraction done" ) );
    statusCPSChopnodAction->setMenuText( tr( "Chop-nod skysubtraction done" ) );
    statusCPSCollapseAction->setText( tr( "Collapse correction done" ) );
    statusCPSCollapseAction->setMenuText( tr( "Collapse correction done" ) );
    statusCPSDebloomingAction->setText( tr( "Deblooming done" ) );
    statusCPSDebloomingAction->setMenuText( tr( "Deblooming done" ) );
    statusCPSDirectphotAction->setText( tr( "Direct abs. photometry done" ) );
    statusCPSDirectphotAction->setMenuText( tr( "Direct abs. photometry done" ) );
    miscellaneousMake_colour_picture_Action->setText( tr( "Prepare color picture" ) );
    miscellaneousMake_colour_picture_Action->setMenuText( tr( "Prepare color picture" ) );
    editProfessional_instrumentsAction->setText( tr( "Professional instruments" ) );
    editProfessional_instrumentsAction->setMenuText( tr( "Professional instruments" ) );
    editProfessional_instrumentsAction->setToolTip( tr( "Professional instruments" ) );
    miscellaneousCombineImagesAction->setText( tr( "Combine folder of images" ) );
    miscellaneousCombineImagesAction->setMenuText( tr( "Combine folder of images" ) );
    miscellaneousImage_statisticsAction->setText( tr( "Image statistics" ) );
    miscellaneousImage_statisticsAction->setMenuText( tr( "Image statistics" ) );
    miscellaneousImalyzerAction->setText( tr( "Imalyzer" ) );
    miscellaneousImalyzerAction->setMenuText( tr( "Imalyzer" ) );
    miscellaneousGetZP_Action->setText( tr( "Absolute photometric ZP" ) );
    miscellaneousGetZP_Action->setMenuText( tr( "Absolute photometric ZP" ) );
    miscellaneousAnimateAction->setText( tr( "Animate" ) );
    miscellaneousAnimateAction->setMenuText( tr( "Animate" ) );
    miscellaneousProgress_MonitorAction->setText( tr( "Progress Monitor (test)" ) );
    miscellaneousProgress_MonitorAction->setMenuText( tr( "Progress Monitor (test)" ) );
    editCommercial_instrumentsAction->setText( tr( "Commercial instruments" ) );
    editCommercial_instrumentsAction->setMenuText( tr( "Commercial instruments" ) );
    editUser_defined_instrumentsAction->setText( tr( "User defined instruments" ) );
    editUser_defined_instrumentsAction->setMenuText( tr( "User defined instruments" ) );
    if (theliMenuBar->findItem(1))
        theliMenuBar->findItem(1)->setText( tr( "Edit" ) );
    Settings->changeItem( Settings->idAt( 2 ), tr( "Editor" ) );
    Settings->changeItem( Settings->idAt( 3 ), tr( "Image viewer" ) );
    Settings->changeItem( Settings->idAt( 4 ), tr( "PDF viewer" ) );
    if (theliMenuBar->findItem(2))
        theliMenuBar->findItem(2)->setText( tr( "Settings" ) );
    if (theliMenuBar->findItem(3))
        theliMenuBar->findItem(3)->setText( tr( "View" ) );
    if (theliMenuBar->findItem(4))
        theliMenuBar->findItem(4)->setText( tr( "Status" ) );
    if (theliMenuBar->findItem(5))
        theliMenuBar->findItem(5)->setText( tr( "Miscellaneous" ) );
    if (theliMenuBar->findItem(6))
        theliMenuBar->findItem(6)->setText( tr( "Delete" ) );
    if (theliMenuBar->findItem(7))
        theliMenuBar->findItem(7)->setText( tr( "Help" ) );
}

