#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QtGui>
#include <QPainter>
#include <QtSql>
#include "database.h"
#include "widgets/mydateedit.h"

class graphWidget : public QWidget
{
public:
    graphWidget(QWidget *parent = 0);

private:
    Database db;
    QMap<QString,double> list, list2;
    int count;
    double summ;
    int image_width, image_height;

protected:
    virtual void paintEvent(QPaintEvent *pe);
};

#endif // GRAPHWIDGET_H
