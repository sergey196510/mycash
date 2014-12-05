#include "summaccount.h"

summAccount::summAccount(int type)
{
    Account acc;

    db = new Database;

    acc.read(type);
    value = acc.get_summ();
}

summAccount::~summAccount()
{
    delete db;
}

QString summAccount::text()
{
//    return QString("%1").arg(value);
    return default_locale->toString(value.toDouble(),'f',2);
}

MyCurrency summAccount::Value()
{
    return value;
}
