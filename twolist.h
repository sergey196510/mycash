#ifndef TWOLIST_H
#define TWOLIST_H


class TwoList
{
    struct Item {
        Item *prev, *next;
        void *data;
    };
    Item *first, *curr, *last;
public:
    TwoList();
    void Add(void *d);
    void First() { curr = first; }
    bool Prev() {
        if (curr != first) {
            curr = curr->prev;
            return true;
        }
        return false;
    }
    bool Next() {
        if (curr != last) {
            curr = curr->next;
            return true;
        }
        return false;
    }
    void Last() { curr = last; }
    void *Value();
};

#endif // TWOLIST_H
