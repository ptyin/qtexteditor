#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "login.h"

namespace Ui
{
    class Register;
}

class Register : public QDialog
{
Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);

    ~Register();

    void setParent(Login *dialog);

    QSqlDatabase database;//database为注册的数据库名称
    bool tableFlag;
    int sql_max_id;

private slots:

    void on_registerCheckButton_clicked();
//     void receivedb(QSqlDatabase db);

private:
    Ui::Register *ui;

    Login *pWidget;         //父对象

};

#endif // REGISTER_H
