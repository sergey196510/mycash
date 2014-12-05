#include "mainwidget.h"
#include "widgets/mydateedit.h"
#include "ui_mainwidget.h"

MainWidgetModel::MainWidgetModel(Database *d, QObject *parent) :
    QAbstractTableModel(parent)
{
    db = d;
    list = PlanOperation().get_list(1);
    header_data << tr("") << tr("Day") << tr("Month") << tr("Year") << tr("From Account") << tr("Summ") << tr("To Account") << tr("Summ") << tr("Status") << tr("Descr");
//    acc_list = db->get_accounts_list();
    var = new Globals;
}

MainWidgetModel::~MainWidgetModel()
{
//    delete db;
    delete var;
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
            PlanOperation data = list.at(index.row());
            return data.Id();
        }
        else if (index.column() == 1) {
            PlanOperation data = list.at(index.row());
            return data.Day();
        }
        else if (index.column() == 2) {
            PlanOperation data = list.at(index.row());
            if (data.Month())
                return data.Month();
            else
                return "";
        }
        else if (index.column() == 3) {
            return QVariant();
        }
        else if (index.column() == 4) {
            PlanOperation data = list.at(index.row());
            return data.From().at(0).account().fullName();
        }
        else if (index.column() == 5) {
            PlanOperation data = list.at(index.row());
            Currency curr(data.From().at(0).account().Curr());
            return default_locale->toString(data.From().at(0).balance().toDouble(),'f',2) + " " + curr.SCod();
        }
        else if (index.column() == 6) {
            PlanOperation data = list.at(index.row());
            return data.To().at(0).account().fullName();
        }
        else if (index.column() == 7) {
            PlanOperation data = list.at(index.row());
            Currency curr(data.To().at(0).account().Curr());
            return default_locale->toString(data.To().at(0).balance().toDouble(),'f',2) + " " + curr.SCod();
        }
        else if (index.column() == 8) {
            PlanOperation data = list.at(index.row());
            if (data.Status() == Plan_Status::expired)
                return tr("Expired");
            else if (data.Status() == Plan_Status::minimum)
                return tr("<3 days");
            else
                return QVariant();
        }
        else if (index.column() == 9) {
            PlanOperation data = list.at(index.row());
            return data.Descr();
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
            if (index.column() == 5)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 7)
                return int(Qt::AlignRight | Qt::AlignVCenter);

    case Qt::BackgroundColorRole:
        PlanOperation data = list.at(index.row());
        if (data.Status() == Plan_Status::minimum)
            return QVariant(QColor(Qt::yellow));
        if (data.Status() == Plan_Status::expired)
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

    ui->active_value->setText(default_locale->toCurrencyString(active.Value().toDouble()));
    ui->passive_value->setText(default_locale->toCurrencyString(passive.Value().toDouble()));
    saldo = active.Value()-passive.Value();
    if (saldo < 0) {
        font.setBold(true);
        ui->saldo_value->setFont(font);
    }
    ui->saldo_value->setText(default_locale->toCurrencyString(saldo));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
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

    ui->active_value->setText(active.text() + " " + Currency(var.Currency()).SCod());
    ui->passive_value->setText(passive.text() + " " + Currency(var.Currency()).SCod());
    saldo = active.Value()-passive.Value();
    ui->saldo_value->setText(QString("%1 %2").arg(default_locale->toString(saldo,'f',2)).arg(Currency(var.Currency()).SCod()));
}

void MainWidget::clear_model()
{
    ui->active_value->setText("0");
    ui->passive_value->setText("0");
}
