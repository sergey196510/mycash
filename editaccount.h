#ifndef EDITACCOUNT_H
#define EDITACCOUNT_H

#include <QDialog>

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
    double balance();
    
private:
    Ui::EditAccount *ui;
};

#endif // EDITACCOUNT_H
