QT += testlib
QT -= gui
QT += multimedia

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src

SOURCES +=  tst_testjsonfile.cpp \
    ../../src/jsonfile.cpp \
    ../../src/config.cpp
