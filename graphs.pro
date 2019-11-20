#-------------------------------------------------
#
# Project created by QtCreator 2019-09-06T20:23:02
# Author : tagolder
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphs
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        graphdata.cpp \
        drawgraphwidget.cpp

HEADERS += \
        mainwindow.h \
        graphdata.h \
        drawgraphwidget.h


FORMS += \
        mainwindow.ui \
        drawgraphwidget.ui
