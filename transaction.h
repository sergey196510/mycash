#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QtSql>

class Transaction
{
private:
    static int level;
public:
    Transaction();
    ~Transaction();
    void begin();
    void commit();
    void rollback();
};

#endif // TRANSACTION_H
