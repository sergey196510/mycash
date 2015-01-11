#include "updatedescrioption.h"

updateDescrioption::updateDescrioption(QString lbl, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    this->label_2->setText(lbl);
}
