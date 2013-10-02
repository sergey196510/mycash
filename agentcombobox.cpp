#include "agentcombobox.h"

AgentComboBox::AgentComboBox(QWidget *parent) :
    QComboBox(parent)
{
    load();
    this->setEditable(false);
}

void AgentComboBox::load(int type)
{
    QSqlQuery q;
    QSqlRecord r;
    QString name;
    int id;
    int num = 0;

    this->clear();

    addItem(tr("Agent not present"), 0);

    if (type == 0) {
	q.prepare("SELECT id,name FROM agent ORDER BY name");
    }
    else {
	q.prepare("SELECT id,name FROM agent WHERE type = :type ORDER BY name");
	q.bindValue(":type", type);
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

int AgentComboBox::value()
{
    return itemData(currentIndex()).toInt();
}

void AgentComboBox::setValue(int val)
{
	int i = findData(val);
	if (i != -1)
		setCurrentIndex(i);
}
