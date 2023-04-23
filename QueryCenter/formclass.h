#ifndef FORMCLASS_H
#define FORMCLASS_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QItemSelectionModel>

namespace Ui {
class FormClass;
}

class FormClass : public QWidget
{
    Q_OBJECT

public:
    explicit FormClass(QWidget *parent = nullptr);
    void refresh();
    ~FormClass();

private:
    Ui::FormClass *ui;

private slots:
    void initForm();

    void on_tBtnAdd_clicked();

    void on_tBtnDelete_clicked();

    void on_tBtnRefresh_clicked();

    void on_tBtnEdit_clicked();

    void updateRecord(int recNo);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_tBtnScreen_clicked();

    void initScreen();

    void on_comboCollege_currentIndexChanged(int index);

private:
    QSqlQueryModel  *queryModel;  //数据模型
    QItemSelectionModel *theSelection; //选择模型
};

#endif // FORMCLASS_H
