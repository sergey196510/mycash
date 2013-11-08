#include "listoperations.h"
#include "ui_listoperations.h"
#include "global.h"

enum {
    col_Id = 0,
    col_Date = 1,
    col_Account = 2,
    col_Debet = 3,
    col_Credit = 4,
    col_Descr = 5
};

ListOperationsModel::ListOperationsModel(QString *dt1, QString *dt2, int acc_id, QObject *parent) :
    QStandardItemModel(parent)
{
    QSqlQuery q1, q2;
    QString query;
    int row = 0;
    int a1, a2;
    double s1, s2;

    header_data << "" << tr("Date") << tr("Account") << tr("Debet") << tr("Credit") << tr("Description");

    insertColumns(0,6);

    if (acc_id == 0)
        return;

    db = new Database;
    var = new Globals;
    list = db->get_accounts_list();

    query = QString("SELECT id,dt,descr FROM operation WHERE dt >= '%1' AND dt <= '%2'").arg(*dt1).arg(*dt2);
    if (!q1.exec(query)) {
        qDebug() << q1.lastError().text();
        return;
    }
    while (q1.next()) {
        int oid = q1.value(0).toInt();

        q2.prepare("SELECT a_id,summ FROM account_oper WHERE o_id = :oid and direction = 1");
        q2.bindValue(":aid", acc_id);
        q2.bindValue(":oid", oid);
        if (!q2.exec()) {
            qDebug() << q1.lastError().text();
            return;
        }
        int ss = q2.size();
        if (q2.next()) {
            a1 = q2.value(0).toInt();
            s1 = q2.value(1).toDouble();
        }

        q2.prepare("SELECT a_id,summ FROM account_oper WHERE o_id = :oid and direction = 2");
        q2.bindValue(":aid", acc_id);
        q2.bindValue(":oid", oid);
        if (!q2.exec()) {
            qDebug() << q1.lastError().text();
            return;
        }
        ss = q2.size();
        if (q2.next()) {
            a2 = q2.value(0).toInt();
            s2 = q2.value(1).toDouble();
        }

        if (a1 != acc_id && a2 != acc_id)
            continue;

        insertRow(row);
        setData(index(row,col_Id), oid);

        if (a1 == acc_id) {
            setData(index(row,col_Account), list[a2]);
            setData(index(row,col_Credit), s2);
        }
        else if (a2 == acc_id) {
            setData(index(row,col_Account), list[a1]);
            setData(index(row,col_Debet), s1);
        }

        setData(index(row,col_Date), q1.value(1).toString());
        setData(index(row,col_Descr), q1.value(2).toString());

        row++;
    }
}

ListOperationsModel::~ListOperationsModel()
{
    delete db;
    delete var;
}

QVariant ListOperationsModel::data(const QModelIndex &index, int role) const
{
    QDate curr = QDate::currentDate();
    QVariant value = QStandardItemModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == col_Debet && value.toDouble() != 0) {
            return default_locale->toString(value.toDouble()/var->Kurs(),'f',2);
        }
        if (index.column() == col_Credit && value.toDouble() != 0) {
            return default_locale->toString(value.toDouble()/var->Kurs(),'f',2);
        }
        else if (index.column() == col_Date) {
            QDate dt = value.toDate();
            return value.toDate().toString(Qt::SystemLocaleDate);
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == col_Debet)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == col_Credit)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == col_Date)
                return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    return value;
}

QVariant ListOperationsModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

ListOperations::ListOperations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListOperations)
{
    Globals var;
    QString fdate, ldate;

    ui->setupUi(this);

    d.agent = 0;

    db = new Database;

    list = db->get_currency_list();

    ui->accountcomboBox->load(1);
    ui->accountcomboBox->setValue(var.Account());
    reload_model();
    ui->fdate->setDate(QDate().currentDate().addDays(-29));
    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    model = new ListOperationsModel(&fdate, &ldate, var.Account());

    QAction *debt = new QAction(tr("Debet"), this);
    QAction *cred = new QAction(tr("Credit"), this);
    QAction *tran = new QAction(tr("Transfer"), this);
    QAction *plan = new QAction(tr("Planning operation"), this);
    QAction *dele = new QAction(tr("Delete current operation"), this);

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(4, QHeaderView::ResizeToContents);

    ui->treeView->addAction(debt);
    ui->treeView->addAction(cred);
    ui->treeView->addAction(tran);
    ui->treeView->addAction(plan);
    ui->treeView->addAction(dele);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(debt, SIGNAL(triggered()), SLOT(debet_operation()));
    connect(cred, SIGNAL(triggered()), SLOT(credit_operation()));
    connect(tran, SIGNAL(triggered()), SLOT(transfer_operation()));
    connect(plan, SIGNAL(triggered()), SLOT(plann_operation()));
    connect(dele, SIGNAL(triggered()), SLOT(del_operation()));
    connect(ui->accountcomboBox, SIGNAL(currentIndexChanged(int)), SLOT(reload_model()));
    connect(ui->fdate, SIGNAL(dateChanged(QDate)), SLOT(reload_model()));
    connect(ui->ldate, SIGNAL(dateChanged(QDate)), SLOT(reload_model()));
}

