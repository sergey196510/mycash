#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "global.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Globals var;
    Ui::Settings *ui;

private slots:
    void save_vars();
};

#endif // SETTINGS_H
