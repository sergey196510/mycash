#include "graphwidget.h"

graphWidget::graphWidget(QWidget *parent) :
    QWidget(parent)
{
    QSqlQuery q;

    q.prepare("SELECT acc_to, summ FROM operation");

    list["First"] = 20;
    list["Two"] = 30;
    list["Three"] = 50;
    count = list.size();
}

void graphWidget::paintEvent(QPaintEvent *pe)
{
    int x = 0, y = 0;
    QMap<QString,double>::iterator i;
    QList<QString> cols = QColor::colorNames();

    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.initFrom(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.eraseRect(rect());

    image_width = image.width();
    image_height = image.height();

    int k = 20;
    int l = 5;
    for (i = list.begin(); i != list.end(); i++) {
        x += y;
        y = 360*(i.value())/100;
        painter.setBrush(QBrush(cols.at(l), Qt::DiagCrossPattern));
        painter.drawPie(QRect(10,10,220,220), x*16, y*16);
        painter.drawText(250,k,i.key()+": "+QString("%1").arg(i.value())+" "+cols.at(l));
        l++;
        k += 15;
    }
//    x += y;
//    y = 360*(40)/100;
//    painter.setBrush(QBrush(Qt::blue, Qt::DiagCrossPattern));
//    painter.drawPie(QRect(10,10,220,220), x*16, y*16);
//    x += y;
//    y = 360*(50)/100;
//    painter.setBrush(QBrush(Qt::cyan, Qt::DiagCrossPattern));
//    painter.drawPie(QRect(10,10,220,220), x*16, y*16);

    painter.end();

    QPainter widgetPainter(this);
    widgetPainter.drawImage(0,0,image);
}
