#include "currencyedit.h"
#include "global.h"

currencyEdit::currencyEdit(QDialog *parent) :
    QLineEdit(parent)
{
}

double currencyEdit::value()
{
    return this->text().toDouble();
}

void currencyEdit::setValue(double value)
{
    this->setText(default_locale->toCurrencyString(value));
}
