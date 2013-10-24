#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidgetModel::MainWidgetModel(QObject *parent) :
    QStandardItemModel(parent)
{
    db = new Database;
    list = db->get_plan_oper_list();
    QList<PlanOper_data>::iterator i;
    PlanOper_data data;

    for (i = list.begin(); i != list.end(); i++) {
        data = *i;
    }
}

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    db = new Database;
    model = new MainWidgetModel;

    ui->groupBox->setTitle(tr("Balance status"));
    ui->groupBox_2->setTitle(tr("Prosrochennie operations"));

    ui->active_value->setText(default_locale->toString(db->get_account_summ(active)) + " " + default_locale->currencySymbol(default_locale->CurrencyIsoCode));
    ui->passive_value->setText(default_locale->toString(db->get_account_summ(passive)) + " " + default_locale->currencySymbol(default_locale->CurrencyIsoCode));
    ui->debet_value->setText(default_locale->toString(db->get_operation_summ(debet)) + " " + default_locale->currencySymbol(default_locale->CurrencyIsoCode));
    ui->credit_value->setText(default_locale->toString(db->get_operation_summ(credit)) + " " + default_locale->currencySymbol(default_locale->CurrencyIsoCode));

    ui->treeView->setModel(model);
}

MainWidget::~MainWidget()
{
    delete model;
    delete db;
    delete ui;
}
