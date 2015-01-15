#-------------------------------------------------
#
# Project created by QtCreator 2013-04-28T19:09:01
#
#-------------------------------------------------

QT       += core gui sql network xml

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    DEFINES += HAVE_QT5
}

TARGET = MyCash
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE += -MP4

SOURCES += \
    widgets/accountcombobox.cpp \
    widgets/agentcombobox.cpp \
    widgets/accounttypecombobox.cpp \
    widgets/accounttype2combobox.cpp \
    widgets/currencycombobox.cpp \
    widgets/currencyedit.cpp \
    widgets/okpushbutton.cpp \
    widgets/cancelpushbutton.cpp \
    widgets/mydateedit.cpp \
    widgets/mycurrency.cpp \
    accountoper2.cpp \
    accountsview.cpp \
    database.cpp \
    debetcreditreport.cpp \
    editaccount.cpp \
    editagent.cpp \
    editoperation.cpp \
    global.cpp \
    graphwidget.cpp \
    listaccounts.cpp \
    listaccountsmodel.cpp \
    listagents.cpp \
    listcurrency.cpp \
    listoperations.cpp \
    listplanoper.cpp \
    listseparateoper.cpp \
    main.cpp \
    mainwidget.cpp \
    mycash.cpp \
    opendatabase.cpp \
    correctbalance.cpp \
    selectaccount.cpp \
    settings.cpp \
    summaccount.cpp \
    mytreeview.cpp \
    listbudget.cpp \
    transaction.cpp \
    editbudget.cpp \
    accountgraph.cpp \
    budget.cpp \
    mytableview.cpp \
    downloader.cpp \
    editpayment.cpp \
    currency.cpp \
    operation.cpp \
    account.cpp \
    regoperation.cpp \
    editplanoperation.cpp \
    payment.cpp \
    accountsumm.cpp \
    updatedescrioption.cpp

HEADERS  += \
    widgets/accountcombobox.h \
    widgets/agentcombobox.h \
    widgets/accounttypecombobox.h \
    widgets/accounttype2combobox.h \
    widgets/currencycombobox.h \
    widgets/currencyedit.h \
    widgets/okpushbutton.h \
    widgets/cancelpushbutton.h \
    widgets/mycurrency.h \
    accountoper2.h \
    accountsview.h \
    database.h \
    debetcreditreport.h \
    editaccount.h \
    editagent.h \
    editoperation.h \
    global.h \
    graphwidget.h \
    listaccounts.h \
    listaccountsmodel.h \
    listagents.h \
    listcurrency.h \
    listoperations.h \
    listplanoper.h \
    listseparateoper.h \
    mainwidget.h \
    mycash.h \
    widgets/mydateedit.h \
    opendatabase.h \
    correctbalance.h \
    selectaccount.h \
    settings.h \
    summaccount.h \
    mytreeview.h \
    listbudget.h \
    transaction.h \
    editbudget.h \
    accountgraph.h \
    budget.h \
    mytableview.h \
    downloader.h \
    editpayment.h \
    currency.h \
    operation.h \
    account.h \
    regoperation.h \
    editplanoperation.h \
    payment.h \
    accountsumm.h \
    updatedescrioption.h

FORMS    += \
    debetcreditreport.ui \
    editaccount.ui \
    editagent.ui \
    editoperation.ui \
    listaccounts.ui \
    listagents.ui \
    listcurrency.ui \
    listoperations.ui \
    listplanoper.ui \
    listseparateoper.ui \
    mainwidget.ui \
    mycash.ui \
    opendatabase.ui \
    correctbalance.ui \
    selectaccount.ui \
    settings.ui \
    listbudget.ui \
    editbudget.ui \
    editpayment.ui \
    regoperation.ui \
    editplanoperation.ui \
    updatedescrioption.ui

TRANSLATIONS += \
    mycash_ru.ts

CODECFORSRC = UTF-8

RESOURCES += \
    icons.qrc
