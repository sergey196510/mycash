#include "database.h"
#include "transaction.h"

Database::Database()
{
//    opened = false;
//    scod_list2 = get_scod_list();
//    currency_list = get_currency_list();
}

double Database::convert_currency(double val, int id)
{
//    QString scod = scod_list2[id];
//    double account_kurs = currency_list[scod];
//    double global_kurs = currency_list[var.Symbol()];
    double global_kurs = Currency(id).Kurs();
//    double kurs = account_kurs / global_kurs;

    if (global_kurs == 0)
        return 0;

    return 0;
}

/*
double Database::get_account_summ(int id)
{
    QSqlQuery q;
    double summ = 0;
    Transaction tr;

    if (!var.database_Opened())
        return summ;

    tr.begin();

    q.prepare("SELECT id,ccod,balance FROM account WHERE id = :id AND hidden = 0");
    q.bindValue(":id", id);
    if (!q.exec()) {
        tr.rollback();
        return 0;
    }
    if (q.next())
        summ += q.value(2).toDouble();

    q.prepare("SELECT id,ccod,balance FROM account WHERE parent = :parent AND hidden = 0");
    q.bindValue(":parent", id);
    if (!q.exec()) {
        tr.rollback();
        return 0;
    }
    while (q.next()) {
        summ += get_account_summ(q.value(0).toInt());
    }

    tr.commit();
    return summ;
}
*/

