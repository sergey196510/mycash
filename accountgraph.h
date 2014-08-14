#ifndef ACCOUNTGRAPH_H
#define ACCOUNTGRAPH_H

#include <QWidget>
#include <QTextEdit>
#include <QPainter>
#include <QtSql>
#include "global.h"
#include "database.h"

struct SbD {
    double value;
    QDate dt;
    SbD() {
        value = 0;
        dt.currentDate();
    }
};

class AccountGraph : public QWidget
{
    Q_OBJECT
public:
    explicit AccountGraph(QWidget *parent = 0);
    ~AccountGraph();

public slots:
    void calc_array(int id);

private:
    int width, height;
    Database *db;
    QList<SbD> list;
    bool isFree;

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent *pe);
};

#endif // ACCOUNTGRAPH_H
