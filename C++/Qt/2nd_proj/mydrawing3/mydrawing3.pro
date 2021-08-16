#-------------------------------------------------
#
# Project created by QtCreator 2017-05-04T12:40:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mydrawing3
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

LIBS+=-lgif -lpng -ljpeg -lpthread

CONFIG+=gif
