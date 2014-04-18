#include "editagent.h"
#include "ui_editagent.h"

EditAgent::EditAgent(QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAgent)
{
    ui->setupUi(this);

    this->setWindowTitle(title);

    ui->okButton->setEnabled(false);

    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(checkOk()));
}

EditAgent::~EditAgent()
{
    delete ui;
}

agent_data EditAgent::data()
{
    agent_data data;

    data.set_name(ui->nameEdit->text());
    data.set_city(ui->cityEdit->text());
    data.set_address(ui->addrEdit->text());
    data.set_phone(ui->phoneEdit->text());
    data.set_contact(ui->contactEdit->text());

    return data;
}

void EditAgent::checkOk()
{
    ui->okButton->setEnabled(ui->nameEdit->text().length());
}
