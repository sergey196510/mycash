#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QFont>
#include "widgets/mycurrency.h"

extern QLocale *default_locale;

class Direction {
public:
    static const int from;
    static const int to;
};

class Account_Type {
public:
    static const int active;
    static const int passive;
    static const int debet;
    static const int credit;
    static const int initial;
};

struct Font {
    QString name;
    int size;
    bool bold;
};

class Globals {
    static int account;
    static int currency;
    static int correct_account;
    static int initial_account;
    static double kurs;
    static int precision;
    static QString symbol;
    static QString list_font;
    static QString version;
    static bool database_opened;
    static Font font;
public:
    int Account() { return account; }
    void setAccount(int i) { account = i; }
    int Currency() { return currency; }
    void setCurrency(int i) { currency = i; }
    int CorrectAccount() { return correct_account; }
    void setCorrectAccount(int i) { correct_account = i; }
    int InitialAccount() { return initial_account; }
    void setInitialAccount(int i) { initial_account = i; }
    double Kurs() { return kurs; }
    void setKurs(double i) { kurs = i; }
    int Precision() { return precision; }
    void setPrecision(int i) { precision = i; }
    QString Symbol() { return symbol; }
    void setSymbol(QString s) { symbol = s; }
    QString ListFont() { return list_font; }
    void setListFont(QString s) { list_font = s; }
    QString Version() { return version; }
    void setVersion(QString v) { version = v; }
    bool database_Opened() { return database_opened; }
    void database_Open() { database_opened = true; }
    void database_Close() { database_opened = false; }
};

class Plan_Status {
public:
    static const int actual;
    static const int committed;
    static const int minimum;
    static const int expired;
};

class account_summ {
    int acc;
    double s;
public:
    account_summ() {
        acc = 0;
        s = 0;
    };
    int account() const { return acc; }
    double summ() const { return s; }
    void set_account(int val) { acc = val; }
    void set_summ(double val) { s = val; }
};

struct Account_Data {
    QString name;
    int type;
    int curr;
    MyCurrency balance;
    bool hidden;
    int parent;
    int top;
    int system;
    QString descr;
    QDate dt;
    Account_Data() {
        name = "";
        type = 0;
        curr = 0;
        hidden = false;
        parent = 0;
        top = 0;
        system = 0;
//        balance = 0;
        descr = "";
        dt = QDate::currentDate();
    }
};

struct operation_data {
    int id;
    int day,month,year;
    int agent;
    double kurs;
    QList<account_summ> from;
    QList<account_summ> to;
//    int plan_id;
    QDate date;
    QString descr;
    int status;
    operation_data() {
        id = day = month = year = 0;
        agent = 0;
        date = QDate::currentDate();
        descr.clear();
    }
};

class agent_data {
    int ii;
    QString nm;
    QString ct;
    QString addr;
    QString ph;
    QString cont;
public:
    int id() { return ii; }
    QString name() { return nm; }
    QString city() { return ct; }
    QString address() { return addr; }
    QString phone() { return ph; }
    QString contact() { return cont; }
    void set_id(int i) { ii = i; }
    void set_name(QString s) { nm = s; }
    void set_city(QString s) { ct = s; }
    void set_address(QString s) { addr = s; }
    void set_phone(QString s) { ph = s; }
    void set_contact(QString s) { cont = s; }
};

struct Currency_Data {
    int id;
    QString name;
    int icod;
    QString scod;
    double kurs;
};

#endif
