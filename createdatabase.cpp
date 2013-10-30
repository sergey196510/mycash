#include "createdatabase.h"
#include "ui_createdatabase.h"

CreateDatabase::CreateDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDatabase)
{
    ui->setupUi(this);

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), SLOT(check_ok(QString)));
    connect(ui->okButton, SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
    ui->okButton->setEnabled(false);
}

CreateDatabase::~CreateDatabase()
{
    delete ui;
}

void CreateDatabase::check_ok(QString str)
{
    ui->okButton->setEnabled(str.length());
}

QString CreateDatabase::name()
{
    return ui->lineEdit->text();
}
