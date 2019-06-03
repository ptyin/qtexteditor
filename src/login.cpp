#include "login.h"
#include "ui_login.h"

#include "exam.h"
#include "passwdedit.h"
#include "register.h"
#include "systemtrayicon.h"
#include "editor.h"

#include <QMessageBox>
#include <QUrl>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QtCore/QTime>

float opacity1 = 0.0, opacity2 = 1.0;

Login::Login(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Login)
{
    ui->setupUi(this);
    manager = new AccountManager();

    init();
    connect(this, SIGNAL(close()), this, SLOT(close()));
    connect(manager, SIGNAL(login_success()), this, SLOT(login_success()));
}

Login::Login(QWidget *parent, AccountManager *manager_ptr, Editor* editor) :
        QWidget(parent),
        ui(new Ui::Login)
{
    ui->setupUi(this);
    parentEditor=editor;
    manager = manager_ptr;
    init();
}

Login::~Login()
{
    delete ui;
}

void Login::showAgain()
{
    ui->btn_regist->hide();
    ui->btn_annonymous->hide();
    connect(this, SIGNAL(close()), this, SLOT(close()));
    connect(manager, SIGNAL(login_success()), this, SLOT(login_success_without_entering()));
    show();
}

void Login::init()
{
    setWindowTitle(tr("login"));

//    ui->btn_edit_pwd->setStyleSheet("background-color:transparent;");
//    ui->btn_regist->setStyleSheet("background-color:transparent;");
//    ui->btn_login->setStyleSheet("background-color:transparent;");

    m_Drag = false;

    timer1 = new QTimer;
    timer1->start(5);
    timer2 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(slot_timer1()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(slot_timer2()));

    configWindow();//UI界面设置  去边框，最小化，最大化button
    init_sql();//初始化界面密码，帐号的初值

    //init记住密码
    ui->lineEdit_passwd->setEchoMode(QLineEdit::Password);
}

void Login::get_user_info()
{
    user_info_stu.userName.clear();
    user_info_stu.userName = ui->cBox_account->currentText();
    user_info_stu.passwd.clear();
    user_info_stu.passwd = ui->lineEdit_passwd->text();
    user_info_stu.server.clear();
    user_info_stu.server = ui->cBox_server->currentText();
}

void Login::configWindow()
{

    // 填充背景图片
    QPalette palette;
    palette.setBrush(/*QPalette::Background*/this->backgroundRole(),
                                             QBrush(QPixmap(":/images/background.png")));
    this->setPalette(palette);


    //去掉窗口边框
    setWindowFlags(Qt::FramelessWindowHint);

    //程序init时,设置top ,user img
    set_top_img(true, -1);//设置图片显示为随机显示
    set_user_img(true, -1);//设置user图片为随机显示

    //设置UI的按钮button
    set_button();


    //设置任务栏系统托盘 start
    QStringList strList;
    strList << "Rose" << "Login";//login 为设置trayico的显示提示
    QIcon icon(":/images/ico/Qt-TextEditor.png");
    SystemTrayIcon *trayIcon = new SystemTrayIcon(strList, icon, this);

    connect(trayIcon, SIGNAL(signal_lang_refresh()), this, SLOT(refresh()));//关联语言设置函数,刷新界面
    //设置任务栏系统托盘 end

}


void Login::init_sql()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if (!db.open())
    {
        qDebug() << "database open fail!";
    } else
    {
        qDebug() << "database open success!";
        QSqlQuery query;

        //创建一个名为userInfo的表 顺序为: 用户名 密码 email
        QString sql_create_user_table = "CREATE TABLE if not exists userInfo (name VARCHAR PRIMARY KEY,passwd VARCHAR, email VARCHAR, rememberPasswd boolean);";
        QString sql_create_server_table("CREATE TABLE if not exists serverList (address VARCHAR PRIMARY KEY);");
//        query.prepare(sql_create_user_table);
        if (!query.exec(sql_create_user_table))
        {
            qDebug() << "creater user table error";
        }
        if (!query.exec(sql_create_server_table))
        {
            qDebug() << "creater server table error";
        }
        query.exec("INSERT INTO serverList VALUES ('www.peter-sia.top:2333')");

        query.exec("select * from userInfo");

        while (query.next())
        {
            QString userName = query.value("name").toString();
            ui->cBox_account->addItem(userName);
            QString passwd = query.value("passwd").toString();
            userPasswd.append(passwd);
            is_remembered.append(query.value("rememberPasswd").toBool());
            qDebug() << "userName:::" << userName << "passwd:::" << passwd;
        }

        if (!is_remembered.empty()&&is_remembered.at(0))
        {
            ui->checkBox_rPasswd->setChecked(true);
            ui->lineEdit_passwd->setText(userPasswd.at(0));
        }

        query.exec("select address from serverList");
        while (query.next())
        {
            ui->cBox_server->addItem(query.value("address").toString());
            qDebug() << "address:" << query.value("address");
        }

        ui->cBox_account->setCurrentIndex(0);
        ui->cBox_server->setCurrentIndex(0);
    }
    db.close();
    qDebug() << "database closed!";
}

