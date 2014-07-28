#include "editoperation.h"
#include "ui_editoperation.h"
#include "global.h"

EditOperation::EditOperation(int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOperation)
{
    ui->setupUi(this);
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

    from = db->get_account_scod(ui->fromWidget->value());
    to = db->get_account_scod(ui->toWidget->value());
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

    from = db->get_account_scod(ui->fromWidget->value());
    to = db->get_account_scod(ui->toWidget->value());
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
    Account_Data data = db->get_account_data(ui->fromWidget->value());

    if (data.type != 1)
        return;

    if (value.toDouble() > data.balance.value())
        ui->warning->show();
    else
        ui->warning->hide();
}

operation_data EditOperation::data()
{
    operation_data d;
    account_summ a;

    d.day = ui->dayBox->currentIndex();
    d.month = ui->monBox->currentIndex();
    d.year = ui->yearBox->value();
    d.agent = ui->agent_comboBox->value();
    d.kurs = ui->kursEdit->value();

    a.set_account(ui->fromWidget->value());
    a.set_balance(ui->fromSpinBox->value());
    d.from.append(a);

    a.set_account(ui->toWidget->value());
    a.set_balance(ui->toSpinBox->value());
    d.to.append(a);

    d.date = ui->dateEdit->value();
    d.descr = ui->descrEdit->text();

    return d;
}

void EditOperation::setdata(operation_data &d)
{
    Account_Data data;
    QList<account_summ>::iterator i;

    ui->dayBox->setCurrentIndex(d.day);
    ui->monBox->setCurrentIndex(d.month);
    ui->yearBox->setValue(d.year);

    if (!d.from.empty()) {
        i = d.from.begin();
        account_summ a = *i;
        ui->fromWidget->setValue(a.account());
        ui->fromSpinBox->setValue(a.balance().value());
    }

    if (!d.to.empty()) {
        i = d.to.begin();
        account_summ a = *i;
        ui->toWidget->setValue(a.account());
        ui->toSpinBox->setValue(a.balance().value());
    }

    ui->agent_comboBox->setValue(d.agent);
    ui->kursEdit->setValue(d.kurs);
    ui->descrEdit->setText(d.descr);

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
    agent_data data;

    if (agent->exec() == QDialog::Accepted) {
        data = agent->data();
        int id = db->new_agent(data);
        ui->agent_comboBox->load();
        ui->agent_comboBox->setValue(id);
    }

    return 0;
}
