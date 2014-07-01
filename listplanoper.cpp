#include "listplanoper.h"
#include "ui_listplanoper.h"

ListPlanOperModel::ListPlanOperModel(Database *d, QObject *parent) :
    QAbstractTableModel(parent)
{
    db = d;
    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Auto") << tr("Description");
    list = read_list();
    acc_list = db->get_accounts_list();
    var = new Globals;
}

ListPlanOperModel::~ListPlanOperModel()
{
    delete var;
}

QList<operation_data> ListPlanOperModel::read_list()
{
    QList<operation_data> list = db->get_plan_oper_list();
    return list;
}

QVariant ListPlanOperModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 0) {
            operation_data data = list.at(index.row());
            return data.id;
        }
        else if (index.column() == 1) {
            operation_data data = list.at(index.row());
            return data.day;
        }
        else if (index.column() == 2) {
            operation_data data = list.at(index.row());
            if (data.month)
                return data.month;
            else
                return "";
        }
        else if (index.column() == 3) {
            operation_data data = list.at(index.row());
            if (data.year)
                return data.year;
            else
                return "";
        }
        else if (index.column() == 4) {
            operation_data data = list.at(index.row());
            return acc_list[data.from.at(0).account()];
        }
        else if (index.column() == 5) {
            operation_data data = list.at(index.row());
            QMap<int,double> oper = db->get_plan_account_oper_list(data.id,2);
            QMap<int,double>::iterator i = oper.begin();
            return acc_list[i.key()];
        }
        else if (index.column() == 6) {
            operation_data data = list.at(index.row());
            QMap<int,double> oper = db->get_plan_account_oper_list(data.id,2);
            QMap<int,double>::iterator i = oper.begin();
            return default_locale->toString(i.value()/var->Kurs(),'f',2);
        }
        else if (index.column() == 8) {
            operation_data data = list.at(index.row());
            return data.descr;
        }
        else
            return QVariant();

        case Qt::TextAlignmentRole:
            if (index.column() == 1)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 2)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 3)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 6)
                return int(Qt::AlignRight | Qt::AlignVCenter);

    }

    return QVariant();
}

QVariant ListPlanOperModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

int ListPlanOperModel::rowCount(const QModelIndex &parent) const
{
    return list.count();
}

int ListPlanOperModel::columnCount(const QModelIndex &parent) const
{
    return header_data.count();
}

void ListPlanOperModel::change_data()
{
    list.clear();
    list = db->get_plan_oper_list();
    reset();
}

ListPlanOper::ListPlanOper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListPlanOper)
{
    QList<QAction*>::iterator i;

    ui->setupUi(this);

    db = new Database;

    model = new ListPlanOperModel(db);
    connect(this, SIGNAL(data_changed()), model, SLOT(change_data()));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);

    tran = new QAction(tr("New plan operation"), this);
    comm = new QAction(tr("Commit this operation"), this);
    delo = new QAction(tr("Delete selected operation"), this);
    comm->setEnabled(false);
    delo->setEnabled(false);

    acts.append(tran);
    acts.append(comm);
    acts.append(delo);

    ui->tableView->addActions(acts);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    connect(tran, SIGNAL(triggered()), SLOT(new_oper()));
    connect(comm, SIGNAL(triggered()), SLOT(commit_oper()));
    connect(delo, SIGNAL(triggered()), SLOT(del_oper()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(check_selected()));
}

ListPlanOper::~ListPlanOper()
{
    delete ui;
    delete db;
}

void ListPlanOper::new_oper()
{
    EditOperation *po = new EditOperation(2, this);

    if (po->exec() == QDialog::Accepted) {
        operation_data data = po->data();
        if (db->new_plan_oper(data)) {
        }
        emit data_changed();
    }

    delete po;
}

int ListPlanOper::get_selected_id()
{
    QModelIndexList list;

    list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return 0;
    }

     return list.at(0).data(Qt::DisplayRole).toInt();
}

void ListPlanOper::commit_oper()
{
    operation_data pod;
    operation_data od;
    EditOperation *eo = new EditOperation(1, this);
    int id = get_selected_id();

    if (id == 0)
        return;

    pod = db->get_plan_oper_data(id);

    eo->setdata(pod);
    if (eo->exec() == QDialog::Rejected)
        return;

    pod = eo->data();
    db->save_operation(pod);
    db->new_mon_oper(id);

    emit data_changed();
}

void ListPlanOper::del_oper()
{
    QSqlQuery q;
    int id = get_selected_id();

    if (id == 0)
        return;

    q.prepare("DELETE FROM plan_oper WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "Error DELETE:" << q.lastError().text();
        return;
    }

    emit data_changed();
}

void ListPlanOper::check_selected()
{
    int id = get_selected_id();
    comm->setEnabled(id);
    delo->setEnabled(id);
}

void ListPlanOper::reload_model()
{
}

void ListPlanOper::clear_model()
{
}
