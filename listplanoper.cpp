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

QList<Operation_Data> ListPlanOperModel::read_list()
{
    QList<Operation_Data> list = db->get_plan_oper_list(0);
    return list;
}

QVariant ListPlanOperModel::data(const QModelIndex &index, int role) const
{
    Operation_Data data;

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 0) {
            data = list.at(index.row());
            return data.id;
        }
        else if (index.column() == 1) {
            data = list.at(index.row());
            return data.day;
        }
        else if (index.column() == 2) {
            data = list.at(index.row());
            if (data.month)
                return data.month;
            else
                return QVariant();
        }
        else if (index.column() == 3) {
            data = list.at(index.row());
            if (data.year)
                return data.year;
            else
                return QVariant();
        }
        else if (index.column() == 4) {
            data = list.at(index.row());
            return acc_list[data.from.at(0).account()];
        }
        else if (index.column() == 5) {
            data = list.at(index.row());
            QMap<int,double> oper = db->get_plan_account_oper_list(data.id,2);
            QMap<int,double>::iterator i = oper.begin();
            return acc_list[i.key()];
        }
        else if (index.column() == 6) {
            data = list.at(index.row());
            QMap<int,double> oper = db->get_plan_account_oper_list(data.id,2);
            QMap<int,double>::iterator i = oper.begin();
            return default_locale->toString(i.value()/Currency(var->Currency()).Kurs(),'f',2);
        }
        else if (index.column() == 7) {
            data = list.at(index.row());
            if (data.auto_exec == 1)
                return tr("Y");
            else
                return QVariant();
        }
//        else if (index.column() == 7) {
//            operation_data data = list.at(index.row());
//            if (data.status == Plan_Status::expired)
//                return tr("Expired");
//            else if (data.status == Plan_Status::minimum)
//                return tr("<3 days");
//            else
//                return QVariant();
//        }
        else if (index.column() == 8) {
            data = list.at(index.row());
            return data.descr;
        }
        else
            return QVariant();

        case Qt::TextAlignmentRole:
            if (index.column() == 1)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            else if (index.column() == 2)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            else if (index.column() == 3)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            else if (index.column() == 6)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            else
                return QVariant();

    case Qt::TextColorRole:
        data = list.at(index.row());
        if (data.status == Plan_Status::committed)
            return QVariant(QColor(Qt::gray));
        else if (data.status == Plan_Status::cancelled)
            return QVariant(QColor(Qt::gray));
        else
            return QVariant();

    case Qt::BackgroundColorRole:
        data = list.at(index.row());
        if (data.status == Plan_Status::minimum)
            return QVariant(QColor(Qt::yellow));
        else if (data.status == Plan_Status::expired)
            return QVariant(QColor(Qt::red));
        else
            return QVariant();
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
    if (parent.isValid())
        return 0;
    return list.size();
}

int ListPlanOperModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return header_data.size();
}

void ListPlanOperModel::change_data()
{
    beginResetModel();
    list.clear();
    list = db->get_plan_oper_list(0);
    endResetModel();
}

int ListPlanOperModel::get_ident(int row)
{
    return list.at(row).id;
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

    // новая плановая операция
    tran = new QAction(tr("New plan operation"), this);
    acts.append(tran);
    connect(tran, SIGNAL(triggered()), SLOT(new_oper()));

    // выполнение плановой операции
    comm = new QAction(tr("Commit this operation"), this);
    acts.append(comm);
    comm->setEnabled(false);
    connect(comm, SIGNAL(triggered()), SLOT(commit_oper()));

    // редактирование плановой операции
    upd = new QAction(tr("Update this operation"), this);
    acts.append(upd);
    upd->setEnabled(false);
    connect(upd, SIGNAL(triggered()), SLOT(update_oper()));

    // удаление плановой операции
    delo = new QAction(tr("Delete selected operation"), this);
    delo->setEnabled(false);
    acts.append(delo);
    connect(delo, SIGNAL(triggered()), SLOT(del_oper()));

    // отметить плановую операцию как выполненную без фактического исполнения
    can = new QAction(tr("Cancel selected operation"), this);
    can->setEnabled(false);
    acts.append(can);
    connect(can, SIGNAL(triggered()), SLOT(cancel_oper()));

    // отмена выполненной плановой операции

    ui->tableView->addActions(acts);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(check_selected()));
}

