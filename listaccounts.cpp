#include "listaccounts.h"
#include "ui_listaccounts.h"

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

ListAccounts::ListAccounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
    ui->setupUi(this);

    ListAccountsModel *model = new ListAccountsModel;

    model->setQuery("SELECT a.id,a.name,t.name,a.balance FROM account a, account_type t WHERE a.type = t.id");
//    model->select();

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
}

ListAccounts::~ListAccounts()
{
    delete ui;
}
