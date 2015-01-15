#ifndef UPDATEDESCRIOPTION_H
#define UPDATEDESCRIOPTION_H

#include "ui_updatedescrioption.h"

class updateDescrioption : public QDialog, private Ui::updateDescrioption
{
    Q_OBJECT

public:
    explicit updateDescrioption(QString lbl, QString descr, QWidget *parent = 0);
    QString Description() { return descrEdit->text(); }
};

#endif // UPDATEDESCRIOPTION_H
