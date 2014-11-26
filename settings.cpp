#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    QFont fnt;
    Account data;
    ui->setupUi(this);
    db = new Database;

    ui->sizeBox->setValue(fnt.SizeResolved);
    ui->correctWidget->setValue(var.CorrectAccount());
    ui->initialWidget->setValue(var.InitialAccount());
//    ui->correctBox->setValue(var.CorrectAccount());
//    data = db->get_account_data(var.CorrectAccount());
//    ui->correctLine->setText(data.name);
//    data = db->get_account_data(var.InitialAccount());
//    ui->initialLine->setText(data.name);
//    ui->initialBox->setValue(var.InitialAccount());
    ui->precisionBox->setValue(var.Precision());
    ui->precisionBox->setMinimumSize(ui->precisionBox->sizeHint());

    ui->cancelButton->setText(tr("Cancel Alt+Q"));
    ui->cancelButton->setShortcut(tr("Alt+Q"));

    ui->checkBox->setChecked(var.ProxyEnable());
    ui->serverEdit->setText(var.ProxyServer());
    ui->portEdit->setText(tr("%1").arg(var.ProxyPort()));
    ui->userEdit->setText(var.ProxyUser());
    ui->passwordEdit->setText(var.ProxyPassword());

    proxy_click();

//    connect(ui->correctButton, SIGNAL(clicked()), SLOT(correct_account()));
//    connect(ui->initialButton, SIGNAL(clicked()), SLOT(initial_account()));
    connect(ui->acceptButton, SIGNAL(clicked()), SLOT(accept_values()));
    connect(ui->checkBox, SIGNAL(clicked()), SLOT(proxy_click()));
}

Settings::~Settings()
{
    delete db;
    delete ui;
}

void Settings::accept_values()
{
    ui->fontComboBox->itemText(ui->fontComboBox->currentIndex());
    ui->sizeBox->value();
    ui->boldButton->isChecked();
    var.setCorrectAccount(ui->correctWidget->value());
    var.setInitialAccount(ui->initialWidget->value());
    var.setPrecision(ui->precisionBox->value());

    var.setProxyEnable(ui->checkBox->isChecked());
    var.setProxyServer(ui->serverEdit->text());
    var.setProxyPort(ui->portEdit->text().toInt());
    var.setProxyUser(ui->userEdit->text());
    var.setProxyPassword(ui->passwordEdit->text());
}

void Settings::check_down()
{
    ui->boldButton->setDown(isdown);
    isdown = !isdown;
}

void Settings::proxy_click()
{
    ui->label_5->setEnabled(ui->checkBox->isChecked());
    ui->label_6->setEnabled(ui->checkBox->isChecked());
    ui->label_7->setEnabled(ui->checkBox->isChecked());
    ui->label_8->setEnabled(ui->checkBox->isChecked());
    ui->serverEdit->setEnabled(ui->checkBox->isChecked());
    ui->portEdit->setEnabled(ui->checkBox->isChecked());
    ui->userEdit->setEnabled(ui->checkBox->isChecked());
    ui->passwordEdit->setEnabled(ui->checkBox->isChecked());
}
