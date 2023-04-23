#include "formclass.h"
#include "ui_formclass.h"
#include "sqlmanager.h"
#include "dialogmajordata.h"
#include <QDateTime>
#include <QAbstractItemView>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <dialogclassdata.h>

FormClass::FormClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormClass)
{
    ui->setupUi(this);
    initForm();
    initScreen();
}

FormClass::~FormClass()
{
    delete ui;
}
void FormClass::refresh()
{
    initForm();
}

// 初始化页面
void FormClass::initForm()
{
    // 表格设置
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 全部展开
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 表格不可编辑
    ui->tableView->verticalHeader()->hide(); // 不显示行号
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中行

    queryModel = SqlManager::getInstance()->classDataQuery(); // 接受数据
    queryModel->setHeaderData(0,Qt::Horizontal,tr("班级编号"));
    queryModel->setHeaderData(1,Qt::Horizontal,tr("班级名称"));
    queryModel->setHeaderData(2,Qt::Horizontal,tr("学院编号"));
    queryModel->setHeaderData(3,Qt::Horizontal,tr("学院名称"));
    queryModel->setHeaderData(4,Qt::Horizontal,tr("专业编号"));
    queryModel->setHeaderData(5,Qt::Horizontal,tr("专业名称"));
    queryModel->setHeaderData(6,Qt::Horizontal,tr("年级"));
    queryModel->setHeaderData(7,Qt::Horizontal,tr("班级人数"));
    queryModel->setHeaderData(8,Qt::Horizontal,tr("创建时间"));

    theSelection = new QItemSelectionModel(queryModel); // 关联模型
    ui->tableView->setModel(queryModel); // 数据模型
    ui->tableView->setSelectionModel(theSelection); //选择模型
}

void FormClass::initScreen()
{
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("formclass");
    QMapIterator<QString, QString> iClass(collegeDataMap);
    while (iClass.hasNext()) {
      iClass.next();
      ui->comboCollege->addItem(iClass.value());
    }

    // 年级和学院名称 查询专业
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("class_grade",ui->comboClassGrade->currentText(),"college_name",ui->comboCollege->currentText());
    QMapIterator<QString, QString> iMajor(majorDataMap);
    while (iMajor.hasNext()) {
        iMajor.next();
        ui->comboMajor->addItem(iMajor.value());
    }
}

// 添加
void FormClass::on_tBtnAdd_clicked()
{
    DialogClassData *dlgCD = new DialogClassData(this);
    Qt::WindowFlags    flags=dlgCD->windowFlags();
    dlgCD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlRecord curRec=queryModel->record();//获取当前记录,实际为空记录
    dlgCD->setInsertRecord(curRec);
    if(dlgCD->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgCD->getRecordData();
        if(SqlManager::getInstance()->classDataInsert(recData.value("class_id").toString(),recData.value("class_name").toString(),recData.value("class_grade").toString(),recData.value("major_id").toString(),recData.value("college_id").toString()))
        {
            QMessageBox::information(this,"提示信息","添加成功",QMessageBox::Ok,QMessageBox::NoButton);
        }
        else
        {
            QMessageBox::information(this, "消息", "数据添加错误,错误信息\n"+queryModel->lastError().text(),
                                     QMessageBox::Ok,QMessageBox::NoButton);
        }
    }
    on_tBtnRefresh_clicked();
}

// 删除
void FormClass::on_tBtnDelete_clicked()
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
            QString class_id = var.value<QString>();
            if(!SqlManager::getInstance()->formClassDelete(class_id))
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
void FormClass::on_tBtnRefresh_clicked()
{
    initForm();
    ui->tBtnEdit->setEnabled(false);
    ui->tBtnDelete->setEnabled(false);
}

// 编辑
void FormClass::on_tBtnEdit_clicked()
{
    updateRecord(theSelection->currentIndex().row());
}

void FormClass::updateRecord(int recNo)
{
    auto classId = queryModel->data(queryModel->index(recNo,0),Qt::DisplayRole); // 获取id
    QString strClassid= classId.value<QString>();

    DialogClassData *dlgCD = new DialogClassData(this);
    Qt::WindowFlags    flags= dlgCD->windowFlags();
    dlgCD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlRecord curRec=queryModel->record(recNo);//获取当前记录,实际为空记录

    dlgCD->setUpdateRecord(curRec);
    if(dlgCD->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgCD->getRecordData();
        if(SqlManager::getInstance()->classDataUpdate(
                    strClassid,recData.value("class_id").toString(),recData.value("class_name").toString(),recData.value("class_grade").toString(),recData.value("major_id").toString(),recData.value("college_id").toString()))
        {
            QMessageBox::information(this,"提示信息","修改成功",QMessageBox::Ok,QMessageBox::NoButton);
        }
        else
        {
            QMessageBox::information(this, "消息", "数据修改错误,错误信息\n"+queryModel->lastError().text(),
                                     QMessageBox::Ok,QMessageBox::NoButton);
        }
    }
    on_tBtnRefresh_clicked(); // 刷新
}

void FormClass::on_tableView_doubleClicked(const QModelIndex &index)
{
    int curRecNo = index.row();
    updateRecord(curRecNo);
}

void FormClass::on_tableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->tBtnEdit->setEnabled(true);
    ui->tBtnDelete->setEnabled(true);
}

void FormClass::on_tBtnScreen_clicked()
{
    queryModel = SqlManager::getInstance()->classDataQuery("class_grade",ui->comboClassGrade->currentText(),"college_name",ui->comboCollege->currentText(),"major_name",ui->comboMajor->currentText());
    // 表格设置

    queryModel->setHeaderData(0,Qt::Horizontal,tr("班级编号"));
    queryModel->setHeaderData(1,Qt::Horizontal,tr("班级名称"));
    queryModel->setHeaderData(2,Qt::Horizontal,tr("学院编号"));
    queryModel->setHeaderData(3,Qt::Horizontal,tr("学院名称"));
    queryModel->setHeaderData(4,Qt::Horizontal,tr("专业编号"));
    queryModel->setHeaderData(5,Qt::Horizontal,tr("专业名称"));
    queryModel->setHeaderData(6,Qt::Horizontal,tr("年级"));
    queryModel->setHeaderData(7,Qt::Horizontal,tr("班级人数"));
    queryModel->setHeaderData(8,Qt::Horizontal,tr("创建时间"));
}

void FormClass::on_comboCollege_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("class_grade",ui->comboClassGrade->currentText(),"college_name",ui->comboCollege->currentText());
    QMapIterator<QString, QString> iMajor(majorDataMap);
    ui->comboMajor->clear();
    while (iMajor.hasNext()) {
      iMajor.next();
      ui->comboMajor->addItem(iMajor.value());
    }
}
