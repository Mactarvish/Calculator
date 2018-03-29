#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T23:44:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Calculator
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    calculator.cpp

HEADERS  += dialog.h \
    calculator.h

FORMS    += dialog.ui

RESOURCES += \
    calculatorresource.qrc