double Database::get_operation_summ(int top)
{
    QSqlQuery q;
    QDate current = QDate::currentDate();
    QDate dt(current.year(), current.month(), 1);
    double summ = 0;

    QString query = QString("SELECT id,dt FROM oper WHERE dt >= '%1'")
            .arg(dt.toString("yyyy-MM-dd"));
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

/*
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
*/

/*
int Database::get_account_curr(int id)
{
    QSqlQuery query;

    query.prepare("SELECT ccod FROM account a WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
        return 0;
    if (query.next())
        return query.value(0).toInt();
    return 0;
}
*/

/*
QString Database::get_parent_account(int id)
{
    QString name;
    QSqlQuery q;

    q.prepare("SELECT id,name,parent FROM account WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return QVariant().toString();
    }
    if (q.next()) {
        QString s = q.value(1).toString();
        QString str;
        if (q.value(2).toInt() > 0) {
            str = get_parent_account(q.value(2).toInt()) + ":";
        }
        return str + s;
    }
    return QVariant().toString();
}
*/

/*
QMap<int,QString> Database::get_accounts_list()
{
    QMap<int,QString> list;
    QSqlQuery q;

    q.prepare("SELECT id,name,parent FROM account");
    if (!q.exec())
        return list;
    while (q.next()) {
        QString s = q.value(1).toString();
        QString str;
        if (q.value(2).toInt() > 0) {
            str = get_parent_account(q.value(2).toInt()) + ":";
        }
        list[q.value(0).toInt()] = str + s;
    }

    return list;
}
*/

/*
QList<Budget> Database::read_list(int mon)
{
    QList<Budget> list;
    Budget data;
    QSqlQuery q;
    QString query;

    if (mon)
        query = QString("SELECT id,mon,a_id,summ FROM budget_plan WHERE mon=%1 ORDER BY mon,a_id").arg(mon);
    else
        query = QString("SELECT id,mon,a_id,summ FROM budget_plan ORDER BY mon,a_id");

    q.prepare(query);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return list;
    }
    while (q.next()) {
        data.set_Value(q.value(0).toInt(), q.value(1).toInt(), q.value(2).toInt(), q.value(3).toDouble());
        list.append(data);
    }

    return list;
}
*/

/*
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
*/

/*
Account Database::get_account(int id)
{
    Account data;
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
        data.balance = q.value(2).toDouble();
        data.descr = q.value(3).toString();
        data.curr = q.value(4).toInt();
        data.hidden = q.value(5).toInt();
        data.parent = q.value(6).toInt();
        data.top = q.value(7).toInt();
        data.system = q.value(8).toInt();
    }
    return data;
}
*/

/*
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
*/

/*
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
*/

/*
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
*/

/*
int Database::new_account(Account_Data &data)
{
    QSqlQuery q;

    q.prepare("INSERT INTO account(name, type, ccod, balance, descr, hidden, parent, top, dt) VALUES(:name, :type, :ccod, :balance, :descr, :hidden, :parent, :top, :dt)");
    q.bindValue(":name",    data.name);
    q.bindValue(":type",    data.type);
    q.bindValue(":ccod",    data.curr);
    q.bindValue(":balance", 0);
    q.bindValue(":descr",   data.descr);
    q.bindValue(":hidden",  (data.hidden == false) ? 0 : 1);
    q.bindValue(":parent",  data.parent);
    q.bindValue(":top",     data.top);
    q.bindValue(":dt", data.dt);
    if (!q.exec())
        return 0;

    q.prepare("SELECT MAX(id) FROM account");
    if (!q.exec())
        return 0;
    if (q.next())
        return q.value(0).toInt();
    return 0;
}
*/

/*
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

int Database::new_operation(Operation &data)
{
    QSqlQuery query;

    query.prepare("INSERT INTO oper(dt, descr) VALUES(:dt, :descr)");
    query.bindValue(":dt", data.Date());
    query.bindValue(":descr", data.Descr());
    qDebug() << query.executedQuery();
    if (!query.exec())
        return 0;

    query.prepare("SELECT MAX(id) FROM oper");
    if (!query.exec())
        return 0;
    if (query.next())
        return query.value(0).toInt();
    return 0;
}

bool Database::new_account_oper(QString table, const int o_id, account_summ &acc, const int direction, int agent)
{
    QString str;
    QSqlQuery q;

    qDebug() << acc.balance().toDouble();
    str = QString("INSERT INTO %1(a_id, o_id, summ, direction, agent) VALUES(%2, %3, %4, %5, %6)")
            .arg(table)
            .arg(acc.account().Id())
            .arg(o_id)
            .arg(acc.balance().toDouble())
            .arg(direction)
            .arg(agent);
    qDebug() << str;
    if (!q.exec(str)) {
        qDebug() << q.lastError().text();
        return false;
    }

    return true;
}
*/

bool Database::del_account_oper(int id)
{
    QSqlQuery q;

    q.prepare("DELETE FROM account_oper WHERE o_id = :id");
    q.bindValue(":id", id);
    if (!q.exec())
        return false;

    return true;
}

/*
bool Database::del_operation(int id)
{
    QSqlQuery q;
    QMap<int,double> list;
    QMap<int,double>::iterator i;
//    account_summ acc;
    Transaction tr;

    tr.begin();

    list = get_account_oper_list(id,Direction::from);
    for (i = list.begin(); i != list.end(); i++) {
        account_summ acc;
        Account data;
        data.read(i.key());
        acc.set_account(data);
        acc.set_balance((data.Top() == Account_Type::passive ||
                        data.Top() == Account_Type::debet ||
                        data.Top() == Account_Type::initial) ?
                           -i.value() :
                           i.value());
        if (change_account_balance(acc) == false) {
            tr.rollback();
            return false;
        }
    }

    list = get_account_oper_list(id,Direction::to);
    for (i = list.begin(); i != list.end(); i++) {
        account_summ acc;
        Account data;
        data.read(i.key());
        acc.set_account(data);
        acc.set_balance(-i.value());
        if (change_account_balance(acc) == false) {
            tr.rollback();
            return false;
        }
    }

    if (del_account_oper(id) == false) {
        tr.rollback();
        return false;
    }

    q.prepare("DELETE FROM oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        tr.rollback();
        return false;
    }

    tr.commit();

    return true;
}

bool Database::change_account_balance(account_summ &acc)
{
    QSqlQuery query;
    MyCurrency summ;
//    Account data;
    int flag = 1;

//    data = get_account_data(acc.account());

//    if (data.top == Account_Type::active || data.top == Account_Type::debet)
//        flag = 1;
//    else
//        flag = -1;

    summ = acc.balance() * flag;

    query.prepare("UPDATE account set balance = balance + :summ WHERE id = :id");
    query.bindValue(":id", acc.account().Id());
    query.bindValue(":summ", summ.toDouble());
    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Database::save_operation(Operation &oper)
{
    QSqlQuery q;
    int oper_id;
    QList<account_summ>::iterator i;
    Transaction tr;
    MyCurrency from = 0, to = 0;

    tr.begin();

    if ((oper_id = new_operation(oper)) == 0) {
        tr.rollback();
        return false;
    }
    for (i = oper.from.begin(); i != oper.from.end(); i++) {
        account_summ d = *i;
        Account data;
        data.read(d.account().Id());
        if (data.Top() == Account_Type::debet && oper.agent)
            data.setAgent(oper.agent);
        if (new_account_oper("account_oper", oper_id, d, Direction::from, data.Agent()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::active ||
                data.Top() == Account_Type::credit)
            d.set_balance(-d.balance().toDouble()); // сменить знак
        if (change_account_balance(d) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::debet) {
            add_budget(d);
        }
        from += abs(d.balance().toDouble());
    }
    for (i = oper.to.begin(); i != oper.to.end(); i++) {
        account_summ d = *i;
        Account data;
        data.read(d.account().Id());
        if (data.Top() == Account_Type::credit && oper.agent)
            data.setAgent(oper.agent);
        if (new_account_oper("account_oper", oper_id, d, Direction::to, data.Agent()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::passive ||
                data.Top() == Account_Type::debet ||
                data.Top() == Account_Type::initial)
            d.set_balance(-d.balance().toDouble()); // сменить знак
        if (change_account_balance(d) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::credit) {
            add_budget(d);
        }
        to += abs(d.balance().toDouble());
    }

    if (from != to) {
        qDebug() << "from not equal to";
        tr.rollback();
        return false;
    }

    tr.commit();
    return true;
}
*/

/*
bool Database::add_budget(account_summ &d)
{
    QSqlQuery q;
    QList<Budget> list = read_budget_list(QDate::currentDate().month());
    QList<Budget>::iterator i;

    for (i = list.begin(); i != list.end(); i++) {
        Budget data = *i;
        if (find_budget_id(data.account().Id(), d.account().Id())) {
            int id = data.Id();
            qDebug() << id;
        }
    }

    return true;
}

bool Database::find_budget_parent(int budget, int acc)
{
    QSqlQuery q;

    if (budget == acc)
        return true;

    q.prepare("SELECT id,parent FROM account WHERE id=:id");
    q.bindValue(":id",acc);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }
    if (q.next()) {
        if (q.value(1).toInt())
            return find_budget_id(budget,q.value(1).toInt());
//        else
//            return true;
    }

    return false;
}
*/

/*
Operation Database::get_operation(int id)
{
    QSqlQuery q;
    Operation data;
    QMap<int,double> list;
    QMap<int,double>::iterator i;

    q.prepare("SELECT id,dt,descr FROM oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec() || !q.next()) {
        return data;
    }

    data.date =      q.value(2).toDate();
    data.descr =     q.value(3).toString();

    list = get_account_oper_list(q.value(0).toInt(), Direction::from);
    for (i = list.begin(); i != list.end(); i++) {
        account_summ d;
        Account acc;
        acc.read(i.key());
        d.set_account(acc);
        d.set_balance(i.value());
        data.from.append(d);
    }

    list = get_account_oper_list(q.value(0).toInt(), Direction::to);
    for (i = list.begin(); i != list.end(); i++) {
        account_summ d;
        Account acc;
        acc.read(i.key());
        d.set_account(acc);
        d.set_balance(i.value());
        data.to.append(d);
    }

    return data;
}

int Database::new_plan_oper(Operation_Data &data)
{
    QSqlQuery q;
    int id = 0;
    QList<account_summ>::iterator i;
    Transaction tr;

    tr.begin();

    q.prepare("INSERT INTO plan_oper(day, month, year, descr, dt, auto) VALUES(:day, :month, :year, :descr, :dt, :auto)");
    q.bindValue(":day", data.day);
    q.bindValue(":month", data.month);
    q.bindValue(":year", data.year);
    q.bindValue(":descr", data.descr);
    q.bindValue(":dt", data.date.toString("yyyy-MM-dd"));
    q.bindValue(":auto", data.auto_exec);
    if (!q.exec()) {
        qDebug() << "Error Insert:" << q.lastError().text();
        tr.rollback();
        return 0;
    }

    q.prepare("SELECT MAX(id) FROM plan_oper");
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        tr.rollback();
        return 0;
    }

    if (q.next()) {
        id = q.value(0).toInt();
        for (i = data.from.begin(); i != data.from.end(); i++) {
            account_summ d = *i;
            if (!new_account_oper("plan_oper_acc", id, d, Direction::from)) {
                qDebug() << q.lastError().text();
                tr.rollback();
                return 0;
            }
        }
        for (i = data.to.begin(); i != data.to.end(); i++) {
            account_summ d = *i;
            if (!new_account_oper("plan_oper_acc", id, d, Direction::to)) {
                qDebug() << q.lastError().text();
                tr.rollback();
                return 0;
            }
        }
    }

    tr.commit();
    return id;
}

bool Database::update_plan_oper(Operation_Data &data)
{
    QSqlQuery q;
    QList<account_summ>::iterator i;
    Transaction tr;

    tr.begin();

    q.prepare("DELETE FROM plan_oper_acc WHERE o_id = :id");
    q.bindValue(":id", data.id);
    if (!q.exec()) {
        tr.rollback();
        qDebug() << "Error DELETE:" << q.lastError().text();
        return false;
    }

    q.prepare("UPDATE plan_oper SET day=:day, month=:month, year=:year, descr=:descr, auto=:auto WHERE id=:id");
    q.bindValue(":day", data.day);
    q.bindValue(":month", data.month);
    q.bindValue(":year", data.year);
    q.bindValue(":descr", data.descr);
    q.bindValue(":auto", data.auto_exec);
    q.bindValue(":id", data.id);
    if (!q.exec()) {
        qDebug() << "Error update:" << q.lastError().text();
        tr.rollback();
        return false;
    }

    for (i = data.from.begin(); i != data.from.end(); i++) {
        account_summ d = *i;
        if (!new_account_oper("plan_oper_acc", data.id, d, Direction::from)) {
            qDebug() << q.lastError().text();
            tr.rollback();
            return false;
        }
    }
    for (i = data.to.begin(); i != data.to.end(); i++) {
        account_summ d = *i;
        if (!new_account_oper("plan_oper_acc", data.id, d, Direction::to)) {
            qDebug() << q.lastError().text();
            tr.rollback();
            return false;
        }
    }

    tr.commit();
    return true;
}
*/

bool Database::new_mon_oper(int p_id, int status)
{
    QSqlQuery q;
    QDate curr = QDate::currentDate();

    q.prepare("INSERT INTO plan_oper_mon(mon, year, p_id, dt, status) VALUES(:mon, :year, :p_id, :dt, :status)");
    q.bindValue(":mon", curr.month());
    q.bindValue(":year", curr.year());
    q.bindValue(":p_id", p_id);
    q.bindValue(":dt", curr.toString("yyyy-MM-dd"));
    q.bindValue(":status", status);
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        q.exec("ROLLBACK");
        return false;
    }

    return true;
}

