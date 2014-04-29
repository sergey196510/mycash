#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QWidget>
#include <QtGui/QTreeView>

class myTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit myTreeView(QWidget *parent = 0);

signals:

public slots:

};

#endif // MYTREEVIEW_H
