#include "selectaccount.h"
#include "ui_selectaccount.h"

SelectAccount::SelectAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectAccount)
{
    db = new Database;

    ui->setupUi(this);

    connect(ui->treeView, SIGNAL(activated(QModelIndex)), SLOT(accept()));
    connect(ui->toolButton, SIGNAL(clicked()), SLOT(new_account()));
}

SelectAccount::~SelectAccount()
{
    delete ui;
    delete db;
}

void SelectAccount::setValue(int idx)
{
    ui->treeView->setValue(idx);
}

int SelectAccount::value()
{
    return ui->treeView->value();
}

void SelectAccount::new_account()
{
    EditAccount *acc = new EditAccount(1, this);
    Account data;

    if (acc->exec() == QDialog::Accepted) {
        data = acc->data();
        Account parent;
        parent.read(data.Parent());
        data.setTop(parent.Top());
        if (data.insert())
            ui->treeView->reload();
    }
}
