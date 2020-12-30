#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QWidget>
#include <QTreeView>
#include <QHeaderView>

class MyTableView : public QTreeView
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
