#include "updatedescrioption.h"

updateDescrioption::updateDescrioption(QString lbl, QString descr, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    this->label_2->setText(lbl);
    this->descrEdit->setText(descr);
}
