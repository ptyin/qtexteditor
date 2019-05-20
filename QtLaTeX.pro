#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T16:52:48
#
#-------------------------------------------------

QT       += core gui sql
QT       += widgets

TARGET = QtLaTeX
TEMPLATE = app

RC_FILE = myapp.rc


SOURCES += src/main.cpp\
        src/login.cpp \
    src/exam.cpp \
    src/passwdedit.cpp \
    src/register.cpp \
    src/systemtrayicon.cpp \
    src/editor.cpp

HEADERS  += src/login.h \
    src/exam.h \
    src/passwdedit.h \
    src/register.h \
    src/systemtrayicon.h \
    src/editor.h

FORMS    += src/login.ui \
    src/exam.ui \
    src/passwdedit.ui \
    src/register.ui \
    src/editor.ui

RESOURCES += \
    resources/myqrc.qrc \
    resources/lang.qrc \
    resources/qss.qrc

TRANSLATIONS+=cn.ts\
              en.ts
