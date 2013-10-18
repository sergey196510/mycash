#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>

class Database
{
//    bool opened;

public:
    Database();
//    bool isOpened() { return opened; }
    int     get_account_type(int);
    double  get_account_summ(int);
    double  get_account_balance(int);
    QString get_account_name(int);
    QString get_agent_name(int);
    int     get_account_ccod(int);
    QString get_account_scod(int);
    QString get_currency_scod(int);
    QMap<int,QString> get_accounts_list();
    QMap<int,QString> get_scod_list();
    int     new_operation(const int, const int, const int, const double, const QString, const QString);
    bool    new_account_oper(const int, const int, const double);
    bool    change_account_balance(const int, const double);
    bool    save_operation(const int, const int, const int, const double, const QString, const QString);
};

#endif // DATABASE_H
