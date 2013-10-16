#ifndef LISTAGENTS_H
#define LISTAGENTS_H

#include <QtGui>
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
    int get_selected_id();

private slots:
    void check_new_button(QString);
    void check_select_line();
    void save_new_record();
    void update_record();
    void del_record();
};

#endif // LISTAGENTS_H
