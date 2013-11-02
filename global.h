#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QFont>

extern QString version;
//extern int     current_account;
//extern int     current_currency;
extern QLocale *default_locale;
//extern QLocale *english;
extern QFont   fnt;

class Globals {
    static int account;
    static int currency;
    static int correct_account;
    static double kurs;
    static QString symbol;
public:
    int Account() { return account; }
    void setAccount(int i) { account = i; }
    int Currency() { return currency; }
    void setCurrency(int i) { currency = i; }
    int CorrectAccount() { return correct_account; }
    void setCorrectAccount(int i) { correct_account = i; }
    double Kurs() { return kurs; }
    void setKurs(double i) { kurs = i; }
    QString Symbol() { return symbol; }
    void setSymbol(QString s) { symbol = s; }
};

struct Account_Data {
    QString name;
    int type;
    int curr;
    double balance;
    bool hidden;
    int parent;
    QString descr;
    Account_Data() {
        name = "";
        type = curr = hidden = parent = 0;
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
    int id;
    int day;
    int month;
    int year;
    int from;
    int to;
    double summ;
    QString descr;
    PlanOper_data() {
        id = day = month = year = from = to = 0;
        summ = 0;
        descr = "";
    }
};

#endif
