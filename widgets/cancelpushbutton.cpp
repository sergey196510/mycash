#include "widgets/cancelpushbutton.h"

CancelPushButton::CancelPushButton(QWidget *parent) :
    QPushButton(parent)
{
    setIcon(QPixmap(":icons/block_32.png"));
}
