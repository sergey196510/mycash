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

class ListPlanOperModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ListPlanOperModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;

private:
    QStringList header_data;
    QMap<int,QString> list;
    Database *db;
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
    QAction *tran, *comm, *delo;

private slots:
    void new_oper();
    int get_selected_id();
    void commit_oper();
    void del_oper();
    void check_selected();
};

#endif // LISTPLANOPER_H
