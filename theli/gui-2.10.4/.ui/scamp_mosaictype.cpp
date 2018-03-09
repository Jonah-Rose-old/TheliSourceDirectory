/****************************************************************************
** Form implementation generated from reading ui file 'scamp_mosaictype.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "scamp_mosaictype.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtextbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a scamp_mosaictype as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
scamp_mosaictype::scamp_mosaictype( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "scamp_mosaictype" );
    setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "guiback.png" ) );
    setSizeGripEnabled( TRUE );
    setModal( TRUE );
    scamp_mosaictypeLayout = new QGridLayout( this, 1, 1, 11, 6, "scamp_mosaictypeLayout"); 

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    scamp_mosaictypeLayout->addLayout( Layout1, 1, 0 );

    scamp_mosacitypeTextBrowser = new QTextBrowser( this, "scamp_mosacitypeTextBrowser" );
    scamp_mosacitypeTextBrowser->setPaletteBackgroundColor( QColor( 255, 254, 212 ) );
    scamp_mosacitypeTextBrowser->setTextFormat( QTextBrowser::RichText );
    scamp_mosacitypeTextBrowser->setAutoFormatting( int( QTextBrowser::AutoAll ) );

    scamp_mosaictypeLayout->addWidget( scamp_mosacitypeTextBrowser, 0, 0 );
    languageChange();
    resize( QSize(625, 645).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
scamp_mosaictype::~scamp_mosaictype()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void scamp_mosaictype::languageChange()
{
    setCaption( tr( "SCAMP: MOSAICTYPE" ) );
    buttonOk->setText( tr( "&Close" ) );
    buttonOk->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    scamp_mosacitypeTextBrowser->setText( tr( "From the SCAMP user manual:\n"
"\n"
"<p>SCAMP can manipulate mosaics in a number of ways to perform the matching of sources on the sky, and the astrometric calibration itself. <b>For single-chip cameras only the UNCHANGED mode makes sense!</b></p>\n"
"\n"
"<p><b><font color=\"#00007f\">UNCHANGED</font></b>:<br>\n"
"The relative positioning of detectors on the focal plane, as recorded in the WCS keywords of the FITS headers, is assumed to be correct and constant from exposure to exposure. Matching with the reference catalogue will be done for all the detectors at once.</p>\n"
"\n"
"<p><b><font color=\"#00007f\">SHARE PROJAXIS</font></b>: <br>\n"
"The relative positioning of detectors is assumed to be constant and correct, but the different extensions within the same catalogue file do not share the same projection axis (the CRVAL FITS WCS keywords are different): although this does not prevent SCAMP to derive an accurate solution, this is generally not an efficient astrometric description of the focal plane. This option brings all extensions to the same centered projection axis while compensating with other WCS parameters.</p>\n"
"\n"
"<p><b><font color=\"#00007f\">SAME CRVAL</font></b>: <br>\n"
"Like SHARE PROJAXIS above, brings all extensions to the same centered projection axis (CRVAL parameters), but does not compensate by changing other WCS parameters. This option is useful when the CRPIX and CD WCS parameters are overrided by some focal plane model stored in a global .ahead file.</p>\n"
"\n"
"<p><b><font color=\"#00007f\">FIX FOCALPLANE</font></b>: <br>\n"
"Applies first a SHARE PROJAXIS correction to the headers and then attempts to derive a common, median relative positioning of detectors within the\n"
"focal plane. This mode is useful to fix the positions of detectors when these have been derived independently at each exposure in an earlier not-so-robust calibration. A minimum of 5 exposures per astrometric instrument is recommended.</p>\n"
"\n"
"<p><b><font color=\"#00007f\">LOOSE</font></b>: <br>\n"
"Makes all detector positions to be considered as independent between exposures. Contrary to other modes, matching with the reference catalogue will\n"
"be conducted separately for each extension. The LOOSE mode is generally used for totally uncalibrated mosaics in a first SCAMP pass before doing a FIX FOCALPLANE.</p>" ) );
}

