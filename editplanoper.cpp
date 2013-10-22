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
    months << "Any" << "Januar" << "Februar" << "Mart" << "April" << "May" << "June" << "July" << "August" << "September" << "October" << "November" << "December";

    ui->dayBox->addItems(days);
    ui->monthBox->addItems(months);

    ui->okButton->setIcon(QPixmap(":icons/accept.png"));
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
