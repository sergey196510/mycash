#include "accountcombobox.h"

AccountComboBox::AccountComboBox(QWidget *parent) :
    QComboBox(parent)
{
    load();
    this->setEditable(false);
}

void AccountComboBox::load()
{
    QSqlQuery q;
    QSqlRecord r;
    QString name;
    int id;
    int num = 0;

    this->clear();

    addItem(tr("Account not present"), 0);

    q.exec("SELECT id,name FROM account ORDER BY name");
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

void AccountComboBox::load2()
{
//	QTreeModel *mdl;

//	mdl->setQuery("SELECT name FROM account ORDER BY name");

//	setModel(mdl);
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
