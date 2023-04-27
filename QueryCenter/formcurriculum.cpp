#include "formcurriculum.h"
#include "ui_formcurriculum.h"
#include <QMessageBox>
#include "sqlmanager.h"
#include "dialogcurriculum.h"
#include <QSqlError>

FormCurriculum::FormCurriculum(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCurriculum)
{
    ui->setupUi(this);
    initTable();
    initTree();
}

FormCurriculum::~FormCurriculum()
{
    delete ui;
}

void FormCurriculum::refresh()
{
    initTable();
}

// 初始化页面
void FormCurriculum::initTable()
{
    // 表格设置
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 全部展开
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 表格不可编辑
    ui->tableView->verticalHeader()->hide(); // 不显示行号
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中行

    queryModel = SqlManager::getInstance()->curriculumDataQuery(); // 接受数据
    showModel(queryModel);
    theSelection = new QItemSelectionModel(queryModel); // 关联模型
    ui->tableView->setModel(queryModel); // 数据模型
    ui->tableView->setSelectionModel(theSelection); //选择模型
}

// 初始化tree
void FormCurriculum::initTree()
{
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("formmajor");
    QMapIterator<QString,QString> iCollege(collegeDataMap);
    while (iCollege.hasNext()) {
        iCollege.next();
        QTreeWidgetItem *item1 = new QTreeWidgetItem(FormCurriculum::itTop);
        item1->setText(0,iCollege.value());
        item1->setIcon(0,QIcon(":/res/icon/dir2.png"));
        ui->treeWidget->addTopLevelItem(item1);
        QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_name",iCollege.value(),1);
        QMapIterator<QString,QString> iMajor(majorDataMap);
        while (iMajor.hasNext()) {
            iMajor.next();
            QTreeWidgetItem *item2 = new QTreeWidgetItem(FormCurriculum::itSecond);
            QMap<QString,QString> curriculumDdataMap = SqlManager::getInstance()->curriculumDataQuery("college_name",item1->text(0),"major_name",iMajor.value(),2);
            QMapIterator<QString,QString> iCurriculum(curriculumDdataMap);
            if(!curriculumDdataMap.isEmpty())
            {
                item2->setText(0,iMajor.value());
                item2->setIcon(0,QIcon(":/res/icon/hasfile.png"));
            }
            else
            {
                item2->setText(0,iMajor.value());
                item2->setIcon(0,QIcon(":/res/icon/nofile.png"));
            }
            item1->addChild(item2);
        }
        ui->treeWidget->setCurrentItem(item1);
    }

}

void FormCurriculum::removeItem(QTreeWidgetItem *item)
{
    int count = item->childCount();
    if(count == 0)//没有子节点，直接删除
    {
        delete item;
        return;
    }

    for(int i=0; i<count; i++)
    {
        QTreeWidgetItem *childItem = item->child(0);//删除子节点
        removeItem(childItem);
    }
    delete item;//最后将自己删除
    return;
}

void FormCurriculum::removeAllTreeItem()
{
    QTreeWidgetItem *item;
    item = ui->treeWidget->topLevelItem(0);
    int topCount =  ui->treeWidget->topLevelItemCount();
    for(int i =0;i<topCount;i++)
    {
        qDebug() << i;
        qDebug() << item;
        removeItem(item);
        item = ui->treeWidget->topLevelItem(0);
    }
}

void FormCurriculum::showModel(QSqlQueryModel *queryModel)
{
    queryModel->setHeaderData(0,Qt::Horizontal,tr("课程编号"));
    queryModel->setHeaderData(1,Qt::Horizontal,tr("课程名称"));
    queryModel->setHeaderData(2,Qt::Horizontal,tr("教师编号"));
    queryModel->setHeaderData(3,Qt::Horizontal,tr("教师名称"));
    queryModel->setHeaderData(4,Qt::Horizontal,tr("学院编号"));
    queryModel->setHeaderData(5,Qt::Horizontal,tr("学院名称"));
    queryModel->setHeaderData(6,Qt::Horizontal,tr("专业编号"));
    queryModel->setHeaderData(7,Qt::Horizontal,tr("专业名称"));
    queryModel->setHeaderData(8,Qt::Horizontal,tr("班级人数"));
    queryModel->setHeaderData(9,Qt::Horizontal,tr("创建时间"));
}

