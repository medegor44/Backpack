#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T19:37:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = backpack
TEMPLATE = app
CONFIG += C++11


SOURCES += main.cpp\
        widget.cpp \
    scene.cpp \
    listwidget.cpp \
    createnewdialog.cpp \
    algorythms/next_fit_algo.cpp \
    algorythms/algo_base.cpp \
    algorythms/best_fit_algo.cpp \
    algorythms/guillotine_algo.cpp \
    algorythms/max_rects_algo.cpp

HEADERS  += widget.h \
    scene.h \
    listwidget.h \
    createnewdialog.h \
    algorythms/next_fit_algo.h \
    algorythms/algo_base.h \
    algorythms/best_fit_algo.h \
    algorythms/guillotine_algo.h \
    algorythms/max_rects_algo.h

RESOURCES +=
