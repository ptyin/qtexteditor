#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QDirModel>
#include <QtGui/QTextDocument>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextList>
#include <QtCore/QPointer>
#include <QtWidgets/QLabel>
#include "ui_editor.h"

namespace Ui
{
    class Editor;
}

class Editor : public QMainWindow
{
Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);

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
    { ui->statusbar->addPermanentWidget(new QLabel("username:" + username)); };

    void setServer(QString address)
    { ui->statusbar->addPermanentWidget(new QLabel("server:" + address)); };

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

    void on_actionopen_triggered();

    void on_actionopen_folder_triggered();

    void on_actionsave_triggered();

    void on_actionexport_triggered();

private:
    Ui::Editor *ui;
    QDirModel *dirModel;

    void showLocalFileSystem();
};

#endif // EDITOR_H
