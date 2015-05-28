#include "listseparateoper.h"
#include "ui_listseparateoper.h"

AccountDelegate::AccountDelegate(int column, QObject *parent) :
    QItemDelegate(parent)
{
    this->column = column;
}

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
//        qDebug() << "setEditorData:" << value;
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
//        qDebug() << "setModelData:" << value;
        model->setData(index,value);
        account_summ acc;
    }
    else
        QItemDelegate::setModelData(editor, model, index);

//    qDebug() << "Model data:" << model->data(index);
//        QItemDelegate::setEditorData(editor, index);
}

void AccountDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

ListSeparateOperModel::ListSeparateOperModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    Account a;
    account_summ data;
    Database db;

    header_data << tr("Account") << tr("Summ");

//    insertColumns(0,2);

//    a.read(1);
//    data.set_account(a);
//    data.set_balance(10);
//    list.append(data);

//    a.read(2);
//    data.set_account(a);
//    data.set_balance(20);
//    list.append(data);

//    a.read(0);
//    data.set_account(a);
//    data.set_balance(0);
//    list.append(data);
}

int ListSeparateOperModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return list.size()+1;
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

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (list.size() == 0)
            return QVariant();
        account_summ data = list.at(index.row());
        if (index.column() == 0) {
            Account a;
            a.read(data.account().Id());
            return a.Name();
//            return acc_list[data.account().Id()];
        }
        else if (index.column() == 1)
            return data.balance().toDouble();
    }
//    else
    return QVariant();
}

bool ListSeparateOperModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    account_summ data;

//    qDebug() << "setData:" << value;

    if (index.isValid() && role == Qt::EditRole) {
        if (index.row() < list.size()) {
            data = list.at(index.row());
            if (index.column() == 0) {
                Account a;
                a.read(value.toInt());
                data.set_account(a);
            }
            if (index.column() == 1) {
                data.set_balance(value.toDouble());
            }
            list.replace(index.row(), data);
        }
        else {
            if (index.column() == 0) {
                Account a;
                a.read(value.toInt());
                data.set_account(a);
            }
            if (index.column() == 1) {
                data.set_balance(value.toDouble());
            }
            list.append(data);
//            insertRow(index.row(),QModelIndex());
        }

        emit dataChanged(index,index);

        return true;
    }

    return false;
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

QModelIndex ListSeparateOperModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row,column);
}

QModelIndex ListSeparateOperModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QList<account_summ> ListSeparateOperModel::getData()
{
    return list;
}

//void ListSeparateOperModel::updateData()
//{
//    qDebug() << "Model::updateData()";
//    endResetModel();
//}

ListSeparateOper::ListSeparateOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListSeparateOper)
{
    ui->setupUi(this);
    AccountDelegate *ad = new AccountDelegate(0,this);
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint);

    model = new ListSeparateOperModel;

    ui->tableView->setItemDelegate(ad);
    ui->tableView->setModel(model);
//    ui->tableView->resizeRowsToContents();

//    connect(ad, SIGNAL(closeEditor(QWidget*)), model, SLOT(updateData()));

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
