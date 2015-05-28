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
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int column;
};

class ListSeparateOperModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ListSeparateOperModel(QObject *parent = 0);
    QList<account_summ> getData();

public slots:
//    void updateData();

private:
    QStringList header_data;
    QList<account_summ> list;
//    QMap<int,QString> acc_list;
//    QMap<int,QString>
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
};

class ListSeparateOper : public QDialog
{
    Q_OBJECT

public:
    explicit ListSeparateOper(QWidget *parent = 0);
    ~ListSeparateOper();
    QList<account_summ> data();

private:
    Ui::ListSeparateOper *ui;
    ListSeparateOperModel *model;
};

#endif // LISTSEPARATEOPER_H
