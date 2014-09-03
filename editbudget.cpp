#include "editbudget.h"
#include "ui_editbudget.h"

EditBudget::EditBudget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBudget)
{
    ui->setupUi(this);

    for (int i = 1; i <= 12; i++) {
        ui->comboBox->addItem(QDate::shortMonthName(i));
    }

    connect(ui->okButton,     SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

EditBudget::~EditBudget()
{
    delete ui;
}

Budget EditBudget::data()
{
    Budget d;

    d.set_Value(0, ui->comboBox->currentIndex()+1, ui->accountEdit->value(), ui->summEdit->value());

    return d;
}
