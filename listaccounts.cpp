#include "listaccounts.h"
#include "ui_listaccounts.h"
#include "editaccount.h"
#include "correctbalance.h"

ListAccounts::ListAccounts(Database *d, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
//    int type;
    QFont font, fnt;
    summAccount active(Account_Type::active);

    font.setBold(true);

    ui->setupUi(this);
    ui->verticalLayout->setStretch(1,1);
    ui->splitter->setStretchFactor(0,0);

//    db = new Database;
    db = d;

//    type = ui->typeComboBox->value();

//    query = "SELECT a.id,a.name,t.name,a.balance,a.descr,a.hidden FROM account a, account_type t WHERE a.type = t.id AND a.type = " + QString("%1").arg(type) + " ORDER BY type,a.name";

//    fill_model();
    model = new ListAccountsModel;
    model->fill_model();

    ui->treeView->setItemDelegate(new ViewCurrency(1));
    ui->treeView->setModel(model);
//    ui->treeView->expandAll();
//    ui->treeView->hideColumn(4);
    ui->treeView->hideColumn(6);
//    ui->treeView->header()->ResizeToContents;
    for (int i = 0; i < 5; i++)
        ui->treeView->resizeColumnToContents(i);
//        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);

    if (var.ListFont().length() > 0)
        fnt.setFamily(var.ListFont());
    ui->treeView->setFont(fnt);

    nacct = new QAction(tr("New"), this);
    nacct->setShortcut(tr("Alt+N"));
    nacct->setToolTip("New account");
    connect(nacct, SIGNAL(triggered()), SLOT(new_account()));

    chacc = new QAction(tr("Change"), this);
    chacc->setShortcut(tr("Alt+E"));
    chacc->setToolTip("Change account");
    connect(chacc, SIGNAL(triggered()), SLOT(change_account()));

    cacct = new QAction(tr("Correct"), this);
    cacct->setShortcut(tr("Alt+C"));
    cacct->setToolTip("Correct balance");
    connect(cacct, SIGNAL(triggered()), SLOT(correct_balance()));

    dacct = new QAction(tr("Delete"), this);
    dacct->setShortcut(tr("Alt+D"));
    dacct->setToolTip("Delete this account");
    connect(dacct, SIGNAL(triggered()), SLOT(del_account()));

    analis = new QAction(tr("Analis"), this);
    analis->setToolTip(tr("Analis linear approximate"));
    connect(analis, SIGNAL(triggered()), SLOT(show_analis()));

    acts.append(nacct);
    acts.append(chacc);
    acts.append(cacct);
    acts.append(dacct);
    acts.append(analis);
    ui->treeView->addActions(acts);

    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->treeView->setAlternatingRowColors(true);
    
    connect(ui->treeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(select_account()));

    connect(this, SIGNAL(pressInsert()), SLOT(new_account()));
    connect(this, SIGNAL(pressEnter()), SLOT(change_account()));
    connect(this, SIGNAL(pressDelete()), SLOT(del_account()));

    connect(this, SIGNAL(select_account(int)), ui->widget, SLOT(calc_array(int)));

//    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(check_type()));

    ui->act_summ->setText(default_locale->toCurrencyString(active.Value().toDouble()));
    ui->act_summ->setFont(font);
}

ListAccounts::~ListAccounts()
{
//    delete db;
    delete ui;
    delete model;
}

void ListAccounts::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Insert:
        emit pressInsert();
        break;
    case Qt::Key_Delete:
        emit pressDelete();
        break;
    case Qt::Key_Enter:
        emit pressEnter();
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void ListAccounts::clear_model()
{
    ui->act_summ->setText(default_locale->toCurrencyString(0));
    model->clear();
}

void ListAccounts::reload_model()
{
    summAccount active(1);

    model->fill_model();
    ui->treeView->expandAll();
    ui->act_summ->setText(active.text());
    ui->treeView->hideColumn(6);
//    for (int i = 0; i < 6; i++)
//        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);
}

