#include "accountsview.h"

accountsView::accountsView(QWidget *parent) :
    QTreeView(parent)
{
    model = new ListAccountsModel;
    index_list = model->fill_model();
    setModel(model);
    expandAll();
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    for (int i = 1; i < 6; i++)
        hideColumn(i);
}

int accountsView::value()
{
    QModelIndexList list;

    list = selectionModel()->selectedIndexes();
    if (list.count() == 0) {
//        QMessageBox::critical(this, "Operation cancellation", "Nothing selected");
        return 0;
    }

     return list.at(5).data(Qt::DisplayRole).toInt();
}

void accountsView::setValue(int idx)
{
/*
    QItemSelectionModel *sel = selectionModel();

    index = model->fill_model(idx);
//    qDebug() << index;
    expandAll();
    for (int i = 1; i < 6; i++)
        hideColumn(i);
*/
    QItemSelectionModel *sel = selectionModel();
    QModelIndex index = index_list[idx];
    if (index.isValid()) {
        sel->select(index, QItemSelectionModel::Select);
        setCurrentIndex(index);
    }
}
