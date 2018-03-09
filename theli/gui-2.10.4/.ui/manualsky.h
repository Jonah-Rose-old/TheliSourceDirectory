/****************************************************************************
** Form interface generated from reading ui file 'manualsky.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MANUALSKY_H
#define MANUALSKY_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QButtonGroup;
class QRadioButton;
class QFrame;
class QLabel;
class QLineEdit;
class QTable;
class QComboBox;
class QGroupBox;
class QCheckBox;

class manualsky : public QDialog
{
    Q_OBJECT

public:
    manualsky( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~manualsky();

    QPushButton* ms_defaultsPushButton;
    QPushButton* ms_cancelPushButton;
    QPushButton* ms_closePushButton;
    QButtonGroup* ms_buttonGroup;
    QRadioButton* ms_modelskyRadioButton;
    QRadioButton* ms_manualskyRadioButton;
    QFrame* ms_frame2;
    QLabel* textLabel1_3;
    QLabel* textLabel3;
    QLabel* textLabel4_2_3;
    QLabel* textLabel4_2;
    QLabel* textLabel4_2_2;
    QLineEdit* ms_dec2LineEdit;
    QLineEdit* ms_dec1LineEdit;
    QLineEdit* ms_ra2LineEdit;
    QLineEdit* ms_ra1LineEdit;
    QLineEdit* ms_xmaxLineEdit;
    QLineEdit* ms_yminLineEdit;
    QLineEdit* ms_ymaxLineEdit;
    QLineEdit* ms_xminLineEdit;
    QLabel* textLabel4_4;
    QLabel* textLabel4_5;
    QLabel* textLabel4_3;
    QLabel* textLabel4;
    QLabel* textLabel5;
    QFrame* ms_frame3;
    QTable* manualsky_table;
    QPushButton* ms_fillPushButton;
    QComboBox* ms_showchipComboBox;
    QComboBox* ms_ccdselectionComboBox;
    QPushButton* ms_getvaluesPushButton;
    QPushButton* ms_loadPushButton;
    QPushButton* ms_saveasPushButton;
    QPushButton* ms_clearPushButton;
    QComboBox* ms_showmethodComboBox;
    QLineEdit* ms_fillLineEdit;
    QLabel* textLabel2;
    QGroupBox* ms_1GroupBox;
    QComboBox* ms_chipComboBox;
    QLabel* textLabel1;
    QComboBox* ms_methodComboBox;
    QPushButton* ms_infoPushButton;
    QButtonGroup* ms_modelButtonGroup;
    QLineEdit* ms_skysubdetectminareaLineEdit;
    QLineEdit* ms_skysubdetectthreshLineEdit;
    QLabel* cc_textLabel10_4;
    QLabel* cc_textLabel10_3_2;
    QLabel* cc_textLabel10_2_2;
    QCheckBox* ms_saveskymodelCheckBox;
    QLabel* textLabel1_17_3;
    QLineEdit* ms_skysubbacksizeLineEdit;
    QLineEdit* ms_maskexpandfactorLineEdit;

public slots:
    virtual void validate();
    virtual void clearfields();
    virtual void showhide_rows();
    virtual void update_table();
    virtual void get_manualsky();
    virtual void load_manualsky();
    virtual void fill_manualvalue();
    virtual void save_quit();
    virtual void save_fullstatistics();
    virtual void save_wysiwyg();
    virtual void populate_manualskytable();
    virtual void write_parameters();
    virtual void read_parameters();
    virtual void modify_parametersini();
    virtual void defaults();
    virtual void showhide_ccdselection();
    virtual void switch_selection();
    virtual void show_info();

protected:
    QGridLayout* manualskyLayout;
    QSpacerItem* spacer17;
    QSpacerItem* spacer190;
    QHBoxLayout* layout10;
    QSpacerItem* spacer180;
    QGridLayout* ms_buttonGroupLayout;
    QGridLayout* ms_frame2Layout;
    QSpacerItem* spacer20;
    QGridLayout* ms_frame3Layout;
    QGridLayout* layout7;
    QGridLayout* layout6;
    QSpacerItem* spacer181_5;
    QSpacerItem* spacer181_4;
    QSpacerItem* spacer181;
    QSpacerItem* spacer181_2;
    QSpacerItem* spacer181_3;
    QHBoxLayout* layout5;
    QSpacerItem* spacer14;
    QGridLayout* ms_1GroupBoxLayout;
    QGridLayout* ms_modelButtonGroupLayout;
    QSpacerItem* spacer34;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // MANUALSKY_H
