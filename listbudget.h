#ifndef LISTBUDGET_H
#define LISTBUDGET_H

#include "ui_listbudget.h"
#include "QtSql"

class ListBudgetModel : public QSqlQueryModel
{
    Q_OBJECT
};

class ListBudget : public QWidget
{
    Q_OBJECT

public:
    explicit ListBudget(QWidget *parent = 0);

private:
    Ui::ListBudget ui;
};

#endif // LISTBUDGET_H
