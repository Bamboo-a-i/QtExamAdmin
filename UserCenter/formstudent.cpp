#include "formstudent.h"
#include "ui_formstudent.h"
#include <QMessageBox>
#include "sqlmanager.h"
#include "dialogstudent.h"
#include <QCryptographicHash>
#include <QSqlError>
FormStudent::FormStudent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormStudent)
{
    ui->setupUi(this);
    initForm();
    initScreen();
}

FormStudent::~FormStudent()
{
    delete ui;
}

void FormStudent::refresh()
{
    initForm();
}

// 初始化页面
void FormStudent::initForm()
{
    // 表格设置
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 全部展开
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 表格不可编辑
    ui->tableView->verticalHeader()->hide(); // 不显示行号
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中行

    connect(ui->tBtnClassGroup,SIGNAL(clicked()),this,SLOT(on_tBtnGroup_clicked()));
    connect(ui->tBtnCollegeGroup,SIGNAL(clicked()),this,SLOT(on_tBtnGroup_clicked()));
    connect(ui->tBtnGradeGroup,SIGNAL(clicked()),this,SLOT(on_tBtnGroup_clicked()));
    connect(ui->tBtnMajorGroup,SIGNAL(clicked()),this,SLOT(on_tBtnGroup_clicked()));

    queryModel = SqlManager::getInstance()->studentDataQuery(); // 接受数据
    theSelection = new QItemSelectionModel(queryModel); // 关联模型
    ui->tableView->setModel(queryModel); // 数据模型
    ui->tableView->setSelectionModel(theSelection); //选择模型
    initTableView(queryModel);
}

void FormStudent::initTableView(QSqlQueryModel *queryModel)
{
    queryModel->setHeaderData(0,Qt::Horizontal,tr("学生编号"));
    queryModel->setHeaderData(1,Qt::Horizontal,tr("学生姓名"));
    queryModel->setHeaderData(2,Qt::Horizontal,tr("学生性别"));
    queryModel->setHeaderData(3,Qt::Horizontal,tr("年级"));
    queryModel->setHeaderData(4,Qt::Horizontal,tr("班级编号"));
    queryModel->setHeaderData(5,Qt::Horizontal,tr("班级名称"));
    queryModel->setHeaderData(6,Qt::Horizontal,tr("专业编号"));
    queryModel->setHeaderData(7,Qt::Horizontal,tr("专业名称"));
    queryModel->setHeaderData(8,Qt::Horizontal,tr("学院编号"));
    queryModel->setHeaderData(9,Qt::Horizontal,tr("学院名称"));
    queryModel->setHeaderData(10,Qt::Horizontal,tr("创建时间"));
}

void FormStudent::screenClass()
{
    // 班级
    QMap<QString,QString> classDataMap = SqlManager::getInstance()->classDataQuery
            ("class_grade",ui->comboGrade->currentText(),
             "college_name",ui->comboCollege->currentText(),
             "major_name",ui->comboMajor->currentText(),"dialogstudent");
    QMapIterator<QString, QString> iclass(classDataMap);
    ui->comboClass->clear();
    while (iclass.hasNext()) {
        iclass.next();
        ui->comboClass->addItem(iclass.value());
    }
}

void FormStudent::initScreen()
{
    // 学院
    QMap<QString,QString> collegeDataMap = SqlManager::getInstance()->collegeDataQuery("formstudent");
    QMapIterator<QString, QString> ic(collegeDataMap);
    while (ic.hasNext()) {
      ic.next();
      ui->comboCollege->addItem(ic.value());
    }

    // 专业
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_name",ui->comboCollege->currentText(),1);
    QMapIterator<QString, QString> im(majorDataMap);
    while (im.hasNext()) {
        im.next();
        ui->comboMajor->addItem(im.value());
    }
}

