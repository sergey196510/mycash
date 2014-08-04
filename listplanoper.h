#ifndef LISTPLANOPER_H
#define LISTPLANOPER_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include "database.h"
#include "global.h"
#include "editoperation.h"
#include "transaction.h"

namespace Ui {
class ListPlanOper;
}

class ListPlanOperModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ListPlanOperModel(Database *db, QObject *parent = 0);
    ~ListPlanOperModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

public slots:
    void change_data();

private:
    Globals *var;
    QStringList header_data;
    QList<Operation_Data> list;
    QMap<int,QString> acc_list;
    QMap<QString,double> currency;
    QString symbol;
    Database *db;

private slots:
    QList<Operation_Data> read_list();
};

class ListPlanOper : public QWidget
{
    Q_OBJECT

public:
    explicit ListPlanOper(QWidget *parent = 0);
    ~ListPlanOper();
    QAction *tran, *comm, *delo, *can;
    QList<QAction *> acts;

public slots:
    void reload_model();
    void clear_model();

private:
    Ui::ListPlanOper *ui;
    ListPlanOperModel *model;
    QString query;
    Database *db;

private slots:
    void new_oper();
    int get_selected_id();
    void commit_oper();
    void del_oper();
    void cancel_oper();
    void check_selected();

signals:
    void data_changed();
};

#endif // LISTPLANOPER_H
