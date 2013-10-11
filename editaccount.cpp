#include "QtSql"
#include "editaccount.h"
#include "ui_editaccount.h"
#include "global.h"

EditAccount::EditAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAccount)
{
    ui->setupUi(this);

    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(nameCheck(QString)));
    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));

//    ui->balanceSpinBox->setSuffix(default_locale->toCurrencyString(0));
//    ui->balanceSpinBox->setRange(-1000000, 1000000);
    ui->balanceSpinBox->setValue(0);

    ui->okButton->setIcon(QPixmap(":icons/accept.png"));
    ui->cancelButton->setIcon(QPixmap(":icons/block_32.png"));
    ui->okButton->setEnabled(false);
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

int EditAccount::curr()
{
    return ui->currencyBox->value();
}

void EditAccount::set_curr(int id)
{
    ui->currencyBox->setValue(id);
}

double EditAccount::balance()
{
    return ui->balanceSpinBox->value();
}

QString EditAccount::descr()
{
    return ui->descrEdit->text();
}

Account_Data EditAccount::data()
{
    Account_Data d;

    d.name = ui->nameEdit->text();
    d.type = ui->typeBox->value();
    d.curr = ui->currencyBox->value();
    d.balance = ui->balanceSpinBox->value();
    d.descr = ui->descrEdit->text();

    return d;
}

void EditAccount::nameCheck(QString str)
{
    if (str.length() > 0 && nameFind(str) == true)
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

bool EditAccount::hidden()
{
    return ui->hiddenBox->isEnabled();
}
