#-------------------------------------------------
#
# Project created by QtCreator 2018-08-18T18:17:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cylinder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

# OpenCv Configuration
INCLUDEPATH += D:\Bulid\opencv34\include
               D:\Bulid\opencv34\include\opencv
               D:\Bulid\opencv34\include\opencv2

LIBS += D:\Bulid\opencv34\bin\libopencv_*.dll
