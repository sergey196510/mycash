#ifndef LISTACCOUNTSMODEL_H
#define LISTACCOUNTSMODEL_H

#include <QtGui>
#include "database.h"
#include "currency.h"
#include "operation.h"
#include "account.h"

class ViewCurrency : public QItemDelegate
{
    Q_OBJECT

public:
    ViewCurrency(int column, QObject *parent = 0);
//    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

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
//    QMap<int,QString> list;
    QMap<int,QModelIndex> list_index;
//    QMap<QString,double> list_curs;
    QVector<PlanOperation> plan_list;
    Globals var;
    MyCurrency get_reserv(int id);

public:
    explicit ListAccountsModel(QObject *parent = 0);
    ~ListAccountsModel();
    MyCurrency get_list(int, QModelIndex);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;

public slots:
    QMap<int,QModelIndex> fill_model();
};

class ListAccountsModel2 : public QAbstractTableModel
{
    Q_OBJECT

private:
    Database *db;
    QStringList header_data;
    QMap<int,QModelIndex> list_index;
    QVector<Account> list;
    QVector<Account> read_list();
    QVector<PlanOperation> plan_list;
    Globals var;
    MyCurrency get_reserv(int);

public:
    explicit ListAccountsModel2(QObject *parent = 0);
    ~ListAccountsModel2();
    MyCurrency get_list(int QModelIndex);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

public slots:
//    QMap<int,QModelIndex> fill_model();
};

#endif // LISTACCOUNTSMODEL_H
