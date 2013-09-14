TEMPLATE = app
TARGET = octave-qt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console

include(../common.pri)

SOURCES = octave-qt.cpp

win32:TOPLEVELTARGET = ..\\$(TARGET)
!win32:TOPLEVELTARGET = ../$(TARGET)

QMAKE_POST_LINK += $(COPY) $@ $$TOPLEVELTARGET
QMAKE_DISTCLEAN += $$TOPLEVELTARGET
