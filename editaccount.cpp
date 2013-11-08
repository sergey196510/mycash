#include "QtSql"
#include "editaccount.h"
#include "ui_editaccount.h"

EditAccount::EditAccount(int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAccount)
{
    ui->setupUi(this);

    if (type == 2) {
        ui->typeBox->setEnabled(false);
        ui->currencyBox->setEnabled(false);
        ui->parentBox->setEnabled(false);
        ui->balanceSpinBox->setEnabled(false);
    }

    if (type != 2)
        connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(nameCheck(QString)));
    connect(ui->parentBox, SIGNAL(currentIndexChanged(int)), SLOT(parentCheck()));
    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));

//    ui->balanceSpinBox->setSuffix(default_locale->toCurrencyString(0));
//    ui->balanceSpinBox->setRange(-1000000, 1000000);
    ui->balanceSpinBox->setValue(0);

//    ui->cancelButton->setIcon(QPixmap(":icons/block_32.png"));
    if (type != 2)
        ui->okButton->setEnabled(false);
}

EditAccount::~EditAccount()
{
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
    d.parent = ui->parentBox->value();

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
    ui->parentBox->setValue(data.parent);
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

/*
bool EditAccount::hidden()
{
    return ui->hiddenBox->isChecked();
}
*/

void EditAccount::parentCheck()
{
    Account_Data data = db.get_account_data(ui->parentBox->value());
    ui->typeBox->setValue(data.type);
}
