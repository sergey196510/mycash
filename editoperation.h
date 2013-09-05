#ifndef EDITOPERATION_H
#define EDITOPERATION_H

#include <QDialog>

namespace Ui {
class EditOperation;
}

class EditOperation : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditOperation(QWidget *parent = 0);
    ~EditOperation();
    
private:
    Ui::EditOperation *ui;
};

#endif // EDITOPERATION_H
