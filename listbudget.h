#ifndef LISTBUDGET_H
#define LISTBUDGET_H

#include "ui_listbudget.h"
#include "QtSql"
#include "global.h"

class ListBudgetModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ListBudgetModel(QObject *parent = 0);

private:
    QList<Budget_Data> list;
    QStringList header_data;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QList<Budget_Data> read_list();
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
