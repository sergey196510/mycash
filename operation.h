#ifndef OPERATION_H
#define OPERATION_H

#include <vector>
#include <QObject>
#include <QDate>
#include "database.h"
#include "widgets/mycurrency.h"
#include "transaction.h"

using namespace std;

class Operation
{
private:
    QString table;
protected:
    int id;
    int agent;
    double kurs;
    QList<account_summ> from;
    QList<account_summ> to;
//    int plan_id;
    QDate date;
    QString descr;
    int status;
public:
    Operation();
    int new_operation();
    bool del_operation(int);
//    bool save_operation();
    bool new_account_oper(QString table, const int o_id, account_summ &acc, int direction, int agent = 0);
//    bool change_account_balance(account_summ &acc);
    bool insert();
    int find_oper_by_plan(int plan, int mon, int year);
    QMap<int,double> get_account_oper_list(int oper, int type, QString);
//    QMap<int,double> get_plan_account_oper_list(int oper, int type);
    virtual bool read(int id);
    void append_from(account_summ &);
    void append_to( account_summ &);
    void update_descr();

    QDate Date() { return date; }
    QString Descr() { return descr; }
    QList<account_summ> From() { return from; }
    QList<account_summ> To() { return to; }
    int Id() const { return id; }
    int Agent() { return agent; }
    int Status() { return status; }
    double Kurs() { return kurs; }
    void setId(int i) { id = i; }
    void setDate(QDate d) { date = d; }
    void setDescr(QString d) { descr = d; }
    void setFrom(QList<account_summ> l) { from = l; }
    void setTo(QList<account_summ> l) { to = l; }
    void setAgent(int a) { agent = a; }
    void setKurs(double k) { kurs = k; }
};

class PlanOperation : public Operation
{
private:
    QString table;
    int day,month;
    int auto_exec;
    bool ins_account_oper(QList<account_summ>, QString str, int id, int direction);

public:
    PlanOperation() {
        day = month = auto_exec = 0;
    }

    vector<PlanOperation> read_list(int status);
    bool read(int id);
    int new_operation();
    int insert();
    bool update_plan_oper();
    int Day() { return day; }
    int Month() { return month; }
    int Auto() { return auto_exec; }
    void calcStatus();
    void setDay(int d) { day = d; }
    void setMonth(int m) { month = (m == -1) ? 0 : m; }
    void setAuto(int a) { auto_exec = a; }
};

#endif // OPERATION_H
