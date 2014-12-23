#include "payment.h"

void Payment::setValue(double oldval, double newval, double prval, double num, QString d)
{
    oldValue = oldval;
    newValue = newval;
    price = prval;
    number = num;
    cost = (newValue-oldValue)*price.toDouble();
    descr = d;
}
