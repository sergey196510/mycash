#ifndef LISTPLANOPER_H
#define LISTPLANOPER_H

#include <QtGui>
#include <QWidget>
#include <QtSql>

namespace Ui {
class ListPlanOper;
}

class ListPlanOperModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ListPlanOperModel(QObject *parent = 0);
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;

private:
    QStringList header_data;

};

class ListPlanOper : public QWidget
{
    Q_OBJECT

public:
    explicit ListPlanOper(QWidget *parent = 0);
    ~ListPlanOper();

private:
    Ui::ListPlanOper *ui;
    ListPlanOperModel *model;
    QString query;

private slots:
    void new_oper();
};

#endif // LISTPLANOPER_H
