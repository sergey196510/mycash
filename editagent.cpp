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

    data.name    = ui->nameEdit->text();
    data.city    = ui->cityEdit->text();
    data.address = ui->addrEdit->text();
    data.phone   = ui->phoneEdit->text();
    data.contact = ui->contactEdit->text();

    return data;
}

void EditAgent::checkOk()
{
    ui->okButton->setEnabled(ui->nameEdit->text().length());
}
