#ifndef CurrencyCOMBOBOX_H
#define CurrencyCOMBOBOX_H

#include <QComboBox>
#include <QtSql>

class CurrencyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CurrencyComboBox(QWidget *parent = 0);
    void load();
    int value();
    void setValue(int);

signals:
    
public slots:
    
};

#endif // CurrencyCOMBOBOX_H
