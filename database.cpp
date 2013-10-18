#include "database.h"

Database::Database()
{
//    opened = false;
}

double Database::get_account_summ(int type)
{
    QSqlQuery query;

    query.prepare("SELECT SUM(balance) FROM account WHERE type = :type AND hidden = 'false'");
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

int Database::get_account_ccod(int id)
{
    QSqlQuery query;

    query.prepare("SELECT ccod FROM account WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toInt();
}

QString Database::get_account_scod(int id)
{
    QSqlQuery query;

    query.prepare("SELECT c.scod FROM account a, currency c WHERE a.id = :id AND a.ccod = c.id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toString();
}

int Database::get_account_type(int id)
{
    QSqlQuery query;

    query.prepare("SELECT type FROM account WHERE id = :id");
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

QMap<int,QString> Database::get_accounts_list()
{
    QMap<int,QString> list;
    QSqlQuery q;

    q.prepare("SELECT id,name FROM account");
    if (!q.exec())
        return list;
    while (q.next()) {
        list[q.value(0).toInt()] = q.value(1).toString();
    }

    return list;
}

QString Database::get_agent_name(int id)
{
    QSqlQuery query;

    query.prepare("SELECT name FROM agent WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toString();
}

QString Database::get_currency_scod(int id)
{
    QSqlQuery query;

    query.prepare("SELECT scod FROM currency WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toString();
}

QMap<int,QString> Database::get_scod_list()
{
    QMap<int,QString> list;
    QSqlQuery q;

    q.prepare("SELECT id,scod FROM currency");
    if (!q.exec())
        return list;
    while (q.next()) {
        list[q.value(0).toInt()] = q.value(1).toString();
    }

    return list;
}

int Database::new_operation(const int from, const int to, const int agent, const double summ, const QString date, const QString descr)
{
    QSqlQuery query;

    query.prepare("INSERT INTO operation(acc_from, acc_to, agent, summ, dt, descr) VALUES(:from, :to, :agent, :summ, :dt, :descr)");
    query.bindValue(":from", from);
    query.bindValue(":to", to);
    query.bindValue(":agent", agent);
    query.bindValue(":summ", summ);
    query.bindValue(":dt", date);
    query.bindValue(":descr", descr);
    if (!query.exec())
        return 0;

    query.prepare("SELECT MAX(id) FROM operation");
    if (!query.exec() || !query.next())
	return -1;
    return query.value(0).toInt();
}

bool Database::new_account_oper(const int a_id, const int o_id, const double delta)
{
    QSqlQuery q;
    int type = get_account_type(a_id);
    int flag = (type == 1 || type == 4) ? 1 : -1;
    double summ = delta * flag;

    q.prepare("INSERT INTO account_oper(a_id, o_id, summ) VALUES(:a_id, :o_id, :summ)");
    q.bindValue(":a_id", a_id);
    q.bindValue(":o_id", o_id);
    q.bindValue(":summ", summ);
    if (!q.exec())
        return false;

    return true;
}

bool Database::change_account_balance(const int id, const double delta)
{
    QSqlQuery query;
    double summ;
    int type, flag;

    query.exec("BEGIN TRANSACTION");

    type = get_account_type(id);

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

bool Database::save_operation(const int from, const int to, const int agent, const double summ, const QString date, const QString descr)
{
    QSqlQuery q;
    int oper_id;

    q.exec("BEGIN TRANSACTION");

    if ((oper_id = new_operation(from, to, agent, summ, date, descr)) == 0) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (new_account_oper(from, oper_id, -summ) == false) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (new_account_oper(to, oper_id, summ) == false) {
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
