#ifndef LISTBUDGET_H
#define LISTBUDGET_H

#include "ui_listbudget.h"
#include "QtSql"

class ListBudgetModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ListBudgetModel(QObject *parent = 0);

private:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

class ListBudget : public QWidget
{
    Q_OBJECT

public:
    explicit ListBudget(QWidget *parent = 0);

private:
    Ui::ListBudget ui;
    ListBudgetModel *model;
};

#endif // LISTBUDGET_H
