#include "listoperations.h"
#include "ui_listoperations.h"
#include "editoperation.h"

ListOperations::ListOperations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListOperations)
{
    ui->setupUi(this);

    query = "SELECT";

    QAction *noper = new QAction(tr("New operation"), this);

    ui->tableView->addAction(noper);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

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
    }
}

void ListOperations::change_current_account(int idx)
{

}
