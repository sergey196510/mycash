#include "listaccounts.h"
#include "ui_listaccounts.h"
#include "QtSql"

ListAccounts::ListAccounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
    ui->setupUi(this);

    QSqlTableModel *model = new QSqlTableModel;

    model->setTable("account");
    model->select();

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
}

ListAccounts::~ListAccounts()
{
    delete ui;
}
