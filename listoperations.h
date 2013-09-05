#ifndef LISTOPERATIONS_H
#define LISTOPERATIONS_H

#include <QWidget>
#include <QtSql>
#include "database.h"

namespace Ui {
class ListOperations;
}

class ListOperations : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListOperations(QWidget *parent = 0);
    ~ListOperations();
    
private:
    Ui::ListOperations *ui;
    QString query;
    Database db;

private slots:
    void new_operation();
    void change_current_account(int);
};

#endif // LISTOPERATIONS_H
