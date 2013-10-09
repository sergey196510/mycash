#ifndef LISTCURRENCY_H
#define LISTCURRENCY_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include "global.h"

namespace Ui {
class ListCurrency;
}

class ListCurrency : public QWidget
{
    Q_OBJECT

public:
    explicit ListCurrency(QWidget *parent = 0);
    ~ListCurrency();

private:
    Ui::ListCurrency *ui;
    QSqlQueryModel *model;
    QString query;

private slots:
    void check_new_button(QString);
    void check_symbol(QString);
    void new_currency();
    void set_default();
    void check_select();
};

#endif // LISTCURRENCY_H
