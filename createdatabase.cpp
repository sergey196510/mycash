#include "createdatabase.h"
#include "ui_createdatabase.h"

CreateDatabase::CreateDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDatabase)
{
    ui->setupUi(this);
}

CreateDatabase::~CreateDatabase()
{
    delete ui;
}
