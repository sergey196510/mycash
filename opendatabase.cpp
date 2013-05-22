#include "opendatabase.h"
#include "ui_opendatabase.h"

OpenDatabase::OpenDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDatabase)
{
    ui->setupUi(this);

    connect(ui->nameButton, SIGNAL(released()), SLOT(select_filename()));
}

OpenDatabase::~OpenDatabase()
{
    delete ui;
}

void OpenDatabase::select_filename()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose File"), ".", tr("Database file (*.db)"));
    if (!filename.isEmpty())
        ui->nameEdit->setText(filename);
}

QString OpenDatabase::filename()
{
    return ui->nameEdit->text();
}
