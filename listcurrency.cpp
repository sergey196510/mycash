#include "listcurrency.h"
#include "ui_listcurrency.h"

ListCurrencyModel::ListCurrencyModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    header_data << tr("") << tr("Icod") << tr("Scod") << tr("Nominal") << tr("Kurs")<< tr("Name") ;
    list = read_list();
}

ListCurrencyModel::~ListCurrencyModel()
{
    list.clear();
}

QList<Currency> ListCurrencyModel::read_list()
{
    QSqlQuery q;
    QList<Currency> list;

    q.prepare("SELECT id FROM currency ORDER BY name");
    if (!q.exec()) {
        qDebug() << q.lastError();
        return list;
    }
    while (q.next()) {
        Currency data(q.value(0).toInt());
        list.append(data);
    }

    return list;
}

int ListCurrencyModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return list.count();
}

int ListCurrencyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return header_data.count();
}

QVariant ListCurrencyModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 0) {
            Currency data = list.at(index.row());
            return data.Id();
        }
        if (index.column() == 1) {
            Currency data = list.at(index.row());
            return data.ICod();
        }
        if (index.column() == 2) {
            Currency data = list.at(index.row());
            return data.SCod();
        }
        if (index.column() == 3) {
            Currency data = list.at(index.row());
            return data.Nominal();
        }
        if (index.column() == 4) {
            Currency data = list.at(index.row());
            return default_locale->toString(data.Kurs(),'f',4);
        }
        if (index.column() == 5) {
            Currency data = list.at(index.row());
            return data.Name();
        }
        else
            return QVariant();

        case Qt::TextAlignmentRole:
            if (index.column() == 1 || index.column() == 3 || index.column() == 4)
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
    beginResetModel();
    list.clear();
    list = read_list();
    endResetModel();
}

bool ListCurrencyModel::compare(int i, int j, int col, Qt::SortOrder order)
{
    Currency f, n;

    if (order == 0) {
        f = list.at(i);
        n = list.at(j);
    }
    else {
        f = list.at(j);
        n = list.at(i);
    }

    if (col == 1 && f.ICod() > n.ICod())
            return true;
    if (col == 2 && f.SCod() > n.SCod())
            return true;
    if (col == 5 && f.Name() > n.Name())
            return true;

    return false;
}

void ListCurrencyModel::sort(int col, Qt::SortOrder order)
{
    int i = 0;

//    return;

    beginResetModel();

    while (i < list.size()-1) {
//        Currency f = list.at(i), n = list.at(i+1);
//        qDebug() << col << order << i << list.size() << f.ICod() << n.ICod();
        if (compare(i, i+1, col, order)) {
            list.swap(i, i+1);
            i = 0;
            continue;
        }
        i++;
    }

    endResetModel();
}

ListCurrency::ListCurrency(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCurrency)
{
    ui->setupUi(this);

    ld = new Downloader(this);
    connect(ld, SIGNAL(done(QUrl,QByteArray)), SLOT(done_load(QUrl,QByteArray)));
//    connect(ld, &Downloader::done, this, &ListCurrency::done_load);
#ifdef HAVE_QT5
    connect(ld, &Downloader::error, this, &ListCurrency::eror_load);
#else
    connect(ld, SIGNAL(error()), SLOT(eror_load()));
#endif

    dumpObjectInfo();

    ui->groupBox->setTitle(tr("Currency"));

    if (var.database_Opened()) {
        model = new ListCurrencyModel(this);
        connect(this, SIGNAL(data_change()), model, SLOT(changed_data()));
        ui->tableView->setModel(model);
        ui->tableView->hideColumn(0);
    }

//    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
//    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
//    ui->treeView->header()->setResizeMode(3, QHeaderView::ResizeToContents);

    ui->newButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    ui->clearButton->setEnabled(false);

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(check_select()));
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(check_select()));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(check_new_button(QString)));
    connect(ui->symbolEdit, SIGNAL(textChanged(QString)), SLOT(check_symbol(QString)));

    connect(ui->newButton, SIGNAL(clicked()), SLOT(new_currency()));
    connect(ui->editButton, SIGNAL(clicked()), SLOT(update_currency()));

#ifdef HAVE_QT5
    connect(ui->loadButton, &QToolButton::clicked, this, &ListCurrency::load);
#else
    connect(ui->loadButton, SIGNAL(clicked()), SLOT(load()));
#endif

    connect(ui->delButton, SIGNAL(clicked()), SLOT(delete_currency()));
    connect(ui->clearButton, SIGNAL(clicked()), SLOT(clear_currency()));

    ui->tableView->setSortingEnabled(true);
    ui->tableView->header()->setSortIndicator(1, Qt::AscendingOrder);
    connect(ui->tableView->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), model, SLOT(sort(int,int)));
}

ListCurrency::~ListCurrency()
{
    delete ld;
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

    q.prepare("INSERT INTO currency(name, icod, scod, nominal, kurs) VALUES(:name, :icod, :scod, :nominal, :kurs)");
    q.bindValue(":name", ui->nameEdit->text());
    q.bindValue(":icod", ui->numberEdit->text().toInt());
    q.bindValue(":scod", ui->symbolEdit->text());
    q.bindValue(":nominal", ui->nominalEdit->text().toInt());
    q.bindValue(":kurs", ui->kursEdit->value());
    q.exec();

    emit data_change();
}

