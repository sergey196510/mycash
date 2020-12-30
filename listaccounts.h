#ifndef LISTACCOUNTS_H
#define LISTACCOUNTS_H

#include <QAction>
#include <QtGui>
#include <QWidget>
#include "QtSql"
#include <QDate>
#include <QMessageBox>
#include "database.h"
#include "global.h"
#include "listaccountsmodel.h"
#include "summaccount.h"
#include "transaction.h"

namespace Ui {
class ListAccounts;
}

class ListAccounts : public QWidget
{
    Q_OBJECT

public:
    explicit ListAccounts(Database *d, QWidget *parent = nullptr);
    ~ListAccounts();
    QAction *nacct;
    QAction *chacc;
    QAction *cacct;
    QAction *dacct;
    QAction *analis;
    QList<QAction*> acts;

public slots:
    void clear_model();

protected:
    void keyPressEvent(QKeyEvent *);

signals:
    void pressEnter();
    void pressInsert();
    void pressDelete();
    void select_account(int id);

private slots:
//    void fill_model();
    void reload_model();
    void new_account();
    void change_account();
    void correct_balance();
    void del_account();
//    void check_type();
    int get_selected_id();
    void show_analis();
    void select_account();

private:
    Ui::ListAccounts *ui;
    ListAccountsModel *model;
//    QString query;
    Database *db;
    Globals var;
};

#endif // LISTACCOUNTS_H
