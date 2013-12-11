#ifndef LISTOPERATIONS_H
#define LISTOPERATIONS_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include "database.h"
#include "editoperation.h"
#include "widgets/mydateedit.h"
#include "listaccountsmodel.h"
#include "accountsview.h"

namespace Ui {
class ListOperations;
}

class ListOperationsModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit ListOperationsModel(QObject *parent = 0);
    ~ListOperationsModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;

public slots:
    void fill_model(QString *dt1, QString *dt2, int id = 0);

private:
    Database *db;
    Globals *var;
    QStringList header_data;
    QMap<int,QString> list;
};

class ListOperations : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListOperations(QWidget *parent = 0);
    ~ListOperations();

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
    QMap<QString,double> list;
    void edit_operation(operation_data &);

signals:
    void call_reload_table();

private slots:
    void debet_operation();
    void credit_operation();
    void transfer_operation();
    void change_current_account(int i = 0);
    void del_operation();
    void plann_operation();
    int get_selected_id();
};

#endif // LISTOPERATIONS_H
