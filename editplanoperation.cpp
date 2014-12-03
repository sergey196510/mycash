#include "editplanoperation.h"
#include "ui_editplanoperation.h"

EditPlanOperation::EditPlanOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPlanOperation)
{
    ui->setupUi(this);

    ui->typeBox->addItem(tr("Month"));
    ui->typeBox->addItem(tr("Year"));
    ui->dayBox->setRange(0,31);

    changeDay();
    changeType();

    connect(ui->typeBox, SIGNAL(currentIndexChanged(int)), SLOT(changeType()));
    connect(ui->dayBox, SIGNAL(valueChanged(int)), SLOT(changeDay()));

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

EditPlanOperation::~EditPlanOperation()
{
    delete ui;
}

PlanOperation EditPlanOperation::Data()
{
    Operation oper;
    PlanOperation item;

    oper = ui->widget->data();
    item.setAgent(oper.Agent());
    item.setDate(oper.Date());
    item.setDescr(oper.Descr());
    item.setFrom(oper.From());
    item.setId(oper.Id());
    item.setKurs(oper.Kurs());
    item.setTo(oper.To());
    item.setDay(ui->dayBox->value());
    item.setMonth(ui->monthBox->currentIndex());

    return item;
}

void EditPlanOperation::setData(PlanOperation &d)
{
    ui->dayBox->setValue(d.Day());
    ui->monthBox->setCurrentIndex(d.Month());
    ui->widget->setdata(d);
}

void EditPlanOperation::changeType()
{
    ui->label_2->setVisible(ui->typeBox->currentIndex());
    ui->monthBox->setVisible(ui->typeBox->currentIndex());
}

void EditPlanOperation::changeDay()
{
    ui->okButton->setEnabled(ui->dayBox->value());
}
