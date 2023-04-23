#ifndef SETTINGS_H
#define SETTINGS_H
#include <QCoreApplication>
// 定义配置文件位置
#define CONFIGPATH (QCoreApplication::applicationDirPath() + "/config/config.ini")

class Settings
{
public:
    Settings();
    bool isExist();
    QStringList userInfo();
    void writeIni(QString info,QString content);

};

#endif // SETTINGS_H
