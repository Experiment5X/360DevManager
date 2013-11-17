#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T13:46:32
#
#-------------------------------------------------

QT       += core gui
CONFIG   += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 360DevManager
TEMPLATE = app

INCLUDEPATH += $$PWD/../XBDM


SOURCES += main.cpp \
    MainWindow.cpp \
    PropertiesDockWidget.cpp \
    ModulesDockWidget.cpp \
    FileExplorerDockWidget.cpp

HEADERS  += \
    MainWindow.h \
    PropertiesDockWidget.h \
    QtExtensions.h \
    ModulesDockWidget.h \
    FileExplorerDockWidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

win32:LIBS += -L$$PWD/../XBDM-Win/debug/ -lXBDM
