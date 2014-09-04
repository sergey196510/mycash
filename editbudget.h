#ifndef EDITBUDGET_H
#define EDITBUDGET_H

#include <QDialog>
#include "global.h"
//#include "budget.h"

namespace Ui {
class EditBudget;
}

class EditBudget : public QDialog
{
    Q_OBJECT

public:
    explicit EditBudget(QWidget *parent = 0);
    ~EditBudget();
//    Budget data();
    int Month();
    int Account();
    MyCurrency Summ();
    void setMonth(int m);
    void setAccount(int a);
    void setSumm(MyCurrency s);

private:
    Ui::EditBudget *ui;
};

#endif // EDITBUDGET_H
