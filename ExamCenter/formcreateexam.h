#ifndef FORMCREATEEXAM_H
#define FORMCREATEEXAM_H

#include <QWidget>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QTreeWidgetItem>

namespace Ui {
class FormCreateExam;
}

class FormCreateExam : public QWidget
{
    Q_OBJECT

public:
    explicit FormCreateExam(QWidget *parent = nullptr);
    ~FormCreateExam();

private slots:
    void on_tBtnExcelDownload_clicked();

    void on_tBtnUpload_clicked();

    bool txtByStrream(const QString &filePath);

    bool excelRead(const QString &filePath);

    void on_tBtnTxtDownload_clicked();

    void on_btnNext_clicked();

    void on_btnPre_clicked();

    void on_btnNext1_clicked();

    void on_btnPre1_clicked();

    void on_btnNext2_clicked();

    void on_btnPre2_clicked();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_btnCommitExamination_clicked();


    void on_dteStart_dateTimeChanged(const QDateTime &dateTime);

private:
    enum treeItemtype{itTop = 1001,itSecond,itThird};
    void initForm();
    void initTree();
    void initCombo();
    bool createTestPaper();
    QTreeWidgetItem *createItem(int index);
    void on_finished();

private:

    Ui::FormCreateExam *ui;
    QNetworkAccessManager networkManger;
    QNetworkReply *reply;
    QFile *downloadedFile;
    QList<QList<QString>> questions; // 存储所有一次性导入的题目
};

#endif // FORMCREATEEXAM_H
