#ifndef MYCURRENCY_H
#define MYCURRENCY_H

class MyCurrency
{
private:
    double val;

public:
    MyCurrency() { val = 0; }
    MyCurrency(double i) { val = i; }
    double value() { return val; }
    void setValue(double i) { val = i; }
};

#endif // MYCURRENCY_H
