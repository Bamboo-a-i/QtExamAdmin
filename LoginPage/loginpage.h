#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include "settings.h"
#include <QWidget>

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private:
    Ui::LoginPage *ui;
    Settings *settings;


private slots:
    void initPage();
    void on_btnLogin_clicked();
};

#endif // LOGINPAGE_H
