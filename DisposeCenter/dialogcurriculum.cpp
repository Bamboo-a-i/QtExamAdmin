#include "dialogcurriculum.h"
#include "ui_dialogcurriculum.h"
#include "sqlmanager.h"

DialogCurriculum::DialogCurriculum(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCurriculum)
{
    ui->setupUi(this);
    initDialog();
}

DialogCurriculum::~DialogCurriculum()
{
    delete ui;
}

void DialogCurriculum::initDialog()
{
    // 学院
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("dialogcurriculumdata");
    QMapIterator<QString, QString> iCollege(collegeDataMap);
    while (iCollege.hasNext()) {
      iCollege.next();
      ui->comboCollegeId->addItem(iCollege.key());
      ui->comboCollegeName->addItem(iCollege.value());
    }

    // 专业
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_id",ui->comboCollegeId->currentText(),1);
    QMapIterator<QString, QString> iMajor(majorDataMap);
    while (iMajor.hasNext()) {
        iMajor.next();
        ui->comboMajorId->addItem(iMajor.key());
        ui->comboMajorName->addItem(iMajor.value());
    }

     // 教师 不受专业变化而改变
     QMap<QString, QString> teaDataMap = SqlManager::getInstance()->teacherDataQuery("college_id",ui->comboCollegeId->currentText());
     QMapIterator<QString, QString> iTeacher(teaDataMap);
     while (iTeacher.hasNext()) {
       iTeacher.next();
       ui->comboTeaId->addItem(iTeacher.key());
       ui->comboTeaName->addItem(iTeacher.value());
     }
}

void DialogCurriculum::setUpdateRecord(QSqlRecord &recData)
{
    setWindowTitle("更新记录");
    mRecord=recData;
    ui->leCurriculumId->setText(mRecord.value("curriculum_id").toString());
    ui->leCurriculumName->setText(mRecord.value("curriculum_name").toString());
    ui->comboCollegeId->setCurrentText(mRecord.value("college_id").toString());
    ui->comboMajorId->setCurrentText(mRecord.value("major_id").toString());
    ui->comboTeaId->setCurrentText(mRecord.value("tea_id").toString());
}

void DialogCurriculum::setInsertRecord(QSqlRecord &recData)
{
    setWindowTitle("添加记录");
    mRecord = recData;
}

QSqlRecord DialogCurriculum::getRecordData()
{
    mRecord.setValue("curriculum_id",ui->leCurriculumId->text());
    mRecord.setValue("curriculum_name",ui->leCurriculumName->text());
    mRecord.setValue("college_id",ui->comboCollegeId->currentText());
    mRecord.setValue("college_name",ui->comboCollegeName->currentText());
    mRecord.setValue("tea_id",ui->comboTeaId->currentText());
    mRecord.setValue("major_id",ui->comboMajorId->currentText());
    mRecord.setValue("major_name",ui->comboMajorName->currentText());
    return mRecord;
}

void DialogCurriculum::on_comboCollegeName_currentIndexChanged(int index)
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
    ui->comboTeaId->clear();
    ui->comboTeaName->clear();
    QMap<QString, QString> teaDataMap = SqlManager::getInstance()->teacherDataQuery("college_id",ui->comboCollegeId->currentText());
    QMapIterator<QString, QString> iTeacher(teaDataMap);
    while (iTeacher.hasNext()) {
      iTeacher.next();
      ui->comboTeaId->addItem(iTeacher.key());
      ui->comboTeaName->addItem(iTeacher.value());
    }
}

void DialogCurriculum::on_comboMajorName_currentIndexChanged(int index)
{
    ui->comboMajorId->setCurrentIndex(index);
}

void DialogCurriculum::on_comboTeaName_currentIndexChanged(int index)
{
    ui->comboTeaId->setCurrentIndex(index);
}
