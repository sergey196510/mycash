#ifndef LISTCURRENCY_H
#define LISTCURRENCY_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include "global.h"

namespace Ui {
class ListCurrency;
}

class ListCurrencyModel : public QSqlQueryModel
{
    Q_OBJECT

private:
    QStringList header_data;

public:
    explicit ListCurrencyModel(QObject *parent = 0);
    ~ListCurrencyModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
};

class ListCurrency : public QWidget
{
    Q_OBJECT

public:
    explicit ListCurrency(QWidget *parent = 0);
    ~ListCurrency();

private:
    Ui::ListCurrency *ui;
    ListCurrencyModel *model;
    QString query;
    int get_selected_id();

private slots:
    void check_new_button(QString);
    void check_symbol(QString);
    void new_currency();
    void update_currency();
    void delete_currency();
    void clear_currency();
    void check_select();
};

#endif // LISTCURRENCY_H