void Login::set_top_img(bool isSandom, int index_img)
{
    //427 185
    int set_index_img = 1;
    if (isSandom == true)//随机显示topimg
    {

        QTime time_sand;
        time_sand = QTime::currentTime();//获取当前时间
        qsrand(time_sand.msec() + time_sand.second() * 1000);

        //index_img = qrand()%5 ;//在0-4中选出随机数
        set_index_img = qrand() % 5 + 1;//在1-5中选出随机数

    }
    if (isSandom == false) //不随机显示，按index_img显示图片s
    {
        set_index_img = index_img;
    }

    QString top_img_path = ":/images/top_img1.png";
    qDebug() << "             [leo]" << top_img_path;
    QImage top_img;
    top_img_path = ":/images/top_img" + QString::number(set_index_img, 10) + ".png";
    qDebug() << "             [leo]" << top_img_path;
    top_img.load(top_img_path);
    QPixmap top_pic = QPixmap::fromImage(top_img.scaled(ui->label_top_img->width(), ui->label_top_img->height()));
    ui->label_top_img->setPixmap(top_pic);
    qDebug() << "          [leo]top_img width heigh:" << ui->label_top_img->width()
             << " " << ui->label_top_img->height();
}

void Login::set_button()
{
    //构建最小化、关闭按钮,设置按钮，键盘ico
    minBtn = new QToolButton(this);
    closeBbtn = new QToolButton(this);
    setBtn = new QToolButton(this);

//    //获取最小化、关闭按钮图标
//    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
//    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
//    //设置最小化、关闭按钮图标
//    minBtn->setIcon(minPix);
//    closeBbtn->setIcon(closePix);

    //获取界面的宽度
    int width = this->width();
    //设置最小化、关闭按钮在界面的位置
    minBtn->setGeometry(width - 55, 5, 20, 20);
    closeBbtn->setGeometry(width - 25, 5, 20, 20);
    setBtn->setGeometry(width - 80, 7, 15, 15);
    //设置键盘ico坐标
    int x = ui->lineEdit_passwd->x();
    int y = ui->lineEdit_passwd->y();
    int widthkey = ui->lineEdit_passwd->width();

    qDebug() << "[leo]width:" << width;
    qDebug() << "[leo]minBtn" << minBtn->geometry();
    qDebug() << "[leo]closeBbtn" << closeBbtn->geometry();

    //设置鼠标移至按钮上的提示信息
    minBtn->setToolTip(tr("最小化"));
    closeBbtn->setToolTip(tr("关闭"));
    setBtn->setToolTip(tr("设置"));

    //设置最小化、关闭按钮的样式图标
    minBtn->setIcon(QIcon(":/images/ico/mini.png"));
    minBtn->setStyleSheet("background-color:transparent;");
    closeBbtn->setIcon(QIcon(":/images/ico/close.png"));
    closeBbtn->setStyleSheet("background-color:transparent;");
    setBtn->setIcon(QIcon(":/images/ico/setting.png"));
    setBtn->setStyleSheet("background-color:transparent;");

    //关联最小化、关闭按钮的槽函数,键盘exe
    connect(minBtn, SIGNAL(clicked()), this, SLOT(slot_minWindow()));
    connect(closeBbtn, SIGNAL(clicked()), this, SLOT(slot_closeWindow()));
    connect(setBtn, SIGNAL(clicked()), this, SLOT(slot_setLanguage()));

    create_menuLanguage();      //创建语言菜单
}

