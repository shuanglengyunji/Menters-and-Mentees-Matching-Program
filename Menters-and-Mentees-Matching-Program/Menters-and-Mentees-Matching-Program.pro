#-------------------------------------------------
#
# Project created by QtCreator 2019-06-17T11:08:14
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Menters-and-Mentees-Matching-Program
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

Release:DEFINES += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        mainwindow_algorithm_group.cpp \
        mainwindow_algorithm_match.cpp \
        mainwindow_group.cpp \
        mainwindow_import_export.cpp \
        mainwindow_manage.cpp \
        mainwindow_match.cpp \
        mainwindow_mentees.cpp \
        mainwindow_mentors.cpp \
        my_qsqltablemodel_grouping.cpp \
        mymenteestablemodel.cpp \
        mymentorstablemodel.cpp

HEADERS += \
        mainwindow.h \
        my_qsqltablemodel_grouping.h \
        mymenteestablemodel.h \
        mymentorstablemodel.h

FORMS += \
        mainwindow.ui

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=../QXlsx/QXlsx         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=../QXlsx/QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=../QXlsx/QXlsx/source/  # current QXlsx source path is ./source/
include(../QXlsx/QXlsx/QXlsx.pri)

# CSVWriter
CSVWriter_HEADERPATH = ../CSVWriter/include/
INCLUDEPATH += CSVWriter_HEADERPATH
HEADERS += $${CSVWriter_HEADERPATH}CSVWriter.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# App Info

VERSION = 4.2

RESOURCES += \
    icon.qrc

RC_ICONS = icon.ico
