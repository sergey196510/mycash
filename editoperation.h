#ifndef EDITOPERATION_H
#define EDITOPERATION_H

#include <QDialog>
#include "database.h"
#include "global.h"

namespace Ui {
class EditOperation;
}

class EditOperation : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditOperation(QWidget *parent = 0);
    ~EditOperation();
    void data(operation_data &);
    void setdata(operation_data &);
    
private:
    Ui::EditOperation *ui;
    Database *db;

private slots:
    void check_Ok();
    void check_balance(QString);
};

#endif // EDITOPERATION_H
