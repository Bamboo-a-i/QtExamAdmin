#ifndef DIALOGCOLLEGEDATA_H
#define DIALOGCOLLEGEDATA_H

#include <QSqlRecord>
#include <QDialog>

namespace Ui {
class DialogCollegeData;
}

class DialogCollegeData : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCollegeData(QWidget *parent = nullptr);
    ~DialogCollegeData();

    void    setUpdateRecord(QSqlRecord &recData); //更新记录
    void    setInsertRecord(QSqlRecord &recData); //插入记录

    QSqlRecord  getRecordData();//获取录入的数据

private:
    QSqlRecord  mRecord; //保存一条记录的数据
private:
    Ui::DialogCollegeData *ui;
};

#endif // DIALOGCOLLEGEDATA_H
