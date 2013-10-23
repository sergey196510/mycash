#ifndef LISTPLANOPER_H
#define LISTPLANOPER_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include <QAbstractItemModel>
#include "database.h"
#include "global.h"

namespace Ui {
class ListPlanOper;
}

class Node
{
public:
    enum Type { Root, OrExpression, AndExpression, NotExpression, Atom,
              Identifier, Operator, Punctuator};
    Node(Type type, const QString &str = "");
    ~Node();
    Type type;
    QString str;
    Node *parent;
    QList<Node*> children;
};

class ListPlanOperModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ListPlanOperModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

private:
    QStringList header_data;
    QMap<int,QString> list;
    Database *db;
    Node *nodeFromIndex(const QModelIndex &index) const;
    Node *rootNode;
};

class ListPlanOper : public QWidget
{
    Q_OBJECT

public:
    explicit ListPlanOper(QWidget *parent = 0);
    ~ListPlanOper();

private:
    Ui::ListPlanOper *ui;
    ListPlanOperModel *model;
    QString query;
    Database *db;

private slots:
    void new_oper();
    int get_selected_id();
    void del_oper();
};

#endif // LISTPLANOPER_H
