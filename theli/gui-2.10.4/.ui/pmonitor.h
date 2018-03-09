/****************************************************************************
** Form interface generated from reading ui file 'pmonitor.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PMONITOR_H
#define PMONITOR_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListBox;
class QListBoxItem;
class QPushButton;
class QLabel;
class QLineEdit;
class QProgressBar;
class QFrame;
class QCheckBox;
class QSpinBox;

class pmonitor : public QWidget
{
    Q_OBJECT

public:
    pmonitor( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~pmonitor();

    QListBox* progress_listBox;
    QPushButton* progress_resetpushButton;
    QPushButton* progress_startpushButton;
    QPushButton* progress_startpushButton_newstamp;
    QPushButton* btn_service;
    QLabel* textLabel1_2;
    QLineEdit* projectname;
    QLabel* textLabel1;
    QLineEdit* progress_status;
    QProgressBar* progressBar;
    QLineEdit* progress_info;
    QPushButton* btn_plots;
    QPushButton* btn_protocol;
    QFrame* frame3;
    QCheckBox* chk_blink;
    QLineEdit* fitsFile;
    QLabel* textLabel2;
    QSpinBox* blinkInterval;
    QCheckBox* chk_cat;
    QPushButton* btn_fits;
    QPushButton* btn_show_Bayer;
    QPushButton* btn_cor_NOMAD;
    QPushButton* btn_stats;

public slots:
    virtual void p_start();
    virtual void p_reset();
    virtual void timer_done();
    virtual void show_fits();
    virtual void show_plots();
    virtual void show_stats();
    virtual void show_protocol();
    virtual void btn_cor_NOMAD_clicked();
    virtual void newStamp();
    virtual void btn_service_clicked();
    virtual void btn_show_Bayer_clicked();

protected:

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // PMONITOR_H
