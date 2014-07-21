#include "transaction.h"

int Transaction::level = 0;

Transaction::Transaction()
{
    level++;
}

Transaction::~Transaction()
{
    if (level > 0)
        level--;
}

void Transaction::begin()
{
    QSqlQuery q;

    if (level == 1)
        q.exec("BEGIN");
}

void Transaction::commit()
{
    QSqlQuery q;

    if (level == 1)
        q.exec("COMMIT");
}

void Transaction::rollback()
{
    QSqlQuery q;

    if (level == 1)
        q.exec("ROLLBACK");
}
