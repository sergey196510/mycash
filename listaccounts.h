#ifndef LISTACCOUNTS_H
#define LISTACCOUNTS_H

#include <QWidget>
#include "QtSql"
#include "database.h"

namespace Ui {
class ListAccounts;
}

class ListAccountsModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ListAccountsModel(QObject *parent = 0);
};

class ListAccounts : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListAccounts(QWidget *parent = 0);
    ~ListAccounts();

public slots:
    void new_account();
    
private:
    Ui::ListAccounts *ui;
    ListAccountsModel *model;
    QString query;
    Database db;
};

#endif // LISTACCOUNTS_H
