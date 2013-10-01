#ifndef LISTAGENTS_H
#define LISTAGENTS_H

#include <QWidget>
#include <QtSql>

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
    QString query;
    QSqlQueryModel *model;

private slots:
    void check_new_button(QString);
    void check_select_line();
    void save_new_record();
};

#endif // LISTAGENTS_H
