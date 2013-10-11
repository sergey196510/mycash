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
    QString dbname;
    Ui::MyCash *ui;
    void readsettings();
    void writesettings();
    void setconnects();
    void opendb(QString);

private slots:
    void create();
    void open();
    void closeDatabase();
    void settings();
    void aboutProgram();
    void mark_Object();
    void list_accounts();
    void list_currency();
    void list_operations();
    void list_agents();
    void report1();

};

#endif // MYCASH_H
