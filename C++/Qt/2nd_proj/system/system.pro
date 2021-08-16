#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T12:56:35
#
#-------------------------------------------------

QT+=sql widgets

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = system
TEMPLATE = app


SOURCES += main.cpp\
        guahao.cpp \
    total.cpp \
    welcome.cpp \
    login_adm.cpp \
    adm.cpp \
    adddoc_wid.cpp \
    addadm_wid.cpp \
    doc_information.cpp \
    xiugaidoc_wid.cpp \
    login_doc.cpp \
    zhenduan.cpp \
    pat_information.cpp \
    inforpat_wid.cpp \
    symptom_dia.cpp

HEADERS  += guahao.h \
    total.h \
    doctor.h \
    patient.h \
    welcome.h \
    login_adm.h \
    adm.h \
    adddoc_wid.h \
    addadm_wid.h \
    doc_information.h \
    xiugaidoc_wid.h \
    login_doc.h \
    zhenduan.h \
    pat_information.h \
    inforpat_wid.h \
    symptom_dia.h

FORMS    += guahao.ui \
    welcome.ui \
    login_adm.ui \
    adm.ui \
    adddoc_wid.ui \
    addadm_wid.ui \
    doc_information.ui \
    xiugaidoc_wid.ui \
    login_doc.ui \
    zhenduan.ui \
    pat_information.ui \
    inforpat_wid.ui \
    symptom_dia.ui
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
