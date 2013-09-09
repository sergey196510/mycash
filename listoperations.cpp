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
            return value;
        }
        else if (index.column() == 3) {
            return value.toDate().toString("dddd dd MMMM yyyy");
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 2)
                return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    return value;
}

ListOperations::ListOperations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListOperations)
{
    ui->setupUi(this);

    query = "SELECT a.name, acc_to, summ, dt FROM account a, operation o WHERE o.acc_from = a.id ORDER BY dt";

    model = new ListOperationsModel;
    model->setQuery(query);

    QAction *noper = new QAction(tr("New operation"), this);

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->addAction(noper);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->account_ostatok->setNum(db.get_account_balance(ui->accountcomboBox->value()));

    connect(noper, SIGNAL(triggered()), SLOT(new_operation()));
    connect(ui->accountcomboBox, SIGNAL(currentIndexChanged(int)), SLOT(change_current_account(int)));
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
        db.save_operation(d.from, d.to, d.summ, d.date);
        model->setQuery(query);
    }
}

void ListOperations::change_current_account(int idx)
{
    ui->account_ostatok->setNum(db.get_account_balance(ui->accountcomboBox->value()));
}
