#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include "global.h"

class Database
{
//    bool opened;

public:
    Database();
    double  get_account_summ(int);
    Account_Data get_account_data(int);
    QString get_account_scod(int);
    QString get_agent_name(int);
    QString get_currency_scod(int);
    QMap<int,QString> get_accounts_list();
    QMap<int,QString> get_scod_list();
    int     new_operation(operation_data &);
    bool    new_account_oper(const int, const int, const double);
    bool    change_account_balance(const int, const double);
    bool    save_operation(operation_data &);
    bool    new_plan_oper(PlanOper_data &data);
};

#endif // DATABASE_H
