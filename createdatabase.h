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
    
private:
    Ui::CreateDatabase *ui;
};

#endif // CREATEDATABASE_H