void ListAccounts::new_account()
{
    Globals var;
    EditAccount *ea = new EditAccount(1, this);
    int id = get_selected_id();
    Account data, d;
    Operation oper;
    account_summ a;
    QList<account_summ> lst;
    int acc;

    data.setCurr(var.Currency());
    data.setParent(id);
    ea->setData(data);

    if (ea->exec() == QDialog::Accepted) {
        data = ea->data();
        Account parent;
        Transaction t;

//        qDebug() << data.top << parent.top;

        t.begin();

        parent.read(data.Parent());
        if (data.Name().length() == 0) {
            t.rollback();
            return;
        }

        data.setTop(parent.Top());
        acc = data.insert();
        if (acc == 0) {
            t.rollback();
            return;
        }

        if (data.Balance().toDouble() == 0) {
            t.commit();
            reload_model();
            return;
        }

        if (var.InitialAccount() == 0) {
            QMessageBox::critical(this, tr("Account"),
                                         tr("You can not create new account wich initial balance\n\
                                            You must set initial account over settings."));
            t.rollback();
            return;
        }

        oper.setDate(data.Date());
        oper.setDescr(tr("Primary balance"));

        d.read(var.InitialAccount());
        a.set_account(d);
        a.set_balance(data.Balance().toDouble());
        lst.clear();
        lst.append(a);
        oper.setFrom(lst);

        d.read(acc);
        a.set_account(d);
        a.set_balance(data.Balance().toDouble());
        lst.clear();
        lst.append(a);
        oper.setTo(lst);

        oper.save_operation();
        t.commit();

        reload_model();
    }
}

void ListAccounts::change_account()
{
    QSqlQuery q;
    EditAccount *ac = new EditAccount(1, this);
    Account data;
    int id = get_selected_id();

    if (id == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return;
    }

    data.read(id);
    ac->setData(data);
    if (ac->exec() != QDialog::Accepted)
        return;

    data = ac->data();

    q.prepare("UPDATE account SET name=:name, type=:type, descr=:descr, ccod=:ccod, hidden=:hidden, parent=:parent WHERE id=:id");
    q.bindValue(":name", data.Name());
    q.bindValue(":type", data.Type());
    q.bindValue(":descr", data.Descr());
    q.bindValue(":ccod", data.Curr());
    q.bindValue(":hidden", (data.Hidden() == false) ? 0 : 1);
    q.bindValue(":parent", data.Parent());
    q.bindValue(":id", id);
    if (!q.exec())
        qDebug() << q.lastError().text();

    reload_model();
}

int ListAccounts::get_selected_id()
{
    QModelIndexList list;

    list = ui->treeView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        return 0;
    }

     return list.at(6).data(Qt::DisplayRole).toInt();
}

void ListAccounts::correct_balance()
{
    CorrectBalance *cb = new CorrectBalance(this);
    int id = get_selected_id();
    Account data;
    account_summ a;

    if (id == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return;
    }

    data.read(id);
    if (data.Top() != 1 && data.Top() != 2) {
	QMessageBox::critical(this, tr("Account"),
                                 tr("You can correct balans only active or passive accounts"));
	return;
    }

    cb->setBalance(data.Balance().toDouble());
    if (cb->exec() == QDialog::Accepted) {
        MyCurrency current_balance = data.Balance();
        MyCurrency new_balance = cb->balance();

        Operation od;
        QList<account_summ> lst;
        od.setDate(cb->date());
//        od.agent = 0;
        od.setDescr(tr("correct"));
        if (new_balance < current_balance) {
            double summ = current_balance-new_balance;
            Account d;

            d.read(id);
            a.set_account(d);
            a.set_balance(summ);
            lst.clear();
            lst.append(a);
            od.setFrom(lst);

            d.read(cb->account());
            a.set_account(d);
            a.set_balance(summ);
            lst.clear();
            lst.append(a);
            od.setTo(lst);
        }
        else {
            double summ = new_balance-current_balance;
            Account d;

            d.read(cb->account());
            a.set_account(d);
            a.set_balance(summ);
            lst.clear();
            lst.append(a);
            od.setFrom(lst);

            d.read(id);
            a.set_account(d);
            a.set_balance(summ);
            lst.clear();
            lst.append(a);
            od.setTo(lst);
        }
        od.save_operation();

        reload_model();
    }
}

void ListAccounts::del_account()
{
    QSqlQuery q;
    int id = get_selected_id();
    Account data;

    if (id == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return;
    }

    data.read(id);

    if (data.System() == 1) {
        QMessageBox::critical(this, tr("Account"),
                                     tr("You can't delete system account"));
        return;
    }

    int r = QMessageBox::warning(this, tr("Account"),
                                 tr("You want to delete account?"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (r == QMessageBox::No)
        return;

    q.prepare("SELECT count(id) FROM account_oper WHERE a_id = :id");
    q.bindValue(":id", id);
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

void ListAccounts::show_analis()
{
    QDialog *w = new QDialog(this);
    int id = get_selected_id();

    if (id == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return;
    }

    w->exec();
}

//
// Анализ временного ряда значений баланса на момент совершения операции прихода, расхода
//
void ListAccounts::select_account()
{
    int id = get_selected_id();
    emit select_account(id);
}
