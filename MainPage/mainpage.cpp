#include "mainpage.h"
#include "ui_mainpage.h"
#include "formcreateexam.h"
#include <QDebug>

MainPage::MainPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    initPage();
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::initPage()
{
    //设置顶部导航按钮
    QList<QAbstractButton *> tbtns = ui->widgetTop->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *btn, tbtns) {
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    }
    ui->tbtn1->click(); // 默认主页面

    // page2
    formCollege = new FormCollege(this);
    formCollege->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    ui->tabQueryCenter->addTab(formCollege,"学院数据");

    formMajor = new FormMajor(this);
    formMajor->setAttribute(Qt::WA_DeleteOnClose);
    ui->tabQueryCenter->addTab(formMajor,"专业数据");

    formClass = new FormClass(this);
    formClass->setAttribute(Qt::WA_DeleteOnClose);
    ui->tabQueryCenter->addTab(formClass,"班级数据");

    formCurriculum = new FormCurriculum(this);
    formCurriculum->setAttribute(Qt::WA_DeleteOnClose);
    ui->tabQueryCenter->addTab(formCurriculum,"课程数据");

    ui->tabQueryCenter->setVisible(true);
    ui->tabQueryCenter->setCurrentIndex(0);

    // page4
    fromStudent = new FormStudent(this);
    fromStudent->setAttribute(Qt::WA_DeleteOnClose);
    ui->tabUserCenter->addTab(fromStudent,"学生数据");

    ui->tabQueryCenter->setVisible(true);
    ui->tabUserCenter->setCurrentIndex(0);
}

void MainPage::buttonClick()
{
    QAbstractButton *b = (QAbstractButton *)sender();
    QString name = b->text();

    QList<QAbstractButton *> tbtns = ui->widgetTop->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *btn, tbtns) {
        btn->setChecked(btn == b);
    }

    if (name == "主界面") {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "查询中心") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "考试中心") {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "用户中心") {
        ui->stackedWidget->setCurrentIndex(3);
    } else if (name == "分析中心") {
        ui->stackedWidget->setCurrentIndex(4);
    }
    else{
        ui->stackedWidget->setCurrentIndex(5);
    }
}

// 学院数据
void MainPage::on_btnCollegeData_clicked()
{
    ui->tabQueryCenter->setCurrentIndex(0);
    formCollege->refresh();
}

// 班级数据
void MainPage::on_btnClassData_clicked()
{
    ui->tabQueryCenter->setCurrentIndex(2);
    formClass->refresh();
}

// 课程数据
void MainPage::on_btnCurriculumData_clicked()
{
    ui->tabQueryCenter->setCurrentIndex(3);
    formCurriculum->refresh();
}

// 专业
void MainPage::on_btnMajorData_clicked()
{
    ui->tabQueryCenter->setCurrentIndex(1);
    formMajor->refresh();
}

void MainPage::on_tabQueryCenter_tabBarClicked(int index)
{
    qDebug() << index;
    switch (index) {
    case 0:
        on_btnCollegeData_clicked();
        break;
    case 1:
        on_btnMajorData_clicked();
        break;
    case 2:
        on_btnClassData_clicked();
        break;
    case 3:
        on_btnCurriculumData_clicked();
        break;
    default:
        break;
    }
}

void MainPage::on_btnStudentData_clicked()
{

}


void MainPage::on_tBtnAllExam_clicked()
{

}

// 考试创建
void MainPage::on_tBtnCreateExam_clicked()
{
    FormCreateExam* formCE = new FormCreateExam(this);
    formCE->setAttribute(Qt::WA_DeleteOnClose);
    formCE->setWindowTitle("考试创建");
    formCE->setWindowFlag(Qt::Window,true);
    formCE->show();
}

// 学院
void MainPage::on_tBtn1_clicked()
{

}
