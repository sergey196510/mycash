#include "currencyedit.h"
#include "global.h"

currencyEdit::currencyEdit(QWidget *parent) :
    QLineEdit(parent)
{
    QDoubleValidator *val = new QDoubleValidator(this);
    setValidator(val);
}

double currencyEdit::value()
{
    return default_locale->toDouble(this->text());
}

void currencyEdit::setValue(double value)
{
    this->setText(default_locale->toString(value));
}
