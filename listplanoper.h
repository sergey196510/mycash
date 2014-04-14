#ifndef LISTPLANOPER_H
#define LISTPLANOPER_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include "database.h"
#include "global.h"
#include "editoperation.h"

namespace Ui {
class ListPlanOper;
}

class ListPlanOperModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit ListPlanOperModel(QObject *parent = 0);
    ~ListPlanOperModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;

public slots:
    void fill_model();

private:
    Globals *var;
    QStringList header_data;
    QMap<int,QString> list;
    QMap<QString,double> currency;
    QString symbol;
    Database *db;

private slots:
    bool find_operations(int);
};

class ListPlanOper : public QWidget
{
    Q_OBJECT

public:
    explicit ListPlanOper(QWidget *parent = 0);
    ~ListPlanOper();
    QAction *tran, *comm, *delo;
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
    void check_selected();
};

#endif // LISTPLANOPER_H
