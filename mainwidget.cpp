#include "mainwidget.h"
#include "widgets/mydateedit.h"
#include "ui_mainwidget.h"

void MainWidgetModel::fill_model(Database *db)
{
    QList<operation_data>::iterator i;
    QList<account_summ>::iterator j;
    account_summ a;
    operation_data data;
    int row = 0;

    clear();

    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Status") << tr("Descr");

    if (!var.database_Opened())
        return;

    accounts = db->get_accounts_list();

    insertColumns(0,9);
    for (i = list.begin(); i != list.end(); i++) {
        data = *i;
        if (data.status == Plan_Status::actual)
            continue;
        insertRow(row);
        setData(index(row,id),    data.id);
        setData(index(row,day),   data.day);
        setData(index(row,month), data.month);
        setData(index(row,year),  data.year);
        setData(index(row,column_from),  accounts[data.from.at(0).account()]);
        j = data.to.begin();
        a = *j;
        setData(index(row,column_to),    accounts[a.account()]);
        setData(index(row,summ),  default_locale->toString(data.from.at(0).balance().value(),'f',2));
        if (data.status == Plan_Status::actual)
            setData(index(row,status), tr("Actual"));
        else if (data.status == Plan_Status::committed)
            setData(index(row,status), tr("Committed"));
        else if (data.status == Plan_Status::expired)
            setData(index(row,status), tr("Expired"));
        else if (data.status == Plan_Status::minimum)
            setData(index(row,status), tr("< 3 days"));
        else
            setData(index(row,status), data.status);
        setData(index(row,descr), data.descr);
        row++;
    }
}

MainWidgetModel::MainWidgetModel(Database *db, QObject *parent) :
    QStandardItemModel(parent)
{
//    db = new Database;
    list = db->get_plan_oper_list();
//    fill_model();
}

MainWidgetModel::~MainWidgetModel()
{
//    delete db;
}

QVariant MainWidgetModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

QVariant MainWidgetModel::data(const QModelIndex &index, int role) const
{
//    QDate curr = QDate::currentDate();
    QVariant value = QStandardItemModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
//        if (index.column() == 2 || index.column() == 3) {
//            return (value.toInt() == 0) ? "" : value;
//        }
//        if (index.column() == 4 || index.column() == 5) {
//            return list[value.toInt()];
//        }
//        else if (index.column() == 6) {
//            return default_locale->toString(value.toDouble());
//        }
//        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == day)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == month)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == year)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == summ)
                return int(Qt::AlignRight | Qt::AlignVCenter);

//        case Qt::TextColorRole:
//            if (record(index.row()).value(0).toInt() == current_currency) {
//                return QVariant(QColor(Qt::red));
//            }
//            else
//                return value;
    }

    return value;
}

MainWidget::MainWidget(Database *d, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    summAccount active(Account_Type::active), passive(Account_Type::passive);

    ui->setupUi(this);

//    db = new Database;
    db = d;
    model = new MainWidgetModel(db);
    model->fill_model(db);

    ui->groupBox->setTitle(tr("Balance status"));
    ui->groupBox_2->setTitle(tr("Prosrochennie operations"));

//    reload_model();

    ui->active_value->setText(active.text());
    ui->passive_value->setText(passive.text());

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
    ui->treeView->header()->setResizeMode(model->day, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->month, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->year, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->column_from, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->column_to, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->summ, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->status, QHeaderView::ResizeToContents);
}

MainWidget::~MainWidget()
{
    delete model;
//    delete db;
    delete ui;
}

void MainWidget::reload_model()
{
    summAccount active(Account_Type::active), passive(Account_Type::passive);

    model->fill_model(db);
    ui->treeView->hideColumn(0);
    ui->treeView->header()->setResizeMode(model->day, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->month, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->year, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->column_from, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->column_to, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->summ, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(model->status, QHeaderView::ResizeToContents);

//    double active = db->get_account_summ(Account_Type::active);
//    double passive = db->get_account_summ(Account_Type::passive);
//    double debet = db->get_operation_summ(Account_Type::debet);
//    double credit = db->get_operation_summ(Account_Type::credit);

    ui->active_value->setText(active.text() + " " + var.Symbol());
    ui->passive_value->setText(passive.text() + " " + var.Symbol());
//    ui->debet_value->setText(default_locale->toString(debet,'f',2) + " " + var.Symbol());
//    ui->credit_value->setText(default_locale->toString(credit,'f',2) + " " + var.Symbol());
}

void MainWidget::clear_model()
{
    ui->active_value->setText("0");
    ui->passive_value->setText("0");
//    ui->debet_value->setText("0");
//    ui->credit_value->setText("0");
    model->clear();
}
