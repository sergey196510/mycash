#include "mytreeview.h"

myTreeView::myTreeView(QWidget *parent) :
    QTreeView(parent)
{
//    expandAll();
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}
