#ifndef OPERATION_H
#define OPERATION_H

#include <QObject>
#include <QDate>
#include "database.h"
#include "widgets/mycurrency.h"
#include "transaction.h"

class Operation {
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
public:
    Operation();
    int new_operation();
    bool del_operation(int);
    bool save_operation();
    bool new_account_oper(QString table, const int o_id, account_summ &acc, int direction, int agent = 0);
//    bool change_account_balance(account_summ &acc);
    bool insert();
    int find_oper_by_plan(int plan, int mon, int year);
    QMap<int,double> get_plan_account_oper_list(int oper, int type);
    bool get_plan_data(int id, QDate oper_date);
    QList<Operation> get_plan_oper_list(int status);
    int new_plan_oper();
    bool read(int id);
    bool update_plan_oper();

    int Day() { return day; }
    int Month() { return month; }
    int Year() { return year; }
    QDate Date() { return date; }
    QString Descr() { return descr; }
    QList<account_summ> From() { return from; }
    QList<account_summ> To() { return to; }
    int Id() const { return id; }
    int Auto() { return auto_exec; }
    int Agent() { return agent; }
    int Status() { return status; }
    double Kurs() { return kurs; }
    void setId(int i) { id = i; }
    void setDate(QDate d) { date = d; }
    void setDescr(QString d) { descr = d; }
    void setFrom(QList<account_summ> l) { from = l; }
    void setTo(QList<account_summ> l) { to = l; }
    void setDay(int d) { day = d; }
    void setMonth(int m) { month = m; }
    void setYear(int y) { year = y; }
    void setAgent(int a) { agent = a; }
    void setKurs(double k) { kurs = k; }
    void setAuto(int a) { auto_exec = a; }
};

#endif // OPERATION_H
