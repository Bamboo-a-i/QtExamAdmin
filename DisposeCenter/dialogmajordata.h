#ifndef DIALOGMAJORDATA_H
#define DIALOGMAJORDATA_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class DialogMajorData;
}

class DialogMajorData : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMajorData(QWidget *parent = nullptr);
    ~DialogMajorData();

    void    setUpdateRecord(QSqlRecord &recData); //更新记录
    void    setInsertRecord(QSqlRecord &recData); //插入记录

    QSqlRecord  getRecordData();//获取录入的数据

private slots:
    void on_comboCollegeName_currentIndexChanged(int index);

private:
    QSqlRecord  mRecord; //保存一条记录的数据
private:
    Ui::DialogMajorData *ui;
};

#endif // DIALOGMAJORDATA_H
