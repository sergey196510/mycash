#include "mytableview.h"

MyTableView::MyTableView(QWidget *parent) :
    QTableView(parent)
{
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

#ifdef HAVE_QT5
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif

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
