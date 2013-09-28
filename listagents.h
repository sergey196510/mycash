#ifndef LISTAGENTS_H
#define LISTAGENTS_H

#include <QWidget>

namespace Ui {
class ListAgents;
}

class ListAgents : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListAgents(QWidget *parent = 0);
    ~ListAgents();
    
private:
    Ui::ListAgents *ui;
};

#endif // LISTAGENTS_H
