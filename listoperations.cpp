#include "listoperations.h"
#include "ui_listoperations.h"
#include "global.h"

ListOperationsModel::ListOperationsModel(int account, QDate fdate, QDate ldate, QObject *parent) :
    QAbstractTableModel(parent)
{
    header_data << "" << tr("Date") << tr("Account") << tr("Debet") << tr("Credit") << tr("Description");
    list = read_list(account, fdate, ldate);
//    accounts_list = db->get_accounts_list();
    var = new Globals;
}

ListOperationsModel::~ListOperationsModel()
{
//    delete db;
    delete var;
}

QList<Operation> ListOperationsModel::read_list(int account, QDate fdate, QDate ldate)
{
    QSqlQuery q1, q2;
    QList<Operation> list;
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
        Operation data;
        data.setId(q1.value(0).toInt());
        data.setDate(q1.value(1).toDate());
        data.setDescr(q1.value(2).toString());

        a1 = 0;
        s1 = 0;
        q2.bindValue(":oid", data.Id());
        q2.bindValue(":direct", Direction::from);
        if (!q2.exec()) {
            qDebug() << q2.lastError();
            return list;
        }
        if (q2.next()) {
            a1 = q2.value(0).toInt();
            s1 = q2.value(1).toDouble();
        }

        a2 = 0;
        s2 = 0;
        q2.bindValue(":oid", data.Id());
        q2.bindValue(":direct", Direction::to);
        if (!q2.exec()) {
            qDebug() << q2.lastError();
            return list;
        }
        if (q2.next()) {
            a2 = q2.value(0).toInt();
            s2 = q2.value(1).toDouble();
        }

        if (a1 != account && a2 != account)
            continue;

        Account a;
        QList<account_summ> lst;

        a.read(a1);
        summ.set_account(a);
        summ.set_balance(s1);
        lst.clear();
        lst.append(summ);
        data.setFrom(lst);

        a.read(a2);
        summ.set_account(a);
        summ.set_balance(s2);
        lst.clear();
        lst.append(summ);
        data.setTo(lst);

        list.append(data);
    }

//    qDebug() << list.size();
    return list;
}

void ListOperationsModel::reload_data(int account, QDate fdate, QDate ldate)
{
    current_account = account;
    beginResetModel();
    list.clear();
    list = read_list(account, fdate, ldate);
    endResetModel();
}

