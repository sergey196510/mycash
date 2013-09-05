#include "listaccounts.h"
#include "ui_listaccounts.h"
#include "editaccount.h"

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

ListAccounts::ListAccounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
    ui->setupUi(this);

    query = "SELECT a.id,a.name,t.name,a.balance FROM account a, account_type t WHERE a.type = t.id ORDER BY type";

    model = new ListAccountsModel;
    model->setQuery(query);

//    model->select();

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
    ui->treeView->resizeColumnToContents(1);

    QAction *nacct = new QAction(tr("New account"), this);
    QAction *eacct = new QAction(tr("Edit balance"), this);
    QAction *dacct = new QAction(tr("Delete this account"), this);

    ui->treeView->addAction(nacct);
    ui->treeView->addAction(eacct);
    ui->treeView->addAction(dacct);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->treeView->setAlternatingRowColors(true);

    connect(nacct, SIGNAL(triggered()), SLOT(new_account()));

    ui->act_summ->setNum(db.get_accout_summ(1));
}

ListAccounts::~ListAccounts()
{
    delete ui;
    delete model;
}

void ListAccounts::new_account()
{
    EditAccount ea;

    if (ea.exec() == QDialog::Accepted) {
        QString name = ea.name();
        int type = ea.type();
        double balance = ea.balance();
        QSqlQuery q;

        if (name.length() == 0) {
            return;
        }

        q.prepare("INSERT INTO account(name, type, balance) VALUES(:name, :type, :balance)");
        q.bindValue(":name", name);
        q.bindValue(":type", type);
        q.bindValue(":balance", balance);
        q.exec();

        model->setQuery(query);
    }
}
