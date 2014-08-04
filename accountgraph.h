#ifndef ACCOUNTGRAPH_H
#define ACCOUNTGRAPH_H

#include <QWidget>
#include <QTextEdit>
#include <QPainter>

class AccountGraph : public QWidget
{
    Q_OBJECT
public:
    explicit AccountGraph(QWidget *parent = 0);

private:
    int width, height;

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent *pe);
};

#endif // ACCOUNTGRAPH_H