void Login::set_user_img(bool isSandom, int index_img)
{
    //40,182 85 85
    int set_index_img = 1;
    if (isSandom == true)//随机显示userimg
    {

        QTime time_sand;
        time_sand = QTime::currentTime();//获取当前时间
        qsrand(time_sand.msec() + time_sand.second() * 1000);
        set_index_img = qrand() % 5 + 1;//在1-5中选出随机数

    }
    if (isSandom == false) //不随机显示，按index_img显示图片s
    {
        set_index_img = index_img;
    }

    QString user_img_path = ":/images/ico/user1.png";
    qDebug() << "             [leo]user" << user_img_path;
    QImage user_img;
    user_img_path = ":/images/ico/user" + QString::number(set_index_img, 10) + ".png";
    qDebug() << "             [leo]user" << user_img_path;
    user_img.load(user_img_path);
    QPixmap img_pic = QPixmap::fromImage(user_img.scaled(ui->label_user_img->width(),
                                                         ui->label_user_img->height()));
    ui->label_user_img->setPixmap(img_pic);
    qDebug() << "             [leo]user_img width height:" << ui->label_user_img->width()
             << " " << ui->label_user_img->height();
}

void Login::create_menuLanguage()
{
    //语言
    act_chinese = new QAction(tr("简体中文"), this);
    act_english = new QAction(tr("English"), this);
    menu1 = new QMenu;
    menu1->addAction(act_chinese);
    menu1->addAction(act_english);

    //在线状态
    act0 = new QAction(tr("在线"), this);
    act1 = new QAction(tr("隐身"), this);
    act2 = new QAction(tr("离线"), this);
    act3 = new QAction(tr("忙碌"), this);

    actGrp = new QActionGroup(this);
    actGrp->addAction(act0);
    actGrp->addAction(act1);
    actGrp->addAction(act2);
    actGrp->addAction(act3);
    connect(actGrp, SIGNAL(triggered(QAction * )), this, SLOT(slot_actGrp(QAction * )));

    menu2 = new QMenu;
    menu2->addAction(act0);
    menu2->addAction(act1);
    menu2->addAction(act2);
    menu2->addAction(act3);

    menu1->addAction(act0);
    menu1->addAction(act1);
    menu1->addAction(act2);
    menu1->addAction(act3);
}

void Login::setStyle(const QString &style)
{
    QFile qss(":/images/qss/" + style);//black.qss
    qDebug() << "qss                 :";
    qss.open(QIODevice::ReadOnly | QIODevice::Text);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}


void Login::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_Drag = true;
        m_point = e->globalPos() - this->pos();
        e->accept();
        //  qDebug()<<"leo";
    }
}

void Login::mouseMoveEvent(QMouseEvent *e)
{
    if (m_Drag && (e->buttons() && Qt::LeftButton))
    {
        move(e->globalPos() - m_point);
        e->accept();
        // qDebug()<<"leomove";
    }
}

void Login::mouseReleaseEvent(QMouseEvent *e)
{
    m_Drag = false;
}

void Login::on_btn_login_clicked()
{
    qDebug() << "login:" << user_info_stu.userName << user_info_stu.passwd;
    if (ui->cBox_account->currentText().isEmpty() ||
        ui->lineEdit_passwd->text().isEmpty() ||
        ui->cBox_server->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请同时输入服务器地址和用户名密码！"));
    } else
    {
        //TODO
        get_user_info();
        manager->setUsername(user_info_stu.userName);
        manager->setPassword(user_info_stu.passwd);
        manager->setServer(user_info_stu.server);
        manager->login();
//        manager.setUsername("lalal");
//        manager.setPassword("peter123");
//        manager.setServer("www.peter-sia.top:2333");
//        manager.login();
    }
}

void Login::on_btn_annonymous_clicked()
{
    Editor *e = new Editor(*manager);
    e->show();
    emit close();
}

