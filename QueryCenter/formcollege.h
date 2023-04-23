#ifndef FORMCOLLEGE_H
#define FORMCOLLEGE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QItemSelectionModel>
namespace Ui {
class FormCollege;
}

class FormCollege : public QWidget
{
    Q_OBJECT

public:
    explicit FormCollege(QWidget *parent = nullptr);
    void refresh();
    ~FormCollege();

private slots:
    void initForm();

    void on_tBtnAdd_clicked();

    void on_tBtnDelete_clicked();

    void on_tBtnRefresh_clicked();

    void on_tBtnEdit_clicked();

    void updateRecord(int recNo);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::FormCollege *ui;

    QSqlQueryModel  *queryModel;  //数据模型

    QItemSelectionModel *theSelection; //选择模型
};

#endif // FORMCOLLEGE_H
