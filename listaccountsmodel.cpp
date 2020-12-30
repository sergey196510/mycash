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
//    QVector<PlanOperation>::iterator i;
    Operation oper;
    account_summ as;
    MyCurrency summ = 0;

    acc.read(id);
    if (acc.Top() != Account_Type::active)
        return 0;

//    for (i = plan_list.begin(); i != plan_list.end(); i++) {
    for(auto item: plan_list) {
//        oper = *i;
        if (item.From().at(0).account().Id() == id) {
            as = oper.From().at(0);
            summ += as.balance();
        }
    }

    return summ;
}

QMap<int,QModelIndex> ListAccountsModel::fill_model(Node *p, const int parent)
{
    QSqlQuery query;
    MyCurrency summ, summ2, summ3;
    QFont fnt;
    header_data << tr("Name") << tr("Balance") << ("Reserved") << tr("C") << tr("H") << tr("Description") << "" << "";
    int row = 0;

    //clear();

    if (!var.database_Opened())
        return list_index;

    summ = 0;
    QSqlQuery q;
    q.prepare("SELECT id,name,balance,descr,ccod,hidden FROM account WHERE parent = :parent ORDER BY id");
    q.bindValue(":parent", parent);
    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return list_index;
    }
    while (q.next()) {
        Account acc;
        acc.setId(q.value(0).toInt());
        acc.setName(q.value(1).toString());
        acc.setBalance(q.value(2).toDouble());
        acc.setDescr(q.value(3).toString());
        acc.setCurr(q.value(4).toBool());
        acc.setHidden(q.value(5).toBool());

        Node *n1 = new Node;
        n1->parent = p;
        n1->acc = acc;
        n1->row = row;
        p->children.push_back(n1);
        fill_model(n1, acc.Id());
        row++;
    }

    return list_index;
}

ListAccountsModel::ListAccountsModel(QObject *parent) :
    QAbstractItemModel(parent)
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
    QVariant value = QVariant();

    if (!index.isValid())
        return QVariant();

//    qDebug() << index;

    switch (role) {
        case Qt::DisplayRole:
        if (index.column() == 1) {
            return default_locale->toString(value.toDouble(),'f',2);
        }
        else if (index.column() == 2)
            if (value.toDouble() >= 0.01)
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
//          qDebug() << index.data(Qt::DisplayRole) << value.toString();
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

QModelIndex ListAccountsModel::index(int row, int column, const QModelIndex &parent) const
{
    size_t r = static_cast<size_t>(row);

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) { // запрашивают индексы корневых узлов
        return createIndex(row, column, const_cast<Node*>(&nodes[r]));
    }

    Node* parentInfo = static_cast<Node*>(parent.internalPointer());
    return createIndex(row, column, &parentInfo->children[r]);
}

QModelIndex ListAccountsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    Node* childInfo = static_cast<Node*>(child.internalPointer());
    Node* parentInfo = childInfo->parent;
    if (parentInfo != nullptr) { // parent запрашивается не у корневого элемента
//        auto offset = parentInfo->row;
        return createIndex(parentInfo->row, 0, parentInfo);
    }
    return QModelIndex();
}

/*
int ListAccountsModel::findRow(Node *node) const
{
    const vector<Node> &parentInfoChildren = node->parent != nullptr ? node->parent->children: nodes;
    vector<Node>::const_iterator position = qFind(parentInfoChildren, *node);
    return std::distance(parentInfoChildren.begin(), position);
}
*/

int ListAccountsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return nodes.size();
    }
    const Node* parentInfo = static_cast<const Node*>(parent.internalPointer());
    return parentInfo->children.size();
}

int ListAccountsModel::columnCount(const QModelIndex &parent) const
{
//    if (!parent.isValid()) {
//        return nodes.size();
//    }
//    qDebug() << header_data.size();
    return header_data.size();
}

