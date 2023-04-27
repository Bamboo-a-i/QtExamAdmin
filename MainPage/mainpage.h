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

    void tBtnPageClick();

    void tBtnTabClick();

    void on_tabQueryCenter_tabBarClicked(int index);

    void on_tBtnAllExam_clicked();

    void on_tBtnCreateExam_clicked();

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
