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

class ListAccountsModel : public QStandardItemModel
{
    Q_OBJECT

private:
    Database *db;
    QStringList header_data;
    QMap<int,QString> list;

public:
    explicit ListAccountsModel(QObject *parent = 0);
    ~ListAccountsModel();
    double get_list(int);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
};

class ListAccounts : public QWidget
{
    Q_OBJECT

public:
    explicit ListAccounts(QWidget *parent = 0);
    ~ListAccounts();

public slots:
    void clear_list();

private slots:
//    void fill_model();
    void reload_model();
    void new_account();
    void correct_balance();
    void del_account();
    void check_type();
    int get_selected_id();

private:
    Ui::ListAccounts *ui;
    ListAccountsModel *model;
//    QString query;
    Database db;
};

#endif // LISTACCOUNTS_H
