#ifndef EDITAGENT_H
#define EDITAGENT_H

#include <QDialog>
#include "global.h"

namespace Ui {
class EditAgent;
}

class EditAgent : public QDialog
{
    Q_OBJECT

public:
    explicit EditAgent(QString title, QWidget *parent = 0);
    ~EditAgent();
    Agent data();

private:
    Ui::EditAgent *ui;

private slots:
    void checkOk();
};

#endif // EDITAGENT_H
