#ifndef LISTACCOUNTSMODEL_H
#define LISTACCOUNTSMODEL_H

#include <vector>
#include <QtGui>
#include "database.h"
#include "currency.h"
#include "operation.h"
#include "account.h"
#include <vector>

using namespace std;

struct Node {
    int row;
    Account acc;
    Node *parent;
    vector<Node*> children;
    bool mapped;
    bool operator==(const Node n1) const {
        return n1.acc.Id() == this->acc.Id();
    }
};

class ViewCurrency : public QItemDelegate
{
    Q_OBJECT

public:
    ViewCurrency(int column, QObject *parent = nullptr);
//    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int column;
};

class ListAccountsModel : public QAbstractItemModel
{
    Q_OBJECT

private:
//    QModelIndex curr_index;
    Database *db;
    QStringList header_data;
//    QMap<int,QString> list;
    QMap<int,QModelIndex> list_index;
//    QMap<QString,double> list_curs;
    vector<PlanOperation> plan_list;
    QVector<Node> nodes;
    Globals var;
    MyCurrency get_reserv(int id);

public:
    explicit ListAccountsModel(QObject *parent = nullptr);
    ~ListAccountsModel();
//    MyCurrency get_list(int, QModelIndex);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
//    int findRow(Node *nodeInfo) const;

public slots:
    QMap<int,QModelIndex> fill_model(Node *p, const int parent);
};

#endif // LISTACCOUNTSMODEL_H
