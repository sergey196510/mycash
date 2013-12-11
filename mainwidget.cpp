#include "mainwidget.h"
#include "ui_mainwidget.h"

enum Column {
    id = 0,
    day = 1,
    month = 2,
    year = 3,
    column_from = 4,
    column_to = 5,
    summ = 6,
    status = 7,
    descr = 8
};
enum Status {
    actual = 1,
    committed = 2,
    minimum = 3,
    expired = 4
};

bool MainWidgetModel::get_operations(int plan)
{
    QSqlQuery q;
    QDate curr = QDate::currentDate();
    QString query;

    query = QString("SELECT count(id) FROM operation WHERE dt >= '01-%1-%2' AND plan_id = %3").arg(curr.month()).arg(curr.year()).arg(plan);
//    qDebug() << query;
    if (!q.exec(query)) {
        qDebug() << q.lastError().text();
        return false;
    }
    if (q.next() && q.value(0).toInt() == 0)
        return false;

    return true;
}

void MainWidgetModel::fill_model()
{
    QDate curr = QDate::currentDate();
    QList<operation_data>::iterator i;
    operation_data data;
    int row = 0;
    int stat = actual;

    clear();

    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Status") << tr("Descr");

    if (!var.database_Opened())
        return;

    accounts = db->get_accounts_list();

    insertColumns(0,9);
    for (i = list.begin(); i != list.end(); i++) {
        data = *i;
        if (data.month == 0 && data.year == 0) {
            int diff = data.day - curr.day();
            if (get_operations(data.id) == true) {
                stat = committed;
                continue;
            }
            else if (diff < 3 && diff >= 0)
                stat = minimum;
            else if (diff < 0)
                stat = expired;
            else
                continue;
        }
        insertRow(row);
        setData(index(row,id),    data.id);
        setData(index(row,day),   data.day);
        setData(index(row,month), data.month);
        setData(index(row,year),  data.year);
        setData(index(row,column_from),  accounts[data.from.account]);
        setData(index(row,column_to),    accounts[data.to.account]);
        setData(index(row,summ),  default_locale->toString(data.from.summ,'f',2));
        if (stat == actual)
            setData(index(row,status), tr("Actual"));
        else if (stat == committed)
            setData(index(row,status), tr("Committed"));
        else if (stat == expired)
            setData(index(row,status), tr("Expired"));
        else if (stat == minimum)
            setData(index(row,status), tr("< 3 days"));
        else
            setData(index(row,status), stat);
        setData(index(row,descr), data.descr);
        row++;
    }
}

MainWidgetModel::MainWidgetModel(QObject *parent) :
    QStandardItemModel(parent)
{
    db = new Database;
    list = db->get_plan_oper_list();
//    fill_model();
}

MainWidgetModel::~MainWidgetModel()
{
    delete db;
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
    QDate curr = QDate::currentDate();
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

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    summAccount active(Active_type), passive(Passive_type);

    ui->setupUi(this);

    db = new Database;
    model = new MainWidgetModel;
    model->fill_model();

    ui->groupBox->setTitle(tr("Balance status"));
    ui->groupBox_2->setTitle(tr("Prosrochennie operations"));

//    reload_model();

    ui->active_value->setText(active.text());
    ui->passive_value->setText(passive.text());

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->header()->setResizeMode(day, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(month, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(year, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(column_from, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(column_to, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(summ, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(status, QHeaderView::ResizeToContents);
}

MainWidget::~MainWidget()
{
    delete model;
    delete db;
    delete ui;
}

void MainWidget::reload_model()
{
    summAccount active(Active_type), passive(Passive_type);

    model->fill_model();
    ui->treeView->hideColumn(0);
    ui->treeView->header()->setResizeMode(day, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(month, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(year, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(column_from, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(column_to, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(summ, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(status, QHeaderView::ResizeToContents);

//    double active = db->get_account_summ(Active_type);
//    double passive = db->get_account_summ(Passive_type);
    double debet = db->get_operation_summ(Debet_type);
    double credit = db->get_operation_summ(Credit_type);

    ui->active_value->setText(active.text() + " " + var.Symbol());
    ui->passive_value->setText(passive.text() + " " + var.Symbol());
    ui->debet_value->setText(default_locale->toString(debet,'f',2) + " " + var.Symbol());
    ui->credit_value->setText(default_locale->toString(credit,'f',2) + " " + var.Symbol());
}

void MainWidget::clear_model()
{
    ui->active_value->setText("0");
    ui->passive_value->setText("0");
    ui->debet_value->setText("0");
    ui->credit_value->setText("0");
    model->clear();
}
