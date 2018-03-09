/****************************************************************************
** theliForm meta object code from reading C++ file 'theliform.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/theliform.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *theliForm::className() const
{
    return "theliForm";
}

QMetaObject *theliForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_theliForm( "theliForm", &theliForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString theliForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "theliForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString theliForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "theliForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* theliForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "showversionwarning", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"versioncheck", 1, param_slot_0 };
    static const QUMethod slot_1 = {"changestatus_set_imextension", 0, 0 };
    static const QUMethod slot_2 = {"open_pipelinepaper", 0, 0 };
    static const QUMethod slot_3 = {"changestatus_obtain_imextension", 0, 0 };
    static const QUMethod slot_4 = {"flush_tempdir", 0, 0 };
    static const QUMethod slot_5 = {"flush_tempdir_updatespace", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "datadir", &static_QUType_ptr, "QLineEdit", QUParameter::In },
	{ "subdir", &static_QUType_QString, 0, QUParameter::In },
	{ "extension", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"delete_data_helper", 3, param_slot_6 };
    static const QUMethod slot_7 = {"delete_MASK_IMAGES", 0, 0 };
    static const QUMethod slot_8 = {"delete_SPLIT_IMAGES", 0, 0 };
    static const QUMethod slot_9 = {"delete_OFC_IMAGES", 0, 0 };
    static const QUMethod slot_10 = {"delete_OFCB_IMAGES", 0, 0 };
    static const QUMethod slot_11 = {"delete_OFCH_IMAGES", 0, 0 };
    static const QUMethod slot_12 = {"delete_undebloomed", 0, 0 };
    static const QUMethod slot_13 = {"delete_beforedirectphot", 0, 0 };
    static const QUMethod slot_14 = {"delete_sub", 0, 0 };
    static const QUMethod slot_15 = {"delete_resampled", 0, 0 };
    static const QUMethod slot_16 = {"style_pushbuttonclicked", 0, 0 };
    static const QUMethod slot_17 = {"font_pushbuttonclicked", 0, 0 };
    static const QUMethod slot_18 = {"show_software_version", 0, 0 };
    static const QUMethod slot_19 = {"cleardirs_initialisepushbutton", 0, 0 };
    static const QUMethod slot_20 = {"viewimageOpen", 0, 0 };
    static const QUParameter param_slot_21[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_21 = {"get_lastchar", 1, param_slot_21 };
    static const QUMethod slot_22 = {"call_parameters_helper", 0, 0 };
    static const QUMethod slot_23 = {"call_parameters", 0, 0 };
    static const QUMethod slot_24 = {"call_parameters_pre", 0, 0 };
    static const QUMethod slot_25 = {"call_parameters_cal", 0, 0 };
    static const QUMethod slot_26 = {"call_parameters_sf1", 0, 0 };
    static const QUMethod slot_27 = {"call_parameters_sf2", 0, 0 };
    static const QUMethod slot_28 = {"call_parameters_w1", 0, 0 };
    static const QUMethod slot_29 = {"call_parameters_w3", 0, 0 };
    static const QUMethod slot_30 = {"call_parameters_w2", 0, 0 };
    static const QUMethod slot_31 = {"call_parameters_absphotom_indirect", 0, 0 };
    static const QUMethod slot_32 = {"call_parameters_absphotom_direct", 0, 0 };
    static const QUMethod slot_33 = {"call_parameters_sexcat", 0, 0 };
    static const QUMethod slot_34 = {"call_parameters_astrom", 0, 0 };
    static const QUMethod slot_35 = {"call_parameters_cc1", 0, 0 };
    static const QUMethod slot_36 = {"call_cni", 0, 0 };
    static const QUMethod slot_37 = {"call_color", 0, 0 };
    static const QUMethod slot_38 = {"call_animate", 0, 0 };
    static const QUMethod slot_39 = {"call_zp", 0, 0 };
    static const QUMethod slot_40 = {"call_combineimages", 0, 0 };
    static const QUMethod slot_41 = {"call_imagestatistics", 0, 0 };
    static const QUMethod slot_42 = {"call_imalyzer", 0, 0 };
    static const QUMethod slot_43 = {"call_select_astroref_image", 0, 0 };
    static const QUMethod slot_44 = {"call_commandhistory", 0, 0 };
    static const QUMethod slot_45 = {"edit_scripts", 0, 0 };
    static const QUMethod slot_46 = {"edit_userinstconfigs", 0, 0 };
    static const QUMethod slot_47 = {"edit_profinstconfigs", 0, 0 };
    static const QUMethod slot_48 = {"edit_comminstconfigs", 0, 0 };
    static const QUMethod slot_49 = {"show_prelog", 0, 0 };
    static const QUMethod slot_50 = {"show_callog", 0, 0 };
    static const QUMethod slot_51 = {"show_sflog", 0, 0 };
    static const QUMethod slot_52 = {"show_wlog", 0, 0 };
    static const QUMethod slot_53 = {"show_aplog", 0, 0 };
    static const QUMethod slot_54 = {"show_cclog", 0, 0 };
    static const QUParameter param_slot_55[] = {
	{ "keyword", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_55 = {"get_fitsext_fromcheckboxes", 1, param_slot_55 };
    static const QUParameter param_slot_56[] = {
	{ "result", &static_QUType_int, 0, QUParameter::InOut }
    };
    static const QUMethod slot_56 = {"is_imextension_empty", 1, param_slot_56 };
    static const QUParameter param_slot_57[] = {
	{ "array", &static_QUType_varptr, "\x10", QUParameter::In },
	{ "result", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_57 = {"get_fitsext_forchopnod", 2, param_slot_57 };
    static const QUParameter param_slot_58[] = {
	{ "array", &static_QUType_varptr, "\x10", QUParameter::In },
	{ "result", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_58 = {"get_fitsext_forcollmerge", 2, param_slot_58 };
    static const QUParameter param_slot_59[] = {
	{ "array", &static_QUType_varptr, "\x10", QUParameter::In },
	{ "result", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_59 = {"get_fitsext_fordebloom", 2, param_slot_59 };
    static const QUParameter param_slot_60[] = {
	{ "array", &static_QUType_varptr, "\x10", QUParameter::In },
	{ "result", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_60 = {"get_fitsext_forweights", 2, param_slot_60 };
    static const QUParameter param_slot_61[] = {
	{ "array", &static_QUType_varptr, "\x10", QUParameter::In },
	{ "result", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_61 = {"get_fitsext_forastromcoadd", 2, param_slot_61 };
    static const QUMethod slot_62 = {"which_instlist", 0, 0 };
    static const QUMethod slot_63 = {"check_for_instrument_duplicates", 0, 0 };
    static const QUParameter param_slot_64[] = {
	{ "instrumentname", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_64 = {"highlight_instrumentname", 1, param_slot_64 };
    static const QUMethod slot_65 = {"refresh_instlist", 0, 0 };
    static const QUParameter param_slot_66[] = {
	{ "dir", &static_QUType_QString, 0, QUParameter::In },
	{ "dir2", &static_QUType_QString, 0, QUParameter::In },
	{ "myLineEdit", &static_QUType_ptr, "QLineEdit", QUParameter::In }
    };
    static const QUMethod slot_66 = {"repaint_defaultpaths_helper", 3, param_slot_66 };
    static const QUParameter param_slot_67[] = {
	{ "dir", &static_QUType_QString, 0, QUParameter::In },
	{ "dir2", &static_QUType_QString, 0, QUParameter::In },
	{ "myLineEdit", &static_QUType_ptr, "QLineEdit", QUParameter::In }
    };
    static const QUMethod slot_67 = {"repaint_defaultabspaths_helper", 3, param_slot_67 };
    static const QUMethod slot_68 = {"repaint_defaultpaths", 0, 0 };
    static const QUParameter param_slot_69[] = {
	{ "paramset1inipath", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_69 = {"get_numcpu", 1, param_slot_69 };
    static const QUMethod slot_70 = {"update_current_commandlist", 0, 0 };
    static const QUMethod slot_71 = {"update_tabchanged_commandlists", 0, 0 };
    static const QUParameter param_slot_72[] = {
	{ "myCheckBox", &static_QUType_ptr, "QCheckBox", QUParameter::In },
	{ "myLineEdit", &static_QUType_ptr, "QLineEdit", QUParameter::In }
    };
    static const QUMethod slot_72 = {"repaint_parameter_fields_helper", 2, param_slot_72 };
    static const QUParameter param_slot_73[] = {
	{ "text_edit", &static_QUType_ptr, "QTextEdit", QUParameter::In },
	{ "errormessage", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_73 = {"show_scriptlog", 2, param_slot_73 };
    static const QUMethod slot_74 = {"updateError", 0, 0 };
    static const QUMethod slot_75 = {"updateText", 0, 0 };
    static const QUParameter param_slot_76[] = {
	{ "command", &static_QUType_QString, 0, QUParameter::In },
	{ "reply", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_76 = {"create_thelimessage", 2, param_slot_76 };
    static const QUMethod slot_77 = {"updateExit", 0, 0 };
    static const QUParameter param_slot_78[] = {
	{ "fullpath", &static_QUType_QString, 0, QUParameter::In },
	{ "scripts", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_78 = {"write_superscriptbody", 2, param_slot_78 };
    static const QUMethod slot_79 = {"update_presinst", 0, 0 };
    static const QUMethod slot_80 = {"get_numchips", 0, 0 };
    static const QUMethod slot_81 = {"get_cameratype", 0, 0 };
    static const QUMethod slot_82 = {"modify_npara", 0, 0 };
    static const QUMethod slot_83 = {"modify_paramset2", 0, 0 };
    static const QUMethod slot_84 = {"modify_paramset3", 0, 0 };
    static const QUMethod slot_85 = {"modify_paramsini", 0, 0 };
    static const QUMethod slot_86 = {"restore_BIAS", 0, 0 };
    static const QUMethod slot_87 = {"restore_DARK", 0, 0 };
    static const QUMethod slot_88 = {"restore_FLAT", 0, 0 };
    static const QUMethod slot_89 = {"restore_FLATOFF", 0, 0 };
    static const QUMethod slot_90 = {"restore_SCIENCE", 0, 0 };
    static const QUMethod slot_91 = {"restore_SKY", 0, 0 };
    static const QUMethod slot_92 = {"restore_STANDARD", 0, 0 };
    static const QUMethod slot_93 = {"restore_ALL", 0, 0 };
    static const QUParameter param_slot_94[] = {
	{ "datasubdir", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_94 = {"restore_subdirs", 1, param_slot_94 };
    static const QUParameter param_slot_95[] = {
	{ "key", &static_QUType_int, 0, QUParameter::In },
	{ "myCheckBox", &static_QUType_ptr, "QCheckBox", QUParameter::In }
    };
    static const QUMethod slot_95 = {"paint_checkboxes_helper", 2, param_slot_95 };
    static const QUMethod slot_96 = {"paint_checkboxes", 0, 0 };
    static const QUMethod slot_97 = {"update_lastlog", 0, 0 };
    static const QUMethod slot_98 = {"get_memory", 0, 0 };
    static const QUMethod slot_99 = {"get_diskspace", 0, 0 };
    static const QUMethod slot_100 = {"check_instname_consistency", 0, 0 };
    static const QUMethod slot_101 = {"get_nframes", 0, 0 };
    static const QUMethod slot_102 = {"call_skysubmethod", 0, 0 };
    static const QUMethod slot_103 = {"redo_calib", 0, 0 };
    static const QUMethod slot_104 = {"redo_background", 0, 0 };
    static const QUMethod slot_105 = {"redo_chopnod", 0, 0 };
    static const QUMethod slot_106 = {"redo_collapse", 0, 0 };
    static const QUMethod slot_107 = {"redo_debloom", 0, 0 };
    static const QUMethod slot_108 = {"redo_directphot", 0, 0 };
    static const QUParameter param_slot_109[] = {
	{ "pushbutton", &static_QUType_ptr, "QPushButton", QUParameter::In },
	{ "action", &static_QUType_ptr, "QAction", QUParameter::In },
	{ "checkbox", &static_QUType_ptr, "QCheckBox", QUParameter::In },
	{ "result", &static_QUType_QString, 0, QUParameter::In },
	{ "flag", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_109 = {"redo_helper", 5, param_slot_109 };
    static const QUMethod slot_110 = {"update_zeroheader", 0, 0 };
    static const QUMethod slot_111 = {"restore_header", 0, 0 };
    static const QUParameter param_slot_112[] = {
	{ "superscript", &static_QUType_QString, 0, QUParameter::In },
	{ "command", &static_QUType_QString, 0, QUParameter::In },
	{ "type", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_112 = {"write_zerohead_superscriptbody", 3, param_slot_112 };
    static const QUMethod slot_113 = {"update_zeroheaderText", 0, 0 };
    static const QUMethod slot_114 = {"update_restoreheaderText", 0, 0 };
    static const QUParameter param_slot_115[] = {
	{ "textedit", &static_QUType_ptr, "QTextEdit", QUParameter::In },
	{ "datadir", &static_QUType_QString, 0, QUParameter::In },
	{ "extension", &static_QUType_QString, 0, QUParameter::In },
	{ "flag", &static_QUType_int, 0, QUParameter::InOut },
	{ "flag_ini", &static_QUType_int, 0, QUParameter::InOut }
    };
    static const QUMethod slot_115 = {"check_filepresence", 5, param_slot_115 };
    static const QUMethod slot_116 = {"call_monitor", 0, 0 };
    static const QUMethod slot_117 = {"update_mainwindowtitle", 0, 0 };
    static const QUParameter param_slot_118[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_118 = {"get_full_instpath", 1, param_slot_118 };
    static const QUParameter param_slot_119[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_119 = {"get_full_aheadpath", 1, param_slot_119 };
    static const QUMethod slot_120 = {"resolve_target", 0, 0 };
    static const QUMethod slot_121 = {"resolve_target_readstdout", 0, 0 };
    static const QUMethod slot_122 = {"resolve_target_exit", 0, 0 };
    static const QUMethod slot_123 = {"enable_server", 0, 0 };
    static const QUParameter param_slot_124[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_124 = {"getfilterlist", 1, param_slot_124 };
    static const QUParameter param_slot_125[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::InOut }
    };
    static const QUMethod slot_125 = {"check_filenamelength", 3, param_slot_125 };
    static const QUMethod slot_126 = {"make_ascii_conform", 0, 0 };
    static const QUParameter param_slot_127[] = {
	{ "dir", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_127 = {"make_ascii_conform_helper", 1, param_slot_127 };
    static const QUMethod slot_128 = {"get_astref_catalog_button", 0, 0 };
    static const QUParameter param_slot_129[] = {
	{ "science", &static_QUType_QString, 0, QUParameter::In },
	{ "type", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_129 = {"get_astref_catalog_normal", 2, param_slot_129 };
    static const QUMethod slot_130 = {"check_nonlinearity", 0, 0 };
    static const QUMethod slot_131 = {"check_coordinate_format", 0, 0 };
    static const QUMethod slot_132 = {"clean_refcoordsLineEdit", 0, 0 };
    static const QUMethod slot_133 = {"write_logfile", 0, 0 };
    static const QUMethod slot_134 = {"read_logfile", 0, 0 };
    static const QUMethod slot_135 = {"clear_processingstatus_in_logfile", 0, 0 };
    static const QUMethod slot_136 = {"obtain_imextension", 0, 0 };
    static const QUParameter param_slot_137[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In },
	{ "position", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_137 = {"set_imextension", 2, param_slot_137 };
    static const QUMethod slot_138 = {"update_imextension", 0, 0 };
    static const QUMethod slot_139 = {"clearscripts_pre_pushbutton", 0, 0 };
    static const QUMethod slot_140 = {"resetfields_pre_pushbutton", 0, 0 };
    static const QUMethod slot_141 = {"resetfields_cal_pushbutton", 0, 0 };
    static const QUMethod slot_142 = {"clearscripts_cal_pushbutton", 0, 0 };
    static const QUMethod slot_143 = {"resetfields_sf_pushbutton", 0, 0 };
    static const QUMethod slot_144 = {"clearscripts_sf_pushbutton", 0, 0 };
    static const QUMethod slot_145 = {"clearscripts_w_pushbutton", 0, 0 };
    static const QUMethod slot_146 = {"resetfields_w_pushbutton", 0, 0 };
    static const QUMethod slot_147 = {"clearscripts_ap_pushbutton", 0, 0 };
    static const QUMethod slot_148 = {"resetfields_ap_pushbutton", 0, 0 };
    static const QUMethod slot_149 = {"clearscripts_cc_pushbutton", 0, 0 };
    static const QUMethod slot_150 = {"resetfields_cc_pushbutton", 0, 0 };
    static const QUMethod slot_151 = {"update_instrument_inlogfile", 0, 0 };
    static const QUMethod slot_152 = {"load_reductionlog", 0, 0 };
    static const QUMethod slot_153 = {"apply_logfile", 0, 0 };
    static const QUMethod slot_154 = {"update_logfile", 0, 0 };
    static const QUMethod slot_155 = {"read_lastlog", 0, 0 };
    static const QUMethod slot_156 = {"update_ap_variousfields", 0, 0 };
    static const QUParameter param_slot_157[] = {
	{ "defaultdir", &static_QUType_ptr, "QLineEdit", QUParameter::In }
    };
    static const QUMethod slot_157 = {"pre_splitdirs", 1, param_slot_157 };
    static const QUParameter param_slot_158[] = {
	{ "defaultdir", &static_QUType_ptr, "QLineEdit", QUParameter::In }
    };
    static const QUMethod slot_158 = {"pre_createlinks", 1, param_slot_158 };
    static const QUMethod slot_159 = {"toggle_darkinsteadbiascheckbox", 0, 0 };
    static const QUMethod slot_160 = {"update_biascheckboxcolor", 0, 0 };
    static const QUMethod slot_161 = {"update_pre_commandlist", 0, 0 };
    static const QUMethod slot_162 = {"update_cal_commandlist", 0, 0 };
    static const QUMethod slot_163 = {"update_sf_commandlist", 0, 0 };
    static const QUMethod slot_164 = {"update_w_commandlist", 0, 0 };
    static const QUMethod slot_165 = {"update_ap_commandlist", 0, 0 };
    static const QUMethod slot_166 = {"update_cc_commandlist", 0, 0 };
    static const QUMethod slot_167 = {"repaint_parameter_fields_pre", 0, 0 };
    static const QUMethod slot_168 = {"repaint_parameter_fields_sf", 0, 0 };
    static const QUMethod slot_169 = {"repaint_parameter_fields_w", 0, 0 };
    static const QUMethod slot_170 = {"repaint_parameter_fields_ap", 0, 0 };
    static const QUMethod slot_171 = {"exec_pre", 0, 0 };
    static const QUMethod slot_172 = {"exec_cal", 0, 0 };
    static const QUMethod slot_173 = {"exec_sf", 0, 0 };
    static const QUMethod slot_174 = {"exec_w", 0, 0 };
    static const QUMethod slot_175 = {"exec_ap", 0, 0 };
    static const QUMethod slot_176 = {"exec_cc", 0, 0 };
    static const QUMethod slot_177 = {"kill_pre", 0, 0 };
    static const QUMethod slot_178 = {"kill_cal", 0, 0 };
    static const QUMethod slot_179 = {"kill_sf", 0, 0 };
    static const QUMethod slot_180 = {"kill_w", 0, 0 };
    static const QUMethod slot_181 = {"kill_ap", 0, 0 };
    static const QUMethod slot_182 = {"kill_cc", 0, 0 };
    static const QUMethod slot_183 = {"validate_refcoords", 0, 0 };
    static const QUParameter param_slot_184[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::InOut }
    };
    static const QUMethod slot_184 = {"check_splittingstatus", 2, param_slot_184 };
    static const QUMethod slot_185 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "versioncheck(int)", &slot_0, QMetaData::Public },
	{ "changestatus_set_imextension()", &slot_1, QMetaData::Public },
	{ "open_pipelinepaper()", &slot_2, QMetaData::Public },
	{ "changestatus_obtain_imextension()", &slot_3, QMetaData::Public },
	{ "flush_tempdir()", &slot_4, QMetaData::Public },
	{ "flush_tempdir_updatespace()", &slot_5, QMetaData::Public },
	{ "delete_data_helper(QLineEdit*,QString,QString)", &slot_6, QMetaData::Public },
	{ "delete_MASK_IMAGES()", &slot_7, QMetaData::Public },
	{ "delete_SPLIT_IMAGES()", &slot_8, QMetaData::Public },
	{ "delete_OFC_IMAGES()", &slot_9, QMetaData::Public },
	{ "delete_OFCB_IMAGES()", &slot_10, QMetaData::Public },
	{ "delete_OFCH_IMAGES()", &slot_11, QMetaData::Public },
	{ "delete_undebloomed()", &slot_12, QMetaData::Public },
	{ "delete_beforedirectphot()", &slot_13, QMetaData::Public },
	{ "delete_sub()", &slot_14, QMetaData::Public },
	{ "delete_resampled()", &slot_15, QMetaData::Public },
	{ "style_pushbuttonclicked()", &slot_16, QMetaData::Public },
	{ "font_pushbuttonclicked()", &slot_17, QMetaData::Public },
	{ "show_software_version()", &slot_18, QMetaData::Public },
	{ "cleardirs_initialisepushbutton()", &slot_19, QMetaData::Public },
	{ "viewimageOpen()", &slot_20, QMetaData::Public },
	{ "get_lastchar(QString&)", &slot_21, QMetaData::Public },
	{ "call_parameters_helper()", &slot_22, QMetaData::Public },
	{ "call_parameters()", &slot_23, QMetaData::Public },
	{ "call_parameters_pre()", &slot_24, QMetaData::Public },
	{ "call_parameters_cal()", &slot_25, QMetaData::Public },
	{ "call_parameters_sf1()", &slot_26, QMetaData::Public },
	{ "call_parameters_sf2()", &slot_27, QMetaData::Public },
	{ "call_parameters_w1()", &slot_28, QMetaData::Public },
	{ "call_parameters_w3()", &slot_29, QMetaData::Public },
	{ "call_parameters_w2()", &slot_30, QMetaData::Public },
	{ "call_parameters_absphotom_indirect()", &slot_31, QMetaData::Public },
	{ "call_parameters_absphotom_direct()", &slot_32, QMetaData::Public },
	{ "call_parameters_sexcat()", &slot_33, QMetaData::Public },
	{ "call_parameters_astrom()", &slot_34, QMetaData::Public },
	{ "call_parameters_cc1()", &slot_35, QMetaData::Public },
	{ "call_cni()", &slot_36, QMetaData::Public },
	{ "call_color()", &slot_37, QMetaData::Public },
	{ "call_animate()", &slot_38, QMetaData::Public },
	{ "call_zp()", &slot_39, QMetaData::Public },
	{ "call_combineimages()", &slot_40, QMetaData::Public },
	{ "call_imagestatistics()", &slot_41, QMetaData::Public },
	{ "call_imalyzer()", &slot_42, QMetaData::Public },
	{ "call_select_astroref_image()", &slot_43, QMetaData::Public },
	{ "call_commandhistory()", &slot_44, QMetaData::Public },
	{ "edit_scripts()", &slot_45, QMetaData::Public },
	{ "edit_userinstconfigs()", &slot_46, QMetaData::Public },
	{ "edit_profinstconfigs()", &slot_47, QMetaData::Public },
	{ "edit_comminstconfigs()", &slot_48, QMetaData::Public },
	{ "show_prelog()", &slot_49, QMetaData::Public },
	{ "show_callog()", &slot_50, QMetaData::Public },
	{ "show_sflog()", &slot_51, QMetaData::Public },
	{ "show_wlog()", &slot_52, QMetaData::Public },
	{ "show_aplog()", &slot_53, QMetaData::Public },
	{ "show_cclog()", &slot_54, QMetaData::Public },
	{ "get_fitsext_fromcheckboxes(QString)", &slot_55, QMetaData::Public },
	{ "is_imextension_empty(int&)", &slot_56, QMetaData::Public },
	{ "get_fitsext_forchopnod(int*,QString&)", &slot_57, QMetaData::Public },
	{ "get_fitsext_forcollmerge(int*,QString&)", &slot_58, QMetaData::Public },
	{ "get_fitsext_fordebloom(int*,QString&)", &slot_59, QMetaData::Public },
	{ "get_fitsext_forweights(int*,QString&)", &slot_60, QMetaData::Public },
	{ "get_fitsext_forastromcoadd(int*,QString&)", &slot_61, QMetaData::Public },
	{ "which_instlist()", &slot_62, QMetaData::Public },
	{ "check_for_instrument_duplicates()", &slot_63, QMetaData::Public },
	{ "highlight_instrumentname(QString)", &slot_64, QMetaData::Public },
	{ "refresh_instlist()", &slot_65, QMetaData::Public },
	{ "repaint_defaultpaths_helper(QString,QString,QLineEdit*)", &slot_66, QMetaData::Public },
	{ "repaint_defaultabspaths_helper(QString,QString,QLineEdit*)", &slot_67, QMetaData::Public },
	{ "repaint_defaultpaths()", &slot_68, QMetaData::Public },
	{ "get_numcpu(QString)", &slot_69, QMetaData::Public },
	{ "update_current_commandlist()", &slot_70, QMetaData::Public },
	{ "update_tabchanged_commandlists()", &slot_71, QMetaData::Public },
	{ "repaint_parameter_fields_helper(QCheckBox*,QLineEdit*)", &slot_72, QMetaData::Public },
	{ "show_scriptlog(QTextEdit*,QString)", &slot_73, QMetaData::Public },
	{ "updateError()", &slot_74, QMetaData::Public },
	{ "updateText()", &slot_75, QMetaData::Public },
	{ "create_thelimessage(QString,QString&)", &slot_76, QMetaData::Public },
	{ "updateExit()", &slot_77, QMetaData::Public },
	{ "write_superscriptbody(QString,QString)", &slot_78, QMetaData::Public },
	{ "update_presinst()", &slot_79, QMetaData::Public },
	{ "get_numchips()", &slot_80, QMetaData::Public },
	{ "get_cameratype()", &slot_81, QMetaData::Public },
	{ "modify_npara()", &slot_82, QMetaData::Public },
	{ "modify_paramset2()", &slot_83, QMetaData::Public },
	{ "modify_paramset3()", &slot_84, QMetaData::Public },
	{ "modify_paramsini()", &slot_85, QMetaData::Public },
	{ "restore_BIAS()", &slot_86, QMetaData::Public },
	{ "restore_DARK()", &slot_87, QMetaData::Public },
	{ "restore_FLAT()", &slot_88, QMetaData::Public },
	{ "restore_FLATOFF()", &slot_89, QMetaData::Public },
	{ "restore_SCIENCE()", &slot_90, QMetaData::Public },
	{ "restore_SKY()", &slot_91, QMetaData::Public },
	{ "restore_STANDARD()", &slot_92, QMetaData::Public },
	{ "restore_ALL()", &slot_93, QMetaData::Public },
	{ "restore_subdirs(QString)", &slot_94, QMetaData::Public },
	{ "paint_checkboxes_helper(int,QCheckBox*)", &slot_95, QMetaData::Public },
	{ "paint_checkboxes()", &slot_96, QMetaData::Public },
	{ "update_lastlog()", &slot_97, QMetaData::Public },
	{ "get_memory()", &slot_98, QMetaData::Public },
	{ "get_diskspace()", &slot_99, QMetaData::Public },
	{ "check_instname_consistency()", &slot_100, QMetaData::Public },
	{ "get_nframes()", &slot_101, QMetaData::Public },
	{ "call_skysubmethod()", &slot_102, QMetaData::Public },
	{ "redo_calib()", &slot_103, QMetaData::Public },
	{ "redo_background()", &slot_104, QMetaData::Public },
	{ "redo_chopnod()", &slot_105, QMetaData::Public },
	{ "redo_collapse()", &slot_106, QMetaData::Public },
	{ "redo_debloom()", &slot_107, QMetaData::Public },
	{ "redo_directphot()", &slot_108, QMetaData::Public },
	{ "redo_helper(QPushButton*,QAction*,QCheckBox*,QString,int)", &slot_109, QMetaData::Public },
	{ "update_zeroheader()", &slot_110, QMetaData::Public },
	{ "restore_header()", &slot_111, QMetaData::Public },
	{ "write_zerohead_superscriptbody(QString,QString,QString)", &slot_112, QMetaData::Public },
	{ "update_zeroheaderText()", &slot_113, QMetaData::Public },
	{ "update_restoreheaderText()", &slot_114, QMetaData::Public },
	{ "check_filepresence(QTextEdit*,QString,QString,int&,int&)", &slot_115, QMetaData::Public },
	{ "call_monitor()", &slot_116, QMetaData::Public },
	{ "update_mainwindowtitle()", &slot_117, QMetaData::Public },
	{ "get_full_instpath(QString&)", &slot_118, QMetaData::Public },
	{ "get_full_aheadpath(QString&)", &slot_119, QMetaData::Public },
	{ "resolve_target()", &slot_120, QMetaData::Public },
	{ "resolve_target_readstdout()", &slot_121, QMetaData::Public },
	{ "resolve_target_exit()", &slot_122, QMetaData::Public },
	{ "enable_server()", &slot_123, QMetaData::Public },
	{ "getfilterlist(QString&)", &slot_124, QMetaData::Public },
	{ "check_filenamelength(QString,QString,int&)", &slot_125, QMetaData::Public },
	{ "make_ascii_conform()", &slot_126, QMetaData::Public },
	{ "make_ascii_conform_helper(QString)", &slot_127, QMetaData::Public },
	{ "get_astref_catalog_button()", &slot_128, QMetaData::Public },
	{ "get_astref_catalog_normal(QString,QString)", &slot_129, QMetaData::Public },
	{ "check_nonlinearity()", &slot_130, QMetaData::Public },
	{ "check_coordinate_format()", &slot_131, QMetaData::Public },
	{ "clean_refcoordsLineEdit()", &slot_132, QMetaData::Public },
	{ "write_logfile()", &slot_133, QMetaData::Public },
	{ "read_logfile()", &slot_134, QMetaData::Public },
	{ "clear_processingstatus_in_logfile()", &slot_135, QMetaData::Public },
	{ "obtain_imextension()", &slot_136, QMetaData::Public },
	{ "set_imextension(int,int)", &slot_137, QMetaData::Public },
	{ "update_imextension()", &slot_138, QMetaData::Public },
	{ "clearscripts_pre_pushbutton()", &slot_139, QMetaData::Public },
	{ "resetfields_pre_pushbutton()", &slot_140, QMetaData::Public },
	{ "resetfields_cal_pushbutton()", &slot_141, QMetaData::Public },
	{ "clearscripts_cal_pushbutton()", &slot_142, QMetaData::Public },
	{ "resetfields_sf_pushbutton()", &slot_143, QMetaData::Public },
	{ "clearscripts_sf_pushbutton()", &slot_144, QMetaData::Public },
	{ "clearscripts_w_pushbutton()", &slot_145, QMetaData::Public },
	{ "resetfields_w_pushbutton()", &slot_146, QMetaData::Public },
	{ "clearscripts_ap_pushbutton()", &slot_147, QMetaData::Public },
	{ "resetfields_ap_pushbutton()", &slot_148, QMetaData::Public },
	{ "clearscripts_cc_pushbutton()", &slot_149, QMetaData::Public },
	{ "resetfields_cc_pushbutton()", &slot_150, QMetaData::Public },
	{ "update_instrument_inlogfile()", &slot_151, QMetaData::Public },
	{ "load_reductionlog()", &slot_152, QMetaData::Public },
	{ "apply_logfile()", &slot_153, QMetaData::Public },
	{ "update_logfile()", &slot_154, QMetaData::Public },
	{ "read_lastlog()", &slot_155, QMetaData::Public },
	{ "update_ap_variousfields()", &slot_156, QMetaData::Public },
	{ "pre_splitdirs(QLineEdit*)", &slot_157, QMetaData::Public },
	{ "pre_createlinks(QLineEdit*)", &slot_158, QMetaData::Public },
	{ "toggle_darkinsteadbiascheckbox()", &slot_159, QMetaData::Public },
	{ "update_biascheckboxcolor()", &slot_160, QMetaData::Public },
	{ "update_pre_commandlist()", &slot_161, QMetaData::Public },
	{ "update_cal_commandlist()", &slot_162, QMetaData::Public },
	{ "update_sf_commandlist()", &slot_163, QMetaData::Public },
	{ "update_w_commandlist()", &slot_164, QMetaData::Public },
	{ "update_ap_commandlist()", &slot_165, QMetaData::Public },
	{ "update_cc_commandlist()", &slot_166, QMetaData::Public },
	{ "repaint_parameter_fields_pre()", &slot_167, QMetaData::Public },
	{ "repaint_parameter_fields_sf()", &slot_168, QMetaData::Public },
	{ "repaint_parameter_fields_w()", &slot_169, QMetaData::Public },
	{ "repaint_parameter_fields_ap()", &slot_170, QMetaData::Public },
	{ "exec_pre()", &slot_171, QMetaData::Public },
	{ "exec_cal()", &slot_172, QMetaData::Public },
	{ "exec_sf()", &slot_173, QMetaData::Public },
	{ "exec_w()", &slot_174, QMetaData::Public },
	{ "exec_ap()", &slot_175, QMetaData::Public },
	{ "exec_cc()", &slot_176, QMetaData::Public },
	{ "kill_pre()", &slot_177, QMetaData::Public },
	{ "kill_cal()", &slot_178, QMetaData::Public },
	{ "kill_sf()", &slot_179, QMetaData::Public },
	{ "kill_w()", &slot_180, QMetaData::Public },
	{ "kill_ap()", &slot_181, QMetaData::Public },
	{ "kill_cc()", &slot_182, QMetaData::Public },
	{ "validate_refcoords()", &slot_183, QMetaData::Public },
	{ "check_splittingstatus(QString,int&)", &slot_184, QMetaData::Public },
	{ "languageChange()", &slot_185, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"theliForm", parentObject,
	slot_tbl, 186,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_theliForm.setMetaObject( metaObj );
    return metaObj;
}

void* theliForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "theliForm" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool theliForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: versioncheck((int)static_QUType_int.get(_o+1)); break;
    case 1: changestatus_set_imextension(); break;
    case 2: open_pipelinepaper(); break;
    case 3: changestatus_obtain_imextension(); break;
    case 4: flush_tempdir(); break;
    case 5: flush_tempdir_updatespace(); break;
    case 6: delete_data_helper((QLineEdit*)static_QUType_ptr.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 7: delete_MASK_IMAGES(); break;
    case 8: delete_SPLIT_IMAGES(); break;
    case 9: delete_OFC_IMAGES(); break;
    case 10: delete_OFCB_IMAGES(); break;
    case 11: delete_OFCH_IMAGES(); break;
    case 12: delete_undebloomed(); break;
    case 13: delete_beforedirectphot(); break;
    case 14: delete_sub(); break;
    case 15: delete_resampled(); break;
    case 16: style_pushbuttonclicked(); break;
    case 17: font_pushbuttonclicked(); break;
    case 18: show_software_version(); break;
    case 19: cleardirs_initialisepushbutton(); break;
    case 20: viewimageOpen(); break;
    case 21: get_lastchar((QString&)static_QUType_QString.get(_o+1)); break;
    case 22: call_parameters_helper(); break;
    case 23: call_parameters(); break;
    case 24: call_parameters_pre(); break;
    case 25: call_parameters_cal(); break;
    case 26: call_parameters_sf1(); break;
    case 27: call_parameters_sf2(); break;
    case 28: call_parameters_w1(); break;
    case 29: call_parameters_w3(); break;
    case 30: call_parameters_w2(); break;
    case 31: call_parameters_absphotom_indirect(); break;
    case 32: call_parameters_absphotom_direct(); break;
    case 33: call_parameters_sexcat(); break;
    case 34: call_parameters_astrom(); break;
    case 35: call_parameters_cc1(); break;
    case 36: call_cni(); break;
    case 37: call_color(); break;
    case 38: call_animate(); break;
    case 39: call_zp(); break;
    case 40: call_combineimages(); break;
    case 41: call_imagestatistics(); break;
    case 42: call_imalyzer(); break;
    case 43: call_select_astroref_image(); break;
    case 44: call_commandhistory(); break;
    case 45: edit_scripts(); break;
    case 46: edit_userinstconfigs(); break;
    case 47: edit_profinstconfigs(); break;
    case 48: edit_comminstconfigs(); break;
    case 49: show_prelog(); break;
    case 50: show_callog(); break;
    case 51: show_sflog(); break;
    case 52: show_wlog(); break;
    case 53: show_aplog(); break;
    case 54: show_cclog(); break;
    case 55: get_fitsext_fromcheckboxes((QString)static_QUType_QString.get(_o+1)); break;
    case 56: is_imextension_empty((int&)static_QUType_int.get(_o+1)); break;
    case 57: get_fitsext_forchopnod((int*)static_QUType_varptr.get(_o+1),(QString&)static_QUType_QString.get(_o+2)); break;
    case 58: get_fitsext_forcollmerge((int*)static_QUType_varptr.get(_o+1),(QString&)static_QUType_QString.get(_o+2)); break;
    case 59: get_fitsext_fordebloom((int*)static_QUType_varptr.get(_o+1),(QString&)static_QUType_QString.get(_o+2)); break;
    case 60: get_fitsext_forweights((int*)static_QUType_varptr.get(_o+1),(QString&)static_QUType_QString.get(_o+2)); break;
    case 61: get_fitsext_forastromcoadd((int*)static_QUType_varptr.get(_o+1),(QString&)static_QUType_QString.get(_o+2)); break;
    case 62: which_instlist(); break;
    case 63: check_for_instrument_duplicates(); break;
    case 64: highlight_instrumentname((QString)static_QUType_QString.get(_o+1)); break;
    case 65: refresh_instlist(); break;
    case 66: repaint_defaultpaths_helper((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QLineEdit*)static_QUType_ptr.get(_o+3)); break;
    case 67: repaint_defaultabspaths_helper((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QLineEdit*)static_QUType_ptr.get(_o+3)); break;
    case 68: repaint_defaultpaths(); break;
    case 69: get_numcpu((QString)static_QUType_QString.get(_o+1)); break;
    case 70: update_current_commandlist(); break;
    case 71: update_tabchanged_commandlists(); break;
    case 72: repaint_parameter_fields_helper((QCheckBox*)static_QUType_ptr.get(_o+1),(QLineEdit*)static_QUType_ptr.get(_o+2)); break;
    case 73: show_scriptlog((QTextEdit*)static_QUType_ptr.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 74: updateError(); break;
    case 75: updateText(); break;
    case 76: create_thelimessage((QString)static_QUType_QString.get(_o+1),(QString&)static_QUType_QString.get(_o+2)); break;
    case 77: updateExit(); break;
    case 78: write_superscriptbody((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 79: update_presinst(); break;
    case 80: get_numchips(); break;
    case 81: get_cameratype(); break;
    case 82: modify_npara(); break;
    case 83: modify_paramset2(); break;
    case 84: modify_paramset3(); break;
    case 85: modify_paramsini(); break;
    case 86: restore_BIAS(); break;
    case 87: restore_DARK(); break;
    case 88: restore_FLAT(); break;
    case 89: restore_FLATOFF(); break;
    case 90: restore_SCIENCE(); break;
    case 91: restore_SKY(); break;
    case 92: restore_STANDARD(); break;
    case 93: restore_ALL(); break;
    case 94: restore_subdirs((QString)static_QUType_QString.get(_o+1)); break;
    case 95: paint_checkboxes_helper((int)static_QUType_int.get(_o+1),(QCheckBox*)static_QUType_ptr.get(_o+2)); break;
    case 96: paint_checkboxes(); break;
    case 97: update_lastlog(); break;
    case 98: get_memory(); break;
    case 99: get_diskspace(); break;
    case 100: check_instname_consistency(); break;
    case 101: get_nframes(); break;
    case 102: call_skysubmethod(); break;
    case 103: redo_calib(); break;
    case 104: redo_background(); break;
    case 105: redo_chopnod(); break;
    case 106: redo_collapse(); break;
    case 107: redo_debloom(); break;
    case 108: redo_directphot(); break;
    case 109: redo_helper((QPushButton*)static_QUType_ptr.get(_o+1),(QAction*)static_QUType_ptr.get(_o+2),(QCheckBox*)static_QUType_ptr.get(_o+3),(QString)static_QUType_QString.get(_o+4),(int)static_QUType_int.get(_o+5)); break;
    case 110: update_zeroheader(); break;
    case 111: restore_header(); break;
    case 112: write_zerohead_superscriptbody((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 113: update_zeroheaderText(); break;
    case 114: update_restoreheaderText(); break;
    case 115: check_filepresence((QTextEdit*)static_QUType_ptr.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3),(int&)static_QUType_int.get(_o+4),(int&)static_QUType_int.get(_o+5)); break;
    case 116: call_monitor(); break;
    case 117: update_mainwindowtitle(); break;
    case 118: get_full_instpath((QString&)static_QUType_QString.get(_o+1)); break;
    case 119: get_full_aheadpath((QString&)static_QUType_QString.get(_o+1)); break;
    case 120: resolve_target(); break;
    case 121: resolve_target_readstdout(); break;
    case 122: resolve_target_exit(); break;
    case 123: enable_server(); break;
    case 124: getfilterlist((QString&)static_QUType_QString.get(_o+1)); break;
    case 125: check_filenamelength((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(int&)static_QUType_int.get(_o+3)); break;
    case 126: make_ascii_conform(); break;
    case 127: make_ascii_conform_helper((QString)static_QUType_QString.get(_o+1)); break;
    case 128: get_astref_catalog_button(); break;
    case 129: get_astref_catalog_normal((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 130: check_nonlinearity(); break;
    case 131: check_coordinate_format(); break;
    case 132: clean_refcoordsLineEdit(); break;
    case 133: write_logfile(); break;
    case 134: read_logfile(); break;
    case 135: clear_processingstatus_in_logfile(); break;
    case 136: obtain_imextension(); break;
    case 137: set_imextension((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 138: update_imextension(); break;
    case 139: clearscripts_pre_pushbutton(); break;
    case 140: resetfields_pre_pushbutton(); break;
    case 141: resetfields_cal_pushbutton(); break;
    case 142: clearscripts_cal_pushbutton(); break;
    case 143: resetfields_sf_pushbutton(); break;
    case 144: clearscripts_sf_pushbutton(); break;
    case 145: clearscripts_w_pushbutton(); break;
    case 146: resetfields_w_pushbutton(); break;
    case 147: clearscripts_ap_pushbutton(); break;
    case 148: resetfields_ap_pushbutton(); break;
    case 149: clearscripts_cc_pushbutton(); break;
    case 150: resetfields_cc_pushbutton(); break;
    case 151: update_instrument_inlogfile(); break;
    case 152: load_reductionlog(); break;
    case 153: apply_logfile(); break;
    case 154: update_logfile(); break;
    case 155: read_lastlog(); break;
    case 156: update_ap_variousfields(); break;
    case 157: pre_splitdirs((QLineEdit*)static_QUType_ptr.get(_o+1)); break;
    case 158: pre_createlinks((QLineEdit*)static_QUType_ptr.get(_o+1)); break;
    case 159: toggle_darkinsteadbiascheckbox(); break;
    case 160: update_biascheckboxcolor(); break;
    case 161: update_pre_commandlist(); break;
    case 162: update_cal_commandlist(); break;
    case 163: update_sf_commandlist(); break;
    case 164: update_w_commandlist(); break;
    case 165: update_ap_commandlist(); break;
    case 166: update_cc_commandlist(); break;
    case 167: repaint_parameter_fields_pre(); break;
    case 168: repaint_parameter_fields_sf(); break;
    case 169: repaint_parameter_fields_w(); break;
    case 170: repaint_parameter_fields_ap(); break;
    case 171: exec_pre(); break;
    case 172: exec_cal(); break;
    case 173: exec_sf(); break;
    case 174: exec_w(); break;
    case 175: exec_ap(); break;
    case 176: exec_cc(); break;
    case 177: kill_pre(); break;
    case 178: kill_cal(); break;
    case 179: kill_sf(); break;
    case 180: kill_w(); break;
    case 181: kill_ap(); break;
    case 182: kill_cc(); break;
    case 183: validate_refcoords(); break;
    case 184: check_splittingstatus((QString)static_QUType_QString.get(_o+1),(int&)static_QUType_int.get(_o+2)); break;
    case 185: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool theliForm::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool theliForm::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool theliForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
