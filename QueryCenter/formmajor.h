#ifndef FORMMAJOR_H
#define FORMMAJOR_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include <QTreeWidget>

namespace Ui {
class FormMajor;
}

class FormMajor : public QWidget
{
    Q_OBJECT

public:
    explicit FormMajor(QWidget *parent = nullptr);
    void refresh();
    ~FormMajor();

private:
    Ui::FormMajor *ui;


private slots:
    void initTable();

    void initTree();

    void showModel(QSqlQueryModel *queryModel);

    void on_tBtnAdd_clicked();

    void on_tBtnDelete_clicked();

    void on_tBtnRefresh_clicked();

    void on_tBtnEdit_clicked();

    void updateRecord(int recNo);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_tBtnShowAll_clicked();

private:

    QSqlQueryModel  *queryModel;  //数据模型

    QItemSelectionModel *theSelection; //选择模型
};

#endif // FORMMAJOR_H
