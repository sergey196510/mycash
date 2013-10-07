#include "correctbalance.h"
#include "ui_correctbalance.h"

CorrectBalance::CorrectBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CorrectBalance)
{
    ui->setupUi(this);

    ui->okButton->setIcon(QPixmap(":icons/accept.png"));
    ui->cancelButton->setIcon(QPixmap(":icons/block_32.png"));
    ui->okButton->setEnabled(false);

//    ui->doubleSpinBox->setRange(-1000000, 1000000);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(check_ok()));
    connect(ui->summEdit, SIGNAL(textChanged(QString)), SLOT(check_ok()));
    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

CorrectBalance::~CorrectBalance()
{
    delete ui;
}

void CorrectBalance::check_ok()
{
    if (ui->summEdit == 0 || ui->comboBox->value() == 0)
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
    ui->summEdit->setValue(value);
}

double CorrectBalance::balance()
{
    return ui->summEdit->text().toDouble();
}

int CorrectBalance::account()
{
    return ui->comboBox->value();
}

QString CorrectBalance::date()
{
    return ui->dateEdit->value();
}
