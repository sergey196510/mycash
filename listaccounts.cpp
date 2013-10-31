#include "listaccounts.h"
#include "ui_listaccounts.h"
#include "editaccount.h"
#include "correctbalance.h"

double ListAccountsModel::get_list(int parent, QModelIndex idx)
{
    QSqlQuery q;
    int id;
    int i = 0;
    int row;
    double summ = 0;

    q.prepare("SELECT id,name,balance,descr,ccod,hidden FROM account WHERE parent = :parent");
    q.bindValue(":parent", parent);
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return 0;
    }
    while (q.next()) {
        summ += get_list(q.value(0).toInt(), idx);
        if (q.value(5) == false) {
            summ += q.value(2).toDouble();
        }
        insertRow(i, idx);
        if (i == 0)
            insertColumns(0,6,idx);
        setData(index(i,0,idx), q.value(1).toString());
        setData(index(i,1,idx), q.value(2).toDouble());
        setData(index(i,2,idx), q.value(4).toInt());
        setData(index(i,4,idx), q.value(3).toString());
        setData(index(i,3,idx), q.value(5).toBool());
        setData(index(i,5,idx), q.value(0).toInt());
        i++;
    }
    setData(index(row,1), summ);

    return summ;
}

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QStandardItemModel(parent)
{
    QSqlQuery query;
    db = new Database;
    int type;
    int row = 0;
    double summ, summ2;
    QModelIndex idx;
    header_data << tr("Name") << tr("Balance") << tr("Currency") << "Hidden" << tr("Description") << "" << "";

    list = db->get_scod_list();

    insertColumns(0,6);

    query.prepare("SELECT id,name FROM account_type ORDER BY name");
    if (!query.exec()) {
        qDebug() << "Error select";
        return;
    }
    while (query.next()) {
        type = query.value(0).toInt();
        insertRow(row);
        idx = index(row, 0);
        setData(idx, query.value(1).toString());
        QFont font;
        font.setBold(true);
        setData(idx, font, Qt::FontRole);
        int i = 0;
        summ = 0;
        summ2 = 0;
        QSqlQuery q;
        q.prepare("SELECT id,name,balance,descr,ccod,hidden FROM account WHERE type = :type AND parent = 0 ORDER BY name");
        q.bindValue(":type", type);
        if (!q.exec())
            continue;
        while (q.next()) {

            summ2 += get_list(q.value(0).toInt(), index(row,0));

            if (q.value(5) == false) {
                summ += q.value(2).toDouble();
                summ2 += q.value(2).toDouble();
            }
            insertRow(i, idx);
            if (i == 0)
                insertColumns(0,6,idx);
            setData(index(i,0,idx), q.value(1).toString());
            setData(index(i,1,idx), q.value(2).toDouble());
            setData(index(i,2,idx), q.value(4).toInt());
            setData(index(i,4,idx), q.value(3).toString());
            setData(index(i,3,idx), q.value(5).toBool());
            setData(index(i,5,idx), q.value(0).toInt());
            i += 1;
        }
        setData(index(row,1), summ);
        setData(index(row,1), font, Qt::FontRole);
        row += 1;
    }
}

ListAccountsModel::~ListAccountsModel()
{
    delete db;
}

QVariant ListAccountsModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QStandardItemModel::data(index, role);

    if (!index.isValid())
        return QVariant();

//    qDebug() << index;

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 1) {
            return default_locale->toString(value.toDouble());
        }
        else if (index.column() == 2) {
//            return db->get_currency_scod(value.toInt());
            return list[value.toInt()];
        }
        else if (index.column() == 3) {
            return (value.toBool() == false) ? "" : tr("H");
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 1)
                return int(Qt::AlignRight | Qt::AlignVCenter);

    case Qt::TextColorRole:
//        qDebug() << index.data(Qt::DisplayRole) << value.toString();
        if (index.data(Qt::DisplayRole).toString() == "true") {
            return QVariant(QColor(Qt::gray));
        }

        return value;
    }

    return value;
}

QVariant ListAccountsModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

ListAccounts::ListAccounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
    int type;
    QFont font;
    font.setBold(true);

    ui->setupUi(this);

//    type = ui->typeComboBox->value();

//    query = "SELECT a.id,a.name,t.name,a.balance,a.descr,a.hidden FROM account a, account_type t WHERE a.type = t.id AND a.type = " + QString("%1").arg(type) + " ORDER BY type,a.name";

//    fill_model();
    model = new ListAccountsModel;

    ui->treeView->setModel(model);
