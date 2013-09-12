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

QString Database::get_account_name(int id)
{
    QSqlQuery query;

    query.prepare("SELECT name FROM account WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toString();
}

bool Database::new_operation(const int from, const int to, const double summ, const QString date, const QString descr)
{
    QSqlQuery query;

    query.prepare("INSERT INTO operation(acc_from, acc_to, summ, dt, descr) VALUES(:from, :to, :summ, :dt, :descr)");
    query.bindValue(":from", from);
    query.bindValue(":to", to);
    query.bindValue(":summ", summ);
    query.bindValue(":dt", date);
    query.bindValue(":descr", descr);
    if (!query.exec())
        return false;
    else
        return true;
}

bool Database::change_account_balance(const int id, const double delta)
{
    QSqlQuery query;
    double summ;
    int type, flag;

    query.exec("BEGIN TRANSACTION");

    query.prepare("SELECT type FROM account WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next()) {
	query.exec("ROLLBACK TRANSACTION");
        return false;
    }
    type = query.value(0).toInt();

    if (type == 1 || type == 4)
        flag = 1;
    else if (type < 1 || type > 4) {
	qDebug() << "type is unknown: " << type;
	query.exec("ROLLBACK TRANSACTION");
        return false;
    }
    else
        flag = -1;

    summ = delta * flag;

    query.prepare("UPDATE account set balance = balance + :summ WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":summ", summ);
    if (!query.exec()) {
	query.exec("ROLLBACK TRANSACTION");
	return false;
    }

    query.exec("COMMIT TRANSACTION");
    return true;
}

bool Database::save_operation(const int from, const int to, const double summ, const QString date, const QString descr)
{
    QSqlQuery q;

    q.exec("BEGIN TRANSACTION");

    if (new_operation(from, to, summ, date, descr) == false) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (change_account_balance(from, -summ) == false) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (change_account_balance(to, summ) == false) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }

    q.exec("COMMIT TRANSACTION");
    return true;
}
