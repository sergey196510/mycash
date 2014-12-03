#include "editoperation.h"
#include "ui_editoperation.h"
#include "global.h"

EditOperation::EditOperation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditOperation)
{
    ui->setupUi(this);
    Account acc;
    QString from, to, to2;

    db = new Database;

//    acc.read(ui->fromWidget->value());
//    from = Currency(acc.Curr()).SCod();
//    acc.read(ui->toWidget->value());
//    to = Currency(acc.Curr()).SCod();

    ui->fromSpinBox->setEnabled(false);
    ui->toSpinBox->setEnabled(false);

    ui->from_cod->setText(QString());
    ui->to_cod->setText(QString());

    ui->warning->hide();

    connect(ui->fromWidget,     SIGNAL(changed_value()), SLOT(check_Ok()));
    connect(ui->toWidget,       SIGNAL(changed_value()), SLOT(check_Ok()));
    connect(ui->fromSpinBox,    SIGNAL(textChanged(QString)), SLOT(check_Ok()));
    connect(ui->fromSpinBox,    SIGNAL(textChanged(QString)), SLOT(check_balance(QString)));
    connect(ui->kursEdit,       SIGNAL(textChanged(QString)), SLOT(check_Ok()));
    connect(ui->separateButton, SIGNAL(clicked()), SLOT(separate_account()));
    connect(ui->newAgentButton, SIGNAL(clicked()), SLOT(new_agent()));
}

EditOperation::~EditOperation()
{
    delete ui;
    delete db;
}

void EditOperation::check_Ok()
{
    Account acc;
    QString from, to, to2;
    double kurs;

    ui->fromSpinBox->setEnabled(ui->fromWidget->value());
    ui->toSpinBox->setEnabled(ui->toWidget->value());

    if (acc.read(ui->fromWidget->value())) {
        from = Currency(acc.Curr()).SCod();
        ui->from_cod->setText(from);
    }
    else
        ui->from_cod->setText(QString());

    if (acc.read(ui->toWidget->value())) {
        to = Currency(acc.Curr()).SCod();
        ui->to_cod->setText(to);
    }
    else
        ui->to_cod->setText(QString());

    if (db->currency_list[from] > 0 && db->currency_list[to] > 0)
        kurs = db->currency_list[from]/db->currency_list[to];
    else
        kurs = 0;

    ui->kursEdit->setValue(kurs);
    ui->toSpinBox->setValue(ui->fromSpinBox->value()*kurs);
}

void EditOperation::check_balance(QString value)
{
//    double balance;
    Account data;

    data.read(ui->fromWidget->value());
    if (data.Type() != 1)
        return;

    if (value.toDouble() > data.Balance().toDouble())
        ui->warning->show();
    else
        ui->warning->hide();
}

Operation EditOperation::data()
{
    Account acc;
    Operation d;
    account_summ a;

    d.setAgent(ui->agent_comboBox->value());
    d.setKurs(ui->kursEdit->value());

    acc.read(ui->fromWidget->value());
    a.set_account(acc);
    a.set_balance(ui->fromSpinBox->value());
    d.append_from(a);

    acc.read(ui->toWidget->value());
    a.set_account(acc);
    a.set_balance(ui->toSpinBox->value());
    d.append_to(a);

    d.setDate(ui->dateEdit->value());
    d.setDescr(ui->descrEdit->text());

    return d;
}

void EditOperation::setdata(Operation &d)
{
    Account data;
    QList<account_summ>::iterator i;

    if (!d.From().empty()) {
        account_summ a = d.From().at(0);
        qDebug() << a.account().Id();
        ui->fromWidget->setValue(a.account().Id());
        ui->from_cod->setText(Currency(a.account().Curr()).SCod());
        ui->fromSpinBox->setValue(a.balance().toDouble());
    }
    ui->fromSpinBox->setEnabled(!d.From().empty());

    if (!d.To().empty()) {
        account_summ a = d.To().at(0);
        qDebug() << a.account().Id();
        ui->toWidget->setValue(a.account().Id());
        ui->to_cod->setText(Currency(a.account().Curr()).SCod());
        ui->toSpinBox->setValue(a.balance().toDouble());
    }
    ui->toSpinBox->setEnabled(!d.To().empty());

    ui->agent_comboBox->setValue(d.Agent());
    ui->kursEdit->setValue(d.Kurs());
    ui->descrEdit->setText(d.Descr());

    check_Ok();
}

void EditOperation::separate_account()
{
    ListSeparateOper *lst = new ListSeparateOper(this);
    QList<account_summ> acc_oper;

    lst->move(QCursor().pos());
    if (lst->exec() == QDialog::Accepted) {
        acc_oper = lst->data();
        qDebug() << "accept";
    }
    else
        qDebug() << "Reject";
}

int EditOperation::new_agent()
{
    EditAgent *agent = new EditAgent(tr("Agent"), this);
    Agent data;

    if (agent->exec() == QDialog::Accepted) {
        data = agent->data();
        int id = data.insert();
        ui->agent_comboBox->load();
        ui->agent_comboBox->setValue(id);
    }

    return 0;
}
