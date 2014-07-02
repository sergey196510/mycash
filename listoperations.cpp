#include "listoperations.h"
#include "ui_listoperations.h"
#include "global.h"

ListOperationsModel::ListOperationsModel(int account, QDate fdate, QDate ldate, QObject *parent) :
    QAbstractItemModel(parent)
{
    header_data << "" << tr("Date") << tr("Account") << tr("Debet") << tr("Credit") << tr("Description");
    list = read_list(account, fdate, ldate);
    accounts_list = db->get_accounts_list();
}

ListOperationsModel::~ListOperationsModel()
{
//    delete db;
    delete var;
}

QList<operation_data> ListOperationsModel::read_list(int account, QDate fdate, QDate ldate)
{
    QSqlQuery q1, q2;
    QList<operation_data> list;
    account_summ summ;
    int a1, a2;
    double s1, s2;

    if (account == 0)
        return list;

    q1.prepare("SELECT id,dt,descr FROM oper WHERE dt >= :fdate AND dt <= :ldate ORDER BY dt");
    q2.prepare("SELECT a_id,summ FROM account_oper WHERE o_id = :oid and direction = :direct");

    q1.bindValue(":fdate", fdate.toString("yyyy-MM-dd"));
    q1.bindValue(":ldate", ldate.toString("yyyy-MM-dd"));
    if (!q1.exec()) {
        qDebug() << q1.lastError();
        return list;
    }
    while (q1.next()) {
        operation_data data;
        data.id = q1.value(0).toInt();
        data.date = q1.value(1).toDate();
        data.descr = q1.value(2).toString();

        q2.bindValue(":oid", data.id);
        q2.bindValue(":direction", Direction::from);
        if (!q2.exec() || !q2.next()) {
            qDebug() << q2.lastError();
            return list;
        }
        a1 = q2.value(0).toInt();
        s1 = q2.value(1).toDouble();

        q2.bindValue(":oid", data.id);
        q2.bindValue(":direction", Direction::to);
        if (!q2.exec() || !q2.next()) {
            qDebug() << q2.lastError();
            return list;
        }
        a2 = q2.value(0).toInt();
        s2 = q2.value(1).toDouble();

        if (a1 != account && a2 != account)
            continue;

        summ.set_account(a1);
        summ.set_balance(s1);
        data.from.append(summ);
        summ.set_account(a2);
        summ.set_balance(s2);
        data.to.append(summ);

        list.append(data);
    }

    return list;
}

void ListOperationsModel::reload_data(int account, QDate fdate, QDate ldate)
{
    current_account = account;
    list.clear();
    list = read_list(account, fdate, ldate);
    reset();
//    qDebug() << list.count();
}

QVariant ListOperationsModel::data(const QModelIndex &index, int role) const
{
//    QVariant value = QStandardItemModel::data(index, role);

    if (index.row() < 0 || index.row() >= list.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        operation_data data = list.at(index.row());
        account_summ from = data.from.at(0);
        account_summ to = data.to.at(0);
        if (index.column() == col_Id) {
            return data.id;
        }
        else if (index.column() == col_Account) {
//            qDebug() << current_account << from.account() << to.account();
            if (from.account() == current_account) {
//                qDebug() << "from";
                return accounts_list[to.account()];
            }
            else {
//                qDebug() << "to";
                return accounts_list[from.account()];
            }
        }
        else if (index.column() == col_Debet) {
            if (from.account() != current_account)
                return default_locale->toString(to.balance().value()/var->Kurs(),'f',2);
            else
                return QVariant();
        }
        else if (index.column() == col_Credit) {
            if (to.account() != current_account)
                return default_locale->toString(from.balance().value()/var->Kurs(),'f',2);
            else
                return QVariant();
        }
        else if (index.column() == col_Date) {
            return data.date.toString(Qt::SystemLocaleDate);
        }
        else if (index.column() == col_Descr) {
            return data.descr;
        }
        else
            return QVariant();
    }

    else if (role == Qt::TextAlignmentRole) {
            if (index.column() == col_Debet)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            else if (index.column() == col_Credit)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            else if (index.column() == col_Date)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            else
                return QVariant();
    }

    return QVariant();
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

QModelIndex ListOperationsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (row >= list.size() || row < 0 || column >= header_data.size())
        return QModelIndex();

    operation_data pkt = list.at(row);
    return createIndex(row, column, &pkt);
}

