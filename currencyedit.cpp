#include "currencyedit.h"

currencyEdit::currencyEdit(QLineEdit *parent) :
    QLineEdit(parent)
{
}

double currencyEdit::value()
{
    return this->text().toDouble();
}
