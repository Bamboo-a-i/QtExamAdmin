#include "dialogstudent.h"
#include "ui_dialogstudent.h"
#include "sqlmanager.h"
#include <QCryptographicHash>

DialogStudent::DialogStudent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStudent)
{
    ui->setupUi(this);
    initDialog();
}

DialogStudent::~DialogStudent()
{
    delete ui;
}

void DialogStudent::initDialog()
{
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("dialogstudent");
    QMapIterator<QString, QString> ic(collegeDataMap);
    while (ic.hasNext()) {
      ic.next();
      ui->comboCollegeId->addItem(ic.key());
      ui->comboCollegeName->addItem(ic.value());
    }

     QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_id",ui->comboCollegeId->currentText());
     QMapIterator<QString, QString> im(majorDataMap);
     while (im.hasNext()) {
       im.next();
       ui->comboMajorId->addItem(im.key());
       ui->comboMajorName->addItem(im.value());
     }

     QMap<QString,QString> classDataMap = SqlManager::getInstance()->classDataQuery
             ("class_grade",ui->comboGrade->currentText(),
              "college_id",ui->comboCollegeId->currentText(),
              "major_id",ui->comboMajorId->currentText(),"dialogstudent");
     QMapIterator<QString, QString> iclass(classDataMap);
     while (iclass.hasNext()) {
         iclass.next();
         ui->comboClassId->addItem(iclass.key());
         ui->comboClassName->addItem(iclass.value());
     }
}

void DialogStudent::setUpdateRecord(QSqlRecord &recData)
{
    setWindowTitle("更新记录");
    mRecord=recData;
    ui->leStuId->setText(mRecord.value("stu_id").toString());
    ui->leStuName->setText(mRecord.value("stu_name").toString());
    ui->comboGender->setCurrentText(mRecord.value("stu_gender").toString());
    ui->comboGrade->setCurrentText(mRecord.value("class_grade").toString());
    ui->comboCollegeId->setCurrentText(mRecord.value("college_id").toString());
    ui->comboMajorId->setCurrentText(mRecord.value("major_id").toString());
    ui->comboClassId->setCurrentText(mRecord.value("class_id").toString());
}

void DialogStudent::setInsertRecord(QSqlRecord &recData)
{
    setWindowTitle("添加记录");
    mRecord = recData;
}

QSqlRecord DialogStudent::getRecordData()
{
    setWindowTitle("插入新记录");
    mRecord.setValue("stu_id",ui->leStuId->text());
    mRecord.setValue("stu_name",ui->leStuName->text());
    mRecord.setValue("stu_gender",ui->comboGender->currentText());
    mRecord.setValue("college_id",ui->comboCollegeId->currentText());
    mRecord.setValue("college_name",ui->comboCollegeName->currentText());
    mRecord.setValue("major_id",ui->comboMajorId->currentText());
    mRecord.setValue("major_name",ui->comboMajorName->currentText());
    mRecord.setValue("class_id",ui->comboClassId->currentText());
    mRecord.setValue("class_name",ui->comboClassName->currentText());
    return mRecord;
}

void DialogStudent::on_comboCollegeName_currentIndexChanged(int index)
{
    ui->comboCollegeId->setCurrentIndex(index);
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("class_grade",ui->comboGrade->currentText(),"college_id",ui->comboCollegeId->currentText());
    QMapIterator<QString, QString> im(majorDataMap);
    ui->comboMajorId->clear(); // 清除
    ui->comboMajorName->clear();
    while (im.hasNext()) {
      im.next();
      ui->comboMajorId->addItem(im.key());
      ui->comboMajorName->addItem(im.value());
    }

    QMap<QString,QString> classDataMap = SqlManager::getInstance()->classDataQuery
            ("class_grade",ui->comboGrade->currentText(),
             "college_id",ui->comboCollegeId->currentText(),
             "major_id",ui->comboMajorId->currentText(),"dialogstudent");
    QMapIterator<QString, QString> iclass(classDataMap);
    while (iclass.hasNext()) {
        iclass.next();
        ui->comboClassId->addItem(iclass.key());
        ui->comboClassName->addItem(iclass.value());
    }
}

void DialogStudent::on_comboMajorName_currentIndexChanged(int index)
{
    ui->comboMajorId->setCurrentIndex(index);
}

void DialogStudent::on_comboClassName_currentIndexChanged(int index)
{
    ui->comboCollegeId->setCurrentIndex(index);
}

