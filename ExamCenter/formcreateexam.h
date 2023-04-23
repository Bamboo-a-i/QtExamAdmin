#ifndef FORMCREATEEXAM_H
#define FORMCREATEEXAM_H

#include <QWidget>

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
    void initForm();


    void on_tBtnExcelDownload_clicked();

    void on_tBtnUploadTemplate_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FormCreateExam *ui;
};

#endif // FORMCREATEEXAM_H
