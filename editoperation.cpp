#include "editoperation.h"
#include "ui_editoperation.h"

EditOperation::EditOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOperation)
{
    ui->setupUi(this);

    ui->okButton->setEnabled(false);

    ui->summSpinBox->setRange(-1000000, 1000000);

    connect(ui->from_account, SIGNAL(currentIndexChanged(int)), SLOT(check_Ok()));
    connect(ui->to_Account, SIGNAL(currentIndexChanged(int)), SLOT(check_Ok()));
    connect(ui->summSpinBox, SIGNAL(valueChanged(double)), SLOT(check_Ok()));
    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

EditOperation::~EditOperation()
{
    delete ui;
}

void EditOperation::check_Ok()
{
    if (ui->from_account->value() > 0 && ui->to_Account->value() > 0 && ui->summSpinBox->value() > 0)
        ui->okButton->setEnabled(true);
    else
        ui->okButton->setEnabled(false);
}

void EditOperation::data(operation_data &d)
{
    d.from = ui->from_account->value();
    d.to   = ui->to_Account->value();
    d.summ = ui->summSpinBox->value();
    d.date = ui->dateEdit->value();
    d.descr = ui->descrEdit->text();
}

void EditOperation::setdata(operation_data &d)
{
    ui->from_account->setValue(d.from);
    ui->to_Account->setValue(d.to);
    ui->summSpinBox->setValue(d.summ);
}
