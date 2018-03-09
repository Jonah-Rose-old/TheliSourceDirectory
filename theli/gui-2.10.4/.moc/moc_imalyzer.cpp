/****************************************************************************
** imalyzer meta object code from reading C++ file 'imalyzer.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/imalyzer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *imalyzer::className() const
{
    return "imalyzer";
}

QMetaObject *imalyzer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_imalyzer( "imalyzer", &imalyzer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString imalyzer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "imalyzer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString imalyzer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "imalyzer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* imalyzer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"validate", 0, 0 };
    static const QUMethod slot_1 = {"select_dirimage", 0, 0 };
    static const QUMethod slot_2 = {"analyse_images", 0, 0 };
    static const QUMethod slot_3 = {"makeplot_images", 0, 0 };
    static const QUMethod slot_4 = {"ia_analyse_done", 0, 0 };
    static const QUMethod slot_5 = {"ia_makeplot_done", 0, 0 };
    static const QUMethod slot_6 = {"ia_showhide", 0, 0 };
    static const QUMethod slot_7 = {"radioselect", 0, 0 };
    static const QUMethod slot_8 = {"kill_analyse", 0, 0 };
    static const QUMethod slot_9 = {"kill_makeplot", 0, 0 };
    static const QUMethod slot_10 = {"quit", 0, 0 };
    static const QUMethod slot_11 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "validate()", &slot_0, QMetaData::Public },
	{ "select_dirimage()", &slot_1, QMetaData::Public },
	{ "analyse_images()", &slot_2, QMetaData::Public },
	{ "makeplot_images()", &slot_3, QMetaData::Public },
	{ "ia_analyse_done()", &slot_4, QMetaData::Public },
	{ "ia_makeplot_done()", &slot_5, QMetaData::Public },
	{ "ia_showhide()", &slot_6, QMetaData::Public },
	{ "radioselect()", &slot_7, QMetaData::Public },
	{ "kill_analyse()", &slot_8, QMetaData::Public },
	{ "kill_makeplot()", &slot_9, QMetaData::Public },
	{ "quit()", &slot_10, QMetaData::Public },
	{ "languageChange()", &slot_11, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"imalyzer", parentObject,
	slot_tbl, 12,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_imalyzer.setMetaObject( metaObj );
    return metaObj;
}

void* imalyzer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "imalyzer" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool imalyzer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: validate(); break;
    case 1: select_dirimage(); break;
    case 2: analyse_images(); break;
    case 3: makeplot_images(); break;
    case 4: ia_analyse_done(); break;
    case 5: ia_makeplot_done(); break;
    case 6: ia_showhide(); break;
    case 7: radioselect(); break;
    case 8: kill_analyse(); break;
    case 9: kill_makeplot(); break;
    case 10: quit(); break;
    case 11: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool imalyzer::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool imalyzer::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool imalyzer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
