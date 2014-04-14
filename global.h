#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QFont>
#include "widgets/mycurrency.h"

//extern QString version;
//extern int     current_account;
//extern int     current_currency;
extern QLocale *default_locale;
//extern QLocale *english;
//extern QFont   fnt;

enum Direction {
    direction_from = 1,
    direction_to = 2
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

struct account_summ {
    int account;
    double summ;
    account_summ() {
        account = 0;
        summ = 0;
    };
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
    QString dt;
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
        dt = "";
    }
};

struct operation_data {
    int id;
    int day,month,year;
//    int from;
//    int to;
//    int to2;
    int agent;
    double kurs;
    account_summ from;
    account_summ to;
    account_summ to2;
//    MyCurrency summ_from;
//    MyCurrency summ_to;
//    MyCurrency summ_to2;
    int plan_id;
    QString date;
    QString descr;
    operation_data() {
        id = day = month = year = 0;
        agent = plan_id = 0;
//        summ_to2 = 0;
        date = "";
        descr = "";
    }
};

struct agent_data {
    QString name;
    QString city;
    QString address;
    QString phone;
    QString contact;
};

/*
struct PlanOper_data {
    int id;
    int day;
    int month;
    int year;
    int from;
    int to;
    MyCurrency summ;
    QString descr;
    PlanOper_data() {
        id = day = month = year = from = to = 0;
        summ = 0;
        descr = "";
    }
};
*/

#endif
