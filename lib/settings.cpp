#include "settings.h"
#include <QSettings>
#include <QTextCodec>
#include <QFile>

Settings::Settings()
{

}

bool Settings::isExist()
{
    if(QFile::exists(CONFIGPATH))
        return true;
    else
        return false;
}

QStringList Settings::userInfo()
{
    QSettings setting(CONFIGPATH, QSettings::IniFormat); // 打开存放登录信息的ini文件
    setting.setIniCodec(QTextCodec::codecForName("UTF-8")); // 设置编码格式，有中文，读和写的时候都要保持一致
    QString isremember = setting.value("login/isremember").toString(); // 是否选择了记住密码
    QString user_id = QString::fromUtf8(setting.value("login/user_id").toByteArray()); // 账号
    QString user_pswd = setting.value("login/user_pswd").toString(); // 密码
    QStringList strList;
    strList << user_id << user_pswd << isremember;
    return strList;
}

void Settings::writeIni(QString info,QString content)
{
    QSettings writeini(CONFIGPATH, QSettings::IniFormat); // 打开ini文件
    writeini.setIniCodec(QTextCodec::codecForName("UTF-8")); // 设置编码格式
    writeini.setValue(info, content);
}
