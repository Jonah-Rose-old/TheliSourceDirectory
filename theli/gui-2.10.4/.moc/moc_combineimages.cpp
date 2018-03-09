/****************************************************************************
** combineimages meta object code from reading C++ file 'combineimages.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/combineimages.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *combineimages::className() const
{
    return "combineimages";
}

QMetaObject *combineimages::metaObj = 0;
static QMetaObjectCleanUp cleanUp_combineimages( "combineimages", &combineimages::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString combineimages::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "combineimages", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString combineimages::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "combineimages", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* combineimages::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"validate", 0, 0 };
    static const QUMethod slot_1 = {"clearfields", 0, 0 };
    static const QUMethod slot_2 = {"compile_scriptsyntax", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "fullpath", &static_QUType_QString, 0, QUParameter::In },
	{ "scripts", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"write_superscriptbody", 2, param_slot_3 };
    static const QUMethod slot_4 = {"updateText", 0, 0 };
    static const QUMethod slot_5 = {"updateExit", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "errormessage", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"show_scriptlog", 1, param_slot_6 };
    static const QUMethod slot_7 = {"kill_combineimage", 0, 0 };
    static const QUMethod slot_8 = {"quit", 0, 0 };
    static const QUMethod slot_9 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "validate()", &slot_0, QMetaData::Public },
	{ "clearfields()", &slot_1, QMetaData::Public },
	{ "compile_scriptsyntax()", &slot_2, QMetaData::Public },
	{ "write_superscriptbody(QString,QString)", &slot_3, QMetaData::Public },
	{ "updateText()", &slot_4, QMetaData::Public },
	{ "updateExit()", &slot_5, QMetaData::Public },
	{ "show_scriptlog(QString)", &slot_6, QMetaData::Public },
	{ "kill_combineimage()", &slot_7, QMetaData::Public },
	{ "quit()", &slot_8, QMetaData::Public },
	{ "languageChange()", &slot_9, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"combineimages", parentObject,
	slot_tbl, 10,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_combineimages.setMetaObject( metaObj );
    return metaObj;
}

void* combineimages::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "combineimages" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool combineimages::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: validate(); break;
    case 1: clearfields(); break;
    case 2: compile_scriptsyntax(); break;
    case 3: write_superscriptbody((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 4: updateText(); break;
    case 5: updateExit(); break;
    case 6: show_scriptlog((QString)static_QUType_QString.get(_o+1)); break;
    case 7: kill_combineimage(); break;
    case 8: quit(); break;
    case 9: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool combineimages::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool combineimages::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool combineimages::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
