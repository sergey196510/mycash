#include "listoperations.h"
#include "ui_listoperations.h"
#include "editoperation.h"

ListOperations::ListOperations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListOperations)
{
    ui->setupUi(this);

    query = "SELECT acc_from, acc_to, summ, dt FROM operation ORDER BY dt";

    model = new QSqlQueryModel;
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
