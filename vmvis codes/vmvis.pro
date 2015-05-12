#-------------------------------------------------
#
# Project created by QtCreator 2015-04-04T12:01:03
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += serialport
QT       += multimedia
QT       += multimediawidgets
QT       += network
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vmvis
TEMPLATE = app


SOURCES += main.cpp\
        guruwindow.cpp \
    booter.cpp \
    shishyawindow.cpp \
    sensorgl.cpp \
    consoledisp.cpp \
    sensorsetting.cpp \
    testgl.cpp \
    gurutcp.cpp \
    shishpass.cpp \
    consoleclient.cpp

HEADERS  += guruwindow.h \
    booter.h \
    shishyawindow.h \
    sensorgl.h \
    consoledisp.h \
    sensorsetting.h \
    testgl.h \
    gurutcp.h \
    shishpass.h \
    consoleclient.h

FORMS    += guruwindow.ui \
    booter.ui \
    shishyawindow.ui \
    sensorsetting.ui \
    testgl.ui \
    shishpass.ui

RESOURCES += \
    image.qrc
