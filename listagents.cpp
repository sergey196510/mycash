#include "listagents.h"
#include "ui_listagents.h"

ListAgentsModel::ListAgentsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    header_data << tr("") << tr("Name") << tr("City") << tr("Address");
    list = read_list();
}

QVariant ListAgentsModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

int ListAgentsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return list.size();
}

int ListAgentsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return header_data.size();
}

QVariant ListAgentsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= list.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        Agent data = list.at(index.row());
        if (index.column() == 0)
            return data.Id();
        else if (index.column() == 1)
            return data.Name();
        else if (index.column() == 2)
            return data.City();
        else if (index.column() == 3)
            return data.Address();
        else
            return QVariant();
    }
    else
        return QVariant();
}

QList<Agent> ListAgentsModel::read_list()
{
    QSqlQuery q;
    Agent data;
    QList<Agent> list;

    q.prepare("SELECT id,name,city,address FROM agent ORDER BY name");
    if (!q.exec()) {
        qDebug() << q.lastError();
        return list;
    }
    while (q.next()) {
        data.set_Id(q.value(0).toInt());
        data.set_Name(q.value(1).toString());
        data.set_City(q.value(2).toString());
        data.set_Address(q.value(3).toString());
        list.append(data);
    }

    return list;
}

ListAgents::ListAgents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAgents)
{
    ui->setupUi(this);

    if (var.database_Opened()) {
        query = "SELECT id,name,city,address FROM agent ORDER BY name";
        model = new ListAgentsModel;
//        model->setQuery(query);
    }

    db = new Database;

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);

    ui->newButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    ui->clearButton->setEnabled(false);

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(check_select_line()));
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(check_select_line()));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(check_new_button(QString)));
    connect(ui->newButton, SIGNAL(clicked()), SLOT(save_new_record()));
    connect(ui->editButton, SIGNAL(clicked()), SLOT(update_record()));
    connect(ui->delButton, SIGNAL(clicked()), SLOT(del_record()));
    connect(ui->clearButton, SIGNAL(clicked()), SLOT(clear_record()));
}

ListAgents::~ListAgents()
{
    delete db;
    delete ui;
    delete model;
}

void ListAgents::check_new_button(QString str)
{
    ui->newButton->setEnabled(str.length());
}

void ListAgents::check_select_line()
{
    QSqlQuery q;
    int id;
    Agent a;

    if ((id = ui->tableView->get_selected_id()) == 0) {
//        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    if (a.read(id)) {
        ui->nameEdit->setText(a.Name());
        ui->cityEdit->setText(a.City());
        ui->addrEdit->setText(a.Address());
        ui->phoneEdit->setText(a.Phone());
        ui->contactEdit->setText(a.Contact());
    }

    ui->editButton->setEnabled(true);
    ui->delButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
}

void ListAgents::save_new_record()
{
    QSqlQuery q;
    Agent data;

    data.set_Name(ui->nameEdit->text());
    data.set_City(ui->cityEdit->text());
    data.set_Address(ui->addrEdit->text());
    data.set_Phone(ui->phoneEdit->text());
    data.set_Contact(ui->contactEdit->text());

    if (data.insert() == 0)
        return;

    reload_model();
}

void ListAgents::update_record()
{
    QSqlQuery q;
    Agent a;
    int id;

    if ((id = ui->tableView->get_selected_id()) == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    a.set_Id(id);
    a.set_Name(ui->nameEdit->text());
    a.set_City(ui->cityEdit->text());
    a.set_Address(ui->addrEdit->text());
    a.set_Phone(ui->phoneEdit->text());
    a.set_Contact(ui->contactEdit->text());
    a.update();

    reload_model();
}

void ListAgents::del_record()
{
    QSqlQuery q;
    int id;

    if ((id = ui->tableView->get_selected_id()) == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    Agent item;
    item.read(id);
    QString name = item.Name();

    int r = QMessageBox::warning(this, tr("Agent"),
                                 tr("You want delete agent %1?").arg(name),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (r == QMessageBox::No)
        return;

    q.prepare("DELETE FROM agent WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "Delete error";
        return;
    }

    q.prepare("UPDATE operation SET agent = 0 WHERE agent = :agent");
    q.bindValue(":agent", id);
    if (!q.exec()) {
        qDebug() << "Update error";
        return;
    }

    reload_model();
}

void ListAgents::clear_record()
{
    ui->nameEdit->clear();
    ui->cityEdit->clear();
    ui->addrEdit->clear();
    ui->phoneEdit->clear();
    ui->contactEdit->clear();
}

void ListAgents::reload_model()
{
//    model->setQuery(query);
}

void ListAgents::clear_model()
{
//    model->clear();
}
