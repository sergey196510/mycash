#include "accountgraph.h"

AccountGraph::AccountGraph(QWidget *parent) :
    QWidget(parent)
{
    isFree = true;
    db = new Database;
}

AccountGraph::~AccountGraph()
{
    delete db;
}

void AccountGraph::calc_array(int id)
{
    QSqlQuery q, q2;
    Account_Data data;
    QList<SbD> stack;
    QList<SbD> summ;
    QMap<QDate,double> result;

//    qDebug() << id;
    if (id == 0) {
        isFree = true;
        return;
    }

    isFree = false;

    data = db->get_account_data(id);

    // выборка значений операций по счету
    q.prepare("SELECT id,dt FROM oper ORDER BY dt");
    if (!q.exec()) {
        qDebug() << q.lastError();
        isFree = true;
        return;
    }
    while (q.next()) {
        q2.prepare("SELECT summ, direction FROM account_oper WHERE o_id = :oid AND a_id = :aid");
        q2.bindValue(":oid", q.value(0).toInt());
        q2.bindValue(":aid", id);
        if (!q2.exec()) {
            qDebug() << q2.lastError();
            isFree = true;
            return;
        }
        while (q2.next()) {
            SbD val;
            if (q2.value(1).toInt() == 2)
                val.value = -q2.value(0).toDouble();
            else
                val.value = q2.value(0).toDouble();
            val.dt = q.value(1).toDate();
            stack.append(val);
        }
    }

    // расчет баланса по счету на момент совершения операции
    // итоговые значения расположены в массиве result
    SbD val;
    val.value = data.balance.value();
    val.dt = QDate::currentDate();
    summ.append(val);
    for (int j = stack.size()-1; j >= 0; j--) {
        SbD v = stack.at(j);
        val.value += v.value;
        val.dt = v.dt;
        summ.append(val);
    }

    int days = 0;
    double all = 0;
    for (int j = summ.size()-1; j > 0; j--) {
        val.dt = summ.at(j).dt;
        val.value = summ.at(j-1).value;
        result[val.dt] = val.value;
        days++;
        all += val.value;
    }
    average = all/days;
    qDebug() << average;

    QDate ldate = QDate::currentDate();
    QDate pdate = ldate.addDays(-29);
    double prev = 0;
    QDate dt;
    list.clear();
    for (dt = pdate; dt <= ldate; dt = dt.addDays(1)) {
        SbD val;
        if (result.contains(dt)) {
            prev = result[dt];
        }
        val.dt = dt;
        val.value = prev;
//        qDebug() << val.dt << val.value;
        list.append(val);
    }

    isFree = true;
    this->update();
}

void AccountGraph::paintEvent(QPaintEvent *)
{
    double max = 0;
    qreal x, y;
    QList<SbD>::iterator i;

    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    if (isFree == false) {
        qDebug() << "false";
        painter.end();
        return;
    }

    // пустой список
    if (list.size() == 0) {
        painter.end();
        return;
    }

    width = image.width();
    height = image.height();
    if (height > width/2)
        height = width/2;

    painter.initFrom(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.eraseRect(rect());

    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine));
    painter.drawRect(QRect(5,5,width-10,height-10));

    for (i = list.begin(); i != list.end(); i++) {
        SbD val = *i;
        if (val.value > max)
            max = val.value;
    }

//    qreal step = (width-20)/30;
    x = 5;
    int j = 0;
    QPoint a[30];
    painter.setPen(QPen(Qt::gray, 0, Qt::DotLine));
    for (i = list.begin(); i != list.end(); i++) {
        SbD val = *i;
        y = (val.value * (height-20) / max) + 10;
        x = (j * (width-20)/30)+10;
        painter.drawLine(QPointF(x,height-5), QPointF(x,5));
        painter.drawLine(QPointF(5,height-y), QPointF(width-10,height-y));
        a[j] = QPoint(x,height-y);
        j++;
    }
    painter.setPen(QPen(Qt::blue, 0, Qt::SolidLine));
    painter.drawPolyline(a,30);

    painter.setPen(QPen(Qt::red, 0, Qt::SolidLine));
    y = (average*(height-20)/max)+10;
    painter.drawLine(QPointF(10,height-10), QPointF(width-10,y));

//    painter.drawText(10,10,"ttt");

    painter.end();

    painter.begin(this);
    painter.drawImage(0,0,image);
    painter.end();
}
