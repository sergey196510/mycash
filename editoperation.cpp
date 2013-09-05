#include "editoperation.h"
#include "ui_editoperation.h"

EditOperation::EditOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOperation)
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

EditOperation::~EditOperation()
{
    delete ui;
}

void EditOperation::data(operation_data &d)
{
    d.from = ui->from_account->value();
    d.to   = ui->to_Account->value();
    d.summ = ui->summSpinBox->value();
    d.date = ui->dateEdit->value();
}
