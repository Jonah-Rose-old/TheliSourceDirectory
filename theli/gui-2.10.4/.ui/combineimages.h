/****************************************************************************
** Form interface generated from reading ui file 'combineimages.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef COMBINEIMAGES_H
#define COMBINEIMAGES_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class QCheckBox;

class combineimages : public QDialog
{
    Q_OBJECT

public:
    combineimages( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~combineimages();

    QGroupBox* groupBox2;
    QPushButton* combima_closePushButton;
    QPushButton* combima_clearPushButton;
    QPushButton* combima_abortPushButton;
    QLineEdit* combima_statusLineEdit;
    QPushButton* combima_startPushButton;
    QLabel* textLabel3;
    QGroupBox* groupBox1;
    QLabel* textLabel1_2;
    QLabel* textLabel1_3;
    QLabel* textLabel1;
    QLineEdit* combima_dirLineEdit;
    QLineEdit* combima_filterLineEdit;
    QLineEdit* combima_nameLineEdit;
    QLineEdit* combima_dtLineEdit;
    QLineEdit* combima_dminLineEdit;
    QLabel* textLabel1_6;
    QLabel* textLabel2_2;
    QLineEdit* combima_nhighLineEdit;
    QLineEdit* combima_nlowLineEdit;
    QLabel* textLabel1_4;
    QLabel* textLabel1_5;
    QComboBox* combima_methodComboBox;
    QCheckBox* combima_rescaleCheckBox;
    QLineEdit* combima_levelLineEdit;

public slots:
    virtual void validate();
    virtual void clearfields();
    virtual void compile_scriptsyntax();
    virtual void write_superscriptbody( QString fullpath, QString scripts );
    virtual void updateText();
    virtual void updateExit();
    virtual void show_scriptlog( QString errormessage );
    virtual void kill_combineimage();
    virtual void quit();

protected:
    QGridLayout* combineimagesLayout;
    QSpacerItem* spacer18;
    QGridLayout* groupBox2Layout;
    QSpacerItem* spacer21_2;
    QGridLayout* groupBox1Layout;
    QSpacerItem* spacer1_2;
    QSpacerItem* spacer1;
    QSpacerItem* spacer10;
    QSpacerItem* spacer1_3;
    QSpacerItem* spacer10_3;
    QSpacerItem* spacer1_5;
    QSpacerItem* spacer8_2;
    QSpacerItem* spacer8_2_2;
    QHBoxLayout* layout29;
    QSpacerItem* spacer54;
    QGridLayout* layout28;
    QSpacerItem* spacer53;
    QHBoxLayout* layout26;
    QSpacerItem* spacer52;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // COMBINEIMAGES_H
