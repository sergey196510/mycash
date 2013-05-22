#include <QtGui>
#include <QtSql>
#include "mycash.h"
#include "ui_mycash.h"
#include "opendatabase.h"

MyCash::MyCash(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyCash)
{
    ui->setupUi(this);

    opened = false;

    readsettings();
    setconnects();

    emit call_mark();
}

MyCash::~MyCash()
{
    writesettings();
    delete ui;
}

void MyCash::setconnects()
{
    connect(this, SIGNAL(call_mark()), SLOT(mark_Object()));
    connect(ui->action_Open, SIGNAL(triggered()), SLOT(open()));
    connect(ui->action_Settings, SIGNAL(triggered()), SLOT(settings()));
    connect(ui->action_Quit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionAbout_program, SIGNAL(triggered()), SLOT(aboutProgram()));
    connect(ui->actionAbout_QT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MyCash::mark_Object()
{
    ui->action_Create->setEnabled(!opened);
    ui->action_Open->setEnabled(!opened);
    ui->actionC_lose->setEnabled(opened);
}

void MyCash::readsettings()
{
    QSettings settings("MyCash", "MyCash");
    restoreGeometry(settings.value("geometry").toByteArray());
}

void MyCash::writesettings()
{
    QSettings settings("MyCash", "MyCash");
    settings.setValue("geometry", saveGeometry());
}

void MyCash::open()
{
    OpenDatabase *db = new OpenDatabase(this);
    QString filename;

    if (db->exec() == QDialog::Rejected) {
        delete db;
        return;
    }

    filename = db->filename();
    if (filename.isEmpty()) {
        delete db;
        return;
    }

    delete db;
}

void MyCash::settings()
{
    QMessageBox::information(this, "Settings", "Settings");
}

void MyCash::aboutProgram()
{
    QMessageBox::about(this, tr("MyCash"), tr("MyCash ver 0.1"));
}
