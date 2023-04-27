#include "formmajor.h"
#include "ui_formmajor.h"
#include "sqlmanager.h"
#include "dialogmajordata.h"
#include <QDateTime>
#include <QAbstractItemView>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QStandardItem>
#include <QGroupBox>
#include <QSqlError>
#include <QLineEdit>
FormMajor::FormMajor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMajor)
{
    ui->setupUi(this);
    initTable();
    initTree();
}

FormMajor::~FormMajor()
{
    delete ui;
}

void FormMajor::refresh()
{
    initTable();
}

// 初始化tableview
void FormMajor::initTable()
{
    // 表格设置
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 全部展开
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 表格不可编辑
    ui->tableView->verticalHeader()->hide(); // 不显示行号
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中行

    queryModel = SqlManager::getInstance()->majorDataQuery(); // 接受数据
    showModel(queryModel);
    theSelection = new QItemSelectionModel(queryModel); // 关联模型
    ui->tableView->setModel(queryModel); // 数据模型
    ui->tableView->setSelectionModel(theSelection); //选择模型
}

// 初始化 treewidget
void FormMajor::initTree()
{
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("formmajor");

    QMapIterator<QString,QString> iCollege(collegeDataMap);

    while (iCollege.hasNext()) {
        iCollege.next();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0,iCollege.value());
        item->setIcon(0,QIcon(":/res/icon/dir1.png"));
        ui->treeWidget->setCurrentItem(item);
    }
}

void FormMajor::showModel(QSqlQueryModel *queryModel)
{
    queryModel->setHeaderData(0,Qt::Horizontal,tr("专业编号"));
    queryModel->setHeaderData(1,Qt::Horizontal,tr("专业名称"));
    queryModel->setHeaderData(2,Qt::Horizontal,tr("学院编号"));
    queryModel->setHeaderData(3,Qt::Horizontal,tr("学院名称"));
    queryModel->setHeaderData(4,Qt::Horizontal,tr("创建时间"));
}

// 添加
void FormMajor::on_tBtnAdd_clicked()
{
    DialogMajorData *dlgMD = new DialogMajorData(this);
    Qt::WindowFlags    flags=dlgMD->windowFlags();
    dlgMD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlRecord curRec=queryModel->record();//获取当前记录,实际为空记录
    dlgMD->setInsertRecord(curRec);
    if(dlgMD->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgMD->getRecordData();
        if(recData.value("major_id").toString().isEmpty() || recData.value("major_name").toString().isEmpty())
        {
            QMessageBox::information(this,"提示信息","不能为空",QMessageBox::Ok,QMessageBox::NoButton);

        }
        else
        {
            if(SqlManager::getInstance()->majorDataInsert(recData.value("major_id").toString(),recData.value("major_name").toString(),recData.value("college_id").toString()))
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
void FormMajor::on_tBtnDelete_clicked()
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
            if(!SqlManager::getInstance()->formMajorDelete(college_id))
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
void FormMajor::on_tBtnRefresh_clicked()
{
    queryModel = SqlManager::getInstance()->majorDataQuery("college_name",ui->treeWidget->currentItem()->text(0));
    showModel(queryModel);
    ui->tBtnEdit->setEnabled(false);
    ui->tBtnDelete->setEnabled(false);
    ui->tBtnAdd->setEnabled(false);
}

// 编辑
void FormMajor::on_tBtnEdit_clicked()
{
    updateRecord(theSelection->currentIndex().row());
}

void FormMajor::updateRecord(int recNo)
{
    auto majorId = queryModel->data(queryModel->index(recNo,0),Qt::DisplayRole); // 获取id
    QString strMajorId= majorId.value<QString>();

    DialogMajorData *dlgMD = new DialogMajorData(this);
    Qt::WindowFlags    flags= dlgMD->windowFlags();
    dlgMD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlRecord curRec=queryModel->record(recNo);//获取当前记录,实际为空记录
    dlgMD->setUpdateRecord(curRec);
    if(dlgMD->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgMD->getRecordData();
        if(recData.value("major_id").toString().isEmpty() || recData.value("major_name").toString().isEmpty())
        {
            QMessageBox::information(this,"提示信息","不能为空",QMessageBox::Ok,QMessageBox::NoButton);

        }
        else
        {
            if(SqlManager::getInstance()->majorDateUpdate(strMajorId,recData.value("major_id").toString(),recData.value("major_name").toString(),recData.value("college_id").toString()))
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

void FormMajor::on_tableView_doubleClicked(const QModelIndex &index)
{
    int curRecNo = index.row();
    updateRecord(curRecNo);
}

void FormMajor::on_tableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->tBtnEdit->setEnabled(true);
    ui->tBtnDelete->setEnabled(true);
    ui->tBtnAdd->setEnabled(true);
}

void FormMajor::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    queryModel = SqlManager::getInstance()->majorDataQuery("college_name",current->text(0));
    showModel(queryModel);
}

void FormMajor::on_tBtnShowAll_clicked()
{
    queryModel = SqlManager::getInstance()->majorDataQuery(); // 接受数据
    showModel(queryModel);
}
