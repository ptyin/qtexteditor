#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QTranslator>
#include <QActionGroup>

class SystemTrayIcon : public QWidget
{
    Q_OBJECT
public:
    //explicit SystemTrayIcon(QWidget *parent = 0);
    explicit SystemTrayIcon(QStringList strList, QIcon icon, QWidget *parent = 0);

    void create_sysTrayMenu();          //创建托盘菜单
    void create_sysTrayIcon();          //创建托盘菜单的图标
    void create_sysTrayMenuAct();       //创建点击托盘菜单项的act行为
    void create_sysTrayMenuLangeSettingAct();       //创建点击托盘菜单项的act行为


signals:
    void signal_lang_refresh();

public slots:

    void slot_sys_tray_iconActivated(QSystemTrayIcon::ActivationReason reason);//系统托盘触发槽函数
    void set_lang(QAction *act);


public:
    QAction *actFixed;                  //固定位置
    QActionGroup *acrLangGrp;
    void refresh();

private:

    QWidget *pWidget;                   //父对象

    QSystemTrayIcon *mSysTrayIcon;        //系统托盘
    QMenu *mSysTrayMenu;                  //系统托盘显示菜单
    QMenu *mSysTrayMenuLangSetting;                  //系统托盘语言设置
    QAction *act_sys_tray_min;                    //最小化
    QAction *act_sys_tray_normal;                 //正常
    QAction *act_sys_tray_exit;                   //退出
    QAction *act_sys_tray_lang_ch;                   //china
    QAction *act_sys_tray_lang_en;                   //english
    QStringList m_strList;              //托盘信息
    QIcon m_icon;                       //托盘图标
    QTranslator translator;


};

#endif // SYSTEMTRAYICON_H
