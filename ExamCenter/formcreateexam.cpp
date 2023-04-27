#include "formcreateexam.h"
#include "ui_formcreateexam.h"

#include <QDebug>
#include "scrolltext.h"
#include "sqlmanager.h"
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QAxObject>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QTextEdit>
#include <QUuid>

FormCreateExam::FormCreateExam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCreateExam)
{
    ui->setupUi(this);
    initForm();
    initCombo();
}

FormCreateExam::~FormCreateExam()
{
    delete ui;
}

void FormCreateExam::initForm()
{
    // 模板下载和上传文件
    ui->stackedWidget->setCurrentIndex(0);
    ui->dteStart->setDateTime(QDateTime::currentDateTime());
}



// 下载模板
void FormCreateExam::on_tBtnExcelDownload_clicked()
{
//    QString curPath = QCoreApplication::applicationDirPath();
//    QString dlgTitle = "选择保存目录";
//    QString selectedDir = QFileDialog::getExistingDirectory(this,dlgTitle,curPath,QFileDialog::ShowDirsOnly);

//    QString urlStr = "https://iceimgurl.oss-cn-beijing.aliyuncs.com/%E6%95%99%E5%B8%88%E9%A2%98%E7%9B%AE%E5%AF%BC%E5%85%A5%E6%A8%A1%E6%9D%BF.xlsx";
//    QUrl newUrl = QUrl::fromUserInput(urlStr);

//    QString fullFileName = curPath+newUrl.fileName();

//    if (QFile::exists(fullFileName))
//        QFile::remove(fullFileName);

//    downloadedFile =new QFile(fullFileName);//创建临时文件
//    if (!downloadedFile->open(QIODevice::WriteOnly))
//    {
//        QMessageBox::information(this, tr("错误"),"临时文件打开错误");
//        return;
//    }

//    reply = networkManger.get(QNetworkRequest(newUrl));
//    connect(reply, SIGNAL(finished()), this, SLOT(on_finished()));
}

void FormCreateExam::on_tBtnTxtDownload_clicked()
{

}

void FormCreateExam::on_finished()
{//网络响应结束
//    QFileInfo fileInfo;
//    fileInfo.setFile(downloadedFile->fileName());

//    downloadedFile->close();
//    delete downloadedFile;
//    downloadedFile = Q_NULLPTR;

//    reply->deleteLater(); //
    //    reply = Q_NULLPTR;
}

// 上传
void FormCreateExam::on_tBtnUpload_clicked()
{
    QString curPath = QDir::currentPath();
    QString dlgTitle = "选择一个文件";
    QString filter = "excel文件(*.xls);;excel文件(*.xlsx);;文本文件(*.txt)";
    QString filePath = QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    QFileInfo fileInfo = QFileInfo(filePath);
    // 文件成功读入到questions
    if(fileInfo.suffix() == "txt"){
        if(txtByStrream(filePath)){
            QMessageBox::information(this, tr("信息"),"导入成功");
            initTree();
            ui->btnNext->setEnabled(true);
        }
    }
    else if(fileInfo.suffix() == "xls" || fileInfo.suffix() == "xlsx"){
        if(excelRead(filePath)){
            QMessageBox::information(this, tr("信息"),"导入成功");
            initTree();
            ui->btnNext->setEnabled(true);
        }
    }
}

