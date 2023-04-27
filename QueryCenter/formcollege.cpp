#include "formcollege.h"
#include "ui_formcollege.h"
#include "sqlmanager.h"
#include "dialogcollegedata.h"
#include <QDateTime>
#include <QAbstractItemView>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QStandardItem>
#include <QGroupBox>
#include <QSqlError>
#include <QLineEdit>
FormCollege::FormCollege(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCollege)
{
    ui->setupUi(this);
    initForm();
}

void FormCollege::refresh()
{
    initForm();
}

FormCollege::~FormCollege()
{
    delete ui;
}

// 初始化页面
void FormCollege::initForm()
{
    // 表格设置
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 全部展开
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 表格不可编辑
    ui->tableView->verticalHeader()->hide(); // 不显示行号
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中行

    queryModel = SqlManager::getInstance()->collegeDataQuery(); // 接收数据
    queryModel->setHeaderData(0,Qt::Horizontal,tr("学院编号"));
    queryModel->setHeaderData(1,Qt::Horizontal,tr("学院名称"));
    queryModel->setHeaderData(2,Qt::Horizontal,tr("创建时间"));


    theSelection = new QItemSelectionModel(queryModel); // 关联模型
    ui->tableView->setModel(queryModel); // 数据模型
    ui->tableView->setSelectionModel(theSelection); //选择模型
}

// 添加
void FormCollege::on_tBtnAdd_clicked()
{
    DialogCollegeData *dlgCD = new DialogCollegeData(this);
    Qt::WindowFlags    flags=dlgCD->windowFlags();
    dlgCD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlQuery query;
    query.exec("select * from college where college_id =-1"); //实际不查询出记录，只查询字段信息
    QSqlRecord curRec=query.record();//获取当前记录,实际为空记录

    dlgCD->setInsertRecord(curRec);
    if(dlgCD->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgCD->getRecordData();
        if(recData.value("college_id").toString().isEmpty() || recData.value("college_name").toString().isEmpty())
        {
            QMessageBox::information(this,"提示信息","不能为空",QMessageBox::Ok,QMessageBox::NoButton);

        }
        else
        {
            if(SqlManager::getInstance()->collegeDataInsert(recData.value("college_id").toString(),recData.value("college_name").toString()))
            {
                QMessageBox::information(this,"提示信息","添加成功",QMessageBox::Ok,QMessageBox::NoButton);
            }
            else
            {
                QMessageBox::information(this, "消息", "数据添加错误,错误信息\n"+queryModel->lastError().text(),
                                         QMessageBox::Ok,QMessageBox::NoButton);
            }
        }
    }
    on_tBtnRefresh_clicked();
}

// 删除
void FormCollege::on_tBtnDelete_clicked()
{
    int selectedRow =  theSelection->selectedRows().count();
    int res = QMessageBox::information(this,"提示信息",QString("是否删除%1条数据").arg(selectedRow),QMessageBox::Ok,QMessageBox::No);
    if(res ==QMessageBox::Ok)
    {
        QModelIndexList theList = theSelection->selectedRows();
        bool isDelete = true;
        QString errorStr;
        foreach (QModelIndex item, theList)
        {
            auto var = queryModel->data(queryModel->index(item.row(),0),Qt::DisplayRole); // 获取数据
            QString college_id = var.value<QString>();
            if(!SqlManager::getInstance()->formCollegeDelete(college_id))
            {
                isDelete = false;
                errorStr += QString("第%1条数据删除失败.\n").arg(item.row());
            }
        }

        if(isDelete)
        {
            QMessageBox::information(this,"提示信息","删除成功",QMessageBox::Ok,QMessageBox::NoButton);
        }
        else
        {
            QMessageBox::information(this, "消息", errorStr,
                                     QMessageBox::Ok,QMessageBox::NoButton);
        }
        on_tBtnRefresh_clicked(); // 刷新
    }
}

// 刷新
void FormCollege::on_tBtnRefresh_clicked()
{
    initForm();
    ui->tBtnAdd->setEnabled(false);
    ui->tBtnEdit->setEnabled(false);
    ui->tBtnDelete->setEnabled(false);
}

// 编辑
void FormCollege::on_tBtnEdit_clicked()
{
    updateRecord(theSelection->currentIndex().row());
}

void FormCollege::updateRecord(int recNo)
{
    auto c_id = queryModel->data(queryModel->index(recNo,0),Qt::DisplayRole); // 获取id
    QString strid= c_id.value<QString>();

    DialogCollegeData *dlgCD = new DialogCollegeData(this);
    Qt::WindowFlags    flags=dlgCD->windowFlags();
    dlgCD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlRecord curRec=queryModel->record(recNo);//获取当前记录,实际为空记录

    dlgCD->setUpdateRecord(curRec);
    int res = dlgCD->exec();
    if(res == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgCD->getRecordData();
        if(recData.value("college_id").toString().isEmpty() || recData.value("college_name").toString().isEmpty())
        {
            QMessageBox::information(this,"提示信息","不能为空",QMessageBox::Ok,QMessageBox::NoButton);

        }
        else
        {
            if(SqlManager::getInstance()->collegeDateUpdate(strid,recData.value("college_id").toString(),recData.value("college_name").toString()))
            {
                QMessageBox::information(this,"提示信息","修改成功",QMessageBox::Ok,QMessageBox::NoButton);
            }
            else
            {
                QMessageBox::information(this, "消息", "数据修改错误,错误信息\n"+queryModel->lastError().text(),
                                         QMessageBox::Ok,QMessageBox::NoButton);
            }
        }
    }
    on_tBtnRefresh_clicked(); // 刷新
}

void FormCollege::on_tableView_doubleClicked(const QModelIndex &index)
{
    int curRecNo = index.row();
    updateRecord(curRecNo);
}

void FormCollege::on_tableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->tBtnAdd->setEnabled(true);
    ui->tBtnEdit->setEnabled(true);
    ui->tBtnDelete->setEnabled(true);
}
