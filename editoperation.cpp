#include "editoperation.h"
#include "ui_editoperation.h"

EditOperation::EditOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOperation)
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

EditOperation::~EditOperation()
{
    delete ui;
}
