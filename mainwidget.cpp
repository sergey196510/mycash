#include "mainwidget.h"
#include "widgets/mydateedit.h"
#include "ui_mainwidget.h"

MainWidgetModel::MainWidgetModel(Database *d, QObject *parent) :
    QAbstractTableModel(parent)
{
    db = d;
    list = db->get_plan_oper_list(1);
    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("To Account") << tr("Summ") << tr("Status") << tr("Descr");
    acc_list = db->get_accounts_list();
    var = new Globals;
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
    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 0) {
            Operation_Data data = list.at(index.row());
            return data.id;
        }
        else if (index.column() == 1) {
            Operation_Data data = list.at(index.row());
            return data.day;
        }
        else if (index.column() == 2) {
            Operation_Data data = list.at(index.row());
            if (data.month)
                return data.month;
            else
                return "";
        }
        else if (index.column() == 3) {
            Operation_Data data = list.at(index.row());
            if (data.year)
                return data.year;
            else
                return "";
        }
        else if (index.column() == 4) {
            Operation_Data data = list.at(index.row());
            return acc_list[data.from.at(0).account()];
        }
        else if (index.column() == 5) {
            Operation_Data data = list.at(index.row());
            QMap<int,double> oper = db->get_plan_account_oper_list(data.id,2);
            QMap<int,double>::iterator i = oper.begin();
            return acc_list[i.key()];
        }
        else if (index.column() == 6) {
            Operation_Data data = list.at(index.row());
            QMap<int,double> oper = db->get_plan_account_oper_list(data.id,2);
            QMap<int,double>::iterator i = oper.begin();
            return default_locale->toString(i.value()/var->Kurs(),'f',2);
        }
        else if (index.column() == 7) {
            Operation_Data data = list.at(index.row());
            if (data.status == Plan_Status::expired)
                return tr("Expired");
            else if (data.status == Plan_Status::minimum)
                return tr("<3 days");
            else
                return QVariant();
        }
        else if (index.column() == 8) {
            Operation_Data data = list.at(index.row());
            return data.descr;
        }
        else
            return QVariant();

        case Qt::TextAlignmentRole:
            if (index.column() == 1)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 2)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 3)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 6)
                return int(Qt::AlignRight | Qt::AlignVCenter);

    case Qt::BackgroundColorRole:
        Operation_Data data = list.at(index.row());
        if (data.status == Plan_Status::minimum)
            return QVariant(QColor(Qt::yellow));
        if (data.status == Plan_Status::expired)
            return QVariant(QColor(Qt::red));

    }

    return QVariant();
}

int MainWidgetModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return list.size();
}

int MainWidgetModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return header_data.size();
}

MainWidget::MainWidget(Database *d, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    summAccount active(Account_Type::active), passive(Account_Type::passive);
    QFont font;

    ui->setupUi(this);
    double saldo;

    db = d;
    model = new MainWidgetModel(db);

    ui->groupBox->setTitle(tr("Balance status"));
    ui->groupBox_2->setTitle(tr("Prosrochennie operations"));

    ui->active_value->setText(default_locale->toCurrencyString(active.Value()));
    ui->passive_value->setText(default_locale->toCurrencyString(passive.Value()));
    saldo = active.Value()-passive.Value();
    if (saldo < 0) {
        font.setBold(true);
        ui->saldo_value->setFont(font);
    }
    ui->saldo_value->setText(default_locale->toCurrencyString(saldo));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
//    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
//    ui->tableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

MainWidget::~MainWidget()
{
    delete model;
    delete ui;
}

void MainWidget::reload_model()
{
    summAccount active(Account_Type::active), passive(Account_Type::passive);
    double saldo;

    ui->active_value->setText(active.text() + " " + var.Symbol());
    ui->passive_value->setText(passive.text() + " " + var.Symbol());
    saldo = active.Value()-passive.Value();
    ui->saldo_value->setText(QString("%1 %2").arg(default_locale->toString(saldo,'f',2)).arg(var.Symbol()));
}

void MainWidget::clear_model()
{
    ui->active_value->setText("0");
    ui->passive_value->setText("0");
}
