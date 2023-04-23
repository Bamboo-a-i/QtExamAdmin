#include "formcreateexam.h"
#include "ui_formcreateexam.h"

FormCreateExam::FormCreateExam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCreateExam)
{
    ui->setupUi(this);
    initForm();
}

FormCreateExam::~FormCreateExam()
{
    delete ui;
}

void FormCreateExam::initForm()
{
    // 上传文件
    ui->stackedWidget->setCurrentIndex(0);
}

// 下载excel模板
void FormCreateExam::on_tBtnExcelDownload_clicked()
{

}

void FormCreateExam::on_tBtnUploadTemplate_clicked()
{
    // 读取文件成功 转到读题页面
    ui->stackedWidget->setCurrentIndex(1);
}

// 开始设置试卷各种属性
void FormCreateExam::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void FormCreateExam::on_pushButton_2_clicked()
{
    // 存储考试内容
}
