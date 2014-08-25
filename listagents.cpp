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
        agent_data data = list.at(index.row());
        if (index.column() == 0)
            return data.Id();
        else if (index.column() == 1)
            return data.name();
        else if (index.column() == 2)
            return data.city();
        else if (index.column() == 3)
            return data.address();
        else
            return QVariant();
    }
    else
        return QVariant();
}

QList<agent_data> ListAgentsModel::read_list()
{
    QSqlQuery q;
    agent_data data;
    QList<agent_data> list;

    q.prepare("SELECT id,name,city,address FROM agent ORDER BY name");
    if (!q.exec()) {
        qDebug() << q.lastError();
        return list;
    }
    while (q.next()) {
        data.set_id(q.value(0).toInt());
        data.set_name(q.value(1).toString());
        data.set_city(q.value(2).toString());
        data.set_address(q.value(3).toString());
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

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(0);
//    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
//    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);

    ui->newButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    ui->clearButton->setEnabled(false);

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), SLOT(check_select_line()));
    connect(ui->treeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(check_select_line()));
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

int ListAgents::get_selected_id()
{
    QModelIndexList list;
//    int id;

    list = ui->treeView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
//        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return 0;
    }

    return list.at(0).data((Qt::DisplayRole)).toInt();
}

void ListAgents::check_select_line()
{
    QSqlQuery q;
    int id;

    if ((id = get_selected_id()) == 0) {
//        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    q.prepare("SELECT name,city,address,phone,contact FROM agent WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        ui->nameEdit->setText(q.value(0).toString());
        ui->cityEdit->setText(q.value(1).toString());
        ui->addrEdit->setText(q.value(2).toString());
        ui->phoneEdit->setText(q.value(3).toString());
        ui->contactEdit->setText(q.value(4).toString());
    }

    ui->editButton->setEnabled(true);
    ui->delButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
}

void ListAgents::save_new_record()
{
    QSqlQuery q;
    agent_data data;

    data.set_name(ui->nameEdit->text());
    data.set_city(ui->cityEdit->text());
    data.set_address(ui->addrEdit->text());
    data.set_phone(ui->phoneEdit->text());
    data.set_contact(ui->contactEdit->text());

    if (db->new_agent(data) == 0)
        return;

    reload_model();
}

void ListAgents::update_record()
{
    QSqlQuery q;
    int id;

    if ((id = get_selected_id()) == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    q.prepare("UPDATE agent SET name = :name, city = :city, address = :address, phone = :phone, contact = :contact WHERE id = :id");
    q.bindValue(":name", ui->nameEdit->text());
    q.bindValue(":city", ui->cityEdit->text());
    q.bindValue(":address", ui->addrEdit->text());
    q.bindValue(":phone", ui->phoneEdit->text());
    q.bindValue(":contact", ui->contactEdit->text());
    q.bindValue(":id", id);
    q.exec();
    if (!q.exec()) {
        qDebug() << "Update error";
        return;
    }

    reload_model();
}

void ListAgents::del_record()
{
    QSqlQuery q;
    int id;

    if ((id = get_selected_id()) == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    QString name = db->get_agent_name(id);

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
