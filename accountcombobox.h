#ifndef PAYMENTCOMBOBOX_H
#define PAYMENTCOMBOBOX_H

#include <QtGui>
#include <QComboBox>
#include <QtSql>

class AccountComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AccountComboBox(QWidget *parent = 0);
    void load();
    void load2();
    int value();
    void setValue(int i);

signals:
    
public slots:
    
};

#endif // PAYMENTCOMBOBOX_H
