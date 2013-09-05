#include "QtSql"
#include "database.h"

Database::Database()
{
//    opened = false;
}

double Database::get_accout_summ(int type)
{
    QSqlQuery query;

    query.prepare("SELECT SUM(balance) FROM account WHERE type = :type");
    query.bindValue(":type", type);
    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toDouble();
}
