#include <QtGui>
#include <QtSql>
#include "mycash.h"
#include "ui_mycash.h"
#include "createdatabase.h"
#include "opendatabase.h"
#include "listaccounts.h"
#include "listoperations.h"
#include "listagents.h"
#include "global.h"

MyCash::MyCash(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyCash)
{
    ui->setupUi(this);

    opened = false;

    readsettings();
    setconnects();

    if (!dbname.isEmpty())
	opendb(dbname);

//    QDockWidget *pdock = new QDockWidget("MyDock", this);
//    QLabel * plbl = new QLabel("Label in Dock", pdock);
//    pdock->addAction(ui->action_ListAccounts);
//    pdock->setWidget(plbl);
//    addDockWidget(Qt::LeftDockWidgetArea, pdock);

    emit call_mark();
}

MyCash::~MyCash()
{
    writesettings();
    delete ui;
}

void MyCash::setconnects()
{
    ui->action_Quit->setIcon(QPixmap(":icons/stop_16.png"));

    connect(this,                    SIGNAL(call_mark()), SLOT(mark_Object()));
    connect(ui->action_Create,       SIGNAL(triggered()), SLOT(create()));
    connect(ui->action_Open,         SIGNAL(triggered()), SLOT(open()));
    connect(ui->action_Close,        SIGNAL(triggered()), SLOT(closeDatabase()));
    connect(ui->action_Settings,     SIGNAL(triggered()), SLOT(settings()));
    connect(ui->action_Quit,         SIGNAL(triggered()), SLOT(close()));

    connect(ui->action_ListAccounts, SIGNAL(triggered()), SLOT(list_accounts()));
    connect(ui->action_ListOperations, SIGNAL(triggered()), SLOT(list_operations()));
    connect(ui->action_ListAgents, SIGNAL(triggered()), SLOT(list_agents()));

    connect(ui->actionAbout_program, SIGNAL(triggered()), SLOT(aboutProgram()));
    connect(ui->actionAbout_QT,      SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    ui->mainToolBar->addAction(ui->action_Create);
    ui->mainToolBar->addAction(ui->action_Open);
    ui->mainToolBar->addAction(ui->action_Close);
    ui->mainToolBar->addAction(ui->action_ListAccounts);
    ui->mainToolBar->addAction(ui->action_ListOperations);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->action_Quit);
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
    current_account = settings.value("current_account", "").toInt();
//    fnt = settings.value("operations_font");
}

void MyCash::writesettings()
{
    QSettings settings("MyCash", "MyCash");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("dbname", dbname);
    settings.setValue("current_account", current_account);
    settings.setValue("operations_font", fnt);
}

void MyCash::create()
{
    CreateDatabase *db = new CreateDatabase(this);

    db->exec();

    delete db;
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

void MyCash::list_accounts()
{
    ListAccounts *la = new ListAccounts;

//    connect(ui->action_NewAccounts, SIGNAL(triggered()), la, SLOT(new_account()));

    setCentralWidget(la);
}

void MyCash::list_operations()
{
    ListOperations *lo = new ListOperations;

    setCentralWidget(lo);
}

void MyCash::list_agents()
{
    ListAgents *la = new ListAgents;

    setCentralWidget(la);
}

void MyCash::settings()
{
    QMessageBox::information(this, "Settings", "Settings");
}

void MyCash::aboutProgram()
{
    QMessageBox::about(this, tr("MyCash"), tr("MyCash ver 0.1"));
}
