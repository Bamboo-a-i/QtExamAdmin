#include "loginpage.h"
#include "sqlmanager.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 启动登录界面
    LoginPage l;
    l.show();

    return a.exec();
}
