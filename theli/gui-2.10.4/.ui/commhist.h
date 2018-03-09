/****************************************************************************
** Form interface generated from reading ui file 'commhist.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef COMMHIST_H
#define COMMHIST_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTextEdit;
class QPushButton;

class commhist : public QDialog
{
    Q_OBJECT

public:
    commhist( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~commhist();

    QLabel* ch_titleTextLabel;
    QTextEdit* ch_commandhistoryTextEdit;
    QPushButton* ch_clearPushButton;
    QPushButton* ch_saveasPushButton;
    QPushButton* ch_quitPushButton;

public slots:
    virtual void clear_history();
    virtual void save_history();

protected:
    QVBoxLayout* commhistLayout;
    QHBoxLayout* layout12;
    QHBoxLayout* layout13;
    QSpacerItem* spacer10;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // COMMHIST_H
