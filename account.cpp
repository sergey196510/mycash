#include "account.h"

Account::Account() {
    id = 0;
    name.clear();
    type = 0;
    curr = 0;
    hidden = false;
    parent = 0;
    top = 0;
    system = 0;
    agent = 0;
    balance = 0;
    descr.clear();
    dt = QDate::currentDate();
}
bool Account::read(int i)
{
    QSqlQuery q;

    id = i;
    if (id == 0)
        return false;

    q.prepare("SELECT name,type,balance,descr,ccod,hidden,parent,top,system FROM account WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "SELECT Error:" << q.lastError().text();
        return false;
    }
    if (q.next()) {
//            id = i;
        name = q.value(0).toString();
        type = q.value(1).toInt();
        balance = q.value(2).toDouble();
        descr = q.value(3).toString();
        curr = q.value(4).toInt();
        hidden = q.value(5).toInt();
        parent = q.value(6).toInt();
        top = q.value(7).toInt();
        system = q.value(8).toInt();
        return true;
    }
    return false;
}
int Account::insert()
{
    QSqlQuery q;

    q.prepare("INSERT INTO account(name, type, ccod, balance, descr, hidden, parent, top, dt) VALUES(:name, :type, :ccod, :balance, :descr, :hidden, :parent, :top, :dt)");
    q.bindValue(":name",    name);
    q.bindValue(":type",    type);
    q.bindValue(":ccod",    curr);
    q.bindValue(":balance", 0);
    q.bindValue(":descr",   descr);
    q.bindValue(":hidden",  (hidden == false) ? 0 : 1);
    q.bindValue(":parent",  parent);
    q.bindValue(":top",     top);
    q.bindValue(":dt", dt);
    if (!q.exec())
        return 0;

    q.prepare("SELECT MAX(id) FROM account");
    if (!q.exec())
        return 0;
    if (q.next())
        return q.value(0).toInt();
    return 0;
}
bool Account::change_balance(MyCurrency val)
{
    QSqlQuery query;
    MyCurrency summ;
    int flag = 1;

    summ = val * flag;

    query.prepare("UPDATE account set balance = balance + :summ WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":summ", summ.toDouble());
    if (!query.exec()) {
        return false;
    }

    return true;
}
QString Account::get_parent_account(int id)
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

QString Account::fullName()
{
    QString str;
    QSqlQuery q;

    q.prepare("SELECT name,parent FROM account WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return str;
    }
    if (q.next()) {
        QString s = q.value(0).toString();
        QString str;
        if (q.value(1).toInt() > 0) {
            str = get_parent_account(q.value(1).toInt()) + ":";
        }
        return str + s;
    }

    return QVariant().toString();
}

double Account::get_summ()
{
    QSqlQuery q;
    double summ = 0;
    Transaction tr;
    Account acc;

//    if (!var.database_Opened())
//        return summ;

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
        acc.read(q.value(0).toInt());
        summ += acc.get_summ();
    }

    tr.commit();
    return summ;
}
