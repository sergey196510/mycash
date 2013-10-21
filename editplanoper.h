#ifndef EDITPLANOPER_H
#define EDITPLANOPER_H

#include <QDialog>

namespace Ui {
class editPlanOper;
}

class editPlanOper : public QDialog
{
    Q_OBJECT

public:
    explicit editPlanOper(QWidget *parent = 0);
    ~editPlanOper();

private:
    Ui::editPlanOper *ui;
};

#endif // EDITPLANOPER_H
