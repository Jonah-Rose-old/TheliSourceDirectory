/****************************************************************************
** Form implementation generated from reading ui file 'commhist.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "commhist.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../commhist.ui.h"
/*
 *  Constructs a commhist as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
commhist::commhist( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "commhist" );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    setModal( TRUE );
    commhistLayout = new QVBoxLayout( this, 11, 6, "commhistLayout"); 

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    ch_titleTextLabel = new QLabel( this, "ch_titleTextLabel" );
    layout12->addWidget( ch_titleTextLabel );
    commhistLayout->addLayout( layout12 );

    ch_commandhistoryTextEdit = new QTextEdit( this, "ch_commandhistoryTextEdit" );
    ch_commandhistoryTextEdit->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    ch_commandhistoryTextEdit->setWordWrap( QTextEdit::NoWrap );
    ch_commandhistoryTextEdit->setReadOnly( TRUE );
    commhistLayout->addWidget( ch_commandhistoryTextEdit );

    layout13 = new QHBoxLayout( 0, 0, 6, "layout13"); 
    spacer10 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout13->addItem( spacer10 );

    ch_clearPushButton = new QPushButton( this, "ch_clearPushButton" );
    layout13->addWidget( ch_clearPushButton );

    ch_saveasPushButton = new QPushButton( this, "ch_saveasPushButton" );
    layout13->addWidget( ch_saveasPushButton );

    ch_quitPushButton = new QPushButton( this, "ch_quitPushButton" );
    layout13->addWidget( ch_quitPushButton );
    commhistLayout->addLayout( layout13 );
    languageChange();
    resize( QSize(714, 267).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ch_quitPushButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( ch_clearPushButton, SIGNAL( clicked() ), this, SLOT( clear_history() ) );
    connect( ch_saveasPushButton, SIGNAL( clicked() ), this, SLOT( save_history() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
commhist::~commhist()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void commhist::languageChange()
{
    setCaption( tr( "Command history" ) );
    ch_titleTextLabel->setText( tr( "Command history for LOG :" ) );
    ch_clearPushButton->setText( tr( "Clear" ) );
    ch_saveasPushButton->setText( tr( "Save as ..." ) );
    ch_quitPushButton->setText( tr( "Close" ) );
}

