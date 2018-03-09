/****************************************************************************
** Form interface generated from reading ui file 'zp.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ZP_H
#define ZP_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QGroupBox;
class QLabel;
class QLineEdit;
class QComboBox;

class zp : public QDialog
{
    Q_OBJECT

public:
    zp( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~zp();

    QPushButton* zp_abortPushButton;
    QPushButton* zp_closePushButton;
    QPushButton* zp_startPushButton;
    QGroupBox* groupBox20;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLabel* textLabel1_3;
    QLabel* textLabel1;
    QPushButton* zp_loadPushButton;
    QLineEdit* zp_imageLineEdit;
    QLabel* textLabel4_2;
    QLineEdit* zp_resultLineEdit;
    QLabel* textLabel4;
    QLabel* textLabel5_3;
    QLineEdit* zp_resultrmsLineEdit;
    QLabel* textLabel1_4;
    QLabel* textLabel5;
    QLabel* textLabel4_4;
    QLineEdit* zp_resulterrLineEdit;
    QLabel* textLabel4_5;
    QLabel* textLabel2_2;
    QLabel* textLabel5_2;
    QLineEdit* zp_resultnumLineEdit;
    QLabel* textLabel3_2;
    QLabel* textLabel5_4;
    QLabel* textLabel4_3;
    QLabel* textLabel1_2_2;
    QLabel* textLabel1_2_2_2_2;
    QLineEdit* zp_maxphoterrorLineEdit;
    QComboBox* zp_magtypeComboBox;
    QLineEdit* zp_saturationLineEdit;
    QLabel* textLabel1_2;
    QLineEdit* zp_aperturesizeLineEdit;
    QLabel* textLabel1_2_2_2;
    QComboBox* zp_refcatComboBox;
    QComboBox* zp_serverComboBox;
    QComboBox* zp_filterComboBox;

public slots:
    virtual void validate();
    virtual void get_zp();
    virtual void show_hide();
    virtual void select_image();
    virtual void update_filter_comboboxes();
    virtual void fill_in_zp();
    virtual void kill_process();
    virtual int scan_for_errors();
    virtual void quit();

protected:
    QGridLayout* zpLayout;
    QSpacerItem* spacer42;
    QSpacerItem* spacer40;
    QGridLayout* groupBox20Layout;
    QSpacerItem* spacer37;
    QSpacerItem* spacer39;
    QSpacerItem* spacer13_4;
    QSpacerItem* spacer17;
    QSpacerItem* spacer13_2;
    QSpacerItem* spacer13_3;
    QSpacerItem* spacer13;
    QSpacerItem* spacer41;
    QGridLayout* layout3;
    QSpacerItem* spacer38;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // ZP_H
