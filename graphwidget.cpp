#include "graphwidget.h"

graphWidget::graphWidget(QWidget *parent) :
    QWidget(parent)
{
    QSqlQuery q;
    QDate current = QDate::currentDate();
    QDate first(1, current.month(), current.year());
    QMap<QString,double>::iterator i;

    summ = 0;

//    list["First"] = 20;
//    list["Two"] = 30;
//    list["Three"] = 50;

    QString str = "SELECT a.name, sum(o.summ) FROM account a, operation o WHERE o.dt >= '%1-%2-01' AND a.type = 4 AND o.acc_to = a.id GROUP BY a.name";
    QString query = str.arg(current.year()).arg(current.month());
    if (!q.exec(query)) {
        return;
    }
    while (q.next()) {
        list2[q.value(0).toString()] = q.value(1).toDouble();
        summ += q.value(1).toDouble();
    }
    for (i = list2.begin(); i != list2.end(); i++) {
        list[i.key()] = i.value()*100/summ;
    }

    count = list.size();
}

void graphWidget::paintEvent(QPaintEvent *pe)
{
    double x = 0, y = 0;
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
        painter.setPen(QPen(cols.at(l)));
        painter.drawPie(QRect(10,10,220,220), x*16, y*16);
        painter.drawText(250,k,i.key()+": "+QString("%1").arg(list2[i.key()]));
        l+=5;
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
