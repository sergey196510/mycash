#include "listaccounts.h"
#include "ui_listaccounts.h"
#include "editaccount.h"
#include "correctbalance.h"

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

QVariant ListAccountsModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 3) {
//            return tr("%1").arg(value.toDouble(), 0, 'f', 2);
            return locale->toCurrencyString(value.toDouble());
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 3)
                return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    return value;
}

ListAccounts::ListAccounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
    ui->setupUi(this);

    query = "SELECT a.id,a.name,t.name,a.balance,a.desct FROM account a, account_type t WHERE a.type = t.id ORDER BY type,a.name";

    model = new ListAccountsModel;
    model->setQuery(query);

//    model->select();

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
    ui->treeView->resizeColumnToContents(1);

    QAction *nacct = new QAction(tr("New account"), this);
    QAction *cacct = new QAction(tr("Correct balance"), this);
    QAction *dacct = new QAction(tr("Delete this account"), this);

    ui->treeView->addAction(nacct);
    ui->treeView->addAction(cacct);
    ui->treeView->addAction(dacct);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
//    ui->treeView->horizontalHeader()->setStretchLastSection(true);

    ui->treeView->setAlternatingRowColors(true);

    connect(nacct, SIGNAL(triggered()), SLOT(new_account()));
    connect(cacct, SIGNAL(triggered()), SLOT(correct_balance()));

    ui->act_summ->setNum(db.get_account_summ(1));
}

ListAccounts::~ListAccounts()
{
    delete ui;
    delete model;
}

void ListAccounts::new_account()
{
    EditAccount ea(this);

    if (ea.exec() == QDialog::Accepted) {
        QString name = ea.name();
        int type = ea.type();
        double balance = ea.balance();
        QString descr = ea.descr();
        QSqlQuery q;

        if (name.length() == 0) {
            return;
        }

        q.prepare("INSERT INTO account(name, type, balance, desct) VALUES(:name, :type, :balance, :descr)");
        q.bindValue(":name", name);
        q.bindValue(":type", type);
        q.bindValue(":balance", balance);
        q.bindValue(":descr", descr);
        q.exec();

        model->setQuery(query);
    }
}

void ListAccounts::correct_balance()
{
    CorrectBalance cb(this);
    QModelIndexList list;
    int id;

    list = ui->treeView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return;
    }

    id = list.at(0).data((Qt::DisplayRole)).toInt();

    cb.setBalance(db.get_account_balance(id));
    if (cb.exec() == QDialog::Accepted) {
        double current_balance = db.get_account_balance(id);
        double new_balance = cb.balance();

        if (new_balance < current_balance)
            db.save_operation(id, cb.account(), 0, current_balance-new_balance, cb.date(), tr("Correct"));
        else
            db.save_operation(cb.account(), 0, id, new_balance-current_balance, cb.date(), tr("Correct"));

        model->setQuery(query);
    }
}
