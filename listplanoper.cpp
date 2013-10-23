#include "listplanoper.h"
#include "ui_listplanoper.h"
#include "editplanoper.h"

ListPlanOperModel::ListPlanOperModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    db = new Database;
    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Description");
    list = db->get_accounts_list();

    insertColumns(0,8);
    this->insertRow(0);
    setData(index(0,7), "test");
}

QVariant ListPlanOperModel::data(const QModelIndex &index, int role) const
{
    QDate curr = QDate::currentDate();
//    QVariant value = QAbstractItemModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 2 || index.column() == 3) {
            return (QVariant().toInt() == 0) ? "" : QVariant();
        }
        if (index.column() == 4 || index.column() == 5) {
            return list[QVariant().toInt()];
        }
        else if (index.column() == 6) {
            return default_locale->toString(QVariant().toDouble());
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

//        case Qt::TextColorRole:
//            if (record(index.row()).value(0).toInt() == current_currency) {
//                return QVariant(QColor(Qt::red));
//            }
//            else
//                return value;
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

Node *ListPlanOperModel::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        return static_cast<Node*>(index.internalPointer());
    }
    else {
        return rootNode;
    }
}

QModelIndex ListPlanOperModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex ListPlanOperModel::parent(const QModelIndex &child) const
{
    Node *node = nodeFromIndex(child);
    if (!node)
        return QModelIndex();
    Node *parentNode = node->parent;
    if (!parentNode)
        return QModelIndex();
    Node *grandparentNode = parentNode->parent;
    if (!grandparentNode)
        return QModelIndex();
    int row = grandparentNode->children.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

int ListPlanOperModel::rowCount(const QModelIndex &parent) const
{

}

int ListPlanOperModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

ListPlanOper::ListPlanOper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListPlanOper)
{
    ui->setupUi(this);

    query = "SELECT id,day,month,year,acc_from,acc_to,summ,descr FROM plan_oper ORDER BY day,month,year";
    db = new Database;

    model = new ListPlanOperModel;
//    model->setQuery(query);

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
//    ui->treeView->resizeColumnsToContents();
//    ui->treeView->resizeRowsToContents();
    ui->treeView->setAlternatingRowColors(true);
//    ui->treeView->horizontalHeader()->setStretchLastSection(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->treeView->setShowGrid(false);

    QAction *tran = new QAction(tr("New plan operation"), this);
    QAction *delo = new QAction(tr("Delete selected operation"), this);
    ui->treeView->addAction(tran);
    ui->treeView->addAction(delo);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    for (int i = 1; i < 7; i++)
        ui->treeView->resizeColumnToContents(i);

    connect(tran, SIGNAL(triggered()), SLOT(new_oper()));
    connect(delo, SIGNAL(triggered()), SLOT(del_oper()));
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

//        model->setQuery(query);
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

//    model->setQuery(query);
//    ui->treeView->resizeColumnsToContents();
//    ui->treeView->resizeRowsToContents();
}
