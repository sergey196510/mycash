#ifndef LISTCURRENCY_H
#define LISTCURRENCY_H

#include <QtGui>
#include <QWidget>

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

private slots:
    void check_new_button(QString);
    void check_symbol(QString);
};

#endif // LISTCURRENCY_H
