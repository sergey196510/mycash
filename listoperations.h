#ifndef LISTOPERATIONS_H
#define LISTOPERATIONS_H

#include <QWidget>
#include <QtSql>
#include "database.h"
#include "editoperation.h"

namespace Ui {
class ListOperations;
}

class ListOperationsModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ListOperationsModel(QObject *parent = 0);
    ~ListOperationsModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    Database *db;
};

class ListOperations : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListOperations(QWidget *parent = 0);
    ~ListOperations();
    
private:
    Ui::ListOperations *ui;
    ListOperationsModel *model;
    QString query;
    Database *db;
    EditOperation eo;
    operation_data d;
    void edit_operation(int);

signals:
    void call_reload_table();

private slots:
    void debet_operation();
    void credit_operation();
    void transfer_operation();
    void select_list_operations();
    void change_current_account(int);
    void select_font();
    void reload_table();
};

#endif // LISTOPERATIONS_H
