/****************************************************************************
** manualsky meta object code from reading C++ file 'manualsky.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/manualsky.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *manualsky::className() const
{
    return "manualsky";
}

QMetaObject *manualsky::metaObj = 0;
static QMetaObjectCleanUp cleanUp_manualsky( "manualsky", &manualsky::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString manualsky::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "manualsky", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString manualsky::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "manualsky", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* manualsky::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"validate", 0, 0 };
    static const QUMethod slot_1 = {"clearfields", 0, 0 };
    static const QUMethod slot_2 = {"showhide_rows", 0, 0 };
    static const QUMethod slot_3 = {"update_table", 0, 0 };
    static const QUMethod slot_4 = {"get_manualsky", 0, 0 };
    static const QUMethod slot_5 = {"load_manualsky", 0, 0 };
    static const QUMethod slot_6 = {"fill_manualvalue", 0, 0 };
    static const QUMethod slot_7 = {"save_quit", 0, 0 };
    static const QUMethod slot_8 = {"save_fullstatistics", 0, 0 };
    static const QUMethod slot_9 = {"save_wysiwyg", 0, 0 };
    static const QUMethod slot_10 = {"populate_manualskytable", 0, 0 };
    static const QUMethod slot_11 = {"write_parameters", 0, 0 };
    static const QUMethod slot_12 = {"read_parameters", 0, 0 };
    static const QUMethod slot_13 = {"modify_parametersini", 0, 0 };
    static const QUMethod slot_14 = {"defaults", 0, 0 };
    static const QUMethod slot_15 = {"showhide_ccdselection", 0, 0 };
    static const QUMethod slot_16 = {"switch_selection", 0, 0 };
    static const QUMethod slot_17 = {"show_info", 0, 0 };
    static const QUMethod slot_18 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "validate()", &slot_0, QMetaData::Public },
	{ "clearfields()", &slot_1, QMetaData::Public },
	{ "showhide_rows()", &slot_2, QMetaData::Public },
	{ "update_table()", &slot_3, QMetaData::Public },
	{ "get_manualsky()", &slot_4, QMetaData::Public },
	{ "load_manualsky()", &slot_5, QMetaData::Public },
	{ "fill_manualvalue()", &slot_6, QMetaData::Public },
	{ "save_quit()", &slot_7, QMetaData::Public },
	{ "save_fullstatistics()", &slot_8, QMetaData::Public },
	{ "save_wysiwyg()", &slot_9, QMetaData::Public },
	{ "populate_manualskytable()", &slot_10, QMetaData::Public },
	{ "write_parameters()", &slot_11, QMetaData::Public },
	{ "read_parameters()", &slot_12, QMetaData::Public },
	{ "modify_parametersini()", &slot_13, QMetaData::Public },
	{ "defaults()", &slot_14, QMetaData::Public },
	{ "showhide_ccdselection()", &slot_15, QMetaData::Public },
	{ "switch_selection()", &slot_16, QMetaData::Public },
	{ "show_info()", &slot_17, QMetaData::Public },
	{ "languageChange()", &slot_18, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"manualsky", parentObject,
	slot_tbl, 19,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_manualsky.setMetaObject( metaObj );
    return metaObj;
}

void* manualsky::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "manualsky" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool manualsky::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: validate(); break;
    case 1: clearfields(); break;
    case 2: showhide_rows(); break;
    case 3: update_table(); break;
    case 4: get_manualsky(); break;
    case 5: load_manualsky(); break;
    case 6: fill_manualvalue(); break;
    case 7: save_quit(); break;
    case 8: save_fullstatistics(); break;
    case 9: save_wysiwyg(); break;
    case 10: populate_manualskytable(); break;
    case 11: write_parameters(); break;
    case 12: read_parameters(); break;
    case 13: modify_parametersini(); break;
    case 14: defaults(); break;
    case 15: showhide_ccdselection(); break;
    case 16: switch_selection(); break;
    case 17: show_info(); break;
    case 18: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool manualsky::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool manualsky::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool manualsky::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
