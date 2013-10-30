#ifndef EDITPLANOPER_H
#define EDITPLANOPER_H

#include <QDialog>
#include "global.h"

namespace Ui {
class editPlanOper;
}

class editPlanOper : public QDialog
{
    Q_OBJECT

public:
    explicit editPlanOper(QWidget *parent = 0);
    ~editPlanOper();
    PlanOper_data Value();
    void setValue(PlanOper_data &);

private:
    Ui::editPlanOper *ui;

private slots:
    void check_ok();
};

#endif // EDITPLANOPER_H
