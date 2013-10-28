#include "listplanoper.h"
#include "ui_listplanoper.h"
#include "editplanoper.h"

ListPlanOperModel::ListPlanOperModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    db = new Database;
    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Description");
    list = db->get_accounts_list();
}

QVariant ListPlanOperModel::data(const QModelIndex &index, int role) const
{
    QDate curr = QDate::currentDate();
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 2 || index.column() == 3) {
            return (value.toInt() == 0) ? "" : value;
        }
        if (index.column() == 4 || index.column() == 5) {
            return list[value.toInt()];
        }
        else if (index.column() == 6) {
            return default_locale->toString(value.toDouble());
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

//        case Qt::TextColorRole:
//            if (record(index.row()).value(0).toInt() == current_currency) {
//                return QVariant(QColor(Qt::red));
//            }
//            else
//                return value;
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
    ui->setupUi(this);

    query = "SELECT id,day,month,year,acc_from,acc_to,summ,descr FROM plan_oper ORDER BY day,month,year";
    db = new Database;

    model = new ListPlanOperModel;
    model->setQuery(query);

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
//    ui->treeView->resizeColumnsToContents();
//    ui->treeView->resizeRowsToContents();
    ui->treeView->setAlternatingRowColors(true);
//    ui->treeView->horizontalHeader()->setStretchLastSection(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->treeView->setShowGrid(false);

    tran = new QAction(tr("New plan operation"), this);
    comm = new QAction(tr("Commit this operation"), this);
    delo = new QAction(tr("Delete selected operation"), this);
    comm->setEnabled(false);
    delo->setEnabled(false);

    ui->treeView->addAction(tran);
    ui->treeView->addAction(comm);
    ui->treeView->addAction(delo);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(4, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(5, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(6, QHeaderView::ResizeToContents);

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
    editPlanOper *po = new editPlanOper(this);

    if (po->exec() == QDialog::Accepted) {
        PlanOper_data data = po->Value();
        db->new_plan_oper(data);

        model->setQuery(query);
//        ui->treeView->resizeColumnsToContents();
//        ui->treeView->resizeRowsToContents();
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
    PlanOper_data pod;
    operation_data od;
    EditOperation *eo = new EditOperation;
    int id = get_selected_id();

    if (id == 0)
        return;

    pod = db->get_plan_oper_data(id);
    od.from = pod.from;
    od.to = pod.to;
    od.summ = pod.summ;
    od.descr = pod.descr;
    od.plan_id = id;

    eo->setdata(od);
    if (eo->exec() == QDialog::Rejected)
        return;

    eo->data(od);
    od.plan_id = id;
    db->save_operation(od);

    model->setQuery(query);
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

    model->setQuery(query);
//    ui->treeView->resizeColumnsToContents();
//    ui->treeView->resizeRowsToContents();
}

void ListPlanOper::check_selected()
{
    int id = get_selected_id();
    comm->setEnabled(id);
    delo->setEnabled(id);
}
