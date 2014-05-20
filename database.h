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
    QMap<int,QString> get_scod_list();
    QMap<QString,double> get_currency_list();
    QMap<QString,double> get_opersummbyaccount_list(int type,int month,int year);
    QMap<int,double> get_account_oper_list(int, int);
    QMap<int,double> get_plan_account_oper_list(int, int);
    QList<operation_data> get_plan_oper_list();
    int     new_account(Account_Data &);
    int     new_agent(agent_data &);
    int     new_operation(operation_data &);
    bool    del_operation(int);
    bool    new_account_oper(const QString, const int oper, account_summ &, const int direction);
    bool    del_account_oper(int);
    bool    change_account_balance(account_summ &);
    bool    save_operation(operation_data &);
    int     new_plan_oper(operation_data &data);
    bool    new_mon_oper(int p_id);
    operation_data get_operation(int);
    operation_data get_plan_oper_data(int);
    bool find_oper_by_plan(int plan, int mon, int year);
    QMap<int,QString> scod_list;
    QMap<QString,double> currency_list;
};

#endif // DATABASE_H
