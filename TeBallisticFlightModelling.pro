#-------------------------------------------------
#
# Project created by QtCreator 2017-07-16T07:30:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TeBallisticFlightModelling
TEMPLATE = app

DEFINES += DEBUG

SOURCES += main.cpp\
        widget.cpp \
    scene.cpp \
    mythread.cpp \
    ball.cpp \
    trajectory.cpp

HEADERS  += widget.h \
    scene.h \
    mythread.h \
    ball.h \
    trajectory.h

FORMS    += widget.ui

QMAKE_CXXFLAGS += -std=c++14
