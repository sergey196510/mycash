#include "widgets/okpushbutton.h"

OkPushButton::OkPushButton(QWidget *parent) :
    QPushButton(parent)
{
    setIcon(QPixmap(":icons/accept.png"));
}
