#ifndef DIALOGCLASSDATA_H
#define DIALOGCLASSDATA_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class DialogClassData;
}

class DialogClassData : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClassData(QWidget *parent = nullptr);
    ~DialogClassData();

    void    setUpdateRecord(QSqlRecord &recData); //更新记录
    void    setInsertRecord(QSqlRecord &recData); //插入记录


    QSqlRecord  getRecordData();//获取录入的数据

private slots:
    void initDialog();

    void on_comboCollegeName_currentIndexChanged(int index);

    void on_comboMajorName_currentIndexChanged(int index);

private:
    QSqlRecord  mRecord; //保存一条记录的数据
private:
    Ui::DialogClassData *ui;
};

#endif // DIALOGCLASSDATA_H
