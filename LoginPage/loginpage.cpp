#include "loginpage.h"
#include "ui_loginpage.h"
#include "sqlmanager.h"
#include "mainpage.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    initPage();
}

LoginPage::~LoginPage()
{
    qDebug() << "结束";
    delete ui;
}

void LoginPage::initPage()
{
    this->resize(600,400);
    this->setWindowTitle("管理员登录");
    settings = new Settings();
    if(settings->isExist())
    {
        QStringList userInfo = settings->userInfo();
        ui->lineEditUserid->setText(userInfo[0]); // 显示上一次登录的账号名
        if (userInfo[2] == "true") // 记住密码
        {
            ui->lineEditPwsd->setText(userInfo[1]); // 密码写在密码输入框
            ui->chkRemember->setChecked(true); // 记住密码打√
        }
        else
        {
            ui->chkRemember->setChecked(false); // 记住密码不打√
        }
    }
    else
        ui->chkRemember->setChecked(false);
}

// 登录
void LoginPage::on_btnLogin_clicked()
{
    QString user_id = ui->lineEditUserid->text();
    QString user_pswd  = ui->lineEditPwsd->text();
    QString user_pswd_md5 = QCryptographicHash::hash(user_pswd.toLatin1(),QCryptographicHash::Md5).toHex();


    if(user_id.isEmpty() && user_pswd.isEmpty())
    {
        ui->tipsColumn->textlable->setText("请输入用户id和密码");
        ui->tipsColumn->animationStart();
    }
    else if(user_pswd.isEmpty())
    {
        ui->tipsColumn->textlable->setText("请输入密码");
        ui->tipsColumn->animationStart();
    }
    else if(user_id.isEmpty())
    {
        ui->tipsColumn->textlable->setText("请输入用户id");
        ui->tipsColumn->animationStart();
    }
    else
    {
        // 登录成功
        if(SqlManager::getInstance()->loginQuery(user_id,user_pswd_md5))
        {
            qDebug() << "successlogin";
            if (ui->chkRemember->isChecked()) // 选择记住我
            {
                settings->writeIni("login/isremember", "true");
            }
            else // 不保存密码
            {
                settings->writeIni("login/isremember", "false"); // 写ini文件时记录为false
            }
            settings->writeIni("login/user_id", user_id); // 把账号写入ini文件
            settings->writeIni("login/user_pswd", user_pswd); // 把密码写入ini文件
            QString dlgTitle="登录成功";
            QString strInfo="即将进入主界面";
            QMessageBox::information(this, dlgTitle, strInfo,QMessageBox::Ok,QMessageBox::NoButton);
            SqlManager::getInstance()->loginUpdate(user_id);
            if(SqlManager::getInstance()->roleQuery(user_id) == "1")
            {
                MainPage *adminPage = new MainPage();
                adminPage->show();
            }
            else
            {}
            this->close();
        }
        else
        {
            ui->tipsColumn->textlable->setText("用户id或者密码错误");
            ui->tipsColumn->animationStart();
        }
    }
}
