#include "listplanoper.h"
#include "ui_listplanoper.h"

ListPlanOperModel::ListPlanOperModel(Database *db, QObject *parent) :
    QStandardItemModel(parent)
{
//    db = new Database;
    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Auto") << tr("Description");
    list = db->get_accounts_list();
    var = new Globals;
//    currency = db->get_currency_list();
//    symbol = db->get_currency_scod(var.Currency());
}

ListPlanOperModel::~ListPlanOperModel()
{
//    delete db;
    delete var;
}

void ListPlanOperModel::fill_model(Database *db)
{
//    QSqlQuery q;
    QMap<int,double> oper;
    QMap<int,double>::iterator i;
    QDate curr = QDate::currentDate();
    QList<operation_data> po = db->get_plan_oper_list();
    QList<operation_data>::iterator j;
    int row = 0;

    clear();

    if (!var->database_Opened())
        return;

    insertColumns(0,9);

    for (j = po.begin(); j != po.end(); j++) {
        operation_data data = *j;

        insertRow(row);

        if (db->find_oper_by_plan(data.id, curr.month(), curr.year()) == true) {
            int i;
            for (i = 0; i < 8; i++)
                setData(index(row,i), QColor(Qt::gray), Qt::TextColorRole);
        }
        else if (data.day - curr.day() < 0) {
            int i;
            for (i = 0; i < 8; i++)
                setData(index(row,i), QColor(Qt::red), Qt::BackgroundColorRole);
        }
        else if (data.day - curr.day() < 3) {
            int i;
            for (i = 0; i < 8; i++)
                setData(index(row,i), QColor(Qt::yellow), Qt::BackgroundColorRole);
        }
        setData(index(row,0), data.id);
        setData(index(row,1), data.day);
        setData(index(row,2), data.month);
        setData(index(row,3), data.year);
        oper = db->get_plan_account_oper_list(data.id,1);
        i = oper.begin();
        if (i != oper.end()) {
            setData(index(row,4), list[i.key()]);
        }
        oper = db->get_plan_account_oper_list(data.id,2);
        i = oper.begin();
        if (i != oper.end()) {
            setData(index(row,5), list[i.key()]);
            setData(index(row,6), default_locale->toString(i.value()/var->Kurs(),'f',2));
        }
        setData(index(row,8), data.descr);

        row++;
    }
}

QVariant ListPlanOperModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QStandardItemModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 2 || index.column() == 3) {
            return (value.toInt() == 0) ? "" : value;
        }
        else
            return value;

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

    return value;
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

ListPlanOper::ListPlanOper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListPlanOper)
{
    QList<QAction*>::iterator i;

    ui->setupUi(this);

//    query = "SELECT id,day,month,year,acc_from,acc_to,summ,descr FROM plan_oper ORDER BY day,month,year";
    db = new Database;

    model = new ListPlanOperModel(db);
    model->fill_model(db);
//    model->setQuery(query);

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);

    tran = new QAction(tr("New plan operation"), this);
    comm = new QAction(tr("Commit this operation"), this);
    delo = new QAction(tr("Delete selected operation"), this);
    comm->setEnabled(false);
    delo->setEnabled(false);

    acts.append(tran);
    acts.append(comm);
    acts.append(delo);

    ui->treeView->addActions(acts);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    for (int i = 1; i < 8; i++)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);

    connect(tran, SIGNAL(triggered()), SLOT(new_oper()));
    connect(comm, SIGNAL(triggered()), SLOT(commit_oper()));
    connect(delo, SIGNAL(triggered()), SLOT(del_oper()));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), SLOT(check_selected()));
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
        if (db->new_plan_oper(data) > 0) {
//            model->setQuery(query);
        }
//        ui->treeView->resizeColumnsToContents();
//        ui->treeView->resizeRowsToContents();
        reload_model();
    }

    delete po;
}

int ListPlanOper::get_selected_id()
{
    QModelIndexList list;

    list = ui->treeView->selectionModel()->selectedIndexes();
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
//    od.from = pod.from;
//    od.to = pod.to;
//    od.summ_from = pod.summ.value();
//    od.descr = pod.descr;
//    od.plan_id = id;

    eo->setdata(pod);
    if (eo->exec() == QDialog::Rejected)
        return;

    pod = eo->data();
//    pod.plan_id = id;
    db->save_operation(pod);
    db->new_mon_oper(id);
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

    reload_model();
}

void ListPlanOper::check_selected()
{
    int id = get_selected_id();
    comm->setEnabled(id);
    delo->setEnabled(id);
}

void ListPlanOper::reload_model()
{
//    model->setQuery(query);
    model->fill_model(db);
    ui->treeView->hideColumn(0);
    for (int i = 1; i < 7; i++)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);
}

void ListPlanOper::clear_model()
{
    model->clear();
}
