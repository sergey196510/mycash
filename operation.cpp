#include "operation.h"

Operation::Operation() {
    id = 0;
//    day = month = year = 0;
    agent = 0;
//    auto_exec = 0;
    date = QDate::currentDate();
    descr.clear();
}

int Operation::new_operation()
{
    QSqlQuery query;

    query.prepare("INSERT INTO oper(dt, descr) VALUES(:dt, :descr)");
    query.bindValue(":dt", date);
    query.bindValue(":descr", descr);
//    qDebug() << query.executedQuery();
    if (!query.exec())
        return 0;

    query.prepare("SELECT MAX(id) FROM oper");
    if (!query.exec())
        return 0;
    if (query.next())
        return query.value(0).toInt();
    return 0;
}

int PlanOperation::new_operation()
{
    QSqlQuery q;
    Transaction tr;
    int id;

    tr.begin();

    q.prepare("INSERT INTO plan_oper(day, month, descr, dt, auto) VALUES(:day, :month, :descr, :dt, :auto)");
    q.bindValue(":day", day);
    q.bindValue(":month", month);
    q.bindValue(":descr", descr);
    q.bindValue(":dt", date.toString("yyyy-MM-dd"));
    q.bindValue(":auto", auto_exec);
    if (!q.exec()) {
        qDebug() << "Error Insert:" << q.lastError().text();
        tr.rollback();
        return 0;
    }

    q.prepare("SELECT MAX(id) FROM plan_oper");
    if (!q.exec() || !q.next()) {
        qDebug() << q.lastError().text();
        tr.rollback();
        return 0;
    }
    id = q.value(0).toInt();

    tr.commit();
    return id;
}

bool Operation::new_account_oper(QString table, const int o_id, account_summ &acc, int direction, int agent)
{
    QString str;
    QSqlQuery q;

//    qDebug() << acc.balance().toDouble();
//    qDebug() << acc.account().Id();
    str = QString("INSERT INTO %1(a_id, o_id, summ, direction, agent) VALUES(%2, %3, %4, %5, %6)")
            .arg(table)
            .arg(acc.account().Id())
            .arg(o_id)
            .arg(acc.balance().toDouble())
            .arg(direction)
            .arg(agent);
//    qDebug() << str;
    if (!q.exec(str)) {
        qDebug() << q.lastError().text();
        return false;
    }

    return true;
}

bool Operation::insert() {
    QSqlQuery q;
    int oper_id;
    QList<account_summ>::iterator i;
    Transaction tr;
    MyCurrency mfrom = 0, mto = 0;
//    Database *db = new Database;

    tr.begin();

    if (from.size() == 0 || to.size() == 0) {
        qDebug() << "New opertation don't has accounts";
        tr.rollback();
        return false;
    }

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
//        if (data.Top() == Account_Type::active ||
//                data.Top() == Account_Type::credit)
//            d.set_balance(-d.balance().toDouble()); // сменить знак
//        if (data.change_balance(d.balance()) == false) {
        if (data.credit(d.balance()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::debet) {
            Budget().add_budget(d);
        }
        mfrom += std::abs(d.balance().toDouble());
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
//        if (data.Top() == Account_Type::passive ||
//                data.Top() == Account_Type::debet ||
//                data.Top() == Account_Type::initial)
//            d.set_balance(-d.balance().toDouble()); // сменить знак
//       if (data.change_balance(d.balance()) == false) {
        if (data.debet(d.balance()) == false) {
            tr.rollback();
            return false;
        }
        if (data.Top() == Account_Type::credit) {
            Budget().add_budget(d);
        }
        mto += std::abs(d.balance().toDouble());
    }

    if (mfrom != mto) {
        qDebug() << "from not equal to";
        tr.rollback();
        return false;
    }

    tr.commit();
    return true;
}

bool PlanOperation::ins_account_oper(QList<account_summ> lst, QString str, int id, int direction)
{
    QList<account_summ>::iterator i;

    for (i = lst.begin(); i != lst.end(); i++) {
        account_summ d = *i;
        if (!new_account_oper(str, id, d, direction)) {
//            qDebug() << q.lastError().text();
//            tr.rollback();
            return false;
        }
    }

    return true;
}

