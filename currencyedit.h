#ifndef CURRENCYEDIT_H
#define CURRENCYEDIT_H

#include <QtGui>

class currencyEdit : public QLineEdit
{
public:
    currencyEdit(QLineEdit *parent = 0);
    double value();
};

#endif // CURRENCYEDIT_H
