#include "currencycombobox.h"

CurrencyComboBox::CurrencyComboBox(QWidget *parent) :
    QComboBox(parent)
{
    load();
    this->setEditable(true);
}

void CurrencyComboBox::load()
{
    QSqlQuery q;
    QSqlRecord r;
    QString name;
    int id;

    this->clear();

    q.exec("SELECT id,name FROM currency ORDER BY name");
    while (q.next()) {
        r = q.record();
        name = q.value(r.indexOf("name")).toString();
        id = q.value(r.indexOf("id")).toInt();
        addItem(name, id);
    }
}

int CurrencyComboBox::value()
{
    return itemData(currentIndex()).toInt();
}

void CurrencyComboBox::setValue(int val)
{
	int i = findData(val);
	if (i != -1)
		setCurrentIndex(i);
}