// 题目生成
void FormCreateExam::initTree()
{

    QTreeWidgetItem *item1 = new QTreeWidgetItem(FormCreateExam::itTop);
    QTreeWidgetItem *item2 = new QTreeWidgetItem(FormCreateExam::itTop);
    QTreeWidgetItem *item3 = new QTreeWidgetItem(FormCreateExam::itTop);
    QTreeWidgetItem *item4 = new QTreeWidgetItem(FormCreateExam::itTop);
    QTreeWidgetItem *item5 = new QTreeWidgetItem(FormCreateExam::itTop);
    item1->setText(0,"单选题");
    item1->setIcon(0,QIcon(":/res/icon/dir.png"));
    item2->setText(0,"多选题");
    item2->setIcon(0,QIcon(":/res/icon/dir.png"));
    item3->setText(0,"判断题");
    item3->setIcon(0,QIcon(":/res/icon/dir.png"));
    item4->setText(0,"填空题");
    item4->setIcon(0,QIcon(":/res/icon/dir.png"));
    item5->setText(0,"简答题");
    item5->setIcon(0,QIcon(":/res/icon/dir.png"));
    ui->treeWidget->addTopLevelItem(item1);
    ui->treeWidget->addTopLevelItem(item2);
    ui->treeWidget->addTopLevelItem(item3);
    ui->treeWidget->addTopLevelItem(item4);
    ui->treeWidget->addTopLevelItem(item5);

    for (int i =0;i<questions.size();i++) {
        QList<QString> question = questions[i];

        if(questions[i][0] == "单选题"){
            QTreeWidgetItem *item = createItem(i);
            item1->addChild(item);
        }
        else if(questions[i][0] == "多选题"){
            QTreeWidgetItem *item = createItem(i);
            item2->addChild(item);
        }
        else if(questions[i][0] == "判断题"){
            QTreeWidgetItem *item = createItem(i);
            item3->addChild(item);
        }
        else if(questions[i][0] == "填空题"){
            QTreeWidgetItem *item = createItem(i);
            item4->addChild(item);
        }
        if(questions[i][0] == "简答题"){
            QTreeWidgetItem *item = createItem(i);
            item5->addChild(item);
        }
    }

    // 设置二级层text
    int questionCount = 0;
    for(int i = 0;i<ui->treeWidget->topLevelItemCount();i++){
        for(int j=0;j<ui->treeWidget->topLevelItem(i)->childCount();j++){
            questionCount++;
            ui->treeWidget->topLevelItem(i)->child(j)->setText(0,"题-"+QString::number(questionCount));
        }
    }
    ui->treeWidget->setCurrentItem(item1->child(0));
}

// 初始page3的combobox
void FormCreateExam::initCombo()
{
    QMap<QString,QString> examHDataMap = SqlManager::getInstance()->examHallDataQuery();
    QMapIterator<QString, QString> iExamH(examHDataMap);
    while (iExamH.hasNext()) {
        iExamH.next();
        ui->comboExamHall->setProperty("examH_id",iExamH.key());
        ui->comboExamHall->addItem(iExamH.value());
    }
}

// 创建试卷
bool FormCreateExam::createTestPaper()
{
    // question << type << tigan << options << biaoda << score << star << jiexi;
   //question_id,question_type, question_tigan, question_options, question_biaoda, question_score, question_star, question_jiexi);
    // 生成试卷id
    QUuid testPId = QUuid::createUuid();
    QString strTestId = testPId.toString().remove("-").remove("{").remove("}");

    // 插入试卷信息
    // .arg(testP_id).arg(testP_name).arg(tea_id).arg(curriculum_id).arg(testP_comment));
    SqlManager::getInstance()->testPaperDataInsert(strTestId,ui->leTPName->text(),ui->leCTeaId->text(),ui->leCurriculumId->text());

    // 开始插入题目
    for (int i = 0;i<questions.count();i++) {
        QUuid questionId = QUuid::createUuid();
        QString strQuestionId = questionId.toString().remove("-").remove("{").remove("}");
        QString strScore = questions[i][4];
        int intScore = strScore.toInt();
        SqlManager::getInstance()->questionsDataInsert(
                    strQuestionId,questions[i][0],questions[i][1],questions[i][2],questions[i][3],intScore,questions[i][5],questions[i][6],strTestId);
    }

    // 插入考试信息
    //  .arg(examination_id).arg(examination_name).arg(examination_comment).arg(examination_duration).arg(examination_start_time).arg(examination_end_time).arg(testP_id));
    QUuid examinationId = QUuid::createUuid();
    QString strExaminationId = examinationId.toString().remove("-").remove("{").remove("}");


    SqlManager::getInstance()->examinationDataInsert
            (strExaminationId,ui->leEName->text(),ui->leEComment->text(),
             ui->leDuration->text().toInt(),ui->dteStart->text(),ui->dteEnd->text(),strTestId,ui->comboExamHall->property("examH_id").toString());
}

QTreeWidgetItem *FormCreateExam::createItem(int index)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(FormCreateExam::itSecond);
    item->setIcon(0,QIcon(":/res/icon/dir1.png"));
    item->setData(0,Qt::UserRole,QVariant(index)); // 存储questions位置
    qDebug() << item->data(0,Qt::UserRole);
    return item;
}



