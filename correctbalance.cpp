#include "correctbalance.h"
#include "ui_correctbalance.h"

CorrectBalance::CorrectBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CorrectBalance)
{
    ui->setupUi(this);

    ui->lineEdit->setValue(var.CorrectAccount());

//    ui->okButton->setIcon(QPixmap(":icons/accept.png"));
//    ui->cancelButton->setIcon(QPixmap(":icons/block_32.png"));
    ui->okButton->setEnabled(false);

//    ui->doubleSpinBox->setRange(-1000000, 1000000);

    connect(ui->lineEdit, SIGNAL(changed_value()), SLOT(check_ok()));
    connect(ui->summEdit, SIGNAL(textChanged(QString)), SLOT(check_ok()));
    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

CorrectBalance::~CorrectBalance()
{
    delete ui;
}

void CorrectBalance::check_ok()
{
    if (ui->summEdit == 0 || ui->lineEdit->value() == 0)
        ui->okButton->setEnabled(false);
    else
        ui->okButton->setEnabled(true);
}

void CorrectBalance::setAccount(int value)
{
    ui->lineEdit->setValue(value);
}

void CorrectBalance::setBalance(double value)
{
    ui->summEdit->setValue(value);
}

double CorrectBalance::balance()
{
//    return ui->summEdit->text().toDouble();
//    return default_locale->toDouble(ui->summEdit->text());
    return ui->summEdit->value();
}

int CorrectBalance::account()
{
    return ui->lineEdit->value();
}

QDate CorrectBalance::date()
{
    return ui->dateEdit->value();
}
