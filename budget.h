#ifndef BUDGET_H
#define BUDGET_H

#include <QDialog>
#include <QComboBox>
#include <QtSql>
#include <QMessageBox>
#include "widgets/mycurrency.h"
#include "editbudget.h"

class Budget
{
    int id;
    int mon;
    int account;
    MyCurrency summ;
public:
    Budget(int i = 0);
    int Id() const { return id; }
    int Month() { return mon; }
    int Account() { return account; }
    MyCurrency Summ() { return summ; }
    void set_Value(int i, int m, int a, double s);
    bool read(int id);
    bool insert();
    bool update();
    bool remove();
};

#endif // BUDGET_H
