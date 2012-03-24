QT       += gui
QT       += xml

CONFIG         += plugin
CONFIG         += debug_and_release
TARGET          = $$qtLibraryTarget(wind)

TEMPLATE = lib

OBJECTS_DIR   = objs
MOC_DIR       = moc
DESTDIR       = ../build/plugins_real

INCLUDEPATH += ../common

SOURCES       += wind.cpp

HEADERS       += wind.h

SOURCES       += ../common/arduino-serial.cpp
HEADERS       += ../common/arduino-serial.h
