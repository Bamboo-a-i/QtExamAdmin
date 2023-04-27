#ifndef FORMCURRICULUM_H
#define FORMCURRICULUM_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include <QTreeWidgetItem>

namespace Ui {
class FormCurriculum;
}

class FormCurriculum : public QWidget
{
    Q_OBJECT

public:
    explicit FormCurriculum(QWidget *parent = nullptr);
    void refresh();
    ~FormCurriculum();

private:
    enum treeItemtype{itTop = 1001,itSecond,itThird};

    Ui::FormCurriculum *ui;

private slots:

    void initTable();

    void initTree();

    void removeItem(QTreeWidgetItem *item);

    void removeAllTreeItem();

    void showModel(QSqlQueryModel *queryModel);

    void on_tBtnAdd_clicked();

    void on_tBtnDelete_clicked();

    void on_tBtnRefresh_clicked();

    void on_tBtnEdit_clicked();

    void updateRecord(int recNo);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);


//    void on_tBtnScreen_clicked();

//    void on_comboCollege_currentIndexChanged(int index);

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_tBtnShowAll_clicked();

private:
    QSqlQueryModel  *queryModel;  //数据模型
    QItemSelectionModel *theSelection; //选择模型
};

#endif // FORMCURRICULUM_H
