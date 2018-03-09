/****************************************************************************
** Form interface generated from reading ui file 'color_form.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef COLOR_FORM_H
#define COLOR_FORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTabWidget;
class QWidget;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListBox;
class QListBoxItem;
class QCheckBox;
class QComboBox;
class QButtonGroup;
class QTable;

class color_form : public QDialog
{
    Q_OBJECT

public:
    color_form( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~color_form();

    QPushButton* color_closePushButton;
    QTabWidget* color_TabWidget;
    QWidget* tab;
    QGroupBox* groupBox5;
    QLabel* textLabel10;
    QLineEdit* color_xmaxLineEdit;
    QLineEdit* color_xminLineEdit;
    QLabel* textLabel9;
    QLabel* textLabel9_4;
    QLineEdit* color_ymaxLineEdit;
    QLineEdit* color_yminLineEdit;
    QListBox* color_imagesListBox;
    QPushButton* color_removeimagePushButton;
    QListBox* color_coaddListBox;
    QPushButton* color_rereadimagePushButton;
    QLabel* textLabel1_5;
    QLabel* textLabel1;
    QPushButton* color_getcoaddedimagesPushButton;
    QPushButton* color_selectdirPushButton;
    QLineEdit* color_dirLineEdit;
    QWidget* TabPage;
    QGroupBox* groupBox5_2;
    QLabel* textLabel2_2;
    QLabel* textLabel1_9;
    QListBox* color_ELum_imagesListBox;
    QLabel* textLabel1_4;
    QPushButton* color_ELum_doitPushButton;
    QCheckBox* color_ELum_maskCheckBox;
    QComboBox* color_ELum_modeComboBox;
    QGroupBox* combine_GroupBox;
    QLabel* textLabel1_7;
    QLabel* textLabel2_3;
    QLabel* textLabel1_9_3;
    QLineEdit* color_bbnb_weightLineEdit;
    QLabel* textLabel4;
    QPushButton* color_bbnb_combinePushButton;
    QComboBox* color_bbnb_broadComboBox;
    QComboBox* color_bbnb_narrowComboBox;
    QLineEdit* color_bbnb_narrowwidthLineEdit;
    QLineEdit* color_bbnb_broadwidthLineEdit;
    QLabel* textLabel3_2;
    QWidget* tab_2;
    QGroupBox* groupBox3;
    QComboBox* color_BV_r_ComboBox;
    QComboBox* color_BV_b_ComboBox;
    QLabel* textLabel1_2_3;
    QLabel* textLabel1_2_2;
    QLabel* textLabel1_2;
    QComboBox* color_BV_v_ComboBox;
    QButtonGroup* color_resultButtonGroup;
    QPushButton* color_SDSSPushButton;
    QPushButton* color_UCAC4PushButton;
    QLabel* color_nstarsSDSSTextLabel;
    QLabel* color_nstarsNOMADTextLabel;
    QLabel* color_nstarsAVGWHITETextLabel;
    QPushButton* color_NOMADPushButton;
    QPushButton* color_AVGWHITEPushButton;
    QLabel* color_nstarsUCAC4TextLabel;
    QGroupBox* groupBox7;
    QLineEdit* color_errorGLineEdit;
    QLineEdit* color_factorGLineEdit;
    QLineEdit* color_factorRLineEdit;
    QLabel* textLabel2;
    QLabel* textLabel3_3;
    QLineEdit* color_errorRLineEdit;
    QLineEdit* color_errorBLineEdit;
    QLabel* textLabel4_2;
    QLineEdit* color_factorBLineEdit;
    QLabel* textLabel3;
    QLabel* textLabel1_3;
    QPushButton* color_stiffpreviewPushButton;
    QLabel* color_serverTextLabel;
    QComboBox* color_serverComboBox;
    QPushButton* color_getBVcalPushButton;
    QPushButton* color_BV_resetPushButton;
    QWidget* TabPage_2;
    QGroupBox* groupBox15;
    QLabel* textLabel5;
    QLabel* textLabel4_3;
    QLabel* textLabel3_4;
    QLabel* textLabel2_4;
    QLabel* textLabel1_10;
    QComboBox* color_stiffRGBLUMComboBox;
    QPushButton* color_stiffPushButton;
    QPushButton* color_stiffdefaultsPushButton;
    QLineEdit* color_stiffsatvalLineEdit;
    QLineEdit* color_stiffgammaLineEdit;
    QLineEdit* color_stiffminvalLineEdit;
    QLineEdit* color_stiffmaxvalLineEdit;
    QGroupBox* groupBox6;
    QPushButton* color_fits2tiffPushButton;
    QLabel* textLabel1_6;
    QTable* color_statisticsTable;
    QPushButton* color_getstatisticsPushButton;
    QLabel* textLabel5_2;

public slots:
    virtual void select_dir();
    virtual void get_coadddirs();
    virtual void link_coaddimages();
    virtual void get_colorchannels();
    virtual void validator_main();
    virtual void validator_sec();
    virtual void remove_imagefromlist();
    virtual void flush_table();
    virtual void get_statistics();
    virtual void update_tiffthresholds( int numimages );
    virtual void stiff_defaults();
    virtual void enhance_luminance();
    virtual void fill_in_elum();
    virtual void get_current_imagelist();
    virtual void populate_stiffcombobox();
    virtual void write_superscriptbody(QString, QString, QString);
    virtual void update_cropText();
    virtual void update_combineText();
    virtual void update_colcalibText();
    virtual void update_stiffText();
    virtual void update_fits2tiffText();
    virtual void update_elumText();
    virtual void update_combine_elements();
    virtual void combine_images();
    virtual void show_preview();
    virtual void reset_BVcorr();
    virtual void stiff();
    virtual void get_color_correction();
    virtual void fill_in_colcalib();
    virtual void read_into_struct(QString, int);
    virtual void set_toggle_button();
    virtual void fits2tiff();
    virtual void get_maxoverlap();
    virtual void update_colordirpath();

protected:
    QGridLayout* color_formLayout;
    QHBoxLayout* layout39;
    QSpacerItem* spacer62;
    QGridLayout* tabLayout;
    QSpacerItem* spacer47;
    QSpacerItem* spacer29_2;
    QGridLayout* groupBox5Layout;
    QSpacerItem* spacer5;
    QSpacerItem* spacer9;
    QSpacerItem* spacer8;
    QSpacerItem* spacer44;
    QSpacerItem* spacer63;
    QGridLayout* layout32;
    QGridLayout* TabPageLayout;
    QSpacerItem* spacer34;
    QSpacerItem* spacer41_3;
    QGridLayout* groupBox5_2Layout;
    QSpacerItem* spacer44_2;
    QGridLayout* layout5;
    QSpacerItem* spacer39;
    QSpacerItem* spacer40;
    QSpacerItem* spacer41;
    QGridLayout* combine_GroupBoxLayout;
    QSpacerItem* spacer41_2;
    QSpacerItem* spacer32;
    QSpacerItem* spacer32_3;
    QSpacerItem* spacer32_2;
    QGridLayout* tabLayout_2;
    QSpacerItem* spacer37_2;
    QSpacerItem* spacer57;
    QGridLayout* groupBox3Layout;
    QSpacerItem* spacer51;
    QSpacerItem* spacer52;
    QSpacerItem* spacer56;
    QGridLayout* layout34;
    QGridLayout* color_resultButtonGroupLayout;
    QGridLayout* groupBox7Layout;
    QHBoxLayout* layout6;
    QGridLayout* TabPageLayout_2;
    QSpacerItem* spacer39_2;
    QSpacerItem* spacer61;
    QSpacerItem* spacer61_2;
    QGridLayout* groupBox15Layout;
    QSpacerItem* spacer83;
    QGridLayout* groupBox6Layout;
    QSpacerItem* spacer27_3;
    QSpacerItem* spacer27_2;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // COLOR_FORM_H
