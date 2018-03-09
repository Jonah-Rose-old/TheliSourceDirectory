/****************************************************************************
** Form implementation generated from reading ui file 'manualsky.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "manualsky.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../manualsky.ui.h"
/*
 *  Constructs a manualsky as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
manualsky::manualsky( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "manualsky" );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    setModal( TRUE );
    manualskyLayout = new QGridLayout( this, 1, 1, 11, 6, "manualskyLayout"); 

    layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 
    spacer180 = new QSpacerItem( 280, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10->addItem( spacer180 );

    ms_defaultsPushButton = new QPushButton( this, "ms_defaultsPushButton" );
    ms_defaultsPushButton->setMinimumSize( QSize( 100, 0 ) );
    ms_defaultsPushButton->setMaximumSize( QSize( 100, 32767 ) );
    ms_defaultsPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout10->addWidget( ms_defaultsPushButton );

    ms_cancelPushButton = new QPushButton( this, "ms_cancelPushButton" );
    ms_cancelPushButton->setMinimumSize( QSize( 100, 0 ) );
    ms_cancelPushButton->setMaximumSize( QSize( 100, 32767 ) );
    ms_cancelPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout10->addWidget( ms_cancelPushButton );

    ms_closePushButton = new QPushButton( this, "ms_closePushButton" );
    ms_closePushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ms_closePushButton->sizePolicy().hasHeightForWidth() ) );
    ms_closePushButton->setMinimumSize( QSize( 100, 0 ) );
    ms_closePushButton->setMaximumSize( QSize( 100, 32767 ) );
    ms_closePushButton->setFocusPolicy( QPushButton::NoFocus );
    layout10->addWidget( ms_closePushButton );

    manualskyLayout->addMultiCellLayout( layout10, 5, 5, 0, 1 );

    ms_buttonGroup = new QButtonGroup( this, "ms_buttonGroup" );
    ms_buttonGroup->setFrameShadow( QButtonGroup::Plain );
    ms_buttonGroup->setLineWidth( 0 );
    ms_buttonGroup->setColumnLayout(0, Qt::Vertical );
    ms_buttonGroup->layout()->setSpacing( 6 );
    ms_buttonGroup->layout()->setMargin( 11 );
    ms_buttonGroupLayout = new QGridLayout( ms_buttonGroup->layout() );
    ms_buttonGroupLayout->setAlignment( Qt::AlignTop );

    ms_modelskyRadioButton = new QRadioButton( ms_buttonGroup, "ms_modelskyRadioButton" );
    ms_modelskyRadioButton->setFocusPolicy( QRadioButton::NoFocus );
    ms_buttonGroup->insert( ms_modelskyRadioButton, 0 );

    ms_buttonGroupLayout->addWidget( ms_modelskyRadioButton, 0, 0 );

    ms_manualskyRadioButton = new QRadioButton( ms_buttonGroup, "ms_manualskyRadioButton" );
    ms_manualskyRadioButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ms_manualskyRadioButton->sizePolicy().hasHeightForWidth() ) );
    ms_manualskyRadioButton->setFocusPolicy( QRadioButton::NoFocus );
    ms_buttonGroup->insert( ms_manualskyRadioButton, 1 );

    ms_buttonGroupLayout->addWidget( ms_manualskyRadioButton, 1, 0 );

    manualskyLayout->addWidget( ms_buttonGroup, 0, 0 );
    spacer17 = new QSpacerItem( 280, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    manualskyLayout->addItem( spacer17, 0, 1 );
    spacer190 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    manualskyLayout->addItem( spacer190, 4, 0 );

    ms_frame2 = new QFrame( this, "ms_frame2" );
    ms_frame2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, ms_frame2->sizePolicy().hasHeightForWidth() ) );
    ms_frame2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    ms_frame2->setFrameShape( QFrame::GroupBoxPanel );
    ms_frame2->setFrameShadow( QFrame::Sunken );
    ms_frame2Layout = new QGridLayout( ms_frame2, 1, 1, 11, 6, "ms_frame2Layout"); 

    textLabel1_3 = new QLabel( ms_frame2, "textLabel1_3" );

    ms_frame2Layout->addMultiCellWidget( textLabel1_3, 0, 0, 0, 2 );

    textLabel3 = new QLabel( ms_frame2, "textLabel3" );

    ms_frame2Layout->addWidget( textLabel3, 2, 0 );

    textLabel4_2_3 = new QLabel( ms_frame2, "textLabel4_2_3" );

    ms_frame2Layout->addWidget( textLabel4_2_3, 3, 0 );

    textLabel4_2 = new QLabel( ms_frame2, "textLabel4_2" );

    ms_frame2Layout->addWidget( textLabel4_2, 4, 0 );

    textLabel4_2_2 = new QLabel( ms_frame2, "textLabel4_2_2" );

    ms_frame2Layout->addWidget( textLabel4_2_2, 5, 0 );

    ms_dec2LineEdit = new QLineEdit( ms_frame2, "ms_dec2LineEdit" );
    ms_dec2LineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ms_dec2LineEdit->sizePolicy().hasHeightForWidth() ) );
    ms_dec2LineEdit->setMinimumSize( QSize( 120, 0 ) );
    ms_dec2LineEdit->setMaximumSize( QSize( 120, 32767 ) );
    ms_dec2LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_frame2Layout->addWidget( ms_dec2LineEdit, 5, 1 );

    ms_dec1LineEdit = new QLineEdit( ms_frame2, "ms_dec1LineEdit" );
    ms_dec1LineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ms_dec1LineEdit->sizePolicy().hasHeightForWidth() ) );
    ms_dec1LineEdit->setMinimumSize( QSize( 120, 0 ) );
    ms_dec1LineEdit->setMaximumSize( QSize( 120, 32767 ) );
    ms_dec1LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_frame2Layout->addWidget( ms_dec1LineEdit, 4, 1 );

    ms_ra2LineEdit = new QLineEdit( ms_frame2, "ms_ra2LineEdit" );
    ms_ra2LineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ms_ra2LineEdit->sizePolicy().hasHeightForWidth() ) );
    ms_ra2LineEdit->setMinimumSize( QSize( 120, 0 ) );
    ms_ra2LineEdit->setMaximumSize( QSize( 120, 32767 ) );
    ms_ra2LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_frame2Layout->addWidget( ms_ra2LineEdit, 3, 1 );

    ms_ra1LineEdit = new QLineEdit( ms_frame2, "ms_ra1LineEdit" );
    ms_ra1LineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ms_ra1LineEdit->sizePolicy().hasHeightForWidth() ) );
    ms_ra1LineEdit->setMinimumSize( QSize( 120, 0 ) );
    ms_ra1LineEdit->setMaximumSize( QSize( 120, 32767 ) );
    ms_ra1LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_frame2Layout->addWidget( ms_ra1LineEdit, 2, 1 );

    ms_xmaxLineEdit = new QLineEdit( ms_frame2, "ms_xmaxLineEdit" );
    ms_xmaxLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ms_xmaxLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ms_xmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_frame2Layout->addWidget( ms_xmaxLineEdit, 3, 4 );

    ms_yminLineEdit = new QLineEdit( ms_frame2, "ms_yminLineEdit" );
    ms_yminLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ms_yminLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ms_yminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_frame2Layout->addWidget( ms_yminLineEdit, 4, 4 );

    ms_ymaxLineEdit = new QLineEdit( ms_frame2, "ms_ymaxLineEdit" );
    ms_ymaxLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ms_ymaxLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ms_ymaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_frame2Layout->addWidget( ms_ymaxLineEdit, 5, 4 );

    ms_xminLineEdit = new QLineEdit( ms_frame2, "ms_xminLineEdit" );
    ms_xminLineEdit->setMinimumSize( QSize( 60, 0 ) );
    ms_xminLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ms_xminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_frame2Layout->addWidget( ms_xminLineEdit, 2, 4 );

    textLabel4_4 = new QLabel( ms_frame2, "textLabel4_4" );

    ms_frame2Layout->addWidget( textLabel4_4, 4, 3 );

    textLabel4_5 = new QLabel( ms_frame2, "textLabel4_5" );

    ms_frame2Layout->addWidget( textLabel4_5, 5, 3 );

    textLabel4_3 = new QLabel( ms_frame2, "textLabel4_3" );

    ms_frame2Layout->addWidget( textLabel4_3, 3, 3 );

    textLabel4 = new QLabel( ms_frame2, "textLabel4" );

    ms_frame2Layout->addWidget( textLabel4, 2, 3 );
    spacer20 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ms_frame2Layout->addItem( spacer20, 5, 2 );

    textLabel5 = new QLabel( ms_frame2, "textLabel5" );

    ms_frame2Layout->addMultiCellWidget( textLabel5, 3, 4, 2, 2 );

    manualskyLayout->addWidget( ms_frame2, 3, 0 );

    ms_frame3 = new QFrame( this, "ms_frame3" );
    ms_frame3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, ms_frame3->sizePolicy().hasHeightForWidth() ) );
    ms_frame3->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    ms_frame3->setFrameShape( QFrame::GroupBoxPanel );
    ms_frame3->setFrameShadow( QFrame::Sunken );
    ms_frame3Layout = new QGridLayout( ms_frame3, 1, 1, 11, 6, "ms_frame3Layout"); 

    layout7 = new QGridLayout( 0, 1, 1, 0, 6, "layout7"); 

    manualsky_table = new QTable( ms_frame3, "manualsky_table" );
    manualsky_table->setMinimumSize( QSize( 250, 100 ) );
    manualsky_table->setFocusPolicy( QTable::ClickFocus );
    manualsky_table->setNumRows( 3 );
    manualsky_table->setNumCols( 2 );

    layout7->addWidget( manualsky_table, 1, 0 );

    layout6 = new QGridLayout( 0, 1, 1, 0, 6, "layout6"); 

    ms_fillPushButton = new QPushButton( ms_frame3, "ms_fillPushButton" );
    ms_fillPushButton->setMinimumSize( QSize( 0, 0 ) );
    ms_fillPushButton->setMaximumSize( QSize( 32767, 32767 ) );
    ms_fillPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout6->addWidget( ms_fillPushButton, 2, 0 );
    spacer181_5 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer181_5, 4, 2 );

    ms_showchipComboBox = new QComboBox( FALSE, ms_frame3, "ms_showchipComboBox" );
    ms_showchipComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, ms_showchipComboBox->sizePolicy().hasHeightForWidth() ) );
    ms_showchipComboBox->setMinimumSize( QSize( 0, 0 ) );
    ms_showchipComboBox->setMaximumSize( QSize( 32767, 32767 ) );
    ms_showchipComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout6->addMultiCellWidget( ms_showchipComboBox, 4, 4, 0, 1 );

    ms_ccdselectionComboBox = new QComboBox( FALSE, ms_frame3, "ms_ccdselectionComboBox" );
    ms_ccdselectionComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, ms_ccdselectionComboBox->sizePolicy().hasHeightForWidth() ) );
    ms_ccdselectionComboBox->setMinimumSize( QSize( 0, 0 ) );
    ms_ccdselectionComboBox->setMaximumSize( QSize( 32767, 32767 ) );
    ms_ccdselectionComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout6->addMultiCellWidget( ms_ccdselectionComboBox, 5, 5, 0, 1 );
    spacer181_4 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer181_4, 0, 2 );
    spacer181 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer181, 3, 2 );

    ms_getvaluesPushButton = new QPushButton( ms_frame3, "ms_getvaluesPushButton" );
    ms_getvaluesPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, ms_getvaluesPushButton->sizePolicy().hasHeightForWidth() ) );
    ms_getvaluesPushButton->setMinimumSize( QSize( 0, 0 ) );
    ms_getvaluesPushButton->setMaximumSize( QSize( 32767, 32767 ) );
    ms_getvaluesPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout6->addMultiCellWidget( ms_getvaluesPushButton, 0, 0, 0, 1 );
    spacer181_2 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer181_2, 2, 2 );

    layout5 = new QHBoxLayout( 0, 0, 6, "layout5"); 

    ms_loadPushButton = new QPushButton( ms_frame3, "ms_loadPushButton" );
    ms_loadPushButton->setMinimumSize( QSize( 0, 0 ) );
    ms_loadPushButton->setMaximumSize( QSize( 32767, 32767 ) );
    ms_loadPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout5->addWidget( ms_loadPushButton );

    ms_saveasPushButton = new QPushButton( ms_frame3, "ms_saveasPushButton" );
    ms_saveasPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, ms_saveasPushButton->sizePolicy().hasHeightForWidth() ) );
    ms_saveasPushButton->setMinimumSize( QSize( 0, 0 ) );
    ms_saveasPushButton->setMaximumSize( QSize( 32767, 32767 ) );
    ms_saveasPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout5->addWidget( ms_saveasPushButton );

    ms_clearPushButton = new QPushButton( ms_frame3, "ms_clearPushButton" );
    ms_clearPushButton->setMinimumSize( QSize( 0, 0 ) );
    ms_clearPushButton->setMaximumSize( QSize( 32767, 32767 ) );
    ms_clearPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout5->addWidget( ms_clearPushButton );
    spacer14 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout5->addItem( spacer14 );

    layout6->addMultiCellLayout( layout5, 1, 1, 0, 2 );
    spacer181_3 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer181_3, 5, 2 );

    ms_showmethodComboBox = new QComboBox( FALSE, ms_frame3, "ms_showmethodComboBox" );
    ms_showmethodComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, ms_showmethodComboBox->sizePolicy().hasHeightForWidth() ) );
    ms_showmethodComboBox->setMinimumSize( QSize( 0, 0 ) );
    ms_showmethodComboBox->setMaximumSize( QSize( 32767, 32767 ) );
    ms_showmethodComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout6->addMultiCellWidget( ms_showmethodComboBox, 3, 3, 0, 1 );

    ms_fillLineEdit = new QLineEdit( ms_frame3, "ms_fillLineEdit" );
    ms_fillLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, ms_fillLineEdit->sizePolicy().hasHeightForWidth() ) );
    ms_fillLineEdit->setMinimumSize( QSize( 0, 0 ) );
    ms_fillLineEdit->setMaximumSize( QSize( 32767, 32767 ) );
    ms_fillLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout6->addWidget( ms_fillLineEdit, 2, 1 );

    layout7->addLayout( layout6, 0, 0 );

    ms_frame3Layout->addLayout( layout7, 1, 0 );

    textLabel2 = new QLabel( ms_frame3, "textLabel2" );

    ms_frame3Layout->addWidget( textLabel2, 0, 0 );

    manualskyLayout->addMultiCellWidget( ms_frame3, 1, 3, 1, 1 );

    ms_1GroupBox = new QGroupBox( this, "ms_1GroupBox" );
    ms_1GroupBox->setEnabled( TRUE );
    ms_1GroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    ms_1GroupBox->setColumnLayout(0, Qt::Vertical );
    ms_1GroupBox->layout()->setSpacing( 6 );
    ms_1GroupBox->layout()->setMargin( 11 );
    ms_1GroupBoxLayout = new QGridLayout( ms_1GroupBox->layout() );
    ms_1GroupBoxLayout->setAlignment( Qt::AlignTop );

    ms_chipComboBox = new QComboBox( FALSE, ms_1GroupBox, "ms_chipComboBox" );
    ms_chipComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ms_chipComboBox->sizePolicy().hasHeightForWidth() ) );
    ms_chipComboBox->setMinimumSize( QSize( 160, 0 ) );
    ms_chipComboBox->setMaximumSize( QSize( 160, 32767 ) );
    ms_chipComboBox->setFocusPolicy( QComboBox::NoFocus );

    ms_1GroupBoxLayout->addWidget( ms_chipComboBox, 2, 0 );

    textLabel1 = new QLabel( ms_1GroupBox, "textLabel1" );

    ms_1GroupBoxLayout->addWidget( textLabel1, 0, 0 );

    ms_methodComboBox = new QComboBox( FALSE, ms_1GroupBox, "ms_methodComboBox" );
    ms_methodComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ms_methodComboBox->sizePolicy().hasHeightForWidth() ) );
    ms_methodComboBox->setMinimumSize( QSize( 160, 0 ) );
    ms_methodComboBox->setMaximumSize( QSize( 160, 32767 ) );
    ms_methodComboBox->setFocusPolicy( QComboBox::NoFocus );

    ms_1GroupBoxLayout->addWidget( ms_methodComboBox, 1, 0 );

    ms_infoPushButton = new QPushButton( ms_1GroupBox, "ms_infoPushButton" );
    ms_infoPushButton->setFocusPolicy( QPushButton::NoFocus );

    ms_1GroupBoxLayout->addMultiCellWidget( ms_infoPushButton, 1, 2, 1, 1 );

    manualskyLayout->addWidget( ms_1GroupBox, 2, 0 );

    ms_modelButtonGroup = new QButtonGroup( this, "ms_modelButtonGroup" );
    ms_modelButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, ms_modelButtonGroup->sizePolicy().hasHeightForWidth() ) );
    ms_modelButtonGroup->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    ms_modelButtonGroup->setColumnLayout(0, Qt::Vertical );
    ms_modelButtonGroup->layout()->setSpacing( 6 );
    ms_modelButtonGroup->layout()->setMargin( 11 );
    ms_modelButtonGroupLayout = new QGridLayout( ms_modelButtonGroup->layout() );
    ms_modelButtonGroupLayout->setAlignment( Qt::AlignTop );

    ms_skysubdetectminareaLineEdit = new QLineEdit( ms_modelButtonGroup, "ms_skysubdetectminareaLineEdit" );
    ms_skysubdetectminareaLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ms_skysubdetectminareaLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ms_skysubdetectminareaLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ms_skysubdetectminareaLineEdit->setMaxLength( 9 );
    ms_skysubdetectminareaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_modelButtonGroupLayout->addWidget( ms_skysubdetectminareaLineEdit, 1, 1 );

    ms_skysubdetectthreshLineEdit = new QLineEdit( ms_modelButtonGroup, "ms_skysubdetectthreshLineEdit" );
    ms_skysubdetectthreshLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ms_skysubdetectthreshLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ms_skysubdetectthreshLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ms_skysubdetectthreshLineEdit->setMaxLength( 9 );
    ms_skysubdetectthreshLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_modelButtonGroupLayout->addWidget( ms_skysubdetectthreshLineEdit, 1, 0 );

    cc_textLabel10_4 = new QLabel( ms_modelButtonGroup, "cc_textLabel10_4" );

    ms_modelButtonGroupLayout->addWidget( cc_textLabel10_4, 0, 0 );

    cc_textLabel10_3_2 = new QLabel( ms_modelButtonGroup, "cc_textLabel10_3_2" );

    ms_modelButtonGroupLayout->addWidget( cc_textLabel10_3_2, 0, 2 );

    cc_textLabel10_2_2 = new QLabel( ms_modelButtonGroup, "cc_textLabel10_2_2" );

    ms_modelButtonGroupLayout->addWidget( cc_textLabel10_2_2, 0, 1 );

    ms_saveskymodelCheckBox = new QCheckBox( ms_modelButtonGroup, "ms_saveskymodelCheckBox" );

    ms_modelButtonGroupLayout->addMultiCellWidget( ms_saveskymodelCheckBox, 3, 3, 0, 2 );
    spacer34 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ms_modelButtonGroupLayout->addItem( spacer34, 3, 3 );

    textLabel1_17_3 = new QLabel( ms_modelButtonGroup, "textLabel1_17_3" );

    ms_modelButtonGroupLayout->addMultiCellWidget( textLabel1_17_3, 2, 2, 0, 1 );

    ms_skysubbacksizeLineEdit = new QLineEdit( ms_modelButtonGroup, "ms_skysubbacksizeLineEdit" );
    ms_skysubbacksizeLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ms_skysubbacksizeLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ms_skysubbacksizeLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ms_skysubbacksizeLineEdit->setMaxLength( 9 );
    ms_skysubbacksizeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_modelButtonGroupLayout->addWidget( ms_skysubbacksizeLineEdit, 1, 2 );

    ms_maskexpandfactorLineEdit = new QLineEdit( ms_modelButtonGroup, "ms_maskexpandfactorLineEdit" );
    ms_maskexpandfactorLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ms_maskexpandfactorLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ms_maskexpandfactorLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ms_maskexpandfactorLineEdit->setMaxLength( 9 );
    ms_maskexpandfactorLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    ms_modelButtonGroupLayout->addWidget( ms_maskexpandfactorLineEdit, 2, 2 );

    manualskyLayout->addWidget( ms_modelButtonGroup, 1, 0 );
    languageChange();
    resize( QSize(670, 549).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ms_ra1LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ms_ra2LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ms_dec2LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ms_dec1LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ms_getvaluesPushButton, SIGNAL( clicked() ), this, SLOT( get_manualsky() ) );
    connect( ms_loadPushButton, SIGNAL( clicked() ), this, SLOT( load_manualsky() ) );
    connect( ms_closePushButton, SIGNAL( clicked() ), this, SLOT( save_quit() ) );
    connect( ms_clearPushButton, SIGNAL( clicked() ), this, SLOT( clearfields() ) );
    connect( ms_saveasPushButton, SIGNAL( clicked() ), this, SLOT( save_fullstatistics() ) );
    connect( ms_showmethodComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_table() ) );
    connect( ms_showchipComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_table() ) );
    connect( ms_fillLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ms_fillPushButton, SIGNAL( clicked() ), this, SLOT( fill_manualvalue() ) );
    connect( ms_ccdselectionComboBox, SIGNAL( activated(const QString&) ), this, SLOT( showhide_rows() ) );
    connect( ms_cancelPushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( ms_defaultsPushButton, SIGNAL( clicked() ), this, SLOT( defaults() ) );
    connect( ms_modelskyRadioButton, SIGNAL( stateChanged(int) ), this, SLOT( switch_selection() ) );
    connect( ms_manualskyRadioButton, SIGNAL( stateChanged(int) ), this, SLOT( switch_selection() ) );
    connect( ms_xminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ms_xmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ms_yminLineEdit, SIGNAL( selectionChanged() ), this, SLOT( validate() ) );
    connect( ms_ymaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ms_infoPushButton, SIGNAL( clicked() ), this, SLOT( show_info() ) );
    connect( ms_maskexpandfactorLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );

    // tab order
    setTabOrder( ms_skysubdetectthreshLineEdit, ms_skysubdetectminareaLineEdit );
    setTabOrder( ms_skysubdetectminareaLineEdit, ms_skysubbacksizeLineEdit );
    setTabOrder( ms_skysubbacksizeLineEdit, ms_ra1LineEdit );
    setTabOrder( ms_ra1LineEdit, ms_ra2LineEdit );
    setTabOrder( ms_ra2LineEdit, ms_dec1LineEdit );
    setTabOrder( ms_dec1LineEdit, ms_dec2LineEdit );
    setTabOrder( ms_dec2LineEdit, ms_xminLineEdit );
    setTabOrder( ms_xminLineEdit, ms_xmaxLineEdit );
    setTabOrder( ms_xmaxLineEdit, ms_yminLineEdit );
    setTabOrder( ms_yminLineEdit, ms_ymaxLineEdit );
    setTabOrder( ms_ymaxLineEdit, ms_fillLineEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
manualsky::~manualsky()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void manualsky::languageChange()
{
    setCaption( tr( "Configure sky subtraction" ) );
    QWhatsThis::add( this, tr( "<b><font color=\"#0d00ff\">Configure sky subtraction</font></b>\n"
"<p></p>\n"
"<p>2 methods are available:</p>\n"
"\n"
"<b><font color=\"#ff0000\">Model the sky</font></b>\n"
"<p>This is the default method. Objects are detected and removed from the image using <i>SExtractor</i>. You need to provide the detection thresholds and the FWHM of the Gaussian convolution kernel.</p>\n"
"\n"
"<b><font color=\"#ff0000\">Subtract a constant sky</font></b>\n"
"You have two choices:<br>\n"
"<p>First, you can specify a box in the images from which an unbiased constant sky estimate can be obtained. That means, the area selected should be free of any extended faint haloes etc that are not detected by <i>SExtractor</i>. From the pulldown menu, you can choose which statistical measure you want to use: the mean, median, mode or the lower quartile. If you leave the sky box empty, the estimate is obtained from the entire image area.<br><font color=\"#007800\">\n"
"This method still runs the <i>SExtractor</i> object detection and removal step using the given parameters, but no background modelling is performed.</font></p>\n"
"<p>Second, you can override the automatic estimate. To show the values for each image, click on the <b><font color=\"#0000ff\">Get estimates</font></b> button. The table below will be populated with the file names and the sky estimate. If you chose a sky box on the left hand side, the estimates will be taken from the box only. You can again choose between 4 different statistical measures for the background estimate. In case of multiple chip cameras, you can display the values of each chip, or only from a particular chip of the mosaic which you consider representative for the other chips. You can then edit the values in the table manually (must be confirmed with the return key or with a mouse click into a different column). <br>\n"
"Upon leaving this dialog with the OK button, your values are stored automatically. \n"
"<br><font color=\"#007800\">\n"
"Contrary to the first option, this method <font color=\"#ff0000\">DOES NOT</font> run the <i>SExtractor</i> object detection and removal step (you get the table populated instantaneously).</font> </p>" ) );
    ms_defaultsPushButton->setText( tr( "Defaults" ) );
    ms_cancelPushButton->setText( tr( "Cancel" ) );
    ms_closePushButton->setText( tr( "OK" ) );
    ms_buttonGroup->setTitle( QString::null );
    ms_modelskyRadioButton->setText( tr( "Model the sky" ) );
    QToolTip::add( ms_modelskyRadioButton, tr( "The sky is modelled automatically after objects have been removed from the images" ) );
    ms_manualskyRadioButton->setText( tr( "Subtract  a constant sky" ) );
    QToolTip::add( ms_manualskyRadioButton, tr( "A constant sky is subtracted" ) );
    QToolTip::add( ms_frame2, tr( "If you want to determine the sky from a small box only, then enter its coordinates here. This applies to the table to the right as well.<br>" ) );
    textLabel1_3->setText( tr( "Region with empty sky (optional)" ) );
    textLabel3->setText( tr( "RA 1" ) );
    textLabel4_2_3->setText( tr( "RA 2" ) );
    textLabel4_2->setText( tr( "DEC 1" ) );
    textLabel4_2_2->setText( tr( "DEC 2" ) );
    QToolTip::add( ms_dec2LineEdit, tr( "Defines the borders of a region of empty sky" ) );
    QToolTip::add( ms_dec1LineEdit, tr( "Defines the borders of a region of empty sky" ) );
    QToolTip::add( ms_ra2LineEdit, tr( "Defines the borders of a region of empty sky" ) );
    ms_ra1LineEdit->setText( QString::null );
    QToolTip::add( ms_ra1LineEdit, tr( "Defines the borders of a region of empty sky" ) );
    QToolTip::add( ms_xmaxLineEdit, tr( "The right edge of an empty sky region" ) );
    QToolTip::add( ms_yminLineEdit, tr( "The lower edge of an empty sky region" ) );
    QToolTip::add( ms_ymaxLineEdit, tr( "The upper edge of an empty sky region" ) );
    QToolTip::add( ms_xminLineEdit, tr( "The left edge of an empty sky region" ) );
    textLabel4_4->setText( tr( "ymin" ) );
    textLabel4_5->setText( tr( "ymax" ) );
    textLabel4_3->setText( tr( "xmax" ) );
    textLabel4->setText( tr( "xmin" ) );
    textLabel5->setText( tr( "or" ) );
    QToolTip::add( manualsky_table, tr( "This table contains the sky background estimates. You can manually change any of the values listed. <br>" ) );
    ms_fillPushButton->setText( tr( "Fill" ) );
    QToolTip::add( ms_fillPushButton, tr( "Replace all values in the table with this one" ) );
    QToolTip::add( ms_showchipComboBox, tr( "Chose from which chip the estimate is taken" ) );
    ms_ccdselectionComboBox->clear();
    ms_ccdselectionComboBox->insertItem( tr( "Show mosaics only" ) );
    ms_ccdselectionComboBox->insertItem( tr( "Show all chips" ) );
    QToolTip::add( ms_ccdselectionComboBox, tr( "Choose if you want to work on exposures (all chips) or on individual images<br>" ) );
    ms_getvaluesPushButton->setText( tr( "Get estimates" ) );
    QToolTip::add( ms_getvaluesPushButton, tr( "Get a statistical estimate for the sky background" ) );
    ms_loadPushButton->setText( tr( "Load ..." ) );
    QToolTip::add( ms_loadPushButton, tr( "Load a sky estimate determined previously" ) );
    ms_saveasPushButton->setText( tr( "Save ..." ) );
    QToolTip::add( ms_saveasPushButton, tr( "Save the table with the sky estimates" ) );
    ms_clearPushButton->setText( tr( "Clear" ) );
    QToolTip::add( ms_clearPushButton, tr( "Clears the table with the sky estimates" ) );
    ms_showmethodComboBox->clear();
    ms_showmethodComboBox->insertItem( tr( "Show mode" ) );
    ms_showmethodComboBox->insertItem( tr( "Show median" ) );
    ms_showmethodComboBox->insertItem( tr( "Show mean" ) );
    ms_showmethodComboBox->insertItem( tr( "Show lower quartile" ) );
    QToolTip::add( ms_showmethodComboBox, tr( "Shows the mode/median/mean or lowe quartile of the images" ) );
    QToolTip::add( ms_fillLineEdit, tr( "This value will replace all others in the table" ) );
    textLabel2->setText( tr( "Manual override (individual exposures)" ) );
    ms_1GroupBox->setTitle( QString::null );
    QToolTip::add( ms_chipComboBox, tr( "Chose from which chip the estimate is taken" ) );
    textLabel1->setText( tr( "Choose statistics" ) );
    ms_methodComboBox->clear();
    ms_methodComboBox->insertItem( tr( "Mode" ) );
    ms_methodComboBox->insertItem( tr( "Median" ) );
    ms_methodComboBox->insertItem( tr( "Mean" ) );
    ms_methodComboBox->insertItem( tr( "Low quartile" ) );
    QToolTip::add( ms_methodComboBox, tr( "Select a method for the statistical estimate" ) );
    ms_infoPushButton->setText( tr( "Info" ) );
    ms_modelButtonGroup->setTitle( QString::null );
    ms_skysubdetectminareaLineEdit->setText( QString::null );
    QToolTip::add( ms_skysubdetectminareaLineEdit, tr( "Minimum number of connected pixels for an object to be masked." ) );
    ms_skysubdetectthreshLineEdit->setText( QString::null );
    QToolTip::add( ms_skysubdetectthreshLineEdit, tr( "Detection threshold per pixel for an object to be masked." ) );
    cc_textLabel10_4->setText( tr( "DT" ) );
    QToolTip::add( cc_textLabel10_4, QString::null );
    cc_textLabel10_3_2->setText( tr( "Kernel width" ) );
    QToolTip::add( cc_textLabel10_3_2, QString::null );
    cc_textLabel10_2_2->setText( tr( "DMIN" ) );
    QToolTip::add( cc_textLabel10_2_2, QString::null );
    ms_saveskymodelCheckBox->setText( tr( "Save sky model (*.sky.fits)" ) );
    QToolTip::add( ms_saveskymodelCheckBox, tr( "Activate if you want the individual sky models to be saved as a separate images (*sky.fits). Otherwise they will be deleted on the fly.<br>" ) );
    textLabel1_17_3->setText( tr( "Mask expansion factor:" ) );
    ms_skysubbacksizeLineEdit->setText( QString::null );
    QToolTip::add( ms_skysubbacksizeLineEdit, tr( "FWHM of the Gaussian convolution kernel used for sky background modelling, in pixels." ) );
    ms_maskexpandfactorLineEdit->setText( QString::null );
    QToolTip::add( ms_maskexpandfactorLineEdit, tr( "If not empty, pixels within the scaled best-fit SExtractor ellipse will be set to zero. In this way very faint flux invisible in an individual image can be caught as well. Good starting value: 3.0 <br>" ) );
}

