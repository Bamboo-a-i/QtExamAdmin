#ifndef DIALOGSTUDENT_H
#define DIALOGSTUDENT_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class DialogStudent;
}

class DialogStudent : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStudent(QWidget *parent = nullptr);
    ~DialogStudent();
    void    setUpdateRecord(QSqlRecord &recData); //更新记录
    void    setInsertRecord(QSqlRecord &recData); //插入记录
    QSqlRecord  getRecordData();//获取录入的数据

private:
    Ui::DialogStudent *ui;

private slots:
    void initDialog();

    void on_comboCollegeName_currentIndexChanged(int index);

    void on_comboMajorName_currentIndexChanged(int index);

    void on_comboClassName_currentIndexChanged(int index);

private:
    QSqlRecord  mRecord; //保存一条记录的数据
};

#endif // DIALOGSTUDENT_H
