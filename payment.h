#ifndef PAYMENT_H
#define PAYMENT_H

#include <QObject>
#include "widgets/mycurrency.h"

class Payment
{
    QString descr;
    double oldValue;
    double newValue;
    double number;
    MyCurrency price;
    double cost;
public:
    Payment() {
        descr.clear();
        oldValue = 0;
        newValue = 0;
        number = 0;
        cost = 0;
    }
    void setValue(double, double, double, double, QString);
};

#endif // PAYMENT_H
