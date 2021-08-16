QT += core
QT += gui
QT += widgets
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = flash
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
QT       += core gui
