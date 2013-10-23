#include "listoperations.h"
#include "ui_listoperations.h"
#include "global.h"

ListOperationsModel::ListOperationsModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    db = new Database;
    header_data << "" << tr("From Account") << tr("To Account") << tr("Summ") << tr("Date") << tr("Description");
    list = db->get_accounts_list();
}

ListOperationsModel::~ListOperationsModel()
{
    delete db;
}

QVariant ListOperationsModel::data(const QModelIndex &index, int role) const
{
    QDate curr = QDate::currentDate();
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 1 || index.column() == 2) {
//            return db->get_account_name(value.toInt());
            return list[value.toInt()];
        }
//        else if (index.column() == 1) {
//            return db->get_account_name(value.toInt());
//        }
        else if (index.column() == 3) {
            return default_locale->toString(value.toDouble());
        }
        else if (index.column() == 4) {
//            return value.toDate().toString("dddd dd MMMM yyyy");
            QDate dt = value.toDate();
            if (dt.daysTo(curr) < 7)
                return value.toDate().toString(Qt::SystemLocaleLongDate);
            else
                return value.toDate().toString(Qt::SystemLocaleDate);
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 3)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 4)
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
    QString fdate, ldate;

    ui->setupUi(this);

    d.agent = 0;

    db = new Database;

    ui->accountcomboBox->load(1);
    ui->accountcomboBox->setValue(current_account);
    change_current_account(0);
    ui->fdate->setDate(QDate().currentDate().addDays(-29));
    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    query = "SELECT id, acc_from, acc_to, summ, dt, descr FROM operation WHERE dt >= '" + fdate + "' AND dt <= '" + ldate + "' ORDER BY dt";

    model = new ListOperationsModel;
    model->setQuery(query);

    QAction *debt = new QAction(tr("Debet"), this);
    QAction *cred = new QAction(tr("Credit"), this);
    QAction *tran = new QAction(tr("Transfer"), this);
    QAction *dele = new QAction(tr("Delete current operation"), this);
    QAction *font = new QAction(tr("Select font"), this);

    ui->treeView->setModel(model);
//    ui->treeView->resizeRowsToContents();
//    ui->treeView->resizeColumnsToContents();
    ui->treeView->setAlternatingRowColors(true);
//    ui->treeView->horizontalHeader()->setStretchLastSection(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->treeView->setShowGrid(false);

    ui->treeView->addAction(debt);
    ui->treeView->addAction(cred);
    ui->treeView->addAction(tran);
    ui->treeView->addAction(dele);
    ui->treeView->addAction(font);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    change_current_account(0);

    connect(debt, SIGNAL(triggered()), SLOT(debet_operation()));
    connect(cred, SIGNAL(triggered()), SLOT(credit_operation()));
    connect(tran, SIGNAL(triggered()), SLOT(transfer_operation()));
    connect(dele, SIGNAL(triggered()), SLOT(del_operation()));
    connect(font, SIGNAL(triggered()), SLOT(select_font()));
    connect(ui->accountcomboBox, SIGNAL(currentIndexChanged(int)), SLOT(change_current_account(int)));
    connect(ui->search_comboBox, SIGNAL(currentIndexChanged(int)), SLOT(select_list_operations()));
    connect(ui->fdate, SIGNAL(dateChanged(QDate)), SLOT(select_list_operations()));
    connect(ui->ldate, SIGNAL(dateChanged(QDate)), SLOT(select_list_operations()));
    connect(this, SIGNAL(call_reload_table()), SLOT(reload_table()));
}

ListOperations::~ListOperations()
{
    delete ui;
    delete db;
}

void ListOperations::edit_operation(int oper)
{
    QModelIndex idx = ui->treeView->currentIndex();

    if (eo.exec() == QDialog::Accepted) {
        eo.data(d);

        db->save_operation(d);
        model->setQuery(query);
        ui->treeView->setCurrentIndex(idx);
        emit call_reload_table();
        change_current_account(0);
        if (oper == 1)
            ui->accountcomboBox->setValue(d.from);
        else
    	    ui->accountcomboBox->setValue(d.to);
    }

    current_account = ui->accountcomboBox->value();
}

void ListOperations::debet_operation()
{
    d.from = 0;
    d.to = ui->accountcomboBox->value();
    d.summ = 0;
    eo.setdata(d);

    edit_operation(1);
}

void ListOperations::credit_operation()
{
    d.from = ui->accountcomboBox->value();
    d.to = 0;
    d.summ = 0;
    eo.setdata(d);

    edit_operation(2);
}

void ListOperations::transfer_operation()
{
    d.from = ui->accountcomboBox->value();
    d.to = 0;
    d.summ = 0;
    eo.setdata(d);

    edit_operation(2);
}

void ListOperations::change_current_account(int idx)
{
    QLocale *lc;
    QFont font;
    font.setBold(true);
    int id = ui->accountcomboBox->value();
    Account_Data data = db->get_account_data(id);
    QString scod = db->get_account_scod(id);

    if (scod == "USD") {
        ui->account_ostatok->setText(scod);
        lc = new QLocale(QLocale::English);
    }
    else if (scod == "EUR") {
        ui->account_ostatok->setText(scod);
        lc = new QLocale(QLocale::German);
    }
    else
        lc = default_locale;
    ui->account_ostatok->setText(lc->toCurrencyString(data.balance));
    ui->account_ostatok->setFont(font);

    current_account = ui->accountcomboBox->value();
}

void ListOperations::select_list_operations()
{
    int id;
    QString fdate, ldate;

    id = ui->search_comboBox->value();
    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    if (id > 0) {
        query = "SELECT acc_from, acc_to, summ, dt, descr FROM operation WHERE (acc_from = " + QString("%1").arg(id) + " OR acc_to = " + QString("%1").arg(id) + ") AND dt >= '" + fdate + "' AND dt <= '" + ldate + "' ORDER BY dt";
    }
    else
        query = "SELECT acc_from, acc_to, summ, dt, descr FROM operation WHERE dt >= '" + fdate + "' AND dt <= '" + ldate + "' ORDER BY dt";

    model->setQuery(query);
    emit call_reload_table();
}

void ListOperations::select_font()
{
    bool bOk;

    fnt = QFontDialog::getFont(&bOk);
    if (bOk) {
        ui->treeView->setFont(fnt);
        emit call_reload_table();
    }
}

void ListOperations::reload_table()
{
//    ui->treeView->resizeRowsToContents();
//    ui->treeView->resizeColumnsToContents();
//    ui->treeView->horizontalHeader()->setStretchLastSection(true);
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

    q.exec("BEGIN");

    data = db->get_operation(id);

    if (db->change_account_balance(data.from, data.summ) == false) {
        q.exec("ROLLBACK TRANSACTION");
        return;
    }
    if (db->change_account_balance(data.to, -data.summ) == false) {
        q.exec("ROLLBACK TRANSACTION");
        return;
    }
    if (db->del_account_oper(id) == false) {
        q.exec("ROLLBACK TRANSACTION");
        return;
    }

    q.exec("COMMIT");

    return;
}
