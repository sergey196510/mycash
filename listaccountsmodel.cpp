#include "listaccountsmodel.h"
#include "operation.h"

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

MyCurrency ListAccountsModel::get_reserv(int id)
{
    Account acc;
    QVector<PlanOperation>::iterator i;
    Operation oper;
    account_summ as;
    MyCurrency summ = 0;

    acc.read(id);
    if (acc.Top() != Account_Type::active)
        return 0;

    for (i = plan_list.begin(); i != plan_list.end(); i++) {
        oper = *i;
        if (oper.From().at(0).account().Id() == id) {
            as = oper.From().at(0);
            summ += as.balance();
        }
    }

    return summ;
}

MyCurrency ListAccountsModel::get_list(int parent, QModelIndex idx)
{
    QSqlQuery q;
//    int id;
    int i = 0;
//    int row = 0;
    MyCurrency summ = 0, summ2 = 0, summ3;
    MyCurrency reserv;

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
        reserv = get_reserv(q.value(0).toInt());
        list_index[q.value(0).toInt()] = index(i,0,idx);
//        qDebug() << q.value(0).toInt() << index(i,0,idx);
        setData(index(i,0,idx), q.value(1).toString());
        setData(index(i,1,idx), q.value(2).toDouble());
        setData(index(i,2,idx), reserv.toDouble());
        setData(index(i,3,idx), q.value(4).toInt());
        setData(index(i,4,idx), q.value(5).toBool());
        setData(index(i,5,idx), q.value(3).toString());
        setData(index(i,6,idx), q.value(0).toInt());
        summ2 = get_list(q.value(0).toInt(), index(i,0,idx));
        summ3 = summ2 + q.value(2).toDouble();
        setData(index(i,1,idx), summ3.toDouble());
        if (reserv>0 && reserv > q.value(2).toDouble()) {
            for (int j = 0; j < 7; j++) {
                setData(index(i,j,idx), QColor(Qt::red), Qt::BackgroundColorRole);
                setData(index(i,j,idx), QColor(Qt::white), Qt::TextColorRole);
            }
            setData(index(i,0,idx), tr("Plan operation\nNedostatochno sredstv\nTrebuetsa %1; v nalichii %2")
                    .arg(default_locale->toCurrencyString(reserv.toDouble()))
                    .arg(default_locale->toCurrencyString(q.value(2).toDouble())),
                    Qt::ToolTipRole);
        }
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
    MyCurrency summ, summ2, summ3;
    QModelIndex idx;
    QFont fnt;
    header_data << tr("Name") << tr("Balance") << ("Reserved") << tr("C") << tr("H") << tr("Description") << "" << "";

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
//        for (int j = 0; j < 7; j++)
//            setData(index(i,j), QColor(Qt::gray), Qt::BackgroundColorRole);
        setData(index(i,0,QModelIndex()), QFont(fnt), Qt::FontRole);
        setData(index(i,0,QModelIndex()), q.value(1).toString());
        setData(index(i,1,QModelIndex()), q.value(2).toDouble());
        setData(index(i,3,QModelIndex()), q.value(4).toInt());
        setData(index(i,4,QModelIndex()), q.value(5).toBool());
        setData(index(i,5,QModelIndex()), q.value(3).toString());
        setData(index(i,6,QModelIndex()), q.value(0).toInt());
        summ2 = get_list(q.value(0).toInt(), index(i,0,QModelIndex()));
        summ3 = summ2 + q.value(2).toDouble();
        setData(index(i,1), summ3.toDouble());
        summ += summ2;
        i += 1;
    }

    return list_index;
}

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QStandardItemModel(parent)
{
    db = new Database;
    PlanOperation op;

    plan_list = op.read_list(1);
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
//            return db->scod_list[value.toInt()];
            return Currency(value.toInt()).SCod();
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

//=================================== new model =====================================

Node::Node(const Account &a, Node *parentNode)
{
    acc = a;
    parent = parentNode;
}

ListAccountsModel2::ListAccountsModel2(QObject *parent) :
    QAbstractTableModel(parent)
{
    header_data << tr("Name") << tr("Balance") << ("Reserved") << tr("C") << tr("H") << tr("Description") << "" << "";
//    list = read_list();
}

ListAccountsModel2::~ListAccountsModel2()
{
}

QModelIndex ListAccountsModel2::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row,column);
}

QModelIndex ListAccountsModel2::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

Node *ListAccountsModel2::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<Node*>(index.internalPointer());
    return rootNode;
}

int ListAccountsModel2::rowCount(const QModelIndex &parent) const
{
    Node *node = nodeFromIndex(parent);
    if (node)
        return node->children.count();
    return 0;
}

int ListAccountsModel2::columnCount(const QModelIndex &parent) const
{
    return header_data.size();
}

QVariant ListAccountsModel2::data(const QModelIndex &index, int role) const
{
    int col = index.column();
    Node *node = nodeFromIndex(index);
    Account data = node->acc;

    if (!index.isValid())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:
        if (col == 0) {
//            Account data = node->acc;
            return data.Id();
        }
        if (index.column() == 1) {
//            Account data = node->acc;
            return data.Name();
        }
    }

    return QVariant();
}

QVariant ListAccountsModel2::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_data.at(section);
    }
    else
        return QString("%1").arg(section+1);
}

bool ListAccountsModel2::hasChildren(const QModelIndex &parent) const
{
    Node *parentNode = nodeFromIndex(parent);
    if (!parentNode)
        return false;
    return (parentNode->children.count() > 0);
}

void ListAccountsModel2::read_children(Node *parent, int id)
{
    QSqlQuery q;

    q.prepare("SELECT id FROM account WHERE pid = :id ORDER BY parent,id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return;
    }
    while (q.next()) {
//        Node *node = new Node;
        Account data;
        data.read(q.value(0).toInt());
        Node *node = new Node(data, parent);
        node->acc = data;
//        parent->children.append(Node);
    }
}

void ListAccountsModel2::read_data()
{
//    Account *acc = new Account;
    rootNode = new Node(Account());
    read_children(rootNode, 0);
}

/*
QVector<Account> ListAccountsModel2::read_list()
{
    QSqlQuery q;
    QVector<Account> lst;
    int i = 0;

    q.prepare("SELECT id FROM account ORDER BY parent,id");
    if (!q.exec()) {
        qDebug() << q.lastError();
        return lst;
    }
    while (q.next()) {
        Account data;
        data.read(q.value(0).toInt());
        list_index[q.value(0).toInt()] = index(i,0,QModelIndex());
        list.append(data);
        i++;
    }

    return lst;
}
*/
