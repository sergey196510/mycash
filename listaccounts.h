#ifndef LISTACCOUNTS_H
#define LISTACCOUNTS_H

#include <QWidget>
#include "QtSql"

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
    QString query = "SELECT a.id,a.name,t.name,a.balance FROM account a, account_type t WHERE a.type = t.id";
};

#endif // LISTACCOUNTS_H
