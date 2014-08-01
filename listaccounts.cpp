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
    ui->splitter->setStretchFactor(2,1);

//    db = new Database;
    db = d;

//    type = ui->typeComboBox->value();

//    query = "SELECT a.id,a.name,t.name,a.balance,a.descr,a.hidden FROM account a, account_type t WHERE a.type = t.id AND a.type = " + QString("%1").arg(type) + " ORDER BY type,a.name";

//    fill_model();
    model = new ListAccountsModel;
    model->fill_model();

    ui->treeView->setItemDelegate(new ViewCurrency(1));
    ui->treeView->setModel(model);
//    ui->treeView->hideColumn(4);
    ui->treeView->hideColumn(5);
    for (int i = 0; i < 5; i++)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);

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

//    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(check_type()));

    ui->act_summ->setText(active.text());
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
    ui->treeView->hideColumn(5);
    for (int i = 0; i < 5; i++)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);
}

void ListAccounts::new_account()
{
    Globals var;
    EditAccount *ea = new EditAccount(1, this);
    int id = get_selected_id();
    Account_Data data;
    operation_data oper;
    account_summ a;
    int acc;

    data.curr = var.Currency();
    data.parent = id;
    ea->setData(data);

    if (ea->exec() == QDialog::Accepted) {
        data = ea->data();
        Account_Data parent = db->get_account_data(data.parent);
        Transaction t;

//        qDebug() << data.top << parent.top;

        t.begin();

        if (data.name.length() == 0) {
            t.rollback();
            return;
        }

        data.top = parent.top;
        acc = db->new_account(data);
        if (acc == 0) {
            t.rollback();
            return;
        }

        if (data.balance.value() == 0) {
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

        oper.date = data.dt;
        oper.descr = tr("Primary balance");

        a.set_account(var.InitialAccount());
        a.set_balance(data.balance.value());
        oper.from.append(a);

        a.set_account(acc);
        a.set_balance(data.balance.value());
        oper.to.append(a);

        db->save_operation(oper);
        t.commit();

        reload_model();
    }
}

void ListAccounts::change_account()
{
    QSqlQuery q;
    EditAccount *ac = new EditAccount(1, this);
    Account_Data data;
    int id = get_selected_id();

    if (id == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return;
    }

    data = db->get_account_data(id);
    ac->setData(data);
    if (ac->exec() != QDialog::Accepted)
        return;

    data = ac->data();

    q.prepare("UPDATE account SET name=:name, type=:type, descr=:descr, ccod=:ccod, hidden=:hidden, parent=:parent WHERE id=:id");
    q.bindValue(":name", data.name);
    q.bindValue(":type", data.type);
    q.bindValue(":descr", data.descr);
    q.bindValue(":ccod", data.curr);
    q.bindValue(":hidden", (data.hidden == false) ? 0 : 1);
    q.bindValue(":parent", data.parent);
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

     return list.at(5).data(Qt::DisplayRole).toInt();
}

void ListAccounts::correct_balance()
{
    CorrectBalance *cb = new CorrectBalance(this);
    int id = get_selected_id();
    Account_Data data;
    account_summ a;

    if (id == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return;
    }

    data = db->get_account_data(id);
    if (data.top != 1 && data.top != 2) {
	QMessageBox::critical(this, tr("Account"),
                                 tr("You can correct balans only active or passive accounts"));
	return;
    }

    cb->setBalance(data.balance.value());
    if (cb->exec() == QDialog::Accepted) {
        double current_balance = data.balance.value();
        double new_balance = cb->balance();

        operation_data od;
        od.date = cb->date();
        od.agent = 0;
        od.descr = tr("correct");
        if (new_balance < current_balance) {
            a.set_account(id);
            a.set_balance(current_balance-new_balance);
            od.from.append(a);
            a.set_account(cb->account());
            a.set_balance(current_balance-new_balance);
            od.to.append(a);
            db->save_operation(od);
        }
        else {
            a.set_account(cb->account());
            a.set_balance(new_balance-current_balance);
            od.from.append(a);
            a.set_account(id);
            a.set_balance(new_balance-current_balance);
            od.to.append(a);
            db->save_operation(od);
        }

        reload_model();
    }
}

void ListAccounts::del_account()
{
    QSqlQuery q;
    int id = get_selected_id();
    Account_Data data;

    if (id == 0) {
        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return;
    }

    data = db->get_account_data(id);

    if (data.system == 1) {
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
    QSqlQuery q, q2;
    int id = get_selected_id();
    Account_Data data;
    QList<SbD> stack;
    QList<SbD> summ;
//    QList<SbD> result;
    QMap<QDate,double> result;

    if (id == 0)
        return;

    data = db->get_account_data(id);

    ui->textEdit->clear();
    ui->textEdit->append(QString("%1").arg(id));

    // выборка значений операций по счету
    q.prepare("SELECT id,dt FROM oper ORDER BY dt");
    if (!q.exec()) {
        qDebug() << q.lastError();
        return;
    }
    while (q.next()) {
        q2.prepare("SELECT summ, direction FROM account_oper WHERE o_id = :oid AND a_id = :aid");
        q2.bindValue(":oid", q.value(0).toInt());
        q2.bindValue(":aid", id);
        if (!q2.exec()) {
            qDebug() << q2.lastError();
            return;
        }
        while (q2.next()) {
            SbD val;
            if (q2.value(1).toInt() == 2)
                val.value = -q2.value(0).toDouble();
            else
                val.value = q2.value(0).toDouble();
            val.dt = q.value(1).toDate();
//            qDebug() << val.dt << val.value;
            stack.append(val);
        }
    }

    // расчет баланса по счету на момент совершения операции
    // итоговые значения расположены в массиве result
    SbD val;
    val.value = data.balance.value();
    val.dt = QDate::currentDate();
    qDebug() << val.dt << val.value;
    summ.append(val);
    for (int j = stack.size()-1; j >= 0; j--) {
        SbD v = stack.at(j);
        val.value += v.value;
        val.dt = v.dt;
//        qDebug() << val.dt << val.value;
        summ.append(val);
    }

    for (int j = summ.size()-1; j > 0; j--) {
//        SbD v;
//        val = summ.at(j);
        val.dt = summ.at(j).dt;
        val.value = summ.at(j-1).value;
        result[val.dt] = val.value;
    }
    qDebug() << result;
}
