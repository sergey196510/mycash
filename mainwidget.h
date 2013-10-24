#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "global.h"
#include "database.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    Database *db;
};

#endif // MAINWIDGET_H
