#include "dialogcollegedata.h"
#include "ui_dialogcollegedata.h"

DialogCollegeData::DialogCollegeData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCollegeData)
{
    ui->setupUi(this);
}

DialogCollegeData::~DialogCollegeData()
{
    delete ui;
}

void DialogCollegeData::setUpdateRecord(QSqlRecord &recData)
{
    setWindowTitle("更新记录");
    mRecord=recData;
    ui->leCollegeId->setText(mRecord.value("college_id").toString());
    ui->leCollegeName->setText(mRecord.value("college_name").toString());
}

void DialogCollegeData::setInsertRecord(QSqlRecord &recData)
{
    mRecord = recData;
}

QSqlRecord DialogCollegeData::getRecordData()
{
    setWindowTitle("插入新记录");
    mRecord.setValue("college_id",ui->leCollegeId->text());
    mRecord.setValue("college_name",ui->leCollegeName->text());
    return mRecord;
}
