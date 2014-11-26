#include "currency.h"

Currency::Currency(int i)
{
    id = i;
    name.clear();
    icod = 0;
    scod.clear();
    kurs = 0;
    nominal = 1;

    if (id != 0) {
        QSqlQuery q;
        q.prepare("SELECT name,icod,scod,kurs,nominal FROM currency WHERE id=:id");
        q.bindValue(":id", id);
        if (!q.exec()) {
            qDebug() << q.lastError();
        }
        if (q.next()) {
            name = q.value(0).toString();
            icod = q.value(1).toInt();
            scod = q.value(2).toString();
            kurs = q.value(3).toDouble();
            nominal = q.value(4).toInt();
        }
    }
}

bool Currency::load(QString sc) {
    QSqlQuery q;
    q.prepare("SELECT name,icod,id,kurs,nominal FROM currency WHERE scod=:scod");
    q.bindValue(":scod", sc);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }
    if (q.next()) {
        name = q.value(0).toString();
        icod = q.value(1).toInt();
        scod = sc;
        id = q.value(2).toInt();
        kurs = q.value(3).toDouble();
        nominal = q.value(4).toInt();
        return true;
    }
    return false;
}

bool Currency::update() {
    QSqlQuery q;

    q.prepare("UPDATE currency SET name=:name, icod=:icod, scod=:scod, nominal=:nominal, kurs=:kurs WHERE id=:id");
    q.bindValue(":name", name);
    q.bindValue(":icod", icod);
    q.bindValue(":scod", scod);
    q.bindValue(":nominal", nominal);
    q.bindValue(":kurs", kurs);
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }
    return true;
}
