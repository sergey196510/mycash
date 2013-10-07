#ifndef CURRENCYEDIT_H
#define CURRENCYEDIT_H

#include <QtGui>

class currencyEdit : public QLineEdit
{
public:
    currencyEdit(QDialog *parent = 0);
    double value();
    void setValue(double);
};

#endif // CURRENCYEDIT_H
