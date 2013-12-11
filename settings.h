#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "global.h"
#include "listaccountsmodel.h"
#include "accountoper.h"
#include "database.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;
    Globals var;
    bool isdown;
    Database *db;
//    SelectAccount *selacc;

private slots:
    void accept_values();
    void check_down();
//    void correct_account();
//    void initial_account();
};

#endif // SETTINGS_H