void Login::login_success()
{
    if (!db.open())
    {
        qDebug() << "database open fail login!";
    } else
    {
        QSqlQuery query;
        qDebug() << "database open success login!";
        QString sql_insert_server_address = "INSERT INTO serverList VALUES (:address);";
        query.prepare(sql_insert_server_address);
        query.bindValue(":address", ui->cBox_server->currentText());
        query.exec();
        QString sql_insert_user = "INSERT INTO userInfo (name, passwd, email) VALUES (:username, :password, :email)";
        query.prepare(sql_insert_user);
        query.bindValue(":username", user_info_stu.userName);
        query.bindValue(":password", user_info_stu.passwd);
        query.bindValue(":email", user_info_stu.email);
        query.exec();
        if (ui->checkBox_rPasswd->isChecked())
        {
            query.prepare("update userInfo set rememberPasswd = 1 where name = :user;");
            query.bindValue(":user", user_info_stu.userName);
            query.exec();
        } else
        {
            query.prepare("update userInfo set rememberPasswd = 0 where name = :user;");
            query.bindValue(":user", user_info_stu.userName);
            query.exec();
        }
        Editor *e = new Editor(*manager);
        e->setUser(user_info_stu.userName);
        e->setServer(ui->cBox_server->currentText());
        e->show();
        emit close();
    }

    db.close();
}

void Login::login_success_without_entering()
{
    if (!db.open())
    {
        qDebug() << "database open fail login!";
    } else
    {
        QSqlQuery query;
        qDebug() << "database open success login!";
        QString sql_insert_server_address = "INSERT INTO serverList VALUES (:address);";
        query.prepare(sql_insert_server_address);
        query.bindValue(":address", ui->cBox_server->currentText());
        query.exec();
        QString sql_insert_user = "INSERT INTO userInfo (name, passwd, email) VALUES (:username, :password, :email)";
        query.prepare(sql_insert_user);
        query.bindValue(":username", user_info_stu.userName);
        query.bindValue(":password", user_info_stu.passwd);
        query.bindValue(":email", user_info_stu.email);
        query.exec();
        if (ui->checkBox_rPasswd->isChecked())
        {
            query.prepare("update userInfo set rememberPasswd = 1 where name = :user;");
            query.bindValue(":user", user_info_stu.userName);
            query.exec();
        } else
        {
            query.prepare("update userInfo set rememberPasswd = 0 where name = :user;");
            query.bindValue(":user", user_info_stu.userName);
            query.exec();
        }
        QMessageBox::information(this, "success", "login successfully!");
        parentEditor->setUser(manager->getUsername());
        parentEditor->setServer(manager->getServer());
        emit close();
    }

    db.close();
}

////注册button
//void Login::on_btn_regist_clicked()
//{
//    Register r(this);
//    this->hide();
//    r.show();
//   //transmitdb(database);
//    r.exec();
//    this->show();
//}

//注册button
void Login::on_btn_regist_clicked()
{
    //TODO
    Register r(*manager, this);
    //transmitdb(database);
    r.exec();    //注册页面r，仅仅获取信息.

//    get_user_info();
//    if (user_info_stu.userName.isEmpty() || user_info_stu.passwd.isEmpty() || user_info_stu.server.isEmpty())
//    {
//        QMessageBox::warning(this, tr("提示"), tr("请同时输入服务器地址和用户名密码！"));
//    } else
//    {
////        bool existFlag = false;       //判断用户是否存在
//        manager.setUsername(user_info_stu.userName);
//        manager.setPassword(user_info_stu.passwd);
//        manager.setServer(user_info_stu.server);
//        manager.registerUser(user_info_stu.email);
//        if (!db.open())
//        {
//            qDebug() << "database open fail regist!";
//        } else
//        {
//            QSqlQuery query;
//            qDebug() << "database open success regist!";
//            query.prepare("select * from userInfo where name = :username;");
//            query.bindValue(":username", user_info_stu.userName);
//            query.exec();
//            qDebug() << query.lastQuery();
//            if (query.first())
//            {
//                QString userName = query.value("name").toString();
//                QString passwd = query.value("passwd").toString();
//                if (userName == user_info_stu.userName)
//                {
//                    existFlag = true;              //用户存在
//                }
//            }
////            while (query.next())
////            {
////                QString userName = query.value(0).toString();
////                QString passwd = query.value(1).toString();
////                qDebug() << "regist userName:::" << userName << "passwd:::" << passwd;
////
////                if (userName == user_info_stu.userName)
////                {
////                    existFlag = true;              //用户存在
////                }
////            }
//
//            if (!existFlag)
//            {
//                if (query.exec(QString("insert into userInfo values ('%1','%2','%3', false)")
//                                       .arg(user_info_stu.userName).arg(user_info_stu.passwd)
//                                       .arg(user_info_stu.email)))
//                {
//                    qDebug() << "ddd:" << user_info_stu.userName << user_info_stu.passwd << user_info_stu.email;
//                    qDebug() << "regist:::" << query.lastQuery();
//                    ui->cBox_account->addItem(user_info_stu.userName);
//                    userPasswd.append(user_info_stu.passwd);
//                    QMessageBox::information(this, tr("提示"), tr("注册成功！"));
//                }
//
//            } else
//            {
//                QMessageBox::warning(this, tr("警告"), tr("用户已存在！"));
//            }
//        }
//        db.close();
//    }
}

