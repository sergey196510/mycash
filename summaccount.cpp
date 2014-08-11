#include "summaccount.h"

summAccount::summAccount(int type)
{
    db = new Database;

    value = db->get_account_summ(type);
}

QString summAccount::text()
{
//    return QString("%1").arg(value);
    return default_locale->toString(value,'f',2);
}

double summAccount::Value()
{
    return value;
}
