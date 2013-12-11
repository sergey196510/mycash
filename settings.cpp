#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    QFont fnt;
    Account_Data data;
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

//    connect(ui->correctButton, SIGNAL(clicked()), SLOT(correct_account()));
//    connect(ui->initialButton, SIGNAL(clicked()), SLOT(initial_account()));
    connect(ui->acceptButton, SIGNAL(clicked()), SLOT(accept_values()));
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
}

void Settings::check_down()
{
    ui->boldButton->setDown(isdown);
    isdown = !isdown;
}

/*
void Settings::correct_account()
{
//    SelectAccount *selacc = new SelectAccount(ui->correctLine);

    selacc->setWindowTitle(tr("Correct Account"));
    selacc->setValue(var.CorrectAccount());
    if (selacc->exec() == QDialog::Accepted) {
        Account_Data data = db->get_account_data(selacc->value());
//        ui->correctLine->setText(data.name);
        var.setCorrectAccount(selacc->value());
    }
}

void Settings::initial_account()
{
    SelectAccount *iniacc = new SelectAccount(ui->initialLine);

    iniacc->setWindowTitle(tr("Initial Account"));
    iniacc->setValue(var.InitialAccount());
    if (iniacc->exec() == QDialog::Accepted) {
        Account_Data data = db->get_account_data(iniacc->value());
        ui->initialLine->setText(data.name);
        var.setInitialAccount(iniacc->value());
    }
}
*/
