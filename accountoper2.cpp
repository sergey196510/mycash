#include "accountoper2.h"

AccountOper2::AccountOper2(QWidget *parent) :
    QLineEdit(parent)
{
    val = 0;
//    db = new Database;
//    list = db->get_accounts_list();

    this->setReadOnly(true);

//    connect(this, SIGNAL(textChanged()), SLOT(select_account()));
}

AccountOper2::~AccountOper2()
{
//    delete db;
}

void AccountOper2::select_account()
{
    SelectAccount *acc = new SelectAccount(this);
    Account item;
//    QCursor m;

//    qDebug() << m.pos();

    acc->setValue(val);
    acc->move(QCursor().pos());
    if (acc->exec() == QDialog::Accepted) {
        val = acc->value();
        item.read(val);
        this->setText(item.fullName());
        emit changed_value();
    }
}

void AccountOper2::setValue(int i)
{
    Account acc;

    acc.read(i);
    val = i;
    this->setText(acc.fullName());
}

void AccountOper2::mousePressEvent(QMouseEvent *)
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
