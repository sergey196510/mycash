#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>
#include <QDate>
#include <QtSql>
#include "widgets/mycurrency.h"
#include "transaction.h"
#include "global.h"

class Account {
    int id;
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
    struct P {
        int debet;
        int credit;
    };

    QMap<int,P> array;

public:
    Account();
    bool read(int i);
    int insert();
    bool change_balance2(MyCurrency val);
    bool debet(MyCurrency val);
    bool credit(MyCurrency val);
    QString get_parent_account(int id);
    QString fullName();
    double get_summ();

    void setId(int i) { id = i; }
    void setBalance(MyCurrency bal) { balance = bal; }
    void setAgent(int a) { agent = a; }
    void setCurr(int c) { curr = c; }
    void setName(QString n) { name = n; }
    void setDescr(QString n) { descr = n; }
    void setType(int t) { type = t; }
    void setParent(int p) { parent = p; }
    void setHidden(bool h) { hidden = h; }
    void setDate(QDate d) { dt = d; }
    void setTop(int t) { top = t; }
    int Id() const { return id; }
    int Agent() { return agent; }
    int Top() { return top; }
    MyCurrency Balance() { return balance; }
    int Type() { return type; }
    QString Name() { return name; }
    QString Descr() { return descr; }
    int Curr() { return curr; }
    int Parent() { return parent; }
    bool Hidden() { return hidden; }
    QDate Date() { return dt; }
    int System() { return system; }
};

#endif // ACCOUNT_H
