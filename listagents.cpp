#include "listagents.h"
#include "ui_listagents.h"

ListAgents::ListAgents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListAgents)
{
    ui->setupUi(this);
}

ListAgents::~ListAgents()
{
    delete ui;
}