////修改密码button
//void Login::on_btn_edit_pwd_clicked()
//{
//    if (ui->cBox_account->currentText().isEmpty() ||
//        ui->lineEdit_passwd->text().isEmpty())
//    {
//        QMessageBox::information(this, tr("提示"), tr("请输入用户名和密码！"));
//    } else
//    {
//        bool is_use_exist_flag = false;       //判断用户是否存在
//        bool is_use_nampwd_check_flag = false;       //判断用户名和密码是否匹配
//        get_user_info();
//
//        if (!db.open())
//        {
//            qDebug() << "database open fail login!";
//        } else
//        {
//            QSqlQuery query;
//            qDebug() << "database open success login!";
//            query.exec("select * from userInfo");
//            while (query.next())
//            {
//                QString userName = query.value(0).toString();
//                QString passwd = query.value(1).toString();
//                qDebug() << "edit userName:::" << userName << "passwd:::" << passwd;
//
//                if (userName == user_info_stu.userName)
//                {
//                    is_use_exist_flag = true;              //用户存在
//                    if (passwd == user_info_stu.passwd)
//                    {
//                        is_use_nampwd_check_flag = true;          //用户名和密码匹配
//                        passwdEdit passwd;
//                        passwd.setLogin(this);
//                        //this->hide();
//                        passwd.exec();
//                    }
//                }
//            }
//
//            if (is_use_exist_flag == false)
//            {
//                QMessageBox::information(this, tr("提示"), tr("用户不存在！"));
//            } else
//            {
//                if (is_use_nampwd_check_flag == 0)
//                {
//                    QMessageBox::warning(this, tr("警告"), tr("用户密码错误！"));
//                }
//            }
//        }
//        db.close();
//    }
//}

//最小化button
void Login::slot_minWindow()
{
    this->showMinimized();
}

void Login::slot_closeWindow()
{
    timer2->start(5);
}

void Login::slot_setLanguage()
{
    menu1->exec(QCursor::pos());
}

void Login::slot_actGrp(QAction *act)
{
    if (act == act0)
    {
        qDebug() << "act0";
    } else if (act == act1)
    {
        qDebug() << "act1";
    } else if (act == act2)
    {
        qDebug() << "act2";
    } else if (act == act3)
    {
        qDebug() << "act3";
    }
}

void Login::slot_timer1()
{
    if (opacity1 >= 1.0)
    {
        timer1->stop();
    } else
    {
        opacity1 += 0.01;
    }
    setWindowOpacity(opacity1);//设置窗口透明度
}

void Login::slot_timer2()
{
    if (opacity2 <= 0.0)
    {
        timer2->stop();

        this->close();
    } else
    {
        opacity2 -= 0.01;
    }
    setWindowOpacity(opacity2);//设置窗口透明度
}

void Login::on_cBox_account_activated(int index)
{
    if (is_remembered.at(index))
    {
        ui->lineEdit_passwd->setText(userPasswd.at(index));
        ui->checkBox_rPasswd->setChecked(true);
    } else
    {
        ui->lineEdit_passwd->setText("");
        ui->checkBox_rPasswd->setChecked(false);
    }
    qDebug() << "change cBox:" << ui->cBox_account->currentText()
             << userPasswd.at(index);
}


//下拉框选里面的项时，会切换top_img的图片和头像图片
void Login::on_cBox_account_currentIndexChanged(int index)
{
    set_top_img(true, index);
    set_user_img(true, index);
}

void Login::refresh()//刷新login页面当前的字符串。其它页面无需刷新，因为打开时候，会自动刷新相关字符串。而主窗口不会。
{
    qDebug() << "xxxxxxxxxxxxxxxxxxx";
    ui->btn_login->setText(tr("login"));
    ui->btn_regist->setText(tr("register"));
    //ui->checkBox_autoLogin->setText(tr("自动登录"));
    ui->label->setText(tr("remember me"));
    ui->label_2->setText(tr("auto login"));

}


