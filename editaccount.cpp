#include "editaccount.h"
#include "ui_editaccount.h"

EditAccount::EditAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAccount)
{
    ui->setupUi(this);
}

EditAccount::~EditAccount()
{
    delete ui;
}
