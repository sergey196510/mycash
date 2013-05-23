#include "listaccounts.h"
#include "ui_listaccounts.h"

ListAccounts::ListAccounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAccounts)
{
    ui->setupUi(this);
}

ListAccounts::~ListAccounts()
{
    delete ui;
}
