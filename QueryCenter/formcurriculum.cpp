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
    initForm();
    initScreen();
}

FormCurriculum::~FormCurriculum()
{
    delete ui;
}

void FormCurriculum::refresh()
{
    initForm();
}

// 初始化页面
void FormCurriculum::initForm()
{
    // 表格设置
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 全部展开
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 表格不可编辑
    ui->tableView->verticalHeader()->hide(); // 不显示行号
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中行

    queryModel = SqlManager::getInstance()->curriculumDataQuery(); // 接受数据
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

    theSelection = new QItemSelectionModel(queryModel); // 关联模型
    ui->tableView->setModel(queryModel); // 数据模型
    ui->tableView->setSelectionModel(theSelection); //选择模型
}

void FormCurriculum::initScreen()
{
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("formcurriculum");
    QMapIterator<QString, QString> ic(collegeDataMap);
    while (ic.hasNext()) {
      ic.next();
      ui->comboCollege->addItem(ic.value());
    }

     QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_name",ui->comboCollege->currentText());
     QMapIterator<QString, QString> im(majorDataMap);
     while (im.hasNext()) {
       im.next();
       ui->comboMajor->addItem(im.value());
     }
}

// 添加
void FormCurriculum::on_tBtnAdd_clicked()
{
    DialogCurriculum *dlgCD = new DialogCurriculum(this);
    Qt::WindowFlags    flags=dlgCD->windowFlags();
    dlgCD->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小
    QSqlRecord curRec=queryModel->record();//获取当前记录,实际为空记录
    dlgCD->setInsertRecord(curRec);
    if(dlgCD->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgCD->getRecordData();
        if(SqlManager::getInstance()->curriculumDataInsert
                (recData.value("curriculum_id").toString(),recData.value("curriculum_name").toString(),
                 recData.value("tea_id").toString(),recData.value("major_id").toString(),
                 recData.value("college_id").toString()))
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
    initForm();
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
    on_tBtnRefresh_clicked(); // 刷新
}

void FormCurriculum::on_tableView_doubleClicked(const QModelIndex &index)
{
    int curRecNo = index.row();
    updateRecord(curRecNo);
}

void FormCurriculum::on_tableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->tBtnEdit->setEnabled(true);
    ui->tBtnDelete->setEnabled(true);
}

void FormCurriculum::on_tBtnScreen_clicked()
{
    queryModel = SqlManager::getInstance()->curriculumDataQuery("college_name",ui->comboCollege->currentText(),"major_name",ui->comboMajor->currentText());
    // 表格设置

    queryModel->setHeaderData(0,Qt::Horizontal,tr("课程编号"));
    queryModel->setHeaderData(1,Qt::Horizontal,tr("课程名称"));
    queryModel->setHeaderData(2,Qt::Horizontal,tr("教师编号"));
    queryModel->setHeaderData(3,Qt::Horizontal,tr("教师名称"));
    queryModel->setHeaderData(4,Qt::Horizontal,tr("学院编号"));
    queryModel->setHeaderData(5,Qt::Horizontal,tr("学院名称"));
    queryModel->setHeaderData(6,Qt::Horizontal,tr("专业编号"));
    queryModel->setHeaderData(7,Qt::Horizontal,tr("专业名称"));
    queryModel->setHeaderData(9,Qt::Horizontal,tr("班级人数"));
    queryModel->setHeaderData(10,Qt::Horizontal,tr("创建时间"));
}

void FormCurriculum::on_comboCollege_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_name",ui->comboCollege->currentText());
    QMapIterator<QString, QString> im(majorDataMap);
    ui->comboMajor->clear();
    while (im.hasNext()) {
      im.next();
      ui->comboMajor->addItem(im.value());
    }
}



