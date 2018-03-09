/****************************************************************************
** Form interface generated from reading ui file 'scamp_mosaictype.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SCAMP_MOSAICTYPE_H
#define SCAMP_MOSAICTYPE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTextBrowser;

class scamp_mosaictype : public QDialog
{
    Q_OBJECT

public:
    scamp_mosaictype( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~scamp_mosaictype();

    QPushButton* buttonOk;
    QTextBrowser* scamp_mosacitypeTextBrowser;

protected:
    QGridLayout* scamp_mosaictypeLayout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // SCAMP_MOSAICTYPE_H
