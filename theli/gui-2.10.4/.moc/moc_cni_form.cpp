/****************************************************************************
** cni_form meta object code from reading C++ file 'cni_form.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/cni_form.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *cni_form::className() const
{
    return "cni_form";
}

QMetaObject *cni_form::metaObj = 0;
static QMetaObjectCleanUp cleanUp_cni_form( "cni_form", &cni_form::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString cni_form::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "cni_form", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString cni_form::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "cni_form", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* cni_form::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"populate_bayerbuttons", 0, 0 };
    static const QUMethod slot_1 = {"untoggle_bayerbuttons", 0, 0 };
    static const QUMethod slot_2 = {"active_passive_bayer", 0, 0 };
    static const QUMethod slot_3 = {"showhide_bayerpattern", 0, 0 };
    static const QUMethod slot_4 = {"showhide_bayerframe", 0, 0 };
    static const QUMethod slot_5 = {"validate", 0, 0 };
    static const QUMethod slot_6 = {"clearfields", 0, 0 };
    static const QUMethod slot_7 = {"count_ccds_entered", 0, 0 };
    static const QUMethod slot_8 = {"fill_in_presentinst", 0, 0 };
    static const QUMethod slot_9 = {"fill_in_WFI_example", 0, 0 };
    static const QUMethod slot_10 = {"fill_in_WFC_example", 0, 0 };
    static const QUMethod slot_11 = {"fill_in_ST10_example", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "conffile", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"read_conffile", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_varptr, "\x03", QUParameter::In }
    };
    static const QUMethod slot_13 = {"get_inst_parameter", 3, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "file", &static_QUType_QString, 0, QUParameter::In },
	{ "key", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"get_fileparameter", 2, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "ccdgeometry", &static_QUType_QString, 0, QUParameter::In },
	{ "numccd", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_15 = {"get_ccdnumber", 2, param_slot_15 };
    static const QUMethod slot_16 = {"compare_nchips_nccd", 0, 0 };
    static const QUMethod slot_17 = {"load_instconfig", 0, 0 };
    static const QUMethod slot_18 = {"save_instconfig", 0, 0 };
    static const QUMethod slot_19 = {"timerSavebuttonDone", 0, 0 };
    static const QUParameter param_slot_20[] = {
	{ "of", &static_QUType_ptr, "FILE", QUParameter::In },
	{ "le", &static_QUType_ptr, "QLineEdit", QUParameter::In },
	{ "lead", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"save_instconfig_helper", 3, param_slot_20 };
    static const QUMethod slot_21 = {"reset_nccdspinbox", 0, 0 };
    static const QUMethod slot_22 = {"ssd_set_instrumentname", 0, 0 };
    static const QUMethod slot_23 = {"ssd_paint_instrumentlineedit", 0, 0 };
    static const QUMethod slot_24 = {"ssd_instfiledialog", 0, 0 };
    static const QUMethod slot_25 = {"ssd_verify_input", 0, 0 };
    static const QUMethod slot_26 = {"ssd_countccds", 0, 0 };
    static const QUMethod slot_27 = {"write_splitscript", 0, 0 };
    static const QUMethod slot_28 = {"timerDone", 0, 0 };
    static const QUParameter param_slot_29[] = {
	{ "output", &static_QUType_ptr, "FILE", QUParameter::In },
	{ "theliheader", &static_QUType_QString, 0, QUParameter::In },
	{ "rawdummy", &static_QUType_QString, 0, QUParameter::In },
	{ "dummykey", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_29 = {"insert_replacekey", 4, param_slot_29 };
    static const QUMethod slot_30 = {"guess_camerageometry", 0, 0 };
    static const QUMethod slot_31 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "populate_bayerbuttons()", &slot_0, QMetaData::Public },
	{ "untoggle_bayerbuttons()", &slot_1, QMetaData::Public },
	{ "active_passive_bayer()", &slot_2, QMetaData::Public },
	{ "showhide_bayerpattern()", &slot_3, QMetaData::Public },
	{ "showhide_bayerframe()", &slot_4, QMetaData::Public },
	{ "validate()", &slot_5, QMetaData::Public },
	{ "clearfields()", &slot_6, QMetaData::Public },
	{ "count_ccds_entered()", &slot_7, QMetaData::Public },
	{ "fill_in_presentinst()", &slot_8, QMetaData::Public },
	{ "fill_in_WFI_example()", &slot_9, QMetaData::Public },
	{ "fill_in_WFC_example()", &slot_10, QMetaData::Public },
	{ "fill_in_ST10_example()", &slot_11, QMetaData::Public },
	{ "read_conffile(QString)", &slot_12, QMetaData::Public },
	{ "get_inst_parameter(QString,QString,QString*)", &slot_13, QMetaData::Public },
	{ "get_fileparameter(QString,QString)", &slot_14, QMetaData::Public },
	{ "get_ccdnumber(QString,QString&)", &slot_15, QMetaData::Public },
	{ "compare_nchips_nccd()", &slot_16, QMetaData::Public },
	{ "load_instconfig()", &slot_17, QMetaData::Public },
	{ "save_instconfig()", &slot_18, QMetaData::Public },
	{ "timerSavebuttonDone()", &slot_19, QMetaData::Public },
	{ "save_instconfig_helper(FILE*,QLineEdit*,QString)", &slot_20, QMetaData::Public },
	{ "reset_nccdspinbox()", &slot_21, QMetaData::Public },
	{ "ssd_set_instrumentname()", &slot_22, QMetaData::Public },
	{ "ssd_paint_instrumentlineedit()", &slot_23, QMetaData::Public },
	{ "ssd_instfiledialog()", &slot_24, QMetaData::Public },
	{ "ssd_verify_input()", &slot_25, QMetaData::Public },
	{ "ssd_countccds()", &slot_26, QMetaData::Public },
	{ "write_splitscript()", &slot_27, QMetaData::Public },
	{ "timerDone()", &slot_28, QMetaData::Public },
	{ "insert_replacekey(FILE*,QString,QString,QString)", &slot_29, QMetaData::Public },
	{ "guess_camerageometry()", &slot_30, QMetaData::Public },
	{ "languageChange()", &slot_31, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"cni_form", parentObject,
	slot_tbl, 32,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_cni_form.setMetaObject( metaObj );
    return metaObj;
}

void* cni_form::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "cni_form" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool cni_form::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: populate_bayerbuttons(); break;
    case 1: untoggle_bayerbuttons(); break;
    case 2: active_passive_bayer(); break;
    case 3: showhide_bayerpattern(); break;
    case 4: showhide_bayerframe(); break;
    case 5: validate(); break;
    case 6: clearfields(); break;
    case 7: count_ccds_entered(); break;
    case 8: fill_in_presentinst(); break;
    case 9: fill_in_WFI_example(); break;
    case 10: fill_in_WFC_example(); break;
    case 11: fill_in_ST10_example(); break;
    case 12: read_conffile((QString)static_QUType_QString.get(_o+1)); break;
    case 13: get_inst_parameter((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString*)static_QUType_varptr.get(_o+3)); break;
    case 14: get_fileparameter((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 15: get_ccdnumber((QString)static_QUType_QString.get(_o+1),(QString&)static_QUType_QString.get(_o+2)); break;
    case 16: compare_nchips_nccd(); break;
    case 17: load_instconfig(); break;
    case 18: save_instconfig(); break;
    case 19: timerSavebuttonDone(); break;
    case 20: save_instconfig_helper((FILE*)static_QUType_ptr.get(_o+1),(QLineEdit*)static_QUType_ptr.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 21: reset_nccdspinbox(); break;
    case 22: ssd_set_instrumentname(); break;
    case 23: ssd_paint_instrumentlineedit(); break;
    case 24: ssd_instfiledialog(); break;
    case 25: ssd_verify_input(); break;
    case 26: ssd_countccds(); break;
    case 27: write_splitscript(); break;
    case 28: timerDone(); break;
    case 29: insert_replacekey((FILE*)static_QUType_ptr.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3),(QString)static_QUType_QString.get(_o+4)); break;
    case 30: guess_camerageometry(); break;
    case 31: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool cni_form::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool cni_form::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool cni_form::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
