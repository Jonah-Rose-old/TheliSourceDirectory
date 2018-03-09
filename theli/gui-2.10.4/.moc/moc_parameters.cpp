/****************************************************************************
** parameters meta object code from reading C++ file 'parameters.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/parameters.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *parameters::className() const
{
    return "parameters";
}

QMetaObject *parameters::metaObj = 0;
static QMetaObjectCleanUp cleanUp_parameters( "parameters", &parameters::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString parameters::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "parameters", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString parameters::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "parameters", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* parameters::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"show_absphot_readme", 0, 0 };
    static const QUMethod slot_1 = {"show_hide_absphot_wcserror", 0, 0 };
    static const QUMethod slot_2 = {"show_hide_twopass", 0, 0 };
    static const QUMethod slot_3 = {"update_refcoord_textlabel", 0, 0 };
    static const QUMethod slot_4 = {"call_mtype_readme", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ "page", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"update_headertextlabel", 1, param_slot_5 };
    static const QUMethod slot_6 = {"switch_page_next", 0, 0 };
    static const QUMethod slot_7 = {"show_hide_filtercombobox", 0, 0 };
    static const QUMethod slot_8 = {"fill_coadd_filtercombobox", 0, 0 };
    static const QUMethod slot_9 = {"fill_skyposangle", 0, 0 };
    static const QUMethod slot_10 = {"switch_page_back", 0, 0 };
    static const QUMethod slot_11 = {"check_for_reset", 0, 0 };
    static const QUMethod slot_12 = {"populate_absphotmenu", 0, 0 };
    static const QUMethod slot_13 = {"show_photcal1", 0, 0 };
    static const QUMethod slot_14 = {"show_photcal2", 0, 0 };
    static const QUMethod slot_15 = {"show_photcal3", 0, 0 };
    static const QUMethod slot_16 = {"show_photcal4", 0, 0 };
    static const QUMethod slot_17 = {"show_photcal5", 0, 0 };
    static const QUMethod slot_18 = {"show_photcal6", 0, 0 };
    static const QUMethod slot_19 = {"show_photcal7", 0, 0 };
    static const QUParameter param_slot_20[] = {
	{ "radius", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_20 = {"get_crossidradius", 1, param_slot_20 };
    static const QUMethod slot_21 = {"get_pixscale", 0, 0 };
    static const QUMethod slot_22 = {"get_numchips", 0, 0 };
    static const QUMethod slot_23 = {"update_crossidradius", 0, 0 };
    static const QUParameter param_slot_24[] = {
	{ "answer", &static_QUType_int, 0, QUParameter::InOut }
    };
    static const QUMethod slot_24 = {"check_distortion_consistency", 1, param_slot_24 };
    static const QUMethod slot_25 = {"parameters_quit", 0, 0 };
    static const QUMethod slot_26 = {"update_stdcolor_comboboxes", 0, 0 };
    static const QUMethod slot_27 = {"check_stripe82", 0, 0 };
    static const QUMethod slot_28 = {"init_stdcolor_comboboxes", 0, 0 };
    static const QUMethod slot_29 = {"update_stdcolorfilter", 0, 0 };
    static const QUParameter param_slot_30[] = {
	{ "checkresult", &static_QUType_int, 0, QUParameter::InOut }
    };
    static const QUMethod slot_30 = {"check_nhigh_window", 1, param_slot_30 };
    static const QUMethod slot_31 = {"toggle_statdyn", 0, 0 };
    static const QUMethod slot_32 = {"toggle_globalweight_params", 0, 0 };
    static const QUMethod slot_33 = {"write_parameters", 0, 0 };
    static const QUMethod slot_34 = {"read_parameters", 0, 0 };
    static const QUMethod slot_35 = {"modify_parametersini", 0, 0 };
    static const QUParameter param_slot_36[] = {
	{ "result", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_36 = {"get_coadd_id", 1, param_slot_36 };
    static const QUMethod slot_37 = {"defaults", 0, 0 };
    static const QUMethod slot_38 = {"defaults_all", 0, 0 };
    static const QUMethod slot_39 = {"populate_xtalkbuttons", 0, 0 };
    static const QUMethod slot_40 = {"untoggle_xtalkbuttons", 0, 0 };
    static const QUMethod slot_41 = {"link_checkbox_xtalk", 0, 0 };
    static const QUMethod slot_42 = {"link_xtalk_checkbox", 0, 0 };
    static const QUMethod slot_43 = {"active_passive_xtalk", 0, 0 };
    static const QUMethod slot_44 = {"validate", 0, 0 };
    static const QUMethod slot_45 = {"adjust_collcorr_direction", 0, 0 };
    static const QUMethod slot_46 = {"showhide_bloom", 0, 0 };
    static const QUMethod slot_47 = {"showhide_clipped", 0, 0 };
    static const QUMethod slot_48 = {"showhide_defectdetection", 0, 0 };
    static const QUMethod slot_49 = {"showhide_cosmicsDTDMIN", 0, 0 };
    static const QUMethod slot_50 = {"check_defect_kernelsize", 0, 0 };
    static const QUMethod slot_51 = {"check_filterthreshold", 0, 0 };
    static const QUMethod slot_52 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "show_absphot_readme()", &slot_0, QMetaData::Public },
	{ "show_hide_absphot_wcserror()", &slot_1, QMetaData::Public },
	{ "show_hide_twopass()", &slot_2, QMetaData::Public },
	{ "update_refcoord_textlabel()", &slot_3, QMetaData::Public },
	{ "call_mtype_readme()", &slot_4, QMetaData::Public },
	{ "update_headertextlabel(int)", &slot_5, QMetaData::Public },
	{ "switch_page_next()", &slot_6, QMetaData::Public },
	{ "show_hide_filtercombobox()", &slot_7, QMetaData::Public },
	{ "fill_coadd_filtercombobox()", &slot_8, QMetaData::Public },
	{ "fill_skyposangle()", &slot_9, QMetaData::Public },
	{ "switch_page_back()", &slot_10, QMetaData::Public },
	{ "check_for_reset()", &slot_11, QMetaData::Public },
	{ "populate_absphotmenu()", &slot_12, QMetaData::Public },
	{ "show_photcal1()", &slot_13, QMetaData::Public },
	{ "show_photcal2()", &slot_14, QMetaData::Public },
	{ "show_photcal3()", &slot_15, QMetaData::Public },
	{ "show_photcal4()", &slot_16, QMetaData::Public },
	{ "show_photcal5()", &slot_17, QMetaData::Public },
	{ "show_photcal6()", &slot_18, QMetaData::Public },
	{ "show_photcal7()", &slot_19, QMetaData::Public },
	{ "get_crossidradius(QString&)", &slot_20, QMetaData::Public },
	{ "get_pixscale()", &slot_21, QMetaData::Public },
	{ "get_numchips()", &slot_22, QMetaData::Public },
	{ "update_crossidradius()", &slot_23, QMetaData::Public },
	{ "check_distortion_consistency(int&)", &slot_24, QMetaData::Public },
	{ "parameters_quit()", &slot_25, QMetaData::Public },
	{ "update_stdcolor_comboboxes()", &slot_26, QMetaData::Public },
	{ "check_stripe82()", &slot_27, QMetaData::Public },
	{ "init_stdcolor_comboboxes()", &slot_28, QMetaData::Public },
	{ "update_stdcolorfilter()", &slot_29, QMetaData::Public },
	{ "check_nhigh_window(int&)", &slot_30, QMetaData::Public },
	{ "toggle_statdyn()", &slot_31, QMetaData::Public },
	{ "toggle_globalweight_params()", &slot_32, QMetaData::Public },
	{ "write_parameters()", &slot_33, QMetaData::Public },
	{ "read_parameters()", &slot_34, QMetaData::Public },
	{ "modify_parametersini()", &slot_35, QMetaData::Public },
	{ "get_coadd_id(QString&)", &slot_36, QMetaData::Public },
	{ "defaults()", &slot_37, QMetaData::Public },
	{ "defaults_all()", &slot_38, QMetaData::Public },
	{ "populate_xtalkbuttons()", &slot_39, QMetaData::Public },
	{ "untoggle_xtalkbuttons()", &slot_40, QMetaData::Public },
	{ "link_checkbox_xtalk()", &slot_41, QMetaData::Public },
	{ "link_xtalk_checkbox()", &slot_42, QMetaData::Public },
	{ "active_passive_xtalk()", &slot_43, QMetaData::Public },
	{ "validate()", &slot_44, QMetaData::Public },
	{ "adjust_collcorr_direction()", &slot_45, QMetaData::Public },
	{ "showhide_bloom()", &slot_46, QMetaData::Public },
	{ "showhide_clipped()", &slot_47, QMetaData::Public },
	{ "showhide_defectdetection()", &slot_48, QMetaData::Public },
	{ "showhide_cosmicsDTDMIN()", &slot_49, QMetaData::Public },
	{ "check_defect_kernelsize()", &slot_50, QMetaData::Public },
	{ "check_filterthreshold()", &slot_51, QMetaData::Public },
	{ "languageChange()", &slot_52, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"parameters", parentObject,
	slot_tbl, 53,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_parameters.setMetaObject( metaObj );
    return metaObj;
}

void* parameters::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "parameters" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool parameters::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: show_absphot_readme(); break;
    case 1: show_hide_absphot_wcserror(); break;
    case 2: show_hide_twopass(); break;
    case 3: update_refcoord_textlabel(); break;
    case 4: call_mtype_readme(); break;
    case 5: update_headertextlabel((int)static_QUType_int.get(_o+1)); break;
    case 6: switch_page_next(); break;
    case 7: show_hide_filtercombobox(); break;
    case 8: fill_coadd_filtercombobox(); break;
    case 9: fill_skyposangle(); break;
    case 10: switch_page_back(); break;
    case 11: check_for_reset(); break;
    case 12: populate_absphotmenu(); break;
    case 13: show_photcal1(); break;
    case 14: show_photcal2(); break;
    case 15: show_photcal3(); break;
    case 16: show_photcal4(); break;
    case 17: show_photcal5(); break;
    case 18: show_photcal6(); break;
    case 19: show_photcal7(); break;
    case 20: get_crossidradius((QString&)static_QUType_QString.get(_o+1)); break;
    case 21: get_pixscale(); break;
    case 22: get_numchips(); break;
    case 23: update_crossidradius(); break;
    case 24: check_distortion_consistency((int&)static_QUType_int.get(_o+1)); break;
    case 25: parameters_quit(); break;
    case 26: update_stdcolor_comboboxes(); break;
    case 27: check_stripe82(); break;
    case 28: init_stdcolor_comboboxes(); break;
    case 29: update_stdcolorfilter(); break;
    case 30: check_nhigh_window((int&)static_QUType_int.get(_o+1)); break;
    case 31: toggle_statdyn(); break;
    case 32: toggle_globalweight_params(); break;
    case 33: write_parameters(); break;
    case 34: read_parameters(); break;
    case 35: modify_parametersini(); break;
    case 36: get_coadd_id((QString&)static_QUType_QString.get(_o+1)); break;
    case 37: defaults(); break;
    case 38: defaults_all(); break;
    case 39: populate_xtalkbuttons(); break;
    case 40: untoggle_xtalkbuttons(); break;
    case 41: link_checkbox_xtalk(); break;
    case 42: link_xtalk_checkbox(); break;
    case 43: active_passive_xtalk(); break;
    case 44: validate(); break;
    case 45: adjust_collcorr_direction(); break;
    case 46: showhide_bloom(); break;
    case 47: showhide_clipped(); break;
    case 48: showhide_defectdetection(); break;
    case 49: showhide_cosmicsDTDMIN(); break;
    case 50: check_defect_kernelsize(); break;
    case 51: check_filterthreshold(); break;
    case 52: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool parameters::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool parameters::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool parameters::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
