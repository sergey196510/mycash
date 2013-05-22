#ifndef MYCASH_H
#define MYCASH_H

#include <QMainWindow>

namespace Ui {
class MyCash;
}

class MyCash : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MyCash(QWidget *parent = 0);
    ~MyCash();

signals:
    void call_mark();

private:
    bool opened;
    Ui::MyCash *ui;
    void readsettings();
    void writesettings();
    void setconnects();

private slots:
    void open();
    void settings();
    void aboutProgram();
    void mark_Object();

};

#endif // MYCASH_H
