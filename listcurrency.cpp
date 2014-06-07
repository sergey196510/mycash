#include "listcurrency.h"
#include "ui_listcurrency.h"

ListCurrencyModel::ListCurrencyModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    header_data << tr("") << tr("Name") << tr("Icod") << tr("Scod") << tr("Kurs");
    list = read_list();
}

ListCurrencyModel::~ListCurrencyModel()
{
}

QList<Currency_Data> ListCurrencyModel::read_list()
{
    QSqlQuery q;
    QList<Currency_Data> list;
    Currency_Data data;

    q.prepare("SELECT id, name, icod, scod, kurs FROM currency ORDER BY name");
    if (!q.exec()) {
        qDebug() << q.lastError();
        return list;
    }
    while (q.next()) {
        data.id = q.value(0).toInt();
        data.name = q.value(1).toString();
        data.icod = q.value(2).toInt();
        data.scod = q.value(3).toString();
        data.kurs = q.value(4).toDouble();
        list.append(data);
    }

    return list;
}

int ListCurrencyModel::rowCount(const QModelIndex &parent) const
{
    return list.count();
}

int ListCurrencyModel::columnCount(const QModelIndex &parent) const
{
    return header_data.count();
}

QVariant ListCurrencyModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 0) {
            Currency_Data data = list.at(index.row());
            return data.id;
        }
        if (index.column() == 1) {
            Currency_Data data = list.at(index.row());
            return data.name;
        }
        if (index.column() == 2) {
            Currency_Data data = list.at(index.row());
            return data.icod;
        }
        if (index.column() == 3) {
            Currency_Data data = list.at(index.row());
            return data.scod;
        }
        else if (index.column() == 4) {
            Currency_Data data = list.at(index.row());
            return default_locale->toString(data.kurs);
        }
        else
            return QVariant();

        case Qt::TextAlignmentRole:
            if (index.column() == 4)
                return int(Qt::AlignRight | Qt::AlignVCenter);

    case Qt::TextColorRole:
//        qDebug() << record(index.row()).value(0).toInt();
//        if (record(index.row()).value(0).toInt() == var.Currency()) {
//            return QVariant(QColor(Qt::blue));
//        }
        return QVariant();
    }

    return QVariant();
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

void ListCurrencyModel::changed_data()
{
    list.clear();
    list = read_list();
    reset();
}

ListCurrency::ListCurrency(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCurrency)
{
    ui->setupUi(this);

    ui->groupBox->setTitle(tr("Currency"));

    if (var.database_Opened()) {
        model = new ListCurrencyModel;
        connect(this, SIGNAL(data_change()), model, SLOT(changed_data()));
        ui->treeView->setModel(model);
        ui->treeView->hideColumn(0);
    }

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

    emit data_change();
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
        qDebug() << q.lastError();
        return;
    }

    emit data_change();
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

    emit data_change();
}

void ListCurrency::clear_currency()
{
    ui->nameEdit->clear();
    ui->symbolEdit->clear();
    ui->kursEdit->clear();
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
        qDebug() << q.lastError();
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
    emit data_change();
}

void ListCurrency::clear_model()
{
//    model->clear();
}
