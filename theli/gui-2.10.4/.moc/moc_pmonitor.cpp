/****************************************************************************
** pmonitor meta object code from reading C++ file 'pmonitor.h'
**
** Created: Fri Feb 2 16:30:22 2018
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/pmonitor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *pmonitor::className() const
{
    return "pmonitor";
}

QMetaObject *pmonitor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_pmonitor( "pmonitor", &pmonitor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString pmonitor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "pmonitor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString pmonitor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "pmonitor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* pmonitor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"p_start", 0, 0 };
    static const QUMethod slot_1 = {"p_reset", 0, 0 };
    static const QUMethod slot_2 = {"timer_done", 0, 0 };
    static const QUMethod slot_3 = {"show_fits", 0, 0 };
    static const QUMethod slot_4 = {"show_plots", 0, 0 };
    static const QUMethod slot_5 = {"show_stats", 0, 0 };
    static const QUMethod slot_6 = {"show_protocol", 0, 0 };
    static const QUMethod slot_7 = {"btn_cor_NOMAD_clicked", 0, 0 };
    static const QUMethod slot_8 = {"newStamp", 0, 0 };
    static const QUMethod slot_9 = {"btn_service_clicked", 0, 0 };
    static const QUMethod slot_10 = {"btn_show_Bayer_clicked", 0, 0 };
    static const QUMethod slot_11 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "p_start()", &slot_0, QMetaData::Public },
	{ "p_reset()", &slot_1, QMetaData::Public },
	{ "timer_done()", &slot_2, QMetaData::Public },
	{ "show_fits()", &slot_3, QMetaData::Public },
	{ "show_plots()", &slot_4, QMetaData::Public },
	{ "show_stats()", &slot_5, QMetaData::Public },
	{ "show_protocol()", &slot_6, QMetaData::Public },
	{ "btn_cor_NOMAD_clicked()", &slot_7, QMetaData::Public },
	{ "newStamp()", &slot_8, QMetaData::Public },
	{ "btn_service_clicked()", &slot_9, QMetaData::Public },
	{ "btn_show_Bayer_clicked()", &slot_10, QMetaData::Public },
	{ "languageChange()", &slot_11, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"pmonitor", parentObject,
	slot_tbl, 12,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_pmonitor.setMetaObject( metaObj );
    return metaObj;
}

void* pmonitor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "pmonitor" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool pmonitor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: p_start(); break;
    case 1: p_reset(); break;
    case 2: timer_done(); break;
    case 3: show_fits(); break;
    case 4: show_plots(); break;
    case 5: show_stats(); break;
    case 6: show_protocol(); break;
    case 7: btn_cor_NOMAD_clicked(); break;
    case 8: newStamp(); break;
    case 9: btn_service_clicked(); break;
    case 10: btn_show_Bayer_clicked(); break;
    case 11: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool pmonitor::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool pmonitor::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool pmonitor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
