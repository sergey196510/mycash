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

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    QAction *tran = new QAction(tr("New plan operation"), this);
    QAction *delo = new QAction(tr("Delete selected operation"), this);
    ui->tableView->addAction(tran);
    ui->tableView->addAction(delo);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

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

     return list.at(5).data(Qt::DisplayRole).toInt();
}

void ListPlanOper::del_oper()
{
    int id = get_selected_id();

    if (id == 0)
        return;
}
