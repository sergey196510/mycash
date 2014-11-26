#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QFont>
#include <QtSql>
#include <widgets/mycurrency.h>

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

class Account {
    int id;
    QString name;
    int type;
    int curr;
    MyCurrency balance;
    bool hidden;
    int parent;
    int top;
    int system;
    int agent;
    QString descr;
    QDate dt;
public:
    Account() {
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
    bool read(int id)
    {
        QSqlQuery q;

        if (id == 0)
            return false;

        q.prepare("SELECT name,type,balance,descr,ccod,hidden,parent,top,system FROM account WHERE id = :id");
        q.bindValue(":id", id);
        if (!q.exec()) {
            qDebug() << "SELECT Error:" << q.lastError().text();
            return false;
        }
        if (q.next()) {
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
    int insert()
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
    void setBalance(MyCurrency bal) { balance = bal; }
    void setAgent(int a) { agent = a; }
    void setCurr(int c) { curr = c; }
    void setName(QString n) { name = n; }
    void setDescr(QString n) { descr = n; }
    void setType(int t) { type = t; }
    void setParent(int p) { parent = p; }
    void setHidden(bool h) { hidden = h; }
    void setDate(QDate d) { dt = d; }
    void setTop(int t) { top = t; }
    int Id() { return id; }
    int Agent() { return agent; }
    int Top() { return top; }
    MyCurrency Balance() { return balance; }
    int Type() { return type; }
    QString Name() { return name; }
    QString Descr() { return descr; }
    int Curr() { return curr; }
    int Parent() { return parent; }
    bool Hidden() { return hidden; }
    QDate Date() { return dt; }
    int System() { return system; }
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

struct Operation_Data {
    int id;
    int day,month,year;
    int agent;
    double kurs;
    QList<account_summ> from;
    QList<account_summ> to;
//    int plan_id;
    QDate date;
    QString descr;
    int status;
    int auto_exec;
    Operation_Data() {
        id = day = month = year = 0;
        agent = 0;
        auto_exec = 0;
        date = QDate::currentDate();
        descr.clear();
    }
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
    bool read(int) {
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
