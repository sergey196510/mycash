#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    ui->accountBox->setValue(var.CorrectAccount());

    connect(ui->okButton, SIGNAL(released()), SLOT(save_vars()));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::save_vars()
{
    var.setCorrectAccount(ui->accountBox->value());
}
