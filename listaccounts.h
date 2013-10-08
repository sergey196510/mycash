#ifndef LISTACCOUNTS_H
#define LISTACCOUNTS_H

#include <QtGui>
#include <QWidget>
#include "QtSql"
#include "database.h"
#include "global.h"

namespace Ui {
class ListAccounts;
}

class ListAccountsModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ListAccountsModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class ListAccounts : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListAccounts(QWidget *parent = 0);
    ~ListAccounts();

private slots:
    void new_account();
    void correct_balance();
    void check_type();
    
private:
    Ui::ListAccounts *ui;
    ListAccountsModel *model;
    QString query;
    Database db;
};

#endif // LISTACCOUNTS_H
