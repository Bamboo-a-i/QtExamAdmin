#ifndef FORMCEXAMOINPUT_H
#define FORMCEXAMOINPUT_H

#include <QWidget>

namespace Ui {
class FormCExamOInput;
}

class FormCExamOInput : public QWidget
{
    Q_OBJECT

public:
    explicit FormCExamOInput(QWidget *parent = nullptr);
    ~FormCExamOInput();

private:
    Ui::FormCExamOInput *ui;
};

#endif // FORMCEXAMOINPUT_H
