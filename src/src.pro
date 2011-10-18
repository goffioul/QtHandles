TEMPLATE = lib
TARGET = __init_qt__

QT += opengl
CONFIG += dll

include(../common.pri)
DEFINES += __STDC_LIMIT_MACROS

SOURCES = \
	 __init_qt__.cpp \
	 Backend.cpp \
	 BaseControl.cpp \
	 ButtonControl.cpp \
	 Canvas.cpp \
	 CheckBoxControl.cpp \
	 Container.cpp \
	 ContextMenu.cpp \
	 EditControl.cpp \
	 Figure.cpp \
	 FigureWindow.cpp \
	 GLCanvas.cpp \
	 ListBoxControl.cpp \
	 Menu.cpp \
	 MouseModeActionGroup.cpp \
	 Object.cpp \
	 ObjectFactory.cpp \
	 ObjectProxy.cpp \
	 Panel.cpp \
	 PopupMenuControl.cpp \
	 PushButtonControl.cpp \
	 RadioButtonControl.cpp \
	 SliderControl.cpp \
	 TextControl.cpp \
	 TextEdit.cpp \
	 ToggleButtonControl.cpp \
	 ToolBar.cpp \
	 Utils.cpp

HEADERS = \
	 __init_qt__.h \
	 Backend.h \
	 BaseControl.h \
	 ButtonControl.h \
	 Canvas.h \
	 CheckBoxControl.h \
	 Container.h \
	 ContextMenu.h \
	 EditControl.h \
	 Figure.h \
	 FigureWindow.h \
	 GLCanvas.h \
	 ListBoxControl.h \
	 Menu.h \
	 MenuContainer.h \
	 MouseModeActionGroup.h \
	 Object.h \
	 ObjectFactory.h \
	 ObjectProxy.h \
	 Panel.h \
	 PopupMenuControl.h \
	 PushButtonControl.h \
	 RadioButtonControl.h \
	 SliderControl.h \
	 TextControl.h \
	 TextEdit.h \
	 ToggleButtonControl.h \
	 ToolBar.h \
	 Utils.h

RESOURCES = qthandles.qrc

win32:TOPLEVELTARGET = ..\\$(QMAKE_TARGET).oct
!win32:TOPLEVELTARGET = ../$(QMAKE_TARGET).oct

QMAKE_POST_LINK += $(COPY) $(DESTDIR)$(TARGET) $$TOPLEVELTARGET
QMAKE_DISTCLEAN += $$TOPLEVELTARGET
