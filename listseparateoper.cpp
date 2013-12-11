#include "listseparateoper.h"
#include "ui_listseparateoper.h"

int ListSeparateOperModel::rowCount(const QModelIndex &parent) const
{
    return list.count();
}

QVariant ListSeparateOperModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= list.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (list.size() == 0)
            return QVariant();
        OperList data = list.at(index.row());
        if (index.column() == 0)
            return data.account;
        else if (index.column() == 1)
            return data.summ;
    }
    else
        return QVariant();
}

QVariant ListSeparateOperModel::headerData(int section, Qt::Orientation orientation, int role) const
{

}

ListSeparateOper::ListSeparateOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListSeparateOper)
{
    ui->setupUi(this);

    model = new ListSeparateOperModel;

    ui->tableView->setModel(model);
}

ListSeparateOper::~ListSeparateOper()
{
    delete ui;
}
