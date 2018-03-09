/****************************************************************************
** Form implementation generated from reading ui file 'color_form.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "color_form.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../color_form.ui.h"
/*
 *  Constructs a color_form as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
color_form::color_form( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "color_form" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    color_formLayout = new QGridLayout( this, 1, 1, 11, 6, "color_formLayout"); 

    layout39 = new QHBoxLayout( 0, 0, 6, "layout39"); 
    spacer62 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout39->addItem( spacer62 );

    color_closePushButton = new QPushButton( this, "color_closePushButton" );
    color_closePushButton->setMinimumSize( QSize( 100, 0 ) );
    color_closePushButton->setFocusPolicy( QPushButton::NoFocus );
    layout39->addWidget( color_closePushButton );

    color_formLayout->addLayout( layout39, 1, 0 );

    color_TabWidget = new QTabWidget( this, "color_TabWidget" );
    color_TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, color_TabWidget->sizePolicy().hasHeightForWidth() ) );
    color_TabWidget->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    color_TabWidget->setFocusPolicy( QTabWidget::NoFocus );

    tab = new QWidget( color_TabWidget, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 
    spacer47 = new QSpacerItem( 209, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout->addItem( spacer47, 0, 1 );
    spacer29_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( spacer29_2, 1, 0 );

    groupBox5 = new QGroupBox( tab, "groupBox5" );
    groupBox5->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    groupBox5->setColumnLayout(0, Qt::Vertical );
    groupBox5->layout()->setSpacing( 6 );
    groupBox5->layout()->setMargin( 11 );
    groupBox5Layout = new QGridLayout( groupBox5->layout() );
    groupBox5Layout->setAlignment( Qt::AlignTop );
    spacer5 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox5Layout->addItem( spacer5, 5, 0 );
    spacer9 = new QSpacerItem( 140, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox5Layout->addMultiCell( spacer9, 9, 9, 2, 3 );

    layout32 = new QGridLayout( 0, 1, 1, 0, 6, "layout32"); 

    textLabel10 = new QLabel( groupBox5, "textLabel10" );
    textLabel10->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, textLabel10->sizePolicy().hasHeightForWidth() ) );

    layout32->addMultiCellWidget( textLabel10, 0, 0, 0, 1 );

    color_xmaxLineEdit = new QLineEdit( groupBox5, "color_xmaxLineEdit" );
    color_xmaxLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, color_xmaxLineEdit->sizePolicy().hasHeightForWidth() ) );
    color_xmaxLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_xmaxLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_xmaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout32->addWidget( color_xmaxLineEdit, 1, 2 );

    color_xminLineEdit = new QLineEdit( groupBox5, "color_xminLineEdit" );
    color_xminLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, color_xminLineEdit->sizePolicy().hasHeightForWidth() ) );
    color_xminLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_xminLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_xminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout32->addWidget( color_xminLineEdit, 1, 1 );

    textLabel9 = new QLabel( groupBox5, "textLabel9" );
    textLabel9->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, textLabel9->sizePolicy().hasHeightForWidth() ) );

    layout32->addWidget( textLabel9, 1, 0 );

    textLabel9_4 = new QLabel( groupBox5, "textLabel9_4" );
    textLabel9_4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, textLabel9_4->sizePolicy().hasHeightForWidth() ) );

    layout32->addWidget( textLabel9_4, 2, 0 );

    color_ymaxLineEdit = new QLineEdit( groupBox5, "color_ymaxLineEdit" );
    color_ymaxLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, color_ymaxLineEdit->sizePolicy().hasHeightForWidth() ) );
    color_ymaxLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_ymaxLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_ymaxLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout32->addWidget( color_ymaxLineEdit, 2, 2 );

    color_yminLineEdit = new QLineEdit( groupBox5, "color_yminLineEdit" );
    color_yminLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, color_yminLineEdit->sizePolicy().hasHeightForWidth() ) );
    color_yminLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_yminLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_yminLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout32->addWidget( color_yminLineEdit, 2, 1 );

    groupBox5Layout->addMultiCellLayout( layout32, 9, 9, 0, 1 );

    color_imagesListBox = new QListBox( groupBox5, "color_imagesListBox" );
    color_imagesListBox->setMinimumSize( QSize( 240, 100 ) );
    color_imagesListBox->setMaximumSize( QSize( 300, 32767 ) );
    color_imagesListBox->setFocusPolicy( QListBox::ClickFocus );
    color_imagesListBox->setSelectionMode( QListBox::Extended );

    groupBox5Layout->addMultiCellWidget( color_imagesListBox, 6, 7, 0, 1 );

    color_removeimagePushButton = new QPushButton( groupBox5, "color_removeimagePushButton" );
    color_removeimagePushButton->setMinimumSize( QSize( 200, 0 ) );
    color_removeimagePushButton->setMaximumSize( QSize( 200, 32767 ) );
    color_removeimagePushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox5Layout->addWidget( color_removeimagePushButton, 6, 3 );

    color_coaddListBox = new QListBox( groupBox5, "color_coaddListBox" );
    color_coaddListBox->setMinimumSize( QSize( 240, 100 ) );
    color_coaddListBox->setFocusPolicy( QListBox::ClickFocus );
    color_coaddListBox->setSelectionMode( QListBox::Extended );

    groupBox5Layout->addMultiCellWidget( color_coaddListBox, 3, 4, 0, 1 );
    spacer8 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox5Layout->addItem( spacer8, 7, 2 );
    spacer44 = new QSpacerItem( 20, 70, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox5Layout->addItem( spacer44, 4, 3 );

    color_rereadimagePushButton = new QPushButton( groupBox5, "color_rereadimagePushButton" );
    color_rereadimagePushButton->setMinimumSize( QSize( 200, 0 ) );
    color_rereadimagePushButton->setMaximumSize( QSize( 200, 32767 ) );
    color_rereadimagePushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox5Layout->addWidget( color_rereadimagePushButton, 7, 3 );
    spacer63 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox5Layout->addItem( spacer63, 8, 0 );

    textLabel1_5 = new QLabel( groupBox5, "textLabel1_5" );

    groupBox5Layout->addMultiCellWidget( textLabel1_5, 2, 2, 0, 1 );

    textLabel1 = new QLabel( groupBox5, "textLabel1" );

    groupBox5Layout->addWidget( textLabel1, 1, 0 );

    color_getcoaddedimagesPushButton = new QPushButton( groupBox5, "color_getcoaddedimagesPushButton" );
    color_getcoaddedimagesPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox5Layout->addWidget( color_getcoaddedimagesPushButton, 3, 3 );

    color_selectdirPushButton = new QPushButton( groupBox5, "color_selectdirPushButton" );

    groupBox5Layout->addWidget( color_selectdirPushButton, 0, 3 );

    color_dirLineEdit = new QLineEdit( groupBox5, "color_dirLineEdit" );
    color_dirLineEdit->setMinimumSize( QSize( 0, 0 ) );
    color_dirLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox5Layout->addMultiCellWidget( color_dirLineEdit, 1, 1, 1, 3 );

    tabLayout->addWidget( groupBox5, 0, 0 );
    color_TabWidget->insertTab( tab, QString::fromLatin1("") );

    TabPage = new QWidget( color_TabWidget, "TabPage" );
    TabPageLayout = new QGridLayout( TabPage, 1, 1, 11, 6, "TabPageLayout"); 

    groupBox5_2 = new QGroupBox( TabPage, "groupBox5_2" );
    groupBox5_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)3, 0, 0, groupBox5_2->sizePolicy().hasHeightForWidth() ) );
    groupBox5_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    groupBox5_2->setColumnLayout(0, Qt::Vertical );
    groupBox5_2->layout()->setSpacing( 6 );
    groupBox5_2->layout()->setMargin( 11 );
    groupBox5_2Layout = new QGridLayout( groupBox5_2->layout() );
    groupBox5_2Layout->setAlignment( Qt::AlignTop );

    textLabel2_2 = new QLabel( groupBox5_2, "textLabel2_2" );

    groupBox5_2Layout->addWidget( textLabel2_2, 1, 0 );

    textLabel1_9 = new QLabel( groupBox5_2, "textLabel1_9" );
    textLabel1_9->setPaletteForegroundColor( QColor( 170, 0, 0 ) );

    groupBox5_2Layout->addMultiCellWidget( textLabel1_9, 0, 0, 0, 1 );

    color_ELum_imagesListBox = new QListBox( groupBox5_2, "color_ELum_imagesListBox" );
    color_ELum_imagesListBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)7, 0, 0, color_ELum_imagesListBox->sizePolicy().hasHeightForWidth() ) );
    color_ELum_imagesListBox->setMinimumSize( QSize( 250, 0 ) );
    color_ELum_imagesListBox->setMaximumSize( QSize( 32767, 32767 ) );
    color_ELum_imagesListBox->setFocusPolicy( QListBox::ClickFocus );
    color_ELum_imagesListBox->setSelectionMode( QListBox::Extended );

    groupBox5_2Layout->addWidget( color_ELum_imagesListBox, 2, 0 );
    spacer44_2 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    groupBox5_2Layout->addItem( spacer44_2, 2, 1 );

    layout5 = new QGridLayout( 0, 1, 1, 0, 6, "layout5"); 

    textLabel1_4 = new QLabel( groupBox5_2, "textLabel1_4" );

    layout5->addWidget( textLabel1_4, 3, 0 );
    spacer39 = new QSpacerItem( 20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout5->addItem( spacer39, 5, 0 );

    color_ELum_doitPushButton = new QPushButton( groupBox5_2, "color_ELum_doitPushButton" );
    color_ELum_doitPushButton->setFocusPolicy( QPushButton::NoFocus );

    layout5->addWidget( color_ELum_doitPushButton, 6, 0 );

    color_ELum_maskCheckBox = new QCheckBox( groupBox5_2, "color_ELum_maskCheckBox" );
    color_ELum_maskCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    layout5->addWidget( color_ELum_maskCheckBox, 1, 0 );
    spacer40 = new QSpacerItem( 20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout5->addItem( spacer40, 2, 0 );
    spacer41 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout5->addItem( spacer41, 0, 0 );

    color_ELum_modeComboBox = new QComboBox( FALSE, groupBox5_2, "color_ELum_modeComboBox" );
    color_ELum_modeComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout5->addWidget( color_ELum_modeComboBox, 4, 0 );

    groupBox5_2Layout->addMultiCellLayout( layout5, 0, 2, 2, 2 );

    TabPageLayout->addWidget( groupBox5_2, 0, 0 );
    spacer34 = new QSpacerItem( 20, 70, QSizePolicy::Minimum, QSizePolicy::Fixed );
    TabPageLayout->addItem( spacer34, 2, 0 );
    spacer41_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    TabPageLayout->addItem( spacer41_3, 1, 2 );

    combine_GroupBox = new QGroupBox( TabPage, "combine_GroupBox" );
    combine_GroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, combine_GroupBox->sizePolicy().hasHeightForWidth() ) );
    combine_GroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    combine_GroupBox->setColumnLayout(0, Qt::Vertical );
    combine_GroupBox->layout()->setSpacing( 6 );
    combine_GroupBox->layout()->setMargin( 11 );
    combine_GroupBoxLayout = new QGridLayout( combine_GroupBox->layout() );
    combine_GroupBoxLayout->setAlignment( Qt::AlignTop );

    textLabel1_7 = new QLabel( combine_GroupBox, "textLabel1_7" );
    textLabel1_7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, textLabel1_7->sizePolicy().hasHeightForWidth() ) );

    combine_GroupBoxLayout->addWidget( textLabel1_7, 2, 0 );

    textLabel2_3 = new QLabel( combine_GroupBox, "textLabel2_3" );

    combine_GroupBoxLayout->addWidget( textLabel2_3, 3, 0 );

    textLabel1_9_3 = new QLabel( combine_GroupBox, "textLabel1_9_3" );
    textLabel1_9_3->setPaletteForegroundColor( QColor( 170, 0, 0 ) );

    combine_GroupBoxLayout->addMultiCellWidget( textLabel1_9_3, 0, 0, 0, 6 );

    color_bbnb_weightLineEdit = new QLineEdit( combine_GroupBox, "color_bbnb_weightLineEdit" );
    color_bbnb_weightLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, color_bbnb_weightLineEdit->sizePolicy().hasHeightForWidth() ) );
    color_bbnb_weightLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_bbnb_weightLineEdit->setMaximumSize( QSize( 80, 32767 ) );

    combine_GroupBoxLayout->addWidget( color_bbnb_weightLineEdit, 3, 6 );

    textLabel4 = new QLabel( combine_GroupBox, "textLabel4" );

    combine_GroupBoxLayout->addWidget( textLabel4, 2, 6 );

    color_bbnb_combinePushButton = new QPushButton( combine_GroupBox, "color_bbnb_combinePushButton" );
    color_bbnb_combinePushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, color_bbnb_combinePushButton->sizePolicy().hasHeightForWidth() ) );
    color_bbnb_combinePushButton->setMinimumSize( QSize( 160, 0 ) );
    color_bbnb_combinePushButton->setMaximumSize( QSize( 160, 32767 ) );
    color_bbnb_combinePushButton->setFocusPolicy( QPushButton::NoFocus );

    combine_GroupBoxLayout->addMultiCellWidget( color_bbnb_combinePushButton, 4, 4, 4, 6 );
    spacer41_2 = new QSpacerItem( 320, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    combine_GroupBoxLayout->addMultiCell( spacer41_2, 4, 4, 0, 3 );

    color_bbnb_broadComboBox = new QComboBox( FALSE, combine_GroupBox, "color_bbnb_broadComboBox" );
    color_bbnb_broadComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, color_bbnb_broadComboBox->sizePolicy().hasHeightForWidth() ) );
    color_bbnb_broadComboBox->setMinimumSize( QSize( 200, 0 ) );

    combine_GroupBoxLayout->addWidget( color_bbnb_broadComboBox, 2, 1 );

    color_bbnb_narrowComboBox = new QComboBox( FALSE, combine_GroupBox, "color_bbnb_narrowComboBox" );
    color_bbnb_narrowComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, color_bbnb_narrowComboBox->sizePolicy().hasHeightForWidth() ) );
    color_bbnb_narrowComboBox->setMinimumSize( QSize( 200, 0 ) );

    combine_GroupBoxLayout->addWidget( color_bbnb_narrowComboBox, 3, 1 );

    color_bbnb_narrowwidthLineEdit = new QLineEdit( combine_GroupBox, "color_bbnb_narrowwidthLineEdit" );
    color_bbnb_narrowwidthLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, color_bbnb_narrowwidthLineEdit->sizePolicy().hasHeightForWidth() ) );
    color_bbnb_narrowwidthLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_bbnb_narrowwidthLineEdit->setMaximumSize( QSize( 80, 32767 ) );

    combine_GroupBoxLayout->addMultiCellWidget( color_bbnb_narrowwidthLineEdit, 3, 3, 3, 4 );

    color_bbnb_broadwidthLineEdit = new QLineEdit( combine_GroupBox, "color_bbnb_broadwidthLineEdit" );
    color_bbnb_broadwidthLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, color_bbnb_broadwidthLineEdit->sizePolicy().hasHeightForWidth() ) );
    color_bbnb_broadwidthLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_bbnb_broadwidthLineEdit->setMaximumSize( QSize( 80, 32767 ) );

    combine_GroupBoxLayout->addMultiCellWidget( color_bbnb_broadwidthLineEdit, 2, 2, 3, 4 );

    textLabel3_2 = new QLabel( combine_GroupBox, "textLabel3_2" );

    combine_GroupBoxLayout->addMultiCellWidget( textLabel3_2, 1, 1, 3, 6 );
    spacer32 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    combine_GroupBoxLayout->addItem( spacer32, 2, 2 );
    spacer32_3 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    combine_GroupBoxLayout->addItem( spacer32_3, 3, 5 );
    spacer32_2 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    combine_GroupBoxLayout->addItem( spacer32_2, 3, 2 );

    TabPageLayout->addWidget( combine_GroupBox, 1, 0 );
    color_TabWidget->insertTab( TabPage, QString::fromLatin1("") );

    tab_2 = new QWidget( color_TabWidget, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 
    spacer37_2 = new QSpacerItem( 252, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer37_2, 0, 1 );
    spacer57 = new QSpacerItem( 20, 121, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout_2->addItem( spacer57, 1, 0 );

    groupBox3 = new QGroupBox( tab_2, "groupBox3" );
    groupBox3->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QGridLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );
    spacer51 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox3Layout->addItem( spacer51, 1, 2 );
    spacer52 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    groupBox3Layout->addItem( spacer52, 3, 0 );
    spacer56 = new QSpacerItem( 16, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    groupBox3Layout->addItem( spacer56, 4, 1 );

    layout34 = new QGridLayout( 0, 1, 1, 0, 6, "layout34"); 

    color_BV_r_ComboBox = new QComboBox( FALSE, groupBox3, "color_BV_r_ComboBox" );
    color_BV_r_ComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, color_BV_r_ComboBox->sizePolicy().hasHeightForWidth() ) );
    color_BV_r_ComboBox->setMinimumSize( QSize( 200, 0 ) );
    color_BV_r_ComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout34->addWidget( color_BV_r_ComboBox, 0, 1 );

    color_BV_b_ComboBox = new QComboBox( FALSE, groupBox3, "color_BV_b_ComboBox" );
    color_BV_b_ComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, color_BV_b_ComboBox->sizePolicy().hasHeightForWidth() ) );
    color_BV_b_ComboBox->setMinimumSize( QSize( 200, 0 ) );
    color_BV_b_ComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout34->addWidget( color_BV_b_ComboBox, 2, 1 );

    textLabel1_2_3 = new QLabel( groupBox3, "textLabel1_2_3" );
    textLabel1_2_3->setPaletteForegroundColor( QColor( 0, 0, 255 ) );
    textLabel1_2_3->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );

    layout34->addWidget( textLabel1_2_3, 2, 0 );

    textLabel1_2_2 = new QLabel( groupBox3, "textLabel1_2_2" );
    textLabel1_2_2->setPaletteForegroundColor( QColor( 0, 140, 0 ) );
    textLabel1_2_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );

    layout34->addWidget( textLabel1_2_2, 1, 0 );

    textLabel1_2 = new QLabel( groupBox3, "textLabel1_2" );
    textLabel1_2->setPaletteForegroundColor( QColor( 255, 0, 0 ) );
    textLabel1_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );

    layout34->addWidget( textLabel1_2, 0, 0 );

    color_BV_v_ComboBox = new QComboBox( FALSE, groupBox3, "color_BV_v_ComboBox" );
    color_BV_v_ComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, color_BV_v_ComboBox->sizePolicy().hasHeightForWidth() ) );
    color_BV_v_ComboBox->setMinimumSize( QSize( 200, 0 ) );
    color_BV_v_ComboBox->setFocusPolicy( QComboBox::NoFocus );

    layout34->addWidget( color_BV_v_ComboBox, 1, 1 );

    groupBox3Layout->addMultiCellLayout( layout34, 0, 0, 0, 2 );

    color_resultButtonGroup = new QButtonGroup( groupBox3, "color_resultButtonGroup" );
    color_resultButtonGroup->setExclusive( TRUE );
    color_resultButtonGroup->setColumnLayout(0, Qt::Vertical );
    color_resultButtonGroup->layout()->setSpacing( 6 );
    color_resultButtonGroup->layout()->setMargin( 11 );
    color_resultButtonGroupLayout = new QGridLayout( color_resultButtonGroup->layout() );
    color_resultButtonGroupLayout->setAlignment( Qt::AlignTop );

    color_SDSSPushButton = new QPushButton( color_resultButtonGroup, "color_SDSSPushButton" );
    color_SDSSPushButton->setMinimumSize( QSize( 100, 0 ) );
    color_SDSSPushButton->setMaximumSize( QSize( 100, 32767 ) );
    color_SDSSPushButton->setToggleButton( TRUE );

    color_resultButtonGroupLayout->addWidget( color_SDSSPushButton, 0, 0 );

    color_UCAC4PushButton = new QPushButton( color_resultButtonGroup, "color_UCAC4PushButton" );
    color_UCAC4PushButton->setMinimumSize( QSize( 100, 0 ) );
    color_UCAC4PushButton->setMaximumSize( QSize( 100, 32767 ) );
    color_UCAC4PushButton->setToggleButton( TRUE );

    color_resultButtonGroupLayout->addWidget( color_UCAC4PushButton, 1, 0 );

    color_nstarsSDSSTextLabel = new QLabel( color_resultButtonGroup, "color_nstarsSDSSTextLabel" );

    color_resultButtonGroupLayout->addWidget( color_nstarsSDSSTextLabel, 0, 1 );

    color_nstarsNOMADTextLabel = new QLabel( color_resultButtonGroup, "color_nstarsNOMADTextLabel" );

    color_resultButtonGroupLayout->addWidget( color_nstarsNOMADTextLabel, 2, 1 );

    color_nstarsAVGWHITETextLabel = new QLabel( color_resultButtonGroup, "color_nstarsAVGWHITETextLabel" );

    color_resultButtonGroupLayout->addWidget( color_nstarsAVGWHITETextLabel, 3, 1 );

    color_NOMADPushButton = new QPushButton( color_resultButtonGroup, "color_NOMADPushButton" );
    color_NOMADPushButton->setMinimumSize( QSize( 100, 0 ) );
    color_NOMADPushButton->setMaximumSize( QSize( 100, 32767 ) );
    color_NOMADPushButton->setToggleButton( TRUE );

    color_resultButtonGroupLayout->addWidget( color_NOMADPushButton, 2, 0 );

    color_AVGWHITEPushButton = new QPushButton( color_resultButtonGroup, "color_AVGWHITEPushButton" );
    color_AVGWHITEPushButton->setMinimumSize( QSize( 100, 0 ) );
    color_AVGWHITEPushButton->setMaximumSize( QSize( 100, 32767 ) );
    color_AVGWHITEPushButton->setToggleButton( TRUE );

    color_resultButtonGroupLayout->addWidget( color_AVGWHITEPushButton, 3, 0 );

    color_nstarsUCAC4TextLabel = new QLabel( color_resultButtonGroup, "color_nstarsUCAC4TextLabel" );

    color_resultButtonGroupLayout->addWidget( color_nstarsUCAC4TextLabel, 1, 1 );

    groupBox3Layout->addWidget( color_resultButtonGroup, 4, 2 );

    groupBox7 = new QGroupBox( groupBox3, "groupBox7" );
    groupBox7->setColumnLayout(0, Qt::Vertical );
    groupBox7->layout()->setSpacing( 6 );
    groupBox7->layout()->setMargin( 11 );
    groupBox7Layout = new QGridLayout( groupBox7->layout() );
    groupBox7Layout->setAlignment( Qt::AlignTop );

    color_errorGLineEdit = new QLineEdit( groupBox7, "color_errorGLineEdit" );
    color_errorGLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_errorGLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_errorGLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    color_errorGLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    color_errorGLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    color_errorGLineEdit->setReadOnly( TRUE );

    groupBox7Layout->addWidget( color_errorGLineEdit, 3, 2 );

    color_factorGLineEdit = new QLineEdit( groupBox7, "color_factorGLineEdit" );
    color_factorGLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_factorGLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_factorGLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    color_factorGLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    color_factorGLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    color_factorGLineEdit->setReadOnly( TRUE );

    groupBox7Layout->addWidget( color_factorGLineEdit, 3, 1 );

    color_factorRLineEdit = new QLineEdit( groupBox7, "color_factorRLineEdit" );
    color_factorRLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_factorRLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_factorRLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox7Layout->addWidget( color_factorRLineEdit, 1, 1 );

    textLabel2 = new QLabel( groupBox7, "textLabel2" );

    groupBox7Layout->addWidget( textLabel2, 1, 0 );

    textLabel3_3 = new QLabel( groupBox7, "textLabel3_3" );

    groupBox7Layout->addWidget( textLabel3_3, 3, 0 );

    color_errorRLineEdit = new QLineEdit( groupBox7, "color_errorRLineEdit" );
    color_errorRLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_errorRLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_errorRLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    color_errorRLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    color_errorRLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    color_errorRLineEdit->setReadOnly( TRUE );

    groupBox7Layout->addWidget( color_errorRLineEdit, 1, 2 );

    color_errorBLineEdit = new QLineEdit( groupBox7, "color_errorBLineEdit" );
    color_errorBLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_errorBLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_errorBLineEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    color_errorBLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    color_errorBLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    color_errorBLineEdit->setReadOnly( TRUE );

    groupBox7Layout->addWidget( color_errorBLineEdit, 2, 2 );

    textLabel4_2 = new QLabel( groupBox7, "textLabel4_2" );

    groupBox7Layout->addWidget( textLabel4_2, 2, 0 );

    color_factorBLineEdit = new QLineEdit( groupBox7, "color_factorBLineEdit" );
    color_factorBLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_factorBLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_factorBLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox7Layout->addWidget( color_factorBLineEdit, 2, 1 );

    textLabel3 = new QLabel( groupBox7, "textLabel3" );

    groupBox7Layout->addWidget( textLabel3, 0, 1 );

    textLabel1_3 = new QLabel( groupBox7, "textLabel1_3" );

    groupBox7Layout->addWidget( textLabel1_3, 0, 2 );

    color_stiffpreviewPushButton = new QPushButton( groupBox7, "color_stiffpreviewPushButton" );
    color_stiffpreviewPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox7Layout->addMultiCellWidget( color_stiffpreviewPushButton, 4, 4, 0, 1 );

    groupBox3Layout->addWidget( groupBox7, 4, 0 );

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    color_serverTextLabel = new QLabel( groupBox3, "color_serverTextLabel" );
    layout6->addWidget( color_serverTextLabel );

    color_serverComboBox = new QComboBox( FALSE, groupBox3, "color_serverComboBox" );
    color_serverComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, color_serverComboBox->sizePolicy().hasHeightForWidth() ) );
    color_serverComboBox->setFocusPolicy( QComboBox::NoFocus );
    color_serverComboBox->setSizeLimit( 11 );
    layout6->addWidget( color_serverComboBox );

    color_getBVcalPushButton = new QPushButton( groupBox3, "color_getBVcalPushButton" );
    color_getBVcalPushButton->setMinimumSize( QSize( 120, 0 ) );
    color_getBVcalPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout6->addWidget( color_getBVcalPushButton );

    color_BV_resetPushButton = new QPushButton( groupBox3, "color_BV_resetPushButton" );
    color_BV_resetPushButton->setMinimumSize( QSize( 80, 0 ) );
    color_BV_resetPushButton->setMaximumSize( QSize( 80, 32767 ) );
    color_BV_resetPushButton->setFocusPolicy( QPushButton::NoFocus );
    layout6->addWidget( color_BV_resetPushButton );

    groupBox3Layout->addMultiCellLayout( layout6, 2, 2, 0, 2 );

    tabLayout_2->addWidget( groupBox3, 0, 0 );
    color_TabWidget->insertTab( tab_2, QString::fromLatin1("") );

    TabPage_2 = new QWidget( color_TabWidget, "TabPage_2" );
    TabPageLayout_2 = new QGridLayout( TabPage_2, 1, 1, 11, 6, "TabPageLayout_2"); 
    spacer39_2 = new QSpacerItem( 40, 20, QSizePolicy::Maximum, QSizePolicy::Minimum );
    TabPageLayout_2->addItem( spacer39_2, 0, 1 );
    spacer61 = new QSpacerItem( 20, 8, QSizePolicy::Minimum, QSizePolicy::Fixed );
    TabPageLayout_2->addItem( spacer61, 3, 0 );

    groupBox15 = new QGroupBox( TabPage_2, "groupBox15" );
    groupBox15->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    groupBox15->setColumnLayout(0, Qt::Vertical );
    groupBox15->layout()->setSpacing( 6 );
    groupBox15->layout()->setMargin( 11 );
    groupBox15Layout = new QGridLayout( groupBox15->layout() );
    groupBox15Layout->setAlignment( Qt::AlignTop );

    textLabel5 = new QLabel( groupBox15, "textLabel5" );

    groupBox15Layout->addMultiCellWidget( textLabel5, 0, 0, 0, 3 );

    textLabel4_3 = new QLabel( groupBox15, "textLabel4_3" );

    groupBox15Layout->addWidget( textLabel4_3, 2, 2 );

    textLabel3_4 = new QLabel( groupBox15, "textLabel3_4" );

    groupBox15Layout->addWidget( textLabel3_4, 1, 2 );
    spacer83 = new QSpacerItem( 110, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox15Layout->addItem( spacer83, 2, 4 );

    textLabel2_4 = new QLabel( groupBox15, "textLabel2_4" );

    groupBox15Layout->addWidget( textLabel2_4, 1, 0 );

    textLabel1_10 = new QLabel( groupBox15, "textLabel1_10" );

    groupBox15Layout->addWidget( textLabel1_10, 2, 0 );

    color_stiffRGBLUMComboBox = new QComboBox( FALSE, groupBox15, "color_stiffRGBLUMComboBox" );
    color_stiffRGBLUMComboBox->setMinimumSize( QSize( 220, 0 ) );
    color_stiffRGBLUMComboBox->setMaximumSize( QSize( 220, 32767 ) );
    color_stiffRGBLUMComboBox->setFocusPolicy( QComboBox::NoFocus );

    groupBox15Layout->addWidget( color_stiffRGBLUMComboBox, 0, 5 );

    color_stiffPushButton = new QPushButton( groupBox15, "color_stiffPushButton" );
    color_stiffPushButton->setMinimumSize( QSize( 220, 0 ) );
    color_stiffPushButton->setMaximumSize( QSize( 220, 32767 ) );
    color_stiffPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox15Layout->addWidget( color_stiffPushButton, 2, 5 );

    color_stiffdefaultsPushButton = new QPushButton( groupBox15, "color_stiffdefaultsPushButton" );
    color_stiffdefaultsPushButton->setMinimumSize( QSize( 220, 0 ) );
    color_stiffdefaultsPushButton->setMaximumSize( QSize( 220, 32767 ) );
    color_stiffdefaultsPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox15Layout->addWidget( color_stiffdefaultsPushButton, 1, 5 );

    color_stiffsatvalLineEdit = new QLineEdit( groupBox15, "color_stiffsatvalLineEdit" );
    color_stiffsatvalLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_stiffsatvalLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_stiffsatvalLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox15Layout->addWidget( color_stiffsatvalLineEdit, 1, 3 );

    color_stiffgammaLineEdit = new QLineEdit( groupBox15, "color_stiffgammaLineEdit" );
    color_stiffgammaLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_stiffgammaLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_stiffgammaLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox15Layout->addWidget( color_stiffgammaLineEdit, 2, 3 );

    color_stiffminvalLineEdit = new QLineEdit( groupBox15, "color_stiffminvalLineEdit" );
    color_stiffminvalLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_stiffminvalLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_stiffminvalLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox15Layout->addWidget( color_stiffminvalLineEdit, 1, 1 );

    color_stiffmaxvalLineEdit = new QLineEdit( groupBox15, "color_stiffmaxvalLineEdit" );
    color_stiffmaxvalLineEdit->setMinimumSize( QSize( 80, 0 ) );
    color_stiffmaxvalLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    color_stiffmaxvalLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    groupBox15Layout->addWidget( color_stiffmaxvalLineEdit, 2, 1 );

    TabPageLayout_2->addWidget( groupBox15, 2, 0 );
    spacer61_2 = new QSpacerItem( 20, 8, QSizePolicy::Minimum, QSizePolicy::Fixed );
    TabPageLayout_2->addItem( spacer61_2, 1, 0 );

    groupBox6 = new QGroupBox( TabPage_2, "groupBox6" );
    groupBox6->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_green.png" ) );
    groupBox6->setColumnLayout(0, Qt::Vertical );
    groupBox6->layout()->setSpacing( 6 );
    groupBox6->layout()->setMargin( 11 );
    groupBox6Layout = new QGridLayout( groupBox6->layout() );
    groupBox6Layout->setAlignment( Qt::AlignTop );

    color_fits2tiffPushButton = new QPushButton( groupBox6, "color_fits2tiffPushButton" );
    color_fits2tiffPushButton->setMinimumSize( QSize( 220, 0 ) );
    color_fits2tiffPushButton->setMaximumSize( QSize( 220, 32767 ) );
    color_fits2tiffPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox6Layout->addWidget( color_fits2tiffPushButton, 4, 2 );
    spacer27_3 = new QSpacerItem( 280, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox6Layout->addMultiCell( spacer27_3, 4, 4, 0, 1 );

    textLabel1_6 = new QLabel( groupBox6, "textLabel1_6" );

    groupBox6Layout->addMultiCellWidget( textLabel1_6, 3, 3, 0, 2 );

    color_statisticsTable = new QTable( groupBox6, "color_statisticsTable" );
    color_statisticsTable->setNumCols( color_statisticsTable->numCols() + 1 );
    color_statisticsTable->horizontalHeader()->setLabel( color_statisticsTable->numCols() - 1, tr( "Name" ) );
    color_statisticsTable->setNumCols( color_statisticsTable->numCols() + 1 );
    color_statisticsTable->horizontalHeader()->setLabel( color_statisticsTable->numCols() - 1, tr( "Sky" ) );
    color_statisticsTable->setNumCols( color_statisticsTable->numCols() + 1 );
    color_statisticsTable->horizontalHeader()->setLabel( color_statisticsTable->numCols() - 1, tr( "TIFFmin" ) );
    color_statisticsTable->setNumCols( color_statisticsTable->numCols() + 1 );
    color_statisticsTable->horizontalHeader()->setLabel( color_statisticsTable->numCols() - 1, tr( "TIFFmax" ) );
    color_statisticsTable->setNumCols( color_statisticsTable->numCols() + 1 );
    color_statisticsTable->horizontalHeader()->setLabel( color_statisticsTable->numCols() - 1, tr( "rms" ) );
    color_statisticsTable->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)7, 0, 0, color_statisticsTable->sizePolicy().hasHeightForWidth() ) );
    color_statisticsTable->setMinimumSize( QSize( 300, 200 ) );
    color_statisticsTable->setFocusPolicy( QTable::ClickFocus );
    color_statisticsTable->setNumRows( 3 );
    color_statisticsTable->setNumCols( 5 );
    color_statisticsTable->setReadOnly( FALSE );
    color_statisticsTable->setSelectionMode( QTable::MultiRow );

    groupBox6Layout->addMultiCellWidget( color_statisticsTable, 2, 2, 0, 2 );

    color_getstatisticsPushButton = new QPushButton( groupBox6, "color_getstatisticsPushButton" );
    color_getstatisticsPushButton->setMinimumSize( QSize( 140, 0 ) );
    color_getstatisticsPushButton->setMaximumSize( QSize( 200, 32767 ) );
    color_getstatisticsPushButton->setFocusPolicy( QPushButton::NoFocus );

    groupBox6Layout->addWidget( color_getstatisticsPushButton, 1, 0 );
    spacer27_2 = new QSpacerItem( 300, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox6Layout->addMultiCell( spacer27_2, 1, 1, 1, 2 );

    textLabel5_2 = new QLabel( groupBox6, "textLabel5_2" );

    groupBox6Layout->addMultiCellWidget( textLabel5_2, 0, 0, 0, 1 );

    TabPageLayout_2->addWidget( groupBox6, 0, 0 );
    color_TabWidget->insertTab( TabPage_2, QString::fromLatin1("") );

    color_formLayout->addWidget( color_TabWidget, 0, 0 );
    languageChange();
    resize( QSize(766, 586).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( color_stiffPushButton, SIGNAL( clicked() ), this, SLOT( stiff() ) );
    connect( color_BV_resetPushButton, SIGNAL( clicked() ), this, SLOT( reset_BVcorr() ) );
    connect( color_closePushButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( color_fits2tiffPushButton, SIGNAL( clicked() ), this, SLOT( fits2tiff() ) );
    connect( color_bbnb_combinePushButton, SIGNAL( clicked() ), this, SLOT( combine_images() ) );
    connect( color_dirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_main() ) );
    connect( color_dirLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( update_colordirpath() ) );
    connect( color_ELum_doitPushButton, SIGNAL( clicked() ), this, SLOT( enhance_luminance() ) );
    connect( color_factorBLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_factorGLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_factorRLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_getBVcalPushButton, SIGNAL( clicked() ), this, SLOT( get_color_correction() ) );
    connect( color_getcoaddedimagesPushButton, SIGNAL( clicked() ), this, SLOT( link_coaddimages() ) );
    connect( color_getstatisticsPushButton, SIGNAL( clicked() ), this, SLOT( get_statistics() ) );
    connect( color_removeimagePushButton, SIGNAL( clicked() ), this, SLOT( remove_imagefromlist() ) );
    connect( color_rereadimagePushButton, SIGNAL( clicked() ), this, SLOT( get_colorchannels() ) );
    connect( color_stiffdefaultsPushButton, SIGNAL( clicked() ), this, SLOT( stiff_defaults() ) );
    connect( color_stiffgammaLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_stiffmaxvalLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_stiffminvalLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_stiffsatvalLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_xmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_xminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_ymaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_yminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_stiffpreviewPushButton, SIGNAL( clicked() ), this, SLOT( stiff() ) );
    connect( color_selectdirPushButton, SIGNAL( clicked() ), this, SLOT( select_dir() ) );
    connect( color_bbnb_broadwidthLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_bbnb_narrowwidthLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_bbnb_weightLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validator_sec() ) );
    connect( color_bbnb_combinePushButton, SIGNAL( clicked() ), this, SLOT( combine_images() ) );
    connect( color_SDSSPushButton, SIGNAL( clicked() ), this, SLOT( set_toggle_button() ) );
    connect( color_UCAC4PushButton, SIGNAL( clicked() ), this, SLOT( set_toggle_button() ) );
    connect( color_NOMADPushButton, SIGNAL( clicked() ), this, SLOT( set_toggle_button() ) );
    connect( color_AVGWHITEPushButton, SIGNAL( clicked() ), this, SLOT( set_toggle_button() ) );

    // tab order
    setTabOrder( color_dirLineEdit, color_coaddListBox );
    setTabOrder( color_coaddListBox, color_imagesListBox );
    setTabOrder( color_imagesListBox, color_xminLineEdit );
    setTabOrder( color_xminLineEdit, color_xmaxLineEdit );
    setTabOrder( color_xmaxLineEdit, color_yminLineEdit );
    setTabOrder( color_yminLineEdit, color_ymaxLineEdit );
    setTabOrder( color_ymaxLineEdit, color_ELum_imagesListBox );
    setTabOrder( color_ELum_imagesListBox, color_factorRLineEdit );
    setTabOrder( color_factorRLineEdit, color_factorBLineEdit );
    setTabOrder( color_factorBLineEdit, color_statisticsTable );
    setTabOrder( color_statisticsTable, color_stiffminvalLineEdit );
    setTabOrder( color_stiffminvalLineEdit, color_stiffsatvalLineEdit );
    setTabOrder( color_stiffsatvalLineEdit, color_stiffmaxvalLineEdit );
    setTabOrder( color_stiffmaxvalLineEdit, color_stiffgammaLineEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
color_form::~color_form()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void color_form::languageChange()
{
    setCaption( tr( "Prepare colour picture" ) );
    color_closePushButton->setText( tr( "Close" ) );
    groupBox5->setTitle( QString::null );
    textLabel10->setText( tr( "Statistics region:" ) );
    QToolTip::add( color_xmaxLineEdit, tr( "Right limit of the statistics region" ) );
    QToolTip::add( color_xminLineEdit, tr( "Left limit of the statistics region" ) );
    textLabel9->setText( tr( "xmin | xmax" ) );
    textLabel9_4->setText( tr( "ymin | ymax" ) );
    QToolTip::add( color_ymaxLineEdit, tr( "Upper limit of the statistics region" ) );
    QToolTip::add( color_yminLineEdit, tr( "Lower limit of the statistics region" ) );
    QToolTip::add( color_imagesListBox, tr( "List of the color channels" ) );
    color_removeimagePushButton->setText( tr( "Remove image(s) from list" ) );
    QToolTip::add( color_removeimagePushButton, tr( "Deletes the highlighted image(s) from the list" ) );
    color_rereadimagePushButton->setText( tr( "Restore all images" ) );
    QToolTip::add( color_rereadimagePushButton, tr( "Restores previously removed images in the list" ) );
    textLabel1_5->setText( tr( "Select coadditions:" ) );
    textLabel1->setText( tr( "SCIENCE dir:" ) );
    color_getcoaddedimagesPushButton->setText( tr( "Get coadded images" ) );
    QToolTip::add( color_getcoaddedimagesPushButton, tr( "Links all coadded images selected to a subdirectory called 'color_theli'. These images are then cropped to the largest area in common in such a way that the same object ends up on the same pixel in all images.<br>" ) );
    color_selectdirPushButton->setText( tr( "Select dir ..." ) );
    QToolTip::add( color_dirLineEdit, tr( "The path where the individual color channels are" ) );
    color_TabWidget->changeTab( tab, tr( "Coadditions" ) );
    groupBox5_2->setTitle( QString::null );
    textLabel2_2->setText( tr( "Select images:" ) );
    textLabel1_9->setText( tr( "<b>Create enhanced luminance</b>" ) );
    QToolTip::add( color_ELum_imagesListBox, tr( "Select the images you want to combine" ) );
    textLabel1_4->setText( tr( "Combination method:" ) );
    color_ELum_doitPushButton->setText( tr( "Create ELum" ) );
    QToolTip::add( color_ELum_doitPushButton, tr( "Start the combination" ) );
    color_ELum_maskCheckBox->setText( tr( "Mask combined image" ) );
    QToolTip::add( color_ELum_maskCheckBox, tr( "Activate this if you want to set pixels to zero that were not covered by all input images. This requires the weight images to be present, too.<br>" ) );
    color_ELum_modeComboBox->clear();
    color_ELum_modeComboBox->insertItem( tr( "Noise normalised" ) );
    color_ELum_modeComboBox->insertItem( tr( "Mean" ) );
    QToolTip::add( color_ELum_modeComboBox, tr( "Noise-optimised combination (chi-square) or average combination (mean)" ) );
    combine_GroupBox->setTitle( QString::null );
    textLabel1_7->setText( tr( "Broad" ) );
    textLabel2_3->setText( tr( "Narrow" ) );
    textLabel1_9_3->setText( tr( "<b>Combine broad-band and narrow-band filters</b> <font color=black></font>" ) );
    QToolTip::add( color_bbnb_weightLineEdit, tr( "Weight of the narrow-band filter in the combination" ) );
    textLabel4->setText( tr( "NB weight" ) );
    color_bbnb_combinePushButton->setText( tr( "Combine" ) );
    QToolTip::add( color_bbnb_combinePushButton, tr( "Combines the broad- and the narrow-band image without changing stellar colors" ) );
    QToolTip::add( color_bbnb_broadComboBox, tr( "Select a broad-band image for combination" ) );
    QToolTip::add( color_bbnb_narrowComboBox, tr( "Select a narrow-band image for combination" ) );
    QToolTip::add( color_bbnb_narrowwidthLineEdit, tr( "Width of the narrow-band filter in nm" ) );
    QToolTip::add( color_bbnb_broadwidthLineEdit, tr( "Width of the broad-band filter in nm" ) );
    textLabel3_2->setText( tr( "Width [nm]" ) );
    color_TabWidget->changeTab( TabPage, tr( "ELum + NB combination" ) );
    groupBox3->setTitle( QString::null );
    textLabel1_2_3->setText( tr( "Blue image" ) );
    textLabel1_2_2->setText( tr( "Green image" ) );
    textLabel1_2->setText( tr( "Red image" ) );
    color_resultButtonGroup->setTitle( tr( "Choose calibration result" ) );
    color_SDSSPushButton->setText( tr( "SDSS" ) );
    QToolTip::add( color_SDSSPushButton, tr( "Using u-g, g-r, r-i colors" ) );
    color_UCAC4PushButton->setText( tr( "UCAC4" ) );
    QToolTip::add( color_UCAC4PushButton, tr( "Using B-V, g-r, r-i colors" ) );
    color_nstarsSDSSTextLabel->setText( tr( "0 stars" ) );
    color_nstarsNOMADTextLabel->setText( tr( "0 stars" ) );
    color_nstarsAVGWHITETextLabel->setText( tr( "0 stars" ) );
    color_NOMADPushButton->setText( tr( "NOMAD" ) );
    QToolTip::add( color_NOMADPushButton, tr( "Using B-V, V-R colors" ) );
    color_AVGWHITEPushButton->setText( tr( "AVGWHITE" ) );
    QToolTip::add( color_AVGWHITEPushButton, tr( "Correction will result in average of all sources being white" ) );
    color_nstarsUCAC4TextLabel->setText( tr( "0 stars" ) );
    groupBox7->setTitle( tr( "Correction factors" ) );
    QToolTip::add( color_errorGLineEdit, tr( "The error level for the green correction factor. As the green channel is the reference channel, the error level is always zero. <br>" ) );
    QToolTip::add( color_factorGLineEdit, tr( "The correction factor for the green channel. As this is the reference channel, it is always 1.0 and displayed just for completeness.<br>" ) );
    QToolTip::add( color_factorRLineEdit, tr( "The correction factor for the red channel" ) );
    textLabel2->setText( tr( "Red" ) );
    textLabel3_3->setText( tr( "Green" ) );
    QToolTip::add( color_errorRLineEdit, tr( "The error level for the red correction factor." ) );
    QToolTip::add( color_errorBLineEdit, tr( "The error level for the blue correction factor." ) );
    textLabel4_2->setText( tr( "Blue" ) );
    QToolTip::add( color_factorBLineEdit, tr( "The correction factor for the blue channel" ) );
    textLabel3->setText( tr( "Factor" ) );
    textLabel1_3->setText( tr( "Error" ) );
    color_stiffpreviewPushButton->setText( tr( "Preview" ) );
    color_serverTextLabel->setText( tr( "Catalog server:" ) );
    color_serverComboBox->clear();
    color_serverComboBox->insertItem( tr( "France" ) );
    color_serverComboBox->insertItem( tr( "USA" ) );
    color_serverComboBox->insertItem( tr( "Canada" ) );
    color_serverComboBox->insertItem( tr( "Japan" ) );
    color_serverComboBox->insertItem( tr( "India" ) );
    color_serverComboBox->insertItem( tr( "China" ) );
    color_serverComboBox->insertItem( tr( "UK" ) );
    color_serverComboBox->insertItem( tr( "Hawaii" ) );
    color_serverComboBox->insertItem( tr( "Russia" ) );
    QToolTip::add( color_serverComboBox, tr( "The server from where the photometric reference data is downloaded." ) );
    color_getBVcalPushButton->setText( tr( "Calibrate" ) );
    QToolTip::add( color_getBVcalPushButton, tr( "Obtain photometric color calibration, as well as an average white point." ) );
    color_BV_resetPushButton->setText( tr( "Reset" ) );
    QToolTip::add( color_BV_resetPushButton, tr( "Sets the colour correction factors to 1.0" ) );
    color_TabWidget->changeTab( tab_2, tr( "Color calibration" ) );
    groupBox15->setTitle( QString::null );
    textLabel5->setText( tr( "<b>Convert FITS to TIFF (using STIFF)</b>" ) );
    textLabel4_3->setText( tr( "Gamma" ) );
    textLabel3_4->setText( tr( "Saturation" ) );
    textLabel2_4->setText( tr( "Min level" ) );
    textLabel1_10->setText( tr( "Max level" ) );
    QToolTip::add( color_stiffRGBLUMComboBox, tr( "Select if you want to create a RGB TIFF or a luminance TIFF" ) );
    color_stiffPushButton->setText( tr( "Convert FITS to TIFF" ) );
    QToolTip::add( color_stiffPushButton, tr( "Converts the RGB or Luminance channel(s) into a 16bit TIFF and displays it.<br>" ) );
    color_stiffdefaultsPushButton->setText( tr( "STIFF defaults" ) );
    QToolTip::add( color_stiffsatvalLineEdit, tr( "The colour saturation of the RGB" ) );
    QToolTip::add( color_stiffgammaLineEdit, tr( "The luminance gamma correction factor." ) );
    QToolTip::add( color_stiffminvalLineEdit, tr( "Pixel values lower than this will be black." ) );
    QToolTip::add( color_stiffmaxvalLineEdit, tr( "Pixel values higher than this will be white." ) );
    groupBox6->setTitle( QString::null );
    color_fits2tiffPushButton->setText( tr( "Convert FITS to TIFF" ) );
    QToolTip::add( color_fits2tiffPushButton, tr( "Converts the FITS images to TIFF format, including sky subtraction" ) );
    textLabel1_6->setText( tr( "Edit the sky level, TIFFmin and TIFFmax according to your needs" ) );
    color_statisticsTable->horizontalHeader()->setLabel( 0, tr( "Name" ) );
    color_statisticsTable->horizontalHeader()->setLabel( 1, tr( "Sky" ) );
    color_statisticsTable->horizontalHeader()->setLabel( 2, tr( "TIFFmin" ) );
    color_statisticsTable->horizontalHeader()->setLabel( 3, tr( "TIFFmax" ) );
    color_statisticsTable->horizontalHeader()->setLabel( 4, tr( "rms" ) );
    QToolTip::add( color_statisticsTable, tr( "The sky level and conversion thresholds for the images" ) );
    color_getstatisticsPushButton->setText( tr( "Get statistics" ) );
    QToolTip::add( color_getstatisticsPushButton, tr( "Obtain the statistics for the region in all listed images. The sky residual is taken as the mode, and an estimate for the lower and upper FITS-to-TIFF threshold is obtained.<br>" ) );
    textLabel5_2->setText( tr( "<b>Convert FITS to TIFF (classic)</b>" ) );
    color_TabWidget->changeTab( TabPage_2, tr( "FITS >> TIFF" ) );
}

