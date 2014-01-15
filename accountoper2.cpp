#include "accountoper2.h"

AccountOper2::AccountOper2(QWidget *parent) :
    QLineEdit(parent)
{
    val = 0;
    db = new Database;
    list = db->get_accounts_list();

    this->setReadOnly(true);

//    connect(this, SIGNAL(textChanged()), SLOT(select_account()));
}

void AccountOper2::select_account()
{
    SelectAccount *acc = new SelectAccount(this);

    acc->setValue(val);
    if (acc->exec() == QDialog::Accepted) {
        val = acc->value();
        this->setText(list[val]);
        emit changed_value();
    }
}

void AccountOper2::setValue(int i)
{
    val = i;
    this->setText(list[val]);
}

void AccountOper2::mousePressEvent(QMouseEvent *event)
{
    select_account();
}

void AccountOper2::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return)
        select_account();
    else
        QWidget::keyPressEvent(event);
}
