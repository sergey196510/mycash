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
    QStack<SbD> stack1, stack2, stack3;
//    QList<SbD> summ;
//    QMap<QDate,MyCurrency> result;
    QDate ldate = QDate::currentDate();
    QDate pdate = ldate.addDays(-29);

//    qDebug() << id;
    if (id == 0) {
        isFree = true;
        return;
    }

    isFree = false;

    list.clear();
    data = db->get_account_data(id);
    if (data.top != Account_Type::active) {
        update();
        return;
    }

//    qDebug() << "stack1:";
    // выборка значений операций по счету
    q.prepare("SELECT id,dt FROM oper WHERE dt >= :dt ORDER BY dt");
    q2.prepare("SELECT summ, direction FROM account_oper WHERE o_id = :oid AND a_id = :aid");

    q.bindValue(":dt", pdate.toString("yyyy-MM-dd"));
    if (!q.exec()) {
        qDebug() << q.lastError();
        isFree = true;
        return;
    }
    while (q.next()) {
        q2.bindValue(":oid", q.value(0).toInt());
        q2.bindValue(":aid", id);
        if (!q2.exec()) {
            qDebug() << q2.lastError();
            isFree = true;
            return;
        }
        while (q2.next()) {
            SbD val;
            if (q2.value(1).toInt() == Account_Type::debet || q2.value(1).toInt() == Account_Type::passive)
                val.value = -q2.value(0).toDouble();
            else
                val.value = q2.value(0).toDouble();
            val.dt = q.value(1).toDate();
//            qDebug() << val.dt << val.value.toDouble();
            stack1.push(val);
        }
    }

    if (stack1.empty()) {
        isFree = true;
        this->update();
        return;
    }

    // инициализация итогового массива
//    qDebug() << "stack2:";
    QDate dt;
    for (dt = pdate; dt <= ldate; dt = dt.addDays(1)) {
        SbD val;
        val.dt = dt;
        val.value = 0;
//        qDebug() << val.dt << val.value.toDouble();
        stack2.push(val);
    }

    qDebug() << stack2.size() << stack3.size();

    //расчет баланса по дням от текущего
    MyCurrency balance = data.balance;
    SbD val1 = stack1.pop();
    QDate dt1 = val1.dt;
    SbD val2 = stack2.pop();
    QDate dt2 = val2.dt;
    SbD val;
    val.dt = dt2;
    val.value = balance;
    stack3.push(val);
    while (1) {
        if (stack2.empty()) {
//            val.dt = dt2;
//            val.value = balance;
//            qDebug() << val.dt << val.value.toDouble();
//            stack3.push(val);
            break;
        }
        if (dt2 > dt1) {
            val.dt = dt2;
            val.value = balance;
//            qDebug() << val.dt << val.value.toDouble();
            stack3.push(val);
            val2 = stack2.pop();
            dt2 = val2.dt;
        }
        else if (dt2 == dt1) {
            balance += val1.value;
            if (!stack1.empty()) {
                val1 = stack1.pop();
                dt1 = val1.dt;
            }
            else
                dt1 = pdate.addDays(-1);
        }
        else if (stack1.empty())
            dt1 = pdate.addDays(-1);
    }

    qDebug() << stack2.size() << stack3.size();

    while (!stack3.empty()) {
        list.append(stack3.pop());
    }

    isFree = true;
    this->update();
}

void AccountGraph::paintEvent(QPaintEvent *)
{
    MyCurrency max = 0;
    qreal x, y;
    QList<SbD>::iterator i;

//    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
//    QPainter painter(&image);
    QPainter painter(this);

    if (isFree == false) {
        qDebug() << "false";
        painter.end();
        return;
    }

    w = width();
    h = height();
//    width = image.width();
//    height = image.height();
    if (h > w/2+30)
        h = w/2;
    else
        h = w-30;

    painter.setViewport(0,0,w,h);

    w = 600;
    h = 300;
    painter.setWindow(0,0,w,h);

    painter.initFrom(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.eraseRect(rect());

    // рисуем рамку
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine));
    painter.drawRect(QRect(5,5,w-10,h-10));

    // пустой список
    if (list.size() == 0) {
        painter.end();
        return;
    }

    SbD f, l;
    for (i = list.begin(); i != list.end(); i++) {
        SbD val = *i;
        if (f.value.isNull() && !val.value.isNull())
            f = val;
        l = val;
        if (val.value > max)
            max = val.value;
    }

    painter.drawLine(QPointF(30,h-30), QPointF(30,10));
    painter.drawLine(QPointF(30,h-30), QPointF(w-10,h-30));
    painter.drawText(QPoint(250,h-10),tr("Date"));

    x = 5;
    int j = 0;
    QPoint a[30];
    MyCurrency summ = 0;
    MyCurrency average = 0;
    painter.setPen(QPen(Qt::black, 0, Qt::DotLine));
    for (i = list.begin(); i != list.end(); i++) {
        SbD val = *i;
//        qDebug() << val.dt << val.value.toDouble();
        summ += val.value;
        y = (val.value * (h-40) / max) + 30;
        x = (j * (w-40)/(30-1))+30;
        if (j%5 == 0) {
            painter.drawLine(QPointF(x,h-30), QPointF(x,h-25));
            painter.drawText(x,h-20,val.dt.toString("dd-MM-yyyy"));
        }
        a[j] = QPoint(x,h-y);
        j++;
    }

    painter.setPen(QPen(Qt::blue, 0, Qt::SolidLine));
    painter.drawPolyline(a,30);

    painter.setPen(QPen(Qt::red, 0, Qt::SolidLine));
    average = summ/30;
    y = (average*(h-20)/max)+10;
    painter.drawLine(a[0], QPoint(a[29].x(),h-y));

    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine));
    painter.drawText(10,h+10, tr("Saldo"));
    painter.drawText(10,h+21, tr("From: %1; to: %2")
                     .arg(default_locale->toCurrencyString(f.value.toDouble()))
                     .arg(default_locale->toCurrencyString(l.value.toDouble())));

    painter.translate(10,100);
    painter.rotate(90);
//    painter.translate(-10,-100);
    painter.drawText(0,0,tr("Summ"));

    painter.end();

//    painter.begin(this);
//    painter.drawImage(0,0,image);
//    painter.end();
}
