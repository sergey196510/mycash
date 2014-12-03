#ifndef REGOPERATION_H
#define REGOPERATION_H

#include <QDialog>
#include <operation.h>

namespace Ui {
class RegOperation;
}

class RegOperation : public QDialog
{
    Q_OBJECT

public:
    explicit RegOperation(QWidget *parent = 0);
    ~RegOperation();
    Operation data();
    void setData(Operation &);

private:
    Ui::RegOperation *ui;
};

#endif // REGOPERATION_H
