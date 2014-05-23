#include "listoperations.h"
#include "ui_listoperations.h"
#include "global.h"

void ListOperationsModel::fill_model(QDate *dt1, QDate *dt2, int acc_id)
{
    QSqlQuery q1, q2;
    QString query;
    int row = 0;
    int a1, a2;
    double s1, s2;

    header_data << "" << tr("Date") << tr("Account") << tr("Debet") << tr("Credit") << tr("Description");

    clear();

    insertColumns(0,6);

    if (acc_id == 0)
        return;

    db = new Database;
    var = new Globals;
    if (!var->database_Opened())
        return;

    list = db->get_accounts_list();

    query = QString("SELECT id,dt,descr FROM oper WHERE dt >= '%1' AND dt <= '%2' ORDER BY dt")
            .arg(dt1->toString("yyyy-MM-dd"))
            .arg(dt2->toString("yyyy-MM-dd"));
    if (!q1.exec(query)) {
        qDebug() << q1.lastError().text();
        return;
    }
    while (q1.next()) {
        int oid = q1.value(0).toInt();

        q2.prepare("SELECT a_id,summ FROM account_oper WHERE o_id = :oid and direction = :direct");
        q2.bindValue(":oid", oid);
        q2.bindValue(":direct", Direction::from);
        if (!q2.exec()) {
            qDebug() << q1.lastError().text();
            return;
        }
        if (q2.next()) {
            a1 = q2.value(0).toInt();
            s1 = q2.value(1).toDouble();
        }

        q2.prepare("SELECT a_id,summ FROM account_oper WHERE o_id = :oid and direction = :direct");
        q2.bindValue(":oid", oid);
        q2.bindValue(":direct", Direction::to);
        if (!q2.exec()) {
            qDebug() << q1.lastError().text();
            return;
        }
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
            setData(index(row,col_Credit), s1);
        }
        else if (a2 == acc_id) {
            setData(index(row,col_Account), list[a1]);
            setData(index(row,col_Debet), s2);
        }

        setData(index(row,col_Date), q1.value(1).toString());
        setData(index(row,col_Descr), q1.value(2).toString());

        row++;
    }
}

ListOperationsModel::ListOperationsModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

ListOperationsModel::~ListOperationsModel()
{
    delete db;
    delete var;
}

QVariant ListOperationsModel::data(const QModelIndex &index, int role) const
{
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
    QDate fdate, ldate;

    ui->setupUi(this);

    ui->splitter->setStretchFactor(1,1);

    db = new Database;

    ui->listAccounts->setValue(var.Account());
    ui->fdate->setDate(QDate().currentDate().addDays(-29));
    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    model = new ListOperationsModel();
    model->fill_model(&fdate, &ldate, var.Account());

    debt = new QAction(tr("Debet"), this);
    debt->setShortcut(tr("Alt+N"));
    debt->setToolTip(tr("Debet operation"));

    cred = new QAction(tr("Credit"), this);
    cred->setShortcut(tr("Alt+C"));
    cred->setToolTip(tr("Credit operation"));

    tran = new QAction(tr("Transfer"), this);
    tran->setShortcut(tr("Alt+T"));
    tran->setToolTip(tr("Transfer operation"));

    plan = new QAction(tr("Planning operation"), this);
    plan->setShortcut(tr("Alt+P"));
    plan->setToolTip(tr("Planning operation"));

    dele = new QAction(tr("Delete current operation"), this);
    dele->setShortcut(tr("Alt+D"));
    dele->setText(tr("Delete operation"));

    acts.append(debt);
    acts.append(cred);
    acts.append(tran);
    acts.append(plan);
    acts.append(dele);

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    for (int i = 1; i < 5; i++)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);

    change_current_account();

    ui->treeView->addActions(acts);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(debt, SIGNAL(triggered()), SLOT(debet_operation()));
    connect(cred, SIGNAL(triggered()), SLOT(credit_operation()));
    connect(tran, SIGNAL(triggered()), SLOT(transfer_operation()));
    connect(plan, SIGNAL(triggered()), SLOT(plann_operation()));
    connect(dele, SIGNAL(triggered()), SLOT(del_operation()));
    connect(ui->fdate, SIGNAL(dateChanged(QDate)), SLOT(reload_model()));
    connect(ui->ldate, SIGNAL(dateChanged(QDate)), SLOT(reload_model()));

    connect(ui->listAccounts, SIGNAL(activated(QModelIndex)), SLOT(reload_model()));
    connect(ui->listAccounts->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(reload_model()));
}

ListOperations::~ListOperations()
{
    delete ui;
    delete db;
}

void ListOperations::edit_operation(operation_data &d)
{
    EditOperation eo(1, this);
    QModelIndex idx = ui->treeView->currentIndex();
    operation_data data;

    eo.setdata(d);
    if (eo.exec() == QDialog::Accepted) {
        data = eo.data();

        db->save_operation(data);
        ui->treeView->setCurrentIndex(idx);
        reload_model();
    }
}

void ListOperations::debet_operation()
{
    operation_data d;
    account_summ a;

    a.set_account(ui->listAccounts->value());
    d.to.append(a);

    edit_operation(d);
}

void ListOperations::credit_operation()
{
    operation_data d;

    d.from.set_account(ui->listAccounts->value());

    edit_operation(d);
}

void ListOperations::transfer_operation()
{
    operation_data d;

    d.from.set_account(ui->listAccounts->value());

    edit_operation(d);
}

int ListOperations::get_selected_id()
{
    QModelIndexList list;

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

    int r = QMessageBox::warning(this, tr("Operation"),
                                 tr("You want to delete operation?\n" \
                 "Nadeusj, vi ponimaete chto delaeye,\n" \
                 "tak kak v etom sluchae budut izmeneni balansi accounts"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (r == QMessageBox::No)
        return;

    db->del_operation(id);

    reload_model();

    return;
}

void ListOperations::plann_operation()
{
    EditOperation pd(2, this);
    int id = get_selected_id();

    if (id == 0)
        return;

    operation_data data = db->get_operation(id);
    data.day = QDate::currentDate().day();

    pd.setdata(data);
    if (pd.exec() == QDialog::Accepted) {
        data = pd.data();
        int plan = db->new_plan_oper(data);
        QSqlQuery q;

	if (plan == 0) {
	    qDebug() << "Error planning operation";
	    return;
	}

        q.prepare("UPDATE operation SET plan_id = :plan WHERE id = :id");
        q.bindValue(":plan", plan);
        q.bindValue(":id", id);
        if (!q.exec())
            qDebug() << q.lastError().text();
    }
}

void ListOperations::change_current_account()
{
    Globals var;
    QLocale *lc;
    QFont font;
    font.setBold(true);
    int id = ui->listAccounts->value();
    Account_Data data = db->get_account_data(id);
    double result = db->convert_currency(data.balance.value(), data.curr);

    lc = default_locale;
    ui->account_ostatok->setFont(font);
    ui->account_ostatok->setText(lc->toString(result,'f',2));

    var.setAccount(ui->listAccounts->value());
}

void ListOperations::reload_model()
{
    QDate fdate, ldate;
    int id = ui->listAccounts->value();

    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    change_current_account();

    model->fill_model(&fdate, &ldate, id);
    ui->treeView->hideColumn(0);
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    for (int i = 1; i < 5; i++)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);
}

void ListOperations::clear_model()
{
    ui->account_ostatok->setText("0");
    model->clear();
}
