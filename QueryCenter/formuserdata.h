#ifndef FORMUSERDATA_H
#define FORMUSERDATA_H

#include <QWidget>

namespace Ui {
class FormUserData;
}

class FormUserData : public QWidget
{
    Q_OBJECT

public:
    explicit FormUserData(QWidget *parent = nullptr);
    ~FormUserData();

private:
    Ui::FormUserData *ui;
};

#endif // FORMUSERDATA_H
