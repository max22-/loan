#-------------------------------------------------
#
# Project created by QtCreator 2019-04-21T12:16:42
#
#-------------------------------------------------

QT       += core gui scxml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = loan
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        audiorecorder.cpp \
        config.cpp \
        init.cpp \
        jsonfile.cpp \
        logger.cpp \
        main.cpp \
        mainwindow.cpp \
        mediaplayerwrapper.cpp \
        playlistmodel.cpp \
        recordingsmodel.cpp \
        statehandler.cpp \
        timeslider.cpp

HEADERS += \
        audiorecorder.h \
        config.h \
        init.h \
        jsonfile.h \
        logger.h \
        mainwindow.h \
        mediaplayerwrapper.h \
        playlistmodel.h \
        recordingsmodel.h \
        startupdatetime.h \
        statehandler.h \
        timeslider.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

STATECHARTS += \
    statechart.scxml
