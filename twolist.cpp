#include "twolist.h"

TwoList::TwoList()
{
    Item *i = new Item;
    i->prev = i->next = i;
    i->data = 0;
    first = curr = last = i;
}

void TwoList::Add(void *d)
{
    Item *i = new Item;
    i->data = d;
    i->next = last->next;
    i->prev = last;
    last->next = i;
    i->next->prev = i;
    curr = last = i;
}

void *TwoList::Value()
{
    return curr->data;
}
