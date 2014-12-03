#ifndef BUDGET_H
#define BUDGET_H

#include <QDialog>
#include <QComboBox>
#include <QtSql>
#include <QMessageBox>
#include "widgets/mycurrency.h"
#include "editbudget.h"
#include "account.h"

class Budget
{
    int id;
    int mon;
    Account acc;
    MyCurrency summ;
public:
    Budget(int i = 0);
    int Id() const { return id; }
    int Month() { return mon; }
    Account account() { return acc; }
    MyCurrency Summ() { return summ; }
    void set_Value(int i, int m, int a, double s);
    bool read(int id);
    bool insert();
    bool update();
    bool remove();
};

#endif // BUDGET_H
