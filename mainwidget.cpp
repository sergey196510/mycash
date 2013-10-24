#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    db = new Database;

    ui->groupBox->setTitle(tr("Balance status"));

    ui->active_value->setText(default_locale->toCurrencyString(db->get_account_summ(1)));
    ui->passive_value->setText(default_locale->toCurrencyString(db->get_account_summ(2)));
}

MainWidget::~MainWidget()
{
    delete db;
    delete ui;
}