void ListCurrency::update_currency()
{
    QSqlQuery q;
    Currency item;
    int id;

    if ((id = ui->tableView->get_selected_id()) == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    item.setName(ui->nameEdit->text());
    item.setICod(ui->numberEdit->text().toInt());
    item.setSCod(ui->symbolEdit->text());
    item.setNominal(ui->nominalEdit->text().toInt());
    item.setKurs(ui->kursEdit->value());
    item.setId(id);
    if (item.update())
        emit data_change();

//    q.prepare("UPDATE currency SET name = :name, icod = :icod, scod = :scod, nominal = :nominal, kurs = :kurs WHERE id = :id");
//    q.bindValue(":name", ui->nameEdit->text());
//    q.bindValue(":icod", ui->numberEdit->text().toInt());
//    q.bindValue(":scod", ui->symbolEdit->text());
//    q.bindValue(":nominal", ui->nominalEdit->text().toInt());
//    q.bindValue(":kurs", ui->kursEdit->value());
//    q.bindValue(":id", id);
//    if (!q.exec()) {
//        qDebug() << q.lastError();
//        return;
//    }

//    emit data_change();
}

void ListCurrency::delete_currency()
{
    QSqlQuery q;
    int ccod;

    if ((ccod = ui->tableView->get_selected_id()) == 0) {
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
    ui->numberEdit->clear();
    ui->symbolEdit->clear();
    ui->nominalEdit->clear();
    ui->kursEdit->clear();
}

void ListCurrency::check_select()
{
    QSqlQuery q;
    int id;

    if ((id = ui->tableView->get_selected_id()) == 0) {
//        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    q.prepare("SELECT name, icod, scod, nominal, kurs FROM currency WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec() || !q.next()) {
        qDebug() << q.lastError();
        return;
    }
    ui->nameEdit->setText(q.value(0).toString());
    ui->numberEdit->setText(q.value(1).toString());
    ui->symbolEdit->setText(q.value(2).toString());
    ui->nominalEdit->setText(q.value(3).toString());
    ui->kursEdit->setValue(q.value(4).toDouble());

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

void ListCurrency::load()
{
//    QString req = "http://www.micex.ru/issrpc/marketdata/stock/shares/daily/download/micex_stock_shares_2014_09_30.xml?collection_id=12&board_group_id=57&start=0&limit=10000&lang=ru";
    QString req = "http://www.cbr.ru/scripts/XML_daily.asp";
    ld->download(req);
//    qDebug() << "load";
}

void ListCurrency::done_load(const QUrl &url, const QByteArray &array)
{
    QDomDocument domDoc;
    QString str1 = array;
    QString str2 = str1.toUtf8();

    ui->textBrowser->clear();
    ui->textBrowser->append(array);

    item_flag = false;
    list.clear();
    if (domDoc.setContent(array)) {
        QDomElement domElement = domDoc.documentElement();
        traverseNode(domElement);
        if (item_flag)
            list.append(item);
        update_database();
    }
}

void ListCurrency::traverseNode(const QDomNode &node)
{
    QDomNode domNode = node.firstChild();

    while (!domNode.isNull()) {
        if (domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if (!domElement.isNull()) {
                if (domElement.tagName() != "Valute") {
//                    qDebug() << domElement.tagName() << ":\t" << domElement.text();
                    if (domElement.tagName() == "NumCode")
                        item.setICod(domElement.text().toInt());
                    if (domElement.tagName() == "CharCode")
                        item.setSCod(domElement.text());
                    if (domElement.tagName() == "Nominal")
                        item.setNominal(domElement.text().toInt());
                    if (domElement.tagName() == "Name")
                        item.setName(domElement.text());
                    if (domElement.tagName() == "Value") {
//                        qDebug() << domElement.text() << default_locale->toDouble(domElement.text());
                        item.setKurs(default_locale->toDouble(domElement.text()));
                    }
                    item_flag = true;
                }
                else {
//                    qDebug() << "--------------";
                    if (item_flag) {
//                        qDebug() << item.ICod() << item.SCod() << item.Name() << item.Nominal() << item.Kurs();
                        list.append(item);
                        item_flag = false;
                    }
                }
            }
        }
        traverseNode(domNode);
        domNode = domNode.nextSibling();
    }
}

bool ListCurrency::update_database()
{
    Currency data, item;
    QList<Currency>::iterator i;
    Transaction tr;

    tr.begin();

    for (i = list.begin(); i != list.end(); i++) {
        data = *i;
//        qDebug() << data.ICod() << data.SCod() << data.Name() << data.Nominal() << data.Kurs();
        if (item.load(data.SCod())) {
            item.setKurs(data.Kurs());
            item.setName(data.Name());
            item.setICod(data.ICod());
            item.setNominal(data.Nominal());
            item.update();
        }
    }

    tr.commit();
    emit data_change();
    return true;
}

void ListCurrency::eror_load()
{
//    qDebug() << "Error load";
}
