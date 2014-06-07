#ifndef LISTCURRENCY_H
#define LISTCURRENCY_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include "global.h"

namespace Ui {
class ListCurrency;
}

class ListCurrencyModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QStringList header_data;
    QList<Currency_Data> list;

private slots:
    QList<Currency_Data> read_list();

public:
    explicit ListCurrencyModel(QObject *parent = 0);
    ~ListCurrencyModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

public slots:
    void changed_data();
};

class ListCurrency : public QWidget
{
    Q_OBJECT

public:
    explicit ListCurrency(QWidget *parent = 0);
    ~ListCurrency();

public slots:
    void reload_model();
    void clear_model();

private:
    Ui::ListCurrency *ui;
    ListCurrencyModel *model;
    int get_selected_id();
    Globals var;

private slots:
    void check_new_button(QString);
    void check_symbol(QString);
    void new_currency();
    void update_currency();
    void delete_currency();
    void clear_currency();
    void check_select();

signals:
    void data_change();
};

#endif // LISTCURRENCY_H
