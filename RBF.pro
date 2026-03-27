#-------------------------------------------------
#
# Project created by QtCreator 2017-11-15T00:30:54
# Updated for Qt6 compatibility
#
#-------------------------------------------------

QT       += core gui widgets charts

CONFIG += c++17

TARGET = RBF
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 5) {
    DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
    QMAKE_CXXFLAGS += -include arm_acle.h
}


SOURCES += \
        src/app/main.cpp \
        src/ui/mainwindow.cpp \
        src/neurons/radialneuron.cpp \
        src/neurons/linearneuron.cpp

HEADERS += \
        src/ui/mainwindow.h \
        src/neurons/radialneuron.h \
        src/neurons/linearneuron.h

FORMS += \
        src/ui/mainwindow.ui

INCLUDEPATH += \
        src/ui \
        src/neurons
