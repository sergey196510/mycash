#include "graphwidget.h"

graphWidget::graphWidget(QWidget *parent) :
    QWidget(parent)
{
}

void graphWidget::paintEvent(QPaintEvent *pe)
{
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.initFrom(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.eraseRect(rect());

    image_width = image.width();
    image_height = image.height();
}
