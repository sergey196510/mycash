#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include "global.h"

enum Type {active = 1,
          passive = 2,
          debet = 3,
          credit = 4
          };

class Database
{
//    bool opened;

public:
    Database();
    double  get_account_summ(int);
    double get_operation_summ(int);
    Account_Data get_account_data(int);
    QString get_account_scod(int);
    QString get_agent_name(int);
    QString get_currency_scod(int);
    QMap<int,QString> get_accounts_list();
    QMap<int,QString> get_scod_list();
    QList<PlanOper_data> get_plan_oper_list();
    int     new_operation(operation_data &);
    bool    del_operation(int);
    bool    new_account_oper(const int, const int, const double);
    bool    del_account_oper(int);
    bool    change_account_balance(const int, const double);
    bool    save_operation(operation_data &);
    bool    new_plan_oper(PlanOper_data &data);
    operation_data get_operation(int);
    PlanOper_data get_plan_oper_data(int);
};

#endif // DATABASE_H
