#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "global.h"

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

private slots:
    void accept_values();
};

#endif // SETTINGS_H