ListOperations::~ListOperations()
{
    delete ui;
    delete db;
}

void ListOperations::edit_operation(int oper)
{
    Globals var;
    QModelIndex idx = ui->treeView->currentIndex();

    if (eo.exec() == QDialog::Accepted) {
        eo.data(d);

        db->save_operation(d);
        ui->treeView->setCurrentIndex(idx);
        reload_model();
    }
}

void ListOperations::debet_operation()
{
    d.from = 0;
    d.to = ui->accountcomboBox->value();
    d.summ_from = 0;
    eo.setdata(d);

    edit_operation(1);
}

void ListOperations::credit_operation()
{
    d.from = ui->accountcomboBox->value();
    d.to = 0;
    d.summ_from = 0;
    eo.setdata(d);

    edit_operation(2);
}

void ListOperations::transfer_operation()
{
    d.from = ui->accountcomboBox->value();
    d.to = 0;
    d.summ_from = 0;
    eo.setdata(d);

    edit_operation(2);
}

int ListOperations::get_selected_id()
{
    QModelIndexList list;
    int id;

    list = ui->treeView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return 0;
    }

    return list.at(0).data((Qt::DisplayRole)).toInt();
}

void ListOperations::del_operation()
{
    QSqlQuery q;
    operation_data data;
    int id = get_selected_id();

    if (id == 0)
        return;

    data = db->get_operation(id);

    int r = QMessageBox::warning(this, tr("Operation"),
                                 tr("You want to delete operation?"),
                                 QMessageBox::Yes | QMessageBox::No);

    if (r == QMessageBox::No)
        return;

    q.exec("BEGIN");

    if (db->change_account_balance(data.from, data.summ_from) == false) {
        q.exec("ROLLBACK TRANSACTION");
        QMessageBox::critical(this, tr("Operation"), tr("Error in delere record"));
        return;
    }
    if (db->change_account_balance(data.to, -data.summ_to) == false) {
        q.exec("ROLLBACK TRANSACTION");
        QMessageBox::critical(this, tr("Operation"), tr("Error in delere record"));
        return;
    }
    if (db->del_account_oper(id) == false) {
        q.exec("ROLLBACK TRANSACTION");
        QMessageBox::critical(this, tr("Operation"), tr("Error in delere record"));
        return;
    }
    if (db->del_operation(id) == false) {
        q.exec("ROLLBACK TRANSACTION");
        QMessageBox::critical(this, tr("Operation"), tr("Error in delere record"));
        return;
    }

    q.exec("COMMIT");

    reload_model();

    return;
}

void ListOperations::plann_operation()
{
    PlanOper_data po;
    editPlanOper pd;
    int id = get_selected_id();

    if (id == 0)
        return;

    operation_data data = db->get_operation(id);
    po.day = QDate::currentDate().day();
    po.month = 0;
    po.year = 0;
    po.from = data.from;
    po.to = data.to;
    po.summ = data.summ_from;
    po.descr = data.descr;

    pd.setValue(po);
    if (pd.exec() == QDialog::Accepted) {
        po = pd.Value();
        int plan = db->new_plan_oper(po);
        QSqlQuery q;

        q.prepare("UPDATE operation SET plan_id = :plan WHERE id = :id");
        q.bindValue(":plan", plan);
        q.bindValue(":id", id);
        if (!q.exec())
            qDebug() << q.lastError().text();
    }
}

void ListOperations::change_current_account(int idx)
{
    Globals var;
    QLocale *lc;
    QFont font;
    font.setBold(true);
    int id = ui->accountcomboBox->value();
    Account_Data data = db->get_account_data(id);

    lc = default_locale;
    ui->account_ostatok->setFont(font);
    ui->account_ostatok->setText(lc->toString(data.balance/var.Kurs(),'f',2));

    var.setAccount(ui->accountcomboBox->value());
}

void ListOperations::reload_model()
{
    QString fdate, ldate;
    int id = ui->accountcomboBox->value();

    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    model = new ListOperationsModel(&fdate, &ldate, id);
    ui->treeView->setModel(model);
    change_current_account();
}
