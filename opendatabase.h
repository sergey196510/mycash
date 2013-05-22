#ifndef OPENDATABASE_H
#define OPENDATABASE_H

#include <QtGui>
#include <QDialog>

namespace Ui {
class OpenDatabase;
}

class OpenDatabase : public QDialog
{
    Q_OBJECT
    
public:
    explicit OpenDatabase(QWidget *parent = 0);
    ~OpenDatabase();
    QString filename();
    
private:
    Ui::OpenDatabase *ui;

private slots:
    void select_filename();
    void nameChanged(QString);
};

#endif // OPENDATABASE_H