/* 数据处理部分*/
bool FormCreateExam::txtByStrream(const QString &filePath)
{
    QFile file(filePath);

    if(!file.exists())
    {
        QMessageBox::information(this, tr("错误"),"文件不存在");
        return false;
    }
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        QMessageBox::information(this, tr("错误"),"文件打开失败");
        return false;
    }

    QTextStream stream(&file);
    stream.setAutoDetectUnicode(true);
    QString readLine;
    QString type;
    QString tigan;
    QString options;
    QString biaoda;
    QString score;
    QString star;
    QString jiexi;
    while (!stream.atEnd()) {
        readLine = stream.readLine();
        if(readLine != ""){
            if(readLine.mid(0,3) == "题干:" || readLine.mid(0,3) == "题干："){
                tigan = readLine.mid(3);
                continue;
            }
            else if (readLine.mid(0,3) == "答案:" || readLine.mid(0,3) == "答案：") {
                biaoda = readLine.mid(3);
                continue;
            }
            else if (readLine.mid(0,3) == "分数:" || readLine.mid(0,3) == "分数：") {
                score = readLine.mid(3);
                continue;
            }
            else if (readLine.mid(0,3) == "难度:" || readLine.mid(0,3) == "难度：") {
                star = readLine.mid(3);
                continue;
            }
            else if (readLine.mid(0,3) == "解析:" || readLine.mid(0,3) == "解析：") {
                jiexi = readLine.mid(3);
                continue;
            }
            else if (readLine.mid(0,3) == "题型:" || readLine.mid(0,3) == "题型：") {
                type = readLine.mid(3);
                continue;
            }
            else{
                options += readLine+"//";
            }
        }
        else{
            QList<QString> question;
            question << type << tigan << options << biaoda << score << star << jiexi;
            questions.append(question);
            options = ""; // 清空
            continue;
        }
    }

    if(questions.isEmpty()){
        QMessageBox::information(this, tr("错误"),"试题模板为空或试题格式不符合规定");
        return false;
    }
    return true;
}

bool FormCreateExam::excelRead(const QString &filePath)
{    // 1.连接excel
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible",false);
    excel.setProperty("DisplayAlerts",false);

    // 2.打开工作簿
    QAxObject *workBooks = excel.querySubObject("WorkBooks");
    if(!workBooks){
        QMessageBox::information(this, tr("错误"),"工作簿获取失败");
        return false;
    }

    QAxObject *workbook = workBooks->querySubObject("Open(const QString&)",(filePath));

    // 3.打开sheet
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1); // 获取第一个工作表

    // 4.获取范围
    QAxObject *usedRange = worksheet->querySubObject("UsedRange");

    // 5.读取数据
    QVariant var = usedRange->dynamicCall("Value");
    QList<QVariant> varList1 = var.toList();
    for (int i = 1;i<varList1.size();i++) {
        QList<QVariant> varlist2 = varList1[i].toList();
        QList<QString> question;
        for (int j=0;j<varlist2.size();j++) {
            question.append(varlist2.value(j).toString());
        }
        questions.append(question);
    }
    if(questions.count() == 0){
        QMessageBox::information(this, tr("错误"),"试题模板为空或试题格式不符合规定");
        return false;
    }
    return true;
}

/* stackedpage 之间的切换 */
// stackedpage1
void FormCreateExam::on_btnNext_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

