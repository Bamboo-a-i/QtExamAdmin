#ifndef FORMSTUDENT_H
#define FORMSTUDENT_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QItemSelectionModel>

namespace Ui {
class FormStudent;
}

class FormStudent : public QWidget
{
    Q_OBJECT

public:
    explicit FormStudent(QWidget *parent = nullptr);
    void refresh();
    ~FormStudent();

private:
    Ui::FormStudent *ui;

private slots:
    void initForm();

    void initTableView(QSqlQueryModel *queryModel);

    void screenClass();

    void on_tBtnAdd_clicked();

    void on_tBtnDelete_clicked();

    void on_tBtnRefresh_clicked();

    void on_tBtnEdit_clicked();

    void updateRecord(int recNo);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_tBtnScreen_clicked();

    void initScreen();

    void on_comboMajor_currentIndexChanged(int index);

    void on_tBtnGroup_clicked();

    void on_comboCollege_currentIndexChanged(int index);

    void on_tBtnDefaultPswd_clicked();

    void on_tBtnQuery_clicked();

    void on_comboGrade_currentIndexChanged(int index);

    void on_tBtnShowAll_clicked();

private:
    int curPage;
    QSqlQueryModel  *queryModel;  //数据模型
    QItemSelectionModel *theSelection; //选择模型
};
#endif // FORMSTUDENT_H
