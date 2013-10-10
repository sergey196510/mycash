#include "listcurrency.h"
#include "ui_listcurrency.h"

ListCurrencyModel::ListCurrencyModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

ListCurrencyModel::~ListCurrencyModel()
{
}

QVariant ListCurrencyModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 4)
                return int(Qt::AlignRight | Qt::AlignVCenter);

    case Qt::TextColorRole:
//        qDebug() << record(index.row()).value(0).toInt();
        if (record(index.row()).value(0).toInt() == current_currency) {
            return QVariant(QColor(Qt::red));
        }
        return value;
    }

    return value;
}

ListCurrency::ListCurrency(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCurrency)
{
    ui->setupUi(this);

    query = "SELECT id, name, icod, scod, kurs FROM currency ORDER BY name";

    model = new ListCurrencyModel;
    model->setQuery(query);

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);

    ui->newButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    ui->defaultButton->setEnabled(false);

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(check_select()));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(check_new_button(QString)));
    connect(ui->symbolEdit, SIGNAL(textChanged(QString)), SLOT(check_symbol(QString)));

    connect(ui->newButton, SIGNAL(released()), SLOT(new_currency()));
    connect(ui->defaultButton, SIGNAL(released()), SLOT(set_default()));
}

ListCurrency::~ListCurrency()
{
    delete model;
    delete ui;
}

void ListCurrency::check_new_button(QString str)
{
    ui->newButton->setEnabled(str.length());
}

void ListCurrency::check_symbol(QString str)
{
    QLocale locale;

    locale = str;

}

void ListCurrency::new_currency()
{
    QSqlQuery q;

    q.prepare("INSERT INTO currency(name, scod, kurs) VALUES(:name, :scod, :kurs)");
    q.bindValue(":name", ui->nameEdit->text());
    q.bindValue(":scod", ui->symbolEdit->text());
    q.bindValue(":kurs", ui->kursEdit->value());
    q.exec();

    model->setQuery(query);
}

void ListCurrency::set_default()
{
    QModelIndexList list;
    QSqlQuery q;
    int id;

    list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    current_currency = list.at(0).data((Qt::DisplayRole)).toInt();
    model->setQuery(query);
}

void ListCurrency::check_select()
{
    ui->defaultButton->setEnabled(true);
}
