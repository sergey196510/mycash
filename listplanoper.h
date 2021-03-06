#ifndef LISTPLANOPER_H
#define LISTPLANOPER_H

using namespace std;

#include <vector>
#include <QObject>
#include <QtGui>
#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include "database.h"
#include "global.h"
#include "editoperation.h"
#include "editplanoperation.h"
#include "regoperation.h"
#include "transaction.h"

namespace Ui {
class ListPlanOper;
}

class ListPlanOperModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ListPlanOperModel(Database *db, QObject *parent = nullptr);
    ~ListPlanOperModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    int get_ident(int row);

public slots:
    void change_data();

private:
    Globals *var;
    QStringList header_data;
    vector<PlanOperation> list;
//    QMap<int,QString> acc_list;
    QMap<QString,double> currency;
    QString symbol;
    Database *db;

private slots:
    vector<PlanOperation> read_list();
};

class ListPlanOper : public QWidget
{
    Q_OBJECT

public:
    explicit ListPlanOper(QWidget *parent = nullptr);
    ~ListPlanOper();
    QAction *tran, *comm, *delo, *upd, *can;
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
    QList<int> get_selected_id();
    void commit_oper();
    void update_oper();
    void del_oper();
    void cancel_oper();
    void check_selected();

signals:
    void data_changed();
};

#endif // LISTPLANOPER_H
