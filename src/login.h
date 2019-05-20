#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMouseEvent>
#include <QTimer>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QActionGroup>


namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    void init();
    void get_user_info();
    void configWindow();
    void init_sql();
    void set_top_img(bool isSandom, int index_img);//isSandom is true,set img show by sandom
    void set_button();//设置UI上的按钮
    void set_user_img(bool isSandom, int index_img);//设置UI上用户头像

    void create_menuLanguage();         //设置语言菜单

    void setStyle(const QString &style);//设置style


protected:
    void mousePressEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void close();
    
private slots:
    void on_btn_login_clicked();

    void on_btn_regist_clicked();

    void slot_minWindow();

    void slot_closeWindow();

    void slot_setLanguage();        //设置语言

    void slot_actGrp(QAction* act);

    void slot_timer1();

    void slot_timer2();

    void on_cBox_account_activated(int index);

    void on_btn_edit_pwd_clicked();

    void on_cBox_account_currentIndexChanged(int index);

    void refresh();//刷新login界面字符串

private:
    Ui::Login *ui;

    struct UserInfoStu{
        QString userName;
        QString passwd;
        QString email;
    };
    bool m_Drag;
    QPoint m_point;
    QTimer *timer1, *timer2;
    QStringList userPasswd;         //用户密码

    QToolButton *minBtn;
    QToolButton *closeBbtn;
    QToolButton *setBtn;
    QToolButton *status_tBtn;       //在线状态

    QMenu *menu1;                   //语言菜单
    QAction *act_chinese;
    QAction *act_english;

    QMenu *menu2;                   //在线状态
    QAction *act0;                  //在线
    QAction *act1;                  //隐身
    QAction *act2;                  //离线
    QAction *act3;                  //忙碌
    QActionGroup *actGrp;

public:
    UserInfoStu user_info_stu;
    QSqlDatabase db;
};

#endif // LOGIN_H
