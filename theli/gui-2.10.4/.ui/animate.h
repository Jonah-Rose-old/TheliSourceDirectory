/****************************************************************************
** Form interface generated from reading ui file 'animate.ui'
**
** Created: Fri Feb 2 16:30:21 2018
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ANIMATE_H
#define ANIMATE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QSpinBox;
class QLabel;
class QListBox;
class QListBoxItem;
class QComboBox;
class QPushButton;
class QLineEdit;
class QCheckBox;

class animate : public QDialog
{
    Q_OBJECT

public:
    animate( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~animate();

    QGroupBox* groupBox5_3;
    QSpinBox* animate_overlapSpinBox;
    QLabel* textLabel11;
    QSpinBox* animate_divySpinBox;
    QLabel* textLabel10_2;
    QListBox* animate_coaddListBox;
    QLabel* textLabel1_7;
    QSpinBox* animate_divxSpinBox;
    QLabel* textLabel2_3;
    QLabel* textLabel3_2;
    QListBox* animate_divListBox;
    QGroupBox* groupBox3;
    QLabel* animate_chipTextLabel;
    QComboBox* animate_chipComboBox;
    QPushButton* animate_animatePushButton;
    QPushButton* animate_createPushButton;
    QSpinBox* animate_delaySpinBox;
    QComboBox* animate_rangeComboBox;
    QSpinBox* animate_contrastSpinBox;
    QLineEdit* animate_TIFFminLineEdit;
    QCheckBox* animate_maskCheckBox;
    QLabel* animate_contrastTextLabel;
    QLabel* animate_minmaxTextLabel;
    QLineEdit* animate_TIFFmaxLineEdit;
    QLabel* textLabel1_8;
    QLabel* textLabel4;
    QGroupBox* groupBox9;
    QPushButton* animate_selectdirPushButton;
    QLabel* textLabel1;
    QLineEdit* animate_dirLineEdit;
    QPushButton* closePushButton;

public slots:
    virtual void select_dir();
    virtual void update_chipnum();
    virtual void fill_in_moviedivisions();
    virtual void setstate_manualthresholds();
    virtual void make_movie_step1();
    virtual void make_movie_step2();
    virtual void animate_division();
    virtual void update_animatebuttontext();
    virtual void get_coadddirs();
    virtual void validator_main();
    virtual void validator_sec();
    virtual void write_superscriptbody_step1( QString superscript, QString command);
    virtual void write_superscriptbody_step2( QString superscript, QString command);
    virtual void update_movieText();
    virtual void kill_process();
    virtual void quit();

protected:
    QGridLayout* animateLayout;
    QSpacerItem* spacer42_2;
    QGridLayout* groupBox5_3Layout;
    QGridLayout* groupBox3Layout;
    QSpacerItem* spacer12_2;
    QSpacerItem* spacer28_2;
    QSpacerItem* spacer14;
    QSpacerItem* spacer11;
    QGridLayout* layout1;
    QSpacerItem* spacer19;
    QSpacerItem* spacer12;
    QSpacerItem* spacer15;
    QGridLayout* groupBox9Layout;
    QSpacerItem* spacer42;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // ANIMATE_H
