#ifndef SUMMACCOUNT_H
#define SUMMACCOUNT_H

#include "global.h"
#include "database.h"

class summAccount
{
private:
    double value;
    Database *db;

public:
    summAccount() { value = 0; }
    summAccount(int);
    QString text();
    double Value();
};

#endif // SUMMACCOUNT_H
