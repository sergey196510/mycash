#include "listbudget.h"

ListBudgetModel::ListBudgetModel(Database *d, QObject *parent) :
    QAbstractTableModel(parent)
{
    db = d;
    header_data << tr("Month") << tr("Account") << tr("Summ") << tr("Description");
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
//        if (index.column() == 0) {
//            Budget data = list.at(index.row());
//            return data.Id();
//        }
        if (index.column() == 0) {
            Budget data = list.at(index.row());
            return QDate::shortMonthName(data.Month());
        }
        if (index.column() == 1) {
            Budget data = list.at(index.row());
            return accounts_list[data.Account()];
        }
        if (index.column() == 2) {
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

int ListBudgetModel::get_id(int row)
{
    return list.at(row).Id();
}

ListBudget::ListBudget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    db = new Database;

    model = new ListBudgetModel(db);
    connect(this, SIGNAL(change_data()), model, SLOT(update_list()));

    ui.tableView->setModel(model);
//    ui.tableView->hideColumn(0);

    QAction *nb = new QAction(tr("New budget"), this);
    nb->setToolTip(tr("New budget"));
    connect(nb, SIGNAL(triggered()), SLOT(new_budget()));
    acts.append(nb);

    QAction *ub = new QAction(tr("Update budget"), this);
    ub->setToolTip(tr("Update budget"));
    connect(ub, SIGNAL(triggered()), SLOT(update_budget()));
    acts.append(ub);

    QAction *rb = new QAction(tr("Remove budget"), this);
    rb->setToolTip(tr("Remove budget"));
    connect(rb, SIGNAL(triggered()), SLOT(remove_budget()));
    acts.append(rb);

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
    Budget eb;
    if (eb.insert())
        emit change_data();
}

void ListBudget::update_budget()
{
    int id = get_selected_id();

    Budget eb(id);
    if (eb.update())
        emit change_data();
}

void ListBudget::remove_budget()
{
    int id = get_selected_id();

    Budget eb(id);
    if (eb.remove())
        emit change_data();
}

int ListBudget::get_selected_id()
{
    return model->get_id(ui.tableView->currentIndex().row());
}
