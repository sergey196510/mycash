#ifndef SELECTACCOUNT_H
#define SELECTACCOUNT_H

#include <QDialog>
#include "listaccountsmodel.h"

namespace Ui {
class SelectAccount;
}

class SelectAccount : public QDialog
{
    Q_OBJECT

public:
    explicit SelectAccount(QWidget *parent = 0);
    ~SelectAccount();

private:
    Ui::SelectAccount *ui;

public slots:
    void setValue(int);
    int value();
};

#endif // SELECTACCOUNT_H
