#include "listoperations.h"
#include "ui_listoperations.h"
#include "global.h"

ListOperationsModel::ListOperationsModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    db = new Database;
}

ListOperationsModel::~ListOperationsModel()
{
    delete db;
}

QVariant ListOperationsModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 0) {
            return db->get_account_name(value.toInt());
//            return value;
        }
        else if (index.column() == 1) {
            return db->get_account_name(value.toInt());
//            return value;
        }
        if (index.column() == 2) {
            return tr("%1").arg(value.toDouble(), 0, 'f', 2);
        }
        else if (index.column() == 3) {
            return value.toDate().toString("dddd dd MMMM yyyy");
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 2)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 3)
                return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    return value;
}

ListOperations::ListOperations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListOperations)
{
    QString fdate, ldate;

    ui->setupUi(this);

    db = new Database;

    ui->accountcomboBox->load(1);
    ui->accountcomboBox->setValue(current_account);
    change_current_account(0);
    ui->fdate->setDate(QDate().currentDate().addDays(-30));
    fdate = ui->fdate->value();
    ldate = ui->ldate->value();

    query = "SELECT acc_from, acc_to, summ, dt, descr FROM operation WHERE dt >= '" + fdate + "' AND dt <= '" + ldate + "' ORDER BY dt";

    model = new ListOperationsModel;
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, tr("From Account"));
    model->setHeaderData(1, Qt::Horizontal, tr("To Account"));
    model->setHeaderData(2, Qt::Horizontal, tr("Summ"));
    model->setHeaderData(3, Qt::Horizontal, tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, tr("Description"));

    QAction *debt = new QAction(tr("Debet"), this);
    QAction *cred = new QAction(tr("Credit"), this);
    QAction *tran = new QAction(tr("Transfer"), this);
    QAction *font = new QAction(tr("Select font"), this);

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setShowGrid(false);

    ui->tableView->addAction(debt);
    ui->tableView->addAction(cred);
    ui->tableView->addAction(tran);
    ui->tableView->addAction(font);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    change_current_account(0);

    connect(debt, SIGNAL(triggered()), SLOT(debet_operation()));
    connect(cred, SIGNAL(triggered()), SLOT(credit_operation()));
    connect(tran, SIGNAL(triggered()), SLOT(transfer_operation()));
    connect(font, SIGNAL(triggered()), SLOT(select_font()));
    connect(ui->accountcomboBox, SIGNAL(currentIndexChanged(int)), SLOT(change_current_account(int)));
    connect(ui->search_comboBox, SIGNAL(currentIndexChanged(int)), SLOT(select_list_operations()));
    connect(ui->fdate, SIGNAL(dateChanged(QDate)), SLOT(select_list_operations()));
    connect(ui->ldate, SIGNAL(dateChanged(QDate)), SLOT(select_list_operations()));
}

ListOperations::~ListOperations()
{
    delete ui;
    delete db;
}

void ListOperations::edit_operation(int oper)
{
    if (eo.exec() == QDialog::Accepted) {
        eo.data(d);
        db->save_operation(d.from, d.to, d.summ, d.date, d.descr);
        model->setQuery(query);
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
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
    ui->account_ostatok->setText(tr("%1").arg(db->get_account_balance(ui->accountcomboBox->value()), 0, 'f', 2));
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
}

void ListOperations::select_font()
{
    bool bOk;

    QFont fnt = QFontDialog::getFont(&bOk);
    if (bOk) {
        ui->tableView->setFont(fnt);
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
    }
}
