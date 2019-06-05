//
// Created by peter on 19-5-27.
//

#include "AccountManager.h"
#include <QtCore>
#include <QtNetwork>
#include <QtWidgets>


void AccountManager::registerUser(const QString &email)
{
    //TODO QNetworkReply不会自动释放空间，一定要主动处理内存释放，可以调用QObject::deleteLater令其自动释放空间
    //TODO utf转码
    QUrl url("http://"+server+"/register");
    QUrlQuery query;
    query.addQueryItem("username", username);
    //MD5 加密
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(QByteArray(password.toStdString().c_str()));
    query.addQueryItem("password", md5.result().toHex());
    query.addQueryItem("email", email);
    url.setQuery(query);
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(on_register_finished()));
}

void AccountManager::login()
{
    QUrl url("http://"+server+"/login");
    QUrlQuery query;
    query.addQueryItem("username", username);
    //MD5 加密
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(QByteArray(password.toStdString().c_str()));
    query.addQueryItem("password", md5.result().toHex());
    url.setQuery(query);
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(on_login_finished()));
}

void AccountManager::uploadFile(QString filename, QString content)
{
    QUrl url("http://"+server+"/uploadFile");
//    QUrlQuery query;
//    query.addQueryItem("token", token);
//    query.addQueryItem("filename", filename);
//    query.addQueryItem("content", content);
//    url.setQuery(query);
    QByteArray postData(QString("token=%1&filename=%2&content=%3").arg(token).arg(filename).arg(urlEncode(content)).toUtf8());
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = manager.post(request,postData);
    connect(reply, SIGNAL(finished()), this, SLOT(on_uploadFile_finished()));
}

//void AccountManager::updateFile(QString filename, QString content)
//{
//    QUrl url("http://"+server+"/updateFile");
//    QByteArray postData(QString("{token:\"%1\", filename:\"%2\", content: \"%3\"}").arg(token).arg(filename).arg(content).toUtf8());
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
//    QNetworkReply *reply = manager.post(request,postData);
//    connect(reply, SIGNAL(finished()), this, SLOT(on_updateFile_finished()));
//}

void AccountManager::getContent(QString filename)
{
    QUrl url("http://"+server+"/getContent");
    QUrlQuery query;
    query.addQueryItem("token", token);
    query.addQueryItem("filename", filename);
    url.setQuery(query);
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(on_getContent_finished()));
}

void AccountManager::getFiles()
{
    QUrl url("http://"+server+"/getFiles");
    QUrlQuery query;
    query.addQueryItem("token", token);
    url.setQuery(query);
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(on_getFiles_finished()));
}

void AccountManager::deleteFile(QString filename)
{
    QUrl url("http://"+server+"/deleteFile");
    QUrlQuery query;
    query.addQueryItem("token", token);
    query.addQueryItem("filename", filename);
    url.setQuery(query);
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(on_deleteFile_finished()));
}

void AccountManager::on_register_finished()
{
//    QString buffer = QString::fromStdString(((QNetworkReply *)sender())->readAll().toStdString()).toUtf8();
//    QJsonDocument document = QJsonDocument::fromJson(buffer.toUtf8());
    QByteArray buffer = ((QNetworkReply *)sender())->readAll();
    QJsonDocument document = QJsonDocument::fromJson(buffer);
    if(document.isObject())
    {
        QJsonObject object = document.object();
        if(object.contains("error"))
        {
            QMessageBox::warning((QWidget *)parent(), "error", object.value("error").toString());
        } else if(object.contains("message"))
        {
            QMessageBox::information((QWidget *)parent(), "message", object.value("message").toString());
        } else
        {
            QMessageBox::warning((QWidget *)parent(), "error", "invalid server!");
        }
    }
    ((QNetworkReply *)sender())->deleteLater();
}

void AccountManager::on_login_finished()
{
    QByteArray buffer = ((QNetworkReply *)sender())->readAll();
    QJsonDocument document = QJsonDocument::fromJson(buffer);
    if(document.isObject())
    {
        QJsonObject object = document.object();
        if(object.contains("error"))
        {
            QMessageBox::warning((QWidget *)parent(), "error", object.value("error").toString());
        } else if(object.contains("message")&&object.contains("token"))
        {
            token = object.value("token").toString();
            emit login_success();//成功
//            QMessageBox::information((QWidget *)parent(), "message", object.value("message").toString()+"!");
        } else
        {
            QMessageBox::warning((QWidget *)parent(), "error", "invalid server!");
        }
    }
    ((QNetworkReply *)sender())->deleteLater();
}

