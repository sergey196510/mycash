#ifndef LISTACCOUNTSMODEL_H
#define LISTACCOUNTSMODEL_H

#include <QtGui>
#include "database.h"

class ViewCurrency : public QItemDelegate
{
    Q_OBJECT

public:
    ViewCurrency(int column, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int column;
};

class ListAccountsModel : public QStandardItemModel
{
    Q_OBJECT

private:
//    QModelIndex curr_index;
    Database *db;
    QStringList header_data;
    QMap<int,QString> list;
    QMap<int,QModelIndex> list_index;
    QMap<QString,double> list_curs;
    Globals var;

public:
    explicit ListAccountsModel(QObject *parent = 0);
    ~ListAccountsModel();
    double get_list(int, QModelIndex);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;

public slots:
    QMap<int,QModelIndex> fill_model();
};

#endif // LISTACCOUNTSMODEL_H
