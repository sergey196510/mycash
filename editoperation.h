#ifndef EDITOPERATION_H
#define EDITOPERATION_H

#include <QDialog>
#include <QTreeView>
#include "database.h"
#include "global.h"
#include "listseparateoper.h"
#include "editagent.h"
#include "operation.h"
#include "listaccountsmodel.h"

namespace Ui {
class EditOperation;
}

class EditOperation : public QWidget
{
    Q_OBJECT
    
public:
    explicit EditOperation(QWidget *parent = nullptr);
    ~EditOperation();
    Operation data();
    void setdata(Operation &);
    
private:
    Ui::EditOperation *ui;
    Database *db;
    ListAccountsModel *model;
//    QMap<QString,double> list;

private slots:
    void check_Ok();
    void check_balance(QString);
    void separate_account();
    int new_agent();
};

#endif // EDITOPERATION_H
