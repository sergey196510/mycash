#ifndef LISTSEPARATEOPER_H
#define LISTSEPARATEOPER_H

#include <QtGui>
#include <QDialog>

namespace Ui {
class ListSeparateOper;
}

class ListSeparateOperModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ListSeparateOperModel(QObject *parent = 0)
        :QAbstractListModel(parent) {;}
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    struct OperList {
        int account;
        double summ;
    };
    QList<OperList> list;
};

class ListSeparateOper : public QDialog
{
    Q_OBJECT

public:
    explicit ListSeparateOper(QWidget *parent = 0);
    ~ListSeparateOper();

private:
    Ui::ListSeparateOper *ui;
    ListSeparateOperModel *model;
};

#endif // LISTSEPARATEOPER_H
