#ifndef EDITOPERATION_H
#define EDITOPERATION_H

#include <QDialog>

struct operation_data {
    int from;
    int to;
    double summ;
    QString date;
};

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
    
private:
    Ui::EditOperation *ui;

private slots:
    void check_Ok();
};

#endif // EDITOPERATION_H
