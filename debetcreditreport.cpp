#include "debetcreditreport.h"
#include "ui_debetcreditreport.h"

DebetCreditReport::DebetCreditReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebetCreditReport)
{
    QStringList months;
    ui->setupUi(this);

    months << "";
    for (int i = 1; i <= 12; i++)
        months << QDate::shortMonthName(i);
//    qDebug() <<months;

    db = new Database;

    ui->monthBox->addItems(months);
    ui->monthBox->setCurrentIndex(QDate::currentDate().month());
    ui->yearBox->setRange(2000, 2020);
    ui->yearBox->setValue(QDate::currentDate().year());
//    ui->fdate->setDate(QDate().currentDate().addDays(-29));

    connect(ui->calcButton, SIGNAL(clicked()), SLOT(recalc_report()));

    calc_report(ui->monthBox->currentIndex(), ui->yearBox->value());
}

DebetCreditReport::~DebetCreditReport()
{
    delete db;
    delete ui;
}

void DebetCreditReport::recalc_report()
{
    calc_report(ui->monthBox->currentIndex(), ui->yearBox->value());
}

void DebetCreditReport::calc_report(int month, int year)
{
    Account acc;
    QMap<QString,double> list;
    QMap<QString,double>::iterator i;
    double summ;

    ui->textEdit->clear();
    ui->textEdit->append("Dohodi:");

    summ = 0;
//    list = db->get_opersummbyaccount_list(Account_Type::debet, month, year);
    for (i = list.begin(); i != list.end(); i++) {
        ui->textEdit->append(i.key() + ": " + default_locale->toString(i.value(),'f',2));
        summ += i.value();
    }
    ui->textEdit->append("Total: " + default_locale->toString(summ,'f',2));

    ui->textEdit->append("-----------------------------------------------");
    ui->textEdit->append("Rashodi:");

    summ = 0;
//    list = db->get_opersummbyaccount_list(Account_Type::credit, month, year);
    for (i = list.begin(); i != list.end(); i++) {
        ui->textEdit->append(i.key() + ": " + default_locale->toString(i.value(),'f',2));
        summ += i.value();
    }
    ui->textEdit->append("Total: " + default_locale->toString(summ,'f',2));

    ui->textEdit->append("-----------------------------------------------");

    acc.read(1);
    summ = acc.get_summ();
    ui->textEdit->append("Balance: " + default_locale->toString(summ,'f',2));
}
