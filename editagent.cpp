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

Agent EditAgent::data()
{
    Agent data;

    data.set_Name(ui->nameEdit->text());
    data.set_City(ui->cityEdit->text());
    data.set_Address(ui->addrEdit->text());
    data.set_Phone(ui->phoneEdit->text());
    data.set_Contact(ui->contactEdit->text());

    return data;
}

void EditAgent::checkOk()
{
    ui->okButton->setEnabled(ui->nameEdit->text().length());
}
