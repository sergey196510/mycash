#ifndef EDITOPERATION_H
#define EDITOPERATION_H

#include <QDialog>
#include "database.h"
#include "global.h"
#include "listseparateoper.h"
#include "editagent.h"

namespace Ui {
class EditOperation;
}

class EditOperation : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditOperation(int type, QWidget *parent = 0);
    ~EditOperation();
    operation_data data();
    void setdata(operation_data &);
    
private:
    Ui::EditOperation *ui;
    Database *db;
//    QMap<QString,double> list;

private slots:
    void check_Ok();
    void check_balance(QString);
    void separate_account();
    int new_agent();
};

#endif // EDITOPERATION_H