ListPlanOper::~ListPlanOper()
{
    delete ui;
    delete model;
    delete db;
}

void ListPlanOper::new_oper()
{
    EditOperation *po = new EditOperation(2, this);

    if (po->exec() == QDialog::Accepted) {
        Operation_Data data = po->data();
        if (db->new_plan_oper(data)) {
        }
        emit data_changed();
    }

    delete po;
}

QList<int> ListPlanOper::get_selected_id()
{
    QModelIndexList list;
    QList<int> l;

    list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.size() == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return l;
    }
    qDebug() << list.size();

    for (int i = 0; i < list.size(); i+=9)
        l.append(list.at(i).data(Qt::DisplayRole).toInt());

    return l;
}

void ListPlanOper::commit_oper()
{
    Operation_Data pod;
    Operation_Data od;
    EditOperation *eo = new EditOperation(1, this);
    QList<int> id = get_selected_id();
    QList<int>::iterator i;

    if (id.size() == 0)
        return;

    for (i = id.begin(); i != id.end(); i++) {
        int d = *i;
        pod = db->get_plan_oper_data(d, QDate::currentDate());

        eo->setdata(pod);
        if (eo->exec() == QDialog::Rejected)
            continue;

        pod = eo->data();
        db->save_operation(pod);
        db->new_mon_oper(d,1);
    }

    emit data_changed();
}

void ListPlanOper::update_oper()
{
    QSqlQuery q;
    Operation_Data pod, od;
    EditOperation *eo = new EditOperation(2, this);
    QList<int> id = get_selected_id();
    Transaction tr;

    if (id.size() == 0)
        return;

    pod = db->get_plan_oper_data(id.at(0), QDate::currentDate());

    eo->setdata(pod);
    if (eo->exec() == QDialog::Rejected)
        return;

    tr.begin();

    od = eo->data();
    delete eo;

    od.id = id.at(0);
    if (db->update_plan_oper(od)) {
        tr.commit();
        emit data_changed();
    }
}

void ListPlanOper::del_oper()
{
    QSqlQuery q;
    QList<int> id = get_selected_id();
    Transaction tr;

    if (id.size() == 0)
        return;

    tr.begin();

    q.prepare("DELETE FROM plan_oper_acc WHERE o_id = :id");
    q.bindValue(":id", id.at(0));
    if (!q.exec()) {
        tr.rollback();
        qDebug() << "Error DELETE:" << q.lastError().text();
        return;
    }

    q.prepare("DELETE FROM plan_oper WHERE id = :id");
    q.bindValue(":id", id.at(0));
    if (!q.exec()) {
        tr.rollback();
        qDebug() << "Error DELETE:" << q.lastError().text();
        return;
    }

    tr.commit();
    emit data_changed();
}

void ListPlanOper::cancel_oper()
{
    QSqlQuery q;
    QList<int> id = get_selected_id();
    Transaction tr;

    if (id.empty())
        return;

    tr.begin();

    db->new_mon_oper(id.at(0),2);

    tr.commit();
    emit data_changed();
}

void ListPlanOper::check_selected()
{
    QList<int> id = get_selected_id();
    if (id.empty())
        return;

    comm->setEnabled(id.at(0));
    upd->setEnabled(id.at(0));
    delo->setEnabled(id.at(0));
    can->setEnabled(id.at(0));
}

void ListPlanOper::reload_model()
{
}

void ListPlanOper::clear_model()
{
}
