#ifndef ACCOUNTSUMM_H
#define ACCOUNTSUMM_H

#include "account.h"

class account_summ {
    Account acc;
    MyCurrency s;
public:
    account_summ() {
//        acc = 0;
//        s = 0;
    };
    Account account() const { return acc; }
//    double balance() const { return s; }
    MyCurrency balance() const { return s; }
    void set_account(Account val) { acc = val; }
    void set_balance(MyCurrency val) { s = val; }
};

#endif // ACCOUNTSUMM_H
