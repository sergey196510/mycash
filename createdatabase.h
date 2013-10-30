#ifndef CREATEDATABASE_H
#define CREATEDATABASE_H

#include <QDialog>

namespace Ui {
class CreateDatabase;
}

class CreateDatabase : public QDialog
{
    Q_OBJECT
    
public:
    explicit CreateDatabase(QWidget *parent = 0);
    ~CreateDatabase();
    QString name();
    
private:
    Ui::CreateDatabase *ui;

private slots:
    void check_ok(QString);
};

#endif // CREATEDATABASE_H
