/****************************************************************************
** Form interface generated from reading ui file 'parameters.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QWidgetStack;
class QWidget;
class QGroupBox;
class QLineEdit;
class QCheckBox;
class QButtonGroup;
class QToolButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QFrame;

class parameters : public QDialog
{
    Q_OBJECT

public:
    parameters( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~parameters();

    QLabel* param_head2TextLabel;
    QLabel* param_head1TextLabel;
    QPushButton* param_defaultsPushButton;
    QPushButton* param_nextPushButton;
    QPushButton* param_okPushButton;
    QPushButton* param_cancelPushButton;
    QPushButton* param_backPushButton;
    QPushButton* param_defaultsallPushButton;
    QWidgetStack* param_WidgetStack;
    QWidget* PreparationStackPage;
    QLabel* textLabel2_7;
    QGroupBox* groupBox11;
    QLineEdit* pre_sort_biasLineEdit;
    QLabel* textLabel11_2;
    QLineEdit* pre_sort_darkLineEdit;
    QLineEdit* pre_sort_domeflatLineEdit;
    QLineEdit* pre_sort_skyflatLineEdit;
    QLineEdit* pre_sort_stdLineEdit;
    QLineEdit* pre_sort_fitskeyLineEdit;
    QLabel* textLabel10_3;
    QLabel* textLabel13_3;
    QLabel* textLabel12_3;
    QLabel* textLabel9_2;
    QLabel* textLabel8_2;
    QLabel* textLabel2;
    QGroupBox* groupBox1;
    QLabel* textLabel3;
    QCheckBox* xtalk_norCheckBox;
    QLineEdit* xtalk_nor_amplitudeLineEdit;
    QButtonGroup* xtalk_norButtonGroup;
    QToolButton* xtalk_nor_2x2ToolButton;
    QToolButton* xtalk_nor_1x2ToolButton;
    QToolButton* xtalk_nor_2x1ToolButton;
    QLabel* textLabel3_2;
    QLineEdit* xtalk_row_amplitudeLineEdit;
    QButtonGroup* xtalk_rowButtonGroup;
    QToolButton* xtalk_row_2x2ToolButton;
    QToolButton* xtalk_row_1x2ToolButton;
    QToolButton* xtalk_row_2x1ToolButton;
    QToolButton* xtalk_col_2x2ToolButton;
    QToolButton* xtalk_col_1x2ToolButton;
    QToolButton* xtalk_col_2x1ToolButton;
    QCheckBox* xtalk_rowCheckBox;
    QButtonGroup* xtalk_multiButtonGroup;
    QToolButton* xtalk_multixToolButton;
    QToolButton* xtalk_multiyToolButton;
    QCheckBox* xtalk_multiCheckBox;
    QLabel* textLabel1_13;
    QLineEdit* xtalk_multi_nsectionLineEdit;
    QGroupBox* groupBox12;
    QLineEdit* pre_rename_fitskeyLineEdit;
    QCheckBox* pre_renameCheckBox;
    QWidget* CalibrationStackPage;
    QLabel* textLabel4;
    QGroupBox* groupBox3;
    QLineEdit* ovscan_nhighLineEdit;
    QLineEdit* ovscan_nlowLineEdit;
    QLabel* textLabel9;
    QLabel* textLabel5;
    QLabel* textLabel7;
    QLineEdit* flat_nlowLineEdit;
    QLineEdit* flat_nhighLineEdit;
    QLineEdit* bias_nlowLineEdit;
    QLineEdit* bias_nhighLineEdit;
    QLabel* textLabel6;
    QLabel* textLabel6_4;
    QLineEdit* dark_nlowLineEdit;
    QLineEdit* dark_nhighLineEdit;
    QLabel* textLabel10;
    QWidget* BackgroundStackPage;
    QLabel* textLabel2_2;
    QGroupBox* groupBox4;
    QLabel* textLabel2_11_3_2;
    QLabel* sf_back_static_dynamicTextlabel;
    QLabel* cal_windowsizeTextLabel_2;
    QLabel* textLabel1_17_2_4;
    QLabel* textLabel2_13_2_2;
    QLabel* textLabel2_13_3_4;
    QLineEdit* sf_back_nhigh2LineEdit;
    QLineEdit* sf_back_nlow1LineEdit;
    QLineEdit* sf_back_nhigh1LineEdit;
    QLabel* sf_back_twopass2TextLabel;
    QLabel* sf_back_twopass1TextLabel;
    QLineEdit* sf_back_nlow2LineEdit;
    QLabel* textLabel2_13_3_2;
    QLineEdit* sf_back_maglimitLineEdit;
    QLabel* textLabel2_13_3_2_3;
    QLineEdit* sf_back_distanceLineEdit;
    QLabel* textLabel2_13_3_2_2;
    QComboBox* sf_back_serverComboBox;
    QLabel* textLabel1_17_2_3;
    QLabel* cal_windowsizeTextLabel;
    QLineEdit* sf_back_gapsizeLineEdit;
    QLabel* textLabel1_17_2_2;
    QLineEdit* sf_back_windowsizeLineEdit;
    QCheckBox* sf_back_twopassCheckBox;
    QCheckBox* sf_back_sexfilterCheckBox;
    QLineEdit* sf_back_detectminareaLineEdit;
    QLineEdit* sf_back_detectthreshLineEdit;
    QLabel* textLabel1_17;
    QLabel* textLabel1_17_4;
    QLabel* cal_detectthreshTextLabel;
    QLabel* cal_detectminareaTextLabel;
    QLineEdit* sf_back_maskexpandfactorLineEdit;
    QComboBox* sf_back_combinemethodComboBox;
    QLabel* textLabel1_17_2;
    QComboBox* sf_back_applymodeComboBox;
    QLineEdit* sf_back_fringesmoothLineEdit;
    QCheckBox* sf_back_adjustgainsCheckBox;
    QCheckBox* sf_back_fringescaleCheckBox;
    QLineEdit* sf_back_illumsmoothLineEdit;
    QLabel* sf_textLabel1;
    QLabel* sf_textLabel1_2;
    QWidget* Background1StackPage;
    QLabel* textLabel2_2_2;
    QGroupBox* collapseregion_groupbox;
    QLineEdit* ccc_collymaxLineEdit;
    QLineEdit* ccc_collxmaxLineEdit;
    QLabel* textLabel4_2;
    QLabel* textLabel5_2;
    QLabel* textLabel6_2;
    QLineEdit* ccc_collxminLineEdit;
    QLineEdit* ccc_collyminLineEdit;
    QLabel* ccc_excludePixmapLabel;
    QGroupBox* groupBox17;
    QLabel* textLabel3_3;
    QLabel* ccc_colldirPixmapLabel;
    QLabel* textLabel2_3;
    QLabel* textLabel1_16_3;
    QLineEdit* ccc_collrejectthreshLineEdit;
    QComboBox* ccc_colldirComboBox;
    QGroupBox* groupBox23;
    QLabel* textLabel1_2;
    QLineEdit* ccc_colldetectminareaLineEdit;
    QLabel* textLabel2_11;
    QCheckBox* ccc_collautothresholdCheckBox;
    QLabel* textLabel1_17_3;
    QLineEdit* ccc_colldetectthreshLineEdit;
    QCheckBox* ccc_maskactionCheckBox;
    QLineEdit* ccc_collmaskexpandfactorLineEdit;
    QWidget* Weights1StackPage;
    QLabel* textLabel2_2_2_2_3;
    QGroupBox* groupBox30;
    QLabel* textLabel1_8;
    QSpinBox* w_binsizeSpinBox;
    QLineEdit* w_binmaxLineEdit;
    QLineEdit* w_binminLineEdit;
    QLabel* textLabel3_7;
    QLabel* textLabel2_9;
    QCheckBox* w_binoutlierCheckBox;
    QGroupBox* defectdetectionGroupBox;
    QLabel* textLabel1_5;
    QLabel* textLabel2_6;
    QLabel* textLabel3_6;
    QLabel* textLabel4_4;
    QLabel* textLabel5_5;
    QLineEdit* defect_clustol_sfLineEdit;
    QLineEdit* defect_kernelsize_sfLineEdit;
    QLineEdit* defect_coltolLineEdit;
    QLineEdit* defect_rowtol_sfLineEdit;
    QLabel* textLabel1_15;
    QLineEdit* defect_clustolLineEdit;
    QLineEdit* defect_coltol_sfLineEdit;
    QLineEdit* defect_kernelsizeLineEdit;
    QLineEdit* defect_rowtolLineEdit;
    QLabel* textLabel2_12;
    QLabel* textLabel2_2_2_2;
    QGroupBox* groupBox24;
    QCheckBox* globw_uniformweightCheckBox;
    QLabel* textLabel14;
    QLabel* textLabel13;
    QLineEdit* globw_darkmaxLineEdit;
    QLineEdit* globw_darkminLineEdit;
    QLineEdit* globw_flatmaxLineEdit;
    QLabel* textLabel12;
    QLineEdit* globw_flatminLineEdit;
    QLabel* textLabel15;
    QCheckBox* globw_usebiasCheckBox;
    QWidget* Weights2StackPage;
    QLabel* textLabel2_2_2_3;
    QGroupBox* groupBox25;
    QLabel* textLabel17;
    QLabel* textLabel17_2;
    QLabel* textLabel1_11;
    QLabel* textLabel1_11_2;
    QLabel* textLabel18;
    QLineEdit* weight_cosmicLineEdit;
    QLineEdit* weight_cosmicDMINLineEdit;
    QLineEdit* weight_cosmicDTLineEdit;
    QLineEdit* weight_threshmaxLineEdit;
    QLineEdit* weight_threshminLineEdit;
    QLabel* weight_bloom2TextLabel;
    QCheckBox* weight_bloomCheckBox;
    QLabel* weight_bloom3TextLabel;
    QLabel* weight_bloom1TextLabel;
    QLineEdit* weight_bloomlowlimitLineEdit;
    QLineEdit* weight_bloomwidthLineEdit;
    QLineEdit* weight_bloomminareaLineEdit;
    QWidget* Ap1StackPage;
    QLabel* textLabel2_2_2_3_2_2;
    QGroupBox* groupBox26_2_2;
    QLabel* textLabel1_10;
    QComboBox* absphot_indirect_stdcatComboBox;
    QGroupBox* groupBox14;
    QCheckBox* absphot_wcserrorCheckBox;
    QComboBox* absphot_stdfilterComboBox;
    QComboBox* absphot_stdcolorComboBox;
    QLineEdit* absphot_apertureLineEdit;
    QLineEdit* absphot_extinctionLineEdit;
    QLineEdit* absphot_colorfixedLineEdit;
    QLineEdit* absphot_zpminLineEdit;
    QLineEdit* absphot_zpmaxLineEdit;
    QLineEdit* absphot_kappaLineEdit;
    QLineEdit* absphot_thresholdLineEdit;
    QLineEdit* absphot_stdminmagLineEdit;
    QComboBox* absphot_calmodeComboBox;
    QLineEdit* absphot_maxiterLineEdit;
    QPushButton* absphot_wcserrorPushButton;
    QLabel* textLabel1_12;
    QLabel* textLabel19_2_2_2_2;
    QLabel* textLabel21_2_2_2;
    QLabel* textLabel21_2_3;
    QLabel* textLabel20_2_2;
    QLabel* textLabel20_2_3_2;
    QLabel* textLabel20_2_3_2_2;
    QLabel* textLabel20_2_3_2_2_2_3_2;
    QLabel* textLabel20_2_3_2_2_2_3;
    QLabel* textLabel20_2_3_2_2_2_3_2_2;
    QLabel* textLabel20_2_3_2_2_2;
    QLineEdit* absphot_convergenceLineEdit;
    QLineEdit* absphot_wcserrorLineEdit;
    QLabel* textLabel1_16;
    QLineEdit* absphot_filterLineEdit;
    QLabel* textLabel19_2_2_2;
    QLabel* textLabel20_2_3_2_2_2_2;
    QWidget* Ap2StackPage;
    QLabel* textLabel2_2_2_3_2_2_2;
    QGroupBox* groupBox26_2_2_2;
    QLabel* textLabel1_10_2;
    QComboBox* absphot_direct_stdcatComboBox;
    QGroupBox* groupBox14_2;
    QLabel* textLabel3_9_2;
    QLabel* textLabel21_2_2_2_2_2;
    QLabel* textLabel4_5_2;
    QComboBox* absphot_fittingmethodM2_ComboBox;
    QComboBox* absphot_stdfilterM2_ComboBox;
    QLineEdit* absphot_maxphoterrorLineEdit;
    QLabel* textLabel5_6_3;
    QLabel* textLabel2_11_2;
    QWidget* Ap3StackPage;
    QLabel* textLabel2_2_2_3_2;
    QGroupBox* groupBox26_2;
    QLabel* textLabel19_2;
    QLabel* textLabel21_2;
    QLineEdit* ap_deblendLineEdit;
    QLineEdit* ap_detectminareaLineEdit;
    QLineEdit* ap_detectthreshLineEdit;
    QLabel* textLabel19_2_2;
    QLabel* textLabel21_2_2;
    QLabel* textLabel20_2;
    QLineEdit* ap_flagLineEdit;
    QLineEdit* ap_minfwhmLineEdit;
    QLabel* textLabel20_2_3;
    QLineEdit* ap_sexbacklevelLineEdit;
    QLabel* textLabel20_2_3_3;
    QLineEdit* ap_lownumLineEdit;
    QLineEdit* ap_saturationLineEdit;
    QLabel* textLabel20_2_3_3_2;
    QCheckBox* ap_filterCheckBox;
    QWidget* Ap4StackPage;
    QLabel* textLabel2_2_2_2_2;
    QTabWidget* astrommethod_TabWidget;
    QWidget* tab;
    QGroupBox* groupBox16;
    QLabel* textLabel1_3;
    QLabel* textLabel2_4;
    QLabel* textLabel1_7;
    QLabel* textLabel4_3;
    QLabel* textLabel6_3;
    QComboBox* ac_sc_stabilityComboBox;
    QLabel* textLabel2_5;
    QLabel* textLabel1;
    QLabel* textLabel3_4;
    QComboBox* ac_sc_mosaictypeComboBox;
    QPushButton* ac_sc_mtypereadmePushButton;
    QComboBox* ac_sc_FPComboBox;
    QLabel* textLabel2_5_2;
    QLabel* textLabel2_5_2_2;
    QLabel* textLabel2_5_3;
    QLabel* textLabel2_5_3_2;
    QLineEdit* ac_sc_astrinstrukeyLineEdit;
    QLineEdit* ac_sc_photinstrukeyLineEdit;
    QLineEdit* ac_sc_astrefweightLineEdit;
    QLineEdit* ac_sc_crossidradiusLineEdit;
    QLineEdit* ac_sc_fgroupradiusLineEdit;
    QLineEdit* ac_sc_distortorderLineEdit;
    QLineEdit* ac_sc_maxscaleLineEdit;
    QLineEdit* ac_sc_maxoffsetLineEdit;
    QLineEdit* ac_sc_maxposangleLineEdit;
    QLabel* textLabel2_5_2_3;
    QLineEdit* ac_sc_distortkeysLineEdit;
    QLabel* textLabel4_3_2;
    QLineEdit* ac_sc_distortorder2LineEdit;
    QLineEdit* ac_sc_maxSNLineEdit;
    QLineEdit* ac_sc_minSNLineEdit;
    QLabel* textLabel5_3;
    QLineEdit* ac_sc_distortgroupsLineEdit;
    QCheckBox* ac_sc_matchflippedCheckBox;
    QCheckBox* ac_sc_matchWCSCheckBox;
    QWidget* tab_2;
    QGroupBox* groupBox16_2;
    QLabel* textLabel1_7_2;
    QLineEdit* ac_an_maxscaleLineEdit;
    QLineEdit* ac_an_distortorderLineEdit;
    QCheckBox* ac_an_scampdistortCheckBox;
    QLabel* textLabel4_3_3;
    QWidget* TabPage;
    QGroupBox* absphot_groupbox;
    QLabel* absphot_textlabel;
    QPushButton* NIGHT_5_PushButton;
    QPushButton* NIGHT_4_PushButton;
    QPushButton* NIGHT_3_PushButton;
    QPushButton* NIGHT_6_PushButton;
    QPushButton* NIGHT_2_PushButton;
    QPushButton* NIGHT_1_PushButton;
    QComboBox* NIGHT_1_ComboBox;
    QComboBox* NIGHT_2_ComboBox;
    QComboBox* NIGHT_3_ComboBox;
    QComboBox* NIGHT_4_ComboBox;
    QComboBox* NIGHT_5_ComboBox;
    QComboBox* NIGHT_6_ComboBox;
    QPushButton* NIGHT_10_PushButton;
    QComboBox* NIGHT_10_ComboBox;
    QPushButton* NIGHT_9_PushButton;
    QComboBox* NIGHT_9_ComboBox;
    QPushButton* NIGHT_8_PushButton;
    QComboBox* NIGHT_8_ComboBox;
    QComboBox* NIGHT_7_ComboBox;
    QPushButton* NIGHT_7_PushButton;
    QWidget* CoaddStackPage;
    QLabel* textLabel2_2_2_2_2_2;
    QFrame* frame4;
    QLabel* textLabel2_8;
    QLabel* coc_filterTextLabel;
    QLabel* textLabel1_2_2;
    QLabel* textLabel1_4;
    QLabel* textLabel3_5_2;
    QLineEdit* coc_skyposangleLineEdit;
    QComboBox* coc_speedunitComboBox;
    QLineEdit* coc_coaddsizeyLineEdit;
    QLineEdit* coc_coaddsizexLineEdit;
    QLineEdit* coc_propmotionraLineEdit;
    QLineEdit* coc_propmotiondecLineEdit;
    QComboBox* coc_filterComboBox;
    QPushButton* coc_skypafromimagePushButton;
    QLabel* textLabel3_5;
    QLineEdit* coc_smoothedgeLineEdit;
    QLineEdit* coc_pixscaleLineEdit;
    QLineEdit* coc_RZPLineEdit;
    QLabel* textLabel3_5_2_2;
    QLineEdit* coc_seeingLineEdit;
    QLineEdit* coc_chipsLineEdit;
    QLineEdit* coc_identLineEdit;
    QLineEdit* coc_refraLineEdit;
    QLabel* textLabel1_6;
    QLabel* coc_refraTextLabel;
    QLineEdit* coc_refdecLineEdit;
    QLabel* textLabel1_9;
    QLineEdit* coc_filterthresholdLineEdit;
    QLabel* textLabel1_14;
    QLabel* textLabel2_10;
    QLabel* textLabel5_4;
    QLineEdit* coc_filterborderwidthLineEdit;
    QLineEdit* coc_filterclustersizeLineEdit;
    QLabel* textLabel3_8;
    QFrame* frame4_2;
    QLabel* cc_projectionTextLabel;
    QLabel* cc_typeTextLabel;
    QLabel* textLabel1_4_2;
    QComboBox* coc_celestialtypeComboBox;
    QComboBox* coc_projectionComboBox;
    QComboBox* coc_combineComboBox;
    QComboBox* coc_kernelComboBox;
    QLabel* textLabel8_3;
    QLineEdit* coc_clipsigmaLineEdit;
    QLabel* coc_clipsigmaLabel;
    QLabel* coc_clipampfracLabel;
    QLineEdit* coc_clipampfracLineEdit;
    QCheckBox* coc_rescaleweightsCheckBox;

public slots:
    virtual void show_absphot_readme();
    virtual void show_hide_absphot_wcserror();
    virtual void show_hide_twopass();
    virtual void update_refcoord_textlabel();
    virtual void call_mtype_readme();
    virtual void update_headertextlabel( int page );
    virtual void switch_page_next();
    virtual void show_hide_filtercombobox();
    virtual void fill_coadd_filtercombobox();
    virtual void fill_skyposangle();
    virtual void switch_page_back();
    virtual void check_for_reset();
    virtual void populate_absphotmenu();
    virtual void show_photcal1();
    virtual void show_photcal2();
    virtual void show_photcal3();
    virtual void show_photcal4();
    virtual void show_photcal5();
    virtual void show_photcal6();
    virtual void show_photcal7();
    virtual void get_crossidradius( QString & radius );
    virtual void get_pixscale();
    virtual void get_numchips();
    virtual void update_crossidradius();
    virtual void check_distortion_consistency( int & answer );
    virtual void parameters_quit();
    virtual void update_stdcolor_comboboxes();
    virtual void check_stripe82();
    virtual void init_stdcolor_comboboxes();
    virtual void update_stdcolorfilter();
    virtual void check_nhigh_window( int & checkresult );
    virtual void toggle_statdyn();
    virtual void toggle_globalweight_params();
    virtual void write_parameters();
    virtual void read_parameters();
    virtual void modify_parametersini();
    virtual void get_coadd_id( QString & result );
    virtual void defaults();
    virtual void defaults_all();
    virtual void populate_xtalkbuttons();
    virtual void untoggle_xtalkbuttons();
    virtual void link_checkbox_xtalk();
    virtual void link_xtalk_checkbox();
    virtual void active_passive_xtalk();
    virtual void validate();
    virtual void adjust_collcorr_direction();
    virtual void showhide_bloom();
    virtual void showhide_clipped();
    virtual void showhide_defectdetection();
    virtual void showhide_cosmicsDTDMIN();
    virtual void check_defect_kernelsize();
    virtual void check_filterthreshold();

protected:
    QGridLayout* parametersLayout;
    QSpacerItem* spacer137_2;
    QGridLayout* layout27;
    QSpacerItem* spacer113;
    QSpacerItem* spacer112;
    QGridLayout* PreparationStackPageLayout;
    QSpacerItem* spacer7;
    QSpacerItem* spacer111;
    QGridLayout* groupBox11Layout;
    QGridLayout* layout2;
    QGridLayout* groupBox1Layout;
    QSpacerItem* spacer5;
    QSpacerItem* spacer2;
    QSpacerItem* spacer2_2_2;
    QSpacerItem* spacer2_2;
    QGridLayout* xtalk_rowButtonGroupLayout;
    QGridLayout* xtalk_multiButtonGroupLayout;
    QGridLayout* groupBox12Layout;
    QSpacerItem* spacer98;
    QGridLayout* CalibrationStackPageLayout;
    QSpacerItem* spacer60;
    QSpacerItem* spacer14;
    QSpacerItem* spacer58;
    QSpacerItem* spacer58_3;
    QGridLayout* groupBox3Layout;
    QSpacerItem* spacer10_3;
    QSpacerItem* spacer10;
    QSpacerItem* spacer10_2;
    QSpacerItem* spacer10_2_3;
    QGridLayout* BackgroundStackPageLayout;
    QSpacerItem* spacer101;
    QGridLayout* groupBox4Layout;
    QSpacerItem* spacer105_5_2;
    QSpacerItem* spacer105_5_2_2;
    QSpacerItem* spacer105_4;
    QSpacerItem* spacer103_2;
    QSpacerItem* spacer104_2;
    QGridLayout* layout11;
    QHBoxLayout* layout26;
    QGridLayout* layout16;
    QGridLayout* layout13;
    QGridLayout* Background1StackPageLayout;
    QSpacerItem* spacer85;
    QSpacerItem* spacer86;
    QSpacerItem* spacer84_2;
    QSpacerItem* spacer88_2;
    QSpacerItem* spacer84;
    QSpacerItem* spacer88;
    QGridLayout* collapseregion_groupboxLayout;
    QSpacerItem* spacer15_3;
    QSpacerItem* spacer15;
    QGridLayout* groupBox17Layout;
    QSpacerItem* spacer22;
    QSpacerItem* spacer15_5;
    QSpacerItem* spacer23;
    QSpacerItem* spacer15_6;
    QGridLayout* groupBox23Layout;
    QSpacerItem* spacer15_7;
    QGridLayout* Weights1StackPageLayout;
    QSpacerItem* spacer202;
    QSpacerItem* spacer94;
    QSpacerItem* spacer90;
    QGridLayout* groupBox30Layout;
    QSpacerItem* spacer201;
    QGridLayout* defectdetectionGroupBoxLayout;
    QGridLayout* groupBox24Layout;
    QSpacerItem* spacer98_2_2;
    QGridLayout* Weights2StackPageLayout;
    QSpacerItem* spacer95;
    QSpacerItem* spacer89;
    QGridLayout* groupBox25Layout;
    QSpacerItem* spacer95_3_2;
    QSpacerItem* spacer95_3_3;
    QSpacerItem* spacer95_3;
    QSpacerItem* spacer91_2;
    QSpacerItem* spacer97;
    QSpacerItem* spacer92_2;
    QSpacerItem* spacer91_3;
    QSpacerItem* spacer93_2;
    QGridLayout* Ap1StackPageLayout;
    QSpacerItem* spacer90_2;
    QSpacerItem* spacer91;
    QGridLayout* groupBox26_2_2Layout;
    QSpacerItem* spacer185;
    QGridLayout* groupBox14Layout;
    QSpacerItem* spacer104;
    QSpacerItem* spacer143_2_3_2_2_3_2;
    QGridLayout* Ap2StackPageLayout;
    QSpacerItem* spacer90_2_2;
    QSpacerItem* spacer91_4;
    QGridLayout* groupBox26_2_2_2Layout;
    QSpacerItem* spacer185_2;
    QGridLayout* groupBox14_2Layout;
    QSpacerItem* spacer142_2_2_2_2_2;
    QSpacerItem* spacer102_2;
    QSpacerItem* spacer105_2_2;
    QSpacerItem* spacer105_3;
    QGridLayout* Ap3StackPageLayout;
    QSpacerItem* spacer132;
    QSpacerItem* spacer131;
    QGridLayout* groupBox26_2Layout;
    QSpacerItem* spacer142_2;
    QSpacerItem* spacer141_2;
    QSpacerItem* spacer141_2_2;
    QSpacerItem* spacer142_2_2;
    QSpacerItem* spacer143_2;
    QSpacerItem* spacer143_2_3;
    QSpacerItem* spacer143_2_3_3;
    QGridLayout* Ap4StackPageLayout;
    QSpacerItem* spacer92_4;
    QSpacerItem* spacer137;
    QGridLayout* tabLayout;
    QGridLayout* groupBox16Layout;
    QSpacerItem* spacer3_6_3_2;
    QSpacerItem* spacer3_6_3;
    QSpacerItem* spacer3_6_2_2;
    QSpacerItem* spacer3_4;
    QSpacerItem* spacer2_3_2_2;
    QSpacerItem* spacer3_3;
    QSpacerItem* spacer3_2;
    QGridLayout* tabLayout_2;
    QSpacerItem* spacer103;
    QSpacerItem* spacer102;
    QGridLayout* groupBox16_2Layout;
    QSpacerItem* spacer3_4_2;
    QSpacerItem* spacer2_3_2_2_2;
    QGridLayout* TabPageLayout;
    QGridLayout* absphot_groupboxLayout;
    QSpacerItem* spacer90_3;
    QSpacerItem* spacer90_3_6;
    QSpacerItem* spacer90_3_5;
    QSpacerItem* spacer90_3_4;
    QSpacerItem* spacer90_3_3;
    QSpacerItem* spacer90_3_2;
    QSpacerItem* spacer90_3_7_4;
    QSpacerItem* spacer90_3_7_3;
    QSpacerItem* spacer90_3_7_2;
    QSpacerItem* spacer90_3_7;
    QGridLayout* CoaddStackPageLayout;
    QSpacerItem* spacer107;
    QSpacerItem* spacer106;
    QGridLayout* frame4Layout;
    QGridLayout* layout5;
    QSpacerItem* spacer113_2;
    QGridLayout* layout9;
    QSpacerItem* spacer114;
    QGridLayout* frame4_2Layout;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // PARAMETERS_H