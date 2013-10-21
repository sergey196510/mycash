#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QtGui>
#include <QPainter>

class graphWidget : public QWidget
{
public:
    graphWidget(QWidget *parent = 0);

private:
    int image_width, image_height;

protected:
    virtual void paintEvent(QPaintEvent *pe);
};

#endif // GRAPHWIDGET_H
