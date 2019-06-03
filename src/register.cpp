#include "register.h"
#include "ui_register.h"
#include <QDebug>

Register::Register(const AccountManager& manager_, QWidget *parent= nullptr) :
        manager(manager_),
        QDialog(parent),
        ui(new Ui::Register)
{
    ui->setupUi(this);

    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditPasswordCheck->setEchoMode(QLineEdit::Password);
    ui->lineEditServer->setText("www.peter-sia.top:2333");
    setWindowTitle(tr("register"));

    //connect((Login *)parent,SIGNAL(transmitdb(QSqlDatabase)),this,SLOT(receivedb(QSqlDatabase)));

}

Register::~Register()
{
    delete ui;
}

//注册函数.仅仅获取相关值，不做检测和数据库操作
void Register::on_registerCheckButton_clicked()
{
    QString userName = ui->lineEditName->text();
    QString passwd = ui->lineEditPassword->text();
    QString email = ui->lineEditEmail->text();
    QString server = ui->lineEditServer->text();
//        bool existFlag = false;       //判断用户是否存在

//    QSqlDatabase db = pWidget->getDb();
//    if(!db.isOpen())
//        db.open();
//    QSqlQuery query(db);
//    query.prepare("insert into userInfo values (user ,passwd, email, false)");
//    query.bindValue(":user", ui->lineEditName->text());
//    query.bindValue(":passwd", ui->lineEditPassword->text());
//    query.bindValue(":email", ui->lineEditEmail->text());
        if (userName.isEmpty() || passwd.isEmpty() ||
            server.isEmpty())
        {
            QMessageBox::warning(this, tr("note"), tr("Username, Password And Server Are Needed！"));
        } else if (ui->lineEditPassword->text() != ui->lineEditPasswordCheck->text())
        {
            QMessageBox::warning(this, "error", "passwords you type are not the same!");
        } else
        {
            manager.setUsername(userName);
            manager.setPassword(passwd);
            manager.setServer(server);
            manager.registerUser(email);
            this->close();
        }
}