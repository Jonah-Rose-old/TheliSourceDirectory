/****************************************************************************
** Form interface generated from reading ui file 'statistics.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef STATISTICS_H
#define STATISTICS_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QButtonGroup;
class QRadioButton;
class QTable;

class statistics : public QDialog
{
    Q_OBJECT

public:
    statistics( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~statistics();

    QGroupBox* groupBox5;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QLineEdit* statistics_dirLineEdit;
    QLineEdit* statistics_filterLineEdit;
    QPushButton* statistics_selectPushButton;
    QPushButton* statistics_closePushButton;
    QGroupBox* groupBox2;
    QLabel* textLabel4_2_2;
    QLabel* textLabel1_4;
    QLabel* textLabel4;
    QLabel* textLabel4_2;
    QLabel* textLabel2_2;
    QLineEdit* statistics_xmaxLineEdit;
    QLineEdit* statistics_xminLineEdit;
    QLineEdit* statistics_yminLineEdit;
    QLineEdit* statistics_ymaxLineEdit;
    QButtonGroup* buttonGroup1;
    QRadioButton* statistics_seeingarcsecRadioButton;
    QRadioButton* statistics_seeingpixelRadioButton;
    QLabel* textLabel1_2;
    QTable* statistics_table;
    QLabel* textLabel1_3;
    QPushButton* statistics_clearPushButton;
    QPushButton* statistics_getstatisticsPushButton;
    QPushButton* statistics_abortPushButton;

public slots:
    virtual void validate();
    virtual void clearfields();
    virtual void get_statistics();
    virtual void fill_in_table();
    virtual void populate_statisticstable();
    virtual void update_statisticstable();
    virtual void select_dirimage();
    virtual void kill_stats();
    virtual void quit();

protected:
    QGridLayout* statisticsLayout;
    QSpacerItem* spacer4;
    QSpacerItem* spacer5;
    QGridLayout* groupBox5Layout;
    QGridLayout* groupBox2Layout;
    QSpacerItem* spacer11;
    QSpacerItem* spacer9;
    QGridLayout* layout8;
    QGridLayout* buttonGroup1Layout;
    QVBoxLayout* layout31;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // STATISTICS_H
