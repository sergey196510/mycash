#ifndef AccountTypeCOMBOBOX_H
#define AccountTypeCOMBOBOX_H

#include <QComboBox>
#include <QtSql>

class AccountTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AccountTypeComboBox(QWidget *parent = 0);
    void load();
    int value();
    void setValue(int);

signals:
    
public slots:
    
};

#endif // AccountTypeCOMBOBOX_H
