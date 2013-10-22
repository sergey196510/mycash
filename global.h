#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QFont>

extern QString version;
extern int     current_account;
extern int     current_currency;
extern QLocale *default_locale;
extern QLocale *english;
extern QFont   fnt;

struct Account_Data {
    QString name;
    int type;
    int curr;
    double balance;
    int hidden;
    QString descr;
};

struct operation_data {
    int from;
    int to;
    int agent;
    double summ;
    QString date;
    QString descr;
};

struct PlanOper_data {
    int day;
    int month;
    int year;
    int from;
    int to;
    double summ;
    QString descr;
};

#endif
