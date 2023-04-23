#ifndef QUIHELPER_H
#define QUIHELPER_H

#include "head.h"

class QuiHelper
{
public:
    QuiHelper();
public:
    //获取当前鼠标所在屏幕索引/区域尺寸/缩放系数
    static int getScreenIndex();
    static QRect getScreenRect(bool available = true);
};

#endif // QUIHELPER_H
