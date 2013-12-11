#include "listcurrency.h"
#include "ui_listcurrency.h"

ListCurrencyModel::ListCurrencyModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    header_data << tr("") << tr("Name") << tr("Icod") << tr("Scod") << tr("Kurs");
}

ListCurrencyModel::~ListCurrencyModel()
{
}

QVariant ListCurrencyModel::data(const QModelIndex &index, int role) const
{
    Globals var;
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
//        if (record(index.row()).value(0).toInt() == var.Currency()) {
//            return QVariant(QColor(Qt::blue));
//        }
        return value;
    }

    return value;
}

QVariant ListCurrencyModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

ListCurrency::ListCurrency(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCurrency)
{
    ui->setupUi(this);

    ui->groupBox->setTitle(tr("Currency"));

    if (var.database_Opened()) {
        query = "SELECT id, name, icod, scod, kurs FROM currency ORDER BY name";
        model = new ListCurrencyModel;
        model->setQuery(query);
        ui->treeView->setModel(model);
        ui->treeView->hideColumn(0);
    }

//    ui->treeView->resizeRowsToContents();
//    ui->treeView->resizeColumnsToContents();
    ui->treeView->setAlternatingRowColors(true);
//    ui->treeView->horizontalHeader()->setStretchLastSection(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->treeView->setShowGrid(false);
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(3, QHeaderView::ResizeToContents);

    ui->newButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    ui->clearButton->setEnabled(false);

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), SLOT(check_select()));
    connect(ui->treeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(check_select()));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(check_new_button(QString)));
    connect(ui->symbolEdit, SIGNAL(textChanged(QString)), SLOT(check_symbol(QString)));

    connect(ui->newButton, SIGNAL(clicked()), SLOT(new_currency()));
    connect(ui->editButton, SIGNAL(clicked()), SLOT(update_currency()));
    connect(ui->delButton, SIGNAL(clicked()), SLOT(delete_currency()));
    connect(ui->clearButton, SIGNAL(clicked()), SLOT(clear_currency()));
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

    reload_model();
//    ui->treeView->resizeRowsToContents();
//    ui->treeView->resizeColumnsToContents();
}

int ListCurrency::get_selected_id()
{
    QModelIndexList list;

    list = ui->treeView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        return 0;
    }

    return list.at(0).data((Qt::DisplayRole)).toInt();
}

void ListCurrency::update_currency()
{
    QSqlQuery q;
    int id;

    if ((id = get_selected_id()) == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    q.prepare("UPDATE currency SET name = :name, scod = :scod, kurs = :kurs WHERE id = :id");
    q.bindValue(":name", ui->nameEdit->text());
    q.bindValue(":scod", ui->symbolEdit->text());
    q.bindValue(":kurs", ui->kursEdit->value());
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "Update error";
        return;
    }

    reload_model();
}

void ListCurrency::delete_currency()
{
    QSqlQuery q;
    int ccod;

    if ((ccod = get_selected_id()) == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    q.prepare("SELECT id FROM account WHERE ccod = :ccod");
    q.bindValue(":ccod", ccod);
    if (q.exec() && q.next()) {
        int r = QMessageBox::warning(this, tr("Currency"),
                                     tr("Exist accounts whith this currencies\n"
                                        "You really want to delete this?"),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (r == QMessageBox::No)
            return;
    }

    q.prepare("DELETE FROM currency WHERE id = :id");
    q.bindValue(":id", ccod);
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return;
    }

    reload_model();
}

void ListCurrency::clear_currency()
{
    ui->nameEdit->setText("");
    ui->symbolEdit->setText("");
    ui->kursEdit->setValue(0);
}

void ListCurrency::check_select()
{
    QSqlQuery q;
    int id;

    if ((id = get_selected_id()) == 0) {
//        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }


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
    ui->clearButton->setEnabled(true);
}

void ListCurrency::reload_model()
{
    model->setQuery(query);
}

void ListCurrency::clear_model()
{
    model->clear();
}
