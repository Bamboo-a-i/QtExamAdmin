#include "dialogclassdata.h"
#include "ui_dialogclassdata.h"
#include "sqlmanager.h"

DialogClassData::DialogClassData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogClassData)
{
    ui->setupUi(this);
    initDialog();
}

DialogClassData::~DialogClassData()
{
    delete ui;
}

void DialogClassData::initDialog()
{
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("dialogclassdata");
    QMapIterator<QString, QString> iCollege(collegeDataMap);
    while (iCollege.hasNext()) {
      iCollege.next();
      ui->comboCollegeId->addItem(iCollege.key());
      ui->comboCollegeName->addItem(iCollege.value());
    }

     QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_id",ui->comboCollegeId->currentText(),1);
     QMapIterator<QString, QString> iMajor(majorDataMap);
     while (iMajor.hasNext()) {
       iMajor.next();
       ui->comboMajorId->addItem(iMajor.key());
       ui->comboMajorName->addItem(iMajor.value());
     }
}

void DialogClassData::setUpdateRecord(QSqlRecord &recData)
{
    setWindowTitle("更新记录");
    mRecord=recData;
    ui->leClassId->setText(mRecord.value("class_id").toString());
    ui->leClassName->setText(mRecord.value("class_name").toString());
    ui->comboClassGrade->setCurrentText(mRecord.value("class_grade").toString());
    ui->comboCollegeId->setCurrentText(mRecord.value("college_id").toString());
    ui->comboMajorId->setCurrentText(mRecord.value("major_id").toString());
}

void DialogClassData::setInsertRecord(QSqlRecord &recData)
{
    setWindowTitle("添加记录");
    mRecord = recData;
}

QSqlRecord DialogClassData::getRecordData()
{
    setWindowTitle("插入新记录");
    mRecord.setValue("class_id",ui->leClassId->text());
    mRecord.setValue("class_name",ui->leClassName->text());
    mRecord.setValue("class_grade",ui->comboClassGrade->currentText());
    mRecord.setValue("college_id",ui->comboCollegeId->currentText());
    mRecord.setValue("college_name",ui->comboCollegeName->currentText());
    mRecord.setValue("major_id",ui->comboMajorId->currentText());
    mRecord.setValue("major_name",ui->comboMajorName->currentText());
    return mRecord;
}

void DialogClassData::on_comboCollegeName_currentIndexChanged(int index)
{
    ui->comboCollegeId->setCurrentIndex(index);
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_id",ui->comboCollegeId->currentText(),1);
    QMapIterator<QString, QString> iMajor(majorDataMap);
    ui->comboMajorId->clear(); // 清除
    ui->comboMajorName->clear();
    while (iMajor.hasNext()) {
      iMajor.next();
      ui->comboMajorId->addItem(iMajor.key());
      ui->comboMajorName->addItem(iMajor.value());
    }
}

void DialogClassData::on_comboMajorName_currentIndexChanged(int index)
{
    ui->comboMajorId->setCurrentIndex(index);
}
