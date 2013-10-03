#include "mydateedit.h"
#include "global.h"

MyDateEdit::MyDateEdit(QWidget *d = 0) :
	QDateEdit(d)
{
	cw = new QCalendarWidget;

	cw->setFirstDayOfWeek(Qt::Monday);
	setCalendarPopup(true);
        setCalendarWidget(cw);

    setDate(QDate().currentDate());
}

MyDateEdit::~MyDateEdit()
{
	delete cw;
}
