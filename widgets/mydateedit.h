#ifndef MYDATEEDIT_H
#define MYDATEEDIT_H

#include <QtGui>
#include <QCalendarWidget>


class MyCalendarWidget : public QCalendarWidget
{
    Q_OBJECT

public:
    MyCalendarWidget(QWidget *parent = 0);
};

class MyDateEdit : public QDateEdit
{
	Q_OBJECT

private:
    MyCalendarWidget *cw;

public:
    MyDateEdit(QWidget *);
	~MyDateEdit();
    QDate value() { return date(); }
    QString toString() { return date().toString("yyyy-MM-dd"); }
};

#endif
