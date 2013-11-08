#include <QtGui>
#include <QtSql>
#include "mycash.h"
#include "ui_mycash.h"
#include "createdatabase.h"
#include "opendatabase.h"
#include "listaccounts.h"
#include "listcurrency.h"
#include "listoperations.h"
#include "listagents.h"
#include "listplanoper.h"
#include "mainwidget.h"
#include "graphwidget.h"
#include "global.h"
#include "settings.h"

MyCash::MyCash(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyCash)
{
    ui->setupUi(this);

    opened = false;

    readsettings();

    if (!dbname.isEmpty())
        opendb(dbname);

    db = new Database;

    curr = new CurrencyComboBox;
    curr->setValue(var.Currency());
    curr->setToolTip(tr("Select current currency"));

    var.setSymbol(db->get_currency_scod(var.Currency()));

    setconnects();

//    QDockWidget *pdock = new QDockWidget("MyDock", this);
//    QLabel * plbl = new QLabel("Label in Dock", pdock);
//    pdock->addAction(ui->action_ListAccounts);
//    pdock->setWidget(plbl);
//    addDockWidget(Qt::LeftDockWidgetArea, pdock);

    list_home();

    emit call_mark();
}

MyCash::~MyCash()
{
    writesettings();
    delete curr;
    delete ui;
}

void MyCash::setconnects()
{
    ui->action_Quit->setIcon(QPixmap(":icons/exit.png"));
    ui->action_Settings->setIcon(QPixmap(":icons/options.gif"));
    ui->action_Home->setIcon(QPixmap(":icons/home.png"));
    ui->action_ListAccounts->setIcon(QPixmap(":icons/money.ico"));
    ui->action_ListAgents->setIcon(QPixmap(":icons/user.png"));
    ui->action_ListOperations->setIcon(QPixmap(":icons/payment.ico"));

    connect(this,                    SIGNAL(call_mark()), SLOT(mark_Object()));
    connect(ui->action_Create,       SIGNAL(triggered()), SLOT(create()));
    connect(ui->action_Open,         SIGNAL(triggered()), SLOT(open()));
    connect(ui->action_Close,        SIGNAL(triggered()), SLOT(closeDatabase()));
    connect(ui->action_Settings,     SIGNAL(triggered()), SLOT(settings()));
    connect(ui->action_Quit,         SIGNAL(triggered()), SLOT(close()));

    connect(ui->action_Home, SIGNAL(triggered()), SLOT(list_home()));
    connect(ui->action_ListAccounts, SIGNAL(triggered()), SLOT(list_accounts()));
    connect(ui->action_ListOperations, SIGNAL(triggered()), SLOT(list_operations()));
    connect(ui->action_ListAgents, SIGNAL(triggered()), SLOT(list_agents()));
    connect(ui->action_ListCurrencies, SIGNAL(triggered()), SLOT(list_currency()));
    connect(ui->action_Plan_Operations, SIGNAL(triggered()), SLOT(list_plan_oper()));

    connect(ui->actionReport_1, SIGNAL(triggered()), SLOT(report1()));
    connect(ui->actionReport_2, SIGNAL(triggered()), SLOT(report2()));
    connect(ui->actionTest_2, SIGNAL(triggered()), SLOT(test()));

    connect(ui->actionAbout_program, SIGNAL(triggered()), SLOT(aboutProgram()));
    connect(ui->actionAbout_QT,      SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(curr, SIGNAL(currentIndexChanged(int)), SLOT(update_curr()));

    ui->mainToolBar->addAction(ui->action_Create);
    ui->mainToolBar->addAction(ui->action_Open);
    ui->mainToolBar->addAction(ui->action_Close);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->action_Home);
    ui->mainToolBar->addAction(ui->action_ListAccounts);
    ui->mainToolBar->addAction(ui->action_ListOperations);
    ui->mainToolBar->addAction(ui->action_ListAgents);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->action_Settings);
    ui->mainToolBar->addAction(ui->action_Quit);

    ui->mainToolBar->addWidget(curr);

    setWindowTitle("MyCash version: " + version);
}