QModelIndex ListOperationsModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
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

    model = new ListOperationsModel(0, fdate, ldate);
    connect(this, SIGNAL(call_reload_table(int, QDate, QDate)), model, SLOT(reload_data(int, QDate, QDate)));

    debt = new QAction(tr("Debet"), this);
    debt->setShortcut(tr("Ctrl+N"));
    debt->setToolTip(tr("Debet operation"));

    cred = new QAction(tr("Credit"), this);
    cred->setShortcut(tr("Ctrl+C"));
    cred->setToolTip(tr("Credit operation"));

    tran = new QAction(tr("Transfer"), this);
    tran->setShortcut(tr("Ctrl+T"));
    tran->setToolTip(tr("Transfer operation"));

    plan = new QAction(tr("Planning operation"), this);
    plan->setShortcut(tr("Ctrl+P"));
    plan->setToolTip(tr("Planning operation"));

    dele = new QAction(tr("Delete current operation"), this);
    dele->setShortcut(tr("Ctrl+D"));
    dele->setText(tr("Delete operation"));

    acts.append(debt);
    acts.append(cred);
    acts.append(tran);
    acts.append(plan);
    acts.append(dele);

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
//    for (int i = 1; i < 5; i++)
//        ui->tableView->header()->setResizeMode(i, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    print_balance();

    ui->tableView->addActions(acts);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(debt, SIGNAL(triggered()), SLOT(debet_operation()));
    connect(cred, SIGNAL(triggered()), SLOT(credit_operation()));
    connect(tran, SIGNAL(triggered()), SLOT(transfer_operation()));
    connect(plan, SIGNAL(triggered()), SLOT(plann_operation()));
    connect(dele, SIGNAL(triggered()), SLOT(del_operation()));
    connect(ui->fdate, SIGNAL(dateChanged(QDate)), SLOT(reload_model()));
    connect(ui->ldate, SIGNAL(dateChanged(QDate)), SLOT(reload_model()));

//    connect(ui->listAccounts, SIGNAL(activated(QModelIndex)), SLOT(reload_model()));
    connect(ui->listAccounts->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(reload_model()));
}

ListOperations::~ListOperations()
{
    delete ui;
    delete db;
}

void ListOperations::edit_operation(operation_data &d)
{
    EditOperation eo(1, this);
    QModelIndex idx = ui->tableView->currentIndex();
    operation_data data;

    eo.setdata(d);
    if (eo.exec() == QDialog::Accepted) {
        data = eo.data();

        db->save_operation(data);
        ui->tableView->setCurrentIndex(idx);
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
    account_summ a;

    a.set_account(ui->listAccounts->value());
    d.from.append(a);

    edit_operation(d);
}

void ListOperations::transfer_operation()
{
    operation_data d;
    account_summ a;

    a.set_account(ui->listAccounts->value());
    d.from.append(a);

    edit_operation(d);
}

int ListOperations::get_selected_id()
{
    QModelIndexList list;

    list = ui->tableView->selectionModel()->selectedIndexes();
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

void ListOperations::print_balance()
{
    Globals var;
//    QLocale *lc;
    QFont font;
    font.setBold(true);
    int id = ui->listAccounts->value();
    Account_Data data = db->get_account_data(id);
    double result = db->convert_currency(data.balance.value(), data.curr);

//    lc = default_locale;
    ui->account_ostatok->setFont(font);
    ui->account_ostatok->setText(default_locale->toString(result,'f',2));

    var.setAccount(ui->listAccounts->value());
}

void ListOperations::reload_model()
{
    QDate fdate, ldate;
    int id = ui->listAccounts->value();

    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    print_balance();

//    ui->tableView->hideColumn(0);
//    ui->tableView->setAlternatingRowColors(true);
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
//    for (int i = 1; i < 5; i++)
//        ui->tableView->header()->setResizeMode(i, QHeaderView::ResizeToContents);

    emit call_reload_table(id, fdate, ldate);
}

void ListOperations::clear_model()
{
    ui->account_ostatok->setText("0");
//    model->clear();
}
