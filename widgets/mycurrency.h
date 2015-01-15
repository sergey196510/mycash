#ifndef MYCURRENCY_H
#define MYCURRENCY_H

#include <QtGlobal>
//#include "../global.h"

class MyCurrency
{
private:
    double val;

public:
//    MyCurrency() { val = 0; }
    MyCurrency(double i = 0) { val = i; }
    double toDouble() const { return val; }
//    QString toString() { return default_locale->toString(val,'f',2); }
    void setValue(double i) { val = i; }
    MyCurrency operator+=(const MyCurrency &v) { return val += v.val; }
    MyCurrency operator*(const MyCurrency &v) { return val*v.val; }
    MyCurrency operator+(const MyCurrency &v) { return val+v.val; }
    double operator-(const MyCurrency &v) { return val-v.val; }
    double operator/(const MyCurrency &v) { return val/v.val; }
    bool operator>(const MyCurrency &v) { return val>v.val; }
    bool operator<(const MyCurrency &v) { return val<v.val; }
    bool operator!=(const MyCurrency &v) { return val!=v.val; }
    bool isNull() { return !val; }
};

#endif // MYCURRENCY_H
