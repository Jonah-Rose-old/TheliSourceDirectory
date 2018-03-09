/****************************************************************************
** statistics meta object code from reading C++ file 'statistics.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/statistics.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *statistics::className() const
{
    return "statistics";
}

QMetaObject *statistics::metaObj = 0;
static QMetaObjectCleanUp cleanUp_statistics( "statistics", &statistics::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString statistics::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "statistics", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString statistics::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "statistics", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* statistics::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"validate", 0, 0 };
    static const QUMethod slot_1 = {"clearfields", 0, 0 };
    static const QUMethod slot_2 = {"get_statistics", 0, 0 };
    static const QUMethod slot_3 = {"fill_in_table", 0, 0 };
    static const QUMethod slot_4 = {"populate_statisticstable", 0, 0 };
    static const QUMethod slot_5 = {"update_statisticstable", 0, 0 };
    static const QUMethod slot_6 = {"select_dirimage", 0, 0 };
    static const QUMethod slot_7 = {"kill_stats", 0, 0 };
    static const QUMethod slot_8 = {"quit", 0, 0 };
    static const QUMethod slot_9 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "validate()", &slot_0, QMetaData::Public },
	{ "clearfields()", &slot_1, QMetaData::Public },
	{ "get_statistics()", &slot_2, QMetaData::Public },
	{ "fill_in_table()", &slot_3, QMetaData::Public },
	{ "populate_statisticstable()", &slot_4, QMetaData::Public },
	{ "update_statisticstable()", &slot_5, QMetaData::Public },
	{ "select_dirimage()", &slot_6, QMetaData::Public },
	{ "kill_stats()", &slot_7, QMetaData::Public },
	{ "quit()", &slot_8, QMetaData::Public },
	{ "languageChange()", &slot_9, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"statistics", parentObject,
	slot_tbl, 10,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_statistics.setMetaObject( metaObj );
    return metaObj;
}

void* statistics::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "statistics" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool statistics::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: validate(); break;
    case 1: clearfields(); break;
    case 2: get_statistics(); break;
    case 3: fill_in_table(); break;
    case 4: populate_statisticstable(); break;
    case 5: update_statisticstable(); break;
    case 6: select_dirimage(); break;
    case 7: kill_stats(); break;
    case 8: quit(); break;
    case 9: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool statistics::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool statistics::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool statistics::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
