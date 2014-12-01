#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QFont>
#include <QtSql>
#include <widgets/mycurrency.h>
#include "account.h"

extern QLocale *default_locale;

class Direction {
public:
    static const int from;
    static const int to;
};

class Account_Type {
public:
    static const int active;
    static const int passive;
    static const int debet;
    static const int credit;
    static const int initial;
};

struct Font {
    QString name;
    int size;
    bool bold;
};

class Globals {
    static int account;
    static int currency;
    static int correct_account;
    static int initial_account;
//    static double kurs;
    static int precision;
//    static QString symbol;
    static QString list_font;
    static QString version;
    static bool database_opened;
    static Font font;
    static int proxy_enable;
    static QString proxy_server;
    static int proxy_port;
    static QString proxy_user;
    static QString proxy_password;
public:
    int Account() { return account; }
    void setAccount(int i) { account = i; }
    int Currency() { return currency; }
    void setCurrency(int i) { currency = i; }
    int CorrectAccount() { return correct_account; }
    void setCorrectAccount(int i) { correct_account = i; }
    int InitialAccount() { return initial_account; }
    void setInitialAccount(int i) { initial_account = i; }
//    double Kurs() { return kurs; }
//    void setKurs(double i) { kurs = i; }
    int Precision() { return precision; }
    void setPrecision(int i) { precision = i; }
//    QString Symbol() { return symbol; }
//    void setSymbol(QString s) { symbol = s; }
    QString ListFont() { return list_font; }
    void setListFont(QString s) { list_font = s; }
    QString Version() { return version; }
    void setVersion(QString v) { version = v; }
    int ProxyEnable() { return proxy_enable; }
    void setProxyEnable(int i) { proxy_enable = i;}
    QString ProxyServer() { return proxy_server; }
    int ProxyPort() { return proxy_port; }
    QString ProxyUser() { return proxy_user; }
    QString ProxyPassword() { return proxy_password; }
    void setProxyServer(QString i) { proxy_server=i; }
    void setProxyPort(int i) { proxy_port=i; }
    void setProxyUser(QString i) { proxy_user=i; }
    void setProxyPassword(QString i) { proxy_password=i; }
    bool database_Opened() { return database_opened; }
    void database_Open() { database_opened = true; }
    void database_Close() { database_opened = false; }
};

class Plan_Status {
public:
    static const int actual;
    static const int committed;
    static const int minimum;
    static const int expired;
    static const int cancelled;
};


class account_summ {
    Account acc;
    MyCurrency s;
public:
    account_summ() {
//        acc = 0;
//        s = 0;
    };
    Account account() const { return acc; }
//    double balance() const { return s; }
    MyCurrency balance() const { return s; }
    void set_account(Account val) { acc = val; }
    void set_balance(MyCurrency val) { s = val; }
};

class Agent {
    int id;
    QString name;
    QString city;
    QString address;
    QString phone;
    QString contact;
public:
    Agent() {
        id = 0;
        name.clear();
        city.clear();
        address.clear();
        phone.clear();
        contact.clear();
    }
    int insert() {
        QSqlQuery q;

        q.prepare("INSERT INTO agent(name, city, address, phone, contact) VALUES(:name, :city, :address, :phone, :contact)");
        q.bindValue(":name", name);
        q.bindValue(":city", city);
        q.bindValue(":address", address);
        q.bindValue(":phone", phone);
        q.bindValue(":contact", contact);
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
    bool read(int i) {
        QSqlQuery q;

        id = i;
        q.prepare("SELECT name,city,address,phone,contact FROM agent WHERE id = :id");
        q.bindValue(":id", id);
        if (q.exec() && q.next()) {
            name = q.value(0).toString();
            city = q.value(1).toString();
            address = q.value(2).toString();
            phone = q.value(3).toString();
            contact = q.value(4).toString();
            return true;
        }
        return false;
    }
    bool update() {
        QSqlQuery q;

        if (id == 0)
            return false;

        q.prepare("UPDATE agent SET name = :name, city = :city, address = :address, phone = :phone, contact = :contact WHERE id = :id");
        q.bindValue(":name", name);
        q.bindValue(":city", city);
        q.bindValue(":address", address);
        q.bindValue(":phone", phone);
        q.bindValue(":contact", contact);
        q.bindValue(":id", id);
        q.exec();
        if (!q.exec()) {
            qDebug() << q.lastError();
            return false;
        }
        return true;
    }

    int Id() { return id; }
    QString Name() { return name; }
    QString City() { return city; }
    QString Address() { return address; }
    QString Phone() { return phone; }
    QString Contact() { return contact; }
    void set_Id(int i) { id = i; }
    void set_Name(QString s) { name = s; }
    void set_City(QString s) { city = s; }
    void set_Address(QString s) { address = s; }
    void set_Phone(QString s) { phone = s; }
    void set_Contact(QString s) { contact = s; }
};

#endif
