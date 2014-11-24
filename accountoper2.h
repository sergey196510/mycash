#ifndef ACCOUNTOPER2_H
#define ACCOUNTOPER2_H

#include <QTextEdit>
#include <QLineEdit>
#include "database.h"
#include "selectaccount.h"

class AccountOper2 : public QLineEdit
{
    Q_OBJECT
public:
    explicit AccountOper2(QWidget *parent = 0);
    ~AccountOper2();
    void setValue(int);
    int value() { return val; }

protected:
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

signals:
    void changed_value();

public slots:

private:
    int val;
    Database *db;
    QMap<int,QString> list;

private slots:
    void select_account();
};

#endif // ACCOUNTOPER2_H
