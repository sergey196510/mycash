#include "listcurrency.h"
#include "ui_listcurrency.h"

ListCurrency::ListCurrency(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCurrency)
{
    ui->setupUi(this);

    ui->newButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);

    connect(ui->nameEdit, SIGNAL(textChanged(QString)), SLOT(check_new_button(QString)));
    connect(ui->symbolEdit, SIGNAL(textChanged(QString)), SLOT(check_symbol(QString)));
}

ListCurrency::~ListCurrency()
{
    delete ui;
}

void ListCurrency::check_new_button(QString str)
{
    ui->newButton->setEnabled(str.length());
}

void ListCurrency::check_symbol(QString str)
{
    QLocale locale;

    locale = str;

}
