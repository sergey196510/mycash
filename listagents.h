#ifndef LISTAGENTS_H
#define LISTAGENTS_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include "database.h"
#include "global.h"

namespace Ui {
class ListAgents;
}

class ListAgentsModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QStringList header_data;
    QList<agent_data> list;
    QList<agent_data> read_list();

public:
    explicit ListAgentsModel(QObject *parent = 0);
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
};

class ListAgents : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListAgents(QWidget *parent = 0);
    ~ListAgents();

public slots:
    void reload_model();
    void clear_model();

private:
    Ui::ListAgents *ui;
    QString query;
    QStringList header_data;
    ListAgentsModel *model;
    Database *db;
    Globals var;

private slots:
    void check_new_button(QString);
    void check_select_line();
    void save_new_record();
    void update_record();
    void del_record();
    void clear_record();
};

#endif // LISTAGENTS_H
