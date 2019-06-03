#include <utility>

#include <utility>

#include <utility>

//
// Created by peter on 19-5-27.
//

#ifndef QTLATEX_ACCOUNTACCESS_H
#define QTLATEX_ACCOUNTACCESS_H


#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>

class AccountManager: public QObject
{
    Q_OBJECT
public:
    explicit AccountManager(QWidget *parent= nullptr);
    AccountManager &operator=(const AccountManager &access){return *(new AccountManager(access));}
    AccountManager(const AccountManager &access):QObject(access.parent()),manager(access.parent()){username=access.username;password=access.password;server=access.server;token=access.token;}
    AccountManager(const QString &username_, const QString &password_, QObject *parent= nullptr):manager(parent){username=username_;password=password_;}
    ~AccountManager() override;
    void setUsername(const QString &username_){username=username_;}
    void setPassword(const QString &password_){password=password_;}
    void setServer(const QString &server_){server=server_;}
    void registerUser(const QString &email);
    void login();
    void uploadFile(QString filename, QString content);
//    void updateFile(QString filename, QString content);
    void getContent(QString filename);
    void getFiles();
    void deleteFile(QString filename);

    const QString &getUsername() const;

    const QString &getServer() const;

signals:
    void login_success();
    void return_content(QString content);
    void return_files_list(QList<QString> *fileList);

private slots:
    void on_register_finished();
    void on_login_finished();
    void on_uploadFile_finished();
//    void on_updateFile_finished();
    void on_getContent_finished();
    void on_getFiles_finished();
    void on_deleteFile_finished();

private:
    QNetworkAccessManager manager;
    QString token;
    QString username;
    QString password;
    QString server;

};


#endif //QTLATEX_ACCOUNTACCESS_H
