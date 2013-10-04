#include "mydateedit.h"
#include "global.h"

MyCalendarWidget::MyCalendarWidget(QWidget *d) :
    QCalendarWidget(d)
{
    default_locale->weekdays();
    default_locale->firstDayOfWeek();
//    this->setFirstDayOfWeek(Qt::Monday);
}

MyDateEdit::MyDateEdit(QWidget *d = 0) :
	QDateEdit(d)
{
    cw = new MyCalendarWidget;

//	cw->setFirstDayOfWeek(Qt::Monday);
	setCalendarPopup(true);
        setCalendarWidget(cw);

    setDate(QDate().currentDate());
}

MyDateEdit::~MyDateEdit()
{
	delete cw;
}
