#include "listagents.h"
#include "ui_listagents.h"

ListAgentsModel::ListAgentsModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    header_data << tr("") << tr("Name") << tr("City") << tr("Address");
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

ListAgents::ListAgents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAgents)
{
    ui->setupUi(this);

    query = "SELECT id,name,city,address FROM agent ORDER BY name";

    model = new ListAgentsModel;
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

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(check_select_line()));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(check_new_button(QString)));
    connect(ui->newButton, SIGNAL(released()), SLOT(save_new_record()));
    connect(ui->editButton, SIGNAL(released()), SLOT(update_record()));
    connect(ui->delButton, SIGNAL(released()), SLOT(del_record()));
}

ListAgents::~ListAgents()
{
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
    int id;

    list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return 0;
    }

    return list.at(0).data((Qt::DisplayRole)).toInt();
}

void ListAgents::check_select_line()
{
    QSqlQuery q;
    int id;

    id = get_selected_id();

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
}

void ListAgents::save_new_record()
{
    QSqlQuery q;
    QString name, city, address, phone, contact;

    name = ui->nameEdit->text();
    city = ui->cityEdit->text();
    address = ui->addrEdit->text();
    phone = ui->phoneEdit->text();
    contact = ui->contactEdit->text();

    q.prepare("INSERT INTO agent(name, city, address, phone, contact) VALUES(:name, :city, :address, :phone, :contact)");
    q.bindValue(":name", name);
    q.bindValue(":city", city);
    q.bindValue(":address", address);
    q.bindValue(":phone", phone);
    q.bindValue(":contact", contact);
    if (!q.exec()) {
        qDebug() << "Insert error";
        return;
    }

    model->setQuery(query);
}

void ListAgents::update_record()
{
    QSqlQuery q;
    int id = get_selected_id();

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

    model->setQuery(query);
}

void ListAgents::del_record()
{
    QSqlQuery q;
    int id;

    id = get_selected_id();
    QString name = db.get_agent_name(id);

    int r = QMessageBox::warning(this, tr("Agent"),
                                 tr("You want delete agent %1?").arg(name),
                                 QMessageBox::Yes | QMessageBox::No);

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

    model->setQuery(query);
}
