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
    months << "Nothing" << "Januar" << "Februar" << "Mart" << "April" << "May" << "June" << "July" << "August" << "September" << "October" << "November" << "December";

    ui->dayBox->addItems(days);
    ui->monthBox->addItems(months);

    ui->okButton->setIcon(QPixmap(":icons/accept.png"));
    ui->cancelButton->setIcon(QPixmap(":icons/block_32.png"));
    ui->okButton->setEnabled(false);

    connect(ui->okButton,     SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));
}

editPlanOper::~editPlanOper()
{
    delete ui;
}
