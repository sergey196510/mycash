#include "mainwidget.h"
#include "ui_mainwidget.h"

enum Column {
    id = 0,
    day = 1,
    month = 2,
    year = 3,
    from = 4,
    to = 5,
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
}

MainWidgetModel::MainWidgetModel(QObject *parent) :
    QStandardItemModel(parent)
{
    QDate curr = QDate::currentDate();
    db = new Database;
    list = db->get_plan_oper_list();
    QList<PlanOper_data>::iterator i;
    PlanOper_data data;
    int row = 0;
    int stat = actual;

    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Status") << tr("Descr");
    accounts = db->get_accounts_list();

    insertColumns(0,9);
    for (i = list.begin(); i != list.end(); i++) {
        data = *i;
        else if (data.month == 0 && data.year == 0) {
            int diff = data.day - curr.day();
            if (get_operations(data.id) == true)
                stat = committed;
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
        setData(index(row,from),  accounts[data.from]);
        setData(index(row,to),    accounts[data.to]);
        setData(index(row,summ),  default_locale->toString(data.summ));
        setData(index(row,status), stat);
        setData(index(row,descr), data.descr);
        row++;
    }
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
    ui->setupUi(this);
    db = new Database;
    model = new MainWidgetModel;

    ui->groupBox->setTitle(tr("Balance status"));
    ui->groupBox_2->setTitle(tr("Prosrochennie operations"));

    ui->active_value->setText(default_locale->toString(db->get_account_summ(active)) + " " + default_locale->currencySymbol(default_locale->CurrencyIsoCode));
    ui->passive_value->setText(default_locale->toString(db->get_account_summ(passive)) + " " + default_locale->currencySymbol(default_locale->CurrencyIsoCode));
    ui->debet_value->setText(default_locale->toString(db->get_operation_summ(debet)) + " " + default_locale->currencySymbol(default_locale->CurrencyIsoCode));
    ui->credit_value->setText(default_locale->toString(db->get_operation_summ(credit)) + " " + default_locale->currencySymbol(default_locale->CurrencyIsoCode));

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
    ui->treeView->header()->setResizeMode(day, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(month, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(year, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(from, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(to, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(summ, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(status, QHeaderView::ResizeToContents);
}

MainWidget::~MainWidget()
{
    delete model;
    delete db;
    delete ui;
}
