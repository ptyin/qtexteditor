#ifndef PASSWDEDIT_H
#define PASSWDEDIT_H

#include <QDialog>

#include "login.h"

namespace Ui {
class passwdEdit;
}

class passwdEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit passwdEdit(QWidget *parent = 0);
    ~passwdEdit();

    void init();
    void setLogin(Login *m);

signals:
    void passwdEditOk();
    
private slots:
    void on_btn_ok_clicked();

private:
    Ui::passwdEdit *ui;

    Login *login;
//    User m_user;
//    QList<User> m_userList;
};

#endif // PASSWDEDIT_H