QVariant ListOperationsModel::data(const QModelIndex &index, int role) const
{
//    QVariant value = QStandardItemModel::data(index, role);

    if (index.row() < 0 || index.row() >= list.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        Operation data = list.at(index.row());
        account_summ from = data.From().at(0);
        account_summ to = data.To().at(0);
        if (index.column() == col_Id) {
            return data.Id();
        }
        else if (index.column() == col_Account) {
//            qDebug() << current_account << from.account() << to.account();
            if (from.account().Id() == current_account) {
//                qDebug() << "from";
                return to.account().fullName();
            }
            else {
//                qDebug() << "to";
                return from.account().fullName();
            }
        }
        else if (index.column() == col_Debet) {
            qDebug() << current_account << from.account().Id();
            if (from.account().Id() != current_account)
                return default_locale->toString(to.balance().toDouble()/Currency(var->Currency()).Kurs(),'f',2);
            else
                return QVariant();
        }
        else if (index.column() == col_Credit) {
            qDebug() << current_account << to.account().Id();
            if (to.account().Id() != current_account)
                return default_locale->toString(from.balance().toDouble()/Currency(var->Currency()).Kurs(),'f',2);
            else
                return QVariant();
        }
        else if (index.column() == col_Date) {
            return data.Date().toString(Qt::SystemLocaleDate);
        }
        else if (index.column() == col_Descr) {
            return data.Descr();
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

/*
QModelIndex ListOperationsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (row >= list.size() || row < 0 || column >= header_data.size())
        return QModelIndex();

    Operation_Data pkt = list.at(row);
    return createIndex(row, column, &pkt);
}

QModelIndex ListOperationsModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}
*/

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

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);

    debt = new QAction(tr("Debet"), this);
    debt->setShortcut(tr("Ctrl+N"));
    debt->setToolTip(tr("Debet operation"));
    connect(debt, SIGNAL(triggered()), SLOT(debet_operation()));
    acts.append(debt);

    cred = new QAction(tr("Credit"), this);
    cred->setShortcut(tr("Ctrl+C"));
    cred->setToolTip(tr("Credit operation"));
    connect(cred, SIGNAL(triggered()), SLOT(credit_operation()));
    acts.append(cred);

    tran = new QAction(tr("Transfer"), this);
    tran->setShortcut(tr("Ctrl+T"));
    tran->setToolTip(tr("Transfer operation"));
    connect(tran, SIGNAL(triggered()), SLOT(transfer_operation()));
    acts.append(tran);

    repe = new QAction(tr("Repeat"), this);
    repe->setShortcut(tr("Ctrl+R"));
    repe->setToolTip(tr("Repeat operation"));
    connect(repe, SIGNAL(triggered()), SLOT(repeat_operation()));
    acts.append(repe);

    plan = new QAction(tr("Planning operation"), this);
    plan->setShortcut(tr("Ctrl+P"));
    plan->setToolTip(tr("Planning operation"));
    connect(plan, SIGNAL(triggered()), SLOT(plann_operation()));
    acts.append(plan);

    dele = new QAction(tr("Delete current operation"), this);
    dele->setShortcut(tr("Ctrl+D"));
    dele->setText(tr("Delete operation"));
    connect(dele, SIGNAL(triggered()), SLOT(del_operation()));
    acts.append(dele);

    ui->tableView->addActions(acts);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(ui->fdate, SIGNAL(dateChanged(QDate)), SLOT(reload_model()));
    connect(ui->ldate, SIGNAL(dateChanged(QDate)), SLOT(reload_model()));

//    connect(ui->listAccounts, SIGNAL(activated(QModelIndex)), SLOT(reload_model()));
    connect(ui->listAccounts->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(reload_model()));

    print_balance();
}

ListOperations::~ListOperations()
{
    acts.clear();
    delete dele;
    delete plan;
    delete repe;
    delete tran;
    delete cred;
    delete debt;

    delete ui;
    delete model;
    delete db;
}

void ListOperations::edit_operation(Operation &d)
{
    RegOperation eo(this);
    EditPayment ep;
    QModelIndex idx = ui->tableView->currentIndex();
    Operation data;

    ep.exec();

    eo.setData(d);
    if (eo.exec() == QDialog::Accepted) {
        data = eo.data();

        data.insert();
        ui->tableView->setCurrentIndex(idx);
        reload_model();
    }
}

void ListOperations::debet_operation()
{
    Account a;
    Operation d;
    account_summ s;
    QList<account_summ> lst;

    a.read(ui->listAccounts->value());
    s.set_account(a);
    lst.clear();
    lst.append(s);
    d.setTo(lst);

    edit_operation(d);
}

void ListOperations::credit_operation()
{
    Account a;
    Operation d;
    account_summ s;
    QList<account_summ> lst;

    a.read(ui->listAccounts->value());
    s.set_account(a);
    lst.clear();
    lst.append(s);
    d.setFrom(lst);

    edit_operation(d);
}

void ListOperations::transfer_operation()
{
    Operation d;
    account_summ s;
    Account a;
    QList<account_summ> lst;

    a.read(ui->listAccounts->value());
    s.set_account(a);
    lst.clear();
    lst.append(s);
    d.setFrom(lst);

    edit_operation(d);
}

void ListOperations::repeat_operation()
{
    Operation oper;
    RegOperation pd(this);
    int id = ui->tableView->get_selected_id();

    if (id == 0)
        return;

    oper.read(id);
//    oper.setDate(QDate::currentDate());
    edit_operation(oper);
}

void ListOperations::del_operation()
{
    QSqlQuery q;
    Operation data;
    int id = ui->tableView->get_selected_id();

    if (id == 0)
        return;

    int r = QMessageBox::warning(this, tr("Operation"),
                                 tr("You want to delete operation?\n" \
                 "Nadeusj, vi ponimaete chto delaeye,\n" \
                 "tak kak v etom sluchae budut izmeneni balansi accounts"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (r == QMessageBox::No)
        return;

    data.del_operation(id);

    reload_model();

    return;
}

void ListOperations::plann_operation()
{
    EditPlanOperation pd(this);
    int id = ui->tableView->get_selected_id();

    if (id == 0)
        return;

    PlanOperation data;
    data.read(id);
    data.setDay(QDate::currentDate().day());

    pd.setData(data);
    if (pd.exec() == QDialog::Accepted) {
        data = pd.Data();
        int plan = data.insert();
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

    ui->account_ostatok->setFont(font);
    if (id)
        ui->account_ostatok->setText(default_locale->toCurrencyString(db->get_account_summ(id)));
    else
        ui->account_ostatok->clear();

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
