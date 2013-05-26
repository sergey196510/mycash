#include "listaccounts.h"
#include "ui_listaccounts.h"

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QSqlTableModel(parent)
{

}

ListAccounts::ListAccounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
    ui->setupUi(this);

    ListAccountsModel *model = new ListAccountsModel;

    model->setTable("account");
    model->select();

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
}

ListAccounts::~ListAccounts()
{
    delete ui;
}