// 添加
void FormCurriculum::on_tBtnAdd_clicked()
{
    DialogCurriculum *dlgCD = new DialogCurriculum(this);
    Qt::WindowFlags    flags=dlgCD->windowFlags();
    dlgCD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小
    QSqlRecord curRec=queryModel->record();//获取当前记录,实际为空记录
    dlgCD->setInsertRecord(curRec);
    int res = dlgCD->exec();
    if(res == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgCD->getRecordData();
        if(recData.value("curriculum_id").toString().isEmpty() || recData.value("curriculum_name").toString().isEmpty())
        {
            QMessageBox::information(this,"提示信息","不能为空",QMessageBox::Ok,QMessageBox::NoButton);

        }
        else
        {
            if(SqlManager::getInstance()->curriculumDataInsert
                (recData.value("curriculum_id").toString(),recData.value("curriculum_name").toString(),
                recData.value("tea_id").toString(),recData.value("major_id").toString(),
                recData.value("college_id").toString()))
            {
                QMessageBox::information(this,"提示信息","添加成功",QMessageBox::Ok,QMessageBox::NoButton);
            }
            else
            {
                QMessageBox::information(this, "消息", "数据添加错误,错误信息\n"+queryModel->lastError().text(),QMessageBox::Ok,QMessageBox::NoButton);
            }
        }
    }
    on_tBtnRefresh_clicked();
}

// 删除
void FormCurriculum::on_tBtnDelete_clicked()
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
            QString curriculum_id = var.value<QString>();
            if(!SqlManager::getInstance()->formCurriuclumDelete(curriculum_id))
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
void FormCurriculum::on_tBtnRefresh_clicked()
{
    int itType = ui->treeWidget->currentItem()->type();
    QString curStr = ui->treeWidget->currentItem()->text(0);
    if(itType == FormCurriculum::itTop)
    {
        queryModel = SqlManager::getInstance()->curriculumDataQuery("college_name",curStr); // 接受数据
        showModel(queryModel);
    }
    else if(itType == FormCurriculum::itSecond)
    {
        QTreeWidgetItem *parent = ui->treeWidget->currentItem()->parent();
        QString strPar = parent->text(0);
        queryModel = SqlManager::getInstance()->curriculumDataQuery("college_name",strPar,"major_name",curStr); // 接受数据
        showModel(queryModel);
    }
    ui->tBtnAdd->setEnabled(false);
    ui->tBtnEdit->setEnabled(false);
    ui->tBtnDelete->setEnabled(false);
}

// 编辑
void FormCurriculum::on_tBtnEdit_clicked()
{
    updateRecord(theSelection->currentIndex().row());
}

void FormCurriculum::updateRecord(int recNo)
{
    auto c_id = queryModel->data(queryModel->index(recNo,0),Qt::DisplayRole); // 获取id
    QString strid= c_id.value<QString>();

    DialogCurriculum *dlgCD = new DialogCurriculum(this);
    Qt::WindowFlags    flags= dlgCD->windowFlags();
    dlgCD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlRecord curRec=queryModel->record(recNo);//获取当前记录,实际为空记录

    dlgCD->setUpdateRecord(curRec);
    if(dlgCD->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgCD->getRecordData();
        if(recData.value("curriculum_id").toString().isEmpty() || recData.value("curriculum_name").toString().isEmpty())
        {
            QMessageBox::information(this,"提示信息","不能为空",QMessageBox::Ok,QMessageBox::NoButton);

        }
        else
        {
            if(SqlManager::getInstance()->curriculumDataUpdate(
                        strid,recData.value("curriculum_id").toString(),recData.value("curriculum_name").toString(),
                        recData.value("tea_id").toString(),recData.value("major_id").toString(),
                        recData.value("college_id").toString()))
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

// 双击编辑
void FormCurriculum::on_tableView_doubleClicked(const QModelIndex &index)
{
    int curRecNo = index.row();
    updateRecord(curRecNo);
}

void FormCurriculum::on_tableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->tBtnAdd->setEnabled(true);
    ui->tBtnEdit->setEnabled(true);
    ui->tBtnDelete->setEnabled(true);
}

// item 变化
void FormCurriculum::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    int itType = current->type();
    QString curStr = current->text(0);
    if(itType == FormCurriculum::itTop)
    {
        queryModel = SqlManager::getInstance()->curriculumDataQuery("college_name",curStr); // 接受数据
        showModel(queryModel);
    }
    else if(itType == FormCurriculum::itSecond)
    {
        QTreeWidgetItem *parent = current->parent();
        QString strPar = parent->text(0);
        queryModel = SqlManager::getInstance()->curriculumDataQuery("college_name",strPar,"major_name",curStr); // 接受数据
        showModel(queryModel);
    }
}

void FormCurriculum::on_tBtnShowAll_clicked()
{
    queryModel = SqlManager::getInstance()->curriculumDataQuery(); // 接受数据
    showModel(queryModel);
}
