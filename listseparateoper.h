#ifndef LISTSEPARATEOPER_H
#define LISTSEPARATEOPER_H

#include <QtGui>
#include <QDialog>
#include "database.h"

namespace Ui {
class ListSeparateOper;
}

class ListSeparateOperModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ListSeparateOperModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    QMap<int,double> getData();

private:
    struct OperList {
        int account;
        double summ;
    };
    QStringList header_data;
    QList<OperList> list;
    QMap<int,QString>  acc_list;
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
