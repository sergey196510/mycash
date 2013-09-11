#ifndef LISTOPERATIONS_H
#define LISTOPERATIONS_H

#include <QWidget>
#include <QtSql>
#include "database.h"

namespace Ui {
class ListOperations;
}

class ListOperationsModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ListOperationsModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
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
    Database db;

private slots:
    void new_operation();
    void change_current_account(int);
    void change_date();
};

#endif // LISTOPERATIONS_H
