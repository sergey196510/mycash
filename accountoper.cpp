#include "accountoper.h"
#include "ui_accountoper.h"

AccountOper::AccountOper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountOper)
{
    ui->setupUi(this);
    db = new Database;

    id = 0;
//    ui->cod->setNum(0);
    ui->name->setText("");

//    ui->cod->hide();

    setLayout(ui->horizontalLayout);

    connect(ui->name, SIGNAL(returnPressed()), SLOT(select_account()));
    connect(ui->button, SIGNAL(clicked()), SLOT(select_account()));
}

AccountOper::~AccountOper()
{
    delete db;
    delete ui;
}

void AccountOper::mousePressEvent(QMouseEvent *)
{
    select_account();
}

void AccountOper::mouseDoubleClickEvent(QMouseEvent *)
{
    select_account();
}

void AccountOper::select_account()
{
    SelectAccount *acc = new SelectAccount(this);
    QPoint point = ui->name->pos();

    qDebug() << point;
    acc->move(point);
    acc->setValue(id);
    if (acc->exec() == QDialog::Accepted) {
        id = acc->value();
        Account_Data data = db->get_account_data(id);
//        ui->cod->setNum(acc->value());
        ui->name->setText(data.name);
        emit changed_value();
    }

    delete acc;
}

int AccountOper::value()
{
//    if (id != ui->cod->text().toInt())
//        qDebug() << id << ui->cod->text().toInt();
    return id;
}

void AccountOper::setValue(int idx)
{
    Account_Data data = db->get_account_data(idx);
    id = idx;
//    ui->cod->setNum(idx);
    ui->name->setText(data.name);
}
