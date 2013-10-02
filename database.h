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
    bool    new_operation(const int, const int, const int, const double, const QString, const QString);
    bool    change_account_balance(const int, const double);
    bool    save_operation(const int, const int, const int, const double, const QString, const QString);
};

#endif // DATABASE_H
