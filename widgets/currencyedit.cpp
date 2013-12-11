#include "currencyedit.h"
#include "global.h"

currencyEdit::currencyEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setValidator(new QDoubleValidator);
}

double currencyEdit::value()
{
//    qDebug() << this->text() << default_locale->toDouble(this->text());
    return default_locale->toDouble(this->text());
}

void currencyEdit::setValue(double value)
{
    this->setText(default_locale->toString(value,'f',2));
}
