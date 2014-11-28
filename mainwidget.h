#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include <QWidget>
#include "global.h"
#include "database.h"
#include "summaccount.h"
#include "currency.h"
#include "operation.h"

namespace Ui {
class MainWidget;
}

class MainWidgetModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MainWidgetModel(Database *db, QObject *parent = 0);
    ~MainWidgetModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    enum Column {
        id = 0,
        day = 1,
        month = 2,
        year = 3,
        column_from = 4,
        column_to = 5,
        summ = 6,
        status = 7,
        descr = 8
    };

public slots:
//    void fill_model(Database *db);

private:
    Database *db;
    QList<Operation> list;
    QMap<int,QString> acc_list;
    QStringList header_data;
    Globals *var;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

private slots:
//    bool get_operations(int);
};

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(Database *d, QWidget *parent = 0);
    ~MainWidget();

public slots:
    void reload_model();
    void clear_model();

private:
    Globals var;
    Ui::MainWidget *ui;
    Database *db;
    MainWidgetModel *model;
};

#endif // MAINWIDGET_H
