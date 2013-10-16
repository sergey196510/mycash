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
        if (index.column() == 4) {
            return default_locale->toString(value.toDouble());
        }
        else
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
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setShowGrid(false);

    ui->newButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    ui->defaultButton->setEnabled(false);

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(check_select()));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(check_new_button(QString)));
    connect(ui->symbolEdit, SIGNAL(textChanged(QString)), SLOT(check_symbol(QString)));

    connect(ui->newButton, SIGNAL(released()), SLOT(new_currency()));
    connect(ui->editButton, SIGNAL(released()), SLOT(update_currency()));
    connect(ui->delButton, SIGNAL(released()), SLOT(delete_currency()));
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
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

int ListCurrency::get_selected_id()
{
    QModelIndexList list;

    list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return 0;
    }

    return list.at(0).data((Qt::DisplayRole)).toInt();
}

void ListCurrency::update_currency()
{
    QSqlQuery q;
    int id;

    id = get_selected_id();

    q.prepare("UPDATE currency SET name = :name, scod = :scod, kurs = :kurs WHERE id = :id");
    q.bindValue(":name", ui->nameEdit->text());
    q.bindValue(":scod", ui->symbolEdit->text());
    q.bindValue(":kurs", ui->kursEdit->value());
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "Update error";
        return;
    }

    model->setQuery(query);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

void ListCurrency::delete_currency()
{
    QSqlQuery q;
    int ccod = get_selected_id();

    q.prepare("SELECT id FROM account WHERE ccod = :ccod");
    q.bindValue(":ccod", ccod);
    if (q.exec() && q.next()) {
        qDebug() << "Exist accounts whith this currencies";
        return;
    }

    q.prepare("DELETE FROM currency WHERE id = :id");
    q.bindValue(":id", ccod);
    q.exec();
}

void ListCurrency::set_default()
{
    QSqlQuery q;

    current_currency = get_selected_id();

    model->setQuery(query);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

void ListCurrency::check_select()
{
    QSqlQuery q;
    int id;

    id = get_selected_id();

    q.prepare("SELECT name, scod, kurs FROM currency WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec() || !q.next()) {
        qDebug() << "Select error";
        return;
    }
    ui->nameEdit->setText(q.value(0).toString());
    ui->symbolEdit->setText(q.value(1).toString());
    ui->kursEdit->setValue(q.value(2).toDouble());

    ui->editButton->setEnabled(true);
    ui->delButton->setEnabled(true);
    ui->defaultButton->setEnabled(true);
}
