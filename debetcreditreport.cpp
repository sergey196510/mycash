#include "debetcreditreport.h"
#include "ui_debetcreditreport.h"

DebetCreditReport::DebetCreditReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebetCreditReport)
{
    QStringList months;
    ui->setupUi(this);

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
    QMap<QString,double> list;
    QMap<QString,double>::iterator i;
    double summ;

    ui->textEdit->clear();
    ui->textEdit->append("Dohodi:");

    summ = 0;
    list = db->get_opersummbyaccount_list(Account_Type::debet, month, year);
    for (i = list.begin(); i != list.end(); i++) {
        ui->textEdit->append(i.key() + ": " + default_locale->toString(i.value(),'f',2));
        summ += i.value();
    }
    ui->textEdit->append("Total: " + default_locale->toString(summ,'f',2));

    ui->textEdit->append("-----------------------------------------------");
    ui->textEdit->append("Rashodi:");

    summ = 0;
    list = db->get_opersummbyaccount_list(Account_Type::credit, month, year);
    for (i = list.begin(); i != list.end(); i++) {
        ui->textEdit->append(i.key() + ": " + default_locale->toString(i.value(),'f',2));
        summ += i.value();
    }
    ui->textEdit->append("Total: " + default_locale->toString(summ,'f',2));

    ui->textEdit->append("-----------------------------------------------");

    summ = db->get_account_summ(1);
    ui->textEdit->append("Balance: " + default_locale->toString(summ,'f',2));
}
