#ifndef CURRENCY_H
#define CURRENCY_H

#include <QObject>
#include <QtSql>

class Currency {
    int id;
    QString name;
    int icod;
    QString scod;
    double kurs;
    int nominal;
public:
    Currency(int i = 0);
    bool load(QString scod);
    bool update();

    int Id()                { return id;      }
    QString Name()          { return name;    }
    int ICod()              { return icod;    }
    QString SCod()          { return scod;    }
    double Kurs()           { return kurs;    }
    int Nominal()           { return nominal; }
    void setId(int i)       { id = i;         }
    void setICod(int i)     { icod = i;       }
    void setSCod(QString i) { scod = i;       }
    void setName(QString i) { name = i;       }
    void setNominal(int i)  { nominal = i;    }
    void setKurs(double i)  { kurs = i;       }
};

#endif // CURRENCY_H
