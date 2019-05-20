#include "systemtrayicon.h"

#include <QApplication>
#include <QDebug>

//SystemTrayIcon::SystemTrayIcon(QWidget *parent) : QWidget(parent)
SystemTrayIcon::SystemTrayIcon(QStringList strList, QIcon icon, QWidget *parent) : QWidget(parent)
{
     pWidget = parent;
     m_strList = strList;
     m_icon = icon;

     create_sysTrayMenuAct();//1.创建点击托盘菜单项的act行为
     create_sysTrayMenu();//2.创建托盘菜单
     create_sysTrayIcon();//3.创建托盘,init
     //注意初始化顺序：1.init托盘菜单包含的项 2.init托盘菜单 3.init托盘Button
}

//创建点击托盘菜单项的act行为
void SystemTrayIcon::create_sysTrayMenuAct()
{
//    actFixed = new QAction("锁定位置(&F)",this);
//    actFixed->setCheckable(true);
//    actFixed->setChecked(true);

    act_sys_tray_min = new QAction(tr("最小化(&M)"),this);
    connect(act_sys_tray_min,SIGNAL(triggered()),pWidget,SLOT( hide() ));

    act_sys_tray_normal = new QAction(tr("还 原(&R)"),this);
    connect(act_sys_tray_normal,SIGNAL(triggered()),pWidget,SLOT( showNormal()) );

    act_sys_tray_exit = new QAction(tr("退出(&Q)"),this);
    connect(act_sys_tray_exit,SIGNAL(triggered()),qApp,SLOT( quit()));

    act_sys_tray_lang_ch = new QAction(tr("简体中文(&C)"),this);
//    connect(act_sys_tray_lang_ch,SIGNAL(triggered()),this,SLOT(set_lang()));

    act_sys_tray_lang_en = new QAction(tr("英语(&E)"),this);
    acrLangGrp = new QActionGroup(this);
    acrLangGrp->addAction(act_sys_tray_lang_ch);
    acrLangGrp->addAction(act_sys_tray_lang_en);
    connect(acrLangGrp,SIGNAL(triggered(QAction*)),this,SLOT( set_lang(QAction*)));


}


//创建托盘菜单
void SystemTrayIcon::create_sysTrayMenu()
{
    mSysTrayMenu = new QMenu(this);
    //mSysTrayMenu = new QMenu((QWidget*)QApplication::desktop());

    mSysTrayMenuLangSetting = new QMenu(tr("语言设置"), this);
//    QList<QAction *> act_list;
//    act_list << act_sys_tray_lang_ch << act_sys_tray_lang_en;
//    mSysTrayMenuLangSetting->addActions(act_list);

    mSysTrayMenuLangSetting->addAction(act_sys_tray_lang_ch);
    mSysTrayMenuLangSetting->addAction(act_sys_tray_lang_en);

    mSysTrayMenu->addMenu(mSysTrayMenuLangSetting);

    //新增菜单项---显示主界面
    mSysTrayMenu->addAction(act_sys_tray_normal);
    //新增菜单项---最小化
    mSysTrayMenu->addAction(act_sys_tray_min);

    //增加分隔符
    mSysTrayMenu->addSeparator();

    //新增菜单项---退出程序
    mSysTrayMenu->addAction(act_sys_tray_exit);



//    mSysTrayMenuLangSetting

    //把QMenu,系统托盘菜单赋给QSystemTrayIcon对象
//    mSysTrayIcon->setContextMenu(mSysTrayMenu);     //new之后才能使用mSysTrayIcon
}


//创建系统托盘按钮
void SystemTrayIcon::create_sysTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())      //判断系统是否支持系统托盘图标
     {
         return;
     }

    mSysTrayIcon = new QSystemTrayIcon(pWidget);
    mSysTrayIcon->setIcon(m_icon);   //设置图标图片
    mSysTrayIcon->setToolTip(m_strList.at(1));    //托盘时，鼠标放上去的提示信息
    mSysTrayIcon->showMessage(m_strList.at(0), m_strList.at(1), QSystemTrayIcon::Information, 10000);

    //    mSysTrayIcon = new QSystemTrayIcon(this);//新建QSystemTrayIcon对象
    //    mSysTrayIcon->setIcon(QIcon(":/images/ico/mini.png")); //设置托盘的icon
    //    mSysTrayIcon->setStyleSheet("background-color:transparent;");
    //    mSysTrayIcon->setToolTip(QObject::tr("login_demo"));//当鼠标移动到托盘上的图标时，会显示此处设置的内容

    mSysTrayIcon->setContextMenu(mSysTrayMenu);//设置托盘上下文菜单

    mSysTrayIcon->show();//在系统托盘显示此对象

    //给QSystemTrayIcon对象mSysTrayIcon添加信号为activated(QSystemTrayIcon::ActivationReason)的槽函数
    connect(mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                  this, SLOT(slot_sys_tray_iconActivated(QSystemTrayIcon::ActivationReason)));

}





//系统托盘触发槽函数
/*********************
enum QSystemTrayIcon::ActivationReason 表述托盘上图标的触发缘由
    常量                        值          描述
QSystemTrayIcon::Unknown        0       未知原因
QSystemTrayIcon::Context        1       请求系统托盘的上下文菜单
QSystemTrayIcon::DoubleClick	2       鼠标双击
QSystemTrayIcon::Trigger        3       鼠标单击
QSystemTrayIcon::MiddleClick	4       鼠标中间按键
**********************/
void SystemTrayIcon::slot_sys_tray_iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Unknown:      //未知原因
            break;
        case QSystemTrayIcon::Context:      //请求系统托盘的上下文菜单
            break;
        case QSystemTrayIcon::DoubleClick:  //鼠标双击
            pWidget->setWindowState(Qt::WindowActive);
            pWidget->show();
            break;
        case QSystemTrayIcon::Trigger:      //鼠标单击

            break;
        case QSystemTrayIcon::MiddleClick:  //鼠标中间按键
            break;
        default:
            break;
    }

}

void SystemTrayIcon::set_lang(QAction *act)
{
    qDebug() << act->text();
    if(act == act_sys_tray_lang_ch)
    {
        qDebug() << "china ";
        translator.load(":/cn.qm");
        qApp->installTranslator(&translator);

    }
    if(act == act_sys_tray_lang_en)
    {
        qDebug() << "english ";
        translator.load(":/en.qm");
        qApp->installTranslator( &translator );
    }
    this->refresh();//刷新托盘相关文字
    emit signal_lang_refresh();//发送刷新页面文字的信号
}


void SystemTrayIcon::refresh()
{
    this->act_sys_tray_exit->setText(tr("退出(&Q)"));
    this->act_sys_tray_min->setText(tr("最小化(&M)"));
    this->act_sys_tray_lang_ch->setText(tr("简体中文(&C)"));
    this->act_sys_tray_lang_en->setText(tr("英语(&E)"));
    this->act_sys_tray_normal->setText(tr("还 原(&R)"));
    this->mSysTrayMenuLangSetting->setWindowTitle(tr("语言设置"));
    qDebug() << "lllllllllllll" << this->mSysTrayMenuLangSetting->windowTitle();

}
