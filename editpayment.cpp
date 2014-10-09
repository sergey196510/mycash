#include "editpayment.h"
#include "ui_editpayment.h"

EditPayment::EditPayment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPayment)
{
    ui->setupUi(this);

    connect(ui->oldEdit, SIGNAL(editingFinished()),
            this, SLOT(calc_summ()));
    connect(ui->newEdit, SIGNAL(editingFinished()),
            this, SLOT(calc_summ()));
    connect(ui->priceEdit, SIGNAL(editingFinished()),
            this, SLOT(calc_summ()));

    connect(ui->okButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

EditPayment::~EditPayment()
{
    delete ui;
}

void EditPayment::calc_summ()
{
    double oval, nval, prval, summ;

    oval = ui->oldEdit->text().toDouble();
    nval = ui->newEdit->text().toDouble();
    prval = ui->priceEdit->text().toDouble();
    summ = (nval - oval) * prval;
    ui->number_value->setText(QString("%1").arg(nval-oval));
    ui->summ_value->setText(default_locale->toCurrencyString(summ));
}
