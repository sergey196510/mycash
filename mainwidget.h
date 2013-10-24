#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include <QWidget>
#include "global.h"
#include "database.h"

namespace Ui {
class MainWidget;
}

class MainWidgetModel : public QStandardItemModel
{
    Q_OBJECT

public:
    MainWidgetModel(QObject *parent = 0);

private:
    Database *db;
    QList<PlanOper_data> list;
};

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    Database *db;
    MainWidgetModel *model;
};

#endif // MAINWIDGET_H
