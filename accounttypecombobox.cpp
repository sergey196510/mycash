#include "accounttypecombobox.h"

AccountTypeComboBox::AccountTypeComboBox(QWidget *parent) :
    QComboBox(parent)
{
    load();
    this->setEditable(true);
}

void AccountTypeComboBox::load()
{
    QSqlQuery q;
    QSqlRecord r;
    QString name;
    int id;

    this->clear();

    q.exec("SELECT id,name FROM account_type ORDER BY name");
    while (q.next()) {
        r = q.record();
        name = q.value(r.indexOf("name")).toString();
        id = q.value(r.indexOf("id")).toInt();
        addItem(name, id);
    }
}

int AccountTypeComboBox::value()
{
    return itemData(currentIndex()).toInt();
}

void AccountTypeComboBox::setValue(int val)
{
	int i = findData(val);
	if (i != -1)
		setCurrentIndex(i);
}