// stackedpage2
void FormCreateExam::on_btnPre_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FormCreateExam::on_btnNext1_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// stackedpage3
void FormCreateExam::on_btnPre1_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void FormCreateExam::on_btnNext2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// stackedpage4
void FormCreateExam::on_btnPre2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// stackedpage2 中的题目展示
void FormCreateExam::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if(current->type() != FormCreateExam::itSecond)
        return;
    // 清除layout布局
    while (ui->vLayoutPreview->count())
    {
        QWidget *p = ui->vLayoutPreview->itemAt(0)->widget();
        if(p)
        {
            p->setParent(NULL);
            ui->vLayoutPreview->removeWidget(p);
            delete p;
        }
    }
    // question << type << tigan << options << biaoda << score << star << jiexi;
    QString itemText = current->parent()->text(0);
    int index = current->data(0,Qt::UserRole).toInt();
    QLabel *tigan = new QLabel(questions[index][1]); // 题干
    ui->vLayoutPreview->addWidget(tigan);
    if(itemText == "单选题"){
        QStringList options = questions[index][2].split("//");
        for (int i =0;i<options.count()-1;i++) {
            QRadioButton *radio = new QRadioButton(options[i]);
            ui->vLayoutPreview->addWidget(radio);
        }
    }
    else if(itemText == "多选题"){
        QStringList options = questions[index][2].split("//");
        for (int i =0;i<options.count()-1;i++) {
            QCheckBox *chkBox = new QCheckBox(options[i]);
            ui->vLayoutPreview->addWidget(chkBox);
        }
    }
    else if(itemText == "判断题"){
        QRadioButton *radioYes = new QRadioButton("是");
        QRadioButton *radioNo = new QRadioButton("否");
        ui->vLayoutPreview->addWidget(radioYes);
        ui->vLayoutPreview->addWidget(radioNo);
    }
    else if(itemText == "填空题"){
        int count = questions[index][1].count("()",Qt::CaseInsensitive );
        for (int i =0 ;i<count;i++) {
            QLabel *label = new QLabel(QString("空:%1").arg(i+1));
            QLineEdit *lineEdit = new QLineEdit;
            ui->vLayoutPreview->addWidget(label);
            ui->vLayoutPreview->addWidget(lineEdit);
        }
    }
    else{
        QTextEdit *jianda = new QTextEdit();
        jianda->setMaximumHeight(200);
        ui->vLayoutPreview->addWidget(jianda);
    }
    QLabel *biaoda = new QLabel("答案:"+questions[index][3]);
    ui->vLayoutPreview->addWidget(biaoda);
    QLabel *score = new QLabel("分数:"+questions[index][4]);
    ui->vLayoutPreview->addWidget(score);
    QLabel *star = new QLabel("难度:"+questions[index][5]);
    ui->vLayoutPreview->addWidget(star);

    ui->btnNext1->setEnabled(true);
}

// 提交考试
void FormCreateExam::on_btnCommitExamination_clicked()
{
    if(ui->leTPName->text().isEmpty()){
        ui->widgetTips->textlable->setText("试卷名称为空");
        ui->widgetTips->animationStart();
        return;
    }
    else if(ui->leCTeaId->text().isEmpty()){
        ui->widgetTips->textlable->setText("教师编号为空");
        ui->widgetTips->animationStart();
        return;
    }
    else if(ui->leEName->text().isEmpty()){
        ui->widgetTips->textlable->setText("考试名称为空");
        ui->widgetTips->animationStart();
        return;
    }
    else if(ui->leCurriculumId->text().isEmpty()){
        ui->widgetTips->textlable->setText("参考课程班级编号为空");
        ui->widgetTips->animationStart();
        return;
    }
    else if(ui->leDuration->text().isEmpty()){
        ui->widgetTips->textlable->setText("考试时长未设置");
        ui->widgetTips->animationStart();
        return;
    }

//    ui->dteEnd->setDateTime(ui->dteStart->dateTime()+ui->leDuration->text());
    if(!SqlManager::getInstance()->teacherExistQuery(ui->leCTeaId->text())){
        QMessageBox::information(this, tr("错误"),"该教师不存在，请仔细核对");
        return;
    }

    if(!SqlManager::getInstance()->curriculumExistQuery(ui->leCurriculumId->text())){
        QMessageBox::information(this, tr("错误"),"该课程不存在，请仔细核对");
        return;
    }

    if(createTestPaper()){
        QMessageBox::information(this, tr("成功"),"考试已成功发布");
        this->close();
    }
    else{
        QMessageBox::information(this, tr("错误"),"考试发布失败");
        this->close();
    }

}



void FormCreateExam::on_dteStart_dateTimeChanged(const QDateTime &dateTime)
{
    int minute = ui->leDuration->text().toInt();
    ui->dteEnd->setDateTime(dateTime.addSecs(minute*60));
    qDebug() << ui->dteEnd->dateTime();
}
