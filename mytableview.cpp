#include "mytableview.h"

MyTableView::MyTableView(QWidget *parent) :
    QTableView(parent)
{
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);
}

int MyTableView::get_selected_id()
{
    QModelIndexList list;

    list = selectionModel()->selectedIndexes();
    if (list.count() == 0) {
        return 0;
    }

    return list.at(0).data((Qt::DisplayRole)).toInt();
}
