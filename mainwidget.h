#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include <QWidget>
#include "global.h"
#include "database.h"
#include "summaccount.h"

namespace Ui {
class MainWidget;
}

class MainWidgetModel : public QStandardItemModel
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
    void fill_model(Database *db);

private:
//    Database *db;
    QList<operation_data> list;
    QMap<int,QString> accounts;
    QStringList header_data;
    Globals var;

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
