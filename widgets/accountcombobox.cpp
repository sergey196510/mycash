#include "accountcombobox.h"

AccountComboBox::AccountComboBox(QWidget *parent) :
    QComboBox(parent)
{
    load();
    this->setEditable(false);
}

void AccountComboBox::load(int top)
{
    QSqlQuery q;
    QSqlRecord r;
    QString name;
    int id;
    int num = 0;

    this->clear();

    addItem(tr("Account not present"), 0);

    if (top == 0) {
        q.prepare("SELECT id,name FROM account WHERE hidden = 0 ORDER BY name");
    }
    else {
        q.prepare("SELECT id,name FROM account WHERE (top = :top1 OR top = :top2) AND hidden = 0 ORDER BY name");
        q.bindValue(":top1", 1);
        q.bindValue(":top2", 2);
    }
    q.exec();
    while (q.next()) {
        r = q.record();
        id = q.value(r.indexOf("id")).toInt();
        name = q.value(r.indexOf("name")).toString();
//        curr = q.value(r.indexOf("curr")).toString();
        addItem(name, id);
        num ++;
//        if (curr == id)
//            this->setCurrentIndex(num);
    }
}

int AccountComboBox::value()
{
    return itemData(currentIndex()).toInt();
}

void AccountComboBox::setValue(int val)
{
	int i = findData(val);
	if (i != -1)
		setCurrentIndex(i);
}
