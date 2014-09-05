#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>

class MyTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MyTableView(QWidget *parent = 0);
    virtual int get_selected_id();

protected:

signals:

public slots:

};

#endif // MYTABLEVIEW_H
