/****************************************************************************
** Form implementation generated from reading ui file 'cni_form.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "cni_form.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../cni_form.ui.h"
/*
 *  Constructs a cni_form as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
cni_form::cni_form( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "cni_form" );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    setModal( TRUE );
    cni_formLayout = new QGridLayout( this, 1, 1, 11, 6, "cni_formLayout"); 

    cni_quitPushButton = new QPushButton( this, "cni_quitPushButton" );
    cni_quitPushButton->setFocusPolicy( QPushButton::NoFocus );

    cni_formLayout->addWidget( cni_quitPushButton, 1, 1 );
    spacer22 = new QSpacerItem( 716, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    cni_formLayout->addItem( spacer22, 1, 0 );

    cni_mainTabWidget = new QTabWidget( this, "cni_mainTabWidget" );
    cni_mainTabWidget->setFocusPolicy( QTabWidget::NoFocus );

    tab = new QWidget( cni_mainTabWidget, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 
    spacer16_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( spacer16_2, 3, 0 );

    frame4 = new QFrame( tab, "frame4" );
    frame4->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    frame4->setFrameShape( QFrame::GroupBoxPanel );
    frame4->setFrameShadow( QFrame::Sunken );
    frame4Layout = new QGridLayout( frame4, 1, 1, 11, 6, "frame4Layout"); 

    cni_savePushButton = new QPushButton( frame4, "cni_savePushButton" );
    cni_savePushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cni_savePushButton->sizePolicy().hasHeightForWidth() ) );
    cni_savePushButton->setFocusPolicy( QPushButton::NoFocus );

    frame4Layout->addWidget( cni_savePushButton, 0, 1 );

    cni_clearPushButton = new QPushButton( frame4, "cni_clearPushButton" );
    cni_clearPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cni_clearPushButton->sizePolicy().hasHeightForWidth() ) );
    cni_clearPushButton->setFocusPolicy( QPushButton::NoFocus );

    frame4Layout->addWidget( cni_clearPushButton, 1, 1 );

    cni_loadPushButton = new QPushButton( frame4, "cni_loadPushButton" );
    cni_loadPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cni_loadPushButton->sizePolicy().hasHeightForWidth() ) );
    cni_loadPushButton->setFocusPolicy( QPushButton::NoFocus );

    frame4Layout->addWidget( cni_loadPushButton, 1, 0 );

    cni_guessPushButton = new QPushButton( frame4, "cni_guessPushButton" );
    cni_guessPushButton->setPaletteBackgroundColor( QColor( 145, 228, 228 ) );

    frame4Layout->addWidget( cni_guessPushButton, 0, 0 );

    tabLayout->addWidget( frame4, 2, 2 );

    cni_fillexampleGroupBox = new QGroupBox( tab, "cni_fillexampleGroupBox" );
    cni_fillexampleGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, cni_fillexampleGroupBox->sizePolicy().hasHeightForWidth() ) );
    cni_fillexampleGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    cni_fillexampleGroupBox->setColumnLayout(0, Qt::Vertical );
    cni_fillexampleGroupBox->layout()->setSpacing( 6 );
    cni_fillexampleGroupBox->layout()->setMargin( 11 );
    cni_fillexampleGroupBoxLayout = new QGridLayout( cni_fillexampleGroupBox->layout() );
    cni_fillexampleGroupBoxLayout->setAlignment( Qt::AlignTop );

    cni_st10PushButton = new QPushButton( cni_fillexampleGroupBox, "cni_st10PushButton" );
    cni_st10PushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cni_st10PushButton->sizePolicy().hasHeightForWidth() ) );
    cni_st10PushButton->setFocusPolicy( QPushButton::NoFocus );

    cni_fillexampleGroupBoxLayout->addWidget( cni_st10PushButton, 0, 0 );

    cni_wfcPushButton = new QPushButton( cni_fillexampleGroupBox, "cni_wfcPushButton" );
    cni_wfcPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cni_wfcPushButton->sizePolicy().hasHeightForWidth() ) );
    cni_wfcPushButton->setFocusPolicy( QPushButton::NoFocus );

    cni_fillexampleGroupBoxLayout->addWidget( cni_wfcPushButton, 1, 0 );

    cni_wfiPushButton = new QPushButton( cni_fillexampleGroupBox, "cni_wfiPushButton" );
    cni_wfiPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cni_wfiPushButton->sizePolicy().hasHeightForWidth() ) );
    cni_wfiPushButton->setFocusPolicy( QPushButton::NoFocus );

    cni_fillexampleGroupBoxLayout->addWidget( cni_wfiPushButton, 0, 1 );

    cni_presentinstPushButton = new QPushButton( cni_fillexampleGroupBox, "cni_presentinstPushButton" );
    cni_presentinstPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cni_presentinstPushButton->sizePolicy().hasHeightForWidth() ) );
    cni_presentinstPushButton->setFocusPolicy( QPushButton::NoFocus );

    cni_fillexampleGroupBoxLayout->addWidget( cni_presentinstPushButton, 1, 1 );

    tabLayout->addWidget( cni_fillexampleGroupBox, 2, 0 );
    spacer17 = new QSpacerItem( 193, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout->addItem( spacer17, 2, 3 );
    spacer14 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    tabLayout->addItem( spacer14, 2, 1 );

    cni_ccdgeomGroupBox = new QGroupBox( tab, "cni_ccdgeomGroupBox" );
    cni_ccdgeomGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, cni_ccdgeomGroupBox->sizePolicy().hasHeightForWidth() ) );
    cni_ccdgeomGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    cni_ccdgeomGroupBox->setColumnLayout(0, Qt::Vertical );
    cni_ccdgeomGroupBox->layout()->setSpacing( 6 );
    cni_ccdgeomGroupBox->layout()->setMargin( 11 );
    cni_ccdgeomGroupBoxLayout = new QGridLayout( cni_ccdgeomGroupBox->layout() );
    cni_ccdgeomGroupBoxLayout->setAlignment( Qt::AlignTop );

    cni_textLabel8 = new QLabel( cni_ccdgeomGroupBox, "cni_textLabel8" );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_textLabel8, 0, 2 );
    spacer10 = new QSpacerItem( 455, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    cni_ccdgeomGroupBoxLayout->addMultiCell( spacer10, 0, 0, 0, 1 );

    cni_crpix2numchkLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_crpix2numchkLineEdit" );
    cni_crpix2numchkLineEdit->setMinimumSize( QSize( 50, 0 ) );
    cni_crpix2numchkLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    cni_crpix2numchkLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    cni_crpix2numchkLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    cni_crpix2numchkLineEdit->setFrameShadow( QLineEdit::Sunken );
    cni_crpix2numchkLineEdit->setLineWidth( 2 );
    cni_crpix2numchkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    cni_crpix2numchkLineEdit->setReadOnly( TRUE );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_crpix2numchkLineEdit, 8, 2 );

    cni_ysizenumchkLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_ysizenumchkLineEdit" );
    cni_ysizenumchkLineEdit->setMinimumSize( QSize( 50, 0 ) );
    cni_ysizenumchkLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    cni_ysizenumchkLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    cni_ysizenumchkLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    cni_ysizenumchkLineEdit->setFrameShadow( QLineEdit::Sunken );
    cni_ysizenumchkLineEdit->setLineWidth( 2 );
    cni_ysizenumchkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    cni_ysizenumchkLineEdit->setReadOnly( TRUE );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ysizenumchkLineEdit, 6, 2 );

    cni_cutonynumchkLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_cutonynumchkLineEdit" );
    cni_cutonynumchkLineEdit->setMinimumSize( QSize( 50, 0 ) );
    cni_cutonynumchkLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    cni_cutonynumchkLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    cni_cutonynumchkLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    cni_cutonynumchkLineEdit->setFrameShadow( QLineEdit::Sunken );
    cni_cutonynumchkLineEdit->setLineWidth( 2 );
    cni_cutonynumchkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    cni_cutonynumchkLineEdit->setReadOnly( TRUE );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_cutonynumchkLineEdit, 4, 2 );

    cni_ovmaxnumchkLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_ovmaxnumchkLineEdit" );
    cni_ovmaxnumchkLineEdit->setMinimumSize( QSize( 50, 0 ) );
    cni_ovmaxnumchkLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    cni_ovmaxnumchkLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    cni_ovmaxnumchkLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    cni_ovmaxnumchkLineEdit->setFrameShadow( QLineEdit::Sunken );
    cni_ovmaxnumchkLineEdit->setLineWidth( 2 );
    cni_ovmaxnumchkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    cni_ovmaxnumchkLineEdit->setReadOnly( TRUE );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ovmaxnumchkLineEdit, 2, 2 );

    cni_crpix1numchkLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_crpix1numchkLineEdit" );
    cni_crpix1numchkLineEdit->setMinimumSize( QSize( 50, 0 ) );
    cni_crpix1numchkLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    cni_crpix1numchkLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    cni_crpix1numchkLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    cni_crpix1numchkLineEdit->setFrameShadow( QLineEdit::Sunken );
    cni_crpix1numchkLineEdit->setLineWidth( 2 );
    cni_crpix1numchkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    cni_crpix1numchkLineEdit->setReadOnly( TRUE );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_crpix1numchkLineEdit, 7, 2 );

    cni_xsizenumchkLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_xsizenumchkLineEdit" );
    cni_xsizenumchkLineEdit->setMinimumSize( QSize( 50, 0 ) );
    cni_xsizenumchkLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    cni_xsizenumchkLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    cni_xsizenumchkLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    cni_xsizenumchkLineEdit->setFrameShadow( QLineEdit::Sunken );
    cni_xsizenumchkLineEdit->setLineWidth( 2 );
    cni_xsizenumchkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    cni_xsizenumchkLineEdit->setReadOnly( TRUE );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_xsizenumchkLineEdit, 5, 2 );

    cni_cutonxnumchkLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_cutonxnumchkLineEdit" );
    cni_cutonxnumchkLineEdit->setMinimumSize( QSize( 50, 0 ) );
    cni_cutonxnumchkLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    cni_cutonxnumchkLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    cni_cutonxnumchkLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    cni_cutonxnumchkLineEdit->setFrameShadow( QLineEdit::Sunken );
    cni_cutonxnumchkLineEdit->setLineWidth( 2 );
    cni_cutonxnumchkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    cni_cutonxnumchkLineEdit->setReadOnly( TRUE );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_cutonxnumchkLineEdit, 3, 2 );

    cni_ovmaxTextLabel = new QLabel( cni_ccdgeomGroupBox, "cni_ovmaxTextLabel" );
    cni_ovmaxTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, cni_ovmaxTextLabel->sizePolicy().hasHeightForWidth() ) );
    QFont cni_ovmaxTextLabel_font(  cni_ovmaxTextLabel->font() );
    cni_ovmaxTextLabel->setFont( cni_ovmaxTextLabel_font ); 
    cni_ovmaxTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ovmaxTextLabel, 2, 0 );

    cni_cutonxTextLabel = new QLabel( cni_ccdgeomGroupBox, "cni_cutonxTextLabel" );
    cni_cutonxTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, cni_cutonxTextLabel->sizePolicy().hasHeightForWidth() ) );
    QFont cni_cutonxTextLabel_font(  cni_cutonxTextLabel->font() );
    cni_cutonxTextLabel->setFont( cni_cutonxTextLabel_font ); 
    cni_cutonxTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_cutonxTextLabel, 3, 0 );

    cni_cutonyTextLabel = new QLabel( cni_ccdgeomGroupBox, "cni_cutonyTextLabel" );
    cni_cutonyTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, cni_cutonyTextLabel->sizePolicy().hasHeightForWidth() ) );
    QFont cni_cutonyTextLabel_font(  cni_cutonyTextLabel->font() );
    cni_cutonyTextLabel->setFont( cni_cutonyTextLabel_font ); 
    cni_cutonyTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_cutonyTextLabel, 4, 0 );

    cni_xsizeTextLabel = new QLabel( cni_ccdgeomGroupBox, "cni_xsizeTextLabel" );
    cni_xsizeTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, cni_xsizeTextLabel->sizePolicy().hasHeightForWidth() ) );
    QFont cni_xsizeTextLabel_font(  cni_xsizeTextLabel->font() );
    cni_xsizeTextLabel->setFont( cni_xsizeTextLabel_font ); 
    cni_xsizeTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_xsizeTextLabel, 5, 0 );

    cni_ysizeTextLabel = new QLabel( cni_ccdgeomGroupBox, "cni_ysizeTextLabel" );
    cni_ysizeTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, cni_ysizeTextLabel->sizePolicy().hasHeightForWidth() ) );
    QFont cni_ysizeTextLabel_font(  cni_ysizeTextLabel->font() );
    cni_ysizeTextLabel->setFont( cni_ysizeTextLabel_font ); 
    cni_ysizeTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ysizeTextLabel, 6, 0 );

    cni_crpix1TextLabel = new QLabel( cni_ccdgeomGroupBox, "cni_crpix1TextLabel" );
    cni_crpix1TextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, cni_crpix1TextLabel->sizePolicy().hasHeightForWidth() ) );
    QFont cni_crpix1TextLabel_font(  cni_crpix1TextLabel->font() );
    cni_crpix1TextLabel->setFont( cni_crpix1TextLabel_font ); 
    cni_crpix1TextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_crpix1TextLabel, 7, 0 );

    cni_crpix2TextLabel = new QLabel( cni_ccdgeomGroupBox, "cni_crpix2TextLabel" );
    cni_crpix2TextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, cni_crpix2TextLabel->sizePolicy().hasHeightForWidth() ) );
    QFont cni_crpix2TextLabel_font(  cni_crpix2TextLabel->font() );
    cni_crpix2TextLabel->setFont( cni_crpix2TextLabel_font ); 
    cni_crpix2TextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_crpix2TextLabel, 8, 0 );

    cni_ysizeLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_ysizeLineEdit" );
    cni_ysizeLineEdit->setMinimumSize( QSize( 320, 0 ) );
    cni_ysizeLineEdit->setMaxLength( 999 );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ysizeLineEdit, 6, 1 );

    cni_crpix2LineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_crpix2LineEdit" );
    cni_crpix2LineEdit->setMinimumSize( QSize( 320, 0 ) );
    cni_crpix2LineEdit->setMaxLength( 999 );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_crpix2LineEdit, 8, 1 );

    cni_cutonyLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_cutonyLineEdit" );
    cni_cutonyLineEdit->setMinimumSize( QSize( 320, 0 ) );
    cni_cutonyLineEdit->setMaxLength( 999 );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_cutonyLineEdit, 4, 1 );

    cni_ovmaxLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_ovmaxLineEdit" );
    cni_ovmaxLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, cni_ovmaxLineEdit->sizePolicy().hasHeightForWidth() ) );
    cni_ovmaxLineEdit->setMinimumSize( QSize( 320, 0 ) );
    cni_ovmaxLineEdit->setMaxLength( 999 );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ovmaxLineEdit, 2, 1 );

    cni_cutonxLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_cutonxLineEdit" );
    cni_cutonxLineEdit->setMinimumSize( QSize( 320, 0 ) );
    cni_cutonxLineEdit->setMaxLength( 999 );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_cutonxLineEdit, 3, 1 );

    cni_xsizeLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_xsizeLineEdit" );
    cni_xsizeLineEdit->setMinimumSize( QSize( 320, 0 ) );
    cni_xsizeLineEdit->setMaxLength( 999 );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_xsizeLineEdit, 5, 1 );

    cni_crpix1LineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_crpix1LineEdit" );
    cni_crpix1LineEdit->setMinimumSize( QSize( 320, 0 ) );
    cni_crpix1LineEdit->setMaxLength( 999 );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_crpix1LineEdit, 7, 1 );

    cni_ovminTextLabel = new QLabel( cni_ccdgeomGroupBox, "cni_ovminTextLabel" );
    cni_ovminTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, cni_ovminTextLabel->sizePolicy().hasHeightForWidth() ) );
    QFont cni_ovminTextLabel_font(  cni_ovminTextLabel->font() );
    cni_ovminTextLabel->setFont( cni_ovminTextLabel_font ); 
    cni_ovminTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ovminTextLabel, 1, 0 );

    cni_ovminnumchkLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_ovminnumchkLineEdit" );
    cni_ovminnumchkLineEdit->setMinimumSize( QSize( 50, 0 ) );
    cni_ovminnumchkLineEdit->setMaximumSize( QSize( 50, 32767 ) );
    cni_ovminnumchkLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    cni_ovminnumchkLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    cni_ovminnumchkLineEdit->setFrameShadow( QLineEdit::Sunken );
    cni_ovminnumchkLineEdit->setLineWidth( 2 );
    cni_ovminnumchkLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    cni_ovminnumchkLineEdit->setReadOnly( TRUE );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ovminnumchkLineEdit, 1, 2 );

    cni_ovminLineEdit = new QLineEdit( cni_ccdgeomGroupBox, "cni_ovminLineEdit" );
    cni_ovminLineEdit->setMinimumSize( QSize( 320, 0 ) );
    cni_ovminLineEdit->setMaxLength( 999 );

    cni_ccdgeomGroupBoxLayout->addWidget( cni_ovminLineEdit, 1, 1 );

    tabLayout->addMultiCellWidget( cni_ccdgeomGroupBox, 1, 1, 0, 2 );
    spacer16 = new QSpacerItem( 193, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout->addItem( spacer16, 1, 3 );

    cni_genpropGroupBox = new QGroupBox( tab, "cni_genpropGroupBox" );
    cni_genpropGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, cni_genpropGroupBox->sizePolicy().hasHeightForWidth() ) );
    cni_genpropGroupBox->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    cni_genpropGroupBox->setColumnLayout(0, Qt::Vertical );
    cni_genpropGroupBox->layout()->setSpacing( 6 );
    cni_genpropGroupBox->layout()->setMargin( 11 );
    cni_genpropGroupBoxLayout = new QGridLayout( cni_genpropGroupBox->layout() );
    cni_genpropGroupBoxLayout->setAlignment( Qt::AlignTop );
    spacer13_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addItem( spacer13_2, 0, 1 );

    cni_instnameTextLabel = new QLabel( cni_genpropGroupBox, "cni_instnameTextLabel" );
    cni_instnameTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, cni_instnameTextLabel->sizePolicy().hasHeightForWidth() ) );
    cni_instnameTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_genpropGroupBoxLayout->addWidget( cni_instnameTextLabel, 0, 0 );

    cni_nccdTextLabel = new QLabel( cni_genpropGroupBox, "cni_nccdTextLabel" );
    cni_nccdTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, cni_nccdTextLabel->sizePolicy().hasHeightForWidth() ) );
    cni_nccdTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_genpropGroupBoxLayout->addWidget( cni_nccdTextLabel, 1, 0 );

    cni_geoglTextLabel = new QLabel( cni_genpropGroupBox, "cni_geoglTextLabel" );
    cni_geoglTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, cni_geoglTextLabel->sizePolicy().hasHeightForWidth() ) );
    cni_geoglTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_genpropGroupBoxLayout->addWidget( cni_geoglTextLabel, 2, 0 );

    cni_gainTextLabel = new QLabel( cni_genpropGroupBox, "cni_gainTextLabel" );
    cni_gainTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, cni_gainTextLabel->sizePolicy().hasHeightForWidth() ) );
    cni_gainTextLabel->setAlignment( int( QLabel::AlignVCenter ) );

    cni_genpropGroupBoxLayout->addWidget( cni_gainTextLabel, 3, 0 );
    spacer1_3 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addItem( spacer1_3, 1, 1 );
    spacer1 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addItem( spacer1, 2, 1 );

    textLabel1_2 = new QLabel( cni_genpropGroupBox, "textLabel1_2" );

    cni_genpropGroupBoxLayout->addWidget( textLabel1_2, 4, 0 );

    cni_pixsclLineEdit = new QLineEdit( cni_genpropGroupBox, "cni_pixsclLineEdit" );
    cni_pixsclLineEdit->setMinimumSize( QSize( 120, 0 ) );
    cni_pixsclLineEdit->setMaximumSize( QSize( 120, 32767 ) );
    cni_pixsclLineEdit->setMaxLength( 99 );
    cni_pixsclLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cni_genpropGroupBoxLayout->addWidget( cni_pixsclLineEdit, 3, 2 );

    cni_geogLineEdit = new QLineEdit( cni_genpropGroupBox, "cni_geogLineEdit" );
    cni_geogLineEdit->setMinimumSize( QSize( 120, 0 ) );
    cni_geogLineEdit->setMaximumSize( QSize( 120, 32767 ) );
    cni_geogLineEdit->setMaxLength( 99 );
    cni_geogLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cni_genpropGroupBoxLayout->addWidget( cni_geogLineEdit, 2, 2 );

    cni_nccdSpinBox = new QSpinBox( cni_genpropGroupBox, "cni_nccdSpinBox" );
    cni_nccdSpinBox->setMinimumSize( QSize( 120, 0 ) );
    cni_nccdSpinBox->setMaximumSize( QSize( 120, 32767 ) );
    cni_nccdSpinBox->setFocusPolicy( QSpinBox::StrongFocus );
    cni_nccdSpinBox->setMaxValue( 999 );
    cni_nccdSpinBox->setMinValue( 1 );

    cni_genpropGroupBoxLayout->addWidget( cni_nccdSpinBox, 1, 2 );
    spacer18 = new QSpacerItem( 40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addMultiCell( spacer18, 1, 1, 3, 4 );
    spacer14_2 = new QSpacerItem( 40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addMultiCell( spacer14_2, 2, 2, 3, 4 );
    spacer14_2_3 = new QSpacerItem( 90, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addItem( spacer14_2_3, 0, 5 );

    cni_instnameLineEdit = new QLineEdit( cni_genpropGroupBox, "cni_instnameLineEdit" );
    cni_instnameLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, cni_instnameLineEdit->sizePolicy().hasHeightForWidth() ) );
    cni_instnameLineEdit->setMinimumSize( QSize( 300, 0 ) );
    cni_instnameLineEdit->setMaximumSize( QSize( 300, 32767 ) );
    cni_instnameLineEdit->setMaxLength( 99 );
    cni_instnameLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    cni_genpropGroupBoxLayout->addMultiCellWidget( cni_instnameLineEdit, 0, 0, 2, 4 );
    spacer1_4 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addItem( spacer1_4, 3, 1 );
    spacer1_4_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addItem( spacer1_4_2, 4, 1 );

    cni_cameratypeComboBox = new QComboBox( FALSE, cni_genpropGroupBox, "cni_cameratypeComboBox" );
    cni_cameratypeComboBox->setMinimumSize( QSize( 120, 0 ) );
    cni_cameratypeComboBox->setMaximumSize( QSize( 120, 32767 ) );

    cni_genpropGroupBoxLayout->addMultiCellWidget( cni_cameratypeComboBox, 4, 4, 2, 3 );
    spacer14_2_2 = new QSpacerItem( 40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addMultiCell( spacer14_2_2, 3, 3, 3, 4 );
    spacer14_2_2_2 = new QSpacerItem( 160, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    cni_genpropGroupBoxLayout->addItem( spacer14_2_2_2, 4, 4 );

    tabLayout->addMultiCellWidget( cni_genpropGroupBox, 0, 0, 0, 2 );
    cni_mainTabWidget->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( cni_mainTabWidget, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 
    spacer17_2 = new QSpacerItem( 19, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer17_2, 1, 6 );
    spacer17_2_2 = new QSpacerItem( 19, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer17_2_2, 0, 6 );

    ssd_GroupBox1 = new QGroupBox( tab_2, "ssd_GroupBox1" );
    ssd_GroupBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, ssd_GroupBox1->sizePolicy().hasHeightForWidth() ) );
    ssd_GroupBox1->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );

    textLabel5 = new QLabel( ssd_GroupBox1, "textLabel5" );
    textLabel5->setGeometry( QRect( 20, 50, 70, 20 ) );

    textLabel2 = new QLabel( ssd_GroupBox1, "textLabel2" );
    textLabel2->setGeometry( QRect( 20, 20, 70, 20 ) );

    textLabel3 = new QLabel( ssd_GroupBox1, "textLabel3" );
    textLabel3->setGeometry( QRect( 110, 20, 70, 20 ) );

    textLabel6 = new QLabel( ssd_GroupBox1, "textLabel6" );
    textLabel6->setGeometry( QRect( 20, 70, 70, 20 ) );

    textLabel7 = new QLabel( ssd_GroupBox1, "textLabel7" );
    textLabel7->setGeometry( QRect( 20, 90, 70, 20 ) );

    textLabel8 = new QLabel( ssd_GroupBox1, "textLabel8" );
    textLabel8->setGeometry( QRect( 20, 110, 70, 20 ) );

    textLabel9 = new QLabel( ssd_GroupBox1, "textLabel9" );
    textLabel9->setGeometry( QRect( 20, 130, 70, 20 ) );

    textLabel10 = new QLabel( ssd_GroupBox1, "textLabel10" );
    textLabel10->setGeometry( QRect( 20, 150, 70, 20 ) );

    textLabel4 = new QLabel( ssd_GroupBox1, "textLabel4" );
    textLabel4->setGeometry( QRect( 20, 170, 70, 20 ) );

    textLabel11 = new QLabel( ssd_GroupBox1, "textLabel11" );
    textLabel11->setGeometry( QRect( 270, 20, 62, 20 ) );

    textLabel4_2 = new QLabel( ssd_GroupBox1, "textLabel4_2" );
    textLabel4_2->setGeometry( QRect( 20, 190, 70, 20 ) );

    ssd_raComboBox = new QComboBox( FALSE, ssd_GroupBox1, "ssd_raComboBox" );
    ssd_raComboBox->setGeometry( QRect( 270, 50, 100, 22 ) );
    ssd_raComboBox->setMinimumSize( QSize( 100, 0 ) );
    ssd_raComboBox->setMaximumSize( QSize( 100, 32767 ) );
    ssd_raComboBox->setFocusPolicy( QComboBox::NoFocus );

    ssd_decComboBox = new QComboBox( FALSE, ssd_GroupBox1, "ssd_decComboBox" );
    ssd_decComboBox->setGeometry( QRect( 270, 70, 100, 22 ) );
    ssd_decComboBox->setMaximumSize( QSize( 100, 32767 ) );
    ssd_decComboBox->setFocusPolicy( QComboBox::NoFocus );

    ssd_rakeyLineEdit = new QLineEdit( ssd_GroupBox1, "ssd_rakeyLineEdit" );
    ssd_rakeyLineEdit->setGeometry( QRect( 110, 50, 150, 22 ) );
    ssd_rakeyLineEdit->setMaxLength( 99 );

    ssd_deckeyLineEdit = new QLineEdit( ssd_GroupBox1, "ssd_deckeyLineEdit" );
    ssd_deckeyLineEdit->setGeometry( QRect( 110, 70, 150, 22 ) );
    ssd_deckeyLineEdit->setMaxLength( 99 );

    ssd_lstkeyLineEdit = new QLineEdit( ssd_GroupBox1, "ssd_lstkeyLineEdit" );
    ssd_lstkeyLineEdit->setGeometry( QRect( 110, 90, 150, 22 ) );
    ssd_lstkeyLineEdit->setMaxLength( 99 );

    ssd_mjdkeyLineEdit = new QLineEdit( ssd_GroupBox1, "ssd_mjdkeyLineEdit" );
    ssd_mjdkeyLineEdit->setGeometry( QRect( 110, 110, 150, 22 ) );
    ssd_mjdkeyLineEdit->setMaxLength( 99 );

    ssd_exptimekeyLineEdit = new QLineEdit( ssd_GroupBox1, "ssd_exptimekeyLineEdit" );
    ssd_exptimekeyLineEdit->setGeometry( QRect( 110, 130, 150, 22 ) );
    ssd_exptimekeyLineEdit->setMaxLength( 99 );

    ssd_airmasskeyLineEdit = new QLineEdit( ssd_GroupBox1, "ssd_airmasskeyLineEdit" );
    ssd_airmasskeyLineEdit->setGeometry( QRect( 110, 150, 150, 22 ) );
    ssd_airmasskeyLineEdit->setMaxLength( 99 );

    ssd_filterkeyLineEdit = new QLineEdit( ssd_GroupBox1, "ssd_filterkeyLineEdit" );
    ssd_filterkeyLineEdit->setGeometry( QRect( 110, 170, 150, 22 ) );
    ssd_filterkeyLineEdit->setMaxLength( 99 );

    ssd_equinoxLineEdit = new QLineEdit( ssd_GroupBox1, "ssd_equinoxLineEdit" );
    ssd_equinoxLineEdit->setGeometry( QRect( 110, 190, 150, 22 ) );
    ssd_equinoxLineEdit->setMaxLength( 99 );

    ssd_lstComboBox = new QComboBox( FALSE, ssd_GroupBox1, "ssd_lstComboBox" );
    ssd_lstComboBox->setGeometry( QRect( 270, 90, 100, 22 ) );
    ssd_lstComboBox->setMaximumSize( QSize( 100, 32767 ) );
    ssd_lstComboBox->setFocusPolicy( QComboBox::NoFocus );

    tabLayout_2->addMultiCellWidget( ssd_GroupBox1, 0, 0, 0, 2 );

    ssd_instrumentLineEdit = new QLineEdit( tab_2, "ssd_instrumentLineEdit" );
    ssd_instrumentLineEdit->setMinimumSize( QSize( 200, 0 ) );
    ssd_instrumentLineEdit->setMaximumSize( QSize( 200, 32767 ) );
    ssd_instrumentLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    ssd_instrumentLineEdit->setAlignment( int( QLineEdit::AlignRight ) );
    ssd_instrumentLineEdit->setDragEnabled( FALSE );
    ssd_instrumentLineEdit->setReadOnly( TRUE );

    tabLayout_2->addWidget( ssd_instrumentLineEdit, 2, 1 );

    ssd_createscriptPushButton = new QPushButton( tab_2, "ssd_createscriptPushButton" );
    ssd_createscriptPushButton->setFocusPolicy( QPushButton::NoFocus );

    tabLayout_2->addWidget( ssd_createscriptPushButton, 2, 5 );
    spacer86 = new QSpacerItem( 246, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addMultiCell( spacer86, 2, 2, 2, 4 );

    ssd_changeinstrumentPushButton = new QPushButton( tab_2, "ssd_changeinstrumentPushButton" );
    ssd_changeinstrumentPushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ssd_changeinstrumentPushButton->sizePolicy().hasHeightForWidth() ) );
    ssd_changeinstrumentPushButton->setMaximumSize( QSize( 170, 32767 ) );
    ssd_changeinstrumentPushButton->setFocusPolicy( QPushButton::NoFocus );

    tabLayout_2->addWidget( ssd_changeinstrumentPushButton, 2, 0 );

    ssd_GroupBox2_2 = new QGroupBox( tab_2, "ssd_GroupBox2_2" );
    ssd_GroupBox2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, ssd_GroupBox2_2->sizePolicy().hasHeightForWidth() ) );
    ssd_GroupBox2_2->setMinimumSize( QSize( 0, 0 ) );
    ssd_GroupBox2_2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    ssd_GroupBox2_2->setColumnLayout(0, Qt::Vertical );
    ssd_GroupBox2_2->layout()->setSpacing( 6 );
    ssd_GroupBox2_2->layout()->setMargin( 11 );
    ssd_GroupBox2_2Layout = new QGridLayout( ssd_GroupBox2_2->layout() );
    ssd_GroupBox2_2Layout->setAlignment( Qt::AlignTop );

    buttonGroup3 = new QButtonGroup( ssd_GroupBox2_2, "buttonGroup3" );
    buttonGroup3->setFrameShape( QButtonGroup::NoFrame );
    buttonGroup3->setColumnLayout(0, Qt::Vertical );
    buttonGroup3->layout()->setSpacing( 6 );
    buttonGroup3->layout()->setMargin( 11 );
    buttonGroup3Layout = new QGridLayout( buttonGroup3->layout() );
    buttonGroup3Layout->setAlignment( Qt::AlignTop );

    monochrome_FITS_RadioButton = new QRadioButton( buttonGroup3, "monochrome_FITS_RadioButton" );
    monochrome_FITS_RadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup3Layout->addWidget( monochrome_FITS_RadioButton, 0, 0 );

    bayer_FITS_RadioButton = new QRadioButton( buttonGroup3, "bayer_FITS_RadioButton" );
    bayer_FITS_RadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup3Layout->addWidget( bayer_FITS_RadioButton, 3, 0 );

    bayer_RAW_RadioButton = new QRadioButton( buttonGroup3, "bayer_RAW_RadioButton" );
    bayer_RAW_RadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup3Layout->addWidget( bayer_RAW_RadioButton, 2, 0 );

    monochrome_RAW_RadioButton = new QRadioButton( buttonGroup3, "monochrome_RAW_RadioButton" );
    monochrome_RAW_RadioButton->setFocusPolicy( QRadioButton::NoFocus );

    buttonGroup3Layout->addWidget( monochrome_RAW_RadioButton, 1, 0 );

    ssd_GroupBox2_2Layout->addWidget( buttonGroup3, 0, 0 );

    bayerframe = new QFrame( ssd_GroupBox2_2, "bayerframe" );
    bayerframe->setFrameShape( QFrame::NoFrame );
    bayerframe->setFrameShadow( QFrame::Raised );
    bayerframeLayout = new QGridLayout( bayerframe, 1, 1, 11, 6, "bayerframeLayout"); 
    spacer20 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    bayerframeLayout->addItem( spacer20, 2, 0 );

    bayer_ButtonGroup = new QButtonGroup( bayerframe, "bayer_ButtonGroup" );
    bayer_ButtonGroup->setFrameShape( QButtonGroup::NoFrame );
    bayer_ButtonGroup->setExclusive( TRUE );
    bayer_ButtonGroup->setColumnLayout(0, Qt::Vertical );
    bayer_ButtonGroup->layout()->setSpacing( 6 );
    bayer_ButtonGroup->layout()->setMargin( 11 );
    bayer_ButtonGroupLayout = new QGridLayout( bayer_ButtonGroup->layout() );
    bayer_ButtonGroupLayout->setAlignment( Qt::AlignTop );

    bayer_GBRG_ToolButton = new QToolButton( bayer_ButtonGroup, "bayer_GBRG_ToolButton" );
    bayer_GBRG_ToolButton->setMinimumSize( QSize( 40, 40 ) );
    bayer_GBRG_ToolButton->setMaximumSize( QSize( 40, 40 ) );
    bayer_GBRG_ToolButton->setFocusPolicy( QToolButton::NoFocus );
    bayer_GBRG_ToolButton->setToggleButton( TRUE );

    bayer_ButtonGroupLayout->addWidget( bayer_GBRG_ToolButton, 0, 1 );

    bayer_BGGR_ToolButton = new QToolButton( bayer_ButtonGroup, "bayer_BGGR_ToolButton" );
    bayer_BGGR_ToolButton->setMinimumSize( QSize( 40, 40 ) );
    bayer_BGGR_ToolButton->setMaximumSize( QSize( 40, 40 ) );
    bayer_BGGR_ToolButton->setFocusPolicy( QToolButton::NoFocus );
    bayer_BGGR_ToolButton->setToggleButton( TRUE );

    bayer_ButtonGroupLayout->addWidget( bayer_BGGR_ToolButton, 0, 3 );

    bayer_GRBG_ToolButton = new QToolButton( bayer_ButtonGroup, "bayer_GRBG_ToolButton" );
    bayer_GRBG_ToolButton->setMinimumSize( QSize( 40, 40 ) );
    bayer_GRBG_ToolButton->setMaximumSize( QSize( 40, 40 ) );
    bayer_GRBG_ToolButton->setFocusPolicy( QToolButton::NoFocus );
    bayer_GRBG_ToolButton->setToggleButton( TRUE );

    bayer_ButtonGroupLayout->addWidget( bayer_GRBG_ToolButton, 0, 2 );

    bayer_RGGB_ToolButton = new QToolButton( bayer_ButtonGroup, "bayer_RGGB_ToolButton" );
    bayer_RGGB_ToolButton->setMinimumSize( QSize( 40, 40 ) );
    bayer_RGGB_ToolButton->setMaximumSize( QSize( 40, 40 ) );
    bayer_RGGB_ToolButton->setFocusPolicy( QToolButton::NoFocus );
    bayer_RGGB_ToolButton->setToggleButton( TRUE );

    bayer_ButtonGroupLayout->addWidget( bayer_RGGB_ToolButton, 0, 0 );

    bayerframeLayout->addWidget( bayer_ButtonGroup, 2, 1 );

    selectBayerPatternLabel = new QLabel( bayerframe, "selectBayerPatternLabel" );

    bayerframeLayout->addWidget( selectBayerPatternLabel, 1, 1 );

    ssd_GroupBox2_2Layout->addWidget( bayerframe, 1, 0 );

    tabLayout_2->addMultiCellWidget( ssd_GroupBox2_2, 1, 1, 4, 5 );

    groupBox26 = new QGroupBox( tab_2, "groupBox26" );
    groupBox26->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, groupBox26->sizePolicy().hasHeightForWidth() ) );
    groupBox26->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );
    groupBox26->setColumnLayout(0, Qt::Vertical );
    groupBox26->layout()->setSpacing( 6 );
    groupBox26->layout()->setMargin( 11 );
    groupBox26Layout = new QGridLayout( groupBox26->layout() );
    groupBox26Layout->setAlignment( Qt::AlignTop );

    ssd_preserveCDCheckBox = new QCheckBox( groupBox26, "ssd_preserveCDCheckBox" );
    ssd_preserveCDCheckBox->setFocusPolicy( QCheckBox::NoFocus );

    groupBox26Layout->addMultiCellWidget( ssd_preserveCDCheckBox, 0, 0, 0, 1 );

    layout3 = new QGridLayout( 0, 1, 1, 0, 6, "layout3"); 

    ssd_numccdsLineEdit = new QLineEdit( groupBox26, "ssd_numccdsLineEdit" );
    ssd_numccdsLineEdit->setMaximumSize( QSize( 60, 32767 ) );
    ssd_numccdsLineEdit->setPaletteBackgroundColor( QColor( 255, 187, 155 ) );
    ssd_numccdsLineEdit->setFocusPolicy( QLineEdit::NoFocus );
    ssd_numccdsLineEdit->setAlignment( int( QLineEdit::AlignRight ) );

    layout3->addWidget( ssd_numccdsLineEdit, 1, 2 );

    textLabel1 = new QLabel( groupBox26, "textLabel1" );

    layout3->addWidget( textLabel1, 0, 2 );

    ssd_transformationcodesLineEdit = new QLineEdit( groupBox26, "ssd_transformationcodesLineEdit" );
    ssd_transformationcodesLineEdit->setMaxLength( 1000 );
    ssd_transformationcodesLineEdit->setAlignment( int( QLineEdit::AlignLeft ) );

    layout3->addMultiCellWidget( ssd_transformationcodesLineEdit, 1, 1, 0, 1 );
    spacer21 = new QSpacerItem( 170, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer21, 0, 1 );

    textLabel2_3 = new QLabel( groupBox26, "textLabel2_3" );

    layout3->addWidget( textLabel2_3, 0, 0 );

    groupBox26Layout->addMultiCellLayout( layout3, 1, 1, 0, 1 );

    ssd_transformationTable = new QTable( groupBox26, "ssd_transformationTable" );
    ssd_transformationTable->setNumCols( ssd_transformationTable->numCols() + 1 );
    ssd_transformationTable->horizontalHeader()->setLabel( ssd_transformationTable->numCols() - 1, tr( "Code" ) );
    ssd_transformationTable->setNumCols( ssd_transformationTable->numCols() + 1 );
    ssd_transformationTable->horizontalHeader()->setLabel( ssd_transformationTable->numCols() - 1, tr( "Rot" ) );
    ssd_transformationTable->setNumCols( ssd_transformationTable->numCols() + 1 );
    ssd_transformationTable->horizontalHeader()->setLabel( ssd_transformationTable->numCols() - 1, tr( "Flip" ) );
    ssd_transformationTable->setNumCols( ssd_transformationTable->numCols() + 1 );
    ssd_transformationTable->horizontalHeader()->setLabel( ssd_transformationTable->numCols() - 1, tr( "Alt" ) );
    ssd_transformationTable->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, ssd_transformationTable->sizePolicy().hasHeightForWidth() ) );
    ssd_transformationTable->setMinimumSize( QSize( 0, 0 ) );
    ssd_transformationTable->setFocusPolicy( QTable::NoFocus );
    ssd_transformationTable->setFrameShape( QTable::StyledPanel );
    ssd_transformationTable->setResizePolicy( QTable::Default );
    ssd_transformationTable->setNumRows( 8 );
    ssd_transformationTable->setNumCols( 4 );
    ssd_transformationTable->setReadOnly( TRUE );
    ssd_transformationTable->setSelectionMode( QTable::NoSelection );

    groupBox26Layout->addWidget( ssd_transformationTable, 2, 0 );
    spacer22_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox26Layout->addItem( spacer22_2, 2, 1 );

    tabLayout_2->addMultiCellWidget( groupBox26, 1, 1, 0, 3 );

    ssd_GroupBox2 = new QGroupBox( tab_2, "ssd_GroupBox2" );
    ssd_GroupBox2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, ssd_GroupBox2->sizePolicy().hasHeightForWidth() ) );
    ssd_GroupBox2->setMinimumSize( QSize( 0, 0 ) );
    ssd_GroupBox2->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback_blue.png" ) );

    textLabel3_2 = new QLabel( ssd_GroupBox2, "textLabel3_2" );
    textLabel3_2->setGeometry( QRect( 150, 20, 70, 20 ) );

    ssd_dummy1LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_dummy1LineEdit" );
    ssd_dummy1LineEdit->setGeometry( QRect( 20, 50, 100, 22 ) );
    ssd_dummy1LineEdit->setMaxLength( 8 );

    ssd_dummy2LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_dummy2LineEdit" );
    ssd_dummy2LineEdit->setGeometry( QRect( 20, 70, 100, 22 ) );
    ssd_dummy2LineEdit->setMaxLength( 8 );

    ssd_dummy3LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_dummy3LineEdit" );
    ssd_dummy3LineEdit->setGeometry( QRect( 20, 90, 100, 22 ) );
    ssd_dummy3LineEdit->setMaxLength( 8 );

    ssd_dummy4LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_dummy4LineEdit" );
    ssd_dummy4LineEdit->setGeometry( QRect( 20, 110, 100, 22 ) );
    ssd_dummy4LineEdit->setMaxLength( 8 );

    ssd_dummy5LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_dummy5LineEdit" );
    ssd_dummy5LineEdit->setGeometry( QRect( 20, 130, 100, 22 ) );
    ssd_dummy5LineEdit->setMaxLength( 8 );

    ssd_dummy6LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_dummy6LineEdit" );
    ssd_dummy6LineEdit->setGeometry( QRect( 20, 150, 100, 22 ) );
    ssd_dummy6LineEdit->setMaxLength( 8 );

    ssd_dummy7LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_dummy7LineEdit" );
    ssd_dummy7LineEdit->setGeometry( QRect( 20, 170, 100, 22 ) );
    ssd_dummy7LineEdit->setMaxLength( 8 );

    ssd_rawdummy1LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_rawdummy1LineEdit" );
    ssd_rawdummy1LineEdit->setGeometry( QRect( 150, 50, 150, 22 ) );
    ssd_rawdummy1LineEdit->setMaxLength( 99 );

    ssd_rawdummy2LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_rawdummy2LineEdit" );
    ssd_rawdummy2LineEdit->setGeometry( QRect( 150, 70, 150, 22 ) );
    ssd_rawdummy2LineEdit->setMaxLength( 99 );

    ssd_rawdummy3LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_rawdummy3LineEdit" );
    ssd_rawdummy3LineEdit->setGeometry( QRect( 150, 90, 150, 22 ) );
    ssd_rawdummy3LineEdit->setMaxLength( 99 );

    ssd_rawdummy4LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_rawdummy4LineEdit" );
    ssd_rawdummy4LineEdit->setGeometry( QRect( 150, 110, 150, 22 ) );
    ssd_rawdummy4LineEdit->setMaxLength( 99 );

    ssd_rawdummy5LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_rawdummy5LineEdit" );
    ssd_rawdummy5LineEdit->setGeometry( QRect( 150, 130, 150, 22 ) );
    ssd_rawdummy5LineEdit->setMaxLength( 99 );

    ssd_rawdummy6LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_rawdummy6LineEdit" );
    ssd_rawdummy6LineEdit->setGeometry( QRect( 150, 150, 150, 22 ) );
    ssd_rawdummy6LineEdit->setMaxLength( 99 );

    ssd_rawdummy7LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_rawdummy7LineEdit" );
    ssd_rawdummy7LineEdit->setGeometry( QRect( 150, 170, 150, 22 ) );
    ssd_rawdummy7LineEdit->setMaxLength( 99 );

    ssd_rawdummy8LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_rawdummy8LineEdit" );
    ssd_rawdummy8LineEdit->setGeometry( QRect( 150, 190, 150, 22 ) );
    ssd_rawdummy8LineEdit->setMaxLength( 99 );

    textLabel2_2 = new QLabel( ssd_GroupBox2, "textLabel2_2" );
    textLabel2_2->setGeometry( QRect( 20, 20, 70, 20 ) );

    ssd_dummy8LineEdit = new QLineEdit( ssd_GroupBox2, "ssd_dummy8LineEdit" );
    ssd_dummy8LineEdit->setGeometry( QRect( 20, 190, 100, 22 ) );
    ssd_dummy8LineEdit->setMaxLength( 8 );

    tabLayout_2->addMultiCellWidget( ssd_GroupBox2, 0, 0, 3, 5 );
    cni_mainTabWidget->insertTab( tab_2, QString::fromLatin1("") );

    cni_formLayout->addMultiCellWidget( cni_mainTabWidget, 0, 0, 0, 1 );
    languageChange();
    resize( QSize(816, 678).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( cni_quitPushButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( cni_clearPushButton, SIGNAL( clicked() ), this, SLOT( clearfields() ) );
    connect( cni_geogLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( cni_instnameLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( cni_pixsclLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( validate() ) );
    connect( cni_crpix1LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_crpix2LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_cutonxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_cutonyLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_nccdSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_ovmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_ovminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_xsizeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_ysizeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( compare_nchips_nccd() ) );
    connect( cni_crpix1LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( count_ccds_entered() ) );
    connect( cni_crpix2LineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( count_ccds_entered() ) );
    connect( cni_cutonxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( count_ccds_entered() ) );
    connect( cni_cutonyLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( count_ccds_entered() ) );
    connect( cni_ovmaxLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( count_ccds_entered() ) );
    connect( cni_ovminLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( count_ccds_entered() ) );
    connect( cni_xsizeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( count_ccds_entered() ) );
    connect( cni_ysizeLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( count_ccds_entered() ) );
    connect( cni_st10PushButton, SIGNAL( clicked() ), this, SLOT( fill_in_ST10_example() ) );
    connect( cni_wfcPushButton, SIGNAL( clicked() ), this, SLOT( fill_in_WFC_example() ) );
    connect( cni_wfiPushButton, SIGNAL( clicked() ), this, SLOT( fill_in_WFI_example() ) );
    connect( cni_loadPushButton, SIGNAL( clicked() ), this, SLOT( load_instconfig() ) );
    connect( cni_savePushButton, SIGNAL( clicked() ), this, SLOT( save_instconfig() ) );
    connect( ssd_changeinstrumentPushButton, SIGNAL( clicked() ), this, SLOT( ssd_instfiledialog() ) );
    connect( ssd_transformationcodesLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( ssd_verify_input() ) );
    connect( ssd_transformationcodesLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( ssd_countccds() ) );
    connect( ssd_createscriptPushButton, SIGNAL( clicked() ), this, SLOT( write_splitscript() ) );
    connect( cni_presentinstPushButton, SIGNAL( clicked() ), this, SLOT( fill_in_presentinst() ) );
    connect( bayer_RAW_RadioButton, SIGNAL( clicked() ), this, SLOT( showhide_bayerpattern() ) );
    connect( bayer_FITS_RadioButton, SIGNAL( clicked() ), this, SLOT( showhide_bayerpattern() ) );
    connect( bayer_RGGB_ToolButton, SIGNAL( clicked() ), this, SLOT( active_passive_bayer() ) );
    connect( bayer_GBRG_ToolButton, SIGNAL( clicked() ), this, SLOT( active_passive_bayer() ) );
    connect( bayer_GRBG_ToolButton, SIGNAL( clicked() ), this, SLOT( active_passive_bayer() ) );
    connect( bayer_BGGR_ToolButton, SIGNAL( clicked() ), this, SLOT( active_passive_bayer() ) );
    connect( monochrome_FITS_RadioButton, SIGNAL( clicked() ), this, SLOT( showhide_bayerframe() ) );
    connect( cni_mainTabWidget, SIGNAL( currentChanged(QWidget*) ), this, SLOT( ssd_set_instrumentname() ) );
    connect( cni_guessPushButton, SIGNAL( clicked() ), this, SLOT( guess_camerageometry() ) );
    connect( monochrome_RAW_RadioButton, SIGNAL( clicked() ), this, SLOT( showhide_bayerpattern() ) );

    // tab order
    setTabOrder( cni_instnameLineEdit, cni_nccdSpinBox );
    setTabOrder( cni_nccdSpinBox, cni_geogLineEdit );
    setTabOrder( cni_geogLineEdit, cni_pixsclLineEdit );
    setTabOrder( cni_pixsclLineEdit, cni_ovminLineEdit );
    setTabOrder( cni_ovminLineEdit, cni_ovmaxLineEdit );
    setTabOrder( cni_ovmaxLineEdit, cni_cutonxLineEdit );
    setTabOrder( cni_cutonxLineEdit, cni_cutonyLineEdit );
    setTabOrder( cni_cutonyLineEdit, cni_xsizeLineEdit );
    setTabOrder( cni_xsizeLineEdit, cni_ysizeLineEdit );
    setTabOrder( cni_ysizeLineEdit, cni_crpix1LineEdit );
    setTabOrder( cni_crpix1LineEdit, cni_crpix2LineEdit );
    setTabOrder( cni_crpix2LineEdit, ssd_rakeyLineEdit );
    setTabOrder( ssd_rakeyLineEdit, ssd_deckeyLineEdit );
    setTabOrder( ssd_deckeyLineEdit, ssd_lstkeyLineEdit );
    setTabOrder( ssd_lstkeyLineEdit, ssd_mjdkeyLineEdit );
    setTabOrder( ssd_mjdkeyLineEdit, ssd_exptimekeyLineEdit );
    setTabOrder( ssd_exptimekeyLineEdit, ssd_airmasskeyLineEdit );
    setTabOrder( ssd_airmasskeyLineEdit, ssd_filterkeyLineEdit );
    setTabOrder( ssd_filterkeyLineEdit, ssd_equinoxLineEdit );
    setTabOrder( ssd_equinoxLineEdit, ssd_dummy1LineEdit );
    setTabOrder( ssd_dummy1LineEdit, ssd_rawdummy1LineEdit );
    setTabOrder( ssd_rawdummy1LineEdit, ssd_dummy2LineEdit );
    setTabOrder( ssd_dummy2LineEdit, ssd_rawdummy2LineEdit );
    setTabOrder( ssd_rawdummy2LineEdit, ssd_dummy3LineEdit );
    setTabOrder( ssd_dummy3LineEdit, ssd_rawdummy3LineEdit );
    setTabOrder( ssd_rawdummy3LineEdit, ssd_dummy4LineEdit );
    setTabOrder( ssd_dummy4LineEdit, ssd_rawdummy4LineEdit );
    setTabOrder( ssd_rawdummy4LineEdit, ssd_dummy5LineEdit );
    setTabOrder( ssd_dummy5LineEdit, ssd_rawdummy5LineEdit );
    setTabOrder( ssd_rawdummy5LineEdit, ssd_dummy6LineEdit );
    setTabOrder( ssd_dummy6LineEdit, ssd_rawdummy6LineEdit );
    setTabOrder( ssd_rawdummy6LineEdit, ssd_dummy7LineEdit );
    setTabOrder( ssd_dummy7LineEdit, ssd_rawdummy7LineEdit );
    setTabOrder( ssd_rawdummy7LineEdit, ssd_dummy8LineEdit );
    setTabOrder( ssd_dummy8LineEdit, ssd_rawdummy8LineEdit );
    setTabOrder( ssd_rawdummy8LineEdit, ssd_transformationcodesLineEdit );

    // buddies
    cni_ovmaxTextLabel->setBuddy( cni_ovmaxLineEdit );
    cni_cutonxTextLabel->setBuddy( cni_cutonxLineEdit );
    cni_cutonyTextLabel->setBuddy( cni_cutonyLineEdit );
    cni_xsizeTextLabel->setBuddy( cni_xsizeLineEdit );
    cni_ysizeTextLabel->setBuddy( cni_ysizeLineEdit );
    cni_crpix1TextLabel->setBuddy( cni_crpix1LineEdit );
    cni_crpix2TextLabel->setBuddy( cni_crpix2LineEdit );
    cni_ovminTextLabel->setBuddy( cni_ovminLineEdit );
    cni_instnameTextLabel->setBuddy( cni_instnameLineEdit );
    cni_nccdTextLabel->setBuddy( cni_nccdSpinBox );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
cni_form::~cni_form()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void cni_form::languageChange()
{
    setCaption( tr( "Create new instrument" ) );
    cni_quitPushButton->setText( tr( "&Close" ) );
    cni_quitPushButton->setAccel( QKeySequence( tr( "Alt+Q" ) ) );
    cni_savePushButton->setText( tr( "&Save" ) );
    cni_savePushButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    cni_clearPushButton->setText( tr( "&Clear All" ) );
    cni_clearPushButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    cni_loadPushButton->setText( tr( "&Load ..." ) );
    cni_loadPushButton->setAccel( QKeySequence( tr( "Alt+L" ) ) );
    cni_guessPushButton->setText( tr( "Guess ..." ) );
    QToolTip::add( cni_guessPushButton, tr( "Guess the camera geometry based on a well-exposed flatfield.<br>" ) );
    cni_fillexampleGroupBox->setTitle( tr( "Fill in examples" ) );
    cni_st10PushButton->setText( tr( "ST-10 (1 CCD)" ) );
    cni_wfcPushButton->setText( tr( "WFC@INT (4 CCDs)" ) );
    cni_wfiPushButton->setText( tr( "WFI@MPGESO (8 CCDs)" ) );
    cni_presentinstPushButton->setText( tr( "current instrument" ) );
    cni_ccdgeomGroupBox->setTitle( tr( "CCD geometries" ) );
    QWhatsThis::add( cni_ccdgeomGroupBox, tr( "<p><font color=\"#aa0000\" size=\"+1\">CCD geometries</font><br>\n"
"Each of the eight fields takes a blank-separated list of <i>n</i> numerical \n"
"values, where <i>n</i> is the number of CCDs your camera has (you entered \n"
"this above).\n"
"</p>\n"
"<p>\n"
"The <b>Overscan xmin</b> and <b>xmax</b> specify the overscan region of your \n"
"detector along the x-axis. <b>Cut-on x/y</b> are the first pixels in x- and y-direction \n"
"that receive light, i.e. those that do not belong to the overscan anymore. <b>x-size\n"
"</b> and <b>y-size</b> are the extents in pixels of the light-sensitive part of the \n"
"detectors, counted from <b>Cut-on x/y</b>, respectively. Finally, you have to \n"
"provide the coordinates of the reference pixels (<b>CRPIX1/2</b>). These refer to the \n"
"pixel that falls upon the sky coordinates given in the FITS header (usually by CRVAL1/2 keywords). It is sufficient if you get these accurate to within some 100 pixels. Remaining offsets are automatically corrected later on in the astrometry section.</p>" ) );
    cni_textLabel8->setText( tr( "# CCDs" ) );
    cni_crpix2numchkLineEdit->setText( tr( "0 " ) );
    cni_crpix2numchkLineEdit->setInputMask( QString::null );
    QToolTip::add( cni_crpix2numchkLineEdit, tr( "Number of blank-separated fields already entered" ) );
    cni_ysizenumchkLineEdit->setText( tr( "0 " ) );
    cni_ysizenumchkLineEdit->setInputMask( QString::null );
    QToolTip::add( cni_ysizenumchkLineEdit, tr( "Number of blank-separated fields already entered" ) );
    cni_cutonynumchkLineEdit->setText( tr( "0 " ) );
    cni_cutonynumchkLineEdit->setInputMask( QString::null );
    QToolTip::add( cni_cutonynumchkLineEdit, tr( "Number of blank-separated fields already entered" ) );
    cni_ovmaxnumchkLineEdit->setText( tr( "0 " ) );
    cni_ovmaxnumchkLineEdit->setInputMask( QString::null );
    QToolTip::add( cni_ovmaxnumchkLineEdit, tr( "Number of blank-separated fields already entered" ) );
    cni_crpix1numchkLineEdit->setText( tr( "0 " ) );
    cni_crpix1numchkLineEdit->setInputMask( QString::null );
    QToolTip::add( cni_crpix1numchkLineEdit, tr( "Number of blank-separated fields already entered" ) );
    cni_xsizenumchkLineEdit->setText( tr( "0 " ) );
    cni_xsizenumchkLineEdit->setInputMask( QString::null );
    QToolTip::add( cni_xsizenumchkLineEdit, tr( "Number of blank-separated fields already entered" ) );
    cni_cutonxnumchkLineEdit->setText( tr( "0 " ) );
    cni_cutonxnumchkLineEdit->setInputMask( QString::null );
    QToolTip::add( cni_cutonxnumchkLineEdit, tr( "Number of blank-separated fields already entered" ) );
    cni_ovmaxTextLabel->setText( tr( "Overscan xmax" ) );
    QToolTip::add( cni_ovmaxTextLabel, tr( "Where the overscan region ends. If your camera does not have an overscan, enter 0.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    cni_cutonxTextLabel->setText( tr( "Cut-on x" ) );
    QToolTip::add( cni_cutonxTextLabel, tr( "Smallest x coordinate of a pixel that contains light. Normally, that's the one next to the end of the overscan region. Use a pixel further inside if you want to exclude some bad columns at the border of the CCD.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    cni_cutonyTextLabel->setText( tr( "Cut-on y" ) );
    QToolTip::add( cni_cutonyTextLabel, tr( "Smallest y coordinate of a pixel that contains light. Use a pixel further inside if you want to exclude some bad rows at the border of the CCD.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    cni_xsizeTextLabel->setText( tr( "x size" ) );
    QToolTip::add( cni_xsizeTextLabel, tr( "Extent of the light-sensitive part of the CCD in x direction.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    cni_ysizeTextLabel->setText( tr( "y size" ) );
    QToolTip::add( cni_ysizeTextLabel, tr( "Extent of the light-sensitive part of the CCD in y direction.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    cni_crpix1TextLabel->setText( tr( "CRPIX1" ) );
    QToolTip::add( cni_crpix1TextLabel, tr( "The reference pixel(s) for the CCD(s). Get it accurate to within 100 or so, we determine the exact value automatically at a later step.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    cni_crpix2TextLabel->setText( tr( "CRPIX2" ) );
    QToolTip::add( cni_crpix2TextLabel, tr( "The reference pixel(s) for the CCD(s). Get it accurate to within 100 or so, we determine the exact value automatically at a later step.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    QToolTip::add( cni_ysizeLineEdit, tr( "Extent of the light-sensitive part of the CCD in y direction.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    QToolTip::add( cni_crpix2LineEdit, tr( "The reference pixel(s) for the CCD(s). Get it accurate to within 100 or so, we determine the exact value automatically at a later step.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    QToolTip::add( cni_cutonyLineEdit, tr( "Smallest y coordinate of a pixel that contains light. Use a pixel further inside if you want to exclude some bad rows at the border of the CCD.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs.  Only numbers larger than zero are permitted!" ) );
    QToolTip::add( cni_ovmaxLineEdit, tr( "Where the overscan region ends. If your camera does not have an overscan, enter 0.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    QToolTip::add( cni_cutonxLineEdit, tr( "Smallest x coordinate of a pixel that contains light. Normally, that's the one next to the end of the overscan region. Use a pixel further inside if you want to exclude some bad columns at the border of the CCD.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs. Only numbers larger than zero are permitted!" ) );
    QToolTip::add( cni_xsizeLineEdit, tr( "Extent of the light-sensitive part of the CCD in x direction.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    QToolTip::add( cni_crpix1LineEdit, tr( "The reference pixel(s) for the CCD(s). Get it accurate to within 100 or so, we determine the exact value automatically at a later step.<br>For multi-chip cameras, fill in a blank separated list. We need as many numbers as CCDs." ) );
    cni_ovminTextLabel->setText( tr( "Overscan xmin" ) );
    QToolTip::add( cni_ovminTextLabel, tr( "Where the overscan region starts. If your camera does not have an overscan, enter 0.<br>For multi-chip cameras, fill in a blank separated list (0s if no overscan present). We need as many numbers as CCDs." ) );
    cni_ovminnumchkLineEdit->setText( tr( "0 " ) );
    cni_ovminnumchkLineEdit->setInputMask( QString::null );
    QToolTip::add( cni_ovminnumchkLineEdit, tr( "Number of blank-separated fields already entered" ) );
    cni_ovminLineEdit->setText( QString::null );
    QToolTip::add( cni_ovminLineEdit, tr( "Where the overscan region starts. If your camera does not have an overscan, enter 0.<br>For multi-chip cameras, fill in a blank separated list (0s if no overscan present). We need as many numbers as CCDs." ) );
    cni_genpropGroupBox->setTitle( tr( "General properties" ) );
    cni_instnameTextLabel->setText( tr( "Instrument name" ) );
    QToolTip::add( cni_instnameTextLabel, QString::null );
    QWhatsThis::add( cni_instnameTextLabel, tr( "<p><font color=\"#aa0000\" size=\"+1\">Instrument name</font><br>\n"
"Specify the name of your instrument. Choose a meaningful name that reflects the \n"
"instrument itself, as well as the telescope or observatory used. Like that you avoid \n"
"confusion. For example, use<br>\n"
"WIDECAM@LARGESCOPE<br>\n"
"for your instrument name instead of <br>\n"
"WCAM.<br>\n"
"This will also become the file name of your instrument configuration file when saving \n"
"the form.</p>" ) );
    cni_nccdTextLabel->setText( tr( "Number of CCDs" ) );
    QToolTip::add( cni_nccdTextLabel, tr( "Select the number of CCDs in your camera<br> (without guide chips)." ) );
    cni_geoglTextLabel->setText( tr( "Geographic latitude" ) );
    QToolTip::add( cni_geoglTextLabel, tr( "Enter the geographical latitude of your site. <br>We need it for airmass calculation if this keyword is not present in your FITS headers." ) );
    QWhatsThis::add( cni_geoglTextLabel, tr( "<p><font color=\"#aa0000\" size=\"+1\">Geographical latitude</font><br>\n"
"Enter the geographical latitude of your site in degrees. We need it for the calculation of \n"
"the airmass if this keyword is not present in your FITS files. Observatories on the southern \n"
"hemisphere count negative.\n"
"</p>" ) );
    cni_gainTextLabel->setText( tr( "Pixel scale (arcsec)" ) );
    QToolTip::add( cni_gainTextLabel, QString::null );
    textLabel1_2->setText( tr( "Camera type" ) );
    QToolTip::add( cni_pixsclLineEdit, tr( "The pixel scale in arcsec" ) );
    QToolTip::add( cni_geogLineEdit, tr( "The geographical latitude of your observatory" ) );
    QToolTip::add( cni_nccdSpinBox, tr( "The number of CCDs your instrument has." ) );
    QToolTip::add( cni_instnameLineEdit, tr( "The name of the instrument you want to define. Choose a meaningful name such as INSTRUMENT@TELESCOPE or INSTRUMENT@OBSERVATORY.<br>" ) );
    cni_cameratypeComboBox->clear();
    cni_cameratypeComboBox->insertItem( tr( "OPTICAL" ) );
    cni_cameratypeComboBox->insertItem( tr( "NEAR-IR" ) );
    cni_cameratypeComboBox->insertItem( tr( "MID-IR" ) );
    QToolTip::add( cni_cameratypeComboBox, tr( "Select whether this is an optical, near-infrared or mid-infrared instrument" ) );
    cni_mainTabWidget->changeTab( tab, tr( "Instrument configuration" ) );
    ssd_GroupBox1->setTitle( tr( "Mandatory FITS keywords" ) );
    QWhatsThis::add( ssd_GroupBox1, tr( "<b><font color=\"#0000ff\">Mandatory FITS key names</font></b>\n"
"\n"
"<p>Here you find eight basic FITS keywords that must appear in the THELI FITS header.  For the first three, RA, DEC and LST, you must specify the format in which they are given in your raw data. If there is a key which is not present in your data, leave the corresponding field empty.</p>" ) );
    textLabel5->setText( tr( "RA" ) );
    textLabel2->setText( tr( "<b>THELI</b>" ) );
    textLabel3->setText( tr( "<b>Raw data</b>" ) );
    textLabel6->setText( tr( "DEC" ) );
    textLabel7->setText( tr( "LST" ) );
    textLabel8->setText( tr( "MJD" ) );
    textLabel9->setText( tr( "EXPTIME" ) );
    textLabel10->setText( tr( "AIRMASS" ) );
    textLabel4->setText( tr( "FILTER" ) );
    textLabel11->setText( tr( "<b>Format</b>" ) );
    textLabel4_2->setText( tr( "EQUINOX" ) );
    ssd_raComboBox->clear();
    ssd_raComboBox->insertItem( tr( "DD.ddd" ) );
    ssd_raComboBox->insertItem( tr( "HH:MM:SS" ) );
    ssd_raComboBox->insertItem( tr( "HH MM SS" ) );
    QToolTip::add( ssd_raComboBox, tr( "Select the format in your FITS header" ) );
    ssd_decComboBox->clear();
    ssd_decComboBox->insertItem( tr( "DD.ddd" ) );
    ssd_decComboBox->insertItem( tr( "DD:MM:SS" ) );
    ssd_decComboBox->insertItem( tr( "DD MM SS" ) );
    QToolTip::add( ssd_decComboBox, tr( "Select the format in your FITS header" ) );
    ssd_rakeyLineEdit->setText( tr( "RA" ) );
    QToolTip::add( ssd_rakeyLineEdit, tr( "Right ascension" ) );
    ssd_deckeyLineEdit->setText( tr( "DEC" ) );
    QToolTip::add( ssd_deckeyLineEdit, tr( "Declination" ) );
    ssd_lstkeyLineEdit->setText( tr( "LST" ) );
    QToolTip::add( ssd_lstkeyLineEdit, tr( "Local sidereal time" ) );
    ssd_mjdkeyLineEdit->setText( tr( "MJD-OBS" ) );
    QToolTip::add( ssd_mjdkeyLineEdit, tr( "Modified julian date" ) );
    ssd_exptimekeyLineEdit->setText( tr( "EXPTIME" ) );
    QToolTip::add( ssd_exptimekeyLineEdit, tr( "Exposure time keyword; can also be a comma-separated list (DIT,NDIT) in case of co-averaged near-IR exposures (DIT=int. time, NDIT=number of coadds)<br>" ) );
    ssd_airmasskeyLineEdit->setText( tr( "AIRMASS" ) );
    QToolTip::add( ssd_airmasskeyLineEdit, QString::null );
    ssd_filterkeyLineEdit->setText( tr( "FILTER" ) );
    QToolTip::add( ssd_filterkeyLineEdit, tr( "The FILTER keyword. If two filters are present, you can provide a comma-separated list (FILTER1,FILTER2). The resulting keyword will then be written as FILTER1+FILTER2<br>" ) );
    ssd_equinoxLineEdit->setText( tr( "EQUINOX" ) );
    QToolTip::add( ssd_equinoxLineEdit, QString::null );
    ssd_lstComboBox->clear();
    ssd_lstComboBox->insertItem( tr( "DD.ddd" ) );
    ssd_lstComboBox->insertItem( tr( "HH:MM:SS" ) );
    ssd_lstComboBox->insertItem( tr( "HH MM SS" ) );
    QToolTip::add( ssd_lstComboBox, tr( "Select the format in your FITS header" ) );
    QToolTip::add( ssd_instrumentLineEdit, tr( "The instrument for which we the script is created." ) );
    ssd_createscriptPushButton->setText( tr( "Create script" ) );
    ssd_changeinstrumentPushButton->setText( tr( "Change instrument" ) );
    ssd_GroupBox2_2->setTitle( tr( "Optional: Bayer matrix pattern" ) );
    buttonGroup3->setTitle( QString::null );
    monochrome_FITS_RadioButton->setText( tr( "Monochrome (FITS format)" ) );
    QToolTip::add( monochrome_FITS_RadioButton, tr( "Select this if you work with a normal CCD or near-IR detector" ) );
    bayer_FITS_RadioButton->setText( tr( "RGB color matrix (FITS format)" ) );
    QToolTip::add( bayer_FITS_RadioButton, tr( "Select this if the image is stored in a single FITS file without colour information." ) );
    bayer_RAW_RadioButton->setText( tr( "RGB color matrix (RAW format)" ) );
    QToolTip::add( bayer_RAW_RadioButton, tr( "Select this if you work with a DSLR camera that has the Bayer matrix in place" ) );
    monochrome_RAW_RadioButton->setText( tr( "Monochrome (RAW format)" ) );
    QToolTip::add( monochrome_RAW_RadioButton, tr( "Select this if you work with a DSLR camera with the Bayer matrix removed" ) );
    bayer_ButtonGroup->setTitle( QString::null );
    bayer_GBRG_ToolButton->setText( QString::null );
    QToolTip::add( bayer_GBRG_ToolButton, tr( "GBRG" ) );
    bayer_BGGR_ToolButton->setText( QString::null );
    QToolTip::add( bayer_BGGR_ToolButton, tr( "BGGR" ) );
    bayer_GRBG_ToolButton->setText( QString::null );
    QToolTip::add( bayer_GRBG_ToolButton, tr( "GRBG" ) );
    bayer_RGGB_ToolButton->setText( QString::null );
    QToolTip::add( bayer_RGGB_ToolButton, tr( "RGGB" ) );
    selectBayerPatternLabel->setText( tr( "Select Bayer pattern:" ) );
    groupBox26->setTitle( tr( "Transformation" ) );
    QWhatsThis::add( groupBox26, tr( "<b><font color=\"#0000ff\">Rotating / Flipping of CCDs</font></b>\n"
"\n"
"<p>We recommend that you orient your CCDs such that they have<p align=\"center\"><b>North up and East left</b></p> For many instruments this is not the case, rotations and flips might be involved. If you work with a multi-chip camera, then you <b>MUST</b> give all CCDs the same orientation. WFC@INT is an example where one of the four CCDs is rotated with respect to the other three.</p>\n"
"\n"
"<p>Enter a blank-separated list of transformation codes. These are explained in the table below, e.g. the code \"4\" corresponds to a rotation of 90 degrees, and a subsequent flip in x direction. If a rotation and a flip are combined, then the rotation is always performed first. Positive rotation angles are counted counter-clock-wise. You need to enter as many codes as you have chips in the camera. The little LineEdit shows you how many CCDs you have already entered.</p>" ) );
    ssd_preserveCDCheckBox->setText( tr( "Preserve CD matrix and reference pixel" ) );
    ssd_numccdsLineEdit->setText( tr( "0" ) );
    QToolTip::add( ssd_numccdsLineEdit, tr( "The number of CCDs you have already entered." ) );
    textLabel1->setText( tr( "# CCDs" ) );
    QToolTip::add( ssd_transformationcodesLineEdit, tr( "Enter here a blank-separated list of transformation codes. We need as many codes as there are chips in the camera.<br>Example: 6 7 6 6 for WFC@INT" ) );
    textLabel2_3->setText( tr( "Code:" ) );
    ssd_transformationTable->horizontalHeader()->setLabel( 0, tr( "Code" ) );
    ssd_transformationTable->horizontalHeader()->setLabel( 1, tr( "Rot" ) );
    ssd_transformationTable->horizontalHeader()->setLabel( 2, tr( "Flip" ) );
    ssd_transformationTable->horizontalHeader()->setLabel( 3, tr( "Alt" ) );
    QToolTip::add( ssd_transformationTable, tr( "This table contains the available codes and associated transformations." ) );
    ssd_GroupBox2->setTitle( tr( "Optional FITS keynames" ) );
    QWhatsThis::add( ssd_GroupBox2, tr( "<b><font color=\"#0000ff\">Optional FITS keynames</font></b>\n"
"\n"
"<p>You can enter here up to 8 <i>descriptive </i>FITS keywords from your original data that should be carried over to the <tt><i>THELI</i> </tt>headers. The maximum length of the THELI keywords is 8 characters, the key in your raw data can be longer. If you leave the THELI field empty, the THELI header keyword will have the same name as in your raw data. It will be truncated after 8 characters.</p>\n"
"<p><b>EXCLUDED </b>are header keywords dealing with any kind of astrometric information, such as CDij, CRPIXi, CRVALi, CDELTi, CROTAi, CTYPEi, or any kind of distortion coefficients. </p>\n"
"<p><b>DO NOT DUPLICATE</b> existing keywords. For example, from the left GroupBox you see that there is already a FILTER keyword. Thus, if you want to include another FILTER in the set of header keywords, name it \"FILTER2\" or similar.</p>" ) );
    textLabel3_2->setText( tr( "<b>Raw data</b>" ) );
    ssd_dummy1LineEdit->setText( QString::null );
    ssd_dummy2LineEdit->setText( QString::null );
    ssd_dummy3LineEdit->setText( QString::null );
    ssd_dummy4LineEdit->setText( QString::null );
    ssd_dummy5LineEdit->setText( QString::null );
    ssd_dummy6LineEdit->setText( QString::null );
    ssd_dummy7LineEdit->setText( QString::null );
    ssd_rawdummy1LineEdit->setText( QString::null );
    ssd_rawdummy2LineEdit->setText( QString::null );
    ssd_rawdummy3LineEdit->setText( QString::null );
    ssd_rawdummy4LineEdit->setText( QString::null );
    ssd_rawdummy5LineEdit->setText( QString::null );
    ssd_rawdummy6LineEdit->setText( QString::null );
    ssd_rawdummy7LineEdit->setText( QString::null );
    ssd_rawdummy8LineEdit->setText( QString::null );
    textLabel2_2->setText( tr( "<b>THELI</b>" ) );
    ssd_dummy8LineEdit->setText( QString::null );
    cni_mainTabWidget->changeTab( tab_2, tr( "Splitting script" ) );
}

