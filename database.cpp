#include "database.h"

Database::Database()
{
//    opened = false;
    scod_list = get_scod_list();
    currency_list = get_currency_list();
}

double Database::convert_currency(double val, int icod)
{
    QString scod = scod_list[icod];
    double account_kurs = currency_list[scod];
    double global_kurs = currency_list[var.Symbol()];
    double kurs = account_kurs / global_kurs;

    if (global_kurs == 0)
        return 0;

    return val * kurs;
}

double Database::get_account_summ(int top)
{
    QSqlQuery query;
    double summ = 0;

    if (!var.database_Opened())
        return summ;

    query.prepare("SELECT id,ccod,balance FROM account WHERE top = :top AND hidden = 0");
    query.bindValue(":top", top);
    if (!query.exec())
        return 0;
    while (query.next()) {
        summ += convert_currency(query.value(2).toDouble(), query.value(1).toInt());
    }

    return summ;
}

double Database::get_operation_summ(int top)
{
    QSqlQuery q;
    QDate current = QDate::currentDate();
    int month = current.month();
    int year = current.year();
    double summ = 0;

    QString query = QString("SELECT id,dt FROM oper WHERE dt >= '%1-%2-01'").arg(year).arg(month);
//    qDebug() << query;
    if (!q.exec(query)) {
        qDebug() << q.lastError().text();
        return 0;
    }
    while (q.next()) {
//        qDebug() << q.value(1).toString();
        QSqlQuery q2;
        q2.prepare("SELECT ao.a_id,ao.summ FROM account a, account_oper ao WHERE ao.o_id = :id AND a.id = ao.a_id AND a.top = :top");
        q2.bindValue(":id", q.value(0).toInt());
        q2.bindValue(":top", top);
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

QMap<QString,double> Database::get_opersummbyaccount_list(int top, int m1, int y1)
{
    QMap<QString,double> list;
    QSqlQuery q1,q2;
    int m2,y2;
    QDate bd, ld;

    if (m1 == 12) {
        m2 = 1;
        y2 = y1+1;
    }
    else {
        m2 = m1 + 1;
        y2 = y1;
    }

    bd.setDate(y1, m1, 1);
    ld.setDate(y2, m2, 1);

    QString query = QString("SELECT id FROM oper WHERE dt >= '%1' AND dt < '%2'")
            .arg(bd.toString("yyyy-MM-dd"))
            .arg(ld.toString("yyyy-MM-dd"));
//    qDebug() << query;

    if (!q1.exec(query)) {
        qDebug() << q1.lastError().text();
        return list;
    }
    while (q1.next()) {
        q2.prepare("SELECT a.name,ao.summ FROM account a, account_oper ao WHERE ao.o_id = :id AND a.id = ao.a_id AND a.top = :top");
        q2.bindValue(":id", q1.value(0).toInt());
        q2.bindValue(":top", top);
        if (!q2.exec()) {
            qDebug() << q2.lastError().text();
        }
        while (q2.next()) {
//            qDebug() << q2.value(0).toString() << q2.value(1).toDouble();
            list[q2.value(0).toString()] += q2.value(1).toDouble();
        }
    }

    return list;
}

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

QMap<int,double> Database::get_account_oper_list(int oper, int type)
{
    QMap<int,double> list;
    QSqlQuery q;

    q.prepare("SELECT a_id,summ FROM account_oper WHERE o_id = :oper AND direction = :type");
    q.bindValue(":oper", oper);
    q.bindValue(":type", type);
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return list;
    }
    while (q.next()) {
        list[q.value(0).toInt()] = q.value(1).toDouble();
    }

    return list;
}

Account_Data Database::get_account_data(int id)
{
    Account_Data data;
    QSqlQuery q;

    q.prepare("SELECT name,type,balance,descr,ccod,hidden,parent,top,system FROM account WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "SELECT Error:" << q.lastError().text();
        return data;
    }
    if (q.next()) {
        data.name = q.value(0).toString();
        data.type = q.value(1).toInt();
        data.balance.setValue(q.value(2).toDouble());
        data.descr = q.value(3).toString();
        data.curr = q.value(4).toInt();
        data.hidden = q.value(5).toInt();
        data.parent = q.value(6).toInt();
        data.top = q.value(7).toInt();
        data.system = q.value(8).toInt();
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

/*
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
*/

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

int Database::new_account(Account_Data &data)
{
    QSqlQuery q;

    q.prepare("INSERT INTO account(name, type, ccod, balance, descr, hidden, parent, top) VALUES(:name, :type, :ccod, :balance, :descr, :hidden, :parent, :top)");
    q.bindValue(":name",    data.name);
    q.bindValue(":type",    data.type);
    q.bindValue(":ccod",    data.curr);
    q.bindValue(":balance", 0);
    q.bindValue(":descr",   data.descr);
    q.bindValue(":hidden",  (data.hidden == false) ? 0 : 1);
    q.bindValue(":parent",  data.parent);
    q.bindValue(":top",     data.top);
    if (!q.exec())
        return 0;

    q.prepare("SELECT MAX(id) FROM account");
    if (!q.exec())
        return 0;
    if (q.next())
        return q.value(0).toInt();
    return 0;
}

int Database::new_agent(agent_data &data) {
    QSqlQuery q;

    q.prepare("INSERT INTO agent(name, city, address, phone, contact) VALUES(:name, :city, :address, :phone, :contact)");
    q.bindValue(":name", data.name());
    q.bindValue(":city", data.city());
    q.bindValue(":address", data.address());
    q.bindValue(":phone", data.phone());
    q.bindValue(":contact", data.contact());
    if (!q.exec()) {
        qDebug() << "Insert error" << q.lastError().text();
        return 0;
    }

    q.prepare("SELECT MAX(id) FROM agent");
    if (!q.exec())
        return 0;
    if (q.next())
        return q.value(0).toInt();
    return 0;
}

int Database::new_operation(operation_data &data)
{
    QSqlQuery query;

    query.prepare("INSERT INTO oper(dt, descr, plan_id) VALUES(:dt, :descr, :plan_id)");
//    query.bindValue(":from", data.from.account());
//    query.bindValue(":to", data.to.account());
//    query.bindValue(":agent", data.agent);
//    query.bindValue(":summ", data.from.summ());
    query.bindValue(":dt", data.date);
    query.bindValue(":descr", data.descr);
    query.bindValue(":plan_id", data.plan_id);
    if (!query.exec())
        return 0;

    query.prepare("SELECT MAX(id) FROM oper");
    if (!query.exec())
        return 0;
    if (query.next())
        return query.value(0).toInt();
    return 0;
}

bool Database::new_account_oper(QString table, const int o_id, account_summ &acc, const int direction)
{
    QString str;
    QSqlQuery q;
//    Account_Data data = get_account_data(a_id);
//    int flag = (data.type == Active_type || data.type == Credit_type) ? 1 : -1;
//    double summ = delta * flag;

    str = QString("INSERT INTO %1(a_id, o_id, summ, direction) VALUES(%2, %3, %4, %5)")
            .arg(table).arg(acc.account()).arg(o_id).arg(acc.summ()).arg(direction);
    qDebug() << str;
    if (!q.exec(str)) {
        qDebug() << q.lastError().text();
        return false;
    }

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
    QMap<int,double> list;
    QMap<int,double>::iterator i;
    account_summ acc;

    q.exec("BEGIN");

    list = get_account_oper_list(id,1);
    for (i = list.begin(); i != list.end(); i++) {
        acc.set_account(i.key());
        acc.set_summ(i.value());
        if (change_account_balance(acc) == false) {
            q.exec("ROLLBACK");
            return false;
        }
    }

    list = get_account_oper_list(id,2);
    for (i = list.begin(); i != list.end(); i++) {
        acc.set_account(i.key());
        acc.set_summ(-i.value());
        if (change_account_balance(acc) == false) {
            q.exec("ROLLBACK");
            return false;
        }
    }

    if (del_account_oper(id) == false) {
        q.exec("ROLLBACK");
        return false;
    }

    q.prepare("DELETE FROM oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        q.exec("ROLLBACK");
        return false;
    }

    q.exec("COMMIT");

    return true;
}

bool Database::change_account_balance(account_summ &acc)
{
    QSqlQuery query;
    double summ;
    Account_Data data;
    int flag;

    data = get_account_data(acc.account());

    if (data.top == Active_type || data.top == Credit_type)
        flag = 1;
    else
        flag = -1;

    summ = acc.summ() * flag;

    query.prepare("UPDATE account set balance = balance + :summ WHERE id = :id");
    query.bindValue(":id", acc.account());
    query.bindValue(":summ", summ);
    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Database::save_operation(operation_data &data)
{
    QSqlQuery q;
    int oper_id;
    QList<account_summ>::iterator i;

    q.exec("BEGIN TRANSACTION");

    if ((oper_id = new_operation(data)) == 0) {
        q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    if (new_account_oper("account_oper", oper_id, data.from, direction_from) == false) {
        q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    for (i = data.to.begin(); i != data.to.end(); i++) {
        account_summ d = *i;
        if (new_account_oper("account_oper", oper_id, d, direction_to) == false) {
            q.exec("ROLLBACK TRANSACTION");
            return false;
        }
    }
    double val = data.from.summ();
    data.from.set_summ(val*-1);
    if (change_account_balance(data.from) == false) {
        q.exec("ROLLBACK TRANSACTION");
        return false;
    }
    for (i = data.to.begin(); i != data.to.end(); i++) {
        account_summ d = *i;
        if (change_account_balance(d) == false) {
            q.exec("ROLLBACK TRANSACTION");
            return false;
        }
    }
//    if (data.to2.summ() > 0) {
//        if (new_account_oper("account_oper", oper_id, data.to2, direction_to) == false) {
//            q.exec("ROLLBACK TRANSACTION");
//            return false;
//        }
//        if (change_account_balance(data.to2) == false) {
//            q.exec("ROLLBACK TRANSACTION");
//            return false;
//        }
//    }

    q.exec("COMMIT TRANSACTION");
    return true;
}

operation_data Database::get_operation(int id)
{
    QSqlQuery q;
    operation_data data;
    QMap<int,double> list;
    QMap<int,double>::iterator i;

    q.prepare("SELECT id,agent,dt,descr,plan_id FROM oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        return data;
    }
    if (q.next()) {
        data.agent =     q.value(1).toInt();
        data.date =      q.value(2).toString();
        data.descr =     q.value(3).toString();
        data.plan_id =   q.value(4).toInt();
    }

    list = get_account_oper_list(q.value(0).toInt(), 1);
    i = list.begin();
    if (i != list.end()) {
        data.from.set_account(i.key());
        data.from.set_summ(i.value());
    }

    list = get_account_oper_list(q.value(0).toInt(), 2);
    for (i = list.begin(); i != list.end(); i++) {
        if (i != list.end()) {
            account_summ d;
                d.set_account(i.key());
            d.set_summ(i.value());
            data.to.append(d);
//        data.to.set_account(i.key());
//        data.to.set_summ(i.value());
        }
    }

//    i++;
//    if (i != list.end()) {
//        data.to2.set_account(i.key());
//        data.to2.set_summ(i.value());
//    }

    return data;
}

int Database::new_plan_oper(operation_data &data)
{
    QSqlQuery q;
    int id = 0;
    QList<account_summ>::iterator i;

    q.exec("BEGIN");

    q.prepare("INSERT INTO plan_oper(day, month, year, descr) VALUES(:day, :month, :year, :descr)");
    q.bindValue(":day", data.day);
    q.bindValue(":month", data.month);
    q.bindValue(":year", data.year);
    q.bindValue(":descr", data.descr);
    if (!q.exec()) {
        qDebug() << "Error Insert:" << q.lastError().text();
        q.exec("ROLLBACK");
        return 0;
    }

    q.prepare("SELECT MAX(id) FROM plan_oper");
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        q.exec("ROLLBACK");
        return 0;
    }

    if (q.next()) {
        id = q.value(0).toInt();
        if (!new_account_oper("plan_account_oper", id, data.from, direction_from)) {
            qDebug() << q.lastError().text();
            q.exec("ROLLBACK");
            return 0;
        }
        for (i = data.to.begin(); i != data.to.end(); i++) {
            account_summ d = *i;
        if (!new_account_oper("plan_account_oper", id, d, direction_to)) {
            qDebug() << q.lastError().text();
            q.exec("ROLLBACK");
            return 0;
        }
        }
//        if (data.to2.summ() > 0) {
//            if (!new_account_oper("plan_account_oper", id, data.to2, direction_to)) {
//                qDebug() << q.lastError().text();
//                q.exec("ROLLBACK");
//                return 0;
//            }
//        }
    }

    q.exec("COMMIT");
    return id;
}

QList<operation_data> Database::get_plan_oper_list()
{
    QList<operation_data> list;
    operation_data data;
    QSqlQuery q;

    q.prepare("SELECT id FROM plan_oper ORDER BY day");
    if (!q.exec()) {
        qDebug() << "Select Error:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
	data = get_plan_oper_data(q.value(0).toInt());
        list.append(data);
    }

    return list;
}

QMap<int,double> Database::get_plan_account_oper_list(int oper, int type)
{
    QMap<int,double> list;
    QSqlQuery q;

    q.prepare("SELECT a_id,summ FROM plan_account_oper WHERE o_id = :oper AND direction = :type");
    q.bindValue(":oper", oper);
    q.bindValue(":type", type);
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return list;
    }
    while (q.next()) {
        list[q.value(0).toInt()] = q.value(1).toDouble();
    }

    return list;
}

operation_data Database::get_plan_oper_data(int id)
{
    operation_data data;
    QSqlQuery q;
    QMap<int,double> list;
    QMap<int,double>::iterator i;

    q.prepare("SELECT id, day, month, year, descr FROM plan_oper WHERE id = :id");
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
        data.descr = q.value(7).toString();
        list = get_plan_account_oper_list(q.value(0).toInt(), 1);
        i = list.begin();
        if (i != list.end()) {
            data.from.set_account(i.key());
            data.from.set_summ(i.value());
        }
        list = get_plan_account_oper_list(q.value(0).toInt(), 2);
        i = list.begin();
        if (i != list.end()) {
            account_summ d;
            d.set_account(i.key());
            d.set_summ(i.value());
            data.to.append(d);
//            data.to.set_account(i.key());
//            data.to.set_summ(i.value());
        }
    }

    return data;
}
