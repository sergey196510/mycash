#-------------------------------------------------
#
# Project created by QtCreator 2013-04-28T19:09:01
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyCash
TEMPLATE = app


SOURCES += main.cpp\
        mycash.cpp \
    database.cpp \
    opendatabase.cpp

HEADERS  += mycash.h \
    database.h \
    opendatabase.h

FORMS    += mycash.ui \
    opendatabase.ui
