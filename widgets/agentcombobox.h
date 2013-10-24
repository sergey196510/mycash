#ifndef AGENTCOMBOBOX_H
#define AGENTCOMBOBOX_H

#include <QtGui>
#include <QComboBox>
#include <QtSql>

class AgentComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AgentComboBox(QWidget *parent = 0);
    void load(int type = 0);
    int value();
    void setValue(int i);

signals:
    
public slots:
    
};

#endif // AGENTCOMBOBOX_H
