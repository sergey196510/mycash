#include "operation.h"

Operation::Operation() {
    id = day = month = year = 0;
    agent = 0;
    auto_exec = 0;
    date = QDate::currentDate();
    descr.clear();
}

int Operation::new_operation()
{
    QSqlQuery query;

    query.prepare("INSERT INTO oper(dt, descr) VALUES(:dt, :descr)");
    query.bindValue(":dt", date);
    query.bindValue(":descr", descr);
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

bool Operation::save_operation()
{
    QSqlQuery q;
    int oper_id;
    QList<account_summ>::iterator i;
    Transaction tr;
    MyCurrency mfrom = 0, mto = 0;

    tr.begin();

    if ((oper_id = new_operation()) == 0) {
        tr.rollback();
        return false;
    }
    for (i = from.begin(); i != from.end(); i++) {
        account_summ d = *i;
        Account data = d.account();
//        data.read(d.account().Id());
        if (data.Top() == Account_Type::debet && agent)
            data.setAgent(agent);
        if (new_account_oper("account_oper", oper_id, d, Direction::from, data.Agent()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::active ||
                data.Top() == Account_Type::credit)
            d.set_balance(-d.balance().toDouble()); // сменить знак
        if (data.change_balance(d.balance()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::debet) {
            Database().add_budget(d);
        }
        mfrom += abs(d.balance().toDouble());
    }
    for (i = to.begin(); i != to.end(); i++) {
        account_summ d = *i;
        Account data = d.account();
//        data.read(d.account().Id());
        if (data.Top() == Account_Type::credit && agent)
            data.setAgent(agent);
        if (new_account_oper("account_oper", oper_id, d, Direction::to, data.Agent()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::passive ||
                data.Top() == Account_Type::debet ||
                data.Top() == Account_Type::initial)
            d.set_balance(-d.balance().toDouble()); // сменить знак
        if (data.change_balance(d.balance()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::credit) {
            Database().add_budget(d);
        }
        mto += abs(d.balance().toDouble());
    }

    if (mfrom != mto) {
        qDebug() << "from not equal to";
        tr.rollback();
        return false;
    }

    tr.commit();
    return true;
}
bool Operation::new_account_oper(QString table, const int o_id, account_summ &acc, int direction, int agent)
{
    QString str;
    QSqlQuery q;

    qDebug() << acc.balance().toDouble();
    qDebug() << acc.account().Id();
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

/*
bool Operation::change_account_balance(account_summ &acc)
{
    QSqlQuery query;
    MyCurrency summ;
    int flag = 1;

    summ = acc.balance() * flag;

    query.prepare("UPDATE account set balance = balance + :summ WHERE id = :id");
    query.bindValue(":id", acc.account().Id());
    query.bindValue(":summ", summ.toDouble());
    if (!query.exec()) {
        return false;
    }

    return true;
}
*/

bool Operation::insert() {
    QSqlQuery q;
    int oper_id;
    QList<account_summ>::iterator i;
    Transaction tr;
    MyCurrency mfrom = 0, mto = 0;
    Database *db = new Database;

    tr.begin();

    if ((oper_id = new_operation()) == 0) {
        tr.rollback();
        return false;
    }
    for (i = from.begin(); i != from.end(); i++) {
        account_summ d = *i;
        Account data;
        data.read(d.account().Id());
        if (data.Top() == Account_Type::debet && agent)
            data.setAgent(agent);
        if (new_account_oper("account_oper", oper_id, d, Direction::from, data.Agent()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::active ||
                data.Top() == Account_Type::credit)
            d.set_balance(-d.balance().toDouble()); // сменить знак
        if (data.change_balance(d.balance()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::debet) {
            db->add_budget(d);
        }
        mfrom += abs(d.balance().toDouble());
    }
    for (i = to.begin(); i != to.end(); i++) {
        account_summ d = *i;
        Account data;
        data.read(d.account().Id());
        if (data.Top() == Account_Type::credit && agent)
            data.setAgent(agent);
        if (new_account_oper("account_oper", oper_id, d, Direction::to, data.Agent()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::passive ||
                data.Top() == Account_Type::debet ||
                data.Top() == Account_Type::initial)
            d.set_balance(-d.balance().toDouble()); // сменить знак
        if (data.change_balance(d.balance()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::credit) {
            db->add_budget(d);
        }
        mto += abs(d.balance().toDouble());
    }

    if (mfrom != mto) {
        qDebug() << "from not equal to";
        tr.rollback();
        return false;
    }

    tr.commit();
    return true;
}

int Operation::find_oper_by_plan(int plan, int mon, int year)
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

QMap<int,double> Operation::get_plan_account_oper_list(int oper, int type)
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

bool Operation::get_plan_data(int id, QDate oper_date)
{
    QSqlQuery q;
//    Account_Data acc;
    QMap<int,double> list;
    QMap<int,double>::iterator i;
    QDate curr = QDate::currentDate();
//    QDate future = curr.addDays(3);
    MyCurrency summ_from = 0, summ_to = 0;
//    Operation data;

    q.prepare("SELECT id, day, month, year, descr, auto FROM plan_oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "Select Error:" << q.lastError().text();
        return false;
    }
    if (q.next()) {
        id = q.value(0).toInt();
        day = q.value(1).toInt();
        month = q.value(2).toInt();
        year = q.value(3).toInt();
        descr = q.value(4).toString();
        auto_exec = q.value(5).toInt();
        date = oper_date;

        // поиск близких планвых операций учетом перехода через границу месяца
        int diff = 0;
        for (int j = 0; j <= 3; j++) {
            QDate future = curr.addDays(j);
            if (day == future.day()) {
                status = Plan_Status::minimum;
                diff = 1;
                break;
            }
        }
        if (diff == 0)
            diff = day - curr.day();

        QDate dt(curr.year(), curr.month(), day);
        if (date > dt) {
            status = Plan_Status::actual;
        }
        else if (int stat = find_oper_by_plan(id, curr.month(), curr.year())) {
            status = stat;
        }
        else if (diff == 1)
            status = Plan_Status::minimum;
        else if (diff < 0)
            status = Plan_Status::expired;
        else
            status = Plan_Status::actual;

        bool from_top = false;
        list = get_plan_account_oper_list(q.value(0).toInt(), Direction::from);
        for (i = list.begin(); i != list.end(); i++) {
            Account acc;
            acc.read(i.key());
            account_summ d;
            d.set_account(acc);
            d.set_balance(i.value());
            from.append(d);
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
            to.append(d);
            if (acc.Top() == Account_Type::credit)
                summ_to += i.value();
        }
        if (from_top == true && (status == Plan_Status::minimum || status == Plan_Status::expired) && summ_from < summ_to)
            descr += QObject::tr(" [Nedostatochno sredstv]");
    }

    return true;
}

QList<Operation> Operation::get_plan_oper_list(int status)
{
    QList<Operation> list;
    Operation oper;
    QSqlQuery q;

    q.prepare("SELECT id,dt FROM plan_oper ORDER BY day");
    if (!q.exec()) {
        qDebug() << "Select Error:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
        oper.get_plan_data(q.value(0).toInt(), q.value(1).toDate());
        if (status && (oper.Status() == Plan_Status::actual || oper.Status() == Plan_Status::committed || oper.Status() == Plan_Status::cancelled))
            continue;
        list.append(oper);
    }

    return list;
}

int Operation::new_plan_oper()
{
    QSqlQuery q;
    int id = 0;
    QList<account_summ>::iterator i;
    Transaction tr;

    tr.begin();

    q.prepare("INSERT INTO plan_oper(day, month, year, descr, dt, auto) VALUES(:day, :month, :year, :descr, :dt, :auto)");
    q.bindValue(":day", day);
    q.bindValue(":month", month);
    q.bindValue(":year", year);
    q.bindValue(":descr", descr);
    q.bindValue(":dt", date.toString("yyyy-MM-dd"));
    q.bindValue(":auto", auto_exec);
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
        for (i = from.begin(); i != from.end(); i++) {
            account_summ d = *i;
            if (!new_account_oper("plan_oper_acc", id, d, Direction::from)) {
                qDebug() << q.lastError().text();
                tr.rollback();
                return 0;
            }
        }
        for (i = to.begin(); i != to.end(); i++) {
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

bool Operation::read(int id)
{
    QSqlQuery q;
//    Operation data;
    QMap<int,double> list;
    QMap<int,double>::iterator i;

    q.prepare("SELECT id,dt,descr FROM oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec() || !q.next()) {
        return false;
    }

    date =      q.value(2).toDate();
    descr =     q.value(3).toString();

    list = Database().get_account_oper_list(q.value(0).toInt(), Direction::from);
    for (i = list.begin(); i != list.end(); i++) {
        account_summ d;
        Account acc;
        acc.read(i.key());
        d.set_account(acc);
        d.set_balance(i.value());
        from.append(d);
    }

    list = Database().get_account_oper_list(q.value(0).toInt(), Direction::to);
    for (i = list.begin(); i != list.end(); i++) {
        account_summ d;
        Account acc;
        acc.read(i.key());
        d.set_account(acc);
        d.set_balance(i.value());
        to.append(d);
    }

    return true;
}

bool Operation::update_plan_oper()
{
    QSqlQuery q;
    QList<account_summ>::iterator i;
    Transaction tr;

    tr.begin();

    q.prepare("DELETE FROM plan_oper_acc WHERE o_id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        tr.rollback();
        qDebug() << "Error DELETE:" << q.lastError().text();
        return false;
    }

    q.prepare("UPDATE plan_oper SET day=:day, month=:month, year=:year, descr=:descr, auto=:auto WHERE id=:id");
    q.bindValue(":day", day);
    q.bindValue(":month", month);
    q.bindValue(":year", year);
    q.bindValue(":descr", descr);
    q.bindValue(":auto", auto_exec);
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "Error update:" << q.lastError().text();
        tr.rollback();
        return false;
    }

    for (i = from.begin(); i != from.end(); i++) {
        account_summ d = *i;
        if (!new_account_oper("plan_oper_acc", id, d, Direction::from)) {
            qDebug() << q.lastError().text();
            tr.rollback();
            return false;
        }
    }
    for (i = to.begin(); i != to.end(); i++) {
        account_summ d = *i;
        if (!new_account_oper("plan_oper_acc", id, d, Direction::to)) {
            qDebug() << q.lastError().text();
            tr.rollback();
            return false;
        }
    }

    tr.commit();
    return true;
}

/*
Operation Operation::get_operation(int id)
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
*/

bool Operation::del_operation(int id)
{
    QSqlQuery q;
    QMap<int,double> list;
    QMap<int,double>::iterator i;
//    account_summ acc;
    Transaction tr;

    tr.begin();

    list = Database().get_account_oper_list(id,Direction::from);
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
        if (data.change_balance(acc.balance()) == false) {
            tr.rollback();
            return false;
        }
    }

    list = Database().get_account_oper_list(id,Direction::to);
    for (i = list.begin(); i != list.end(); i++) {
        account_summ acc;
        Account data;
        data.read(i.key());
        acc.set_account(data);
        acc.set_balance(-i.value());
        if (data.change_balance(acc.balance()) == false) {
            tr.rollback();
            return false;
        }
    }

    if (Database().del_account_oper(id) == false) {
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