//    ui->treeView->hideColumn(4);
    ui->treeView->hideColumn(5);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);

    QAction *nacct = new QAction(tr("New account"), this);
    QAction *cacct = new QAction(tr("Correct balance"), this);
    QAction *dacct = new QAction(tr("Delete this account"), this);

    ui->treeView->addAction(nacct);
    ui->treeView->addAction(cacct);
    ui->treeView->addAction(dacct);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(4, QHeaderView::ResizeToContents);

    connect(nacct, SIGNAL(triggered()), SLOT(new_account()));
    connect(cacct, SIGNAL(triggered()), SLOT(correct_balance()));
    connect(dacct, SIGNAL(triggered()), SLOT(del_account()));
    connect(ui->newButton, SIGNAL(released()), SLOT(new_account()));
    connect(ui->correctButton, SIGNAL(released()), SLOT(correct_balance()));
    connect(ui->deleteButton, SIGNAL(released()), SLOT(del_account()));

//    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(check_type()));

    ui->act_summ->setText(default_locale->toCurrencyString(db.get_account_summ(1)));
    ui->act_summ->setFont(font);
}

ListAccounts::~ListAccounts()
{
    delete ui;
    delete model;
}

void ListAccounts::clear_list()
{
    ui->act_summ->setText(default_locale->toCurrencyString(0));
    model->clear();
}

void ListAccounts::reload_model()
{
    delete model;
    model = new ListAccountsModel;
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->act_summ->setText(default_locale->toCurrencyString(db.get_account_summ(1)));
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(4, QHeaderView::ResizeToContents);
}

void ListAccounts::new_account()
{
    Globals var;
    EditAccount ea(this);

    ea.set_curr(var.Currency());

    if (ea.exec() == QDialog::Accepted) {
        Account_Data data = ea.data();
//        QString name = ea.name();
//        int type = ea.type();
//        int ccod = ea.curr();
//        double balance = ea.balance();
//        QString descr = ea.descr();
//        bool hidden = ea.hidden();
        QSqlQuery q;

        if (data.name.length() == 0) {
            return;
        }

        q.prepare("INSERT INTO account(name, type, ccod, balance, descr, hidden) VALUES(:name, :type, :ccod, :balance, :descr, :hidden)");
        q.bindValue(":name", data.name);
        q.bindValue(":type", data.type);
        q.bindValue(":ccod", data.curr);
        q.bindValue(":balance", data.balance);
        q.bindValue(":descr", data.descr);
        q.bindValue(":hidden", (data.hidden == false) ? "false" : "true");
        q.exec();

        reload_model();
    }
}

int ListAccounts::get_selected_id()
{
    QModelIndexList list;

    list = ui->treeView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return 0;
    }

     return list.at(5).data(Qt::DisplayRole).toInt();
}

void ListAccounts::correct_balance()
{
    CorrectBalance cb(this);
    int id = get_selected_id();
    Account_Data data = db.get_account_data(id);

    if (id == 0)
        return;

    cb.setBalance(data.balance);
    if (cb.exec() == QDialog::Accepted) {
        double current_balance = data.balance;
        double new_balance = cb.balance();

        operation_data od;
        od.date = cb.date();
        od.agent = 0;
        od.descr = tr("correct");
        if (new_balance < current_balance) {
            od.from = id;
            od.to = cb.account();
            od.summ = current_balance-new_balance;
            db.save_operation(od);
        }
        else {
            od.to = id;
            od.from = cb.account();
            od.summ = new_balance-current_balance;
            db.save_operation(od);
        }

        reload_model();
    }
}

void ListAccounts::del_account()
{
    QSqlQuery q;
    int id = get_selected_id();

    if (id == 0)
        return;

    int r = QMessageBox::warning(this, tr("Account"),
                                 tr("You want to delete account?"),
                                 QMessageBox::Yes | QMessageBox::No);

    if (r == QMessageBox::No)
        return;

    q.prepare("SELECT count(id) FROM operation WHERE acc_from = :id1 OR acc_to = :id2");
    q.bindValue(":id1", id);
    q.bindValue(":id2", id);
    if (!q.exec() || !q.next()) {
        qDebug() << q.lastError().text();
        return;
    }
    if (q.value(0).toInt() > 0) {
        qDebug() << "have operations";
        return;
    }

    q.prepare("DELETE from account WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return;
    }

    reload_model();
}

void ListAccounts::check_type()
{
//    int type = ui->typeComboBox->value();

//    query = "SELECT a.id,a.name,t.name,a.balance,a.descr,a.hidden FROM account a, account_type t WHERE a.type = t.id AND a.type = " + QString("%1").arg(type) + " ORDER BY type,a.name";
//    fill_model();
    ui->treeView->resizeColumnToContents(0);
}
