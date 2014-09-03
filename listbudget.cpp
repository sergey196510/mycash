#include "listbudget.h"

ListBudgetModel::ListBudgetModel(Database *d, QObject *parent) :
    QAbstractTableModel(parent)
{
    db = d;
    header_data << "" << tr("Month") << tr("Account") << tr("Summ") << tr("Description");
    list = db->read_budget_list(0);
    accounts_list = db->get_accounts_list();
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
            Budget data = list.at(index.row());
            return data.Id();
        }
        if (index.column() == 1) {
            Budget data = list.at(index.row());
            return QDate::shortMonthName(data.Month());
        }
        if (index.column() == 2) {
            Budget data = list.at(index.row());
            return accounts_list[data.Account()];
        }
        if (index.column() == 3) {
            Budget data = list.at(index.row());
            return data.Summ().toDouble();
        }
    }

    return QVariant();
}

void ListBudgetModel::update_list()
{
    beginResetModel();
    list.clear();
    list = db->read_budget_list(0);
    endResetModel();
}

ListBudget::ListBudget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    db = new Database;

    model = new ListBudgetModel(db);
    connect(this, SIGNAL(change_data()), model, SLOT(update_list()));
    ui.tableView->setModel(model);

    ui.tableView->hideColumn(0);
    ui.tableView->setAlternatingRowColors(true);
    ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
//    ui.tableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->horizontalHeader()->setStretchLastSection(true);

    QAction *nb = new QAction(tr("New budget"), this);
    nb->setToolTip(tr("New budget line"));
    connect(nb, SIGNAL(triggered()), SLOT(new_budget()));

    acts.append(nb);
    ui.tableView->addActions(acts);

    ui.tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

ListBudget::~ListBudget()
{
    delete model;
    delete db;
}

void ListBudget::new_budget()
{
    EditBudget eb;
    Budget data;

    if (eb.exec() != QDialog::Accepted)
        return;

    data = eb.data();
    if (data.insert())
        emit change_data();
}
