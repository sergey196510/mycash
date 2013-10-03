#ifndef LISTCURRENCY_H
#define LISTCURRENCY_H

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
};

#endif // LISTCURRENCY_H
