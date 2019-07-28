#-------------------------------------------------
#
# Project created by QtCreator 2019-06-11T20:04:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataCollector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    additem.cpp \
    fixdialog.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    additem.h \
    fixdialog.h

FORMS    += mainwindow.ui \
    dialog.ui \
    additem.ui \
    fixdialog.ui

RESOURCES += \
    src.qrc
