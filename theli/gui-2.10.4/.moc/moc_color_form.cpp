/****************************************************************************
** color_form meta object code from reading C++ file 'color_form.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/color_form.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *color_form::className() const
{
    return "color_form";
}

QMetaObject *color_form::metaObj = 0;
static QMetaObjectCleanUp cleanUp_color_form( "color_form", &color_form::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString color_form::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "color_form", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString color_form::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "color_form", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* color_form::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"select_dir", 0, 0 };
    static const QUMethod slot_1 = {"get_coadddirs", 0, 0 };
    static const QUMethod slot_2 = {"link_coaddimages", 0, 0 };
    static const QUMethod slot_3 = {"get_colorchannels", 0, 0 };
    static const QUMethod slot_4 = {"validator_main", 0, 0 };
    static const QUMethod slot_5 = {"validator_sec", 0, 0 };
    static const QUMethod slot_6 = {"remove_imagefromlist", 0, 0 };
    static const QUMethod slot_7 = {"flush_table", 0, 0 };
    static const QUMethod slot_8 = {"get_statistics", 0, 0 };
    static const QUParameter param_slot_9[] = {
	{ "numimages", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"update_tiffthresholds", 1, param_slot_9 };
    static const QUMethod slot_10 = {"stiff_defaults", 0, 0 };
    static const QUMethod slot_11 = {"enhance_luminance", 0, 0 };
    static const QUMethod slot_12 = {"fill_in_elum", 0, 0 };
    static const QUMethod slot_13 = {"get_current_imagelist", 0, 0 };
    static const QUMethod slot_14 = {"populate_stiffcombobox", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"write_superscriptbody", 3, param_slot_15 };
    static const QUMethod slot_16 = {"update_cropText", 0, 0 };
    static const QUMethod slot_17 = {"update_combineText", 0, 0 };
    static const QUMethod slot_18 = {"update_colcalibText", 0, 0 };
    static const QUMethod slot_19 = {"update_stiffText", 0, 0 };
    static const QUMethod slot_20 = {"update_fits2tiffText", 0, 0 };
    static const QUMethod slot_21 = {"update_elumText", 0, 0 };
    static const QUMethod slot_22 = {"update_combine_elements", 0, 0 };
    static const QUMethod slot_23 = {"combine_images", 0, 0 };
    static const QUMethod slot_24 = {"show_preview", 0, 0 };
    static const QUMethod slot_25 = {"reset_BVcorr", 0, 0 };
    static const QUMethod slot_26 = {"stiff", 0, 0 };
    static const QUMethod slot_27 = {"get_color_correction", 0, 0 };
    static const QUMethod slot_28 = {"fill_in_colcalib", 0, 0 };
    static const QUParameter param_slot_29[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_29 = {"read_into_struct", 2, param_slot_29 };
    static const QUMethod slot_30 = {"set_toggle_button", 0, 0 };
    static const QUMethod slot_31 = {"fits2tiff", 0, 0 };
    static const QUMethod slot_32 = {"get_maxoverlap", 0, 0 };
    static const QUMethod slot_33 = {"update_colordirpath", 0, 0 };
    static const QUMethod slot_34 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "select_dir()", &slot_0, QMetaData::Public },
	{ "get_coadddirs()", &slot_1, QMetaData::Public },
	{ "link_coaddimages()", &slot_2, QMetaData::Public },
	{ "get_colorchannels()", &slot_3, QMetaData::Public },
	{ "validator_main()", &slot_4, QMetaData::Public },
	{ "validator_sec()", &slot_5, QMetaData::Public },
	{ "remove_imagefromlist()", &slot_6, QMetaData::Public },
	{ "flush_table()", &slot_7, QMetaData::Public },
	{ "get_statistics()", &slot_8, QMetaData::Public },
	{ "update_tiffthresholds(int)", &slot_9, QMetaData::Public },
	{ "stiff_defaults()", &slot_10, QMetaData::Public },
	{ "enhance_luminance()", &slot_11, QMetaData::Public },
	{ "fill_in_elum()", &slot_12, QMetaData::Public },
	{ "get_current_imagelist()", &slot_13, QMetaData::Public },
	{ "populate_stiffcombobox()", &slot_14, QMetaData::Public },
	{ "write_superscriptbody(QString,QString,QString)", &slot_15, QMetaData::Public },
	{ "update_cropText()", &slot_16, QMetaData::Public },
	{ "update_combineText()", &slot_17, QMetaData::Public },
	{ "update_colcalibText()", &slot_18, QMetaData::Public },
	{ "update_stiffText()", &slot_19, QMetaData::Public },
	{ "update_fits2tiffText()", &slot_20, QMetaData::Public },
	{ "update_elumText()", &slot_21, QMetaData::Public },
	{ "update_combine_elements()", &slot_22, QMetaData::Public },
	{ "combine_images()", &slot_23, QMetaData::Public },
	{ "show_preview()", &slot_24, QMetaData::Public },
	{ "reset_BVcorr()", &slot_25, QMetaData::Public },
	{ "stiff()", &slot_26, QMetaData::Public },
	{ "get_color_correction()", &slot_27, QMetaData::Public },
	{ "fill_in_colcalib()", &slot_28, QMetaData::Public },
	{ "read_into_struct(QString,int)", &slot_29, QMetaData::Public },
	{ "set_toggle_button()", &slot_30, QMetaData::Public },
	{ "fits2tiff()", &slot_31, QMetaData::Public },
	{ "get_maxoverlap()", &slot_32, QMetaData::Public },
	{ "update_colordirpath()", &slot_33, QMetaData::Public },
	{ "languageChange()", &slot_34, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"color_form", parentObject,
	slot_tbl, 35,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_color_form.setMetaObject( metaObj );
    return metaObj;
}

void* color_form::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "color_form" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool color_form::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: select_dir(); break;
    case 1: get_coadddirs(); break;
    case 2: link_coaddimages(); break;
    case 3: get_colorchannels(); break;
    case 4: validator_main(); break;
    case 5: validator_sec(); break;
    case 6: remove_imagefromlist(); break;
    case 7: flush_table(); break;
    case 8: get_statistics(); break;
    case 9: update_tiffthresholds((int)static_QUType_int.get(_o+1)); break;
    case 10: stiff_defaults(); break;
    case 11: enhance_luminance(); break;
    case 12: fill_in_elum(); break;
    case 13: get_current_imagelist(); break;
    case 14: populate_stiffcombobox(); break;
    case 15: write_superscriptbody((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 16: update_cropText(); break;
    case 17: update_combineText(); break;
    case 18: update_colcalibText(); break;
    case 19: update_stiffText(); break;
    case 20: update_fits2tiffText(); break;
    case 21: update_elumText(); break;
    case 22: update_combine_elements(); break;
    case 23: combine_images(); break;
    case 24: show_preview(); break;
    case 25: reset_BVcorr(); break;
    case 26: stiff(); break;
    case 27: get_color_correction(); break;
    case 28: fill_in_colcalib(); break;
    case 29: read_into_struct((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 30: set_toggle_button(); break;
    case 31: fits2tiff(); break;
    case 32: get_maxoverlap(); break;
    case 33: update_colordirpath(); break;
    case 34: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool color_form::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool color_form::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool color_form::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
