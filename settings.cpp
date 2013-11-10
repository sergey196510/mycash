#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    ui->comboBox->setValue(var.CorrectAccount());
    ui->precisionBox->setValue(var.Precision());

    connect(ui->acceptButton, SIGNAL(released()), SLOT(accept_values()));
    connect(ui->okButton,     SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::accept_values()
{
    var.setCorrectAccount(ui->comboBox->value());
    var.setPrecision(ui->precisionBox->value());
}
