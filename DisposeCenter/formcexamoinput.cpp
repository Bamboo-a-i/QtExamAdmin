#include "formcexamoinput.h"
#include "ui_formcexamoinput.h"

FormCExamOInput::FormCExamOInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCExamOInput)
{
    ui->setupUi(this);
}

FormCExamOInput::~FormCExamOInput()
{
    delete ui;
}
