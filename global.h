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
    Account_Data() {
        name = "";
        type = curr = hidden = 0;
        balance = 0;
        descr = "";
    }
};

struct operation_data {
    int from;
    int to;
    int agent;
    double summ;
    int plan_id;
    QString date;
    QString descr;
    operation_data() {
        from = to = agent = plan_id = 0;
        summ = 0;
        date = "";
        descr = "";
    }
};

struct PlanOper_data {
    int day;
    int month;
    int year;
    int from;
    int to;
    double summ;
    QString descr;
    PlanOper_data() {
        day = month = year = from = to = 0;
        summ = 0;
        descr = "";
    }
};

#endif
