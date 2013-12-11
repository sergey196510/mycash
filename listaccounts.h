#ifndef LISTACCOUNTS_H
#define LISTACCOUNTS_H

#include <QtGui>
#include <QWidget>
#include "QtSql"
#include "database.h"
#include "global.h"
#include "listaccountsmodel.h"
#include "summaccount.h"

namespace Ui {
class ListAccounts;
}

class ListAccounts : public QWidget
{
    Q_OBJECT

public:
    explicit ListAccounts(QWidget *parent = 0);
    ~ListAccounts();

public slots:
    void clear_model();

protected:
    void keyPressEvent(QKeyEvent *);

signals:
    void pressEnter();
    void pressInsert();
    void pressDelete();

private slots:
//    void fill_model();
    void reload_model();
    void new_account();
    void change_account();
    void correct_balance();
    void del_account();
//    void check_type();
    int get_selected_id();

private:
    Ui::ListAccounts *ui;
    ListAccountsModel *model;
//    QString query;
    Database *db;
    Globals var;
};

#endif // LISTACCOUNTS_H
