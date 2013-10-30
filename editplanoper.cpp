#include "editplanoper.h"
#include "ui_editplanoper.h"

editPlanOper::editPlanOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editPlanOper)
{
    QList<QString> days;
    QList<QString> months;
    ui->setupUi(this);

    for (int i = 1; i < 32; i++)
        days.append(QString("%1").arg(i));

    months <<
              "Any" <<
              QDate::longMonthName(1) <<
              QDate::longMonthName(2) <<
              QDate::longMonthName(3) <<
              QDate::longMonthName(4) <<
              QDate::longMonthName(5) <<
              QDate::longMonthName(6) <<
              QDate::longMonthName(7) <<
              QDate::longMonthName(8) <<
              QDate::longMonthName(9) <<
              QDate::longMonthName(10) <<
              QDate::longMonthName(11) <<
              QDate::longMonthName(12);

    ui->dayBox->addItems(days);
    ui->monthBox->addItems(months);

    ui->cancelButton->setIcon(QPixmap(":icons/block_32.png"));
    ui->okButton->setEnabled(false);

    connect(ui->dayBox,       SIGNAL(currentIndexChanged(int)), SLOT(check_ok()));
    connect(ui->fromBox,      SIGNAL(currentIndexChanged(int)), SLOT(check_ok()));
    connect(ui->toBox,        SIGNAL(currentIndexChanged(int)), SLOT(check_ok()));
    connect(ui->curr_Edit, SIGNAL(textChanged(QString)),     SLOT(check_ok()));
    connect(ui->okButton,     SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

editPlanOper::~editPlanOper()
{
    delete ui;
}

PlanOper_data editPlanOper::Value()
{
    PlanOper_data po;

    po.day = ui->dayBox->currentText().toInt();
    po.month = ui->monthBox->currentText().toInt();
    po.year = ui->yearBox->value();
    po.from = ui->fromBox->value();
    po.to = ui->toBox->value();
    po.summ = ui->curr_Edit->value();
    po.descr = ui->descrEdit->text();

    return po;
}

void editPlanOper::setValue(PlanOper_data &data)
{
    ui->dayBox->setCurrentIndex(data.day);
    ui->monthBox->setCurrentIndex(data.month);
    ui->yearBox->setValue(data.year);
    ui->fromBox->setValue(data.from);
    ui->toBox->setValue(data.to);
    ui->curr_Edit->setValue(data.summ);
    ui->descrEdit->setText(data.descr);
}

void editPlanOper::check_ok()
{
    if (ui->dayBox->currentText().toInt() > 0 &&
            ui->fromBox->value() > 0 &&
            ui->toBox->value() > 0 &&
            ui->curr_Edit->value() > 0)
        ui->okButton->setEnabled(true);
    else
        ui->okButton->setEnabled(false);
}
