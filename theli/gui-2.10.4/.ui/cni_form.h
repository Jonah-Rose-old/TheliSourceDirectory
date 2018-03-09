/****************************************************************************
** Form interface generated from reading ui file 'cni_form.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CNI_FORM_H
#define CNI_FORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTabWidget;
class QWidget;
class QFrame;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSpinBox;
class QComboBox;
class QButtonGroup;
class QRadioButton;
class QToolButton;
class QCheckBox;
class QTable;

class cni_form : public QDialog
{
    Q_OBJECT

public:
    cni_form( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~cni_form();

    QPushButton* cni_quitPushButton;
    QTabWidget* cni_mainTabWidget;
    QWidget* tab;
    QFrame* frame4;
    QPushButton* cni_savePushButton;
    QPushButton* cni_clearPushButton;
    QPushButton* cni_loadPushButton;
    QPushButton* cni_guessPushButton;
    QGroupBox* cni_fillexampleGroupBox;
    QPushButton* cni_st10PushButton;
    QPushButton* cni_wfcPushButton;
    QPushButton* cni_wfiPushButton;
    QPushButton* cni_presentinstPushButton;
    QGroupBox* cni_ccdgeomGroupBox;
    QLabel* cni_textLabel8;
    QLineEdit* cni_crpix2numchkLineEdit;
    QLineEdit* cni_ysizenumchkLineEdit;
    QLineEdit* cni_cutonynumchkLineEdit;
    QLineEdit* cni_ovmaxnumchkLineEdit;
    QLineEdit* cni_crpix1numchkLineEdit;
    QLineEdit* cni_xsizenumchkLineEdit;
    QLineEdit* cni_cutonxnumchkLineEdit;
    QLabel* cni_ovmaxTextLabel;
    QLabel* cni_cutonxTextLabel;
    QLabel* cni_cutonyTextLabel;
    QLabel* cni_xsizeTextLabel;
    QLabel* cni_ysizeTextLabel;
    QLabel* cni_crpix1TextLabel;
    QLabel* cni_crpix2TextLabel;
    QLineEdit* cni_ysizeLineEdit;
    QLineEdit* cni_crpix2LineEdit;
    QLineEdit* cni_cutonyLineEdit;
    QLineEdit* cni_ovmaxLineEdit;
    QLineEdit* cni_cutonxLineEdit;
    QLineEdit* cni_xsizeLineEdit;
    QLineEdit* cni_crpix1LineEdit;
    QLabel* cni_ovminTextLabel;
    QLineEdit* cni_ovminnumchkLineEdit;
    QLineEdit* cni_ovminLineEdit;
    QGroupBox* cni_genpropGroupBox;
    QLabel* cni_instnameTextLabel;
    QLabel* cni_nccdTextLabel;
    QLabel* cni_geoglTextLabel;
    QLabel* cni_gainTextLabel;
    QLabel* textLabel1_2;
    QLineEdit* cni_pixsclLineEdit;
    QLineEdit* cni_geogLineEdit;
    QSpinBox* cni_nccdSpinBox;
    QLineEdit* cni_instnameLineEdit;
    QComboBox* cni_cameratypeComboBox;
    QWidget* tab_2;
    QGroupBox* ssd_GroupBox1;
    QLabel* textLabel5;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLabel* textLabel6;
    QLabel* textLabel7;
    QLabel* textLabel8;
    QLabel* textLabel9;
    QLabel* textLabel10;
    QLabel* textLabel4;
    QLabel* textLabel11;
    QLabel* textLabel4_2;
    QComboBox* ssd_raComboBox;
    QComboBox* ssd_decComboBox;
    QLineEdit* ssd_rakeyLineEdit;
    QLineEdit* ssd_deckeyLineEdit;
    QLineEdit* ssd_lstkeyLineEdit;
    QLineEdit* ssd_mjdkeyLineEdit;
    QLineEdit* ssd_exptimekeyLineEdit;
    QLineEdit* ssd_airmasskeyLineEdit;
    QLineEdit* ssd_filterkeyLineEdit;
    QLineEdit* ssd_equinoxLineEdit;
    QComboBox* ssd_lstComboBox;
    QLineEdit* ssd_instrumentLineEdit;
    QPushButton* ssd_createscriptPushButton;
    QPushButton* ssd_changeinstrumentPushButton;
    QGroupBox* ssd_GroupBox2_2;
    QButtonGroup* buttonGroup3;
    QRadioButton* monochrome_FITS_RadioButton;
    QRadioButton* bayer_FITS_RadioButton;
    QRadioButton* bayer_RAW_RadioButton;
    QRadioButton* monochrome_RAW_RadioButton;
    QFrame* bayerframe;
    QButtonGroup* bayer_ButtonGroup;
    QToolButton* bayer_GBRG_ToolButton;
    QToolButton* bayer_BGGR_ToolButton;
    QToolButton* bayer_GRBG_ToolButton;
    QToolButton* bayer_RGGB_ToolButton;
    QLabel* selectBayerPatternLabel;
    QGroupBox* groupBox26;
    QCheckBox* ssd_preserveCDCheckBox;
    QLineEdit* ssd_numccdsLineEdit;
    QLabel* textLabel1;
    QLineEdit* ssd_transformationcodesLineEdit;
    QLabel* textLabel2_3;
    QTable* ssd_transformationTable;
    QGroupBox* ssd_GroupBox2;
    QLabel* textLabel3_2;
    QLineEdit* ssd_dummy1LineEdit;
    QLineEdit* ssd_dummy2LineEdit;
    QLineEdit* ssd_dummy3LineEdit;
    QLineEdit* ssd_dummy4LineEdit;
    QLineEdit* ssd_dummy5LineEdit;
    QLineEdit* ssd_dummy6LineEdit;
    QLineEdit* ssd_dummy7LineEdit;
    QLineEdit* ssd_rawdummy1LineEdit;
    QLineEdit* ssd_rawdummy2LineEdit;
    QLineEdit* ssd_rawdummy3LineEdit;
    QLineEdit* ssd_rawdummy4LineEdit;
    QLineEdit* ssd_rawdummy5LineEdit;
    QLineEdit* ssd_rawdummy6LineEdit;
    QLineEdit* ssd_rawdummy7LineEdit;
    QLineEdit* ssd_rawdummy8LineEdit;
    QLabel* textLabel2_2;
    QLineEdit* ssd_dummy8LineEdit;

public slots:
    virtual void populate_bayerbuttons();
    virtual void untoggle_bayerbuttons();
    virtual void active_passive_bayer();
    virtual void showhide_bayerpattern();
    virtual void showhide_bayerframe();
    virtual void validate();
    virtual void clearfields();
    virtual void count_ccds_entered();
    virtual void fill_in_presentinst();
    virtual void fill_in_WFI_example();
    virtual void fill_in_WFC_example();
    virtual void fill_in_ST10_example();
    virtual void read_conffile( QString conffile );
    virtual void get_inst_parameter(QString, QString, QString *);
    virtual void get_fileparameter( QString file, QString key );
    virtual void get_ccdnumber( QString ccdgeometry, QString & numccd );
    virtual void compare_nchips_nccd();
    virtual void load_instconfig();
    virtual void save_instconfig();
    virtual void timerSavebuttonDone();
    virtual void save_instconfig_helper( FILE * of, QLineEdit * le, QString lead );
    virtual void reset_nccdspinbox();
    virtual void ssd_set_instrumentname();
    virtual void ssd_paint_instrumentlineedit();
    virtual void ssd_instfiledialog();
    virtual void ssd_verify_input();
    virtual void ssd_countccds();
    virtual void write_splitscript();
    virtual void timerDone();
    virtual void insert_replacekey( FILE * output, QString theliheader, QString rawdummy, QString dummykey );
    virtual void guess_camerageometry();

protected:
    QGridLayout* cni_formLayout;
    QSpacerItem* spacer22;
    QGridLayout* tabLayout;
    QSpacerItem* spacer16_2;
    QSpacerItem* spacer17;
    QSpacerItem* spacer14;
    QSpacerItem* spacer16;
    QGridLayout* frame4Layout;
    QGridLayout* cni_fillexampleGroupBoxLayout;
    QGridLayout* cni_ccdgeomGroupBoxLayout;
    QSpacerItem* spacer10;
    QGridLayout* cni_genpropGroupBoxLayout;
    QSpacerItem* spacer13_2;
    QSpacerItem* spacer1_3;
    QSpacerItem* spacer1;
    QSpacerItem* spacer18;
    QSpacerItem* spacer14_2;
    QSpacerItem* spacer14_2_3;
    QSpacerItem* spacer1_4;
    QSpacerItem* spacer1_4_2;
    QSpacerItem* spacer14_2_2;
    QSpacerItem* spacer14_2_2_2;
    QGridLayout* tabLayout_2;
    QSpacerItem* spacer17_2;
    QSpacerItem* spacer17_2_2;
    QSpacerItem* spacer86;
    QGridLayout* ssd_GroupBox2_2Layout;
    QGridLayout* buttonGroup3Layout;
    QGridLayout* bayerframeLayout;
    QSpacerItem* spacer20;
    QGridLayout* bayer_ButtonGroupLayout;
    QGridLayout* groupBox26Layout;
    QSpacerItem* spacer22_2;
    QGridLayout* layout3;
    QSpacerItem* spacer21;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // CNI_FORM_H
