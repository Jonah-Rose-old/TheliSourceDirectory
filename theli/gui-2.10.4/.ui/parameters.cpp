/****************************************************************************
** Form implementation generated from reading ui file 'parameters.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "parameters.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../parameters.ui.h"
/*
 *  Constructs a parameters as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
parameters::parameters( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "parameters" );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    setFocusPolicy( QDialog::NoFocus );
    setSizeGripEnabled( TRUE );
    setModal( TRUE );
    parametersLayout = new QGridLayout( this, 1, 1, 11, 6, "parametersLayout"); 

    param_head2TextLabel = new QLabel( this, "param_head2TextLabel" );
    param_head2TextLabel->setMinimumSize( QSize( 250, 0 ) );

    parametersLayout->addWidget( param_head2TextLabel, 0, 1 );

    param_head1TextLabel = new QLabel( this, "param_head1TextLabel" );

    parametersLayout->addWidget( param_head1TextLabel, 0, 0 );
    spacer137_2 = new QSpacerItem( 20, 375, QSizePolicy::Minimum, QSizePolicy::Expanding );
    parametersLayout->addItem( spacer137_2, 2, 2 );

    layout27 = new QGridLayout( 0, 1, 1, 0, 6, "layout27"); 

    param_defaultsPushButton = new QPushButton( this, "param_defaultsPushButton" );
    param_defaultsPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout27->addMultiCellWidget( param_defaultsPushButton, 4, 4, 0, 2 );
    spacer113 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout27->addItem( spacer113, 3, 0 );

    param_nextPushButton = new QPushButton( this, "param_nextPushButton" );
    param_nextPushButton->setMinimumSize( QSize( 80, 0 ) );
    param_nextPushButton->setMaximumSize( QSize( 80, 32767 ) );
    param_nextPushButton->setPaletteBackgroundColor( QColor( 255, 255, 127 ) );
    param_nextPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout27->addWidget( param_nextPushButton, 2, 2 );

    param_okPushButton = new QPushButton( this, "param_okPushButton" );
    param_okPushButton->setMinimumSize( QSize( 80, 0 ) );
    param_okPushButton->setMaximumSize( QSize( 80, 32767 ) );
    param_okPushButton->setPaletteForegroundColor( QColor( 0, 118, 0 ) );
    param_okPushButton->setFocusPolicy( QPushButton::NoFocus );
    param_okPushButton->setAutoDefault( TRUE );
    param_okPushButton->setDefault( TRUE );

    layout27->addWidget( param_okPushButton, 7, 2 );

    param_cancelPushButton = new QPushButton( this, "param_cancelPushButton" );
    param_cancelPushButton->setMinimumSize( QSize( 80, 0 ) );
    param_cancelPushButton->setMaximumSize( QSize( 80, 32767 ) );
    param_cancelPushButton->setPaletteForegroundColor( QColor( 185, 0, 0 ) );
    param_cancelPushButton->setFocusPolicy( QPushButton::NoFocus );
    param_cancelPushButton->setAutoDefault( TRUE );

    layout27->addMultiCellWidget( param_cancelPushButton, 7, 7, 0, 1 );

    param_backPushButton = new QPushButton( this, "param_backPushButton" );
    param_backPushButton->setMinimumSize( QSize( 80, 0 ) );
    param_backPushButton->setMaximumSize( QSize( 80, 32767 ) );
    param_backPushButton->setPaletteBackgroundColor( QColor( 255, 255, 127 ) );
    param_backPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout27->addMultiCellWidget( param_backPushButton, 2, 2, 0, 1 );
    spacer112 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout27->addItem( spacer112, 6, 0 );

    param_defaultsallPushButton = new QPushButton( this, "param_defaultsallPushButton" );
    param_defaultsallPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout27->addMultiCellWidget( param_defaultsallPushButton, 5, 5, 0, 2 );

    parametersLayout->addLayout( layout27, 1, 2 );

    param_WidgetStack = new QWidgetStack( this, "param_WidgetStack" );
    param_WidgetStack->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, param_WidgetStack->sizePolicy().hasHeightForWidth() ) );
    param_WidgetStack->setFrameShape( QWidgetStack::GroupBoxPanel );
    param_WidgetStack->setFrameShadow( QWidgetStack::Plain );

    PreparationStackPage = new QWidget( param_WidgetStack, "PreparationStackPage" );
    PreparationStackPageLayout = new QGridLayout( PreparationStackPage, 1, 1, 11, 6, "PreparationStackPageLayout"); 
    spacer7 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    PreparationStackPageLayout->addItem( spacer7, 4, 1 );

    textLabel2_7 = new QLabel( PreparationStackPage, "textLabel2_7" );

    PreparationStackPageLayout->addWidget( textLabel2_7, 0, 0 );

    groupBox11 = new QGroupBox( PreparationStackPage, "groupBox11" );
    groupBox11->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox11->setColumnLayout(0, Qt::Vertical );
    groupBox11->layout()->setSpacing( 6 );
    groupBox11->layout()->setMargin( 11 );
    groupBox11Layout = new QGridLayout( groupBox11->layout() );
    groupBox11Layout->setAlignment( Qt::AlignTop );

    layout2 = new QGridLayout( 0, 1, 1, 0, 6, "layout2"); 

    pre_sort_biasLineEdit = new QLineEdit( groupBox11, "pre_sort_biasLineEdit" );
    pre_sort_biasLineEdit->setMinimumSize( QSize( 100, 0 ) );
    pre_sort_biasLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    pre_sort_biasLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout2->addWidget( pre_sort_biasLineEdit, 1, 1 );

    textLabel11_2 = new QLabel( groupBox11, "textLabel11_2" );

    layout2->addWidget( textLabel11_2, 1, 2 );

    pre_sort_darkLineEdit = new QLineEdit( groupBox11, "pre_sort_darkLineEdit" );
    pre_sort_darkLineEdit->setMinimumSize( QSize( 100, 0 ) );
    pre_sort_darkLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    pre_sort_darkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout2->addWidget( pre_sort_darkLineEdit, 2, 1 );

    pre_sort_domeflatLineEdit = new QLineEdit( groupBox11, "pre_sort_domeflatLineEdit" );
    pre_sort_domeflatLineEdit->setMinimumSize( QSize( 100, 0 ) );
    pre_sort_domeflatLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    pre_sort_domeflatLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout2->addWidget( pre_sort_domeflatLineEdit, 0, 3 );

    pre_sort_skyflatLineEdit = new QLineEdit( groupBox11, "pre_sort_skyflatLineEdit" );
    pre_sort_skyflatLineEdit->setMinimumSize( QSize( 100, 0 ) );
    pre_sort_skyflatLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    pre_sort_skyflatLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout2->addWidget( pre_sort_skyflatLineEdit, 1, 3 );

    pre_sort_stdLineEdit = new QLineEdit( groupBox11, "pre_sort_stdLineEdit" );
    pre_sort_stdLineEdit->setMinimumSize( QSize( 100, 0 ) );
    pre_sort_stdLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    pre_sort_stdLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout2->addWidget( pre_sort_stdLineEdit, 2, 3 );

    pre_sort_fitskeyLineEdit = new QLineEdit( groupBox11, "pre_sort_fitskeyLineEdit" );
    pre_sort_fitskeyLineEdit->setMinimumSize( QSize( 100, 0 ) );
    pre_sort_fitskeyLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    pre_sort_fitskeyLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout2->addWidget( pre_sort_fitskeyLineEdit, 0, 1 );

    textLabel10_3 = new QLabel( groupBox11, "textLabel10_3" );

    layout2->addWidget( textLabel10_3, 2, 0 );

    textLabel13_3 = new QLabel( groupBox11, "textLabel13_3" );

    layout2->addWidget( textLabel13_3, 2, 2 );

    textLabel12_3 = new QLabel( groupBox11, "textLabel12_3" );

    layout2->addWidget( textLabel12_3, 0, 2 );

    textLabel9_2 = new QLabel( groupBox11, "textLabel9_2" );

    layout2->addWidget( textLabel9_2, 1, 0 );

    textLabel8_2 = new QLabel( groupBox11, "textLabel8_2" );
    QFont textLabel8_2_font(  textLabel8_2->font() );
    textLabel8_2_font.setBold( TRUE );
    textLabel8_2->setFont( textLabel8_2_font ); 

    layout2->addWidget( textLabel8_2, 0, 0 );

    groupBox11Layout->addLayout( layout2, 0, 0 );

    PreparationStackPageLayout->addWidget( groupBox11, 1, 0 );

    textLabel2 = new QLabel( PreparationStackPage, "textLabel2" );

    PreparationStackPageLayout->addWidget( textLabel2, 2, 0 );
    spacer111 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    PreparationStackPageLayout->addItem( spacer111, 5, 0 );

    groupBox1 = new QGroupBox( PreparationStackPage, "groupBox1" );
    groupBox1->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QGridLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    textLabel3 = new QLabel( groupBox1, "textLabel3" );

    groupBox1Layout->addWidget( textLabel3, 0, 2 );

    xtalk_norCheckBox = new QCheckBox( groupBox1, "xtalk_norCheckBox" );
    xtalk_norCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox1Layout->addMultiCellWidget( xtalk_norCheckBox, 0, 1, 0, 0 );

    xtalk_nor_amplitudeLineEdit = new QLineEdit( groupBox1, "xtalk_nor_amplitudeLineEdit" );
    xtalk_nor_amplitudeLineEdit->setMinimumSize( QSize( 72, 0 ) );
    xtalk_nor_amplitudeLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    xtalk_nor_amplitudeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addWidget( xtalk_nor_amplitudeLineEdit, 1, 2 );
    spacer5 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox1Layout->addItem( spacer5, 2, 2 );

    xtalk_norButtonGroup = new QButtonGroup( groupBox1, "xtalk_norButtonGroup" );
    xtalk_norButtonGroup->setExclusive( TRUE );

    xtalk_nor_2x2ToolButton = new QToolButton( xtalk_norButtonGroup, "xtalk_nor_2x2ToolButton" );
    xtalk_nor_2x2ToolButton->setGeometry( QRect( 11, 11, 49, 49 ) );
    xtalk_nor_2x2ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_nor_2x2ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_nor_2x2ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_nor_2x2ToolButton->setToggleButton( TRUE );

    xtalk_nor_1x2ToolButton = new QToolButton( xtalk_norButtonGroup, "xtalk_nor_1x2ToolButton" );
    xtalk_nor_1x2ToolButton->setGeometry( QRect( 66, 11, 49, 49 ) );
    xtalk_nor_1x2ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_nor_1x2ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_nor_1x2ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_nor_1x2ToolButton->setToggleButton( TRUE );

    xtalk_nor_2x1ToolButton = new QToolButton( xtalk_norButtonGroup, "xtalk_nor_2x1ToolButton" );
    xtalk_nor_2x1ToolButton->setGeometry( QRect( 121, 11, 49, 49 ) );
    xtalk_nor_2x1ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_nor_2x1ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_nor_2x1ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_nor_2x1ToolButton->setToggleButton( TRUE );

    groupBox1Layout->addMultiCellWidget( xtalk_norButtonGroup, 0, 2, 1, 1 );

    textLabel3_2 = new QLabel( groupBox1, "textLabel3_2" );

    groupBox1Layout->addWidget( textLabel3_2, 3, 2 );

    xtalk_row_amplitudeLineEdit = new QLineEdit( groupBox1, "xtalk_row_amplitudeLineEdit" );
    xtalk_row_amplitudeLineEdit->setMinimumSize( QSize( 72, 0 ) );
    xtalk_row_amplitudeLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    xtalk_row_amplitudeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addMultiCellWidget( xtalk_row_amplitudeLineEdit, 4, 5, 2, 2 );
    spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox1Layout->addItem( spacer2, 6, 2 );

    xtalk_rowButtonGroup = new QButtonGroup( groupBox1, "xtalk_rowButtonGroup" );
    xtalk_rowButtonGroup->setExclusive( TRUE );
    xtalk_rowButtonGroup->setColumnLayout(0, Qt::Vertical );
    xtalk_rowButtonGroup->layout()->setSpacing( 6 );
    xtalk_rowButtonGroup->layout()->setMargin( 11 );
    xtalk_rowButtonGroupLayout = new QGridLayout( xtalk_rowButtonGroup->layout() );
    xtalk_rowButtonGroupLayout->setAlignment( Qt::AlignTop );

    xtalk_row_2x2ToolButton = new QToolButton( xtalk_rowButtonGroup, "xtalk_row_2x2ToolButton" );
    xtalk_row_2x2ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_row_2x2ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_row_2x2ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_row_2x2ToolButton->setToggleButton( TRUE );

    xtalk_rowButtonGroupLayout->addWidget( xtalk_row_2x2ToolButton, 0, 0 );

    xtalk_row_1x2ToolButton = new QToolButton( xtalk_rowButtonGroup, "xtalk_row_1x2ToolButton" );
    xtalk_row_1x2ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_row_1x2ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_row_1x2ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_row_1x2ToolButton->setToggleButton( TRUE );

    xtalk_rowButtonGroupLayout->addWidget( xtalk_row_1x2ToolButton, 0, 1 );

    xtalk_row_2x1ToolButton = new QToolButton( xtalk_rowButtonGroup, "xtalk_row_2x1ToolButton" );
    xtalk_row_2x1ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_row_2x1ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_row_2x1ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_row_2x1ToolButton->setToggleButton( TRUE );

    xtalk_rowButtonGroupLayout->addWidget( xtalk_row_2x1ToolButton, 0, 2 );

    xtalk_col_2x2ToolButton = new QToolButton( xtalk_rowButtonGroup, "xtalk_col_2x2ToolButton" );
    xtalk_col_2x2ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_col_2x2ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_col_2x2ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_col_2x2ToolButton->setToggleButton( TRUE );

    xtalk_rowButtonGroupLayout->addWidget( xtalk_col_2x2ToolButton, 1, 0 );

    xtalk_col_1x2ToolButton = new QToolButton( xtalk_rowButtonGroup, "xtalk_col_1x2ToolButton" );
    xtalk_col_1x2ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_col_1x2ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_col_1x2ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_col_1x2ToolButton->setToggleButton( TRUE );

    xtalk_rowButtonGroupLayout->addWidget( xtalk_col_1x2ToolButton, 1, 1 );

    xtalk_col_2x1ToolButton = new QToolButton( xtalk_rowButtonGroup, "xtalk_col_2x1ToolButton" );
    xtalk_col_2x1ToolButton->setMinimumSize( QSize( 49, 49 ) );
    xtalk_col_2x1ToolButton->setMaximumSize( QSize( 49, 49 ) );
    xtalk_col_2x1ToolButton->setFocusPolicy( QToolButton::NoFocus );
    xtalk_col_2x1ToolButton->setToggleButton( TRUE );

    xtalk_rowButtonGroupLayout->addWidget( xtalk_col_2x1ToolButton, 1, 2 );

    groupBox1Layout->addMultiCellWidget( xtalk_rowButtonGroup, 3, 7, 1, 1 );
    spacer2_2_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox1Layout->addMultiCell( spacer2_2_2, 3, 4, 0, 0 );

    xtalk_rowCheckBox = new QCheckBox( groupBox1, "xtalk_rowCheckBox" );
    xtalk_rowCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox1Layout->addMultiCellWidget( xtalk_rowCheckBox, 5, 6, 0, 0 );
    spacer2_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox1Layout->addItem( spacer2_2, 7, 0 );

    xtalk_multiButtonGroup = new QButtonGroup( groupBox1, "xtalk_multiButtonGroup" );
    xtalk_multiButtonGroup->setExclusive( TRUE );
    xtalk_multiButtonGroup->setColumnLayout(0, Qt::Vertical );
    xtalk_multiButtonGroup->layout()->setSpacing( 6 );
    xtalk_multiButtonGroup->layout()->setMargin( 11 );
    xtalk_multiButtonGroupLayout = new QGridLayout( xtalk_multiButtonGroup->layout() );
    xtalk_multiButtonGroupLayout->setAlignment( Qt::AlignTop );

    xtalk_multixToolButton = new QToolButton( xtalk_multiButtonGroup, "xtalk_multixToolButton" );
    xtalk_multixToolButton->setMinimumSize( QSize( 47, 47 ) );
    xtalk_multixToolButton->setMaximumSize( QSize( 47, 47 ) );
    xtalk_multixToolButton->setToggleButton( TRUE );

    xtalk_multiButtonGroupLayout->addWidget( xtalk_multixToolButton, 0, 0 );

    xtalk_multiyToolButton = new QToolButton( xtalk_multiButtonGroup, "xtalk_multiyToolButton" );
    xtalk_multiyToolButton->setMinimumSize( QSize( 47, 47 ) );
    xtalk_multiyToolButton->setMaximumSize( QSize( 47, 47 ) );
    xtalk_multiyToolButton->setToggleButton( TRUE );

    xtalk_multiButtonGroupLayout->addWidget( xtalk_multiyToolButton, 0, 1 );

    groupBox1Layout->addMultiCellWidget( xtalk_multiButtonGroup, 8, 9, 1, 1 );

    xtalk_multiCheckBox = new QCheckBox( groupBox1, "xtalk_multiCheckBox" );
    xtalk_multiCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox1Layout->addMultiCellWidget( xtalk_multiCheckBox, 8, 9, 0, 0 );

    textLabel1_13 = new QLabel( groupBox1, "textLabel1_13" );

    groupBox1Layout->addWidget( textLabel1_13, 8, 2 );

    xtalk_multi_nsectionLineEdit = new QLineEdit( groupBox1, "xtalk_multi_nsectionLineEdit" );
    xtalk_multi_nsectionLineEdit->setMinimumSize( QSize( 72, 0 ) );
    xtalk_multi_nsectionLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    xtalk_multi_nsectionLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox1Layout->addWidget( xtalk_multi_nsectionLineEdit, 9, 2 );

    PreparationStackPageLayout->addWidget( groupBox1, 4, 0 );

    groupBox12 = new QGroupBox( PreparationStackPage, "groupBox12" );
    groupBox12->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox12->setColumnLayout(0, Qt::Vertical );
    groupBox12->layout()->setSpacing( 6 );
    groupBox12->layout()->setMargin( 11 );
    groupBox12Layout = new QGridLayout( groupBox12->layout() );
    groupBox12Layout->setAlignment( Qt::AlignTop );
    spacer98 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox12Layout->addItem( spacer98, 0, 2 );

    pre_rename_fitskeyLineEdit = new QLineEdit( groupBox12, "pre_rename_fitskeyLineEdit" );
    pre_rename_fitskeyLineEdit->setMinimumSize( QSize( 100, 0 ) );
    pre_rename_fitskeyLineEdit->setMaximumSize( QSize( 100, 32767 ) );
    pre_rename_fitskeyLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox12Layout->addWidget( pre_rename_fitskeyLineEdit, 0, 1 );

    pre_renameCheckBox = new QCheckBox( groupBox12, "pre_renameCheckBox" );
    pre_renameCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox12Layout->addWidget( pre_renameCheckBox, 0, 0 );

    PreparationStackPageLayout->addWidget( groupBox12, 3, 0 );
    param_WidgetStack->addWidget( PreparationStackPage, 0 );

    CalibrationStackPage = new QWidget( param_WidgetStack, "CalibrationStackPage" );
    CalibrationStackPageLayout = new QGridLayout( CalibrationStackPage, 1, 1, 11, 6, "CalibrationStackPageLayout"); 

    textLabel4 = new QLabel( CalibrationStackPage, "textLabel4" );

    CalibrationStackPageLayout->addMultiCellWidget( textLabel4, 0, 0, 0, 1 );
    spacer60 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    CalibrationStackPageLayout->addItem( spacer60, 2, 0 );
    spacer14 = new QSpacerItem( 86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    CalibrationStackPageLayout->addItem( spacer14, 1, 1 );

    groupBox3 = new QGroupBox( CalibrationStackPage, "groupBox3" );
    groupBox3->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QGridLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );

    ovscan_nhighLineEdit = new QLineEdit( groupBox3, "ovscan_nhighLineEdit" );
    ovscan_nhighLineEdit->setMinimumSize( QSize( 88, 0 ) );
    ovscan_nhighLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    ovscan_nhighLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addWidget( ovscan_nhighLineEdit, 1, 3 );

    ovscan_nlowLineEdit = new QLineEdit( groupBox3, "ovscan_nlowLineEdit" );
    ovscan_nlowLineEdit->setMinimumSize( QSize( 88, 0 ) );
    ovscan_nlowLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    ovscan_nlowLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addWidget( ovscan_nlowLineEdit, 1, 2 );

    textLabel9 = new QLabel( groupBox3, "textLabel9" );

    groupBox3Layout->addWidget( textLabel9, 0, 2 );

    textLabel5 = new QLabel( groupBox3, "textLabel5" );

    groupBox3Layout->addWidget( textLabel5, 1, 0 );

    textLabel7 = new QLabel( groupBox3, "textLabel7" );

    groupBox3Layout->addWidget( textLabel7, 4, 0 );

    flat_nlowLineEdit = new QLineEdit( groupBox3, "flat_nlowLineEdit" );
    flat_nlowLineEdit->setMinimumSize( QSize( 88, 0 ) );
    flat_nlowLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    flat_nlowLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addWidget( flat_nlowLineEdit, 4, 2 );

    flat_nhighLineEdit = new QLineEdit( groupBox3, "flat_nhighLineEdit" );
    flat_nhighLineEdit->setMinimumSize( QSize( 88, 0 ) );
    flat_nhighLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    flat_nhighLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addWidget( flat_nhighLineEdit, 4, 3 );
    spacer10_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox3Layout->addItem( spacer10_3, 4, 1 );

    bias_nlowLineEdit = new QLineEdit( groupBox3, "bias_nlowLineEdit" );
    bias_nlowLineEdit->setMinimumSize( QSize( 88, 0 ) );
    bias_nlowLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    bias_nlowLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addWidget( bias_nlowLineEdit, 2, 2 );

    bias_nhighLineEdit = new QLineEdit( groupBox3, "bias_nhighLineEdit" );
    bias_nhighLineEdit->setMinimumSize( QSize( 88, 0 ) );
    bias_nhighLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    bias_nhighLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addWidget( bias_nhighLineEdit, 2, 3 );

    textLabel6 = new QLabel( groupBox3, "textLabel6" );

    groupBox3Layout->addWidget( textLabel6, 2, 0 );

    textLabel6_4 = new QLabel( groupBox3, "textLabel6_4" );

    groupBox3Layout->addWidget( textLabel6_4, 3, 0 );

    dark_nlowLineEdit = new QLineEdit( groupBox3, "dark_nlowLineEdit" );
    dark_nlowLineEdit->setMinimumSize( QSize( 88, 0 ) );
    dark_nlowLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    dark_nlowLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addWidget( dark_nlowLineEdit, 3, 2 );

    dark_nhighLineEdit = new QLineEdit( groupBox3, "dark_nhighLineEdit" );
    dark_nhighLineEdit->setMinimumSize( QSize( 88, 0 ) );
    dark_nhighLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    dark_nhighLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox3Layout->addWidget( dark_nhighLineEdit, 3, 3 );

    textLabel10 = new QLabel( groupBox3, "textLabel10" );

    groupBox3Layout->addWidget( textLabel10, 0, 3 );
    spacer10 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox3Layout->addItem( spacer10, 1, 1 );
    spacer10_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox3Layout->addItem( spacer10_2, 2, 1 );
    spacer10_2_3 = new QSpacerItem( 108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox3Layout->addItem( spacer10_2_3, 3, 1 );

    CalibrationStackPageLayout->addWidget( groupBox3, 1, 0 );
    spacer58 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    CalibrationStackPageLayout->addItem( spacer58, 4, 1 );
    spacer58_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    CalibrationStackPageLayout->addItem( spacer58_3, 5, 0 );
    param_WidgetStack->addWidget( CalibrationStackPage, 1 );

    BackgroundStackPage = new QWidget( param_WidgetStack, "BackgroundStackPage" );
    BackgroundStackPageLayout = new QGridLayout( BackgroundStackPage, 1, 1, 11, 6, "BackgroundStackPageLayout"); 

    textLabel2_2 = new QLabel( BackgroundStackPage, "textLabel2_2" );

    BackgroundStackPageLayout->addMultiCellWidget( textLabel2_2, 0, 0, 0, 1 );
    spacer101 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    BackgroundStackPageLayout->addItem( spacer101, 1, 1 );

    groupBox4 = new QGroupBox( BackgroundStackPage, "groupBox4" );
    groupBox4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, groupBox4->sizePolicy().hasHeightForWidth() ) );
    groupBox4->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox4->setColumnLayout(0, Qt::Vertical );
    groupBox4->layout()->setSpacing( 6 );
    groupBox4->layout()->setMargin( 11 );
    groupBox4Layout = new QGridLayout( groupBox4->layout() );
    groupBox4Layout->setAlignment( Qt::AlignTop );

    textLabel2_11_3_2 = new QLabel( groupBox4, "textLabel2_11_3_2" );

    groupBox4Layout->addMultiCellWidget( textLabel2_11_3_2, 0, 0, 0, 2 );
    spacer105_5_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox4Layout->addItem( spacer105_5_2, 5, 0 );

    sf_back_static_dynamicTextlabel = new QLabel( groupBox4, "sf_back_static_dynamicTextlabel" );

    groupBox4Layout->addWidget( sf_back_static_dynamicTextlabel, 12, 3 );
    spacer105_5_2_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox4Layout->addItem( spacer105_5_2_2, 8, 1 );

    cal_windowsizeTextLabel_2 = new QLabel( groupBox4, "cal_windowsizeTextLabel_2" );

    groupBox4Layout->addMultiCellWidget( cal_windowsizeTextLabel_2, 13, 13, 0, 1 );

    textLabel1_17_2_4 = new QLabel( groupBox4, "textLabel1_17_2_4" );

    groupBox4Layout->addMultiCellWidget( textLabel1_17_2_4, 6, 6, 0, 3 );

    layout11 = new QGridLayout( 0, 1, 1, 0, 6, "layout11"); 

    textLabel2_13_2_2 = new QLabel( groupBox4, "textLabel2_13_2_2" );

    layout11->addWidget( textLabel2_13_2_2, 0, 2 );

    textLabel2_13_3_4 = new QLabel( groupBox4, "textLabel2_13_3_4" );

    layout11->addWidget( textLabel2_13_3_4, 0, 1 );

    sf_back_nhigh2LineEdit = new QLineEdit( groupBox4, "sf_back_nhigh2LineEdit" );
    sf_back_nhigh2LineEdit->setMinimumSize( QSize( 44, 0 ) );
    sf_back_nhigh2LineEdit->setMaximumSize( QSize( 44, 32767 ) );
    sf_back_nhigh2LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout11->addWidget( sf_back_nhigh2LineEdit, 2, 2 );

    sf_back_nlow1LineEdit = new QLineEdit( groupBox4, "sf_back_nlow1LineEdit" );
    sf_back_nlow1LineEdit->setMinimumSize( QSize( 44, 0 ) );
    sf_back_nlow1LineEdit->setMaximumSize( QSize( 44, 32767 ) );
    sf_back_nlow1LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout11->addWidget( sf_back_nlow1LineEdit, 1, 1 );

    sf_back_nhigh1LineEdit = new QLineEdit( groupBox4, "sf_back_nhigh1LineEdit" );
    sf_back_nhigh1LineEdit->setMinimumSize( QSize( 44, 0 ) );
    sf_back_nhigh1LineEdit->setMaximumSize( QSize( 44, 32767 ) );
    sf_back_nhigh1LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout11->addWidget( sf_back_nhigh1LineEdit, 1, 2 );

    sf_back_twopass2TextLabel = new QLabel( groupBox4, "sf_back_twopass2TextLabel" );

    layout11->addWidget( sf_back_twopass2TextLabel, 2, 0 );

    sf_back_twopass1TextLabel = new QLabel( groupBox4, "sf_back_twopass1TextLabel" );

    layout11->addWidget( sf_back_twopass1TextLabel, 1, 0 );

    sf_back_nlow2LineEdit = new QLineEdit( groupBox4, "sf_back_nlow2LineEdit" );
    sf_back_nlow2LineEdit->setMinimumSize( QSize( 44, 0 ) );
    sf_back_nlow2LineEdit->setMaximumSize( QSize( 44, 32767 ) );
    sf_back_nlow2LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout11->addWidget( sf_back_nlow2LineEdit, 2, 1 );

    groupBox4Layout->addMultiCellLayout( layout11, 4, 4, 0, 2 );

    layout26 = new QHBoxLayout( 0, 0, 6, "layout26"); 

    textLabel2_13_3_2 = new QLabel( groupBox4, "textLabel2_13_3_2" );
    layout26->addWidget( textLabel2_13_3_2 );

    sf_back_maglimitLineEdit = new QLineEdit( groupBox4, "sf_back_maglimitLineEdit" );
    sf_back_maglimitLineEdit->setMinimumSize( QSize( 50, 0 ) );
    sf_back_maglimitLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    sf_back_maglimitLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout26->addWidget( sf_back_maglimitLineEdit );

    textLabel2_13_3_2_3 = new QLabel( groupBox4, "textLabel2_13_3_2_3" );
    layout26->addWidget( textLabel2_13_3_2_3 );

    sf_back_distanceLineEdit = new QLineEdit( groupBox4, "sf_back_distanceLineEdit" );
    sf_back_distanceLineEdit->setMinimumSize( QSize( 50, 0 ) );
    sf_back_distanceLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    sf_back_distanceLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout26->addWidget( sf_back_distanceLineEdit );

    textLabel2_13_3_2_2 = new QLabel( groupBox4, "textLabel2_13_3_2_2" );
    layout26->addWidget( textLabel2_13_3_2_2 );

    sf_back_serverComboBox = new QComboBox( FALSE, groupBox4, "sf_back_serverComboBox" );
    sf_back_serverComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, sf_back_serverComboBox->sizePolicy().hasHeightForWidth() ) );
    sf_back_serverComboBox->setMinimumSize( QSize( 100, 0 ) );
    sf_back_serverComboBox->setMaximumSize( QSize( 100, 32767 ) );
    sf_back_serverComboBox->setFocusPolicy( QComboBox::NoFocus );
    sf_back_serverComboBox->setSizeLimit( 11 );
    layout26->addWidget( sf_back_serverComboBox );

    groupBox4Layout->addMultiCellLayout( layout26, 7, 7, 0, 3 );

    textLabel1_17_2_3 = new QLabel( groupBox4, "textLabel1_17_2_3" );

    groupBox4Layout->addMultiCellWidget( textLabel1_17_2_3, 9, 9, 0, 3 );

    cal_windowsizeTextLabel = new QLabel( groupBox4, "cal_windowsizeTextLabel" );

    groupBox4Layout->addWidget( cal_windowsizeTextLabel, 12, 0 );

    sf_back_gapsizeLineEdit = new QLineEdit( groupBox4, "sf_back_gapsizeLineEdit" );
    sf_back_gapsizeLineEdit->setMinimumSize( QSize( 88, 0 ) );
    sf_back_gapsizeLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    sf_back_gapsizeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox4Layout->addWidget( sf_back_gapsizeLineEdit, 13, 2 );

    textLabel1_17_2_2 = new QLabel( groupBox4, "textLabel1_17_2_2" );

    groupBox4Layout->addMultiCellWidget( textLabel1_17_2_2, 11, 11, 0, 3 );

    sf_back_windowsizeLineEdit = new QLineEdit( groupBox4, "sf_back_windowsizeLineEdit" );
    sf_back_windowsizeLineEdit->setMinimumSize( QSize( 88, 0 ) );
    sf_back_windowsizeLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    sf_back_windowsizeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox4Layout->addWidget( sf_back_windowsizeLineEdit, 12, 2 );
    spacer105_4 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox4Layout->addItem( spacer105_4, 2, 1 );

    layout16 = new QGridLayout( 0, 1, 1, 0, 6, "layout16"); 

    sf_back_twopassCheckBox = new QCheckBox( groupBox4, "sf_back_twopassCheckBox" );
    sf_back_twopassCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    layout16->addMultiCellWidget( sf_back_twopassCheckBox, 0, 0, 0, 2 );

    sf_back_sexfilterCheckBox = new QCheckBox( groupBox4, "sf_back_sexfilterCheckBox" );
    sf_back_sexfilterCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    layout16->addMultiCellWidget( sf_back_sexfilterCheckBox, 1, 1, 0, 2 );

    sf_back_detectminareaLineEdit = new QLineEdit( groupBox4, "sf_back_detectminareaLineEdit" );
    sf_back_detectminareaLineEdit->setMinimumSize( QSize( 60, 0 ) );
    sf_back_detectminareaLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    sf_back_detectminareaLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    sf_back_detectminareaLineEdit->setMaxLength( 9 );
    sf_back_detectminareaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout16->addWidget( sf_back_detectminareaLineEdit, 3, 1 );

    sf_back_detectthreshLineEdit = new QLineEdit( groupBox4, "sf_back_detectthreshLineEdit" );
    sf_back_detectthreshLineEdit->setMinimumSize( QSize( 60, 0 ) );
    sf_back_detectthreshLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    sf_back_detectthreshLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    sf_back_detectthreshLineEdit->setMaxLength( 9 );
    sf_back_detectthreshLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout16->addWidget( sf_back_detectthreshLineEdit, 2, 1 );

    textLabel1_17 = new QLabel( groupBox4, "textLabel1_17" );

    layout16->addWidget( textLabel1_17, 3, 2 );

    textLabel1_17_4 = new QLabel( groupBox4, "textLabel1_17_4" );

    layout16->addWidget( textLabel1_17_4, 2, 2 );

    cal_detectthreshTextLabel = new QLabel( groupBox4, "cal_detectthreshTextLabel" );
    cal_detectthreshTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, cal_detectthreshTextLabel->sizePolicy().hasHeightForWidth() ) );

    layout16->addWidget( cal_detectthreshTextLabel, 2, 0 );

    cal_detectminareaTextLabel = new QLabel( groupBox4, "cal_detectminareaTextLabel" );
    cal_detectminareaTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, cal_detectminareaTextLabel->sizePolicy().hasHeightForWidth() ) );

    layout16->addWidget( cal_detectminareaTextLabel, 3, 0 );

    sf_back_maskexpandfactorLineEdit = new QLineEdit( groupBox4, "sf_back_maskexpandfactorLineEdit" );
    sf_back_maskexpandfactorLineEdit->setMinimumSize( QSize( 60, 0 ) );
    sf_back_maskexpandfactorLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    sf_back_maskexpandfactorLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    sf_back_maskexpandfactorLineEdit->setMaxLength( 9 );
    sf_back_maskexpandfactorLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout16->addWidget( sf_back_maskexpandfactorLineEdit, 3, 3 );

    sf_back_combinemethodComboBox = new QComboBox( FALSE, groupBox4, "sf_back_combinemethodComboBox" );
    sf_back_combinemethodComboBox->setMinimumSize( QSize( 100, 0 ) );
    sf_back_combinemethodComboBox->setMaximumSize( QSize( 100, 32767 ) );
    sf_back_combinemethodComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout16->addWidget( sf_back_combinemethodComboBox, 2, 3 );

    groupBox4Layout->addMultiCellLayout( layout16, 1, 1, 0, 3 );

    textLabel1_17_2 = new QLabel( groupBox4, "textLabel1_17_2" );

    groupBox4Layout->addMultiCellWidget( textLabel1_17_2, 3, 3, 0, 3 );

    layout13 = new QGridLayout( 0, 1, 1, 0, 6, "layout13"); 

    sf_back_applymodeComboBox = new QComboBox( FALSE, groupBox4, "sf_back_applymodeComboBox" );

    layout13->addMultiCellWidget( sf_back_applymodeComboBox, 0, 0, 0, 1 );

    sf_back_fringesmoothLineEdit = new QLineEdit( groupBox4, "sf_back_fringesmoothLineEdit" );
    sf_back_fringesmoothLineEdit->setMinimumSize( QSize( 88, 0 ) );
    sf_back_fringesmoothLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    sf_back_fringesmoothLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    sf_back_fringesmoothLineEdit->setMaxLength( 9 );
    sf_back_fringesmoothLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout13->addWidget( sf_back_fringesmoothLineEdit, 2, 1 );

    sf_back_adjustgainsCheckBox = new QCheckBox( groupBox4, "sf_back_adjustgainsCheckBox" );
    sf_back_adjustgainsCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, sf_back_adjustgainsCheckBox->sizePolicy().hasHeightForWidth() ) );
    sf_back_adjustgainsCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    layout13->addMultiCellWidget( sf_back_adjustgainsCheckBox, 4, 4, 0, 1 );

    sf_back_fringescaleCheckBox = new QCheckBox( groupBox4, "sf_back_fringescaleCheckBox" );
    sf_back_fringescaleCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, sf_back_fringescaleCheckBox->sizePolicy().hasHeightForWidth() ) );
    sf_back_fringescaleCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    layout13->addMultiCellWidget( sf_back_fringescaleCheckBox, 3, 3, 0, 1 );

    sf_back_illumsmoothLineEdit = new QLineEdit( groupBox4, "sf_back_illumsmoothLineEdit" );
    sf_back_illumsmoothLineEdit->setMinimumSize( QSize( 88, 0 ) );
    sf_back_illumsmoothLineEdit->setMaximumSize( QSize( 88, 32767 ) );
    sf_back_illumsmoothLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    sf_back_illumsmoothLineEdit->setMaxLength( 9 );
    sf_back_illumsmoothLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout13->addWidget( sf_back_illumsmoothLineEdit, 1, 1 );

    sf_textLabel1 = new QLabel( groupBox4, "sf_textLabel1" );

    layout13->addWidget( sf_textLabel1, 1, 0 );

    sf_textLabel1_2 = new QLabel( groupBox4, "sf_textLabel1_2" );

    layout13->addWidget( sf_textLabel1_2, 2, 0 );

    groupBox4Layout->addMultiCellLayout( layout13, 10, 10, 0, 3 );
    spacer103_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox4Layout->addItem( spacer103_2, 14, 2 );
    spacer104_2 = new QSpacerItem( 70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox4Layout->addItem( spacer104_2, 4, 3 );

    BackgroundStackPageLayout->addWidget( groupBox4, 1, 0 );
    param_WidgetStack->addWidget( BackgroundStackPage, 2 );

    Background1StackPage = new QWidget( param_WidgetStack, "Background1StackPage" );
    Background1StackPageLayout = new QGridLayout( Background1StackPage, 1, 1, 11, 6, "Background1StackPageLayout"); 
    spacer85 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Background1StackPageLayout->addItem( spacer85, 1, 1 );

    textLabel2_2_2 = new QLabel( Background1StackPage, "textLabel2_2_2" );

    Background1StackPageLayout->addWidget( textLabel2_2_2, 0, 0 );
    spacer86 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Background1StackPageLayout->addItem( spacer86, 6, 0 );

    collapseregion_groupbox = new QGroupBox( Background1StackPage, "collapseregion_groupbox" );
    collapseregion_groupbox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    collapseregion_groupbox->setColumnLayout(0, Qt::Vertical );
    collapseregion_groupbox->layout()->setSpacing( 6 );
    collapseregion_groupbox->layout()->setMargin( 11 );
    collapseregion_groupboxLayout = new QGridLayout( collapseregion_groupbox->layout() );
    collapseregion_groupboxLayout->setAlignment( Qt::AlignTop );

    ccc_collymaxLineEdit = new QLineEdit( collapseregion_groupbox, "ccc_collymaxLineEdit" );
    ccc_collymaxLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ccc_collymaxLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ccc_collymaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    collapseregion_groupboxLayout->addWidget( ccc_collymaxLineEdit, 2, 4 );

    ccc_collxmaxLineEdit = new QLineEdit( collapseregion_groupbox, "ccc_collxmaxLineEdit" );
    ccc_collxmaxLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ccc_collxmaxLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ccc_collxmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    collapseregion_groupboxLayout->addWidget( ccc_collxmaxLineEdit, 1, 4 );

    textLabel4_2 = new QLabel( collapseregion_groupbox, "textLabel4_2" );

    collapseregion_groupboxLayout->addWidget( textLabel4_2, 1, 2 );

    textLabel5_2 = new QLabel( collapseregion_groupbox, "textLabel5_2" );

    collapseregion_groupboxLayout->addWidget( textLabel5_2, 2, 2 );

    textLabel6_2 = new QLabel( collapseregion_groupbox, "textLabel6_2" );

    collapseregion_groupboxLayout->addMultiCellWidget( textLabel6_2, 0, 0, 0, 2 );
    spacer15_3 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    collapseregion_groupboxLayout->addItem( spacer15_3, 1, 1 );
    spacer15 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    collapseregion_groupboxLayout->addItem( spacer15, 2, 1 );

    ccc_collxminLineEdit = new QLineEdit( collapseregion_groupbox, "ccc_collxminLineEdit" );
    ccc_collxminLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ccc_collxminLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ccc_collxminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    collapseregion_groupboxLayout->addWidget( ccc_collxminLineEdit, 1, 3 );

    ccc_collyminLineEdit = new QLineEdit( collapseregion_groupbox, "ccc_collyminLineEdit" );
    ccc_collyminLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ccc_collyminLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ccc_collyminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    collapseregion_groupboxLayout->addWidget( ccc_collyminLineEdit, 2, 3 );

    ccc_excludePixmapLabel = new QLabel( collapseregion_groupbox, "ccc_excludePixmapLabel" );
    ccc_excludePixmapLabel->setPixmap( QPixmap::fromMimeSource( "collapse.png" ) );
    ccc_excludePixmapLabel->setScaledContents( TRUE );

    collapseregion_groupboxLayout->addMultiCellWidget( ccc_excludePixmapLabel, 1, 2, 0, 0 );

    Background1StackPageLayout->addWidget( collapseregion_groupbox, 5, 0 );
    spacer84_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Background1StackPageLayout->addItem( spacer84_2, 5, 1 );
    spacer88_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    Background1StackPageLayout->addItem( spacer88_2, 4, 0 );

    groupBox17 = new QGroupBox( Background1StackPage, "groupBox17" );
    groupBox17->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox17->setFrameShadow( QGroupBox::Sunken );
    groupBox17->setColumnLayout(0, Qt::Vertical );
    groupBox17->layout()->setSpacing( 6 );
    groupBox17->layout()->setMargin( 11 );
    groupBox17Layout = new QGridLayout( groupBox17->layout() );
    groupBox17Layout->setAlignment( Qt::AlignTop );
    spacer22 = new QSpacerItem( 55, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox17Layout->addItem( spacer22, 2, 4 );

    textLabel3_3 = new QLabel( groupBox17, "textLabel3_3" );

    groupBox17Layout->addWidget( textLabel3_3, 1, 0 );
    spacer15_5 = new QSpacerItem( 82, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox17Layout->addItem( spacer15_5, 1, 1 );
    spacer23 = new QSpacerItem( 105, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox17Layout->addMultiCell( spacer23, 1, 1, 3, 4 );
    spacer15_6 = new QSpacerItem( 82, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox17Layout->addItem( spacer15_6, 2, 1 );

    ccc_colldirPixmapLabel = new QLabel( groupBox17, "ccc_colldirPixmapLabel" );
    ccc_colldirPixmapLabel->setPixmap( QPixmap::fromMimeSource( "collapse_x.png" ) );
    ccc_colldirPixmapLabel->setScaledContents( TRUE );

    groupBox17Layout->addWidget( ccc_colldirPixmapLabel, 2, 3 );

    textLabel2_3 = new QLabel( groupBox17, "textLabel2_3" );

    groupBox17Layout->addWidget( textLabel2_3, 2, 0 );

    textLabel1_16_3 = new QLabel( groupBox17, "textLabel1_16_3" );

    groupBox17Layout->addMultiCellWidget( textLabel1_16_3, 0, 0, 0, 1 );

    ccc_collrejectthreshLineEdit = new QLineEdit( groupBox17, "ccc_collrejectthreshLineEdit" );
    ccc_collrejectthreshLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ccc_collrejectthreshLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ccc_collrejectthreshLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox17Layout->addWidget( ccc_collrejectthreshLineEdit, 1, 2 );

    ccc_colldirComboBox = new QComboBox( FALSE, groupBox17, "ccc_colldirComboBox" );
    ccc_colldirComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox17Layout->addWidget( ccc_colldirComboBox, 2, 2 );

    Background1StackPageLayout->addWidget( groupBox17, 3, 0 );
    spacer84 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Background1StackPageLayout->addItem( spacer84, 3, 1 );
    spacer88 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    Background1StackPageLayout->addItem( spacer88, 2, 0 );

    groupBox23 = new QGroupBox( Background1StackPage, "groupBox23" );
    groupBox23->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox23->setColumnLayout(0, Qt::Vertical );
    groupBox23->layout()->setSpacing( 6 );
    groupBox23->layout()->setMargin( 11 );
    groupBox23Layout = new QGridLayout( groupBox23->layout() );
    groupBox23Layout->setAlignment( Qt::AlignTop );
    spacer15_7 = new QSpacerItem( 171, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox23Layout->addItem( spacer15_7, 1, 1 );

    textLabel1_2 = new QLabel( groupBox23, "textLabel1_2" );

    groupBox23Layout->addWidget( textLabel1_2, 1, 0 );

    ccc_colldetectminareaLineEdit = new QLineEdit( groupBox23, "ccc_colldetectminareaLineEdit" );
    ccc_colldetectminareaLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ccc_colldetectminareaLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ccc_colldetectminareaLineEdit->setMouseTracking( TRUE );
    ccc_colldetectminareaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox23Layout->addWidget( ccc_colldetectminareaLineEdit, 1, 3 );

    textLabel2_11 = new QLabel( groupBox23, "textLabel2_11" );

    groupBox23Layout->addMultiCellWidget( textLabel2_11, 0, 0, 0, 3 );

    ccc_collautothresholdCheckBox = new QCheckBox( groupBox23, "ccc_collautothresholdCheckBox" );
    ccc_collautothresholdCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox23Layout->addMultiCellWidget( ccc_collautothresholdCheckBox, 4, 4, 0, 2 );

    textLabel1_17_3 = new QLabel( groupBox23, "textLabel1_17_3" );

    groupBox23Layout->addMultiCellWidget( textLabel1_17_3, 2, 2, 0, 1 );

    ccc_colldetectthreshLineEdit = new QLineEdit( groupBox23, "ccc_colldetectthreshLineEdit" );
    ccc_colldetectthreshLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ccc_colldetectthreshLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ccc_colldetectthreshLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox23Layout->addWidget( ccc_colldetectthreshLineEdit, 1, 2 );

    ccc_maskactionCheckBox = new QCheckBox( groupBox23, "ccc_maskactionCheckBox" );
    ccc_maskactionCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox23Layout->addMultiCellWidget( ccc_maskactionCheckBox, 3, 3, 0, 2 );

    ccc_collmaskexpandfactorLineEdit = new QLineEdit( groupBox23, "ccc_collmaskexpandfactorLineEdit" );
    ccc_collmaskexpandfactorLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ccc_collmaskexpandfactorLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ccc_collmaskexpandfactorLineEdit->setFocusPolicy( QLineEdit::StrongFocus );
    ccc_collmaskexpandfactorLineEdit->setMaxLength( 9 );
    ccc_collmaskexpandfactorLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox23Layout->addWidget( ccc_collmaskexpandfactorLineEdit, 2, 2 );

    Background1StackPageLayout->addWidget( groupBox23, 1, 0 );
    param_WidgetStack->addWidget( Background1StackPage, 3 );

    Weights1StackPage = new QWidget( param_WidgetStack, "Weights1StackPage" );
    Weights1StackPageLayout = new QGridLayout( Weights1StackPage, 1, 1, 11, 6, "Weights1StackPageLayout"); 

    textLabel2_2_2_2_3 = new QLabel( Weights1StackPage, "textLabel2_2_2_2_3" );

    Weights1StackPageLayout->addWidget( textLabel2_2_2_2_3, 0, 0 );
    spacer202 = new QSpacerItem( 97, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Weights1StackPageLayout->addItem( spacer202, 1, 1 );
    spacer94 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Weights1StackPageLayout->addItem( spacer94, 3, 1 );

    groupBox30 = new QGroupBox( Weights1StackPage, "groupBox30" );
    groupBox30->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox30->setColumnLayout(0, Qt::Vertical );
    groupBox30->layout()->setSpacing( 6 );
    groupBox30->layout()->setMargin( 11 );
    groupBox30Layout = new QGridLayout( groupBox30->layout() );
    groupBox30Layout->setAlignment( Qt::AlignTop );

    textLabel1_8 = new QLabel( groupBox30, "textLabel1_8" );

    groupBox30Layout->addWidget( textLabel1_8, 0, 0 );

    w_binsizeSpinBox = new QSpinBox( groupBox30, "w_binsizeSpinBox" );
    w_binsizeSpinBox->setFocusPolicy( QSpinBox::StrongFocus );
    w_binsizeSpinBox->setMinValue( 1 );
    w_binsizeSpinBox->setValue( 4 );

    groupBox30Layout->addWidget( w_binsizeSpinBox, 0, 1 );

    w_binmaxLineEdit = new QLineEdit( groupBox30, "w_binmaxLineEdit" );
    w_binmaxLineEdit->setMinimumSize( QSize( 80, 0 ) );
    w_binmaxLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    w_binmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox30Layout->addWidget( w_binmaxLineEdit, 2, 1 );

    w_binminLineEdit = new QLineEdit( groupBox30, "w_binminLineEdit" );
    w_binminLineEdit->setMinimumSize( QSize( 80, 0 ) );
    w_binminLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    w_binminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox30Layout->addWidget( w_binminLineEdit, 1, 1 );

    textLabel3_7 = new QLabel( groupBox30, "textLabel3_7" );

    groupBox30Layout->addWidget( textLabel3_7, 2, 0 );

    textLabel2_9 = new QLabel( groupBox30, "textLabel2_9" );

    groupBox30Layout->addWidget( textLabel2_9, 1, 0 );

    w_binoutlierCheckBox = new QCheckBox( groupBox30, "w_binoutlierCheckBox" );
    w_binoutlierCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox30Layout->addWidget( w_binoutlierCheckBox, 0, 3 );
    spacer201 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox30Layout->addItem( spacer201, 1, 2 );

    Weights1StackPageLayout->addWidget( groupBox30, 1, 0 );
    spacer90 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Weights1StackPageLayout->addItem( spacer90, 5, 0 );

    defectdetectionGroupBox = new QGroupBox( Weights1StackPage, "defectdetectionGroupBox" );
    defectdetectionGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    defectdetectionGroupBox->setColumnLayout(0, Qt::Vertical );
    defectdetectionGroupBox->layout()->setSpacing( 6 );
    defectdetectionGroupBox->layout()->setMargin( 11 );
    defectdetectionGroupBoxLayout = new QGridLayout( defectdetectionGroupBox->layout() );
    defectdetectionGroupBoxLayout->setAlignment( Qt::AlignTop );

    textLabel1_5 = new QLabel( defectdetectionGroupBox, "textLabel1_5" );
    QFont textLabel1_5_font(  textLabel1_5->font() );
    textLabel1_5_font.setBold( TRUE );
    textLabel1_5->setFont( textLabel1_5_font ); 

    defectdetectionGroupBoxLayout->addWidget( textLabel1_5, 0, 0 );

    textLabel2_6 = new QLabel( defectdetectionGroupBox, "textLabel2_6" );

    defectdetectionGroupBoxLayout->addWidget( textLabel2_6, 1, 0 );

    textLabel3_6 = new QLabel( defectdetectionGroupBox, "textLabel3_6" );

    defectdetectionGroupBoxLayout->addWidget( textLabel3_6, 2, 0 );

    textLabel4_4 = new QLabel( defectdetectionGroupBox, "textLabel4_4" );

    defectdetectionGroupBoxLayout->addWidget( textLabel4_4, 3, 0 );

    textLabel5_5 = new QLabel( defectdetectionGroupBox, "textLabel5_5" );

    defectdetectionGroupBoxLayout->addWidget( textLabel5_5, 4, 0 );

    defect_clustol_sfLineEdit = new QLineEdit( defectdetectionGroupBox, "defect_clustol_sfLineEdit" );
    defect_clustol_sfLineEdit->setMinimumSize( QSize( 72, 0 ) );
    defect_clustol_sfLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    defect_clustol_sfLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    defectdetectionGroupBoxLayout->addWidget( defect_clustol_sfLineEdit, 4, 2 );

    defect_kernelsize_sfLineEdit = new QLineEdit( defectdetectionGroupBox, "defect_kernelsize_sfLineEdit" );
    defect_kernelsize_sfLineEdit->setMinimumSize( QSize( 72, 0 ) );
    defect_kernelsize_sfLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    defect_kernelsize_sfLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    defectdetectionGroupBoxLayout->addWidget( defect_kernelsize_sfLineEdit, 1, 2 );

    defect_coltolLineEdit = new QLineEdit( defectdetectionGroupBox, "defect_coltolLineEdit" );
    defect_coltolLineEdit->setMinimumSize( QSize( 72, 0 ) );
    defect_coltolLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    defect_coltolLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    defectdetectionGroupBoxLayout->addWidget( defect_coltolLineEdit, 3, 1 );

    defect_rowtol_sfLineEdit = new QLineEdit( defectdetectionGroupBox, "defect_rowtol_sfLineEdit" );
    defect_rowtol_sfLineEdit->setMinimumSize( QSize( 72, 0 ) );
    defect_rowtol_sfLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    defect_rowtol_sfLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    defectdetectionGroupBoxLayout->addWidget( defect_rowtol_sfLineEdit, 2, 2 );

    textLabel1_15 = new QLabel( defectdetectionGroupBox, "textLabel1_15" );

    defectdetectionGroupBoxLayout->addWidget( textLabel1_15, 0, 1 );

    defect_clustolLineEdit = new QLineEdit( defectdetectionGroupBox, "defect_clustolLineEdit" );
    defect_clustolLineEdit->setMinimumSize( QSize( 72, 0 ) );
    defect_clustolLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    defect_clustolLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    defectdetectionGroupBoxLayout->addWidget( defect_clustolLineEdit, 4, 1 );

    defect_coltol_sfLineEdit = new QLineEdit( defectdetectionGroupBox, "defect_coltol_sfLineEdit" );
    defect_coltol_sfLineEdit->setMinimumSize( QSize( 72, 0 ) );
    defect_coltol_sfLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    defect_coltol_sfLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    defectdetectionGroupBoxLayout->addWidget( defect_coltol_sfLineEdit, 3, 2 );

    defect_kernelsizeLineEdit = new QLineEdit( defectdetectionGroupBox, "defect_kernelsizeLineEdit" );
    defect_kernelsizeLineEdit->setMinimumSize( QSize( 72, 0 ) );
    defect_kernelsizeLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    defect_kernelsizeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    defectdetectionGroupBoxLayout->addWidget( defect_kernelsizeLineEdit, 1, 1 );

    defect_rowtolLineEdit = new QLineEdit( defectdetectionGroupBox, "defect_rowtolLineEdit" );
    defect_rowtolLineEdit->setMinimumSize( QSize( 72, 0 ) );
    defect_rowtolLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    defect_rowtolLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    defectdetectionGroupBoxLayout->addWidget( defect_rowtolLineEdit, 2, 1 );

    textLabel2_12 = new QLabel( defectdetectionGroupBox, "textLabel2_12" );

    defectdetectionGroupBoxLayout->addWidget( textLabel2_12, 0, 2 );

    Weights1StackPageLayout->addWidget( defectdetectionGroupBox, 4, 0 );

    textLabel2_2_2_2 = new QLabel( Weights1StackPage, "textLabel2_2_2_2" );

    Weights1StackPageLayout->addWidget( textLabel2_2_2_2, 2, 0 );

    groupBox24 = new QGroupBox( Weights1StackPage, "groupBox24" );
    groupBox24->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox24->setColumnLayout(0, Qt::Vertical );
    groupBox24->layout()->setSpacing( 6 );
    groupBox24->layout()->setMargin( 11 );
    groupBox24Layout = new QGridLayout( groupBox24->layout() );
    groupBox24Layout->setAlignment( Qt::AlignTop );

    globw_uniformweightCheckBox = new QCheckBox( groupBox24, "globw_uniformweightCheckBox" );
    globw_uniformweightCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox24Layout->addMultiCellWidget( globw_uniformweightCheckBox, 0, 0, 0, 2 );

    textLabel14 = new QLabel( groupBox24, "textLabel14" );

    groupBox24Layout->addWidget( textLabel14, 3, 0 );

    textLabel13 = new QLabel( groupBox24, "textLabel13" );

    groupBox24Layout->addWidget( textLabel13, 2, 2 );

    globw_darkmaxLineEdit = new QLineEdit( groupBox24, "globw_darkmaxLineEdit" );
    globw_darkmaxLineEdit->setMinimumSize( QSize( 72, 0 ) );
    globw_darkmaxLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    globw_darkmaxLineEdit->setMouseTracking( TRUE );
    globw_darkmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox24Layout->addWidget( globw_darkmaxLineEdit, 4, 2 );

    globw_darkminLineEdit = new QLineEdit( groupBox24, "globw_darkminLineEdit" );
    globw_darkminLineEdit->setMinimumSize( QSize( 72, 0 ) );
    globw_darkminLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    globw_darkminLineEdit->setMouseTracking( TRUE );
    globw_darkminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox24Layout->addWidget( globw_darkminLineEdit, 4, 1 );

    globw_flatmaxLineEdit = new QLineEdit( groupBox24, "globw_flatmaxLineEdit" );
    globw_flatmaxLineEdit->setMinimumSize( QSize( 72, 0 ) );
    globw_flatmaxLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    globw_flatmaxLineEdit->setMouseTracking( TRUE );
    globw_flatmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox24Layout->addWidget( globw_flatmaxLineEdit, 3, 2 );

    textLabel12 = new QLabel( groupBox24, "textLabel12" );

    groupBox24Layout->addWidget( textLabel12, 2, 1 );

    globw_flatminLineEdit = new QLineEdit( groupBox24, "globw_flatminLineEdit" );
    globw_flatminLineEdit->setMinimumSize( QSize( 72, 0 ) );
    globw_flatminLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    globw_flatminLineEdit->setMouseTracking( TRUE );
    globw_flatminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox24Layout->addWidget( globw_flatminLineEdit, 3, 1 );

    textLabel15 = new QLabel( groupBox24, "textLabel15" );
    textLabel15->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, textLabel15->sizePolicy().hasHeightForWidth() ) );

    groupBox24Layout->addWidget( textLabel15, 4, 0 );

    globw_usebiasCheckBox = new QCheckBox( groupBox24, "globw_usebiasCheckBox" );
    globw_usebiasCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, globw_usebiasCheckBox->sizePolicy().hasHeightForWidth() ) );
    globw_usebiasCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox24Layout->addMultiCellWidget( globw_usebiasCheckBox, 5, 5, 0, 2 );
    spacer98_2_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox24Layout->addItem( spacer98_2_2, 1, 1 );

    Weights1StackPageLayout->addWidget( groupBox24, 3, 0 );
    param_WidgetStack->addWidget( Weights1StackPage, 4 );

    Weights2StackPage = new QWidget( param_WidgetStack, "Weights2StackPage" );
    Weights2StackPageLayout = new QGridLayout( Weights2StackPage, 1, 1, 11, 6, "Weights2StackPageLayout"); 

    textLabel2_2_2_3 = new QLabel( Weights2StackPage, "textLabel2_2_2_3" );

    Weights2StackPageLayout->addMultiCellWidget( textLabel2_2_2_3, 0, 0, 0, 1 );

    groupBox25 = new QGroupBox( Weights2StackPage, "groupBox25" );
    groupBox25->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, groupBox25->sizePolicy().hasHeightForWidth() ) );
    groupBox25->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox25->setColumnLayout(0, Qt::Vertical );
    groupBox25->layout()->setSpacing( 6 );
    groupBox25->layout()->setMargin( 11 );
    groupBox25Layout = new QGridLayout( groupBox25->layout() );
    groupBox25Layout->setAlignment( Qt::AlignTop );
    spacer95_3_2 = new QSpacerItem( 44, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    groupBox25Layout->addMultiCell( spacer95_3_2, 2, 2, 2, 3 );
    spacer95_3_3 = new QSpacerItem( 44, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    groupBox25Layout->addMultiCell( spacer95_3_3, 3, 3, 2, 3 );
    spacer95_3 = new QSpacerItem( 30, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    groupBox25Layout->addMultiCell( spacer95_3, 1, 1, 2, 3 );
    spacer91_2 = new QSpacerItem( 30, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    groupBox25Layout->addMultiCell( spacer91_2, 0, 0, 2, 3 );

    textLabel17 = new QLabel( groupBox25, "textLabel17" );

    groupBox25Layout->addMultiCellWidget( textLabel17, 0, 0, 0, 1 );

    textLabel17_2 = new QLabel( groupBox25, "textLabel17_2" );

    groupBox25Layout->addMultiCellWidget( textLabel17_2, 1, 1, 0, 1 );

    textLabel1_11 = new QLabel( groupBox25, "textLabel1_11" );

    groupBox25Layout->addMultiCellWidget( textLabel1_11, 2, 2, 0, 1 );

    textLabel1_11_2 = new QLabel( groupBox25, "textLabel1_11_2" );

    groupBox25Layout->addMultiCellWidget( textLabel1_11_2, 3, 3, 0, 1 );

    textLabel18 = new QLabel( groupBox25, "textLabel18" );

    groupBox25Layout->addMultiCellWidget( textLabel18, 4, 4, 0, 1 );
    spacer97 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox25Layout->addMultiCell( spacer97, 4, 4, 2, 3 );

    weight_cosmicLineEdit = new QLineEdit( groupBox25, "weight_cosmicLineEdit" );
    weight_cosmicLineEdit->setMinimumSize( QSize( 80, 0 ) );
    weight_cosmicLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    weight_cosmicLineEdit->setMouseTracking( TRUE );
    weight_cosmicLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox25Layout->addWidget( weight_cosmicLineEdit, 4, 4 );

    weight_cosmicDMINLineEdit = new QLineEdit( groupBox25, "weight_cosmicDMINLineEdit" );
    weight_cosmicDMINLineEdit->setMinimumSize( QSize( 80, 0 ) );
    weight_cosmicDMINLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    weight_cosmicDMINLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox25Layout->addWidget( weight_cosmicDMINLineEdit, 3, 4 );

    weight_cosmicDTLineEdit = new QLineEdit( groupBox25, "weight_cosmicDTLineEdit" );
    weight_cosmicDTLineEdit->setMinimumSize( QSize( 80, 0 ) );
    weight_cosmicDTLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    weight_cosmicDTLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox25Layout->addWidget( weight_cosmicDTLineEdit, 2, 4 );

    weight_threshmaxLineEdit = new QLineEdit( groupBox25, "weight_threshmaxLineEdit" );
    weight_threshmaxLineEdit->setMinimumSize( QSize( 80, 0 ) );
    weight_threshmaxLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    weight_threshmaxLineEdit->setMouseTracking( TRUE );
    weight_threshmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox25Layout->addWidget( weight_threshmaxLineEdit, 1, 4 );

    weight_threshminLineEdit = new QLineEdit( groupBox25, "weight_threshminLineEdit" );
    weight_threshminLineEdit->setMinimumSize( QSize( 80, 0 ) );
    weight_threshminLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    weight_threshminLineEdit->setMouseTracking( TRUE );
    weight_threshminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox25Layout->addWidget( weight_threshminLineEdit, 0, 4 );
    spacer92_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox25Layout->addItem( spacer92_2, 5, 1 );

    weight_bloom2TextLabel = new QLabel( groupBox25, "weight_bloom2TextLabel" );

    groupBox25Layout->addMultiCellWidget( weight_bloom2TextLabel, 8, 8, 1, 2 );

    weight_bloomCheckBox = new QCheckBox( groupBox25, "weight_bloomCheckBox" );
    weight_bloomCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox25Layout->addMultiCellWidget( weight_bloomCheckBox, 6, 6, 0, 4 );

    weight_bloom3TextLabel = new QLabel( groupBox25, "weight_bloom3TextLabel" );

    groupBox25Layout->addMultiCellWidget( weight_bloom3TextLabel, 9, 9, 1, 2 );

    weight_bloom1TextLabel = new QLabel( groupBox25, "weight_bloom1TextLabel" );

    groupBox25Layout->addMultiCellWidget( weight_bloom1TextLabel, 7, 7, 1, 2 );
    spacer91_3 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox25Layout->addItem( spacer91_3, 8, 0 );
    spacer93_2 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox25Layout->addItem( spacer93_2, 8, 3 );

    weight_bloomlowlimitLineEdit = new QLineEdit( groupBox25, "weight_bloomlowlimitLineEdit" );
    weight_bloomlowlimitLineEdit->setMinimumSize( QSize( 80, 0 ) );
    weight_bloomlowlimitLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    weight_bloomlowlimitLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox25Layout->addWidget( weight_bloomlowlimitLineEdit, 7, 4 );

    weight_bloomwidthLineEdit = new QLineEdit( groupBox25, "weight_bloomwidthLineEdit" );
    weight_bloomwidthLineEdit->setMinimumSize( QSize( 80, 0 ) );
    weight_bloomwidthLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    weight_bloomwidthLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox25Layout->addWidget( weight_bloomwidthLineEdit, 9, 4 );

    weight_bloomminareaLineEdit = new QLineEdit( groupBox25, "weight_bloomminareaLineEdit" );
    weight_bloomminareaLineEdit->setMinimumSize( QSize( 80, 0 ) );
    weight_bloomminareaLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    weight_bloomminareaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox25Layout->addWidget( weight_bloomminareaLineEdit, 8, 4 );

    Weights2StackPageLayout->addWidget( groupBox25, 1, 0 );
    spacer95 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Weights2StackPageLayout->addItem( spacer95, 1, 1 );
    spacer89 = new QSpacerItem( 20, 170, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Weights2StackPageLayout->addItem( spacer89, 2, 0 );
    param_WidgetStack->addWidget( Weights2StackPage, 5 );

    Ap1StackPage = new QWidget( param_WidgetStack, "Ap1StackPage" );
    Ap1StackPageLayout = new QGridLayout( Ap1StackPage, 1, 1, 11, 6, "Ap1StackPageLayout"); 

    textLabel2_2_2_3_2_2 = new QLabel( Ap1StackPage, "textLabel2_2_2_3_2_2" );

    Ap1StackPageLayout->addMultiCellWidget( textLabel2_2_2_3_2_2, 0, 0, 0, 1 );

    groupBox26_2_2 = new QGroupBox( Ap1StackPage, "groupBox26_2_2" );
    groupBox26_2_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox26_2_2->setColumnLayout(0, Qt::Vertical );
    groupBox26_2_2->layout()->setSpacing( 6 );
    groupBox26_2_2->layout()->setMargin( 11 );
    groupBox26_2_2Layout = new QGridLayout( groupBox26_2_2->layout() );
    groupBox26_2_2Layout->setAlignment( Qt::AlignTop );
    spacer185 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2_2Layout->addItem( spacer185, 0, 1 );

    textLabel1_10 = new QLabel( groupBox26_2_2, "textLabel1_10" );

    groupBox26_2_2Layout->addWidget( textLabel1_10, 0, 0 );

    absphot_indirect_stdcatComboBox = new QComboBox( FALSE, groupBox26_2_2, "absphot_indirect_stdcatComboBox" );
    absphot_indirect_stdcatComboBox->setFocusPolicy( QComboBox::NoFocus );
    absphot_indirect_stdcatComboBox->setSizeLimit( 12 );

    groupBox26_2_2Layout->addWidget( absphot_indirect_stdcatComboBox, 0, 2 );

    Ap1StackPageLayout->addWidget( groupBox26_2_2, 1, 0 );
    spacer90_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Ap1StackPageLayout->addItem( spacer90_2, 2, 1 );
    spacer91 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Ap1StackPageLayout->addItem( spacer91, 3, 0 );

    groupBox14 = new QGroupBox( Ap1StackPage, "groupBox14" );
    groupBox14->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox14->setColumnLayout(0, Qt::Vertical );
    groupBox14->layout()->setSpacing( 6 );
    groupBox14->layout()->setMargin( 11 );
    groupBox14Layout = new QGridLayout( groupBox14->layout() );
    groupBox14Layout->setAlignment( Qt::AlignTop );

    absphot_wcserrorCheckBox = new QCheckBox( groupBox14, "absphot_wcserrorCheckBox" );

    groupBox14Layout->addWidget( absphot_wcserrorCheckBox, 14, 0 );
    spacer104 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox14Layout->addItem( spacer104, 12, 1 );

    absphot_stdfilterComboBox = new QComboBox( FALSE, groupBox14, "absphot_stdfilterComboBox" );
    absphot_stdfilterComboBox->setMinimumSize( QSize( 72, 0 ) );
    absphot_stdfilterComboBox->setMaximumSize( QSize( 72, 32767 ) );
    absphot_stdfilterComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox14Layout->addMultiCellWidget( absphot_stdfilterComboBox, 3, 3, 2, 3 );

    absphot_stdcolorComboBox = new QComboBox( FALSE, groupBox14, "absphot_stdcolorComboBox" );
    absphot_stdcolorComboBox->setMinimumSize( QSize( 72, 0 ) );
    absphot_stdcolorComboBox->setMaximumSize( QSize( 72, 32767 ) );
    absphot_stdcolorComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox14Layout->addWidget( absphot_stdcolorComboBox, 3, 4 );

    absphot_apertureLineEdit = new QLineEdit( groupBox14, "absphot_apertureLineEdit" );
    absphot_apertureLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_apertureLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_apertureLineEdit->setMouseTracking( TRUE );
    absphot_apertureLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_apertureLineEdit, 1, 4 );

    absphot_extinctionLineEdit = new QLineEdit( groupBox14, "absphot_extinctionLineEdit" );
    absphot_extinctionLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_extinctionLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_extinctionLineEdit->setMouseTracking( TRUE );
    absphot_extinctionLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_extinctionLineEdit, 4, 4 );

    absphot_colorfixedLineEdit = new QLineEdit( groupBox14, "absphot_colorfixedLineEdit" );
    absphot_colorfixedLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_colorfixedLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_colorfixedLineEdit->setMouseTracking( TRUE );
    absphot_colorfixedLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_colorfixedLineEdit, 5, 4 );

    absphot_zpminLineEdit = new QLineEdit( groupBox14, "absphot_zpminLineEdit" );
    absphot_zpminLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_zpminLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_zpminLineEdit->setMouseTracking( TRUE );
    absphot_zpminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addMultiCellWidget( absphot_zpminLineEdit, 6, 6, 2, 3 );

    absphot_zpmaxLineEdit = new QLineEdit( groupBox14, "absphot_zpmaxLineEdit" );
    absphot_zpmaxLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_zpmaxLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_zpmaxLineEdit->setMouseTracking( TRUE );
    absphot_zpmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_zpmaxLineEdit, 6, 4 );

    absphot_kappaLineEdit = new QLineEdit( groupBox14, "absphot_kappaLineEdit" );
    absphot_kappaLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_kappaLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_kappaLineEdit->setMouseTracking( TRUE );
    absphot_kappaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addMultiCellWidget( absphot_kappaLineEdit, 7, 7, 2, 3 );

    absphot_thresholdLineEdit = new QLineEdit( groupBox14, "absphot_thresholdLineEdit" );
    absphot_thresholdLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_thresholdLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_thresholdLineEdit->setMouseTracking( TRUE );
    absphot_thresholdLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_thresholdLineEdit, 7, 4 );

    absphot_stdminmagLineEdit = new QLineEdit( groupBox14, "absphot_stdminmagLineEdit" );
    absphot_stdminmagLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_stdminmagLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_stdminmagLineEdit->setMouseTracking( TRUE );
    absphot_stdminmagLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_stdminmagLineEdit, 8, 4 );

    absphot_calmodeComboBox = new QComboBox( FALSE, groupBox14, "absphot_calmodeComboBox" );

    groupBox14Layout->addMultiCellWidget( absphot_calmodeComboBox, 11, 11, 2, 4 );

    absphot_maxiterLineEdit = new QLineEdit( groupBox14, "absphot_maxiterLineEdit" );
    absphot_maxiterLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_maxiterLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_maxiterLineEdit->setMouseTracking( TRUE );
    absphot_maxiterLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_maxiterLineEdit, 9, 4 );

    absphot_wcserrorPushButton = new QPushButton( groupBox14, "absphot_wcserrorPushButton" );

    groupBox14Layout->addMultiCellWidget( absphot_wcserrorPushButton, 13, 13, 3, 4 );

    textLabel1_12 = new QLabel( groupBox14, "textLabel1_12" );

    groupBox14Layout->addMultiCellWidget( textLabel1_12, 0, 0, 0, 4 );

    textLabel19_2_2_2_2 = new QLabel( groupBox14, "textLabel19_2_2_2_2" );

    groupBox14Layout->addMultiCellWidget( textLabel19_2_2_2_2, 1, 1, 0, 2 );

    textLabel21_2_2_2 = new QLabel( groupBox14, "textLabel21_2_2_2" );

    groupBox14Layout->addMultiCellWidget( textLabel21_2_2_2, 3, 3, 0, 1 );

    textLabel21_2_3 = new QLabel( groupBox14, "textLabel21_2_3" );

    groupBox14Layout->addMultiCellWidget( textLabel21_2_3, 4, 4, 0, 2 );

    textLabel20_2_2 = new QLabel( groupBox14, "textLabel20_2_2" );

    groupBox14Layout->addMultiCellWidget( textLabel20_2_2, 5, 5, 0, 2 );

    textLabel20_2_3_2 = new QLabel( groupBox14, "textLabel20_2_3_2" );

    groupBox14Layout->addWidget( textLabel20_2_3_2, 6, 0 );

    textLabel20_2_3_2_2 = new QLabel( groupBox14, "textLabel20_2_3_2_2" );

    groupBox14Layout->addWidget( textLabel20_2_3_2_2, 7, 0 );

    textLabel20_2_3_2_2_2_3_2 = new QLabel( groupBox14, "textLabel20_2_3_2_2_2_3_2" );

    groupBox14Layout->addMultiCellWidget( textLabel20_2_3_2_2_2_3_2, 9, 9, 0, 1 );

    textLabel20_2_3_2_2_2_3 = new QLabel( groupBox14, "textLabel20_2_3_2_2_2_3" );

    groupBox14Layout->addWidget( textLabel20_2_3_2_2_2_3, 11, 0 );

    textLabel20_2_3_2_2_2_3_2_2 = new QLabel( groupBox14, "textLabel20_2_3_2_2_2_3_2_2" );

    groupBox14Layout->addMultiCellWidget( textLabel20_2_3_2_2_2_3_2_2, 10, 10, 0, 1 );

    textLabel20_2_3_2_2_2 = new QLabel( groupBox14, "textLabel20_2_3_2_2_2" );

    groupBox14Layout->addMultiCellWidget( textLabel20_2_3_2_2_2, 8, 8, 0, 2 );

    absphot_convergenceLineEdit = new QLineEdit( groupBox14, "absphot_convergenceLineEdit" );
    absphot_convergenceLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_convergenceLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_convergenceLineEdit->setMouseTracking( TRUE );
    absphot_convergenceLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_convergenceLineEdit, 10, 4 );

    absphot_wcserrorLineEdit = new QLineEdit( groupBox14, "absphot_wcserrorLineEdit" );
    absphot_wcserrorLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_wcserrorLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_wcserrorLineEdit->setMouseTracking( TRUE );
    absphot_wcserrorLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addWidget( absphot_wcserrorLineEdit, 15, 4 );

    textLabel1_16 = new QLabel( groupBox14, "textLabel1_16" );

    groupBox14Layout->addMultiCellWidget( textLabel1_16, 13, 13, 0, 2 );

    absphot_filterLineEdit = new QLineEdit( groupBox14, "absphot_filterLineEdit" );
    absphot_filterLineEdit->setMinimumSize( QSize( 152, 0 ) );
    absphot_filterLineEdit->setMaximumSize( QSize( 152, 32767 ) );
    absphot_filterLineEdit->setMouseTracking( TRUE );
    absphot_filterLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14Layout->addMultiCellWidget( absphot_filterLineEdit, 2, 2, 2, 4 );

    textLabel19_2_2_2 = new QLabel( groupBox14, "textLabel19_2_2_2" );

    groupBox14Layout->addMultiCellWidget( textLabel19_2_2_2, 2, 2, 0, 1 );
    spacer143_2_3_2_2_3_2 = new QSpacerItem( 130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox14Layout->addMultiCell( spacer143_2_3_2_2_3_2, 15, 15, 1, 3 );

    textLabel20_2_3_2_2_2_2 = new QLabel( groupBox14, "textLabel20_2_3_2_2_2_2" );

    groupBox14Layout->addWidget( textLabel20_2_3_2_2_2_2, 15, 0 );

    Ap1StackPageLayout->addWidget( groupBox14, 2, 0 );
    param_WidgetStack->addWidget( Ap1StackPage, 6 );

    Ap2StackPage = new QWidget( param_WidgetStack, "Ap2StackPage" );
    Ap2StackPageLayout = new QGridLayout( Ap2StackPage, 1, 1, 11, 6, "Ap2StackPageLayout"); 
    spacer90_2_2 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Ap2StackPageLayout->addItem( spacer90_2_2, 2, 1 );

    textLabel2_2_2_3_2_2_2 = new QLabel( Ap2StackPage, "textLabel2_2_2_3_2_2_2" );

    Ap2StackPageLayout->addWidget( textLabel2_2_2_3_2_2_2, 0, 0 );
    spacer91_4 = new QSpacerItem( 20, 75, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Ap2StackPageLayout->addItem( spacer91_4, 3, 0 );

    groupBox26_2_2_2 = new QGroupBox( Ap2StackPage, "groupBox26_2_2_2" );
    groupBox26_2_2_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox26_2_2_2->setColumnLayout(0, Qt::Vertical );
    groupBox26_2_2_2->layout()->setSpacing( 6 );
    groupBox26_2_2_2->layout()->setMargin( 11 );
    groupBox26_2_2_2Layout = new QGridLayout( groupBox26_2_2_2->layout() );
    groupBox26_2_2_2Layout->setAlignment( Qt::AlignTop );
    spacer185_2 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2_2_2Layout->addItem( spacer185_2, 0, 1 );

    textLabel1_10_2 = new QLabel( groupBox26_2_2_2, "textLabel1_10_2" );

    groupBox26_2_2_2Layout->addWidget( textLabel1_10_2, 0, 0 );

    absphot_direct_stdcatComboBox = new QComboBox( FALSE, groupBox26_2_2_2, "absphot_direct_stdcatComboBox" );
    absphot_direct_stdcatComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox26_2_2_2Layout->addWidget( absphot_direct_stdcatComboBox, 0, 2 );

    Ap2StackPageLayout->addWidget( groupBox26_2_2_2, 1, 0 );

    groupBox14_2 = new QGroupBox( Ap2StackPage, "groupBox14_2" );
    groupBox14_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox14_2->setColumnLayout(0, Qt::Vertical );
    groupBox14_2->layout()->setSpacing( 6 );
    groupBox14_2->layout()->setMargin( 11 );
    groupBox14_2Layout = new QGridLayout( groupBox14_2->layout() );
    groupBox14_2Layout->setAlignment( Qt::AlignTop );

    textLabel3_9_2 = new QLabel( groupBox14_2, "textLabel3_9_2" );

    groupBox14_2Layout->addMultiCellWidget( textLabel3_9_2, 3, 3, 0, 1 );
    spacer142_2_2_2_2_2 = new QSpacerItem( 94, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox14_2Layout->addItem( spacer142_2_2_2_2_2, 2, 2 );
    spacer102_2 = new QSpacerItem( 94, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox14_2Layout->addItem( spacer102_2, 3, 2 );

    textLabel21_2_2_2_2_2 = new QLabel( groupBox14_2, "textLabel21_2_2_2_2_2" );

    groupBox14_2Layout->addMultiCellWidget( textLabel21_2_2_2_2_2, 2, 2, 0, 1 );

    textLabel4_5_2 = new QLabel( groupBox14_2, "textLabel4_5_2" );

    groupBox14_2Layout->addMultiCellWidget( textLabel4_5_2, 4, 4, 0, 1 );

    absphot_fittingmethodM2_ComboBox = new QComboBox( FALSE, groupBox14_2, "absphot_fittingmethodM2_ComboBox" );
    absphot_fittingmethodM2_ComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox14_2Layout->addMultiCellWidget( absphot_fittingmethodM2_ComboBox, 4, 4, 2, 3 );
    spacer105_2_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox14_2Layout->addItem( spacer105_2_2, 1, 1 );

    absphot_stdfilterM2_ComboBox = new QComboBox( FALSE, groupBox14_2, "absphot_stdfilterM2_ComboBox" );
    absphot_stdfilterM2_ComboBox->setMinimumSize( QSize( 72, 0 ) );
    absphot_stdfilterM2_ComboBox->setMaximumSize( QSize( 72, 32767 ) );
    absphot_stdfilterM2_ComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox14_2Layout->addWidget( absphot_stdfilterM2_ComboBox, 2, 3 );
    spacer105_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox14_2Layout->addItem( spacer105_3, 5, 0 );

    absphot_maxphoterrorLineEdit = new QLineEdit( groupBox14_2, "absphot_maxphoterrorLineEdit" );
    absphot_maxphoterrorLineEdit->setMinimumSize( QSize( 72, 0 ) );
    absphot_maxphoterrorLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    absphot_maxphoterrorLineEdit->setFocusPolicy( QLineEdit::TabFocus );
    absphot_maxphoterrorLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox14_2Layout->addWidget( absphot_maxphoterrorLineEdit, 3, 3 );

    textLabel5_6_3 = new QLabel( groupBox14_2, "textLabel5_6_3" );
    textLabel5_6_3->setFrameShape( QLabel::Box );

    groupBox14_2Layout->addMultiCellWidget( textLabel5_6_3, 6, 6, 0, 3 );

    textLabel2_11_2 = new QLabel( groupBox14_2, "textLabel2_11_2" );

    groupBox14_2Layout->addMultiCellWidget( textLabel2_11_2, 0, 0, 0, 3 );

    Ap2StackPageLayout->addWidget( groupBox14_2, 2, 0 );
    param_WidgetStack->addWidget( Ap2StackPage, 7 );

    Ap3StackPage = new QWidget( param_WidgetStack, "Ap3StackPage" );
    Ap3StackPageLayout = new QGridLayout( Ap3StackPage, 1, 1, 11, 6, "Ap3StackPageLayout"); 

    textLabel2_2_2_3_2 = new QLabel( Ap3StackPage, "textLabel2_2_2_3_2" );

    Ap3StackPageLayout->addWidget( textLabel2_2_2_3_2, 0, 0 );
    spacer132 = new QSpacerItem( 20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Ap3StackPageLayout->addItem( spacer132, 2, 0 );

    groupBox26_2 = new QGroupBox( Ap3StackPage, "groupBox26_2" );
    groupBox26_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox26_2->setColumnLayout(0, Qt::Vertical );
    groupBox26_2->layout()->setSpacing( 6 );
    groupBox26_2->layout()->setMargin( 11 );
    groupBox26_2Layout = new QGridLayout( groupBox26_2->layout() );
    groupBox26_2Layout->setAlignment( Qt::AlignTop );

    textLabel19_2 = new QLabel( groupBox26_2, "textLabel19_2" );

    groupBox26_2Layout->addWidget( textLabel19_2, 2, 0 );

    textLabel21_2 = new QLabel( groupBox26_2, "textLabel21_2" );

    groupBox26_2Layout->addWidget( textLabel21_2, 3, 0 );
    spacer142_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2Layout->addMultiCell( spacer142_2, 3, 3, 1, 2 );
    spacer141_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2Layout->addMultiCell( spacer141_2, 2, 2, 1, 2 );

    ap_deblendLineEdit = new QLineEdit( groupBox26_2, "ap_deblendLineEdit" );
    ap_deblendLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ap_deblendLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ap_deblendLineEdit->setMouseTracking( TRUE );
    ap_deblendLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox26_2Layout->addWidget( ap_deblendLineEdit, 2, 3 );
    spacer141_2_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2Layout->addMultiCell( spacer141_2_2, 0, 0, 1, 2 );
    spacer142_2_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2Layout->addMultiCell( spacer142_2_2, 1, 1, 1, 2 );

    ap_detectminareaLineEdit = new QLineEdit( groupBox26_2, "ap_detectminareaLineEdit" );
    ap_detectminareaLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ap_detectminareaLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ap_detectminareaLineEdit->setMouseTracking( TRUE );
    ap_detectminareaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox26_2Layout->addWidget( ap_detectminareaLineEdit, 1, 3 );

    ap_detectthreshLineEdit = new QLineEdit( groupBox26_2, "ap_detectthreshLineEdit" );
    ap_detectthreshLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ap_detectthreshLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ap_detectthreshLineEdit->setMouseTracking( TRUE );
    ap_detectthreshLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox26_2Layout->addWidget( ap_detectthreshLineEdit, 0, 3 );

    textLabel19_2_2 = new QLabel( groupBox26_2, "textLabel19_2_2" );

    groupBox26_2Layout->addWidget( textLabel19_2_2, 0, 0 );

    textLabel21_2_2 = new QLabel( groupBox26_2, "textLabel21_2_2" );

    groupBox26_2Layout->addWidget( textLabel21_2_2, 1, 0 );

    textLabel20_2 = new QLabel( groupBox26_2, "textLabel20_2" );

    groupBox26_2Layout->addWidget( textLabel20_2, 4, 0 );
    spacer143_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2Layout->addMultiCell( spacer143_2, 4, 4, 1, 2 );

    ap_flagLineEdit = new QLineEdit( groupBox26_2, "ap_flagLineEdit" );
    ap_flagLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ap_flagLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ap_flagLineEdit->setMouseTracking( TRUE );
    ap_flagLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox26_2Layout->addWidget( ap_flagLineEdit, 4, 3 );

    ap_minfwhmLineEdit = new QLineEdit( groupBox26_2, "ap_minfwhmLineEdit" );
    ap_minfwhmLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ap_minfwhmLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ap_minfwhmLineEdit->setMouseTracking( TRUE );
    ap_minfwhmLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox26_2Layout->addWidget( ap_minfwhmLineEdit, 3, 3 );
    spacer143_2_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2Layout->addMultiCell( spacer143_2_3, 8, 8, 1, 2 );

    textLabel20_2_3 = new QLabel( groupBox26_2, "textLabel20_2_3" );

    groupBox26_2Layout->addWidget( textLabel20_2_3, 8, 0 );

    ap_sexbacklevelLineEdit = new QLineEdit( groupBox26_2, "ap_sexbacklevelLineEdit" );
    ap_sexbacklevelLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ap_sexbacklevelLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ap_sexbacklevelLineEdit->setMouseTracking( TRUE );
    ap_sexbacklevelLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox26_2Layout->addWidget( ap_sexbacklevelLineEdit, 8, 3 );

    textLabel20_2_3_3 = new QLabel( groupBox26_2, "textLabel20_2_3_3" );

    groupBox26_2Layout->addWidget( textLabel20_2_3_3, 7, 0 );
    spacer143_2_3_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26_2Layout->addMultiCell( spacer143_2_3_3, 7, 7, 1, 2 );

    ap_lownumLineEdit = new QLineEdit( groupBox26_2, "ap_lownumLineEdit" );
    ap_lownumLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ap_lownumLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ap_lownumLineEdit->setMouseTracking( TRUE );
    ap_lownumLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox26_2Layout->addWidget( ap_lownumLineEdit, 7, 3 );

    ap_saturationLineEdit = new QLineEdit( groupBox26_2, "ap_saturationLineEdit" );
    ap_saturationLineEdit->setMinimumSize( QSize( 72, 0 ) );
    ap_saturationLineEdit->setMaximumSize( QSize( 72, 32767 ) );
    ap_saturationLineEdit->setMouseTracking( TRUE );
    ap_saturationLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox26_2Layout->addMultiCellWidget( ap_saturationLineEdit, 5, 6, 3, 3 );

    textLabel20_2_3_3_2 = new QLabel( groupBox26_2, "textLabel20_2_3_3_2" );

    groupBox26_2Layout->addMultiCellWidget( textLabel20_2_3_3_2, 5, 6, 0, 2 );

    ap_filterCheckBox = new QCheckBox( groupBox26_2, "ap_filterCheckBox" );
    ap_filterCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox26_2Layout->addMultiCellWidget( ap_filterCheckBox, 9, 9, 0, 3 );

    Ap3StackPageLayout->addWidget( groupBox26_2, 1, 0 );
    spacer131 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Ap3StackPageLayout->addItem( spacer131, 1, 1 );
    param_WidgetStack->addWidget( Ap3StackPage, 8 );

    Ap4StackPage = new QWidget( param_WidgetStack, "Ap4StackPage" );
    Ap4StackPageLayout = new QGridLayout( Ap4StackPage, 1, 1, 11, 6, "Ap4StackPageLayout"); 
    spacer92_4 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Ap4StackPageLayout->addItem( spacer92_4, 2, 0 );

    textLabel2_2_2_2_2 = new QLabel( Ap4StackPage, "textLabel2_2_2_2_2" );

    Ap4StackPageLayout->addWidget( textLabel2_2_2_2_2, 0, 0 );
    spacer137 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Ap4StackPageLayout->addItem( spacer137, 1, 1 );

    astrommethod_TabWidget = new QTabWidget( Ap4StackPage, "astrommethod_TabWidget" );
    astrommethod_TabWidget->setFocusPolicy( QTabWidget::NoFocus );

    tab = new QWidget( astrommethod_TabWidget, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 

    groupBox16 = new QGroupBox( tab, "groupBox16" );
    groupBox16->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox16->setColumnLayout(0, Qt::Vertical );
    groupBox16->layout()->setSpacing( 6 );
    groupBox16->layout()->setMargin( 11 );
    groupBox16Layout = new QGridLayout( groupBox16->layout() );
    groupBox16Layout->setAlignment( Qt::AlignTop );

    textLabel1_3 = new QLabel( groupBox16, "textLabel1_3" );
    textLabel1_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel1_3->sizePolicy().hasHeightForWidth() ) );

    groupBox16Layout->addMultiCellWidget( textLabel1_3, 0, 0, 0, 1 );

    textLabel2_4 = new QLabel( groupBox16, "textLabel2_4" );
    textLabel2_4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel2_4->sizePolicy().hasHeightForWidth() ) );

    groupBox16Layout->addMultiCellWidget( textLabel2_4, 1, 1, 0, 1 );

    textLabel1_7 = new QLabel( groupBox16, "textLabel1_7" );

    groupBox16Layout->addMultiCellWidget( textLabel1_7, 2, 2, 0, 1 );

    textLabel4_3 = new QLabel( groupBox16, "textLabel4_3" );
    textLabel4_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel4_3->sizePolicy().hasHeightForWidth() ) );

    groupBox16Layout->addMultiCellWidget( textLabel4_3, 3, 3, 0, 1 );

    textLabel6_3 = new QLabel( groupBox16, "textLabel6_3" );
    textLabel6_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel6_3->sizePolicy().hasHeightForWidth() ) );

    groupBox16Layout->addWidget( textLabel6_3, 13, 0 );

    ac_sc_stabilityComboBox = new QComboBox( FALSE, groupBox16, "ac_sc_stabilityComboBox" );
    ac_sc_stabilityComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, ac_sc_stabilityComboBox->sizePolicy().hasHeightForWidth() ) );
    ac_sc_stabilityComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox16Layout->addMultiCellWidget( ac_sc_stabilityComboBox, 12, 12, 2, 3 );

    textLabel2_5 = new QLabel( groupBox16, "textLabel2_5" );

    groupBox16Layout->addMultiCellWidget( textLabel2_5, 11, 11, 0, 1 );

    textLabel1 = new QLabel( groupBox16, "textLabel1" );

    groupBox16Layout->addMultiCellWidget( textLabel1, 14, 14, 0, 1 );

    textLabel3_4 = new QLabel( groupBox16, "textLabel3_4" );
    textLabel3_4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel3_4->sizePolicy().hasHeightForWidth() ) );

    groupBox16Layout->addMultiCellWidget( textLabel3_4, 12, 12, 0, 1 );

    ac_sc_mosaictypeComboBox = new QComboBox( FALSE, groupBox16, "ac_sc_mosaictypeComboBox" );
    ac_sc_mosaictypeComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox16Layout->addMultiCellWidget( ac_sc_mosaictypeComboBox, 13, 13, 2, 3 );

    ac_sc_mtypereadmePushButton = new QPushButton( groupBox16, "ac_sc_mtypereadmePushButton" );
    ac_sc_mtypereadmePushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox16Layout->addWidget( ac_sc_mtypereadmePushButton, 13, 1 );

    ac_sc_FPComboBox = new QComboBox( FALSE, groupBox16, "ac_sc_FPComboBox" );
    ac_sc_FPComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox16Layout->addMultiCellWidget( ac_sc_FPComboBox, 14, 14, 2, 3 );

    textLabel2_5_2 = new QLabel( groupBox16, "textLabel2_5_2" );

    groupBox16Layout->addMultiCellWidget( textLabel2_5_2, 10, 10, 0, 1 );

    textLabel2_5_2_2 = new QLabel( groupBox16, "textLabel2_5_2_2" );

    groupBox16Layout->addMultiCellWidget( textLabel2_5_2_2, 4, 4, 0, 1 );

    textLabel2_5_3 = new QLabel( groupBox16, "textLabel2_5_3" );

    groupBox16Layout->addMultiCellWidget( textLabel2_5_3, 5, 5, 0, 1 );

    textLabel2_5_3_2 = new QLabel( groupBox16, "textLabel2_5_3_2" );

    groupBox16Layout->addMultiCellWidget( textLabel2_5_3_2, 6, 6, 0, 1 );

    ac_sc_astrinstrukeyLineEdit = new QLineEdit( groupBox16, "ac_sc_astrinstrukeyLineEdit" );
    ac_sc_astrinstrukeyLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, ac_sc_astrinstrukeyLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_astrinstrukeyLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addMultiCellWidget( ac_sc_astrinstrukeyLineEdit, 10, 10, 2, 3 );

    ac_sc_photinstrukeyLineEdit = new QLineEdit( groupBox16, "ac_sc_photinstrukeyLineEdit" );
    ac_sc_photinstrukeyLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, ac_sc_photinstrukeyLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_photinstrukeyLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addMultiCellWidget( ac_sc_photinstrukeyLineEdit, 11, 11, 2, 3 );
    spacer3_6_3_2 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16Layout->addItem( spacer3_6_3_2, 6, 2 );
    spacer3_6_3 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16Layout->addItem( spacer3_6_3, 5, 2 );
    spacer3_6_2_2 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16Layout->addItem( spacer3_6_2_2, 4, 2 );
    spacer3_4 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16Layout->addItem( spacer3_4, 3, 2 );
    spacer2_3_2_2 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16Layout->addItem( spacer2_3_2_2, 2, 2 );
    spacer3_3 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16Layout->addItem( spacer3_3, 1, 2 );
    spacer3_2 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16Layout->addItem( spacer3_2, 0, 2 );

    ac_sc_astrefweightLineEdit = new QLineEdit( groupBox16, "ac_sc_astrefweightLineEdit" );
    ac_sc_astrefweightLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_astrefweightLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_astrefweightLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_astrefweightLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_astrefweightLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_astrefweightLineEdit, 6, 3 );

    ac_sc_crossidradiusLineEdit = new QLineEdit( groupBox16, "ac_sc_crossidradiusLineEdit" );
    ac_sc_crossidradiusLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_crossidradiusLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_crossidradiusLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_crossidradiusLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_crossidradiusLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_crossidradiusLineEdit, 5, 3 );

    ac_sc_fgroupradiusLineEdit = new QLineEdit( groupBox16, "ac_sc_fgroupradiusLineEdit" );
    ac_sc_fgroupradiusLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_fgroupradiusLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_fgroupradiusLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_fgroupradiusLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_fgroupradiusLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_fgroupradiusLineEdit, 4, 3 );

    ac_sc_distortorderLineEdit = new QLineEdit( groupBox16, "ac_sc_distortorderLineEdit" );
    ac_sc_distortorderLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_distortorderLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_distortorderLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_distortorderLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_distortorderLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_distortorderLineEdit, 3, 3 );

    ac_sc_maxscaleLineEdit = new QLineEdit( groupBox16, "ac_sc_maxscaleLineEdit" );
    ac_sc_maxscaleLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_maxscaleLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_maxscaleLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_maxscaleLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_maxscaleLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_maxscaleLineEdit, 2, 3 );

    ac_sc_maxoffsetLineEdit = new QLineEdit( groupBox16, "ac_sc_maxoffsetLineEdit" );
    ac_sc_maxoffsetLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_maxoffsetLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_maxoffsetLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_maxoffsetLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_maxoffsetLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_maxoffsetLineEdit, 1, 3 );

    ac_sc_maxposangleLineEdit = new QLineEdit( groupBox16, "ac_sc_maxposangleLineEdit" );
    ac_sc_maxposangleLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_maxposangleLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_maxposangleLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_maxposangleLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_maxposangleLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_maxposangleLineEdit, 0, 3 );

    textLabel2_5_2_3 = new QLabel( groupBox16, "textLabel2_5_2_3" );

    groupBox16Layout->addMultiCellWidget( textLabel2_5_2_3, 9, 9, 0, 1 );

    ac_sc_distortkeysLineEdit = new QLineEdit( groupBox16, "ac_sc_distortkeysLineEdit" );
    ac_sc_distortkeysLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, ac_sc_distortkeysLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_distortkeysLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addMultiCellWidget( ac_sc_distortkeysLineEdit, 9, 9, 2, 3 );

    textLabel4_3_2 = new QLabel( groupBox16, "textLabel4_3_2" );
    textLabel4_3_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel4_3_2->sizePolicy().hasHeightForWidth() ) );

    groupBox16Layout->addMultiCellWidget( textLabel4_3_2, 8, 8, 0, 1 );

    ac_sc_distortorder2LineEdit = new QLineEdit( groupBox16, "ac_sc_distortorder2LineEdit" );
    ac_sc_distortorder2LineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_distortorder2LineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_distortorder2LineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_distortorder2LineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_distortorder2LineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_distortorder2LineEdit, 8, 3 );

    ac_sc_maxSNLineEdit = new QLineEdit( groupBox16, "ac_sc_maxSNLineEdit" );
    ac_sc_maxSNLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_maxSNLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_maxSNLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_maxSNLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_maxSNLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_maxSNLineEdit, 7, 3 );

    ac_sc_minSNLineEdit = new QLineEdit( groupBox16, "ac_sc_minSNLineEdit" );
    ac_sc_minSNLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_minSNLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_minSNLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_minSNLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_minSNLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_minSNLineEdit, 7, 2 );

    textLabel5_3 = new QLabel( groupBox16, "textLabel5_3" );
    textLabel5_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel5_3->sizePolicy().hasHeightForWidth() ) );

    groupBox16Layout->addMultiCellWidget( textLabel5_3, 7, 7, 0, 1 );

    ac_sc_distortgroupsLineEdit = new QLineEdit( groupBox16, "ac_sc_distortgroupsLineEdit" );
    ac_sc_distortgroupsLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_sc_distortgroupsLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_sc_distortgroupsLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_sc_distortgroupsLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_sc_distortgroupsLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16Layout->addWidget( ac_sc_distortgroupsLineEdit, 8, 2 );

    ac_sc_matchflippedCheckBox = new QCheckBox( groupBox16, "ac_sc_matchflippedCheckBox" );
    ac_sc_matchflippedCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox16Layout->addMultiCellWidget( ac_sc_matchflippedCheckBox, 15, 15, 0, 2 );

    ac_sc_matchWCSCheckBox = new QCheckBox( groupBox16, "ac_sc_matchWCSCheckBox" );
    ac_sc_matchWCSCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox16Layout->addMultiCellWidget( ac_sc_matchWCSCheckBox, 16, 16, 0, 2 );

    tabLayout->addWidget( groupBox16, 0, 0 );
    astrommethod_TabWidget->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( astrommethod_TabWidget, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 

    groupBox16_2 = new QGroupBox( tab_2, "groupBox16_2" );
    groupBox16_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    groupBox16_2->setColumnLayout(0, Qt::Vertical );
    groupBox16_2->layout()->setSpacing( 6 );
    groupBox16_2->layout()->setMargin( 11 );
    groupBox16_2Layout = new QGridLayout( groupBox16_2->layout() );
    groupBox16_2Layout->setAlignment( Qt::AlignTop );
    spacer3_4_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16_2Layout->addItem( spacer3_4_2, 2, 1 );
    spacer2_3_2_2_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox16_2Layout->addMultiCell( spacer2_3_2_2_2, 0, 1, 1, 1 );

    textLabel1_7_2 = new QLabel( groupBox16_2, "textLabel1_7_2" );

    groupBox16_2Layout->addWidget( textLabel1_7_2, 0, 0 );

    ac_an_maxscaleLineEdit = new QLineEdit( groupBox16_2, "ac_an_maxscaleLineEdit" );
    ac_an_maxscaleLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_an_maxscaleLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_an_maxscaleLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_an_maxscaleLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_an_maxscaleLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16_2Layout->addMultiCellWidget( ac_an_maxscaleLineEdit, 0, 1, 2, 2 );

    ac_an_distortorderLineEdit = new QLineEdit( groupBox16_2, "ac_an_distortorderLineEdit" );
    ac_an_distortorderLineEdit->setEnabled( FALSE );
    ac_an_distortorderLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, ac_an_distortorderLineEdit->sizePolicy().hasHeightForWidth() ) );
    ac_an_distortorderLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ac_an_distortorderLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    ac_an_distortorderLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox16_2Layout->addWidget( ac_an_distortorderLineEdit, 2, 2 );

    ac_an_scampdistortCheckBox = new QCheckBox( groupBox16_2, "ac_an_scampdistortCheckBox" );

    groupBox16_2Layout->addMultiCellWidget( ac_an_scampdistortCheckBox, 3, 3, 0, 2 );

    textLabel4_3_3 = new QLabel( groupBox16_2, "textLabel4_3_3" );
    textLabel4_3_3->setEnabled( FALSE );
    textLabel4_3_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, textLabel4_3_3->sizePolicy().hasHeightForWidth() ) );

    groupBox16_2Layout->addMultiCellWidget( textLabel4_3_3, 1, 2, 0, 0 );

    tabLayout_2->addWidget( groupBox16_2, 0, 0 );
    spacer103 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout_2->addItem( spacer103, 1, 0 );
    spacer102 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer102, 0, 1 );
    astrommethod_TabWidget->insertTab( tab_2, QString::fromLatin1("") );

    TabPage = new QWidget( astrommethod_TabWidget, "TabPage" );
    TabPageLayout = new QGridLayout( TabPage, 1, 1, 11, 6, "TabPageLayout"); 

    absphot_groupbox = new QGroupBox( TabPage, "absphot_groupbox" );
    absphot_groupbox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    absphot_groupbox->setColumnLayout(0, Qt::Vertical );
    absphot_groupbox->layout()->setSpacing( 6 );
    absphot_groupbox->layout()->setMargin( 11 );
    absphot_groupboxLayout = new QGridLayout( absphot_groupbox->layout() );
    absphot_groupboxLayout->setAlignment( Qt::AlignTop );

    absphot_textlabel = new QLabel( absphot_groupbox, "absphot_textlabel" );

    absphot_groupboxLayout->addMultiCellWidget( absphot_textlabel, 0, 0, 0, 2 );

    NIGHT_5_PushButton = new QPushButton( absphot_groupbox, "NIGHT_5_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_5_PushButton, 5, 0 );

    NIGHT_4_PushButton = new QPushButton( absphot_groupbox, "NIGHT_4_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_4_PushButton, 4, 0 );

    NIGHT_3_PushButton = new QPushButton( absphot_groupbox, "NIGHT_3_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_3_PushButton, 3, 0 );

    NIGHT_6_PushButton = new QPushButton( absphot_groupbox, "NIGHT_6_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_6_PushButton, 6, 0 );

    NIGHT_2_PushButton = new QPushButton( absphot_groupbox, "NIGHT_2_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_2_PushButton, 2, 0 );

    NIGHT_1_PushButton = new QPushButton( absphot_groupbox, "NIGHT_1_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_1_PushButton, 1, 0 );

    NIGHT_1_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_1_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_1_ComboBox, 1, 1 );

    NIGHT_2_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_2_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_2_ComboBox, 2, 1 );

    NIGHT_3_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_3_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_3_ComboBox, 3, 1 );

    NIGHT_4_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_4_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_4_ComboBox, 4, 1 );

    NIGHT_5_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_5_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_5_ComboBox, 5, 1 );

    NIGHT_6_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_6_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_6_ComboBox, 6, 1 );
    spacer90_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3, 1, 2 );
    spacer90_3_6 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_6, 6, 2 );
    spacer90_3_5 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_5, 5, 2 );
    spacer90_3_4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_4, 4, 2 );
    spacer90_3_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_3, 3, 2 );
    spacer90_3_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_2, 2, 2 );
    spacer90_3_7_4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_7_4, 10, 2 );

    NIGHT_10_PushButton = new QPushButton( absphot_groupbox, "NIGHT_10_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_10_PushButton, 10, 0 );

    NIGHT_10_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_10_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_10_ComboBox, 10, 1 );

    NIGHT_9_PushButton = new QPushButton( absphot_groupbox, "NIGHT_9_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_9_PushButton, 9, 0 );

    NIGHT_9_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_9_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_9_ComboBox, 9, 1 );
    spacer90_3_7_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_7_3, 9, 2 );

    NIGHT_8_PushButton = new QPushButton( absphot_groupbox, "NIGHT_8_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_8_PushButton, 8, 0 );

    NIGHT_8_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_8_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_8_ComboBox, 8, 1 );
    spacer90_3_7_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_7_2, 8, 2 );
    spacer90_3_7 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    absphot_groupboxLayout->addItem( spacer90_3_7, 7, 2 );

    NIGHT_7_ComboBox = new QComboBox( FALSE, absphot_groupbox, "NIGHT_7_ComboBox" );

    absphot_groupboxLayout->addWidget( NIGHT_7_ComboBox, 7, 1 );

    NIGHT_7_PushButton = new QPushButton( absphot_groupbox, "NIGHT_7_PushButton" );

    absphot_groupboxLayout->addWidget( NIGHT_7_PushButton, 7, 0 );

    TabPageLayout->addWidget( absphot_groupbox, 0, 0 );
    astrommethod_TabWidget->insertTab( TabPage, QString::fromLatin1("") );

    Ap4StackPageLayout->addWidget( astrommethod_TabWidget, 1, 0 );
    param_WidgetStack->addWidget( Ap4StackPage, 9 );

    CoaddStackPage = new QWidget( param_WidgetStack, "CoaddStackPage" );
    CoaddStackPageLayout = new QGridLayout( CoaddStackPage, 1, 1, 11, 6, "CoaddStackPageLayout"); 

    textLabel2_2_2_2_2_2 = new QLabel( CoaddStackPage, "textLabel2_2_2_2_2_2" );

    CoaddStackPageLayout->addMultiCellWidget( textLabel2_2_2_2_2_2, 0, 0, 0, 1 );
    spacer107 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    CoaddStackPageLayout->addItem( spacer107, 3, 0 );
    spacer106 = new QSpacerItem( 100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    CoaddStackPageLayout->addItem( spacer106, 2, 1 );

    frame4 = new QFrame( CoaddStackPage, "frame4" );
    frame4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, frame4->sizePolicy().hasHeightForWidth() ) );
    frame4->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    frame4->setFrameShape( QFrame::GroupBoxPanel );
    frame4->setFrameShadow( QFrame::Sunken );
    frame4Layout = new QGridLayout( frame4, 1, 1, 11, 6, "frame4Layout"); 

    textLabel2_8 = new QLabel( frame4, "textLabel2_8" );

    frame4Layout->addWidget( textLabel2_8, 3, 0 );

    coc_filterTextLabel = new QLabel( frame4, "coc_filterTextLabel" );

    frame4Layout->addWidget( coc_filterTextLabel, 2, 0 );

    textLabel1_2_2 = new QLabel( frame4, "textLabel1_2_2" );

    frame4Layout->addWidget( textLabel1_2_2, 1, 0 );

    textLabel1_4 = new QLabel( frame4, "textLabel1_4" );

    frame4Layout->addMultiCellWidget( textLabel1_4, 4, 4, 0, 1 );

    textLabel3_5_2 = new QLabel( frame4, "textLabel3_5_2" );

    frame4Layout->addMultiCellWidget( textLabel3_5_2, 5, 5, 0, 1 );

    coc_skyposangleLineEdit = new QLineEdit( frame4, "coc_skyposangleLineEdit" );
    coc_skyposangleLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_skyposangleLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_skyposangleLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_skyposangleLineEdit, 5, 3 );

    coc_speedunitComboBox = new QComboBox( FALSE, frame4, "coc_speedunitComboBox" );
    coc_speedunitComboBox->setFocusPolicy( QComboBox::NoFocus );

    frame4Layout->addWidget( coc_speedunitComboBox, 3, 1 );

    coc_coaddsizeyLineEdit = new QLineEdit( frame4, "coc_coaddsizeyLineEdit" );
    coc_coaddsizeyLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_coaddsizeyLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_coaddsizeyLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_coaddsizeyLineEdit, 4, 3 );

    coc_coaddsizexLineEdit = new QLineEdit( frame4, "coc_coaddsizexLineEdit" );
    coc_coaddsizexLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_coaddsizexLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_coaddsizexLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_coaddsizexLineEdit, 4, 2 );

    coc_propmotionraLineEdit = new QLineEdit( frame4, "coc_propmotionraLineEdit" );
    coc_propmotionraLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_propmotionraLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_propmotionraLineEdit, 3, 2 );

    coc_propmotiondecLineEdit = new QLineEdit( frame4, "coc_propmotiondecLineEdit" );
    coc_propmotiondecLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_propmotiondecLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_propmotiondecLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_propmotiondecLineEdit, 3, 3 );

    coc_filterComboBox = new QComboBox( FALSE, frame4, "coc_filterComboBox" );
    coc_filterComboBox->setFocusPolicy( QComboBox::NoFocus );

    frame4Layout->addMultiCellWidget( coc_filterComboBox, 2, 2, 2, 3 );

    coc_skypafromimagePushButton = new QPushButton( frame4, "coc_skypafromimagePushButton" );
    coc_skypafromimagePushButton->setFocusPolicy( QPushButton::NoFocus );

    frame4Layout->addWidget( coc_skypafromimagePushButton, 5, 2 );

    textLabel3_5 = new QLabel( frame4, "textLabel3_5" );

    frame4Layout->addMultiCellWidget( textLabel3_5, 7, 7, 0, 1 );

    coc_smoothedgeLineEdit = new QLineEdit( frame4, "coc_smoothedgeLineEdit" );
    coc_smoothedgeLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_smoothedgeLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_smoothedgeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_smoothedgeLineEdit, 8, 3 );

    coc_pixscaleLineEdit = new QLineEdit( frame4, "coc_pixscaleLineEdit" );
    coc_pixscaleLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_pixscaleLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_pixscaleLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_pixscaleLineEdit, 7, 3 );

    coc_RZPLineEdit = new QLineEdit( frame4, "coc_RZPLineEdit" );
    coc_RZPLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_RZPLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_RZPLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_RZPLineEdit, 6, 3 );

    textLabel3_5_2_2 = new QLabel( frame4, "textLabel3_5_2_2" );

    frame4Layout->addMultiCellWidget( textLabel3_5_2_2, 6, 6, 0, 1 );

    coc_seeingLineEdit = new QLineEdit( frame4, "coc_seeingLineEdit" );
    coc_seeingLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_seeingLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_seeingLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addWidget( coc_seeingLineEdit, 6, 2 );

    coc_chipsLineEdit = new QLineEdit( frame4, "coc_chipsLineEdit" );
    coc_chipsLineEdit->setMinimumSize( QSize( 120, 0 ) );
    coc_chipsLineEdit->setMaximumSize( QSize( 32767, 32767 ) );
    coc_chipsLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4Layout->addMultiCellWidget( coc_chipsLineEdit, 1, 1, 2, 3 );

    layout5 = new QGridLayout( 0, 1, 1, 0, 6, "layout5"); 
    spacer113_2 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout5->addMultiCell( spacer113_2, 0, 0, 1, 2 );

    coc_identLineEdit = new QLineEdit( frame4, "coc_identLineEdit" );
    coc_identLineEdit->setMaxLength( 37 );
    coc_identLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout5->addMultiCellWidget( coc_identLineEdit, 1, 1, 2, 4 );

    coc_refraLineEdit = new QLineEdit( frame4, "coc_refraLineEdit" );
    coc_refraLineEdit->setMinimumSize( QSize( 0, 0 ) );
    coc_refraLineEdit->setMaximumSize( QSize( 32767, 32767 ) );
    coc_refraLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout5->addWidget( coc_refraLineEdit, 0, 3 );

    textLabel1_6 = new QLabel( frame4, "textLabel1_6" );

    layout5->addMultiCellWidget( textLabel1_6, 1, 1, 0, 1 );

    coc_refraTextLabel = new QLabel( frame4, "coc_refraTextLabel" );

    layout5->addWidget( coc_refraTextLabel, 0, 0 );

    coc_refdecLineEdit = new QLineEdit( frame4, "coc_refdecLineEdit" );
    coc_refdecLineEdit->setMinimumSize( QSize( 0, 0 ) );
    coc_refdecLineEdit->setMaximumSize( QSize( 32767, 32767 ) );
    coc_refdecLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout5->addWidget( coc_refdecLineEdit, 0, 4 );

    frame4Layout->addMultiCellLayout( layout5, 0, 0, 0, 3 );

    textLabel1_9 = new QLabel( frame4, "textLabel1_9" );

    frame4Layout->addMultiCellWidget( textLabel1_9, 8, 8, 0, 2 );

    layout9 = new QGridLayout( 0, 1, 1, 0, 6, "layout9"); 
    spacer114 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout9->addItem( spacer114, 1, 1 );

    coc_filterthresholdLineEdit = new QLineEdit( frame4, "coc_filterthresholdLineEdit" );
    coc_filterthresholdLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_filterthresholdLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_filterthresholdLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout9->addWidget( coc_filterthresholdLineEdit, 1, 2 );

    textLabel1_14 = new QLabel( frame4, "textLabel1_14" );

    layout9->addMultiCellWidget( textLabel1_14, 0, 1, 0, 0 );

    textLabel2_10 = new QLabel( frame4, "textLabel2_10" );

    layout9->addWidget( textLabel2_10, 0, 3 );

    textLabel5_4 = new QLabel( frame4, "textLabel5_4" );

    layout9->addWidget( textLabel5_4, 0, 2 );

    coc_filterborderwidthLineEdit = new QLineEdit( frame4, "coc_filterborderwidthLineEdit" );
    coc_filterborderwidthLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_filterborderwidthLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_filterborderwidthLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout9->addWidget( coc_filterborderwidthLineEdit, 1, 4 );

    coc_filterclustersizeLineEdit = new QLineEdit( frame4, "coc_filterclustersizeLineEdit" );
    coc_filterclustersizeLineEdit->setMinimumSize( QSize( 80, 0 ) );
    coc_filterclustersizeLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    coc_filterclustersizeLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout9->addWidget( coc_filterclustersizeLineEdit, 1, 3 );

    textLabel3_8 = new QLabel( frame4, "textLabel3_8" );

    layout9->addWidget( textLabel3_8, 0, 4 );

    frame4Layout->addMultiCellLayout( layout9, 9, 9, 0, 3 );

    CoaddStackPageLayout->addMultiCellWidget( frame4, 1, 1, 0, 1 );

    frame4_2 = new QFrame( CoaddStackPage, "frame4_2" );
    frame4_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_red.png" ) );
    frame4_2->setFrameShape( QFrame::GroupBoxPanel );
    frame4_2->setFrameShadow( QFrame::Sunken );
    frame4_2Layout = new QGridLayout( frame4_2, 1, 1, 11, 6, "frame4_2Layout"); 

    cc_projectionTextLabel = new QLabel( frame4_2, "cc_projectionTextLabel" );
    cc_projectionTextLabel->setEnabled( TRUE );

    frame4_2Layout->addWidget( cc_projectionTextLabel, 1, 0 );

    cc_typeTextLabel = new QLabel( frame4_2, "cc_typeTextLabel" );
    cc_typeTextLabel->setEnabled( TRUE );

    frame4_2Layout->addWidget( cc_typeTextLabel, 2, 0 );

    textLabel1_4_2 = new QLabel( frame4_2, "textLabel1_4_2" );

    frame4_2Layout->addWidget( textLabel1_4_2, 3, 0 );

    coc_celestialtypeComboBox = new QComboBox( FALSE, frame4_2, "coc_celestialtypeComboBox" );
    coc_celestialtypeComboBox->setEnabled( TRUE );
    coc_celestialtypeComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, coc_celestialtypeComboBox->sizePolicy().hasHeightForWidth() ) );
    coc_celestialtypeComboBox->setMinimumSize( QSize( 140, 0 ) );
    coc_celestialtypeComboBox->setFocusPolicy( QComboBox::NoFocus );

    frame4_2Layout->addWidget( coc_celestialtypeComboBox, 2, 1 );

    coc_projectionComboBox = new QComboBox( FALSE, frame4_2, "coc_projectionComboBox" );
    coc_projectionComboBox->setEnabled( TRUE );
    coc_projectionComboBox->setMinimumSize( QSize( 140, 0 ) );
    coc_projectionComboBox->setFocusPolicy( QComboBox::NoFocus );
    coc_projectionComboBox->setSizeLimit( 26 );

    frame4_2Layout->addWidget( coc_projectionComboBox, 1, 1 );

    coc_combineComboBox = new QComboBox( FALSE, frame4_2, "coc_combineComboBox" );
    coc_combineComboBox->setFocusPolicy( QComboBox::NoFocus );

    frame4_2Layout->addWidget( coc_combineComboBox, 3, 1 );

    coc_kernelComboBox = new QComboBox( FALSE, frame4_2, "coc_kernelComboBox" );
    coc_kernelComboBox->setMinimumSize( QSize( 140, 0 ) );
    coc_kernelComboBox->setFocusPolicy( QComboBox::NoFocus );

    frame4_2Layout->addWidget( coc_kernelComboBox, 0, 1 );

    textLabel8_3 = new QLabel( frame4_2, "textLabel8_3" );

    frame4_2Layout->addWidget( textLabel8_3, 0, 0 );

    coc_clipsigmaLineEdit = new QLineEdit( frame4_2, "coc_clipsigmaLineEdit" );
    coc_clipsigmaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4_2Layout->addWidget( coc_clipsigmaLineEdit, 6, 1 );

    coc_clipsigmaLabel = new QLabel( frame4_2, "coc_clipsigmaLabel" );

    frame4_2Layout->addWidget( coc_clipsigmaLabel, 6, 0 );

    coc_clipampfracLabel = new QLabel( frame4_2, "coc_clipampfracLabel" );

    frame4_2Layout->addWidget( coc_clipampfracLabel, 5, 0 );

    coc_clipampfracLineEdit = new QLineEdit( frame4_2, "coc_clipampfracLineEdit" );
    coc_clipampfracLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    frame4_2Layout->addWidget( coc_clipampfracLineEdit, 5, 1 );

    coc_rescaleweightsCheckBox = new QCheckBox( frame4_2, "coc_rescaleweightsCheckBox" );

    frame4_2Layout->addMultiCellWidget( coc_rescaleweightsCheckBox, 4, 4, 0, 1 );

    CoaddStackPageLayout->addWidget( frame4_2, 2, 0 );
    param_WidgetStack->addWidget( CoaddStackPage, 10 );

    parametersLayout->addMultiCellWidget( param_WidgetStack, 1, 2, 0, 1 );
    languageChange();
    resize( QSize(661, 745).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( absphot_direct_stdcatComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_stdcolor_comboboxes() ) );
    connect( absphot_indirect_stdcatComboBox, SIGNAL( activated(const QString&) ), this, SLOT( init_stdcolor_comboboxes() ) );
    connect( ac_sc_mtypereadmePushButton, SIGNAL( clicked() ), this, SLOT( call_mtype_readme() ) );
    connect( bias_nhighLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( bias_nlowLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ccc_colldetectminareaLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ccc_colldetectthreshLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ccc_colldirComboBox, SIGNAL( activated(const QString&) ), this, SLOT( adjust_collcorr_direction() ) );
    connect( ccc_collrejectthreshLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ccc_collxmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ccc_collxminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ccc_collymaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ccc_collyminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( coc_celestialtypeComboBox, SIGNAL( activated(const QString&) ), this, SLOT( update_refcoord_textlabel() ) );
    connect( coc_coaddsizexLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( coc_coaddsizeyLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( coc_filterthresholdLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( check_filterthreshold() ) );
    connect( coc_skypafromimagePushButton, SIGNAL( clicked() ), this, SLOT( fill_skyposangle() ) );
    connect( dark_nhighLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( dark_nlowLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( defect_clustolLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( defect_coltolLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( defect_coltol_sfLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( defect_kernelsizeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( defect_kernelsize_sfLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( defect_kernelsize_sfLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( defect_rowtolLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( defect_rowtol_sfLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( flat_nhighLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( flat_nlowLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( globw_darkmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( globw_darkminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( globw_flatmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( globw_flatminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( globw_uniformweightCheckBox, SIGNAL( clicked() ), this, SLOT( toggle_globalweight_params() ) );
    connect( ovscan_nhighLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ovscan_nlowLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( param_backPushButton, SIGNAL( clicked() ), this, SLOT( switch_page_back() ) );
    connect( param_cancelPushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( param_defaultsallPushButton, SIGNAL( clicked() ), this, SLOT( defaults_all() ) );
    connect( param_defaultsPushButton, SIGNAL( clicked() ), this, SLOT( defaults() ) );
    connect( param_nextPushButton, SIGNAL( clicked() ), this, SLOT( switch_page_next() ) );
    connect( param_okPushButton, SIGNAL( clicked() ), this, SLOT( parameters_quit() ) );
    connect( param_okPushButton, SIGNAL( clicked() ), this, SLOT( check_defect_kernelsize() ) );
    connect( pre_rename_fitskeyLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( pre_sort_biasLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( pre_sort_darkLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( pre_sort_domeflatLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( pre_sort_fitskeyLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( pre_sort_skyflatLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( pre_sort_stdLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_detectminareaLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_detectthreshLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_fringesmoothLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_illumsmoothLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_nhigh1LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_nlow1LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_nhigh2LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_nlow2LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_windowsizeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( toggle_statdyn() ) );
    connect( weight_bloomCheckBox, SIGNAL( clicked() ), this, SLOT( showhide_bloom() ) );
    connect( weight_cosmicLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( showhide_cosmicsDTDMIN() ) );
    connect( xtalk_col_1x2ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_col_2x1ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_col_2x2ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_multiCheckBox, SIGNAL( clicked() ), this, SLOT( link_checkbox_xtalk() ) );
    connect( xtalk_multi_nsectionLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( xtalk_multixToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_multiyToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_nor_1x2ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_nor_2x1ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_nor_2x2ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_nor_amplitudeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( xtalk_norCheckBox, SIGNAL( clicked() ), this, SLOT( link_checkbox_xtalk() ) );
    connect( xtalk_row_1x2ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_row_2x1ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_row_2x2ToolButton, SIGNAL( clicked() ), this, SLOT( link_xtalk_checkbox() ) );
    connect( xtalk_row_amplitudeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( xtalk_rowCheckBox, SIGNAL( clicked() ), this, SLOT( link_checkbox_xtalk() ) );
    connect( sf_back_windowsizeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_gapsizeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( sf_back_maskexpandfactorLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( ccc_collmaskexpandfactorLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( NIGHT_1_PushButton, SIGNAL( clicked() ), this, SLOT( show_photcal1() ) );
    connect( NIGHT_2_PushButton, SIGNAL( clicked() ), this, SLOT( show_photcal2() ) );
    connect( NIGHT_3_PushButton, SIGNAL( clicked() ), this, SLOT( show_photcal3() ) );
    connect( NIGHT_4_PushButton, SIGNAL( clicked() ), this, SLOT( show_photcal4() ) );
    connect( NIGHT_5_PushButton, SIGNAL( clicked() ), this, SLOT( show_photcal5() ) );
    connect( NIGHT_6_PushButton, SIGNAL( clicked() ), this, SLOT( show_photcal6() ) );
    connect( NIGHT_7_PushButton, SIGNAL( clicked() ), this, SLOT( show_photcal7() ) );
    connect( absphot_wcserrorLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( absphot_wcserrorPushButton, SIGNAL( clicked() ), this, SLOT( show_absphot_readme() ) );
    connect( absphot_wcserrorCheckBox, SIGNAL( clicked() ), this, SLOT( show_hide_absphot_wcserror() ) );
    connect( absphot_zpminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( absphot_zpmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( absphot_apertureLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( absphot_filterLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( absphot_colorfixedLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( absphot_extinctionLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( absphot_stdminmagLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( coc_clipampfracLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( coc_clipsigmaLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( coc_combineComboBox, SIGNAL( activated(const QString&) ), this, SLOT( showhide_clipped() ) );
    connect( sf_back_twopassCheckBox, SIGNAL( clicked() ), this, SLOT( show_hide_twopass() ) );

    // tab order
    setTabOrder( pre_sort_fitskeyLineEdit, pre_sort_domeflatLineEdit );
    setTabOrder( pre_sort_domeflatLineEdit, pre_sort_biasLineEdit );
    setTabOrder( pre_sort_biasLineEdit, pre_sort_skyflatLineEdit );
    setTabOrder( pre_sort_skyflatLineEdit, pre_sort_darkLineEdit );
    setTabOrder( pre_sort_darkLineEdit, pre_sort_stdLineEdit );
    setTabOrder( pre_sort_stdLineEdit, pre_rename_fitskeyLineEdit );
    setTabOrder( pre_rename_fitskeyLineEdit, xtalk_nor_amplitudeLineEdit );
    setTabOrder( xtalk_nor_amplitudeLineEdit, xtalk_row_amplitudeLineEdit );
    setTabOrder( xtalk_row_amplitudeLineEdit, xtalk_multi_nsectionLineEdit );
    setTabOrder( xtalk_multi_nsectionLineEdit, ovscan_nlowLineEdit );
    setTabOrder( ovscan_nlowLineEdit, ovscan_nhighLineEdit );
    setTabOrder( ovscan_nhighLineEdit, bias_nlowLineEdit );
    setTabOrder( bias_nlowLineEdit, bias_nhighLineEdit );
    setTabOrder( bias_nhighLineEdit, dark_nlowLineEdit );
    setTabOrder( dark_nlowLineEdit, dark_nhighLineEdit );
    setTabOrder( dark_nhighLineEdit, flat_nlowLineEdit );
    setTabOrder( flat_nlowLineEdit, flat_nhighLineEdit );
    setTabOrder( flat_nhighLineEdit, sf_back_detectthreshLineEdit );
    setTabOrder( sf_back_detectthreshLineEdit, sf_back_detectminareaLineEdit );
    setTabOrder( sf_back_detectminareaLineEdit, sf_back_maskexpandfactorLineEdit );
    setTabOrder( sf_back_maskexpandfactorLineEdit, sf_back_nlow1LineEdit );
    setTabOrder( sf_back_nlow1LineEdit, sf_back_nhigh1LineEdit );
    setTabOrder( sf_back_nhigh1LineEdit, sf_back_nlow2LineEdit );
    setTabOrder( sf_back_nlow2LineEdit, sf_back_nhigh2LineEdit );
    setTabOrder( sf_back_nhigh2LineEdit, sf_back_illumsmoothLineEdit );
    setTabOrder( sf_back_illumsmoothLineEdit, sf_back_fringesmoothLineEdit );
    setTabOrder( sf_back_fringesmoothLineEdit, sf_back_windowsizeLineEdit );
    setTabOrder( sf_back_windowsizeLineEdit, ccc_colldetectthreshLineEdit );
    setTabOrder( ccc_colldetectthreshLineEdit, ccc_colldetectminareaLineEdit );
    setTabOrder( ccc_colldetectminareaLineEdit, ccc_collrejectthreshLineEdit );
    setTabOrder( ccc_collrejectthreshLineEdit, ccc_collxminLineEdit );
    setTabOrder( ccc_collxminLineEdit, ccc_collxmaxLineEdit );
    setTabOrder( ccc_collxmaxLineEdit, ccc_collyminLineEdit );
    setTabOrder( ccc_collyminLineEdit, ccc_collymaxLineEdit );
    setTabOrder( ccc_collymaxLineEdit, w_binsizeSpinBox );
    setTabOrder( w_binsizeSpinBox, w_binminLineEdit );
    setTabOrder( w_binminLineEdit, w_binmaxLineEdit );
    setTabOrder( w_binmaxLineEdit, globw_flatminLineEdit );
    setTabOrder( globw_flatminLineEdit, globw_flatmaxLineEdit );
    setTabOrder( globw_flatmaxLineEdit, globw_darkminLineEdit );
    setTabOrder( globw_darkminLineEdit, globw_darkmaxLineEdit );
    setTabOrder( globw_darkmaxLineEdit, defect_kernelsizeLineEdit );
    setTabOrder( defect_kernelsizeLineEdit, defect_rowtolLineEdit );
    setTabOrder( defect_rowtolLineEdit, defect_coltolLineEdit );
    setTabOrder( defect_coltolLineEdit, defect_clustolLineEdit );
    setTabOrder( defect_clustolLineEdit, defect_kernelsize_sfLineEdit );
    setTabOrder( defect_kernelsize_sfLineEdit, defect_rowtol_sfLineEdit );
    setTabOrder( defect_rowtol_sfLineEdit, defect_coltol_sfLineEdit );
    setTabOrder( defect_coltol_sfLineEdit, defect_clustol_sfLineEdit );
    setTabOrder( defect_clustol_sfLineEdit, weight_threshminLineEdit );
    setTabOrder( weight_threshminLineEdit, weight_threshmaxLineEdit );
    setTabOrder( weight_threshmaxLineEdit, weight_cosmicDTLineEdit );
    setTabOrder( weight_cosmicDTLineEdit, weight_cosmicDMINLineEdit );
    setTabOrder( weight_cosmicDMINLineEdit, weight_cosmicLineEdit );
    setTabOrder( weight_cosmicLineEdit, weight_bloomlowlimitLineEdit );
    setTabOrder( weight_bloomlowlimitLineEdit, weight_bloomminareaLineEdit );
    setTabOrder( weight_bloomminareaLineEdit, weight_bloomwidthLineEdit );
    setTabOrder( weight_bloomwidthLineEdit, absphot_filterLineEdit );
    setTabOrder( absphot_filterLineEdit, absphot_extinctionLineEdit );
    setTabOrder( absphot_extinctionLineEdit, absphot_colorfixedLineEdit );
    setTabOrder( absphot_colorfixedLineEdit, absphot_zpminLineEdit );
    setTabOrder( absphot_zpminLineEdit, absphot_zpmaxLineEdit );
    setTabOrder( absphot_zpmaxLineEdit, absphot_stdminmagLineEdit );
    setTabOrder( absphot_stdminmagLineEdit, absphot_maxphoterrorLineEdit );
    setTabOrder( absphot_maxphoterrorLineEdit, ap_detectthreshLineEdit );
    setTabOrder( ap_detectthreshLineEdit, ap_detectminareaLineEdit );
    setTabOrder( ap_detectminareaLineEdit, ap_deblendLineEdit );
    setTabOrder( ap_deblendLineEdit, ap_minfwhmLineEdit );
    setTabOrder( ap_minfwhmLineEdit, ap_flagLineEdit );
    setTabOrder( ap_flagLineEdit, ap_saturationLineEdit );
    setTabOrder( ap_saturationLineEdit, ap_lownumLineEdit );
    setTabOrder( ap_lownumLineEdit, ap_sexbacklevelLineEdit );
    setTabOrder( ap_sexbacklevelLineEdit, ac_sc_maxposangleLineEdit );
    setTabOrder( ac_sc_maxposangleLineEdit, ac_sc_maxoffsetLineEdit );
    setTabOrder( ac_sc_maxoffsetLineEdit, ac_sc_maxscaleLineEdit );
    setTabOrder( ac_sc_maxscaleLineEdit, ac_sc_distortorderLineEdit );
    setTabOrder( ac_sc_distortorderLineEdit, ac_sc_fgroupradiusLineEdit );
    setTabOrder( ac_sc_fgroupradiusLineEdit, ac_sc_crossidradiusLineEdit );
    setTabOrder( ac_sc_crossidradiusLineEdit, ac_sc_astrefweightLineEdit );
    setTabOrder( ac_sc_astrefweightLineEdit, ac_sc_minSNLineEdit );
    setTabOrder( ac_sc_minSNLineEdit, ac_sc_maxSNLineEdit );
    setTabOrder( ac_sc_maxSNLineEdit, ac_sc_distortgroupsLineEdit );
    setTabOrder( ac_sc_distortgroupsLineEdit, ac_sc_distortorder2LineEdit );
    setTabOrder( ac_sc_distortorder2LineEdit, ac_sc_distortkeysLineEdit );
    setTabOrder( ac_sc_distortkeysLineEdit, ac_sc_astrinstrukeyLineEdit );
    setTabOrder( ac_sc_astrinstrukeyLineEdit, ac_sc_photinstrukeyLineEdit );
    setTabOrder( ac_sc_photinstrukeyLineEdit, ac_an_maxscaleLineEdit );
    setTabOrder( ac_an_maxscaleLineEdit, ac_an_distortorderLineEdit );
    setTabOrder( ac_an_distortorderLineEdit, coc_refraLineEdit );
    setTabOrder( coc_refraLineEdit, coc_refdecLineEdit );
    setTabOrder( coc_refdecLineEdit, coc_identLineEdit );
    setTabOrder( coc_identLineEdit, coc_chipsLineEdit );
    setTabOrder( coc_chipsLineEdit, coc_propmotionraLineEdit );
    setTabOrder( coc_propmotionraLineEdit, coc_propmotiondecLineEdit );
    setTabOrder( coc_propmotiondecLineEdit, coc_coaddsizexLineEdit );
    setTabOrder( coc_coaddsizexLineEdit, coc_coaddsizeyLineEdit );
    setTabOrder( coc_coaddsizeyLineEdit, coc_skyposangleLineEdit );
    setTabOrder( coc_skyposangleLineEdit, coc_seeingLineEdit );
    setTabOrder( coc_seeingLineEdit, coc_RZPLineEdit );
    setTabOrder( coc_RZPLineEdit, coc_pixscaleLineEdit );
    setTabOrder( coc_pixscaleLineEdit, coc_smoothedgeLineEdit );
    setTabOrder( coc_smoothedgeLineEdit, coc_filterthresholdLineEdit );
    setTabOrder( coc_filterthresholdLineEdit, coc_filterclustersizeLineEdit );
    setTabOrder( coc_filterclustersizeLineEdit, coc_filterborderwidthLineEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
parameters::~parameters()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void parameters::languageChange()
{
    setCaption( tr( "THELI -- parameter settings" ) );
    QToolTip::add( this, QString::null );
    param_head2TextLabel->setText( QString::null );
    param_head1TextLabel->setText( tr( "<b>Processing group:</b>" ) );
    param_defaultsPushButton->setText( tr( "Defaults (this page)" ) );
    param_nextPushButton->setText( tr( ">>" ) );
    QToolTip::add( param_nextPushButton, tr( "Switch to the next configuration page" ) );
    param_okPushButton->setText( tr( "&OK" ) );
    param_okPushButton->setAccel( QKeySequence( QString::null ) );
    param_cancelPushButton->setText( tr( "&Cancel" ) );
    param_cancelPushButton->setAccel( QKeySequence( QString::null ) );
    param_backPushButton->setText( tr( "<<" ) );
    QToolTip::add( param_backPushButton, tr( "Switch to the previous configuration page" ) );
    param_defaultsallPushButton->setText( tr( "Defaults (all pages)" ) );
    textLabel2_7->setText( tr( "<b><font color=\"#aa0000\">Sort data:</font></b>" ) );
    groupBox11->setTitle( QString::null );
    QToolTip::add( pre_sort_biasLineEdit, tr( "Key value that identifies a BIAS" ) );
    textLabel11_2->setText( tr( "SKYFLAT" ) );
    QToolTip::add( pre_sort_darkLineEdit, tr( "Key value that identifies a DARK" ) );
    QToolTip::add( pre_sort_domeflatLineEdit, tr( "Key value that identifies a DOMEFLAT" ) );
    QToolTip::add( pre_sort_skyflatLineEdit, tr( "Key value that identifies a SKYFLAT" ) );
    QToolTip::add( pre_sort_stdLineEdit, tr( "Key value that identifies a STANDARD" ) );
    QToolTip::add( pre_sort_fitskeyLineEdit, tr( "The FITS key that identifies the exposure type" ) );
    textLabel10_3->setText( tr( "DARK" ) );
    textLabel13_3->setText( tr( "STANDARD" ) );
    textLabel12_3->setText( tr( "DOMEFLAT" ) );
    textLabel9_2->setText( tr( "BIAS" ) );
    textLabel8_2->setText( tr( "FITS KEY" ) );
    textLabel2->setText( tr( "<b><font color=\"#aa0000\">Split FITS / correct header:</font></b>" ) );
    groupBox1->setTitle( QString::null );
    textLabel3->setText( tr( "Amplitude" ) );
    xtalk_norCheckBox->setText( tr( "Normal \n"
"crosstalk" ) );
    QToolTip::add( xtalk_nor_amplitudeLineEdit, tr( "Crosstalk amplitude. Negative values mean that images leave a negative footprint." ) );
    xtalk_norButtonGroup->setTitle( QString::null );
    xtalk_nor_2x2ToolButton->setText( QString::null );
    xtalk_nor_1x2ToolButton->setText( QString::null );
    xtalk_nor_2x1ToolButton->setText( QString::null );
    textLabel3_2->setText( tr( "Amplitude" ) );
    QToolTip::add( xtalk_row_amplitudeLineEdit, tr( "Crosstalk amplitude. Negative values mean that images leave a negative footprint." ) );
    xtalk_rowButtonGroup->setTitle( QString::null );
    xtalk_row_2x2ToolButton->setText( QString::null );
    xtalk_row_1x2ToolButton->setText( QString::null );
    xtalk_row_2x1ToolButton->setText( QString::null );
    xtalk_col_2x2ToolButton->setText( QString::null );
    xtalk_col_1x2ToolButton->setText( QString::null );
    xtalk_col_2x1ToolButton->setText( QString::null );
    xtalk_rowCheckBox->setText( tr( "Row\n"
"crosstalk" ) );
    xtalk_multiButtonGroup->setTitle( QString::null );
    xtalk_multixToolButton->setText( QString::null );
    xtalk_multiyToolButton->setText( QString::null );
    xtalk_multiCheckBox->setText( tr( "Multi\n"
"crosstalk" ) );
    textLabel1_13->setText( tr( "Number of\n"
"sections" ) );
    QToolTip::add( xtalk_multi_nsectionLineEdit, tr( "The number of readout stripes (usually for the larger HAWAII2 arrays)" ) );
    groupBox12->setTitle( QString::null );
    pre_renameCheckBox->setText( tr( "Rename file to FITS key" ) );
    QToolTip::add( pre_renameCheckBox, tr( "If activated, renames the images to the value of the FITS key given to the right" ) );
    textLabel4->setText( tr( "<b><font color=\"#aa0000\">Rejecting nlow and nhigh pixels from a stack</font></b>" ) );
    groupBox3->setTitle( QString::null );
    QToolTip::add( ovscan_nhighLineEdit, tr( "Number of high pixels to be rejected from the stack" ) );
    QToolTip::add( ovscan_nlowLineEdit, tr( "Number of low pixels to be rejected from the stack" ) );
    textLabel9->setText( tr( "nlow" ) );
    textLabel5->setText( tr( "Overscan" ) );
    textLabel7->setText( tr( "Flat" ) );
    QToolTip::add( flat_nlowLineEdit, tr( "Number of low pixels to be rejected from the stack" ) );
    QToolTip::add( flat_nhighLineEdit, tr( "Number of high pixels to be rejected from the stack" ) );
    QToolTip::add( bias_nlowLineEdit, tr( "Number of low pixels to be rejected from the stack" ) );
    QToolTip::add( bias_nhighLineEdit, tr( "Number of high pixels to be rejected from the stack" ) );
    textLabel6->setText( tr( "Bias" ) );
    textLabel6_4->setText( tr( "Dark" ) );
    QToolTip::add( dark_nlowLineEdit, tr( "Number of low pixels to be rejected from the stack" ) );
    QToolTip::add( dark_nhighLineEdit, tr( "Number of high pixels to be rejected from the stack" ) );
    textLabel10->setText( tr( "nhigh" ) );
    textLabel2_2->setText( tr( "<b><font color=\"#aa0000\">Background model:</font></b>" ) );
    groupBox4->setTitle( QString::null );
    textLabel2_11_3_2->setText( tr( "<b>(1) Object masking (optional)</b>" ) );
    sf_back_static_dynamicTextlabel->setText( tr( "(Model is: <i>STATIC</i>)" ) );
    cal_windowsizeTextLabel_2->setText( tr( "Max gap size (hours) :" ) );
    textLabel1_17_2_4->setText( tr( "<b>(3) Reject chips with bright stars</b>" ) );
    textLabel2_13_2_2->setText( tr( "nhigh" ) );
    textLabel2_13_3_4->setText( tr( "nlow" ) );
    QToolTip::add( sf_back_nhigh2LineEdit, tr( "Number of high pixels to be rejected from the stack during the second pass of the background modeling" ) );
    QToolTip::add( sf_back_nlow1LineEdit, tr( "Number of low pixels to be rejected from the stack during the first pass of the background modeling" ) );
    QToolTip::add( sf_back_nhigh1LineEdit, tr( "Number of high pixels to be rejected from the stack during the first pass of the background modeling" ) );
    sf_back_twopass2TextLabel->setText( tr( "2nd pass" ) );
    sf_back_twopass1TextLabel->setText( tr( "1st pass" ) );
    QToolTip::add( sf_back_nlow2LineEdit, tr( "Number of low pixels to be rejected from the stack during the second pass of the background modeling" ) );
    textLabel2_13_3_2->setText( tr( "Mag" ) );
    QToolTip::add( sf_back_maglimitLineEdit, tr( "Chips that have stars brighter than this magnitude in or near them will be rejected from the background modeling." ) );
    textLabel2_13_3_2_3->setText( tr( "Dist [']" ) );
    QToolTip::add( sf_back_distanceLineEdit, tr( "Minimum distance of a bright star from a chip edge to be considered \"safe\", in arcminutes" ) );
    textLabel2_13_3_2_2->setText( tr( "Server" ) );
    sf_back_serverComboBox->clear();
    sf_back_serverComboBox->insertItem( tr( "France" ) );
    sf_back_serverComboBox->insertItem( tr( "USA" ) );
    sf_back_serverComboBox->insertItem( tr( "Japan" ) );
    sf_back_serverComboBox->insertItem( tr( "Canada" ) );
    sf_back_serverComboBox->insertItem( tr( "India" ) );
    sf_back_serverComboBox->insertItem( tr( "China" ) );
    sf_back_serverComboBox->insertItem( tr( "UK" ) );
    sf_back_serverComboBox->insertItem( tr( "Russia" ) );
    sf_back_serverComboBox->insertItem( tr( "Hawaii" ) );
    QToolTip::add( sf_back_serverComboBox, tr( "The server from which to download the bright star catalog" ) );
    textLabel1_17_2_3->setText( tr( "<b>(4) How to apply the background model</b>" ) );
    cal_windowsizeTextLabel->setText( tr( "Window size :" ) );
    QToolTip::add( sf_back_gapsizeLineEdit, tr( "The maximum amount of time, in hours, by which an exposure sequence may be interrupted without enforcing a new set of background models.<br>" ) );
    textLabel1_17_2_2->setText( tr( "<b>(5) Static or dynamic model</b>" ) );
    QToolTip::add( sf_back_windowsizeLineEdit, tr( "The number of images closest in time to an exposure, used to calculate a background model for this exposure. If set to zero or left empty, a static model will be used.<br>" ) );
    sf_back_twopassCheckBox->setText( tr( "Two-pass background modeling" ) );
    QToolTip::add( sf_back_twopassCheckBox, tr( "Whether a two-pass background correction is desired. If yes, then the first pass will be without SExtractor object masking.<br>" ) );
    sf_back_sexfilterCheckBox->setText( tr( "SExtractor convolution" ) );
    QToolTip::add( sf_back_sexfilterCheckBox, tr( "If set, SExtractor will convolve the image with a general purpose filter. This will increase the footprint of hot pixels, though. If your data has LOTS of hot pixels, then you might get better results when not setting this box.<br>" ) );
    sf_back_detectminareaLineEdit->setText( QString::null );
    QToolTip::add( sf_back_detectminareaLineEdit, tr( "Minimum number of connected pixels for an object to be masked. If left empty, no masking will take place and images are combined right away.<br>" ) );
    sf_back_detectthreshLineEdit->setText( QString::null );
    QToolTip::add( sf_back_detectthreshLineEdit, tr( "Detection threshold per pixel for an object to be masked. If left empty, no masking will take place and images are combined right away.<br>" ) );
    textLabel1_17->setText( tr( "Mask expansion factor:" ) );
    textLabel1_17_4->setText( tr( "Combination method:" ) );
    cal_detectthreshTextLabel->setText( tr( "DT" ) );
    cal_detectminareaTextLabel->setText( tr( "DMIN" ) );
    sf_back_maskexpandfactorLineEdit->setText( QString::null );
    QToolTip::add( sf_back_maskexpandfactorLineEdit, tr( "If not empty, pixels within the scaled best-fit SExtractor ellipse will be set to zero. In this way very faint flux invisible in an individual image can be caught as well. Good starting value: 3.0 <br>" ) );
    sf_back_combinemethodComboBox->clear();
    sf_back_combinemethodComboBox->insertItem( tr( "Median" ) );
    sf_back_combinemethodComboBox->insertItem( tr( "Mean" ) );
    QToolTip::add( sf_back_combinemethodComboBox, tr( "How to combine the images of a stack" ) );
    textLabel1_17_2->setText( tr( "<b>(2) Reject pixels during combination</b>" ) );
    sf_back_applymodeComboBox->clear();
    sf_back_applymodeComboBox->insertItem( tr( "Subtract model" ) );
    sf_back_applymodeComboBox->insertItem( tr( "Divide model" ) );
    sf_back_applymodeComboBox->insertItem( tr( "Subtract fringes" ) );
    sf_back_applymodeComboBox->insertItem( tr( "Divide smoothed model, subtract fringes" ) );
    sf_back_fringesmoothLineEdit->setText( QString::null );
    QToolTip::add( sf_back_fringesmoothLineEdit, tr( "Half width of the smoothing kernel for the FRINGE model. If empty, then no smoothing is performed. Values of 1-2 are usually sufficient.<br>" ) );
    sf_back_adjustgainsCheckBox->setText( tr( "Adjust gains between chips" ) );
    QToolTip::add( sf_back_adjustgainsCheckBox, tr( "Check this if the gain correction from the flat field alone did not work sufficiently well." ) );
    sf_back_fringescaleCheckBox->setText( tr( "Rescale model to match individual background levels" ) );
    QToolTip::add( sf_back_fringescaleCheckBox, tr( "Uncheck this box if you do not want the model to be scaled to the sky background of individual exposures; for example, if you have large and bright galaxies in the images, then rescaling would lead to an overestimation of the background.<br>" ) );
    sf_back_illumsmoothLineEdit->setText( QString::null );
    QToolTip::add( sf_back_illumsmoothLineEdit, tr( "Size of the smoothing kernel. Leave this field EMPTY if the background model should NOT be smoothed." ) );
    sf_textLabel1->setText( tr( "Smoothing scale for background model:" ) );
    QToolTip::add( sf_textLabel1, tr( "The kernel width used for smoothing the SUPERFLAT." ) );
    sf_textLabel1_2->setText( tr( "Smoothing scale for fringing model:" ) );
    QToolTip::add( sf_textLabel1_2, tr( "The kernel width used for smoothing the SUPERFLAT." ) );
    textLabel2_2_2->setText( tr( "<b><font color=\"#aa0000\">Collapse correction</font></b>" ) );
    collapseregion_groupbox->setTitle( QString::null );
    QToolTip::add( ccc_collymaxLineEdit, tr( "Upper limit for the excluded region" ) );
    QToolTip::add( ccc_collxmaxLineEdit, tr( "Right limit for the excluded region" ) );
    textLabel4_2->setText( tr( "xmin | xmax" ) );
    textLabel5_2->setText( tr( "ymin | ymax" ) );
    textLabel6_2->setText( tr( "<b>Exclude this region</b>" ) );
    QToolTip::add( ccc_collxminLineEdit, tr( "Left limit for the excluded region" ) );
    QToolTip::add( ccc_collyminLineEdit, tr( "Lower limit for the excluded region" ) );
    QWhatsThis::add( ccc_collyminLineEdit, QString::null );
    groupBox17->setTitle( QString::null );
    textLabel3_3->setText( tr( "Rejection threshold" ) );
    textLabel2_3->setText( tr( "Collapse direction" ) );
    textLabel1_16_3->setText( tr( "<b>Collapse parameters</b>" ) );
    QToolTip::add( ccc_collrejectthreshLineEdit, tr( "Rejection threshold for the averaging process" ) );
    ccc_colldirComboBox->clear();
    ccc_colldirComboBox->insertItem( tr( "x" ) );
    ccc_colldirComboBox->insertItem( tr( "y" ) );
    ccc_colldirComboBox->insertItem( tr( "xy" ) );
    ccc_colldirComboBox->insertItem( tr( "xyyx" ) );
    ccc_colldirComboBox->insertItem( tr( "yxxy" ) );
    ccc_colldirComboBox->insertItem( tr( "xxxx" ) );
    ccc_colldirComboBox->insertItem( tr( "yyyy" ) );
    groupBox23->setTitle( QString::null );
    textLabel1_2->setText( tr( "DT | DMIN" ) );
    QToolTip::add( ccc_colldetectminareaLineEdit, tr( "Minimum number of connected pixels above the detection threshold; if left empty, no detection will take place" ) );
    textLabel2_11->setText( tr( "<b>Mask objects so that they cannot<br>\n"
"contribute to the background model</b>" ) );
    ccc_collautothresholdCheckBox->setText( tr( "Automatic revision of DT" ) );
    QToolTip::add( ccc_collautothresholdCheckBox, tr( "If activated, the DT parameter is automatically cross-checked. If a larger value is found to be better, then the user-supplied value is overriden.<br>" ) );
    textLabel1_17_3->setText( tr( "Mask expansion factor:" ) );
    QToolTip::add( ccc_colldetectthreshLineEdit, tr( "Detection threshold; if left empty, no detection will take place" ) );
    ccc_maskactionCheckBox->setText( tr( "Store object masks" ) );
    QToolTip::add( ccc_maskactionCheckBox, tr( "Keeps the object masks for visual inspection." ) );
    ccc_collmaskexpandfactorLineEdit->setText( QString::null );
    QToolTip::add( ccc_collmaskexpandfactorLineEdit, tr( "If not empty, pixels within the scaled best-fit SExtractor ellipse will be set to zero. In this way very faint flux invisible in an individual image can be caught as well. Good starting value: 3.0 <br>" ) );
    textLabel2_2_2_2_3->setText( tr( "<b><font color=\"#aa0000\">Create binned preview</font></b>" ) );
    groupBox30->setTitle( QString::null );
    textLabel1_8->setText( tr( "Binning:" ) );
    QToolTip::add( w_binsizeSpinBox, tr( "The binning factor for the mosaic" ) );
    QToolTip::add( w_binmaxLineEdit, tr( "The images have their mode subtracted to ensure consistent min/max levels independent of the sky background. Enter here the upper threshold for the FITS -> TIFF conversion.<br>" ) );
    QToolTip::add( w_binminLineEdit, tr( "The images have their mode subtracted to ensure consistent min/max levels independent of the sky background. Enter here the lower threshold for the FITS -> TIFF conversion.<br>" ) );
    textLabel3_7->setText( tr( "TIFF max:" ) );
    textLabel2_9->setText( tr( "TIFF min:" ) );
    w_binoutlierCheckBox->setText( tr( "Outlier rejection" ) );
    QToolTip::add( w_binoutlierCheckBox, tr( "Set this switch if you want hot pixels etc masked in the binned image." ) );
    defectdetectionGroupBox->setTitle( QString::null );
    textLabel1_5->setText( tr( "Defect detection" ) );
    textLabel2_6->setText( tr( "Smoothing size" ) );
    textLabel3_6->setText( tr( "Row tolerance" ) );
    textLabel4_4->setText( tr( "Column tolerance" ) );
    textLabel5_5->setText( tr( "Cluster tolerance" ) );
    QToolTip::add( defect_clustol_sfLineEdit, tr( "Pixels that deviate by this fraction or more from the pixels in the local neighbourhood are masked (value:  e.g. 0.05 to 0.3)<br>" ) );
    QToolTip::add( defect_kernelsize_sfLineEdit, tr( "The superflat will be smoothed by a kernel of this size. The unsmoothed flat is then divided by the smoothed flat to remove large-scale effects. The resulting image can be used to search for chip defects.<br>" ) );
    QToolTip::add( defect_coltolLineEdit, tr( "Columns that deviate by that fraction or more from the other columns are masked.(value: e.g. 0.02)<br>" ) );
    QToolTip::add( defect_rowtol_sfLineEdit, tr( "Rows that deviate by that fraction or more from the other rows are masked (value: e.g. 0.02)<br>" ) );
    textLabel1_15->setText( tr( "FLAT" ) );
    QToolTip::add( defect_clustolLineEdit, tr( "Pixels that deviate by this fraction or more from the pixels in the local neighbourhood are masked (value:  e.g. 0.05 to 0.3)<br>" ) );
    QToolTip::add( defect_coltol_sfLineEdit, tr( "Columns that deviate by that fraction or more from the other columns are masked.(value: e.g. 0.02)<br>" ) );
    QToolTip::add( defect_kernelsizeLineEdit, tr( "The normalised flat-field will be smoothed by a kernel of this size. The unsmoothed flat is then divided by the smoothed flat to remove large-scale effects. The resulting image can be used to search for chip defects.<br>" ) );
    QToolTip::add( defect_rowtolLineEdit, tr( "Rows that deviate by that fraction or more from the other rows are masked (value: e.g. 0.02)<br>" ) );
    textLabel2_12->setText( tr( "BACKGROUND" ) );
    textLabel2_2_2_2->setText( tr( "<b><font color=\"#aa0000\">Create global weights</font></b>" ) );
    groupBox24->setTitle( QString::null );
    globw_uniformweightCheckBox->setText( tr( "Same weight for all pixels" ) );
    QToolTip::add( globw_uniformweightCheckBox, tr( "If you do not want the normalised flat to form the basis for the weights, then activate this switch" ) );
    textLabel14->setText( tr( "FLAT_norm" ) );
    textLabel13->setText( tr( "max" ) );
    QToolTip::add( globw_darkmaxLineEdit, tr( "Upper threshold for pixels in the masterdark" ) );
    QToolTip::add( globw_darkminLineEdit, tr( "Lower threshold for pixels in the masterdark" ) );
    globw_flatmaxLineEdit->setText( tr( "1.6" ) );
    QToolTip::add( globw_flatmaxLineEdit, tr( "Upper threshold for pixels in the normalised flat" ) );
    textLabel12->setText( tr( "min" ) );
    globw_flatminLineEdit->setText( tr( "0.5" ) );
    QToolTip::add( globw_flatminLineEdit, tr( "Lower threshold for pixels in the normalised flat" ) );
    textLabel15->setText( tr( "DARK / BIAS" ) );
    globw_usebiasCheckBox->setText( tr( "Use BIAS instead of DARK" ) );
    textLabel2_2_2_3->setText( tr( "<b><font color=\"#aa0000\">Create WEIGHTs</font></b>" ) );
    groupBox25->setTitle( QString::null );
    textLabel17->setText( tr( "Image min ADU" ) );
    textLabel17_2->setText( tr( "Image max ADU" ) );
    textLabel1_11->setText( tr( "Cosmics DT" ) );
    textLabel1_11_2->setText( tr( "Cosmics DMIN" ) );
    textLabel18->setText( tr( "Cosmics threshold" ) );
    weight_cosmicLineEdit->setText( tr( "0.1" ) );
    QToolTip::add( weight_cosmicLineEdit, tr( "Increase this value for slightly undersampled data to a value higher than 10, 100 or even 1000 to avoid masked stellar cores. If you do not want any masking of cosmics to take place, then leave this field empty.<br>" ) );
    QToolTip::add( weight_cosmicDMINLineEdit, tr( "Minimum number of connected pixels that make up a cosmic" ) );
    QToolTip::add( weight_cosmicDTLineEdit, tr( "Detection threshold of a pixel affected by a cosmic" ) );
    weight_threshmaxLineEdit->setText( QString::null );
    QToolTip::add( weight_threshmaxLineEdit, tr( "Pixels above this value will be set to zero in the WEIGHT" ) );
    weight_threshminLineEdit->setText( QString::null );
    QToolTip::add( weight_threshminLineEdit, tr( "Pixels below this value will be set to zero in the WEIGHT" ) );
    weight_bloom2TextLabel->setText( tr( "Minimum area" ) );
    weight_bloomCheckBox->setText( tr( "Mask blooming spikes" ) );
    QToolTip::add( weight_bloomCheckBox, tr( "Detects blooming spikes and sets their weight to zero" ) );
    weight_bloom3TextLabel->setText( tr( "Blooming margin" ) );
    weight_bloom1TextLabel->setText( tr( "Lower brightness limit" ) );
    QToolTip::add( weight_bloomlowlimitLineEdit, tr( "A lower brightness threshold for the detection of blooming spikes." ) );
    QToolTip::add( weight_bloomwidthLineEdit, tr( "Extends the dynamic range in a blooming spike by this value on the bright and the faint end. This is useful if the spike is not fully detected automatically. Default setting: 0 (empty)<br>" ) );
    QToolTip::add( weight_bloomminareaLineEdit, tr( "The minimum number of pixels in the blooming spike." ) );
    textLabel2_2_2_3_2_2->setText( tr( "<b><font color=\"#aa0000\">Absolute photometry (indirect)</font></b>" ) );
    groupBox26_2_2->setTitle( QString::null );
    textLabel1_10->setText( tr( "REFERENCE CATALOG" ) );
    absphot_indirect_stdcatComboBox->clear();
    absphot_indirect_stdcatComboBox->insertItem( tr( "LANDOLT_UBVRI" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "STETSON_UBVRI" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "STRIPE82_ugriz" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "STRIPE82_u'g'r'i'z'" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "SMITH_u'g'r'i'z'" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "MKO_JHK" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "HUNT_JHK" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "2MASSfaint_JHK" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "PERSSON_JHKKs" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "JAC_YJHKLM" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "MKO_LM" ) );
    absphot_indirect_stdcatComboBox->insertItem( tr( "WASHINGTON" ) );
    groupBox14->setTitle( QString::null );
    absphot_wcserrorCheckBox->setText( tr( "DO ASTROMETRY" ) );
    QToolTip::add( absphot_wcserrorCheckBox, tr( "Uncheck this switch if NO astrometric solution should be attempted (e.g. because the source density of the standard star field is too low). Provide the uncertainty of the original WCS in the field below.<br>" ) );
    QToolTip::add( absphot_apertureLineEdit, tr( "Diameter of the photometric aperture in pixels" ) );
    QToolTip::add( absphot_extinctionLineEdit, tr( "The extinction coefficient if it should be fixed" ) );
    QToolTip::add( absphot_colorfixedLineEdit, tr( "The color term if it should be fixed" ) );
    QToolTip::add( absphot_zpminLineEdit, tr( "The brightest acceptable limit of the zeropoint" ) );
    QToolTip::add( absphot_zpmaxLineEdit, tr( "The faintest acceptable limit of the zeropoint" ) );
    absphot_kappaLineEdit->setText( tr( "2.0" ) );
    QToolTip::add( absphot_kappaLineEdit, tr( "The kappa-sigma rejection threshold for sources during the iterative fit" ) );
    absphot_thresholdLineEdit->setText( tr( "0.15" ) );
    QToolTip::add( absphot_thresholdLineEdit, tr( "Sources this many mag below the fit will be rejected (likely non-photometric exposures)" ) );
    QToolTip::add( absphot_stdminmagLineEdit, tr( "Stars brighter than this magnitude will not be taken into account during the zeropoint determination.<br>" ) );
    absphot_calmodeComboBox->clear();
    absphot_calmodeComboBox->insertItem( tr( "RUNCALIB" ) );
    absphot_calmodeComboBox->insertItem( tr( "NIGHTCALIB" ) );
    QToolTip::add( absphot_calmodeComboBox, tr( "Choose whether a combined photometric solution is calculated for ALL nights (RUNCALIB), or whether each night is calibrated individually (NIGHTCALIB)<br>" ) );
    absphot_maxiterLineEdit->setText( tr( "10" ) );
    QToolTip::add( absphot_maxiterLineEdit, tr( "The maximum number of iterations" ) );
    absphot_wcserrorPushButton->setText( tr( "README" ) );
    textLabel1_12->setText( tr( "<b>You observed an external standard star field, and want to calibrate other exposures with it.</b>" ) );
    textLabel19_2_2_2_2->setText( tr( "APERTURE diameter (pixel)" ) );
    textLabel21_2_2_2->setText( tr( "FILTER and COLOR in the std. catalog" ) );
    textLabel21_2_3->setText( tr( "EXTINCTION (if fixed; must be negative!)" ) );
    textLabel20_2_2->setText( tr( "COLOR TERM (if fixed)" ) );
    textLabel20_2_3_2->setText( tr( "ZEROPOINT min | max" ) );
    textLabel20_2_3_2_2->setText( tr( "Kappa  | Threshold" ) );
    textLabel20_2_3_2_2_2_3_2->setText( tr( "Max. number of iterations" ) );
    textLabel20_2_3_2_2_2_3->setText( tr( "Calibration mode" ) );
    textLabel20_2_3_2_2_2_3_2_2->setText( tr( "Convergence criterion" ) );
    textLabel20_2_3_2_2_2->setText( tr( "MINIMUM MAG for standard catalog" ) );
    absphot_convergenceLineEdit->setText( tr( "0.01" ) );
    QToolTip::add( absphot_convergenceLineEdit, tr( "Convergence is achieved if ZP, ExtCoeff and ColCoeff change by less than this amount at the same time." ) );
    QToolTip::add( absphot_wcserrorLineEdit, tr( "Provide here the uncertainty of the original WCS if NO astrometric solution shall be attempted.<br>" ) );
    textLabel1_16->setText( tr( "<b>About std. star astrometry</b>" ) );
    QToolTip::add( absphot_filterLineEdit, tr( "The name of the filter in which you observed" ) );
    textLabel19_2_2_2->setText( tr( "FILTER keyword in observation" ) );
    textLabel20_2_3_2_2_2_2->setText( tr( "WCS uncertainty [arcsec]" ) );
    textLabel2_2_2_3_2_2_2->setText( tr( "<b><font color=\"#aa0000\">Absolute photometry (direct)</font></b>" ) );
    groupBox26_2_2_2->setTitle( QString::null );
    textLabel1_10_2->setText( tr( "REFERENCE CATALOG" ) );
    absphot_direct_stdcatComboBox->clear();
    absphot_direct_stdcatComboBox->insertItem( tr( "SDSS" ) );
    absphot_direct_stdcatComboBox->insertItem( tr( "2MASS" ) );
    groupBox14_2->setTitle( QString::null );
    textLabel3_9_2->setText( tr( "MAX PHOTOMETRIC ERROR (mag)" ) );
    textLabel21_2_2_2_2_2->setText( tr( "FILTER IN WHICH YOU OBSERVED" ) );
    textLabel4_5_2->setText( tr( "FITTING METHOD" ) );
    absphot_fittingmethodM2_ComboBox->clear();
    absphot_fittingmethodM2_ComboBox->insertItem( tr( "ZP for each chip" ) );
    absphot_fittingmethodM2_ComboBox->insertItem( tr( "ZP for entire mosaic" ) );
    QToolTip::add( absphot_fittingmethodM2_ComboBox, tr( "Select if you want each chip to be calibrated independently (images get scaled to the same zeropoint), or if you want to treat the chips as one photometrically stable mosaic.<br>" ) );
    QToolTip::add( absphot_stdfilterM2_ComboBox, tr( "Select in which filters your observations were made." ) );
    QToolTip::add( absphot_maxphoterrorLineEdit, tr( "The maximum measurement error for sources in your data that go into the fit.<br>" ) );
    textLabel5_6_3->setText( tr( "<font color=\"#00007f\">This task makes use of the parameter settings<br>\n"
"for both object detection (<b>Create source cat</b>) and<br>\n"
"astrometric solution (<b>Astro+photometry</b>, with Scamp).<br>\n"
"Make sure you enter a working configuration.</font>" ) );
    textLabel2_11_2->setText( tr( "<b>Your target overlaps with SDSS or 2MASS,<br>\n"
"and you want to calibrate directly against it.</b>" ) );
    textLabel2_2_2_3_2->setText( tr( "<b><font color=\"#aa0000\">Create source cat</font></b>" ) );
    groupBox26_2->setTitle( QString::null );
    textLabel19_2->setText( tr( "DEBLEND_MINCONT" ) );
    textLabel21_2->setText( tr( "Lower FWHM limit" ) );
    QToolTip::add( ap_deblendLineEdit, tr( "Minimum contrast for the object detection of SExtractor. Increase this value if you want LESS sources detected within a larger source, e.g. a globular cluster or galaxy.<br>" ) );
    QToolTip::add( ap_detectminareaLineEdit, tr( "Minimum number of connected pixels above the detection threshold. Decrease this value if you want more objects. Minimum useful value: 2-4, depending on sampling." ) );
    QToolTip::add( ap_detectthreshLineEdit, tr( "Minimum detection threshold for the object detection. Decrease this value if you want more sources. Minimum useful value: 1.0-1.5" ) );
    textLabel19_2_2->setText( tr( "DETECT_THRESH" ) );
    textLabel21_2_2->setText( tr( "DETECT_MINAREA" ) );
    textLabel20_2->setText( tr( "Maximum FLAG" ) );
    QToolTip::add( ap_flagLineEdit, tr( "Objects with a SExtractor FLAG larger than this number will be removed from the catalog. Enter 0 if you want only clean sources, or 8 if you want to keep saturated sources as well.<br>" ) );
    QToolTip::add( ap_minfwhmLineEdit, tr( "Sources with a FWHM (in pixels) smaller than this value will be rejected from the catalogs (e.g. to get rid of hot pixels).<br> " ) );
    textLabel20_2_3->setText( tr( "Background level" ) );
    QToolTip::add( ap_sexbacklevelLineEdit, tr( "If the image contains a significant amount of NAN pixel values and you do not or cannot change this, then you should set the sky background level manually. Otherwise leave the field empty.<br>" ) );
    textLabel20_2_3_3->setText( tr( "Min. # of objects" ) );
    QToolTip::add( ap_lownumLineEdit, tr( "The catalog/image must contain at least this many objects (after all filterings) in order to be kept for astrometry.<br>" ) );
    QToolTip::add( ap_saturationLineEdit, tr( "The ADU level at which saturation occurs. This can be pretty low for near-IR cameras after sky subtraction, or for 12bit DSLRs. 50000 ADU if left empty.<br>" ) );
    textLabel20_2_3_3_2->setText( tr( "Saturation level" ) );
    ap_filterCheckBox->setText( tr( "Data is well sampled,\n"
"but plagued with hot pixels" ) );
    QToolTip::add( ap_filterCheckBox, tr( "If your object catalogues are plagued by hot pixels, then try this option (runs an additional outlier rejection)." ) );
    textLabel2_2_2_2_2->setText( tr( "<b><font color=\"#aa0000\">Astro+photometry</font></b>" ) );
    groupBox16->setTitle( QString::null );
    textLabel1_3->setText( tr( "POSANGLE_MAXERR (deg)" ) );
    textLabel2_4->setText( tr( "POSITION_MAXERR (arcmin)" ) );
    textLabel1_7->setText( tr( "PIXSCALE_MAXERR (factor)" ) );
    textLabel4_3->setText( tr( "DISTORT_DEGREES" ) );
    textLabel6_3->setText( tr( "MOSAIC_TYPE" ) );
    ac_sc_stabilityComboBox->clear();
    ac_sc_stabilityComboBox->insertItem( tr( "INSTRUMENT" ) );
    ac_sc_stabilityComboBox->insertItem( tr( "EXPOSURE" ) );
    textLabel2_5->setText( tr( "PHOTINSTRU_KEY" ) );
    textLabel1->setText( tr( "FOCAL_PLANE" ) );
    textLabel3_4->setText( tr( "STABILITY_TYPE" ) );
    ac_sc_mosaictypeComboBox->clear();
    ac_sc_mosaictypeComboBox->insertItem( tr( "UNCHANGED" ) );
    ac_sc_mosaictypeComboBox->insertItem( tr( "SAME_CRVAL" ) );
    ac_sc_mosaictypeComboBox->insertItem( tr( "SHARE_PROJAXIS" ) );
    ac_sc_mosaictypeComboBox->insertItem( tr( "FIX_FOCALPLANE" ) );
    ac_sc_mosaictypeComboBox->insertItem( tr( "LOOSE" ) );
    ac_sc_mtypereadmePushButton->setText( tr( "README" ) );
    ac_sc_FPComboBox->clear();
    ac_sc_FPComboBox->insertItem( tr( "Use default FP" ) );
    ac_sc_FPComboBox->insertItem( tr( "Create new FP" ) );
    ac_sc_FPComboBox->insertItem( tr( "Do not use FP info" ) );
    QToolTip::add( ac_sc_FPComboBox, tr( "Multichip Camera: Choose if you want to use an existing focal plane configuration or if you want to determine it again based on the current data. Should be used with MOSAIC_TYPE set to SAME_CRVAL for multichip cameras. If no FP info is going to be used, set MOSAIC_TYPE to UNCHANGED.<br>" ) );
    textLabel2_5_2->setText( tr( "ASTRINSTRU_KEY" ) );
    textLabel2_5_2_2->setText( tr( "FGROUP_RADIUS (deg)" ) );
    textLabel2_5_3->setText( tr( "CROSSID_RADIUS (arcsec)" ) );
    textLabel2_5_3_2->setText( tr( "ASTREF_WEIGHT" ) );
    QToolTip::add( ac_sc_astrinstrukeyLineEdit, tr( "FITS header key that is used to group exposures together in the astrometric solution (default: FILTER)<br>" ) );
    QToolTip::add( ac_sc_photinstrukeyLineEdit, tr( "FITS header key that is used to group exposures together in the photometric solution (default: FILTER)<br>" ) );
    QToolTip::add( ac_sc_astrefweightLineEdit, tr( "The relative weight for the astrometric reference catalogue<br>" ) );
    QToolTip::add( ac_sc_crossidradiusLineEdit, tr( "This tolerance should be about 5-10 times the pixel scale, e.g. about 2 for 0.2 arcsec pixels. For instruments with small distortions it can be smaller.<br>" ) );
    QToolTip::add( ac_sc_fgroupradiusLineEdit, tr( "Maximum distance between pointings to be included in one coaddition. Maximum value: 180.<br>" ) );
    QToolTip::add( ac_sc_distortorderLineEdit, tr( "The degree of the distortion polynomial. 1 means no distortion, 2 means linear variation of the pixel scale, 3 means quadratic variation of the pixel scale, etc. Limited to values [1...9].<br>" ) );
    QToolTip::add( ac_sc_maxscaleLineEdit, tr( "The maximum error allowed in the pixel scale. A value up to 2 can be entered.<br>" ) );
    QToolTip::add( ac_sc_maxoffsetLineEdit, tr( "The maximum uncertainty in the CRPIX1/2 value" ) );
    QToolTip::add( ac_sc_maxposangleLineEdit, tr( "The maximum uncertainty in the position angle of your data" ) );
    textLabel2_5_2_3->setText( tr( "Additional DISTORT_KEYS" ) );
    QToolTip::add( ac_sc_distortkeysLineEdit, tr( "Comma-separated list of additional distortion keys. FITS keywords must be preceeded with a \":\". Can usually be left empty apart from particularly tricky and instable cases.<br>" ) );
    textLabel4_3_2->setText( tr( "Add. DISTORT_GROUPS | DEGREES" ) );
    QToolTip::add( ac_sc_distortorder2LineEdit, tr( "Comma-separated list of additional distortion degrees. Can usually be left empty apart from particularly tricky and instable cases.<br>" ) );
    QToolTip::add( ac_sc_maxSNLineEdit, tr( "The minimum S/N for a source to be kept for the high-S/N statistics" ) );
    QToolTip::add( ac_sc_minSNLineEdit, tr( "The minimum S/N for a source to be kept in SCAMP" ) );
    textLabel5_3->setText( tr( "SN_THRESHOLDS (low | high)" ) );
    QToolTip::add( ac_sc_distortgroupsLineEdit, tr( "Comma-separated list of additional distortion groups. Can usually be left empty apart from particularly tricky and instable cases.<br>" ) );
    ac_sc_matchflippedCheckBox->setText( tr( "Match flipped images" ) );
    ac_sc_matchWCSCheckBox->setText( tr( "Match WCS" ) );
    QToolTip::add( ac_sc_matchWCSCheckBox, tr( "By default, Scamp will try to match images to sky coordinates (i.e. a reference catalog). You can switch this off if the header coordinates are within CROSSID_RADIUS of the real sky position.<br>" ) );
    QWhatsThis::add( ac_sc_matchWCSCheckBox, QString::null );
    astrommethod_TabWidget->changeTab( tab, tr( "Scamp" ) );
    groupBox16_2->setTitle( QString::null );
    textLabel1_7_2->setText( tr( "Relative uncertainty of the pixel scale" ) );
    QToolTip::add( ac_an_maxscaleLineEdit, tr( "The maximum relative error allowed in the pixel scale (default: 1.05)<br>" ) );
    QToolTip::add( ac_an_distortorderLineEdit, tr( "The degree of the astrometry.net SIP distortion polynomials. If left empty, no distortion will be fitted. For the time being, the SIP coefficients will be ignored as SWarp cannot interpret them.<br>" ) );
    ac_an_scampdistortCheckBox->setText( tr( "Calculate SCAMP distortion after ANET matching" ) );
    QToolTip::add( ac_an_scampdistortCheckBox, tr( "If you want distortion correction with astrometry.net in THELI, you have to calculate it with Scamp. Simply activate this CheckBox. DISTORT_DEGREES, ASTRINSTRU_KEY and STABILITY_TYPE will be taken from the Scamp configuration.<br>" ) );
    textLabel4_3_3->setText( tr( "Distortion polynomial degree\n"
"(currently ignored)" ) );
    astrommethod_TabWidget->changeTab( tab_2, tr( "Astrometry.net" ) );
    absphot_groupbox->setTitle( QString::null );
    absphot_textlabel->setText( tr( "<b>Based on the <tt>STD/calib/night_ID_FILTER.png</tt> checkplots, select one or more photometric solutions that should be used for calibrating the coadded image.</b>" ) );
    NIGHT_5_PushButton->setText( tr( "pushButton14" ) );
    QToolTip::add( NIGHT_5_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_4_PushButton->setText( tr( "pushButton13" ) );
    QToolTip::add( NIGHT_4_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_3_PushButton->setText( tr( "pushButton12" ) );
    QToolTip::add( NIGHT_3_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_6_PushButton->setText( tr( "pushButton15" ) );
    QToolTip::add( NIGHT_6_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_2_PushButton->setText( tr( "pushButton11" ) );
    QToolTip::add( NIGHT_2_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_1_PushButton->setText( tr( "pushButton10" ) );
    QToolTip::add( NIGHT_1_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_1_ComboBox->clear();
    NIGHT_1_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_1_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_1_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_1_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_1_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_2_ComboBox->clear();
    NIGHT_2_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_2_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_2_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_2_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_2_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_3_ComboBox->clear();
    NIGHT_3_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_3_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_3_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_3_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_3_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_4_ComboBox->clear();
    NIGHT_4_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_4_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_4_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_4_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_4_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_5_ComboBox->clear();
    NIGHT_5_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_5_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_5_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_5_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_5_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_6_ComboBox->clear();
    NIGHT_6_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_6_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_6_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_6_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_6_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_10_PushButton->setText( tr( "pushButton19" ) );
    QToolTip::add( NIGHT_10_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_10_ComboBox->clear();
    NIGHT_10_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_10_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_10_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_10_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_10_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_9_PushButton->setText( tr( "pushButton18" ) );
    QToolTip::add( NIGHT_9_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_9_ComboBox->clear();
    NIGHT_9_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_9_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_9_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_9_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_9_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_8_PushButton->setText( tr( "pushButton17" ) );
    QToolTip::add( NIGHT_8_PushButton, tr( "Shows the photometric calibration of that night" ) );
    NIGHT_8_ComboBox->clear();
    NIGHT_8_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_8_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_8_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_8_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_8_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_7_ComboBox->clear();
    NIGHT_7_ComboBox->insertItem( tr( "Reject night" ) );
    NIGHT_7_ComboBox->insertItem( tr( "Use fit #1" ) );
    NIGHT_7_ComboBox->insertItem( tr( "Use fit #2" ) );
    NIGHT_7_ComboBox->insertItem( tr( "Use fit #3" ) );
    NIGHT_7_ComboBox->insertItem( tr( "Use fit #4" ) );
    NIGHT_7_PushButton->setText( tr( "pushButton16" ) );
    QToolTip::add( NIGHT_7_PushButton, tr( "Shows the photometric calibration of that night" ) );
    astrommethod_TabWidget->changeTab( TabPage, tr( "Absolute photometry" ) );
    textLabel2_2_2_2_2_2->setText( tr( "<b><font color=\"#aa0000\">Coaddition</font></b>" ) );
    textLabel2_8->setText( tr( "Proper motion \n"
"in RA / DEC" ) );
    coc_filterTextLabel->setText( tr( "Coadd this FILTER:" ) );
    textLabel1_2_2->setText( tr( "Coadd these chips:" ) );
    textLabel1_4->setText( tr( "Coadd image size (x, y):" ) );
    textLabel3_5_2->setText( tr( "Sky position angle [deg]:" ) );
    QToolTip::add( coc_skyposangleLineEdit, tr( "The sky position angle of the coadded image. A positive angle means a rotation is performed from North over East, i.e. the image is rotated clockwise. If zero is entered or the field left empty, then North will be up and East left. If the data was taken with non-zero position angle, then entering the same value will result in an 'unrotated' coadded image.<br>" ) );
    coc_speedunitComboBox->clear();
    coc_speedunitComboBox->insertItem( tr( "[ \" / sec ]" ) );
    coc_speedunitComboBox->insertItem( tr( "[ \" / min ]" ) );
    coc_speedunitComboBox->insertItem( tr( "[ \" / hour ]" ) );
    coc_speedunitComboBox->insertItem( tr( "[ \" / day ]" ) );
    QToolTip::add( coc_coaddsizeyLineEdit, tr( "The x and y size of the coadded image in pixels. If left empty, it will be determined automatically.<br>" ) );
    QToolTip::add( coc_coaddsizexLineEdit, tr( "The x and y size of the coadded image in pixels. If left empty, it will be determined automatically.<br>" ) );
    QToolTip::add( coc_propmotionraLineEdit, tr( "The proper motion vector of a moving target. The coadded images get shifted accordingly.<br>" ) );
    QToolTip::add( coc_propmotiondecLineEdit, tr( "The proper motion vector of a moving target. The coadded images get shifted accordingly.<br>" ) );
    QToolTip::add( coc_filterComboBox, tr( "If images taken with different filters are in the SCIENCE directory, which ones do you want to coadd?<br>" ) );
    coc_skypafromimagePushButton->setText( tr( "From image" ) );
    textLabel3_5->setText( tr( "Pixel scale [\"]:" ) );
    QToolTip::add( coc_smoothedgeLineEdit, tr( "The length over which overlapping edges in a mosaic should be blended into each other.<br>" ) );
    QToolTip::add( coc_pixscaleLineEdit, tr( "Resampling pixel scale" ) );
    QToolTip::add( coc_RZPLineEdit, tr( "Images with a relative zeropoint lower than this value will not be coadded." ) );
    textLabel3_5_2_2->setText( tr( "Max. seeing [\"] | Min RZP:" ) );
    QToolTip::add( coc_seeingLineEdit, tr( "Images with a seeing larger than this value will not be coadded." ) );
    QToolTip::add( coc_chipsLineEdit, tr( "Blank-separated list of the chips that shall be coadded" ) );
    QToolTip::add( coc_identLineEdit, tr( "Unique identifier for the coaddition. The filter chosen is appended automatically to this string. Max string length: 37 chars<br>" ) );
    QToolTip::add( coc_refraLineEdit, tr( "Projection reference coordinate: right ascension" ) );
    textLabel1_6->setText( tr( "Identification string:" ) );
    coc_refraTextLabel->setText( tr( "RA | DEC:" ) );
    QToolTip::add( coc_refdecLineEdit, tr( "Projection reference coordinate: declination" ) );
    textLabel1_9->setText( tr( "Edge smoothing length (mosaics):" ) );
    QToolTip::add( coc_filterthresholdLineEdit, tr( "Rejection threshold in units of sigma" ) );
    textLabel1_14->setText( tr( "<b>Outlier <br>\n"
"rejection:</b>" ) );
    textLabel2_10->setText( tr( "Cluster size:" ) );
    textLabel5_4->setText( tr( "Threshold:" ) );
    QToolTip::add( coc_filterborderwidthLineEdit, tr( "If a pixel is bad, then mask a border of this width around it, too. If set to 1 (2), a cluster of 3x3 (5x5) pixels centred on the bad pixel will be masked, too, and so on.<br>" ) );
    QToolTip::add( coc_filterclustersizeLineEdit, tr( "Only mask pixel groups consisting of at least this many bad pixels (values larger than 9 have no effect and will be reset to 9).<br>" ) );
    textLabel3_8->setText( tr( "Border width:" ) );
    cc_projectionTextLabel->setText( tr( "Projection:" ) );
    QWhatsThis::add( cc_projectionTextLabel, tr( "For fields less than 10 degrees in size the differences in projection are small. You can go with the traditional tangential (TAN) or conic equal-area (COE) projection. For larger fields TAN becomes inadequate, and equal-area projections (blue) are of interest since they preserve surface brightness.<p><b>Zenithal projections</b><br>AZP - Zenithal perspective<br>TAN - Distorted tangential<br>STG - Stereographic<br>SIN - Slant orthographic<br>ARC - Zenithal equidistant<br><font color=\"#0000ff\">ZPN - Zenithal polynomial</font><br><font color=\"#0000ff\">ZEA - Zenithal equal-area</font><br>AIR - Airy</p><p><b>Cylindrical projections</b><br>CYP - Cylindrical perspective<br><font color=\"#0000ff\">CEA - Cylindrical equal-area</font><br>CAR - Plate carree<br>MER - Mercator</p><p><b>Conic projections</b><br>COP - Conic perspective<br><font color=\"#0000ff\">COE - Conic equal-area</font><br>COD - Conic equidistant<br>COO - Conic orthomorphic</p><p><b>Pseudoconic and polyconic projections</b><br><font color=\"#0000ff\">BON - Bonne's equal area</font><br>PCO - Polyconic</p><p><b>Pseudocylindrical projections</b><br><font color=\"#0000ff\">GLS - Global sinusoidal (Sanson-Flamsteed)</font><br><font color=\"#0000ff\">PAR - Parabolic</font><br><font color=\"#0000ff\">MOL - Mollweide</font><br><font color=\"#0000ff\">AIT - Hammer-Aitoff</font> (Often used for all-sky projections)</p><p><b>Quad-cube projections</b><br>TSC - Tangential spherical cube<br>CSC - COBE quadrilateralised spherical cube<br><font color=\"#0000ff\">QSC - Quadrilateralised spherical cube</font><br><p>For further information and graphical representations of the various projection types see the <i>SWarp</i> documentation." ) );
    cc_typeTextLabel->setText( tr( "Celestial type:" ) );
    QWhatsThis::add( cc_typeTextLabel, tr( "NATIVE: Uses the celestial type present in the first image that goes into the coaddition.<br> The other types are self-explanatory" ) );
    textLabel1_4_2->setText( tr( "Combine type:" ) );
    coc_celestialtypeComboBox->clear();
    coc_celestialtypeComboBox->insertItem( tr( "EQUATORIAL" ) );
    coc_celestialtypeComboBox->insertItem( tr( "GALACTIC" ) );
    coc_celestialtypeComboBox->insertItem( tr( "ECLIPTIC" ) );
    coc_celestialtypeComboBox->insertItem( tr( "SUPERGALACTIC" ) );
    coc_celestialtypeComboBox->setCurrentItem( 0 );
    QToolTip::add( coc_celestialtypeComboBox, tr( "Celestial coordinate system. PIXEL means no (de)projection is used." ) );
    coc_projectionComboBox->clear();
    coc_projectionComboBox->insertItem( tr( "AZP" ) );
    coc_projectionComboBox->insertItem( tr( "TAN" ) );
    coc_projectionComboBox->insertItem( tr( "STG" ) );
    coc_projectionComboBox->insertItem( tr( "SIN" ) );
    coc_projectionComboBox->insertItem( tr( "ARC" ) );
    coc_projectionComboBox->insertItem( tr( "ZPN" ) );
    coc_projectionComboBox->insertItem( tr( "ZEA" ) );
    coc_projectionComboBox->insertItem( tr( "AIR" ) );
    coc_projectionComboBox->insertItem( tr( "CYP" ) );
    coc_projectionComboBox->insertItem( tr( "CEA" ) );
    coc_projectionComboBox->insertItem( tr( "CAR" ) );
    coc_projectionComboBox->insertItem( tr( "MER" ) );
    coc_projectionComboBox->insertItem( tr( "COP" ) );
    coc_projectionComboBox->insertItem( tr( "COE" ) );
    coc_projectionComboBox->insertItem( tr( "COD" ) );
    coc_projectionComboBox->insertItem( tr( "COO" ) );
    coc_projectionComboBox->insertItem( tr( "BON" ) );
    coc_projectionComboBox->insertItem( tr( "PCO" ) );
    coc_projectionComboBox->insertItem( tr( "GLS" ) );
    coc_projectionComboBox->insertItem( tr( "PAR" ) );
    coc_projectionComboBox->insertItem( tr( "MOL" ) );
    coc_projectionComboBox->insertItem( tr( "AIT" ) );
    coc_projectionComboBox->insertItem( tr( "TSC" ) );
    coc_projectionComboBox->insertItem( tr( "CSC" ) );
    coc_projectionComboBox->insertItem( tr( "QSC" ) );
    coc_projectionComboBox->insertItem( tr( "NONE" ) );
    coc_projectionComboBox->setCurrentItem( 1 );
    QToolTip::add( coc_projectionComboBox, tr( "Best choice: TAN or COE if field of view less than 10 deg. <br>For all-sky images AIT is a frequently used method. See also \"Help->What's this?\" for more information." ) );
    QWhatsThis::add( coc_projectionComboBox, tr( "For fields less than 10 degrees in size the differences in projection are small. You can go with the traditional tangential (TAN) or conic equal-area (COE) projection. For larger fields TAN becomes inadequate, and equal-area projections (<font color=\"#0000ff\">blue</font>) are of interest since they preserve surface brightness.<p><b>Zenithal projections</b><br>AZP - Zenithal perspective<br>TAN - Distorted tangential<br>STG - Stereographic<br>SIN - Slant orthographic<br>ARC - Zenithal equidistant<br><font color=\"#0000ff\">ZPN - Zenithal polynomial</font><br><font color=\"#0000ff\">ZEA - Zenithal equal-area</font><br>AIR - Airy</p><p><b>Cylindrical projections</b><br>CYP - Cylindrical perspective<br><font color=\"#0000ff\">CEA - Cylindrical equal-area</font><br>CAR - Plate carree<br>MER - Mercator</p><p><b>Conic projections</b><br>COP - Conic perspective<br><font color=\"#0000ff\">COE - Conic equal-area</font><br>COD - Conic equidistant<br>COO - Conic orthomorphic</p><p><b>Pseudoconic and polyconic projections</b><br><font color=\"#0000ff\">BON - Bonne's equal area</font><br>PCO - Polyconic</p><p><b>Pseudocylindrical projections</b><br><font color=\"#0000ff\">GLS - Global sinusoidal (Sanson-Flamsteed)</font><br><font color=\"#0000ff\">PAR - Parabolic</font><br><font color=\"#0000ff\">MOL - Mollweide</font><br><font color=\"#0000ff\">AIT - Hammer-Aitoff</font> (Often used for all-sky projections)</p><p><b>Quad-cube projections</b><br>TSC - Tangential spherical cube<br>CSC - COBE quadrilateralised spherical cube<br><font color=\"#0000ff\">QSC - Quadrilateralised spherical cube</font><br><p>For further information and graphical representations of the various projection types see the <i>SWarp</i> documentation." ) );
    coc_combineComboBox->clear();
    coc_combineComboBox->insertItem( tr( "WEIGHTED" ) );
    coc_combineComboBox->insertItem( tr( "MEDIAN" ) );
    coc_combineComboBox->insertItem( tr( "AVERAGE" ) );
    coc_combineComboBox->insertItem( tr( "MIN" ) );
    coc_combineComboBox->insertItem( tr( "MAX" ) );
    coc_combineComboBox->insertItem( tr( "CHI2" ) );
    QToolTip::add( coc_combineComboBox, tr( "The image combination method" ) );
    coc_kernelComboBox->clear();
    coc_kernelComboBox->insertItem( tr( "NEAREST" ) );
    coc_kernelComboBox->insertItem( tr( "BILINEAR" ) );
    coc_kernelComboBox->insertItem( tr( "LANCZOS2" ) );
    coc_kernelComboBox->insertItem( tr( "LANCZOS3" ) );
    coc_kernelComboBox->insertItem( tr( "LANCZOS4" ) );
    coc_kernelComboBox->setCurrentItem( 3 );
    QToolTip::add( coc_kernelComboBox, tr( "The kernel used for resampling. Best choice: LANCZOS3" ) );
    textLabel8_3->setText( tr( "Resampling kernel:" ) );
    QToolTip::add( coc_clipsigmaLineEdit, tr( "RMS error multiple variation allowed with clipping when using combine type CLIPPED" ) );
    coc_clipsigmaLabel->setText( tr( "CLIP_SIGMA" ) );
    coc_clipampfracLabel->setText( tr( "CLIP_AMPFRAC" ) );
    QToolTip::add( coc_clipampfracLineEdit, tr( "Fraction of flux variation allowed with clipping when using combine type CLIPPED" ) );
    coc_rescaleweightsCheckBox->setText( tr( "RESCALE_WEIGHTS" ) );
    QToolTip::add( coc_rescaleweightsCheckBox, tr( "Whether the weight images are to be automatically rescaled by SWarp. Switch ON for empty fields only. Large bright galaxies or nebula will distort the automatic rms measurement.<br>" ) );
}

