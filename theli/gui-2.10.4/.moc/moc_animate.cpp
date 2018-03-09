/****************************************************************************
** animate meta object code from reading C++ file 'animate.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/animate.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *animate::className() const
{
    return "animate";
}

QMetaObject *animate::metaObj = 0;
static QMetaObjectCleanUp cleanUp_animate( "animate", &animate::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString animate::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "animate", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString animate::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "animate", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* animate::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"select_dir", 0, 0 };
    static const QUMethod slot_1 = {"update_chipnum", 0, 0 };
    static const QUMethod slot_2 = {"fill_in_moviedivisions", 0, 0 };
    static const QUMethod slot_3 = {"setstate_manualthresholds", 0, 0 };
    static const QUMethod slot_4 = {"make_movie_step1", 0, 0 };
    static const QUMethod slot_5 = {"make_movie_step2", 0, 0 };
    static const QUMethod slot_6 = {"animate_division", 0, 0 };
    static const QUMethod slot_7 = {"update_animatebuttontext", 0, 0 };
    static const QUMethod slot_8 = {"get_coadddirs", 0, 0 };
    static const QUMethod slot_9 = {"validator_main", 0, 0 };
    static const QUMethod slot_10 = {"validator_sec", 0, 0 };
    static const QUParameter param_slot_11[] = {
	{ "superscript", &static_QUType_QString, 0, QUParameter::In },
	{ "command", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"write_superscriptbody_step1", 2, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "superscript", &static_QUType_QString, 0, QUParameter::In },
	{ "command", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"write_superscriptbody_step2", 2, param_slot_12 };
    static const QUMethod slot_13 = {"update_movieText", 0, 0 };
    static const QUMethod slot_14 = {"kill_process", 0, 0 };
    static const QUMethod slot_15 = {"quit", 0, 0 };
    static const QUMethod slot_16 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "select_dir()", &slot_0, QMetaData::Public },
	{ "update_chipnum()", &slot_1, QMetaData::Public },
	{ "fill_in_moviedivisions()", &slot_2, QMetaData::Public },
	{ "setstate_manualthresholds()", &slot_3, QMetaData::Public },
	{ "make_movie_step1()", &slot_4, QMetaData::Public },
	{ "make_movie_step2()", &slot_5, QMetaData::Public },
	{ "animate_division()", &slot_6, QMetaData::Public },
	{ "update_animatebuttontext()", &slot_7, QMetaData::Public },
	{ "get_coadddirs()", &slot_8, QMetaData::Public },
	{ "validator_main()", &slot_9, QMetaData::Public },
	{ "validator_sec()", &slot_10, QMetaData::Public },
	{ "write_superscriptbody_step1(QString,QString)", &slot_11, QMetaData::Public },
	{ "write_superscriptbody_step2(QString,QString)", &slot_12, QMetaData::Public },
	{ "update_movieText()", &slot_13, QMetaData::Public },
	{ "kill_process()", &slot_14, QMetaData::Public },
	{ "quit()", &slot_15, QMetaData::Public },
	{ "languageChange()", &slot_16, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"animate", parentObject,
	slot_tbl, 17,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_animate.setMetaObject( metaObj );
    return metaObj;
}

void* animate::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "animate" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool animate::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: select_dir(); break;
    case 1: update_chipnum(); break;
    case 2: fill_in_moviedivisions(); break;
    case 3: setstate_manualthresholds(); break;
    case 4: make_movie_step1(); break;
    case 5: make_movie_step2(); break;
    case 6: animate_division(); break;
    case 7: update_animatebuttontext(); break;
    case 8: get_coadddirs(); break;
    case 9: validator_main(); break;
    case 10: validator_sec(); break;
    case 11: write_superscriptbody_step1((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 12: write_superscriptbody_step2((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 13: update_movieText(); break;
    case 14: kill_process(); break;
    case 15: quit(); break;
    case 16: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool animate::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool animate::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool animate::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
