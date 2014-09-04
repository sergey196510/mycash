#ifndef LISTOPERATIONS_H
#define LISTOPERATIONS_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include "database.h"
#include "editoperation.h"
#include "widgets/mydateedit.h"
#include "listaccountsmodel.h"
#include "accountsview.h"

namespace Ui {
class ListOperations;
}

class ListOperationsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ListOperationsModel(int account, QDate fdate, QDate ldate, QObject *parent = 0);
    ~ListOperationsModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const { if (parent.isValid()) return 0; return list.size(); }
    int columnCount(const QModelIndex &parent) const { if (parent.isValid()) return 0; return header_data.size(); }
//    QModelIndex index(int row, int column, const QModelIndex &parent) const;
//    QModelIndex parent(const QModelIndex &child) const;
    enum {
        col_Id = 0,
        col_Date = 1,
        col_Account = 2,
        col_Debet = 3,
        col_Credit = 4,
        col_Descr = 5
    };

public slots:
//    void fill_model(QDate *dt1, QDate *dt2, int id = 0);
    void reload_data(int account, QDate fdate, QDate ldate);

private:
    Database *db;
    Globals *var;
    QStringList header_data;
    QMap<int,QString> accounts_list;
    QList<Operation_Data> list;
    QList<Operation_Data> read_list(int account, QDate fdate, QDate ldate);
    int current_account;
};

class ListOperations : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListOperations(QWidget *parent = 0);
    ~ListOperations();
    QAction *debt;
    QAction *cred;
    QAction *tran;
    QAction *plan;
    QAction *dele;
    QList<QAction*> acts;

public slots:
    void reload_model();
    void clear_model();
    
private:
    Ui::ListOperations *ui;
    ListOperationsModel *model;
    ListAccountsModel *accts;
    QString query;
    Database *db;
    Globals var;
//    operation_data d;
//    QMap<QString,double> list;
    void edit_operation(Operation_Data &);

signals:
    void call_reload_table(int account, QDate fdate, QDate ldate);

private slots:
    void debet_operation();
    void credit_operation();
    void transfer_operation();
    void print_balance();
    void del_operation();
    void plann_operation();
};

#endif // LISTOPERATIONS_H
