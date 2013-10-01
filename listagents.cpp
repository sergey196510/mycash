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

void ListAgents::check_select_line()
{
    QModelIndexList list;
    QSqlQuery q;
    int id;

    list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    id = list.at(0).data((Qt::DisplayRole)).toInt();

    q.prepare("SELECT name,city,address,phone,contact FROM agent WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        ui->nameEdit->setText(q.value(0).toString());
        ui->cityEdit->setText(q.value(1).toString());
        ui->addrEdit->setText(q.value(2).toString());
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
    q.exec();

    model->setQuery(query);
}

void ListAgents::del_record()
{
    QModelIndexList list;
    int id;

    list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        QMessageBox::critical(this, tr("Operation cancellation"), tr("Nothing selected"));
        return;
    }

    id = list.at(0).data((Qt::DisplayRole)).toInt();

}
