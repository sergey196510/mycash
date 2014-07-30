#include "listbudget.h"

ListBudgetModel::ListBudgetModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    header_data << "" << tr("Month") << tr("Account") << tr("Summ") << tr("Description");
    list = read_list();
}

int ListBudgetModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return list.size();
}

int ListBudgetModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return header_data.size();
}

QVariant ListBudgetModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

QVariant ListBudgetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            Budget_Data data = list.at(index.row());
            return data.id;
        }
        if (index.column() == 1) {
            Budget_Data data = list.at(index.row());
            return data.mon;
        }
        if (index.column() == 2) {
            Budget_Data data = list.at(index.row());
            return data.account;
        }
        if (index.column() == 3) {
            Budget_Data data = list.at(index.row());
            return data.summ.value();
        }
    }

    return QVariant();
}

QList<Budget_Data> ListBudgetModel::read_list()
{
    QList<Budget_Data> list;
    Budget_Data data;
    QSqlQuery q;

    q.prepare("SELECT id,mon,a_id,summ FROM budget_plan ORDER BY mon,a_id");
    if (!q.exec()) {
        qDebug() << q.lastError();
        return list;
    }
    while (q.next()) {
        data.id = q.value(0).toInt();
        data.mon = q.value(1).toInt();
        data.account = q.value(2).toInt();
        data.summ = q.value(3).toDouble();
        list.append(data);
    }

    return list;
}

ListBudget::ListBudget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    model = new ListBudgetModel;
    ui.tableView->setModel(model);

    ui.tableView->setAlternatingRowColors(true);
    ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->horizontalHeader()->setStretchLastSection(true);
}
