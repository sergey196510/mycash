#ifndef EDITPLANOPERATION_H
#define EDITPLANOPERATION_H

#include <QDialog>
#include "operation.h"

namespace Ui {
class EditPlanOperation;
}

class EditPlanOperation : public QDialog
{
    Q_OBJECT

public:
    explicit EditPlanOperation(QWidget *parent = 0);
    ~EditPlanOperation();
    PlanOperation Data();
    void setData(PlanOperation &);

private:
    Ui::EditPlanOperation *ui;

private slots:
    void changeType();
    void changeDay();
};

#endif // EDITPLANOPERATION_H
