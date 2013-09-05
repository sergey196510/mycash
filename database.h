#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>

class Database
{
//    bool opened;

public:
    Database();
//    bool isOpened() { return opened; }
    double get_accout_summ(int);
    bool   save_operation(const int, const int, const double, const QString);
};

#endif // DATABASE_H
