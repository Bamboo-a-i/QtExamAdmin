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
    // tabpage初始化
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

    fromStudent = new FormStudent(this);
    fromStudent->setAttribute(Qt::WA_DeleteOnClose);
    ui->tabQueryCenter->addTab(fromStudent,"学生数据");

    ui->tabQueryCenter->setVisible(true);


    //设置顶部导航按钮
    QList<QAbstractButton *> tbtns = ui->widgetTop->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *btn, tbtns) {
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(tBtnPageClick()));
    }
    ui->tbtn1->click(); // 默认主页面

    // 主页tBtn初始化
    QList<QAbstractButton *> tBtnTabs = ui->widgetMain->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *tBtn, tBtnTabs) {
        tBtn->setCheckable(true);
        connect(tBtn,SIGNAL(clicked()),this,SLOT(tBtnTabClick()));
    }

}

void MainPage::tBtnPageClick()
{
    QAbstractButton *b = (QAbstractButton *)sender();
    QString name = b->text();

    QList<QAbstractButton *> tbtns = ui->widgetTop->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *btn, tbtns) {
        btn->setChecked(btn == b);
    }
    if (name == " 主界面") {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "查询中心") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "考试中心") {
        ui->stackedWidget->setCurrentIndex(2);
        formClass->refresh();
    } else if (name == "分析中心") {
        ui->stackedWidget->setCurrentIndex(3);
    } else {
        ui->stackedWidget->setCurrentIndex(4);
    }
}

// 主页连接tab
void MainPage::tBtnTabClick()
{
    QAbstractButton *b = (QAbstractButton *)sender();
    QString name = b->text();

    QList<QAbstractButton *> tBtnTabs = ui->widgetMain->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *tBtn, tBtnTabs) {
            tBtn->setChecked(tBtn == b);
    }
    ui->stackedWidget->setCurrentIndex(1);
    if (name == "学 院") {
        ui->tabQueryCenter->setCurrentIndex(0);
        formCollege->refresh();
    } else if (name == "专 业") {
        ui->tabQueryCenter->setCurrentIndex(1);
        formMajor->refresh();
    } else if (name == "班 级") {
        ui->tabQueryCenter->setCurrentIndex(2);
        formClass->refresh();
    } else if (name == "课 程") {
        ui->tabQueryCenter->setCurrentIndex(3);
        formCurriculum->refresh();
    } else if (name == "用 户") {
        ui->tabQueryCenter->setCurrentIndex(4);
        formCollege->refresh();
    }
    else{
        ui->tabQueryCenter->setCurrentIndex(5);
        formCollege->refresh();
    }
}

// 切换时刷新
void MainPage::on_tabQueryCenter_tabBarClicked(int index)
{
    qDebug() << index;
    switch (index) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        break;
    }
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

