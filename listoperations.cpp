#include "listoperations.h"
#include "ui_listoperations.h"
#include "editoperation.h"

ListOperationsModel::ListOperationsModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

QVariant ListOperationsModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 2) {
            return tr("%1").arg(value.toDouble(), 0, 'f', 2);
        }
        else if (index.column() == 3) {
            return value.toDate().toString("dddd dd MMMM yyyy");
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 2)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 3)
                return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    return value;
}

ListOperations::ListOperations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListOperations)
{
    QString fdate, ldate;

    ui->setupUi(this);

    ui->fdate->setDate(QDate().currentDate().addDays(-30));
    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    query = "SELECT a.name, acc_to, summ, dt, descr FROM account a, operation o WHERE o.acc_from = a.id AND dt >= '" + fdate + "' AND dt <= '" + ldate + "' ORDER BY dt";

    model = new ListOperationsModel;
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, tr("From Account"));
    model->setHeaderData(1, Qt::Horizontal, tr("To Account"));
    model->setHeaderData(2, Qt::Horizontal, tr("Summ"));
    model->setHeaderData(3, Qt::Horizontal, tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, tr("Description"));

    QAction *noper = new QAction(tr("New operation"), this);

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->addAction(noper);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    change_current_account(0);

    connect(noper, SIGNAL(triggered()), SLOT(new_operation()));
    connect(ui->accountcomboBox, SIGNAL(currentIndexChanged(int)), SLOT(change_current_account(int)));
    connect(ui->fdate, SIGNAL(dateChanged(QDate)), SLOT(change_date()));
    connect(ui->ldate, SIGNAL(dateChanged(QDate)), SLOT(change_date()));
}

ListOperations::~ListOperations()
{
    delete ui;
}

void ListOperations::new_operation()
{
    EditOperation eo;
    operation_data d;

    if (eo.exec() == QDialog::Accepted) {
        eo.data(d);
        db.save_operation(d.from, d.to, d.summ, d.date, d.descr);
        model->setQuery(query);
    }
}

void ListOperations::change_current_account(int idx)
{
    ui->account_ostatok->setText(tr("%1").arg(db.get_account_balance(ui->accountcomboBox->value()), 0, 'f', 2));
}

void ListOperations::change_date()
{
    QString fdate, ldate;

    fdate = ui->fdate->value();
    ldate = ui->ldate->value();
    query = "SELECT a.name, acc_to, summ, dt, descr FROM account a, operation o WHERE o.acc_from = a.id AND dt >= '" + fdate + "' AND dt <= '" + ldate + "' ORDER BY dt";

    model->setQuery(query);
}
