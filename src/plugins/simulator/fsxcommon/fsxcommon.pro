load(common_pre)

QT += core dbus xml network widgets

TARGET = simulatorfsxcommon
TEMPLATE = lib

CONFIG += staticlib
CONFIG += blackmisc blackcore blackgui

DEPENDPATH += . $$SourceRoot/src
INCLUDEPATH += . $$SourceRoot/src

SOURCES += *.cpp
HEADERS += *.h
FORMS += *.ui

DESTDIR = $$DestRoot/lib

load(common_post)
