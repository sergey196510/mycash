#include "opendatabase.h"
#include "ui_opendatabase.h"

OpenDatabase::OpenDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDatabase)
{
    ui->setupUi(this);

    connect(ui->nameEdit,     SIGNAL(textChanged(QString)), SLOT(nameChanged(QString)));
    connect(ui->nameButton,   SIGNAL(released()), SLOT(select_filename()));
    connect(ui->okButton,     SIGNAL(released()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(released()), SLOT(reject()));

    ui->okButton->setEnabled(false);
}

OpenDatabase::~OpenDatabase()
{
    delete ui;
}

void OpenDatabase::select_filename()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose File"), ".", tr("Database file (*.db)"));
    if (!filename.isEmpty()) {
        ui->nameEdit->setText(filename);
 //       ui->okButton->setEnabled(true);
    }
}

QString OpenDatabase::filename()
{
    return ui->nameEdit->text();
}

void OpenDatabase::nameChanged(QString str)
{
//    qDebug() << str;
    if (str.length() > 0)
        ui->okButton->setEnabled(true);
    else
        ui->okButton->setEnabled(false);
}
