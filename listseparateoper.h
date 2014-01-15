#ifndef LISTSEPARATEOPER_H
#define LISTSEPARATEOPER_H

#include <QtGui>
#include <QDialog>
#include "database.h"
#include "accountoper2.h"

namespace Ui {
class ListSeparateOper;
}

class AccountDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    AccountDelegate(int column, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void commitAndCloseEditor();

private:
    int column;
};

class ListSeparateOperModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ListSeparateOperModel(QObject *parent = 0);
    QMap<int,double> getData();

private:
    struct OperList {
        int account;
        double summ;
    };
    QStringList header_data;
    QList<OperList> list;
    QMap<int,QString>  acc_list;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
};

class ListSeparateOper : public QDialog
{
    Q_OBJECT

public:
    explicit ListSeparateOper(QWidget *parent = 0);
    ~ListSeparateOper();
    QMap<int,double> data();

private:
    Ui::ListSeparateOper *ui;
    ListSeparateOperModel *model;
};

#endif // LISTSEPARATEOPER_H
