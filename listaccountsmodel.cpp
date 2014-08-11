#include "listaccountsmodel.h"

ViewCurrency::ViewCurrency(int col, QObject *parent) :
    QItemDelegate(parent)
{
    this->column = col;
}

/*
void ViewCurrency::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == column) {
        double value = index.model()->data(index,Qt::DisplayRole).toDouble();
        QString text = default_locale->toString(value,'f',2);
        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignRight | Qt::AlignCenter;

        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    }
    else
        QItemDelegate::paint(painter, option, index);
}
*/

double ListAccountsModel::get_reserv(int id)
{
    Account_Data acc = db->get_account_data(id);
    QList<Operation_Data>::iterator i;
    Operation_Data oper;
    double summ = 0;

    if (acc.top != Account_Type::active)
        return 0;

    for (i = plan_list.begin(); i != plan_list.end(); i++) {
        oper = *i;
        if (oper.from.at(0).account() == id)
            summ += oper.from.at(0).balance().value();
    }

    return summ;
}

double ListAccountsModel::get_list(int parent, QModelIndex idx)
{
    QSqlQuery q;
//    int id;
    int i = 0;
//    int row = 0;
    double summ = 0, summ2 = 0;

    q.prepare("SELECT id,name,balance,descr,ccod,hidden FROM account WHERE parent = :parent ORDER BY name");
    q.bindValue(":parent", parent);
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return 0;
    }
    while (q.next()) {
        if (q.value(5) == false) {
            summ += db->convert_currency(q.value(2).toDouble(), q.value(4).toInt());

//            QString scod = list[q.value(4).toInt()];
//            double account_kurs = list_curs[scod];
//            double global_kurs = list_curs[var.Symbol()];
//            double kurs = account_kurs / global_kurs;
//            summ += q.value(2).toDouble() * kurs;
        }
        else
            continue;
        if (i == 0)
            insertColumns(0,7,idx);
        insertRow(i, idx);
        list_index[q.value(0).toInt()] = index(i,0,idx);
//        qDebug() << q.value(0).toInt() << index(i,0,idx);
        setData(index(i,0,idx), q.value(1).toString());
        setData(index(i,1,idx), q.value(2).toDouble());
        setData(index(i,2,idx), get_reserv(q.value(0).toInt()));
        setData(index(i,3,idx), q.value(4).toInt());
        setData(index(i,4,idx), q.value(5).toBool());
        setData(index(i,5,idx), q.value(3).toString());
        setData(index(i,6,idx), q.value(0).toInt());
        summ2 = get_list(q.value(0).toInt(), index(i,0,idx));
        setData(index(i,1,idx), summ2+q.value(2).toDouble());
        summ += summ2;
        i++;
    }

    return summ;
}

QMap<int,QModelIndex> ListAccountsModel::fill_model()
{
    QSqlQuery query;
//    int type;
//    int row = 0;
    double summ, summ2;
    QModelIndex idx;
    QFont fnt;
    header_data << tr("Name") << tr("Balance") << ("Reserved") << tr("Currency") << "Hidden" << tr("Description") << "" << "";

    clear();

    if (!var.database_Opened())
        return list_index;

//    list = db->get_scod_list();
//    list_curs = db->get_currency_list();
    fnt.setBold(true);

    insertColumns(0,7);

    int i = 0;
    summ = 0;
    QSqlQuery q;
    q.prepare("SELECT id,name,balance,descr,ccod,hidden FROM account WHERE parent = 0 ORDER BY id");
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return list_index;
    }
    while (q.next()) {
        if (q.value(5) == false) {
            summ += db->convert_currency(q.value(2).toDouble(), q.value(4).toInt());
        }
        else
            continue;
        insertRow(i);
        list_index[q.value(0).toInt()] = index(i,0,QModelIndex());
        for (int j = 0; j < 7; j++)
//            setData(index(i,j), QColor(Qt::gray), Qt::BackgroundColorRole);
            setData(index(i,0,QModelIndex()), QFont(fnt), Qt::FontRole);
        setData(index(i,0,QModelIndex()), q.value(1).toString());
        setData(index(i,1,QModelIndex()), q.value(2).toDouble());
        setData(index(i,3,QModelIndex()), q.value(4).toInt());
        setData(index(i,4,QModelIndex()), q.value(5).toBool());
        setData(index(i,5,QModelIndex()), q.value(3).toString());
        setData(index(i,6,QModelIndex()), q.value(0).toInt());
        summ2 = get_list(q.value(0).toInt(), index(i,0,QModelIndex()));
        setData(index(i,1), summ2+q.value(2).toDouble());
        summ += summ2;
        i += 1;
    }

    return list_index;
}

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QStandardItemModel(parent)
{
    db = new Database;
    plan_list = db->get_plan_oper_list(1);
}

ListAccountsModel::~ListAccountsModel()
{
    delete db;
}

QVariant ListAccountsModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QStandardItemModel::data(index, role);

    if (!index.isValid())
        return QVariant();

//    qDebug() << index;

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 1) {
            return default_locale->toString(value.toDouble(),'f',2);
        }
        else if (index.column() == 2)
            if (value.toDouble())
                return default_locale->toString(value.toDouble(),'f',2);
            else
                return QVariant();
        else if (index.column() == 3) {
//            return db->get_currency_scod(value.toInt());
            return db->scod_list[value.toInt()];
        }
        else if (index.column() == 4) {
            return (value.toBool() == false) ? QVariant() : tr("H");
        }
        else
            return value;

        case Qt::TextAlignmentRole:
            if (index.column() == 1)
                return int(Qt::AlignRight | Qt::AlignVCenter);
            if (index.column() == 2)
                return int(Qt::AlignRight | Qt::AlignVCenter);

    case Qt::TextColorRole:
//        qDebug() << index.data(Qt::DisplayRole) << value.toString();
        if (index.data(Qt::DisplayRole).toString() == "true") {
            return QVariant(QColor(Qt::gray));
        }

        return value;
    }

    return value;
}

QVariant ListAccountsModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}