void MyCash::mark_Object()
{
    ui->action_Create->setEnabled(!opened);
    ui->action_Open->setEnabled(!opened);
    ui->action_Close->setEnabled(opened);
    ui->menu_Accounts->setEnabled(opened);
    ui->action_ListAccounts->setEnabled(opened);
    ui->action_ListOperations->setEnabled(opened);
}

void MyCash::readsettings()
{
    QSettings settings("MyCash", "MyCash");
    restoreGeometry(settings.value("geometry").toByteArray());
    dbname = settings.value("dbname", "").toString();
    var.setAccount(settings.value("current_account", "").toInt());
    var.setCurrency(settings.value("current_currency", "").toInt());
    var.setCorrectAccount(settings.value("correct_account", "").toInt());
    var.setListFont(settings.value("list_font", "").toString());
//    fnt = settings.value("operations_font");
}

void MyCash::writesettings()
{
    QSettings settings("MyCash", "MyCash");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("dbname", dbname);
    settings.setValue("current_account", var.Account());
    settings.setValue("current_currency", var.Currency());
    settings.setValue("correct_account", var.CorrectAccount());
    settings.setValue("list_font", var.ListFont());
//    settings.setValue("operations_font", fnt);
}

void MyCash::create()
{
    CreateDatabase *dbf = new CreateDatabase(this);
    QString str;

    if (dbf->exec() != QDialog::Accepted) {
        delete dbf;
        return;
    }

    opendb(dbf->name());

    QSqlQuery q;

    QFile file("mycash.sql");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    while (!in.atEnd()) {
        str = in.readLine();
//        qDebug() << str;
        if (!q.exec(str)) {
            qDebug() << q.lastError().text();
            delete dbf;
            return;
        }
    }

    delete dbf;
}

void MyCash::open()
{
    OpenDatabase *db = new OpenDatabase(this);

    db->set_filename(dbname);
    if (db->exec() == QDialog::Rejected) {
        delete db;
        return;
    }

    dbname = db->filename();
    if (dbname.isEmpty()) {
        delete db;
        return;
    }

    opendb(dbname);

    delete db;
}

void MyCash::closeDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.close();
    opened = false;
    emit call_mark();
}

void MyCash::opendb(QString dbname)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(dbname);
    if (!db.open()) {
        opened = false;
//        db->lastError().setDatabaseText()
    }
    else
	opened = true;

    emit call_mark();
}

void MyCash::list_home()
{
    MainWidget *mw = new MainWidget;

    connect(this, SIGNAL(update_currency()), mw, SLOT(update_summ()));
    curr->setEnabled(true);

    setCentralWidget(mw);
}

void MyCash::list_accounts()
{
    ListAccounts *la = new ListAccounts;

    connect(ui->action_Open, SIGNAL(triggered()), la, SLOT(reload_model()));
    connect(ui->action_Close, SIGNAL(triggered()), la, SLOT(clear_list()));
    curr->setEnabled(false);

    setCentralWidget(la);
}

void MyCash::list_currency()
{
    ListCurrency *lc = new ListCurrency;

    curr->setEnabled(false);

    setCentralWidget(lc);
}

void MyCash::list_operations()
{
    ListOperations *lo = new ListOperations;

    connect(this, SIGNAL(update_currency()), lo, SLOT(reload_model()));
    curr->setEnabled(true);

    setCentralWidget(lo);
}

void MyCash::list_agents()
{
    ListAgents *la = new ListAgents;

    curr->setEnabled(false);

    setCentralWidget(la);
}

void MyCash::list_plan_oper()
{
    ListPlanOper *po = new ListPlanOper;

    connect(this, SIGNAL(update_currency()), po, SLOT(reload_model()));
    curr->setEnabled(true);

    setCentralWidget(po);
}

void MyCash::settings()
{
    Settings *st = new Settings;

    curr->setEnabled(false);

    setCentralWidget(st);

//    QMessageBox::information(this, "Settings", "Settings");
}

