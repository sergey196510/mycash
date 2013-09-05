#ifndef MYDATEEDIT_H
#define MYDATEEDIT_H

#include <QtGui>
#include <QCalendarWidget>


class MyDateEdit : public QDateEdit
{
	Q_OBJECT

private:
	QCalendarWidget *cw;

public:
	MyDateEdit(QWidget *);
	~MyDateEdit();
	QString value() { return date().toString("yyyy-MM-dd"); }
};

#endif
