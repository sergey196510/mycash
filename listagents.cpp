#include "listagents.h"
#include "ui_listagents.h"

ListAgents::ListAgents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAgents)
{
    ui->setupUi(this);

    query = "SELECT id,name,city FROM agent ORDER BY name";

    model = new QSqlQueryModel;
    model->setQuery(query);

    ui->tableView->setModel(model);
    ui->newButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(check_select_line()));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(check_new_button(QString)));
    connect(ui->newButton, SIGNAL(released()), SLOT(save_new_record()));
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

void ListAgents::check_select_line()
{
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
    q.exec();

    model->setQuery(query);
}
