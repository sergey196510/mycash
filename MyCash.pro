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
    opendatabase.cpp \
    listaccounts.cpp \
    editaccount.cpp \
    accounttypecombobox.cpp \
    listoperations.cpp \
    mydateedit.cpp \
    editoperation.cpp

HEADERS  += mycash.h \
    database.h \
    opendatabase.h \
    listaccounts.h \
    editaccount.h \
    accounttypecombobox.h \
    listoperations.h \
    mydateedit.h \
    editoperation.h

FORMS    += mycash.ui \
    opendatabase.ui \
    listaccounts.ui \
    editaccount.ui \
    listoperations.ui \
    editoperation.ui

RESOURCES += \
    icons.qrc
