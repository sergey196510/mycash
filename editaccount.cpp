#include "QtSql"
#include "editaccount.h"
#include "ui_editaccount.h"

EditAccount::EditAccount(int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAccount)
{
    ui->setupUi(this);

    db = new Database;

    if (type == 2) {
        ui->typeBox->setEnabled(false);
        ui->currencyBox->setEnabled(false);
        ui->accountWidget->setEnabled(false);
        ui->balanceSpinBox->setEnabled(false);
    }
    ui->okButton->setEnabled(false);

//    qDebug() << ui->accountWidget->value();

    if (type != 2) {
        connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(okCheck()));
        connect(ui->accountWidget, SIGNAL(changed_value()), SLOT(okCheck()));
    }
    connect(ui->accountWidget, SIGNAL(changed_value()), SLOT(parentCheck()));
    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));

    ui->balanceSpinBox->setValue(0);

    if (type != 2)
        ui->okButton->setEnabled(false);
}

EditAccount::~EditAccount()
{
    delete db;
    delete ui;
}

/*
QString EditAccount::name()
{
    return ui->nameEdit->text();
}

int EditAccount::type()
{
    return ui->typeBox->value();
}

int EditAccount::curr()
{
    return ui->currencyBox->value();
}

void EditAccount::set_curr(int id)
{
    ui->currencyBox->setValue(id);
}

void EditAccount::set_parent(int id)
{
    ui->parentBox->setValue(id);
}

double EditAccount::balance()
{
    return ui->balanceSpinBox->value();
}

QString EditAccount::descr()
{
    return ui->descrEdit->text();
}
*/

Account_Data EditAccount::data()
{
    Account_Data d;

    d.name = ui->nameEdit->text();
    d.type = ui->typeBox->value();
    d.curr = ui->currencyBox->value();
    d.balance = ui->balanceSpinBox->value();
    d.descr = ui->descrEdit->text();
    d.hidden = ui->hiddenBox->isChecked();
    d.parent = ui->accountWidget->value();
    d.dt = ui->dateEdit->value();

    return d;
}

void EditAccount::setData(Account_Data &data)
{
    ui->nameEdit->setText(data.name);
    ui->typeBox->setValue(data.type);
    ui->currencyBox->setValue(data.curr);
    ui->balanceSpinBox->setValue(data.balance);
    ui->descrEdit->setText(data.descr);
    ui->hiddenBox->setChecked(data.hidden);
    ui->accountWidget->setValue(data.parent);
    if (ui->accountWidget->value() != 1) {
        ui->typeBox->setEnabled(false);
    }
}

void EditAccount::okCheck()
{
    QString str = ui->nameEdit->text();

//    qDebug() << ui->accountWidget->value();

    if (str.length() > 0 && ui->accountWidget->value() > 0)
        ui->okButton->setEnabled(true);
    else
        ui->okButton->setEnabled(false);
}

bool EditAccount::nameFind(QString name)
{
    QSqlQuery query;

    query.prepare("SELECT name FROM account WHERE name = :name");
    query.bindValue(":name", name);
    if (!query.exec() || !query.next())
            return true;

    return false;
}

void EditAccount::parentCheck()
{
    Account_Data data = db->get_account_data(ui->accountWidget->value());
//    qDebug() << data.top;
    if (data.top == 1) {
        ui->typeBox->setEnabled(true);
    }
    else
        ui->typeBox->setEnabled(false);
//    ui->typeBox->setValue(data.type);
}
