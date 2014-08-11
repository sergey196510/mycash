#include <QtGui>
#include <QtSql>
#include "mycash.h"
#include "ui_mycash.h"
#include "opendatabase.h"
#include "listaccounts.h"
#include "listcurrency.h"
#include "listoperations.h"
#include "listagents.h"
#include "listplanoper.h"
#include "listbudget.h"
#include "mainwidget.h"
#include "graphwidget.h"
#include "global.h"
#include "settings.h"
#include "debetcreditreport.h"

MyCash::MyCash(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyCash)
{
    ui->setupUi(this);

//    opened = false;

    readsettings();

    setconnects();
    emit call_mark();

    base = QSqlDatabase::addDatabase("QSQLITE");
    if (!dbname.isEmpty())
        opendb(dbname);
    else
        return;

    db = new Database;

    curr = new CurrencyComboBox;
    curr->setValue(var.Currency());
    curr->setToolTip(tr("Выберите текущую валюту"));
//    currs = db->get_scod_list();
    ui->mainToolBar->addWidget(curr);
    connect(curr, SIGNAL(currentIndexChanged(int)), SLOT(update_curr()));

//    var.setSymbol(db->scod_list[var.Currency()]);
    var.setSymbol(db->scod_list[var.Currency()]);

    list_home();

    emit call_mark();
}

MyCash::~MyCash()
{
    writesettings();
    delete db;
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
    connect(ui->actionBudget, SIGNAL(triggered()), SLOT(list_budget()));

    connect(ui->actionReport_1, SIGNAL(triggered()), SLOT(report1()));
    connect(ui->actionReport_2, SIGNAL(triggered()), SLOT(report2()));
//    connect(ui->actionTest_2, SIGNAL(triggered()), SLOT(test()));

    connect(ui->actionAbout_program, SIGNAL(triggered()), SLOT(aboutProgram()));
    connect(ui->actionAbout_QT,      SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    ui->mainToolBar->addAction(ui->action_Create);
    ui->mainToolBar->addAction(ui->action_Open);
    ui->mainToolBar->addAction(ui->action_Close);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->action_Home);
    ui->mainToolBar->addAction(ui->action_ListAccounts);
    ui->mainToolBar->addAction(ui->action_ListOperations);
    ui->mainToolBar->addAction(ui->action_Plan_Operations);
    ui->mainToolBar->addAction(ui->action_ListAgents);
    ui->mainToolBar->addAction(ui->actionBudget);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->action_Settings);
    ui->mainToolBar->addAction(ui->action_Quit);

    setWindowTitle("MyCash version: " + var.Version() + " [" + dbname + "]");
}

void MyCash::mark_Object()
{
    ui->action_Create->setEnabled(!var.database_Opened());
    ui->action_Open->setEnabled(!var.database_Opened());
    ui->action_Close->setEnabled(var.database_Opened());
    ui->action_ListAgents->setEnabled(var.database_Opened());
    ui->action_ListCurrencies->setEnabled(var.database_Opened());
    ui->actionReport_1->setEnabled(var.database_Opened());
    ui->actionReport_2->setEnabled(var.database_Opened());
    ui->menu_Accounts->setEnabled(var.database_Opened());
    ui->menuReports->setEnabled(var.database_Opened());
    ui->action_ListAccounts->setEnabled(var.database_Opened());
    ui->action_ListOperations->setEnabled(var.database_Opened());
    ui->action_Home->setEnabled(var.database_Opened());
    ui->action_Settings->setEnabled(var.database_Opened());
}

void MyCash::readsettings()
{
    QSettings settings("MyCash", "MyCash");
    restoreGeometry(settings.value("geometry").toByteArray());
    dbname = settings.value("dbname").toString();
    var.setAccount(settings.value("current_account", 1).toInt());
    var.setCurrency(settings.value("current_currency", 1).toInt());
    var.setCorrectAccount(settings.value("correct_account").toInt());
    var.setInitialAccount(settings.value("initial_account").toInt());
    var.setPrecision(settings.value("precision", 2).toInt());
//    var.setListFont(settings.value("list_font").toString());
//    fnt = settings.value("operations_font");
//    var.setSymbol(db->scod_list[var.Currency()]);
}

void MyCash::writesettings()
{
    QSettings settings("MyCash", "MyCash");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("dbname", dbname);
    settings.setValue("current_account", var.Account());
    settings.setValue("current_currency", var.Currency());
    settings.setValue("correct_account", var.CorrectAccount());
    settings.setValue("initial_account", var.InitialAccount());
    settings.setValue("precision", var.Precision());
//    settings.setValue("list_font", var.ListFont());
//    settings.setValue("operations_font", fnt);
}

void MyCash::create()
{
    OpenDatabase *dbf = new OpenDatabase(this);
    QString str;

    if (dbf->exec() != QDialog::Accepted) {
        delete dbf;
        return;
    }

    dbname = dbf->filename();
    base.setDatabaseName(dbname);
    if (!base.open()) {
        var.database_Close();
        qDebug() << base.lastError().text();
    }

    QSqlQuery q;

    QFile file(":mycash.sql");
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

    var.database_Open();
    emit call_mark();
    delete dbf;
}

void MyCash::open()
{
    OpenDatabase *dbf = new OpenDatabase(this);

    dbf->set_filename(dbname);
    if (dbf->exec() == QDialog::Rejected) {
        delete dbf;
        return;
    }

    dbname = dbf->filename();
    if (dbname.isEmpty()) {
        delete dbf;
        return;
    }

    opendb(dbname);

    curr = new CurrencyComboBox;
    curr->setValue(var.Currency());
    curr->setToolTip(tr("Select current currency"));
//    currs = db->get_scod_list();
    ui->mainToolBar->addWidget(curr);
    connect(curr, SIGNAL(currentIndexChanged(int)), SLOT(update_curr()));

    delete dbf;
}

