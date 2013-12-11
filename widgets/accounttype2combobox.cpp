#include "accounttype2combobox.h"

AccountType2ComboBox::AccountType2ComboBox(QWidget *parent) :
    QComboBox(parent)
{
    load();
    this->setEditable(false);
}

void AccountType2ComboBox::load()
{
    QSqlQuery q;
    QSqlRecord r;
    QString name;
    int id;

    this->clear();

    q.exec("SELECT id,name FROM account_type2 ORDER BY name");
    while (q.next()) {
        r = q.record();
        name = q.value(r.indexOf("name")).toString();
        id = q.value(r.indexOf("id")).toInt();
        addItem(name, id);
    }
}

int AccountType2ComboBox::value()
{
    return itemData(currentIndex()).toInt();
}

void AccountType2ComboBox::setValue(int val)
{
	int i = findData(val);
	if (i != -1)
		setCurrentIndex(i);
}
