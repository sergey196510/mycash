#ifndef CURRENCYEDIT_H
#define CURRENCYEDIT_H

#include <QLineEdit>
#include <QValidator>

class currencyEdit : public QLineEdit
{
public:
    currencyEdit(QWidget *parent = 0);
    double value();
    void setValue(double);
};

#endif // CURRENCYEDIT_H
