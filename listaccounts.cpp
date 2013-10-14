#include "listaccounts.h"
#include "ui_listaccounts.h"
#include "editaccount.h"
#include "correctbalance.h"

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QStandardItemModel(parent)
{

}

QVariant ListAccountsModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QStandardItemModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 3) {
//            return tr("%1").arg(value.toDouble(), 0, 'f', 2);
            return default_locale->toString(value.toDouble());
//            return english->toCurrencyString(value.toDouble());
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 3)
                return int(Qt::AlignRight | Qt::AlignVCenter);

//    case Qt::TextColorRole:
//        if (record(index.row()).value(5).toBool() == true) {
//            return QVariant(QColor(Qt::gray));
//        }
        return value;
    }

    return value;
}

ListAccounts::ListAccounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
    int type;

    ui->setupUi(this);

    type = ui->typeComboBox->value();

    query = "SELECT a.id,a.name,t.name,a.balance,a.desct,a.hidden FROM account a, account_type t WHERE a.type = t.id AND a.type = " + QString("%1").arg(type) + " ORDER BY type,a.name";

    model = new ListAccountsModel;
    fill_model();
//    model->setQuery(query);

//    model->setHeaderData(1, Qt::Horizontal, tr("Name"));
//    model->setHeaderData(2, Qt::Horizontal, tr("Type"));
//    model->setHeaderData(3, Qt::Horizontal, tr("Balance"));
//    model->setHeaderData(4, Qt::Horizontal, tr("Description"));

//    model->select();

    ui->treeView->setModel(model);
//    ui->treeView->hideColumn(0);
//    ui->treeView->hideColumn(5);
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

    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(check_type()));

//    ui->act_summ->setNum(db.get_account_summ(1));
    ui->act_summ->setText(default_locale->toCurrencyString(db.get_account_summ(1)));
}

ListAccounts::~ListAccounts()
{
    delete ui;
    delete model;
}

void ListAccounts::fill_model()
{
    QSqlQuery query;
    QModelIndex index;
    int row = 0;

    model = new ListAccountsModel;
    model->insertColumns(0,5);

    query.prepare("SELECT id,name FROM account_type ORDER BY name");
    if (!query.exec()) {
        qDebug() << "Error select";
        return;
    }

    while (query.next()) {
        model->insertRow(row);
        model->setData(model->index(row,0), query.value(0).toInt());
        model->setData(model->index(row,1), query.value(1).toString());
        index = model->index(row, 0);
        int i = 0;
        while (i < 5) {
            model->insertRow(i, index);
            if (i == 0)
                model->insertColumns(0,5,index);
            model->setData(model->index(i,1,index), "test");
            i += 1;
        }
        row += 1;
    }
}

void ListAccounts::new_account()
{
    EditAccount ea(this);

    ea.set_curr(current_currency);

    if (ea.exec() == QDialog::Accepted) {
        QString name = ea.name();
        int type = ea.type();
        int ccod = ea.curr();
        double balance = ea.balance();
        QString descr = ea.descr();
        bool hidden = ea.hidden();
        QSqlQuery q;

        if (name.length() == 0) {
            return;
        }

        q.prepare("INSERT INTO account(name, type, ccod, balance, desct, hidden) VALUES(:name, :type, :ccod, :balance, :descr, :hidden)");
        q.bindValue(":name", name);
        q.bindValue(":type", type);
        q.bindValue(":ccod", ccod);
        q.bindValue(":balance", balance);
        q.bindValue(":descr", descr);
        q.bindValue(":hidden", hidden);
        q.exec();

//        model->setQuery(query);
        fill_model();
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

//        model->setQuery(query);
        fill_model();
    }
}

void ListAccounts::check_type()
{
    int type = ui->typeComboBox->value();

    query = "SELECT a.id,a.name,t.name,a.balance,a.desct,a.hidden FROM account a, account_type t WHERE a.type = t.id AND a.type = " + QString("%1").arg(type) + " ORDER BY type,a.name";
//    model->setQuery(query);
    fill_model();
    ui->treeView->resizeColumnToContents(1);
}
