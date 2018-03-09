/****************************************************************************
** zp meta object code from reading C++ file 'zp.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/zp.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *zp::className() const
{
    return "zp";
}

QMetaObject *zp::metaObj = 0;
static QMetaObjectCleanUp cleanUp_zp( "zp", &zp::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString zp::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "zp", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString zp::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "zp", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* zp::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"validate", 0, 0 };
    static const QUMethod slot_1 = {"get_zp", 0, 0 };
    static const QUMethod slot_2 = {"show_hide", 0, 0 };
    static const QUMethod slot_3 = {"select_image", 0, 0 };
    static const QUMethod slot_4 = {"update_filter_comboboxes", 0, 0 };
    static const QUMethod slot_5 = {"fill_in_zp", 0, 0 };
    static const QUMethod slot_6 = {"kill_process", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out }
    };
    static const QUMethod slot_7 = {"scan_for_errors", 1, param_slot_7 };
    static const QUMethod slot_8 = {"quit", 0, 0 };
    static const QUMethod slot_9 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "validate()", &slot_0, QMetaData::Public },
	{ "get_zp()", &slot_1, QMetaData::Public },
	{ "show_hide()", &slot_2, QMetaData::Public },
	{ "select_image()", &slot_3, QMetaData::Public },
	{ "update_filter_comboboxes()", &slot_4, QMetaData::Public },
	{ "fill_in_zp()", &slot_5, QMetaData::Public },
	{ "kill_process()", &slot_6, QMetaData::Public },
	{ "scan_for_errors()", &slot_7, QMetaData::Public },
	{ "quit()", &slot_8, QMetaData::Public },
	{ "languageChange()", &slot_9, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"zp", parentObject,
	slot_tbl, 10,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_zp.setMetaObject( metaObj );
    return metaObj;
}

void* zp::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "zp" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool zp::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: validate(); break;
    case 1: get_zp(); break;
    case 2: show_hide(); break;
    case 3: select_image(); break;
    case 4: update_filter_comboboxes(); break;
    case 5: fill_in_zp(); break;
    case 6: kill_process(); break;
    case 7: static_QUType_int.set(_o,scan_for_errors()); break;
    case 8: quit(); break;
    case 9: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool zp::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool zp::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool zp::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
