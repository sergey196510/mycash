#include "editoperation.h"
#include "ui_editoperation.h"
#include "global.h"

EditOperation::EditOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOperation)
{
    ui->setupUi(this);

    db = new Database;

    ui->cancelButton->setIcon(QPixmap(":icons/block_32.png"));
    ui->okButton->setEnabled(false);

    ui->warning->setVisible(false);

//    ui->summSpinBox->setRange(-1000000, 1000000);

    connect(ui->from_account, SIGNAL(currentIndexChanged(int)), SLOT(check_Ok()));
    connect(ui->to_Account, SIGNAL(currentIndexChanged(int)), SLOT(check_Ok()));
    connect(ui->summSpinBox, SIGNAL(textChanged(QString)), SLOT(check_Ok()));
    connect(ui->summSpinBox, SIGNAL(textChanged(QString)), SLOT(check_balance(QString)));
    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

EditOperation::~EditOperation()
{
    delete ui;
    delete db;
}

void EditOperation::check_Ok()
{
    if (ui->from_account->value() > 0 && ui->to_Account->value() > 0 && ui->summSpinBox->value() > 0)
        ui->okButton->setEnabled(true);
    else
        ui->okButton->setEnabled(false);
}

void EditOperation::check_balance(QString value)
{
    double balance;
    Account_Data data = db->get_account_data(ui->from_account->value());

    if (data.type != 1)
        return;

//    balance = db->get_account_balance(ui->from_account->value());
    if (value.toDouble() > data.balance)
        ui->warning->setVisible(true);
    else
        ui->warning->setVisible(false);

//    qDebug() << value << default_locale->toDouble(value) << balance;
//        qDebug() << value << balance << "Balance is small";
}

void EditOperation::data(operation_data &d)
{
    d.from = ui->from_account->value();
    d.to   = ui->to_Account->value();
    d.agent = ui->agent_comboBox->value();
    d.summ = ui->summSpinBox->value();
    d.date = ui->dateEdit->value();
    d.descr = ui->descrEdit->text();
}

void EditOperation::setdata(operation_data &d)
{
    ui->from_account->setValue(d.from);
    ui->to_Account->setValue(d.to);
    ui->agent_comboBox->setValue(d.agent);
    ui->summSpinBox->setValue(d.summ);
    ui->descrEdit->setText(d.descr);
}
