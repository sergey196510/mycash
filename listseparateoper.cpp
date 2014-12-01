#include "listseparateoper.h"
#include "ui_listseparateoper.h"

AccountDelegate::AccountDelegate(int column, QObject *parent) :
    QItemDelegate(parent)
{
    this->column = column;
}

//void AccountDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    if (index.column() == column) {
//        int value = index.model()->data(index,Qt::DisplayRole).toInt();
//        QString text = QString("%1").arg(value);
//        drawDisplay(painter, option, option.rect, text);
//    }
//    else
//        QItemDelegate::paint(painter, option, index);
//}

QWidget *AccountDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == column) {
        int value = index.model()->data(index,Qt::DisplayRole).toInt();
        AccountOper2 *oper = new AccountOper2(parent);
        oper->setValue(value);
        return oper;
    }
    else
        return QItemDelegate::createEditor(parent, option, index);
}

void AccountDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == column) {
        int value = index.model()->data(index,Qt::EditRole).toInt();
        AccountOper2 *oper = static_cast<AccountOper2*>(editor);
        oper->setValue(value);
    }
    else
        QItemDelegate::setEditorData(editor, index);
}

void AccountDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == column) {
        AccountOper2 *oper = static_cast<AccountOper2*>(editor);
        int value = oper->value();
        model->setData(index,value);
        account_summ acc;
    }
    else
        QItemDelegate::setEditorData(editor, index);
}

void AccountDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

ListSeparateOperModel::ListSeparateOperModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    Account a;
    account_summ data;
    Database db;

    header_data << tr("Account") << tr("Summ");

//    acc_list = db.get_accounts_list();

    insertColumns(0,2);

    a.read(1);
    data.set_account(a);
    data.set_balance(10);
    list.append(data);

    a.read(2);
    data.set_account(a);
    data.set_balance(20);
    list.append(data);

    a.read(0);
    data.set_account(a);
    data.set_balance(0);
    list.append(data);
}

int ListSeparateOperModel::rowCount(const QModelIndex &parent) const
{
//    qDebug() << list.count();
    if (parent.isValid())
        return 0;
    return list.size();
}

int ListSeparateOperModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return header_data.size();
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
        account_summ data = list.at(index.row());
        if (index.column() == 0)
            return acc_list[data.account().Id()];
        else if (index.column() == 1)
            return data.balance().toDouble();
    }
//    else
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

/*
bool ListSeparateOperModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    account_summ data;

    if (index.isValid() && role == Qt::EditRole) {
        qDebug() << list.count() << index.row() << index.column();

        if (index.row() < list.count())
            data = list.at(index.row());
        else {
            data.set_account(0);
            data.set_balance(0);
        }

        if (index.column() == 0)
            data.set_account(value.toInt());
        else if (index.column() == 1)
            data.set_balance(value.toDouble());

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
*/

/*
bool ListSeparateOperModel::insertRows(int row, int count, const QModelIndex &parent)
{
    account_summ data;

    beginInsertRows(QModelIndex(), row, 1);

    data.set_account(0);
    data.set_balance(0);
    list.append(data);

    endInsertRows();
    return true;
}

bool ListSeparateOperModel::removeRows(int row, int count, const QModelIndex &)
{
    beginRemoveRows(QModelIndex(), row, 1);

    list.removeAt(row);

    endRemoveRows();
    return true;
}
*/

QList<account_summ> ListSeparateOperModel::getData()
{
    return list;
//    account_summ data;
//    QMap<int,double> lst;

//    for (int i = 0; i < list.count(); i++) {
//        data = list.at(i);
//        if (data.account() != 0)
//            lst[data.account()] = data.balance().value();
//    }

//    return lst;
}

ListSeparateOper::ListSeparateOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListSeparateOper)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint);

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
    delete model;
}

QList<account_summ> ListSeparateOper::data()
{
    return model->getData();
}
