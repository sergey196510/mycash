#include "editaccount.h"
#include "ui_editaccount.h"

EditAccount::EditAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAccount)
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));

    ui->balanceSpinBox->setRange(-1000000, 1000000);
}

EditAccount::~EditAccount()
{
    delete ui;
}

QString EditAccount::name()
{
    return ui->nameEdit->text();
}

int EditAccount::type()
{
    return ui->typeBox->value();
}

double EditAccount::balance()
{
    return ui->balanceSpinBox->value();
}
