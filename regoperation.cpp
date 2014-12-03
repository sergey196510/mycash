#include "regoperation.h"
#include "ui_regoperation.h"

RegOperation::RegOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegOperation)
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

RegOperation::~RegOperation()
{
    delete ui;
}

Operation RegOperation::data()
{
    return ui->widget->data();
}

void RegOperation::setData(Operation & d)
{
    ui->widget->setdata(d);
}
