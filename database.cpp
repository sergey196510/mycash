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
    if (!query.exec())
        return 0;
    if (query.next())
	return query.value(0).toDouble();
    return 0;
}

double Database::get_operation_summ(int type)
{
    QSqlQuery q;
    QDate current = QDate::currentDate();
    int month = current.month();
    int year = current.year();
    double summ = 0;

    QString query = QString("SELECT id,dt FROM operation WHERE dt >= '%1-%2-01'").arg(year).arg(month);
//    qDebug() << query;
    if (!q.exec(query)) {
        qDebug() << q.lastError().text();
        return 0;
    }
    while (q.next()) {
//        qDebug() << q.value(1).toString();
        QSqlQuery q2;
        q2.prepare("SELECT ao.a_id,ao.summ FROM account a, account_oper ao WHERE ao.o_id = :id AND a.id = ao.a_id AND a.type = :type");
        q2.bindValue(":id", q.value(0).toInt());
        q2.bindValue(":type", type);
        if (!q2.exec()) {
            qDebug() << q2.lastError().text();
        }
        while (q2.next()) {
//            qDebug() << q2.value(0).toInt() << q2.value(1).toDouble();
            summ += q2.value(1).toDouble();
        }
    }

    return summ;
}

/*
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
*/

QString Database::get_account_scod(int id)
{
    QSqlQuery query;

    query.prepare("SELECT c.scod FROM account a, currency c WHERE a.id = :id AND a.ccod = c.id");
    query.bindValue(":id", id);
    if (!query.exec())
        return 0;
    if (query.next())
        return query.value(0).toString();
    return 0;
}

/*
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
*/

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

Account_Data Database::get_account_data(int id)
{
    Account_Data data;
    QSqlQuery q;

    q.prepare("SELECT name,type,balance,descr,ccod,hidden FROM account WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "SELECT Error:" << q.lastError().text();
        return data;
    }
    if (q.next()) {
        data.name = q.value(0).toString();
	data.type = q.value(1).toInt();
        data.balance = q.value(2).toDouble();
	data.descr = q.value(3).toString();
        data.curr = q.value(4).toInt();
	data.hidden = q.value(5).toInt();
    }
    return data;
}

