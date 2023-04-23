#include "dialogmajordata.h"
#include "ui_dialogmajordata.h"
#include "sqlmanager.h"
#include <QDebug>

DialogMajorData::DialogMajorData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMajorData)
{
    ui->setupUi(this);
    // 初始化
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("dialogmajordata");
    QMapIterator<QString,QString> iCollege(collegeDataMap);
    while (iCollege.hasNext()) {
        iCollege.next();
        ui->comboCollegeId->addItem(iCollege.key());
        ui->comboCollegeName->addItem(iCollege.value());
    }
}

DialogMajorData::~DialogMajorData()
{
    delete ui;
}


void DialogMajorData::setUpdateRecord(QSqlRecord &recData)
{
    setWindowTitle("更新记录");
    mRecord=recData;
    ui->leMajorId->setText(mRecord.value("major_id").toString());
    ui->leMajorName->setText(mRecord.value("major_name").toString());


}

void DialogMajorData::setInsertRecord(QSqlRecord &recData)
{
    setWindowTitle("添加记录");
    mRecord = recData;
}

QSqlRecord DialogMajorData::getRecordData()
{
    mRecord.setValue("major_id",ui->leMajorId->text());
    mRecord.setValue("major_name",ui->leMajorName->text());
    mRecord.setValue("college_id",ui->comboCollegeId->currentText());
    mRecord.setValue("college_name",ui->comboCollegeName->currentText());
    return mRecord;
}

void DialogMajorData::on_comboCollegeName_currentIndexChanged(int index)
{
    ui->comboCollegeId->setCurrentIndex(index);
}
