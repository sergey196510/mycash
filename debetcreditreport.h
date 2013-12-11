#ifndef DEBETCREDITREPORT_H
#define DEBETCREDITREPORT_H

#include <QDialog>
#include "database.h"

namespace Ui {
class DebetCreditReport;
}

class DebetCreditReport : public QDialog
{
    Q_OBJECT

public:
    explicit DebetCreditReport(QWidget *parent = 0);
    ~DebetCreditReport();

private:
    Ui::DebetCreditReport *ui;
    Database *db;

private slots:
    void calc_report(int, int);
    void recalc_report();
};

#endif // DEBETCREDITREPORT_H