void MyCash::closeDatabase()
{
    delete curr;
//    base = QSqlDatabase::database();
    base.close();
//    opened = false;
    var.database_Close();
    emit call_mark();
    setWindowTitle("MyCash version: " + var.Version() + " []");
}

void MyCash::opendb(QString dbname)
{
    if (!QFile::exists(dbname)) {
        create();
        return;
    }

    base.setDatabaseName(dbname);
    if (!base.open()) {
        var.database_Close();
        qDebug() << base.lastError().text();
    }
    else {
        var.database_Open();
        db = new Database;
        check_plan_oper();
    }

    emit call_mark();
    setWindowTitle("MyCash version: " + var.Version() + " [" + dbname + "]");
}

void MyCash::list_home()
{
    MainWidget *mw = new MainWidget(db);

    connect(ui->action_Open, SIGNAL(triggered()), mw, SLOT(reload_model()));
    connect(ui->action_Close, SIGNAL(triggered()), mw, SLOT(clear_model()));
    connect(this, SIGNAL(update_currency()), mw, SLOT(reload_model()));
    curr->setEnabled(true);

    setCentralWidget(mw);
}

void MyCash::list_accounts()
{
    ListAccounts *la = new ListAccounts(db);

    ui->mainToolBar->addActions(la->acts);

    connect(ui->action_Open, SIGNAL(triggered()), la, SLOT(reload_model()));
    connect(ui->action_Close, SIGNAL(triggered()), la, SLOT(clear_model()));
    curr->setEnabled(false);

    setCentralWidget(la);
}

void MyCash::list_currency()
{
    ListCurrency *lc = new ListCurrency;

    connect(ui->action_Open, SIGNAL(triggered()), lc, SLOT(reload_model()));
    connect(ui->action_Close, SIGNAL(triggered()), lc, SLOT(clear_model()));
    curr->setEnabled(false);

    setCentralWidget(lc);
}

void MyCash::list_operations()
{
    ListOperations *lo = new ListOperations;

    ui->mainToolBar->addActions(lo->acts);

    connect(ui->action_Open, SIGNAL(triggered()), lo, SLOT(reload_model()));
    connect(ui->action_Close, SIGNAL(triggered()), lo, SLOT(clear_model()));
    connect(this, SIGNAL(update_currency()), lo, SLOT(reload_model()));
    curr->setEnabled(false);

    setCentralWidget(lo);
}

void MyCash::list_agents()
{
    ListAgents *la = new ListAgents;

    connect(ui->action_Open, SIGNAL(triggered()), la, SLOT(reload_model()));
    connect(ui->action_Close, SIGNAL(triggered()), la, SLOT(clear_model()));
    curr->setEnabled(false);

    setCentralWidget(la);
}

void MyCash::list_plan_oper()
{
    ListPlanOper *po = new ListPlanOper;

    ui->mainToolBar->addActions(po->acts);

    connect(ui->action_Open, SIGNAL(triggered()), po, SLOT(reload_model()));
    connect(ui->action_Close, SIGNAL(triggered()), po, SLOT(clear_model()));
    connect(this, SIGNAL(update_currency()), po, SLOT(reload_model()));
    curr->setEnabled(true);

    setCentralWidget(po);
}

void MyCash::list_budget()
{
    ListBudget *lb = new ListBudget;

    ui->mainToolBar->addActions(lb->acts);

    setCentralWidget(lb);
}

void MyCash::settings()
{
    Settings *st = new Settings(this);

    st->exec();
}

void MyCash::aboutProgram()
{
    QMessageBox::about(this, tr("MyCash"), tr("MyCash ver ") + var.Version() + " Build: " + QLibraryInfo::buildDate().toString("dd-MM-yyyy") + " " + QLibraryInfo::licensedProducts());
}

void MyCash::update_curr()
{
    var.setCurrency(curr->value());
    var.setSymbol(db->scod_list[var.Currency()]);

//    QMap<QString,double> list = db->get_currency_list();
    var.setKurs(db->currency_list[var.Symbol()]);

    emit update_currency();
}

void MyCash::report1()
{
    DebetCreditReport *dc = new DebetCreditReport;

    setCentralWidget(dc);
}

void MyCash::report2()
{
    graphWidget *gw = new graphWidget;

    curr->setEnabled(false);

    setCentralWidget(gw);
}

// проверка выполнения плановых операций в тек месяце
void MyCash::check_plan_oper()
{
//    int mon = QDate::currentDate().month(), year = QDate::currentDate().year();
    QList<Operation_Data> list = db->get_plan_oper_list(0);
    QList<Operation_Data>::iterator i;
    Account_Data from, to;

//    qDebug() << mon << year;

    for (i = list.begin(); i != list.end(); i++) {
        Operation_Data data = *i;
        if (data.auto_exec == 0)
            continue;
        if (data.status != Plan_Status::expired)
            continue;
        from = db->get_account_data(data.from.at(0).account());
        to = db->get_account_data(data.from.at(0).account());
        if (from.balance.value() < data.to.at(0).balance().value()) {
            QMessageBox::warning(0,
                                 tr("Plan operation"),
                                 tr("Plan operation\nFrom: %1 To: %2\nNedostatocjno sredstv")
                                 .arg(from.name)
                                 .arg(to.name));
            continue;
        }
        db->save_operation(data);
        db->new_mon_oper(data.id,1);
        qDebug() << data.id;
    }
}
