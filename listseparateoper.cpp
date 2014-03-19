#include "listseparateoper.h"
#include "ui_listseparateoper.h"

AccountDelegate::AccountDelegate(int column, QObject *parent) :
    QItemDelegate(parent)
{
    this->column = column;
}

void AccountDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    if (index.column() == column) {
//        int value = index.model()->data(index,Qt::DisplayRole).toInt();
//        QString text = QString("%1").arg(value);
//        drawDisplay(painter, option, option.rect, text);
//    }
//    else
        QItemDelegate::paint(painter, option, index);
}

QWidget *AccountDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == column) {
        int value = index.model()->data(index,Qt::DisplayRole).toInt();
        AccountOper2 *oper = new AccountOper2(parent);
        oper->setValue(value);
//        connect(oper, SIGNAL())
        return oper;
    }
    else
        return QItemDelegate::createEditor(parent, option, index);
}

ListSeparateOperModel::ListSeparateOperModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    OperList data;
    Database db;

    header_data << tr("Account") << tr("Summ");

    acc_list = db.get_accounts_list();

    insertColumns(0,2);

    data.account = 1;
    data.summ = 10;
    list.append(data);

    data.account = 2;
    data.summ = 20;
    list.append(data);

    data.account = 0;
    data.summ = 0;
    list.append(data);
}

int ListSeparateOperModel::rowCount(const QModelIndex &parent) const
{
//    qDebug() << list.count();
    return list.count();
}

int ListSeparateOperModel::columnCount(const QModelIndex &parent) const
{
    return 2;
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
            return acc_list[data.account];
        else if (index.column() == 1)
            return data.summ;
    }
    else
        return QVariant();
}

QVariant ListSeparateOperModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

Qt::ItemFlags ListSeparateOperModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
         return Qt::ItemIsEnabled;

     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ListSeparateOperModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    OperList data;

    if (index.isValid() && role == Qt::EditRole) {
        qDebug() << list.count() << index.row() << index.column();

        if (index.row() < list.count())
            data = list.at(index.row());
        else {
            data.account = 0;
            data.summ = 0;
        }

        if (index.column() == 0)
            data.account = value.toInt();
        else if (index.column() == 1)
            data.summ = value.toDouble();

        list.replace(index.row(), data);
        qDebug() << list.count();
        if (index.row() == list.count()-1) {
//            data.account = 0;
//            data.summ = 0;
//            list.append(data);
            qDebug() << "Append";
            insertRows(1, 1, index);
//            insertRows(list.count()-1, 1, index);
        }
         emit dataChanged(index, index);
         return true;
     }
     return false;
}

bool ListSeparateOperModel::insertRows(int row, int count, const QModelIndex &parent)
{
    OperList data;

    beginInsertRows(QModelIndex(), row, 1);

    data.account = 0;
    data.summ = 0;
    list.append(data);

    endInsertRows();
    return true;
}

bool ListSeparateOperModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, 1);

    list.removeAt(row);

    endRemoveRows();
    return true;
}

QMap<int,double> ListSeparateOperModel::getData()
{
    OperList data;
    QMap<int,double> lst;

    for (int i = 0; i < list.count(); i++) {
        data = list.at(i);
        if (data.account != 0)
            lst[data.account] = data.summ;
    }

    return lst;
}

ListSeparateOper::ListSeparateOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListSeparateOper)
{
    ui->setupUi(this);

    model = new ListSeparateOperModel;

    ui->tableView->setItemDelegate(new AccountDelegate(0));
    ui->tableView->setModel(model);
//    ui->tableView->resizeRowsToContents();

    ui->okButton->setDefault(false);
    ui->cancelButton->setDefault(false);
}

ListSeparateOper::~ListSeparateOper()
{
    delete ui;
}

QMap<int,double> ListSeparateOper::data()
{
    return model->getData();
}