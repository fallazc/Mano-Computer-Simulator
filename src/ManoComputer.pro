#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T21:11:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ManoComputer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    ManoComputerSimulator.cpp \
    MemoryUnitController.cpp \
    CPU16BitsModel.cpp \
    CPU16BitsController.cpp \
    Utils.cpp \
    CPU16BitsDelegateInstructions.cpp

HEADERS  += \
    ManoComputerSimulator.h \
    CPU16BitsModel.h \
    CPU16BitsController.h \
    MemoryUnitModel.h \
    MemoryUnitController.h \
    Utils.h \
    CPU16BitsDelegateInstructions.h

FORMS    += \
    ManoComputerSimulator.ui
