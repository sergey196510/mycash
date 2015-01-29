#ifndef BUDGET_H
#define BUDGET_H

#include <QDialog>
#include <QComboBox>
#include <QtSql>
#include <QMessageBox>
#include "widgets/mycurrency.h"
#include "editbudget.h"
#include "account.h"
#include "accountsumm.h"

class Budget
{
    int id;
    int mon;
    Account acc;
    MyCurrency plan;
    MyCurrency fact;
public:
    Budget(int i = 0);
    int Id() const { return id; }
    int Month() { return mon; }
    Account account() { return acc; }
    MyCurrency Plan() { return plan; }
    QList<Budget> read_list(int mon = 0);
    bool add_budget(account_summ &d);
    bool find_parent(int, int);
    void set_Value(int i, int m, int a, double s);
    bool read(int id);
    bool insert();
    bool update();
    bool remove();
    bool update_fact(int id, MyCurrency bal);
    int find_fact(int id);
    int insert_fact(int id);
};

#endif // BUDGET_H
