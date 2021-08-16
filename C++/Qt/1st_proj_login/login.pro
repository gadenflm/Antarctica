#-------------------------------------------------
#
# Project created by QtCreator 2017-03-23T16:34:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = login
TEMPLATE = app


SOURCES += main.cpp\
        loginwidget.cpp \
    zhucedialog.cpp \
    total.cpp \
    stuwidget.cpp \
    teawidget.cpp \
    admwidget.cpp \
    adm_zhuce_dialog.cpp \
    class_add.cpp \
    class_choose.cpp \
    class_show.cpp

HEADERS  += loginwidget.h \
    zhucedialog.h \
    student.h \
    total.h \
    stuwidget.h \
    teacher.h \
    teawidget.h \
    admwidget.h \
    adm_zhuce_dialog.h \
    class_add.h \
    class_choose.h \
    class_show.h

FORMS    += loginwidget.ui \
    zhucedialog.ui \
    stuwidget.ui \
    teawidget.ui \
    admwidget.ui \
    adm_zhuce_dialog.ui \
    class_add.ui \
    class_choose.ui \
    class_show.ui
