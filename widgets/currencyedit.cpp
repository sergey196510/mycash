#include "currencyedit.h"
#include "global.h"

currencyValidator::currencyValidator(QObject *parent) :
    QValidator(parent)
{
}

currencyEdit::currencyEdit(QWidget *parent) :
    QLineEdit(parent)
{
//    currencyValidator *val = new currencyValidator(this);
//    setValidator(val);
}

double currencyEdit::value()
{
    return default_locale->toDouble(this->text());
}

void currencyEdit::setValue(double value)
{
    this->setText(default_locale->toString(value,'f',2));
}
