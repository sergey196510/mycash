#ifndef EDITPAYMENT_H
#define EDITPAYMENT_H

#include <QDialog>
#include "global.h"

namespace Ui {
class EditPayment;
}

class EditPayment : public QDialog
{
    Q_OBJECT

public:
    explicit EditPayment(QWidget *parent = 0);
    ~EditPayment();

private:
    Ui::EditPayment *ui;

private slots:
    void calc_summ();
};

#endif // EDITPAYMENT_H
