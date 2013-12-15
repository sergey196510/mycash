#ifndef ACCOUNTOPER_H
#define ACCOUNTOPER_H

#include <QWidget>
#include "selectaccount.h"
#include "database.h"

namespace Ui {
class AccountOper;
}

class AccountOper : public QWidget
{
    Q_OBJECT

public:
    explicit AccountOper(QWidget *parent = 0);
    ~AccountOper();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

public slots:
    int value();
    void setValue(int);

private:
    Ui::AccountOper *ui;
    Database *db;
    int id;

private slots:
    void select_account();

signals:
    void changed_value();
};

#endif // ACCOUNTOPER_H
