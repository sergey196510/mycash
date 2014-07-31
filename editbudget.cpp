#include "editbudget.h"
#include "ui_editbudget.h"

EditBudget::EditBudget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBudget)
{
    ui->setupUi(this);

    connect(ui->okButton,     SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

EditBudget::~EditBudget()
{
    delete ui;
}

Budget_Data EditBudget::data()
{
    Budget_Data d;

    d.mon = ui->monthEdit->text().toInt();
    d.account = ui->accountEdit->value();
    d.summ = ui->summEdit->value();

    return d;
}
