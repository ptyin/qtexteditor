#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QDirModel>
#include <QtGui/QTextDocument>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextList>
#include <QtCore/QPointer>
#include <QtWidgets/QLabel>
#include <QtCore/QStringListModel>
#include "ui_editor.h"
#include "AccountManager.h"

namespace Ui
{
    class Editor;
}

class Editor : public QMainWindow
{
Q_OBJECT

public:
    explicit Editor(AccountManager manager_, QWidget *parent = nullptr);

    virtual ~Editor();

    QString toPlainText() const
    { return ui->textEdit->toPlainText(); }

    QString toHtml() const;

    QTextDocument *document()
    { return ui->textEdit->document(); }

    QTextCursor textCursor() const
    { return ui->textEdit->textCursor(); }

    void setTextCursor(const QTextCursor &cursor)
    { ui->textEdit->setTextCursor(cursor); }

    void setUser(QString username)
    {
        l_username->setText(username);
    };

    void setServer(QString address)
    {
        l_server->setText(address);
    };

public slots:

    void setText(const QString &text);

protected slots:

    void setPlainText(const QString &text)
    { ui->textEdit->setPlainText(text); }

    void setHtml(const QString &text)
    { ui->textEdit->setHtml(text); }

    void textRemoveFormat();

    void textRemoveAllFormat();

    void textBold();

    void textUnderline();

    void textStrikeout();

    void textItalic();

    void textSize(const QString &p);

    void textLink(bool checked);

    void textStyle(int index);

    void textFgColor();

    void textBgColor();

    void listBullet(bool checked);

    void listOrdered(bool checked);

    void slotCurrentCharFormatChanged(const QTextCharFormat &format);

    void slotCursorPositionChanged();

    void slotClipboardDataChanged();

    void increaseIndentation();

    void decreaseIndentation();

    void insertImage();

    void textSource();

    void justifyCenter();

    void justifyLeft();

    void justifyRight();

    void splitPage()
    {
//        ui->textEdit->textCursor().insertHtml("<span style='color:green'><hr></span>");
        ui->textEdit->textCursor().insertHtml("<hr /><hr /><br />");
    }

    void splitPragraph(){ui->textEdit->insertHtml("<hr /><br />");}

protected:
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    void fontChanged(const QFont &f);

    void fgColorChanged(const QColor &c);

    void bgColorChanged(const QColor &c);

    void list(bool checked, QTextListFormat::Style style);

    void indent(int delta);

    void focusInEvent(QFocusEvent *event);

    QStringList m_paragraphItems;
    int m_fontsize_h1;
    int m_fontsize_h2;
    int m_fontsize_h3;
    int m_fontsize_h4;

    enum ParagraphItems
    {
        ParagraphStandard = 0,
        ParagraphSansSerif,
        ParagraphSystem,
        ParagraphHelvetica,
        ParagraphCourier,
        ParagraphMonospace
    };

    QPointer<QTextList> m_lastBlockList;
private slots:

    void on_actionnew_triggered();

    void on_actionopen_triggered();

    void on_actionopen_folder_triggered();

    void on_actionsave_triggered();

    void on_actionlatex_triggered();

    void on_actionpdf_triggered();

    void on_actionlogin_triggered();

    void on_actionregister_triggered();

    void on_actionlogout_triggered();

    void on_n_refresh_clicked();

    void on_n_delete_clicked();

    void on_n_upload_clicked();

    void local_filesystem_view_doublclicked(const QModelIndex &);

    void remote_file_view_doublclicked(const QModelIndex &);

    void showRemoteFiles(QList<QString> *stringList);

    void showFile(QString content){setHtml(content);}

private:
    Ui::Editor *ui;
    QDirModel *dirModel;
    QStringListModel *listModel;
    AccountManager manager;
    QList<QString> remoteFileList;
    QString recentFileName;
    QLabel *l_username;
    QLabel *l_server;
    QTimer *fileListTimer;
    int shotsCount;

    void showLocalFileSystem();

};

#endif // EDITOR_H
