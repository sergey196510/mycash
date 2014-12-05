#include "graphwidget.h"

graphWidget::graphWidget(QWidget *parent) :
    QWidget(parent)
{
    QSqlQuery q;
    QDate current = QDate::currentDate();
    QDate first(current.year(), current.month(), 1);
    QMap<QString,double>::iterator i;

    summ = 0;

    db = new Database;

//    list2 = db->get_opersummbyaccount_list(Account_Type::credit, current.month(), current.year());
    for (i = list2.begin(); i != list2.end(); i++) {
        summ += i.value();
    }
    for (i = list2.begin(); i != list2.end(); i++) {
        list[i.key()] = i.value()*100/summ;
    }

    count = list.size();
}

graphWidget::~graphWidget()
{
    delete db;
}

void graphWidget::paintEvent(QPaintEvent *)
{
    qreal x = 0, y = 0;
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
        painter.drawText(250,k,i.key()+": "+
//                         QString("%1").arg(list2[i.key()])+
                default_locale->toString(list2[i.key()],'f',2));
        l+=5;
        k += 15;
    }

    painter.end();

    painter.begin(this);
    painter.drawImage(0,0,image);
    painter.end();
}
