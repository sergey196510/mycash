#include "accountgraph.h"

AccountGraph::AccountGraph(QWidget *parent) :
    QWidget(parent)
{
}

void AccountGraph::paintEvent(QPaintEvent *pe)
{
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.initFrom(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.eraseRect(rect());

    width = image.width();
    height = image.height();

    painter.setBrush(QBrush(Qt::white, Qt::DiagCrossPattern));
    painter.setPen(QPen(Qt::blue, 1, Qt::DashLine));
    painter.drawRect(QRect(10,10,width-20,height-20));

    painter.drawText(10,10,"ttt");

    painter.end();

    painter.begin(this);
    painter.drawImage(0,0,image);
    painter.end();
}
