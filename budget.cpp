#include "budget.h"

Budget::Budget(int i)
{
    id = 0;
    mon = 0;
    account = 0;
    summ = 0;

    if (i != 0)
        read(i);
}

void Budget::set_Value(int i, int m, int a, double s)
{
    id = i;
    mon = m;
    account = a;
    summ = s;
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
        account = q.value(1).toInt();
        summ = q.value(2).toDouble();
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
    eb.setAccount(account);
    eb.setSumm(summ);
    if (eb.exec() != QDialog::Accepted)
        return false;
    return true;
}

bool Budget::remove()
{
    QSqlQuery q;

    if (id == 0)
        return false;

    if (read(id) == false)
        return false;

    QMessageBox::question(0,
                          "Remove budget",
                          QString("You want remove budget:\nMonth: %1, Account: %2, Summ: %3")
                          .arg(mon)
                          .arg(account)
                          .arg(summ.toDouble()));

    return false;
}
