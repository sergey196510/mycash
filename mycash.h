#ifndef MYCASH_H
#define MYCASH_H

#include <QMainWindow>
#include "widgets/currencycombobox.h"
#include "global.h"
#include "database.h"

namespace Ui {
class MyCash;
}

class MyCash : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MyCash(QWidget *parent = 0);
    ~MyCash();

signals:
    void call_mark();

private:
    QSqlDatabase base;
    Globals var;
    Database *db;
//    bool opened;
    QString dbname;
    Ui::MyCash *ui;
    CurrencyComboBox *curr;
    QMap<int,QString> currs;
    void readsettings();
    void writesettings();
    void setconnects();
    void opendb(QString);

private slots:
    void create();
    void open();
    void closeDatabase();
    void settings();
    void aboutProgram();
    void mark_Object();
    void list_home();
    void list_accounts();
    void list_currency();
    void list_operations();
    void list_agents();
    void list_plan_oper();
    void report1();
    void report2();
    void update_curr();
//    void test();

signals:
    void update_currency();

};

#endif // MYCASH_H
