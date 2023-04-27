#include "formuserdata.h"
#include "ui_formuserdata.h"

FormUserData::FormUserData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormUserData)
{
    ui->setupUi(this);
}

FormUserData::~FormUserData()
{
    delete ui;
}
