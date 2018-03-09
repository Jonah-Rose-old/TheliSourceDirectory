/****************************************************************************
** Form interface generated from reading ui file 'imalyzer.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IMALYZER_H
#define IMALYZER_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QButtonGroup;
class QRadioButton;

class imalyzer : public QDialog
{
    Q_OBJECT

public:
    imalyzer( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~imalyzer();

    QGroupBox* groupBox5;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QLineEdit* ia_filterLineEdit;
    QPushButton* ia_selectPushButton;
    QLineEdit* ia_dirLineEdit;
    QPushButton* ia_analysePushButton;
    QPushButton* ia_analyseabortPushButton;
    QGroupBox* groupBox22;
    QLabel* ia_analysis1TextLabel;
    QLabel* textLabel1_2;
    QPushButton* ia_makeplotsPushButton;
    QPushButton* ia_makeplotabortPushButton;
    QGroupBox* groupBox3;
    QCheckBox* ia_gridCheckBox;
    QCheckBox* ia_mediancontourlineCheckBox;
    QLabel* textLabel1_6;
    QComboBox* ia_unitsComboBox;
    QLineEdit* ia_titleLineEdit;
    QLabel* textLabel11;
    QButtonGroup* buttonGroup1;
    QLabel* ia_scalemaxTextLabel;
    QLabel* ia_scaleminTextLabel;
    QLineEdit* ia_scalemaxLineEdit;
    QRadioButton* ia_scaleabsoluteRadioButton;
    QLineEdit* ia_scaleminLineEdit;
    QRadioButton* ia_scalefullrangeRadioButton;
    QRadioButton* ia_scalemaxRadioButton;
    QLineEdit* ia_scalecolorrangeLineEdit;
    QGroupBox* groupBox4;
    QCheckBox* ia_fwhmsplinesmoothCheckBox;
    QCheckBox* ia_interpolateanisoCheckBox;
    QPushButton* ia_closePushButton;

public slots:
    virtual void validate();
    virtual void select_dirimage();
    virtual void analyse_images();
    virtual void makeplot_images();
    virtual void ia_analyse_done();
    virtual void ia_makeplot_done();
    virtual void ia_showhide();
    virtual void radioselect();
    virtual void kill_analyse();
    virtual void kill_makeplot();
    virtual void quit();

protected:
    QGridLayout* imalyzerLayout;
    QSpacerItem* spacer5;
    QSpacerItem* spacer4;
    QGridLayout* groupBox5Layout;
    QGridLayout* groupBox22Layout;
    QSpacerItem* spacer7_2;
    QSpacerItem* spacer5_2;
    QGridLayout* groupBox3Layout;
    QGridLayout* buttonGroup1Layout;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // IMALYZER_H
