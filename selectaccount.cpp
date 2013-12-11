#include "selectaccount.h"
#include "ui_selectaccount.h"

SelectAccount::SelectAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectAccount)
{
    ui->setupUi(this);

    connect(ui->treeView, SIGNAL(activated(QModelIndex)), SLOT(accept()));
}

SelectAccount::~SelectAccount()
{
    delete ui;
}

void SelectAccount::setValue(int idx)
{
    ui->treeView->setValue(idx);
}

int SelectAccount::value()
{
    return ui->treeView->value();
}
