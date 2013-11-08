#ifndef EDITACCOUNT_H
#define EDITACCOUNT_H

#include <QDialog>
#include "global.h"
#include "database.h"

namespace Ui {
class EditAccount;
}

class EditAccount : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditAccount(int type, QWidget *parent = 0);
    ~EditAccount();
//    QString name();
//    int type();
//    int curr();
//    void set_curr(int);
//    void set_parent(int);
//    double balance();
//    bool hidden();
//    QString descr();
    Account_Data data();
    void setData(Account_Data &data);
    
private:
    Ui::EditAccount *ui;
    Database db;
    bool nameFind(QString);

private slots:
    void nameCheck(QString);
    void parentCheck();
};

#endif // EDITACCOUNT_H