/*
QList<Operation_Data> Database::get_plan_oper_list(int status)
{
    QList<Operation_Data> list;
    Operation_Data oper;
    QSqlQuery q;

    q.prepare("SELECT id,dt FROM plan_oper ORDER BY day");
    if (!q.exec()) {
        qDebug() << "Select Error:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
        oper = get_plan_oper_data(q.value(0).toInt(), q.value(1).toDate());
        if (status && (oper.status == Plan_Status::actual || oper.status == Plan_Status::committed || oper.status == Plan_Status::cancelled))
            continue;
        list.append(oper);
    }

    return list;
}

QMap<int,double> Database::get_plan_account_oper_list(int oper, int type)
{
    QMap<int,double> list;
    QSqlQuery q;

    q.prepare("SELECT a_id,summ FROM plan_oper_acc WHERE o_id = :oper AND direction = :type");
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

Operation_Data Database::get_plan_oper_data(int id, QDate oper_date)
{
    QSqlQuery q;
//    Account_Data acc;
    QMap<int,double> list;
    QMap<int,double>::iterator i;
    QDate curr = QDate::currentDate();
//    QDate future = curr.addDays(3);
    MyCurrency summ_from = 0, summ_to = 0;
    Operation_Data data;

    q.prepare("SELECT id, day, month, year, descr, auto FROM plan_oper WHERE id = :id");
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
        data.descr = q.value(4).toString();
        data.auto_exec = q.value(5).toInt();
        data.date = oper_date;

        // поиск близких планвых операций учетом перехода через границу месяца
        int diff = 0;
        for (int j = 0; j <= 3; j++) {
            QDate future = curr.addDays(j);
            if (data.day == future.day()) {
                data.status = Plan_Status::minimum;
                diff = 1;
                break;
            }
        }
        if (diff == 0)
            diff = data.day - curr.day();

        QDate dt(curr.year(), curr.month(), data.day);
        if (data.date > dt) {
            data.status = Plan_Status::actual;
        }
        else if (int stat = find_oper_by_plan(data.id, curr.month(), curr.year())) {
            data.status = stat;
        }
        else if (diff == 1)
            data.status = Plan_Status::minimum;
        else if (diff < 0)
            data.status = Plan_Status::expired;
        else
            data.status = Plan_Status::actual;

        bool from_top = false;
        list = get_plan_account_oper_list(q.value(0).toInt(), Direction::from);
        for (i = list.begin(); i != list.end(); i++) {
            Account acc;
            acc.read(i.key());
            account_summ d;
            d.set_account(acc);
            d.set_balance(i.value());
            data.from.append(d);
            if (acc.Top() == Account_Type::active) {
                from_top = true;
                summ_from += acc.Balance();
            }
        }

        list = get_plan_account_oper_list(q.value(0).toInt(), Direction::to);
        for (i = list.begin(); i != list.end(); i++) {
            Account acc;
            acc.read(i.key());
            account_summ d;
            d.set_account(acc);
            d.set_balance(i.value());
            data.to.append(d);
            if (acc.Top() == Account_Type::credit)
                summ_to += i.value();
        }
        if (from_top == true && (data.status == Plan_Status::minimum || data.status == Plan_Status::expired) && summ_from < summ_to)
            data.descr += QObject::tr(" [Nedostatochno sredstv]");
    }

    return data;
}

int Database::find_oper_by_plan(int plan, int mon, int year)
{
        QSqlQuery q;

        q.prepare("SELECT status FROM plan_oper_mon WHERE mon = :mon AND year = :year AND p_id = :pid");
        q.bindValue(":mon", mon);
        q.bindValue(":year", year);
        q.bindValue(":pid", plan);
        if (!q.exec()) {
            qDebug() << q.lastError().text();
            return false;
        }
        if (!q.next())
            return 0;
        if (q.value(0).toInt() == 1)
            return Plan_Status::committed;
        if (q.value(0).toInt() == 2)
            return Plan_Status::cancelled;
        return 0;
}
*/
