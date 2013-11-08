#include "editoperation.h"
#include "ui_editoperation.h"
#include "global.h"

EditOperation::EditOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOperation)
{
    ui->setupUi(this);
    QString from, to;

    db = new Database;

    list = db->get_currency_list();

    from = db->get_account_scod(ui->from_account->value());
    to = db->get_account_scod(ui->to_Account->value());

    ui->from_cod->setText(from);
    ui->to_cod->setText(to);
    ui->from_to->setText(from + "->" + to);

//    ui->cancelButton->setIcon(QPixmap(":icons/block_32.png"));
    ui->okButton->setEnabled(false);

    ui->warning->setVisible(false);
//    ui->kursEdit->setVisible(false);
//    ui->label_7->setVisible(false);

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
    QString from, to;

    if (ui->from_account->value() > 0 && ui->to_Account->value() > 0 && ui->summSpinBox->value() > 0)
        ui->okButton->setEnabled(true);
    else
        ui->okButton->setEnabled(false);

    from = db->get_account_scod(ui->from_account->value());
    to = db->get_account_scod(ui->to_Account->value());

    ui->from_cod->setText(from);
    ui->to_cod->setText(to);
    ui->from_to->setText(from + "->" + to);

    ui->kursEdit->setValue(list[from]/list[to]);
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
    QString from_sym = db->get_account_scod(ui->from_account->value());
    QString to_sym   = db->get_account_scod(ui->to_Account->value());

    d.from = ui->from_account->value() * list[from_sym];
    d.to   = ui->to_Account->value() * list[to_sym];
    d.agent = ui->agent_comboBox->value();
    d.kurs = ui->kursEdit->value();
    d.summ_from = ui->summSpinBox->value();
    d.summ_to = ui->summSpinBox->value() * ui->kursEdit->value();
    d.date = ui->dateEdit->value();
    d.descr = ui->descrEdit->text();
}

void EditOperation::setdata(operation_data &d)
{
    ui->from_account->setValue(d.from);
    ui->to_Account->setValue(d.to);
    ui->agent_comboBox->setValue(d.agent);
    ui->kursEdit->setValue(d.kurs);
    ui->summSpinBox->setValue(d.summ_from);
    ui->descrEdit->setText(d.descr);
}
