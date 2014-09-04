#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QFont>
#include <QtSql>
#include <widgets/mycurrency.h>

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
    static const int cancelled;
};

class account_summ {
    int acc;
    MyCurrency s;
public:
    account_summ() {
        acc = 0;
        s = 0;
    };
    int account() const { return acc; }
//    double balance() const { return s; }
    MyCurrency balance() const { return s; }
    void set_account(int val) { acc = val; }
    void set_balance(double val) { s = val; }
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
    int agent;
    QString descr;
    QDate dt;
    Account_Data() {
        name.clear();
        type = 0;
        curr = 0;
        hidden = false;
        parent = 0;
        top = 0;
        system = 0;
        agent = 0;
        balance = 0;
        descr.clear();
        dt = QDate::currentDate();
    }
};

struct Operation_Data {
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
    int auto_exec;
    Operation_Data() {
        id = day = month = year = 0;
        agent = 0;
        auto_exec = 0;
        date = QDate::currentDate();
        descr.clear();
    }
};

class agent_data {
    int id;
    QString nm;
    QString ct;
    QString addr;
    QString ph;
    QString cont;
public:
    agent_data() {
        id = 0;
        nm.clear();
        ct.clear();
        addr.clear();
        ph.clear();
        cont.clear();
    }

    int Id() { return id; }
    QString name() { return nm; }
    QString city() { return ct; }
    QString address() { return addr; }
    QString phone() { return ph; }
    QString contact() { return cont; }
    void set_id(int i) { id = i; }
    void set_name(QString s) { nm = s; }
    void set_city(QString s) { ct = s; }
    void set_address(QString s) { addr = s; }
    void set_phone(QString s) { ph = s; }
    void set_contact(QString s) { cont = s; }
};

class Currency {
    int id;
    QString name;
    int icod;
    QString scod;
    double kurs;
public:
    Currency(int i = 0)
    {
        id = i;
        name.clear();
        icod = 0;
        scod.clear();
        kurs = 0;

        if (id != 0) {
            QSqlQuery q;
            q.prepare("SELECT name, icod, scod, kurs FROM currency WHERE id=:id");
            q.bindValue(":id", id);
            if (!q.exec()) {
                qDebug() << q.lastError();
            }
            if (q.next()) {
                name = q.value(0).toString();
                icod = q.value(1).toInt();
                scod = q.value(2).toString();
                kurs = q.value(3).toDouble();
            }
        }
    }
    int Id() { return id; }
    QString Name() { return name; }
    int ICod() { return icod; }
    QString SCod() { return scod; }
    double Kurs() { return kurs; }
};

#endif
