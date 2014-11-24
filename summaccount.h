#ifndef SUMMACCOUNT_H
#define SUMMACCOUNT_H

#include "global.h"
#include "database.h"
#include "widgets/mycurrency.h"

class summAccount
{
private:
    MyCurrency value;
    Database *db;

public:
    summAccount() { value = 0; }
    summAccount(int);
    ~summAccount();
    QString text();
    MyCurrency Value();
};

#endif // SUMMACCOUNT_H
