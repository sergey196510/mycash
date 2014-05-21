#ifndef CORRECTBALANCE_H
#define CORRECTBALANCE_H

#include <QDialog>
#include "global.h"

namespace Ui {
class CorrectBalance;
}

class CorrectBalance : public QDialog
{
    Q_OBJECT
    
public:
    explicit CorrectBalance(QWidget *parent = 0);
    ~CorrectBalance();
    void setBalance(double);
    void setAccount(int);
    double balance();
    int account();
    QDate date();

private:
    Globals var;
    Ui::CorrectBalance *ui;

private slots:
    void check_ok();
};

#endif // CORRECTBALANCE_H
