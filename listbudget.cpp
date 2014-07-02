#include "listbudget.h"

ListBudgetModel::ListBudgetModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

int ListBudgetModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 5;
}

int ListBudgetModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 5;
}

QVariant ListBudgetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return QVariant();
}

ListBudget::ListBudget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    model = new ListBudgetModel;
    ui.tableView->setModel(model);
}
