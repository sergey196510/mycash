#include <QtGui>
#include <QtSql>
#include "mycash.h"
#include "ui_mycash.h"
#include "opendatabase.h"
#include "listaccounts.h"

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

    emit call_mark();
}

MyCash::~MyCash()
{
    writesettings();
    delete ui;
}

void MyCash::setconnects()
{
    connect(this,                    SIGNAL(call_mark()), SLOT(mark_Object()));
    connect(ui->action_Open,         SIGNAL(triggered()), SLOT(open()));
    connect(ui->action_Close,        SIGNAL(triggered()), SLOT(closeDatabase()));
    connect(ui->action_Settings,     SIGNAL(triggered()), SLOT(settings()));
    connect(ui->action_Quit,         SIGNAL(triggered()), SLOT(close()));

    connect(ui->action_ListAccounts, SIGNAL(triggered()), SLOT(list_accounts()));

    connect(ui->actionAbout_program, SIGNAL(triggered()), SLOT(aboutProgram()));
    connect(ui->actionAbout_QT,      SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MyCash::mark_Object()
{
    ui->action_Create->setEnabled(!opened);
    ui->action_Open->setEnabled(!opened);
    ui->action_Close->setEnabled(opened);
    ui->menu_Accounts->setEnabled(opened);
}

void MyCash::readsettings()
{
    QSettings settings("MyCash", "MyCash");
    restoreGeometry(settings.value("geometry").toByteArray());
    dbname = settings.value("dbname", "").toString();
}

void MyCash::writesettings()
{
    QSettings settings("MyCash", "MyCash");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("dbname", dbname);
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
