#ifndef EDITACCOUNT_H
#define EDITACCOUNT_H

#include <QDialog>

struct Account_Data {
    QString name;
    int type;
    int curr;
    double balance;
    QString descr;
};

namespace Ui {
class EditAccount;
}

class EditAccount : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditAccount(QWidget *parent = 0);
    ~EditAccount();
    QString name();
    int type();
    int curr();
    double balance();
    QString descr();
    Account_Data data();
    
private:
    Ui::EditAccount *ui;
    bool nameFind(QString);

private slots:
    void nameCheck(QString);
};

#endif // EDITACCOUNT_H
