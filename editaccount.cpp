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

Account EditAccount::data()
{
    Account d;

    d.setName(ui->nameEdit->text());
    d.setType(ui->typeBox->value());
    d.setCurr(ui->currencyBox->value());
    d.setBalance(ui->balanceSpinBox->value());
    d.setDescr(ui->descrEdit->text());
    d.setHidden(ui->hiddenBox->isChecked());
    d.setParent(ui->accountWidget->value());
    d.setDate(ui->dateEdit->value());

    return d;
}

void EditAccount::setData(Account &data)
{
    ui->nameEdit->setText(data.Name());
    ui->typeBox->setValue(data.Type());
    ui->currencyBox->setValue(data.Curr());
    ui->balanceSpinBox->setValue(data.Balance().toDouble());
    ui->descrEdit->setText(data.Descr());
    ui->hiddenBox->setChecked(data.Hidden());
    ui->accountWidget->setValue(data.Parent());
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
    Account data;
    data.read(ui->accountWidget->value());
//    qDebug() << data.top;
    if (data.Top() == 1) {
        ui->typeBox->setEnabled(true);
    }
    else
        ui->typeBox->setEnabled(false);
//    ui->typeBox->setValue(data.type);
}
