#-------------------------------------------------
#
# Project created by QtCreator 2015-06-06T20:54:05
#
#-------------------------------------------------

QT       += core gui
QT       += core sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FoodGen
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        fgmain.cpp \
    dataworker.cpp \
    foodeditor.cpp

HEADERS  += fgmain.h \
    dataworker.h \
    foodeditor.h

FORMS    += fgmain.ui \
    foodeditor.ui