int PlanOperation::insert()
{
    QSqlQuery q;
    int oper_id = 0;
    QList<account_summ> lst;
    QList<account_summ>::iterator i;
    Transaction tr;

    tr.begin();

    oper_id = new_operation();
    if (!oper_id) {
        qDebug() << "Null operation id";
        tr.rollback();
        return 0;
    }

    if (ins_account_oper(from, "plan_oper_acc", oper_id, Direction::from) == false ||
            ins_account_oper(to, "plan_oper_acc", oper_id, Direction::to) == false) {
        tr.rollback();
        return 0;
    }

/*
    for (i = from.begin(); i != from.end(); i++) {
        account_summ d = *i;
        if (!new_account_oper("plan_oper_acc", oper_id, d, Direction::from)) {
            qDebug() << q.lastError().text();
            tr.rollback();
            return 0;
        }
    }
    for (i = to.begin(); i != to.end(); i++) {
        account_summ d = *i;
        if (!new_account_oper("plan_oper_acc", oper_id, d, Direction::to)) {
            qDebug() << q.lastError().text();
            tr.rollback();
            return 0;
        }
    }
*/

    tr.commit();
    return oper_id;
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

/*
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
*/

QMap<int,double> Operation::get_account_oper_list(int oper, int type, QString table)
{
    QMap<int,double> list;
    QSqlQuery q;

    QString query = QString("SELECT a_id,summ FROM %1 WHERE o_id = %2 AND direction = %3")
            .arg(table)
            .arg(oper)
            .arg(type);
    if (!q.exec(query)) {
        qDebug() << q.lastError().text();
        return list;
    }
    while (q.next()) {
        list[q.value(0).toInt()] = q.value(1).toDouble();
    }

    return list;
}

vector<PlanOperation> PlanOperation::read_list(int status)
{
    vector<PlanOperation> list;
    QSqlQuery q;

    q.prepare("SELECT id,dt FROM plan_oper ORDER BY day,month");
    if (!q.exec()) {
        qDebug() << "Select Error:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
        PlanOperation oper;
        if (!oper.read(q.value(0).toInt()))
            continue;
        if (status && (oper.Status() == Plan_Status::actual || oper.Status() == Plan_Status::committed || oper.Status() == Plan_Status::cancelled))
            continue;
        list.push_back(oper);
    }

    return list;
}

bool Operation::read(int _i)
{
    QSqlQuery q;
//    Operation data;
    QMap<int,double> list;
    QMap<int,double>::iterator i;

    id = _i;
    if (id == 0)
        return false;

    q.prepare("SELECT dt,descr FROM oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec() || !q.next()) {
        return false;
    }

    date =  q.value(0).toDate();
    descr = q.value(1).toString();

    list = get_account_oper_list(id, Direction::from, "account_oper");
    for (i = list.begin(); i != list.end(); i++) {
        account_summ d;
        Account acc;
        acc.read(i.key());
        d.set_account(acc);
        d.set_balance(i.value());
        append_from(d);
    }

    list = get_account_oper_list(id, Direction::to, "account_oper");
    for (i = list.begin(); i != list.end(); i++) {
        account_summ d;
        Account acc;
        acc.read(i.key());
        d.set_account(acc);
        d.set_balance(i.value());
        append_to(d);
    }

    return true;
}

void PlanOperation::calcStatus()
{
    QDate curr = QDate::currentDate();

    // отработанная операция
    if (int stat = find_oper_by_plan(id, curr.month(), curr.year())) {
        status = stat;
        return;
    }

    // поиск ближайших операций в будущем
    for (int j = 0; j <= 3; j++) {
        QDate future = curr.addDays(j);
        if (day == future.day()) {
            status = Plan_Status::minimum;
            return;
        }
    }

    int diff = day - curr.day();
    QDate dt(curr.year(), curr.month(), day);
    if (date > dt) {
        status = Plan_Status::actual;
    }
    else if (diff < 0)
        status = Plan_Status::expired;
    else
        status = Plan_Status::actual;

}

bool PlanOperation::read(int _i)
{
    QSqlQuery q;
//    Account_Data acc;
    QMap<int,double> list;
    QMap<int,double>::iterator i;
//    QDate future = curr.addDays(3);
    MyCurrency summ_from = 0, summ_to = 0;
//    Operation data;

    id = _i;
    if (id == 0)
        return false;

    q.prepare("SELECT day,month,descr,auto,dt FROM plan_oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec() || !q.next()) {
        qDebug() << "Select Error:" << q.lastError().text();
        return false;
    }

    //        id = q.value(0).toInt();
    day = q.value(0).toInt();
    month = q.value(1).toInt();
    //        year = q.value(3).toInt();
    descr = q.value(2).toString();
    auto_exec = q.value(3).toInt();
    date = q.value(4).toDate();

    calcStatus();

    // поиск близких планвых операций учетом перехода через границу месяца
    bool from_top = false;
    list = get_account_oper_list(id, Direction::from, "plan_oper_acc");
    if (list.size() < 1) {
        qDebug() << id << "Null from account";
        return false;
    }
    for (i = list.begin(); i != list.end(); i++) {
        account_summ d;
        Account acc;
        acc.read(i.key());
        d.set_account(acc);
        d.set_balance(i.value());
        append_from(d);
        if (acc.Top() == Account_Type::active) {
//            qDebug() << acc.fullName();
            from_top = true;
            summ_from += acc.Balance();
        }
    }

    list = get_account_oper_list(id, Direction::to, "plan_oper_acc");
    if (list.size() < 1) {
        qDebug() << id << "Null to account";
        return false;
    }
    for (i = list.begin(); i != list.end(); i++) {
        account_summ d;
        Account acc;
        acc.read(i.key());
        d.set_account(acc);
        d.set_balance(i.value());
        append_to(d);
//        if (acc.Top() == Account_Type::credit)
//            qDebug() << acc.fullName();
            summ_to += i.value();
    }
//    qDebug() << this->day << summ_from.toDouble() << summ_to.toDouble();
    if (from_top == true && (status == Plan_Status::minimum || status == Plan_Status::expired) && summ_from < summ_to)
        descr += QObject::tr(" [Nedostatochno sredstv]");

    return true;
}

bool PlanOperation::update_plan_oper()
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
//    q.bindValue(":year", year);
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

    list = get_account_oper_list(id,Direction::from, "account_oper");
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
        if (data.change_balance2(acc.balance()) == false) {
            tr.rollback();
            return false;
        }
    }

    list = get_account_oper_list(id,Direction::to, "account_oper");
    for (i = list.begin(); i != list.end(); i++) {
        account_summ acc;
        Account data;
        data.read(i.key());
        acc.set_account(data);
        acc.set_balance(-i.value());
        if (data.change_balance2(acc.balance()) == false) {
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

void Operation::append_from(account_summ &as)
{
    from.append(as);
}

void Operation::append_to(account_summ &as)
{
    to.append(as);
}

void Operation::update_descr()
{
    QSqlQuery q;

    if (id == 0)
        return;

    q.prepare("UPDATE oper SET descr = :descr WHERE id = :id");
    q.bindValue(":descr", descr);
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError();
    }
}
