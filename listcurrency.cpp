#include "listcurrency.h"
#include "ui_listcurrency.h"

ListCurrency::ListCurrency(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCurrency)
{
    ui->setupUi(this);
}

ListCurrency::~ListCurrency()
{
    delete ui;
}
