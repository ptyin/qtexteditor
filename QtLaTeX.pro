#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T16:52:48
#
#-------------------------------------------------
Release:DEFINES += QT_NO_WARNING_OUTPUT\
QT_NO_DEBUG_OUTPUT

QT       += core gui sql network printsupport
QT       += widgets

TARGET = QtTextEditor
TEMPLATE = app

RC_FILE = myapp.rc


SOURCES += src/main.cpp\
        src/login.cpp \
    src/register.cpp \
    src/systemtrayicon.cpp \
    src/editor.cpp \
    src/mtextedit.cpp \
    src/AccountManager.cpp

HEADERS  += src/login.h \
    src/register.h \
    src/systemtrayicon.h \
    src/editor.h \
    src/mtextedit.h \
    src/AccountManager.h

FORMS    += src/login.ui \
    src/register.ui \
    src/editor.ui

RESOURCES += \
    resources/myqrc.qrc \
    resources/lang.qrc \
    resources/qss.qrc

TRANSLATIONS+=cn.ts\
              en.ts
