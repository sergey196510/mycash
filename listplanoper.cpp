#include "listplanoper.h"
#include "ui_listplanoper.h"
#include "editplanoper.h"

ListPlanOperModel::ListPlanOperModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Description");
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

    model = new ListPlanOperModel;
    model->setQuery(query);

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);

    QAction *tran = new QAction(tr("Transfer"), this);
    ui->tableView->addAction(tran);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(tran, SIGNAL(triggered()), SLOT(new_oper()));
}

ListPlanOper::~ListPlanOper()
{
    delete ui;
}

void ListPlanOper::new_oper()
{
    editPlanOper *po = new editPlanOper(this);

    po->exec();
}
