#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QMainWindow>
#include "formcollege.h"
#include "formclass.h"
#include "formmajor.h"
#include "formcurriculum.h"
#include "formstudent.h"

namespace Ui {
class MainPage;
}

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

private slots:
    void initPage();

    void buttonClick();

    void on_btnCollegeData_clicked();

    void on_btnClassData_clicked();

    void on_btnCurriculumData_clicked();

    void on_btnMajorData_clicked();

    void on_tabQueryCenter_tabBarClicked(int index);

    void on_btnStudentData_clicked();

    void on_tBtnAllExam_clicked();

    void on_tBtnCreateExam_clicked();

    void on_tBtn1_clicked();

private:
    Ui::MainPage *ui;
    // page2
    FormCollege *formCollege;
    FormMajor *formMajor;
    FormClass *formClass;
    FormCurriculum *formCurriculum;

    // page4
    FormStudent *fromStudent;

};

#endif // MAINPAGE_H
