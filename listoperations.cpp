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
        if (index.column() == 0) {
            return value;
        }
        else if (index.column() == 1) {
            return value;
        }
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

    query = "SELECT acc_from, acc_to, summ, dt, descr FROM operation WHERE dt >= '" + fdate + "' AND dt <= '" + ldate + "' ORDER BY dt";

    model = new ListOperationsModel;
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, tr("From Account"));
    model->setHeaderData(1, Qt::Horizontal, tr("To Account"));
    model->setHeaderData(2, Qt::Horizontal, tr("Summ"));
    model->setHeaderData(3, Qt::Horizontal, tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, tr("Description"));

    QAction *debt = new QAction(tr("Debet"), this);
    QAction *cred = new QAction(tr("Credit"), this);
    QAction *tran = new QAction(tr("Transfer"), this);

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->addAction(debt);
    ui->tableView->addAction(cred);
    ui->tableView->addAction(tran);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    change_current_account(0);

    connect(debt, SIGNAL(triggered()), SLOT(debet_operation()));
    connect(tran, SIGNAL(triggered()), SLOT(transfer_operation()));
    connect(ui->accountcomboBox, SIGNAL(currentIndexChanged(int)), SLOT(change_current_account(int)));
    connect(ui->fdate, SIGNAL(dateChanged(QDate)), SLOT(change_date()));
    connect(ui->ldate, SIGNAL(dateChanged(QDate)), SLOT(change_date()));
}

ListOperations::~ListOperations()
{
    delete ui;
}

void ListOperations::debet_operation()
{
    EditOperation eo;
    operation_data d;

    d.from = 0;
    d.to = ui->accountcomboBox->value();
    d.summ = 0;
    eo.setdata(d);

    if (eo.exec() == QDialog::Accepted) {
        eo.data(d);
        db.save_operation(d.from, d.to, d.summ, d.date, d.descr);
        model->setQuery(query);
    }
}

void ListOperations::credit_operation()
{
    EditOperation eo;
    operation_data d;

    d.from = ui->accountcomboBox->value();
    d.to = 0;
    d.summ = 0;
    eo.setdata(d);

    if (eo.exec() == QDialog::Accepted) {
        eo.data(d);
        db.save_operation(d.from, d.to, d.summ, d.date, d.descr);
        model->setQuery(query);
    }
}

void ListOperations::transfer_operation()
{
    EditOperation eo;
    operation_data d;

    d.from = ui->accountcomboBox->value();
    d.to = 0;
    d.summ = 0;
    eo.setdata(d);

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
