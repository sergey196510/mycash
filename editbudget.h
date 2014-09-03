#ifndef EDITBUDGET_H
#define EDITBUDGET_H

#include <QDialog>
#include "global.h"

namespace Ui {
class EditBudget;
}

class EditBudget : public QDialog
{
    Q_OBJECT

public:
    explicit EditBudget(QWidget *parent = 0);
    ~EditBudget();
    Budget data();

private:
    Ui::EditBudget *ui;
};

#endif // EDITBUDGET_H
