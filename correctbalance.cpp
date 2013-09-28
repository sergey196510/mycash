#include "correctbalance.h"
#include "ui_correctbalance.h"

CorrectBalance::CorrectBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CorrectBalance)
{
    ui->setupUi(this);

    ui->okButton->setEnabled(false);

    ui->doubleSpinBox->setRange(-1000000, 1000000);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(check_ok()));
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), SLOT(check_ok()));
    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

CorrectBalance::~CorrectBalance()
{
    delete ui;
}

void CorrectBalance::check_ok()
{
    if (ui->doubleSpinBox->value() == 0 || ui->comboBox->value() == 0)
        ui->okButton->setEnabled(false);
    else
        ui->okButton->setEnabled(true);
}

void CorrectBalance::setAccount(int value)
{
    ui->comboBox->setValue(value);
}

void CorrectBalance::setBalance(double value)
{
    ui->doubleSpinBox->setValue(value);
}

double CorrectBalance::balance()
{
    return ui->doubleSpinBox->value();
}

int CorrectBalance::account()
{
    return ui->comboBox->value();
}

QString CorrectBalance::date()
{
    return ui->dateEdit->value();
}
