#ifndef LISTACCOUNTS_H
#define LISTACCOUNTS_H

#include <QWidget>

namespace Ui {
class ListAccounts;
}

class ListAccounts : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListAccounts(QWidget *parent = 0);
    ~ListAccounts();
    
private:
    Ui::ListAccounts *ui;
};

#endif // LISTACCOUNTS_H
