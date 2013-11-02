#ifndef CURRENCYEDIT_H
#define CURRENCYEDIT_H

#include <QtGui>

class currencyValidator : public QValidator
{
public:
    currencyValidator(QObject *);

    virtual State validate(QString &str, int &pos) const
    {
        QRegExp rxp = QRegExp("[0-9]");
        if (!str.contains(rxp)) {
            return Invalid;
        }
        return Acceptable;
    }
};

class currencyEdit : public QLineEdit
{
public:
    currencyEdit(QWidget *parent = 0);
    double value();
    void setValue(double);
};

#endif // CURRENCYEDIT_H
