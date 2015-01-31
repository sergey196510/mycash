#include "budget.h"

Budget::Budget(int i)
{
    id = 0;
    mon = 0;
//    accunt = 0;
    plan = 0;

    if (i != 0)
        read(i);
}

void Budget::set_Value(int i, int m, int a, double s)
{
    id = i;
    mon = m;
    acc.read(a);
    plan = s;
}

bool Budget::read(int i)
{
    QSqlQuery q;
    if (i == 0)
        return false;

    q.prepare("SELECT mon,a_id,summ FROM budget_plan WHERE id=:id");
    q.bindValue(":id", i);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }
    if (q.next()) {
        id = i;
        mon = q.value(0).toInt();
        acc.read(q.value(1).toInt());
        plan = q.value(2).toDouble();
        return true;
    }

    return false;
}

bool Budget::insert()
{
    QSqlQuery q;
    EditBudget eb;

    if (eb.exec() != QDialog::Accepted)
        return false;

    q.prepare("INSERT INTO budget_plan(mon,a_id,summ) VALUES(:mon,:acc,:summ)");
    q.bindValue(":mon", eb.Month());
    q.bindValue(":acc", eb.Account());
    q.bindValue(":summ", eb.Summ().toDouble());
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }

    return true;
}

bool Budget::update()
{
    QSqlQuery q;
    EditBudget eb;

    if (id == 0)
        return false;

    if (read(id) == false)
        return false;

    eb.setMonth(mon);
    eb.setAccount(acc.Id());
    eb.setSumm(plan);
    if (eb.exec() != QDialog::Accepted)
        return false;

    q.prepare("UPDATE budget_plan SET mon=:mon, a_id=:acc, summ=:summ WHERE id=:id");
    q.bindValue(":mon", eb.Month());
    q.bindValue(":acc", eb.Account());
    q.bindValue(":summ", eb.Summ().toDouble());
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }

    return true;
}

bool Budget::remove()
{
    QSqlQuery q;

    if (id == 0)
        return false;

    if (read(id) == false)
        return false;

    int n = QMessageBox::question(0,
                          QObject::tr("Remove budget"),
                          QObject::tr("You want remove budget:\n\nMonth: %1\nAccount: %2\nSumm: %3")
                          .arg(mon)
                          .arg(acc.fullName())
                          .arg(plan.toDouble()));

    if (n == QMessageBox::Accepted) {

    }

    return false;
}

QList<Budget> Budget::read_list(int mon)
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

bool Budget::add_budget(account_summ &d)
{
    QSqlQuery q;
    QList<Budget> list = read_list(QDate::currentDate().month());
    QList<Budget>::iterator i;

    for (i = list.begin(); i != list.end(); i++) {
        Budget data = *i;
        if (find_parent(data.account().Id(), d.account().Id())) {
            int id = data.Id();
            qDebug() << id;
            update_fact(id, d.balance());
        }
    }

    return true;
}

bool Budget::find_parent(int budget, int acc)
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
            return find_parent(budget,q.value(1).toInt());
//        else
//            return true;
    }

    return false;
}

bool Budget::update_fact(int _id, MyCurrency bal)
{
    QSqlQuery q;
    int id;

    if ((id = find_fact(_id)) == 0)
        if ((id = insert_fact(_id)) == 0)
            return false;

    q.prepare("UPDATE budget_fact SET summ = summ + :summ WHERE id = :id");
    q.bindValue(":summ", bal.toDouble());
    q.bindValue(":id", id);
    if (!q.exec())
        return false;

    return true;
}

int Budget::find_fact(int id)
{
    QSqlQuery q;
    QDate curr = QDate::currentDate();

    q.prepare("SELECT id FROM budget_fact WHERE a_id = :id AND year = :year AND mon = :mon");
    q.bindValue(":id", id);
    q.bindValue(":year", curr.year());
    q.bindValue(":mon", curr.month());
    if (!q.exec() || !q.next())
        return 0;

    return q.value(0).toInt();
}

int Budget::insert_fact(int id)
{
    QSqlQuery q;
    QDate curr = QDate::currentDate();

    q.prepare("INSERT INTO budget_fact(a_id, year, mon, summ) VALUES(:id, :year, :mon, 0)");
    q.bindValue(":id", id);
    q.bindValue(":year", curr.year());
    q.bindValue(":mon", curr.month());
    if (!q.exec())
        return 0;

    q.prepare("SELECT MAX(id) FROM budget_fact");
    if (!q.exec() || !q.next())
        return 0;

    return q.value(0).toInt();
}
