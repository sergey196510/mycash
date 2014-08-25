#ifndef MYDATEEDIT_H
#define MYDATEEDIT_H

#include <QDateEdit>
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
    MyDateEdit(QWidget *d = 0);
	~MyDateEdit();
    QDate value() { return date(); }
    QString toString() { return date().toString("yyyy-MM-dd"); }
};

#endif
