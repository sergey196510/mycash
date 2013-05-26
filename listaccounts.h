#ifndef LISTACCOUNTS_H
#define LISTACCOUNTS_H

#include <QWidget>
#include "QtSql"

namespace Ui {
class ListAccounts;
}

class ListAccountsModel : public QSqlTableModel
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
    
private:
    Ui::ListAccounts *ui;
};

#endif // LISTACCOUNTS_H
