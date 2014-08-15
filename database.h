#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include "global.h"

class Database
{
//    bool opened;

private:
    Globals var;

public:
    Database();
    double convert_currency(double, int);
    double get_account_summ(int);
    double get_operation_summ(int);
    Account_Data get_account_data(int);
    QString get_account_scod(int);
    QString get_agent_name(int);
//    QString get_currency_scod(int);
    QMap<int,QString> get_accounts_list();
    QList<Budget_Data> read_budget_list(int mon);
    QString get_parent_account(int id);
    QMap<int,QString> get_scod_list();
    QMap<QString,double> get_currency_list();
    QMap<QString,double> get_opersummbyaccount_list(int type,int month,int year);
    QMap<int,double> get_account_oper_list(int, int);
    QMap<int,double> get_plan_account_oper_list(int, int);
    QList<Operation_Data> get_plan_oper_list(int status);
    int     new_account(Account_Data &);
    int     new_agent(agent_data &);
    int     new_operation(Operation_Data &);
    bool    del_operation(int);
    bool    new_account_oper(const QString, const int oper, account_summ &, const int direction, const int agent = 0);
    bool    del_account_oper(int);
    bool    change_account_balance(account_summ &);
    bool    save_operation(Operation_Data &);
    int     new_plan_oper(Operation_Data &data);
    bool    update_plan_oper(Operation_Data &data);
    bool    new_mon_oper(int p_id, int status);
    bool    add_budget(account_summ &d);
    bool    find_budget_id(int, int);
    Operation_Data get_operation(int);
    Operation_Data get_plan_oper_data(int,QDate dt);
    int find_oper_by_plan(int plan, int mon, int year);
    QMap<int,QString> scod_list;
    QMap<QString,double> currency_list;
};

#endif // DATABASE_H