void MyCash::aboutProgram()
{
    QMessageBox::about(this, tr("MyCash"), tr("MyCash ver ") + version);
}

void MyCash::update_curr()
{
    var.setCurrency(curr->value());
    var.setSymbol(db->get_currency_scod(var.Currency()));

    QMap<QString,double> list = db->get_currency_list();
    var.setKurs(list[var.Symbol()]);

    emit update_currency();
}

void MyCash::report1()
{
    QTextEdit *te = new QTextEdit;
    QSqlQuery q;
    QDate localdate = QDate::currentDate();
    int month, day, year;
    double summ;

    curr->setEnabled(false);

    te->setReadOnly(true);
    setCentralWidget(te);

    month = localdate.month();
    day = localdate.day();
    year = localdate.year();

    te->append("Dohodi:");
    QMap<QString,double> list = db->get_operation_list(3,month,year);
    QMap<QString,double>::iterator i;
    list = db->get_operation_list(3,month,year);
    for (i = list.begin(); i != list.end(); i++) {
        te->append(i.key() + ": " + default_locale->toString(i.value(),'f',2));
        summ += i.value();
    }
    te->append("Total: " + default_locale->toString(summ,'f',2));

    te->append("-----------------------------------------------");
    te->append("Rashodi:");

    list = db->get_operation_list(4,month,year);
    for (i = list.begin(); i != list.end(); i++) {
        te->append(i.key() + ": " + default_locale->toString(i.value(),'f',2));
        summ += i.value();
    }
    te->append("Total: " + default_locale->toString(summ,'f',2));

    te->append("-----------------------------------------------");

    summ = db->get_account_summ(1);
    te->append("Balance: " + default_locale->toString(summ,'f',2));
}

void MyCash::report2()
{
    graphWidget *gw = new graphWidget;

    curr->setEnabled(false);

    setCentralWidget(gw);
}

void MyCash::test()
{
    QSqlQuery q1, q2, q3;

    q3.prepare("UPDATE account_oper set direction = :dir WHERE id = :id");

    q1.prepare("SELECT id,acc_from,acc_to,summ FROM operation");
    if (!q1.exec()) {
        qDebug() << q1.lastError().text();
        return;
    }
    while (q1.next()) {
        int id = q1.value(0).toInt();
        int from = q1.value(1).toInt();
        Account_Data from_data = db->get_account_data(from);
        int to = q1.value(2).toInt();
        Account_Data to_data = db->get_account_data(to);
        double summ1 = q1.value(3).toDouble();
        qDebug() << summ1;

        q2.prepare("SELECT id,a_id,o_id,summ FROM account_oper WHERE o_id = :id AND a_id = :from");
        q2.bindValue(":id", id);
        q2.bindValue(":from", from);
        if (!q2.exec()) {
            qDebug() << q2.lastError().text();
            return;
        }
        while (q2.next()) {
            int id2 = q2.value(0).toInt();
            int a_id = q2.value(1).toInt();
            int o_id = q2.value(2).toInt();
            double summ2 = q2.value(3).toDouble();
            q3.bindValue(":dir", 1);
            q3.bindValue(":id", id2);
            if (!q3.exec()) {
                qDebug() << q3.lastError().text();
                return;
            }
        }

        q2.prepare("SELECT id,a_id,o_id,summ FROM account_oper WHERE o_id = :id AND a_id = :to");
        q2.bindValue(":id", id);
        q2.bindValue(":from", to);
        if (!q2.exec()) {
            qDebug() << q2.lastError().text();
            return;
        }
        while (q2.next()) {
            int id2 = q2.value(0).toInt();
            int a_id = q2.value(1).toInt();
            int o_id = q2.value(2).toInt();
            double summ2 = q2.value(3).toDouble();
            q3.bindValue(":dir", 2);
            q3.bindValue(":id", id2);
            if (!q3.exec()) {
                qDebug() << q3.lastError().text();
                return;
            }
        }
    }
}
