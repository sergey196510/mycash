#ifndef ACCOUNTSVIEW_H
#define ACCOUNTSVIEW_H

#include <QTreeView>
#include "listaccountsmodel.h"

class accountsView : public QTreeView
{
    Q_OBJECT

private:
    ListAccountsModel *model;
//    QModelIndex index;
    QMap<int,QModelIndex> index_list;

public:
    explicit accountsView(QWidget *parent = nullptr);
    ~accountsView();
    int value();
    void setValue(int);
    void reload();

signals:

public slots:

};

#endif // ACCOUNTSVIEW_H