QString Database::get_agent_name(int id)
{
    QSqlQuery q;

    q.prepare("SELECT name FROM agent WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec())
        return 0;
    if (q.next())
	return q.value(0).toString();
    return 0;
}

QString Database::get_currency_scod(int id)
{
    QSqlQuery q;

    q.prepare("SELECT scod FROM currency WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec())
        return 0;
    if (q.next())
        return q.value(0).toString();
    return 0;
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

QMap<QString,double> Database::get_currency_list()
{
    QMap<QString,double> list;
    QSqlQuery q;

    q.prepare("SELECT scod,kurs FROM currency");
    if (!q.exec())
        return list;
    while (q.next()) {
        list[q.value(0).toString()] = q.value(1).toDouble();
    }

    return list;
}

int Database::new_operation(operation_data &data)
{
    QSqlQuery query;

    query.prepare("INSERT INTO operation(acc_from, acc_to, agent, summ, dt, descr, plan_id) VALUES(:from, :to, :agent, :summ, :dt, :descr, :plan_id)");
    query.bindValue(":from", data.from);
    query.bindValue(":to", data.to);
    query.bindValue(":agent", data.agent);
    query.bindValue(":summ", data.summ);
    query.bindValue(":dt", data.date);
    query.bindValue(":descr", data.descr);
    query.bindValue(":plan_id", data.plan_id);
    if (!query.exec())
        return 0;

    query.prepare("SELECT MAX(id) FROM operation");
    if (!query.exec())
	return 0;
    if (query.next())
        return query.value(0).toInt();
    return 0;
}

bool Database::new_account_oper(const int a_id, const int o_id, const double delta)
{
    QSqlQuery q;
    Account_Data data = get_account_data(a_id);
    int flag = (data.type == Active_type || data.type == Credit_type) ? 1 : -1;
    double summ = delta * flag;

    q.prepare("INSERT INTO account_oper(a_id, o_id, summ) VALUES(:a_id, :o_id, :summ)");
    q.bindValue(":a_id", a_id);
    q.bindValue(":o_id", o_id);
    q.bindValue(":summ", summ);
    if (!q.exec())
        return false;

    return true;
}

bool Database::del_account_oper(int id)
{
    QSqlQuery q;

    q.prepare("DELETE FROM account_oper WHERE o_id = :id");
    q.bindValue(":id", id);
    if (!q.exec())
        return false;

    return true;
}

bool Database::del_operation(int id)
{
    QSqlQuery q;

    q.prepare("DELETE FROM operation WHERE o_id = :id");
    q.bindValue(":id", id);
    if (!q.exec())
        return false;

    return true;
}

bool Database::change_account_balance(const int id, const double delta)
{
    QSqlQuery query;
    double summ;
    Account_Data data;
    int type, flag;

    query.exec("BEGIN TRANSACTION");

    data = get_account_data(id);

    if (data.type == Active_type || data.type == Credit_type)
        flag = 1;
    else if (data.type < 1 || data.type > 4) {
    qDebug() << "type is unknown: " << data.type;
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

bool Database::save_operation(operation_data &data)
{
    QSqlQuery q;
    int oper_id;

    q.exec("BEGIN TRANSACTION");

    if ((oper_id = new_operation(data)) == 0) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (new_account_oper(data.from, oper_id, -data.summ) == false) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (new_account_oper(data.to, oper_id, data.summ) == false) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (change_account_balance(data.from, -data.summ) == false) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (change_account_balance(data.to, data.summ) == false) {
	q.exec("ROLLBACK TRANSACTION");
        return false;
    }

    q.exec("COMMIT TRANSACTION");
    return true;
}

operation_data Database::get_operation(int id)
{
    QSqlQuery q;
    operation_data data;

    q.prepare("SELECT acc_from,acc_to,agent,summ,dt,descr,plan_id FROM operation WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        return data;
    }
    if (q.next()) {
        data.from = q.value(0).toInt();
	data.to   = q.value(1).toInt();
        data.agent = q.value(2).toInt();
	data.summ = q.value(3).toDouble();
        data.date = q.value(4).toString();
	data.descr = q.value(5).toString();
        data.plan_id = q.value(6).toInt();
    }

    return data;
}

int Database::new_plan_oper(PlanOper_data &data)
{
    QSqlQuery q;

    q.prepare("INSERT INTO plan_oper(day, month, year, acc_from, acc_to, summ, descr) VALUES(:day, :month, :year, :from, :to, :summ, :descr)");
    q.bindValue(":day", data.day);
    q.bindValue(":month", data.month);
    q.bindValue(":year", data.year);
    q.bindValue(":from", data.from);
    q.bindValue(":to", data.to);
    q.bindValue(":summ", data.summ);
    q.bindValue(":descr", data.descr);
    if (!q.exec()) {
        qDebug() << "Error Insert:" << q.lastError().text();
        return 0;
    }

    q.prepare("SELECT MAX(id) FROM plan_oper");
    if (!q.exec())
    return 0;
    if (q.next())
        return q.value(0).toInt();
    return 0;
}

QList<PlanOper_data> Database::get_plan_oper_list()
{
    QList<PlanOper_data> list;
    PlanOper_data data;
    QSqlQuery q;

    q.prepare("SELECT id, day, month, year, acc_from, acc_to, summ, descr FROM plan_oper ORDER BY day");
    if (!q.exec()) {
        qDebug() << "Select Error:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
//        data = new PlanOper_data;
        data.id = q.value(0).toInt();
        data.day = q.value(1).toInt();
        data.month = q.value(2).toInt();
        data.year = q.value(3).toInt();
        data.from = q.value(4).toInt();
        data.to = q.value(5).toInt();
        data.summ = q.value(6).toDouble();
        data.descr = q.value(7).toString();

        list.append(data);
    }

    return list;
}

PlanOper_data Database::get_plan_oper_data(int id)
{
    PlanOper_data data;
    QSqlQuery q;

    q.prepare("SELECT id, day, month, year, acc_from, acc_to, summ, descr FROM plan_oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "Select Error:" << q.lastError().text();
        return data;
    }
    if (q.next()) {
        data.id = q.value(0).toInt();
	data.day = q.value(1).toInt();
        data.month = q.value(2).toInt();
	data.year = q.value(3).toInt();
        data.from = q.value(4).toInt();
	data.to = q.value(5).toInt();
        data.summ = q.value(6).toDouble();
	data.descr = q.value(7).toString();
    }

    return data;
}