// 添加
void FormStudent::on_tBtnAdd_clicked()
{
    DialogStudent *dlgStu = new DialogStudent(this);
    Qt::WindowFlags    flags=dlgStu->windowFlags();
    dlgStu->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlRecord curRec=queryModel->record();//获取当前记录,实际为空记录
    dlgStu->setInsertRecord(curRec);

    QString userDefaultPswd = QCryptographicHash::hash(QString("123456").toLatin1(),QCryptographicHash::Md5).toHex();
    if(dlgStu->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgStu->getRecordData();
        qDebug() << recData;
        if(SqlManager::getInstance()->studentDataInsert
                (recData.value("stu_id").toString(),recData.value("stu_name").toString(),
                 recData.value("stu_gender").toString(),userDefaultPswd,recData.value("class_id").toString()))

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
void FormStudent::on_tBtnDelete_clicked()
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
            QString stu_id = var.value<QString>();
            if(!SqlManager::getInstance()->formStudentDelete(stu_id))
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
void FormStudent::on_tBtnRefresh_clicked()
{

    initTableView(queryModel);
    ui->tBtnEdit->setEnabled(false);
    ui->tBtnDelete->setEnabled(false);
    ui->tBtnDefaultPswd->setEnabled(false);
}

// 编辑
void FormStudent::on_tBtnEdit_clicked()
{
    updateRecord(theSelection->currentIndex().row());
}

void FormStudent::updateRecord(int recNo)
{
    auto studentId = queryModel->data(queryModel->index(recNo,0),Qt::DisplayRole); // 获取id
    QString strStudentId= studentId.value<QString>();

    DialogStudent *dlgStu = new DialogStudent(this);
    Qt::WindowFlags    flags= dlgStu->windowFlags();
    dlgStu->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    QSqlRecord curRec=queryModel->record(recNo);//获取当前记录,实际为空记录

    dlgStu->setUpdateRecord(curRec);
    if(dlgStu->exec() == QDialog::Accepted)
    {
        QSqlRecord  recData=dlgStu->getRecordData();
        if(SqlManager::getInstance()->studentUpdate(strStudentId,recData.value("stu_id").toString(),recData.value("stu_name").toString(),recData.value("stu_gender").toString(),recData.value("class_id").toString()))
            QMessageBox::information(this,"提示信息","修改成功",QMessageBox::Ok,QMessageBox::NoButton);
        else
            QMessageBox::information(this, "消息", "数据修改错误,错误信息\n"+queryModel->lastError().text(),QMessageBox::Ok,QMessageBox::NoButton);
    }
    on_tBtnRefresh_clicked(); // 刷新
}

void FormStudent::on_tableView_doubleClicked(const QModelIndex &index)
{
    int curRecNo = index.row();
    updateRecord(curRecNo);
}

void FormStudent::on_tableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->tBtnEdit->setEnabled(true);
    ui->tBtnDelete->setEnabled(true);
    ui->tBtnDefaultPswd->setEnabled(true);
}

void FormStudent::on_tBtnScreen_clicked()
{
    queryModel = SqlManager::getInstance()->studentDataQuery("class_grade",ui->comboGrade->currentText(),"major_name",ui->comboMajor->currentText(),"class_name",ui->comboClass->currentText());
    // 表格设置
    initTableView(queryModel);
}

void FormStudent::on_comboMajor_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->classDataQuery("major_name",ui->comboMajor->currentText());
    QMapIterator<QString, QString> iClass(majorDataMap);
    ui->comboClass->clear();
    while (iClass.hasNext()) {
      iClass.next();
      ui->comboClass->addItem(iClass.value());
    }
    screenClass();
}

void FormStudent::on_tBtnGroup_clicked()
{
    QToolButton *b = (QToolButton *)sender();
    QString tBtnName = b->text();
    if(tBtnName == "按学院分组")
        queryModel = SqlManager::getInstance()->studentDataGroupQuery("college_name");
    else if(tBtnName == "按专业分组")
        queryModel = SqlManager::getInstance()->studentDataGroupQuery("major_name");
    else if(tBtnName == "按年级分组")
        queryModel = SqlManager::getInstance()->studentDataGroupQuery("class_grade");
    else
        queryModel = SqlManager::getInstance()->studentDataGroupQuery("class_name");
    initTableView(queryModel);
}

void FormStudent::on_comboCollege_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QMap<QString,QString> majorDataMap = SqlManager::getInstance()->majorDataQuery("college_name",ui->comboCollege->currentText(),1);
    QMapIterator<QString, QString> im(majorDataMap);
    ui->comboMajor->clear();
    while (im.hasNext()) {
      im.next();
      ui->comboMajor->addItem(im.value());
    }
    screenClass();
}

void FormStudent::on_tBtnDefaultPswd_clicked()
{
    auto studentId = queryModel->data(queryModel->index(theSelection->currentIndex().row(),0),Qt::DisplayRole); // 获取id
    QString strStudentId= studentId.value<QString>();
    int res = QMessageBox::information(this,"提示信息",QString("是否初始化 %1 的用户密码").arg(strStudentId),QMessageBox::Ok,QMessageBox::No);
    if(res ==QMessageBox::Ok)
    {
        QString userDefaultPswd = QCryptographicHash::hash(QString("123456").toLatin1(),QCryptographicHash::Md5).toHex();
        if(SqlManager::getInstance()->studentPswdUpdate(strStudentId,userDefaultPswd))
            QMessageBox::information(this,"提示信息","初始化密码成功",QMessageBox::Ok,QMessageBox::NoButton);
        else
            QMessageBox::information(this,"提示信息","初始化密码失败",QMessageBox::Ok,QMessageBox::NoButton);
    }
}

void FormStudent::on_tBtnQuery_clicked()
{
    queryModel = SqlManager::getInstance()->studentDataQuery(ui->leQuery->text());
    initTableView(queryModel);
}

void FormStudent::on_comboGrade_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    screenClass();
}

void FormStudent::on_tBtnShowAll_clicked()
{
    queryModel = SqlManager::getInstance()->studentDataQuery(); // 接受数据
    initTableView(queryModel);
}

