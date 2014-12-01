#include "editoperation.h"
#include "ui_editoperation.h"
#include "global.h"

EditOperation::EditOperation(int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOperation)
{
    ui->setupUi(this);
//    int ccod;
    Account acc;
    QString from, to, to2;

    if (type == 1) {
        ui->horizontalLayout_5->setEnabled(false);
    }
    else {
        ui->dayBox->addItem("0");
        for (int i = 1; i < 32; i++)
            ui->dayBox->addItem(QString("%1").arg(i));

        QStringList months;
        months <<
                  "Any" <<
                  QDate::longMonthName(1) <<
                  QDate::longMonthName(2) <<
                  QDate::longMonthName(3) <<
                  QDate::longMonthName(4) <<
                  QDate::longMonthName(5) <<
                  QDate::longMonthName(6) <<
                  QDate::longMonthName(7) <<
                  QDate::longMonthName(8) <<
                  QDate::longMonthName(9) <<
                  QDate::longMonthName(10) <<
                  QDate::longMonthName(11) <<
                  QDate::longMonthName(12);
        ui->monBox->addItems(months);

        ui->yearBox->setRange(0, 2020);
    }

    db = new Database;

//    list = db->get_currency_list();

    acc.read(ui->fromWidget->value());
    from = Currency(acc.Curr()).SCod();
    acc.read(ui->toWidget->value());
    to = Currency(acc.Curr()).SCod();
//    to2 = db->get_account_scod(ui->to2Widget->value());

    ui->from_cod->setText(from);
    ui->to_cod->setText(to);
//    ui->to_cod_2->setText(to2);

//    ui->groupBox_3->hide();
//    layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui->okButton->setEnabled(false);

    ui->warning->hide();

//    connect(ui->from_account, SIGNAL(activated(QModelIndex)), SLOT(check_Ok()));
//    connect(ui->to_account, SIGNAL(activated(QModelIndex)), SLOT(check_Ok()));
    connect(ui->fromWidget, SIGNAL(changed_value()), SLOT(check_Ok()));
    connect(ui->toWidget, SIGNAL(changed_value()), SLOT(check_Ok()));
    connect(ui->fromSpinBox, SIGNAL(textChanged(QString)), SLOT(check_Ok()));
    connect(ui->kursEdit,    SIGNAL(textChanged(QString)), SLOT(check_Ok()));
    connect(ui->fromSpinBox, SIGNAL(textChanged(QString)), SLOT(check_balance(QString)));
    connect(ui->separateButton, SIGNAL(clicked()), SLOT(separate_account()));
    connect(ui->newAgentButton, SIGNAL(clicked()), SLOT(new_agent()));
    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
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

    if (ui->fromWidget->value() == 0)
        ui->okButton->setEnabled(false);
    else if (ui->toWidget->value() == 0)
        ui->okButton->setEnabled(false);
    else if (ui->fromSpinBox->value() == 0)
        ui->okButton->setEnabled(false);
    else if (ui->toSpinBox->value() == 0)
        ui->okButton->setEnabled(false);
    else if (ui->fromWidget->value() == ui->toWidget->value())
        ui->okButton->setEnabled(false);
    else
        ui->okButton->setEnabled(true);

    if (ui->fromWidget->value() == 0)
        ui->fromSpinBox->setEnabled(false);
    else
        ui->fromSpinBox->setEnabled(true);

    if (ui->toWidget->value() == 0)
        ui->toSpinBox->setEnabled(false);
    else
        ui->toSpinBox->setEnabled(true);

    acc.read(ui->fromWidget->value());
    from = Currency(acc.Curr()).SCod();
    acc.read(ui->toWidget->value());
    to = Currency(acc.Curr()).SCod();
//    to2 = db->get_account_scod(ui->to2Widget->value());

    ui->from_cod->setText(from);
    ui->to_cod->setText(to);
//    ui->to_cod_2->setText(to2);

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
//    QList<account_summ> lst;

    d.setDay(ui->dayBox->currentIndex());
    d.setMonth(ui->monBox->currentIndex());
    d.setYear(ui->yearBox->value());
    d.setAgent(ui->agent_comboBox->value());
    d.setKurs(ui->kursEdit->value());
    d.setAuto(ui->autoBox->isChecked());

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

    ui->dayBox->setCurrentIndex(d.Day());
    ui->monBox->setCurrentIndex(d.Month());
    ui->yearBox->setValue(d.Year());
    ui->autoBox->setChecked(d.Auto());

    if (!d.From().empty()) {
        account_summ a = d.From().at(0);
//        account_summ a = *i;
        ui->fromWidget->setValue(a.account().Id());
        ui->fromSpinBox->setValue(a.balance().toDouble());
    }

    if (!d.To().empty()) {
        account_summ a = d.To().at(0);
//        account_summ a = *i;
        ui->toWidget->setValue(a.account().Id());
        ui->toSpinBox->setValue(a.balance().toDouble());
    }

    ui->agent_comboBox->setValue(d.Agent());
    ui->kursEdit->setValue(d.Kurs());
    ui->descrEdit->setText(d.Descr());

    ui->fromSpinBox->setEnabled(ui->fromWidget->value());
    ui->toSpinBox->setEnabled(ui->toWidget->value());
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