void AccountManager::on_uploadFile_finished()
{
    QByteArray buffer = ((QNetworkReply *)sender())->readAll();
    QJsonDocument document = QJsonDocument::fromJson(buffer);
    if(document.isObject())
    {
        QJsonObject object = document.object();
        if(object.contains("error"))
        {
            QMessageBox::warning((QWidget *)parent(), "error", object.value("error").toString());
        } else if(object.contains("message"))
        {
            QMessageBox::information((QWidget *)parent(), "message", object.value("message").toString());
        } else
        {
            QMessageBox::warning((QWidget *)parent(), "error", "invalid server!");
        }
    }
    ((QNetworkReply *)sender())->deleteLater();
}

//void AccountManager::on_updateFile_finished()
//{
//    QByteArray buffer = ((QNetworkReply *)sender())->readAll();
//    QJsonDocument document = QJsonDocument::fromJson(buffer);
//    if(document.isObject())
//    {
//        QJsonObject object = document.object();
//        if(object.contains("error"))
//        {
//            QMessageBox::warning((QWidget *)parent(), "error", object.value("error").toString());
//        } else if(object.contains("message"))
//        {
//            QMessageBox::information((QWidget *)parent(), "message", object.value("message").toString());
//        } else
//        {
//            QMessageBox::warning((QWidget *)parent(), "error", "invalid server!");
//        }
//    }
//    ((QNetworkReply *)sender())->deleteLater();
//}

void AccountManager::on_getContent_finished()
{
    QByteArray buffer = ((QNetworkReply *)sender())->readAll();
    QJsonDocument document = QJsonDocument::fromJson(buffer);
    if(document.isObject())
    {
        QJsonObject object = document.object();
        if(object.contains("error"))
        {
            QMessageBox::warning((QWidget *)parent(), "error", object.value("error").toString());
        } else if(object.contains("message")&&object.contains("content"))
        {
            emit return_content(object.value("content").toString());
//            QMessageBox::information((QWidget *)parent(), "message", object.value("message").toString());
        } else
        {
            QMessageBox::warning((QWidget *)parent(), "error", "invalid server!");
        }
    }
    ((QNetworkReply *)sender())->deleteLater();
}

void AccountManager::on_getFiles_finished()
{
    QByteArray buffer = ((QNetworkReply *)sender())->readAll();
    QJsonDocument document = QJsonDocument::fromJson(buffer);
    if(document.isObject())
    {
        QJsonObject object = document.object();
        if(object.contains("error"))
        {
            QMessageBox::warning((QWidget *)parent(), "error", object.value("error").toString());
        } else if(object.contains("message")&&object.contains("files")&&object.value("files").isArray())
        {
            QJsonArray fileArray = object.value("files").toArray();
            QList<QString> *fileList = new QList<QString>();
            //TODO 记得释放fileList
            foreach(QJsonValue entry, fileArray)
            {
                fileList->append(entry.toObject().value("fileName").toString().split("-").value(1));
            }
            emit return_files_list(fileList);
//            QMessageBox::information((QWidget *)parent(), "message", object.value("message").toString());
        } else
        {
            QMessageBox::warning((QWidget *)parent(), "error", "invalid server!");
        }
    }
    ((QNetworkReply *)sender())->deleteLater();
}

void AccountManager::on_deleteFile_finished()
{
    QByteArray buffer = ((QNetworkReply *)sender())->readAll();
    QJsonDocument document = QJsonDocument::fromJson(buffer);
    if(document.isObject())
    {
        QJsonObject object = document.object();
        if(object.contains("error"))
        {
            QMessageBox::warning((QWidget *)parent(), "error", object.value("error").toString());
        } else if(object.contains("message"))
        {
            QMessageBox::information((QWidget *)parent(), "message", object.value("message").toString());
        } else
        {
            QMessageBox::warning((QWidget *)parent(), "error", "invalid server!");
        }
    }
    ((QNetworkReply *)sender())->deleteLater();
}

AccountManager::AccountManager(QWidget *parent):manager(parent)
{

}

QString AccountManager::urlEncode(QString string)
{
    return string
            .replace("%", "%25")
            .replace(" ","%20")
            .replace("\"", "%22")
            .replace("#", "%23")
            .replace("&", "%26")
            .replace("(", "%28")
            .replace(")", "%29")
            .replace("+", "%2B")
            .replace(",", "%2C")
            .replace("/", "%2F")
            .replace(":", "%3A")
            .replace(";", "%3B")
            .replace("<", "%3C")
            .replace("=", "%3D")
            .replace(">", "%3E")
            .replace("?", "%3F")
            .replace("@", "%40")
            .replace("\\", "%5C")
            .replace("|", "%7C");
            ;
}

const QString &AccountManager::getUsername() const
{
    return username;
}

const QString &AccountManager::getServer() const
{
    return server;
}

AccountManager::~AccountManager()
= default;