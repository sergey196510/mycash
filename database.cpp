#include "database.h"

Database::Database()
{
//    opened = false;
}

double Database::get_account_summ(int type)
{
    QSqlQuery query;

    query.prepare("SELECT SUM(balance) FROM account WHERE type = :type");
    query.bindValue(":type", type);
    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toDouble();
}

double Database::get_account_balance(int id)
{
    QSqlQuery query;

    query.prepare("SELECT balance FROM account WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toDouble();
}

bool Database::new_operation(const int from, const int to, const double summ, const QString date)
{
    QSqlQuery query;

    query.prepare("INSERT INTO operation(acc_from, acc_to, summ, dt) VALUES(:from, :to, :summ, :dt)");
    query.bindValue(":from", from);
    query.bindValue(":to", to);
    query.bindValue(":summ", summ);
    query.bindValue(":dt", date);
    if (!query.exec())
        return false;
    else
        return true;
}

bool Database::change_account_balance(const int id, const double summ)
{

}

bool Database::save_operation(const int from, const int to, const double summ, const QString date)
{
    if (new_operation(from, to, summ, date) == false)
        return false;
    if (change_account_balance(from, -summ) == false)
        return false;
    if (change_account_balance(to, summ) == false)
        return false;

    return true;
}
