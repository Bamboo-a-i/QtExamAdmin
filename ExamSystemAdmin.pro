QT       += core gui
QT       += sql
RC_ICONS = admin.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp

HEADERS += \
    head.h

FORMS +=

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/LoginPage
include ($$PWD/LoginPage/loginpage.pri)


INCLUDEPATH += $$PWD/lib
include ($$PWD/lib/lib.pri)

INCLUDEPATH += $$PWD/MainPage
include ($$PWD/MainPage/mainpage.pri)

INCLUDEPATH += $$PWD/QueryCenter
include ($$PWD/QueryCenter/querycenter.pri)

INCLUDEPATH += $$PWD/DisposeCenter
include ($$PWD/DisposeCenter/disposecenter.pri)

INCLUDEPATH += $$PWD/UserCenter
include ($$PWD/UserCenter/usercenter.pri)

INCLUDEPATH += $$PWD/ExamCenter
include ($$PWD/ExamCenter/examcenter.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    qss.qrc
