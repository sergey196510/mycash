#ifndef AccountType2COMBOBOX_H
#define AccountType2COMBOBOX_H

#include <QComboBox>
#include <QtSql>

class AccountType2ComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AccountType2ComboBox(QWidget *parent = 0);
    void load();
    int value();
    void setValue(int);

signals:
    
public slots:
    
};

#endif // AccountType2COMBOBOX_H
