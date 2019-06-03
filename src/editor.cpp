#include <QtWidgets/QDirModel>
#include <QtWidgets/QFileSystemModel>
#include <QtWidgets/QFileDialog>
#include <QDebug>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QColorDialog>
#include <QClipboard>
#include <QMimeData>
#include <QtCore/QSettings>
#include <QImageReader>
#include <QtCore/QProcess>
#include <QtPrintSupport/QPrinter>
#include <QList>
#include "editor.h"
#include "ui_editor.h"
#include "AccountManager.h"

Editor::Editor(AccountManager manager_, QWidget *parent):
        manager(manager_),
        QMainWindow(parent),
        ui(new Ui::Editor)
{
    manager = manager_;
    ui->setupUi(this);
    connect(&manager, SIGNAL(return_files_list(QList<QString> *)), this, SLOT(showRemoteFiles(QList<QString> *)));
    connect(&manager, SIGNAL(return_content(QString)), this, SLOT(showFile(QString)));
    connect(ui->remote_file_view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(remote_file_view_doublclicked(const QModelIndex &)));
    connect(ui->local_filesystem_view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(local_filesystem_view_doublclicked(const QModelIndex &)));

//    connect(ui->actionopen, SIGNAL(triggered()), this, SLOT(on_actionopen_triggered()));
//    connect(ui->actionopen_folder, SIGNAL(triggered()), this, SLOT(on_actionopen_folder_triggered()));
//    connect(ui->actionsave, SIGNAL(triggered()), this, SLOT(on_actionsave_triggered()));
//    connect(ui->actionexport, SIGNAL(triggered()), this, SLOT(on_actionexport_triggered()));
    ui->actionsave->setShortcut(QKeySequence("CTRL+S"));

    showLocalFileSystem();
    m_lastBlockList = nullptr;
    ui->textEdit->setTabStopWidth(40);

    connect(ui->textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(slotCurrentCharFormatChanged(QTextCharFormat)));
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(slotCursorPositionChanged()));

    m_fontsize_h1 = 18;
    m_fontsize_h2 = 16;
    m_fontsize_h3 = 14;
    m_fontsize_h4 = 12;

    fontChanged(ui->textEdit->font());
    bgColorChanged(ui->textEdit->textColor());

    // paragraph formatting

    m_paragraphItems << tr("Standard")
                     << tr("SansSerif")
                     << tr("System")
                     << tr("Helvetica")
                     << tr("Courier")
                     << tr("Monospace");
    ui->f_paragraph->addItems(m_paragraphItems);

    connect(ui->f_paragraph, SIGNAL(activated(int)),
            this, SLOT(textStyle(int)));

    // undo & redo

    ui->f_undo->setShortcut(QKeySequence::Undo);
    ui->f_redo->setShortcut(QKeySequence::Redo);

    connect(ui->textEdit->document(), SIGNAL(undoAvailable(bool)),
            ui->f_undo, SLOT(setEnabled(bool)));
    connect(ui->textEdit->document(), SIGNAL(redoAvailable(bool)),
            ui->f_redo, SLOT(setEnabled(bool)));

    ui->f_undo->setEnabled(ui->textEdit->document()->isUndoAvailable());
    ui->f_redo->setEnabled(ui->textEdit->document()->isRedoAvailable());

    connect(ui->f_undo, SIGNAL(clicked()), ui->textEdit, SLOT(undo()));
    connect(ui->f_redo, SIGNAL(clicked()), ui->textEdit, SLOT(redo()));

    // cut, copy & paste

    ui->f_cut->setShortcut(QKeySequence::Cut);
    ui->f_copy->setShortcut(QKeySequence::Copy);
    ui->f_paste->setShortcut(QKeySequence::Paste);

    ui->f_cut->setEnabled(false);
    ui->f_copy->setEnabled(false);

    connect(ui->f_cut, SIGNAL(clicked()), ui->textEdit, SLOT(cut()));
    connect(ui->f_copy, SIGNAL(clicked()), ui->textEdit, SLOT(copy()));
    connect(ui->f_paste, SIGNAL(clicked()), ui->textEdit, SLOT(paste()));

    connect(ui->f_justify_center, SIGNAL(clicked()), this, SLOT(justifyCenter()));
    connect(ui->f_justify_left, SIGNAL(clicked()), this, SLOT(justifyLeft()));
    connect(ui->f_justify_right, SIGNAL(clicked()), this, SLOT(justifyRight()));

    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->f_cut, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->f_copy, SLOT(setEnabled(bool)));

#ifndef QT_NO_CLIPBOARD
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(slotClipboardDataChanged()));
#endif

    // link

    ui->f_link->setShortcut(Qt::CTRL + Qt::Key_L);

    connect(ui->f_link, SIGNAL(clicked(bool)), this, SLOT(textLink(bool)));
    connect(ui->f_split_page, SIGNAL(clicked()), this, SLOT(splitPage()));
    connect(ui->f_split_paragraph, SIGNAL(clicked()), this, SLOT(splitPragraph()));

    // bold, italic & underline

    ui->f_bold->setShortcut(Qt::CTRL + Qt::Key_B);
    ui->f_italic->setShortcut(Qt::CTRL + Qt::Key_I);
    ui->f_underline->setShortcut(Qt::CTRL + Qt::Key_U);

    connect(ui->f_bold, SIGNAL(clicked()), this, SLOT(textBold()));
    connect(ui->f_italic, SIGNAL(clicked()), this, SLOT(textItalic()));
    connect(ui->f_underline, SIGNAL(clicked()), this, SLOT(textUnderline()));
    connect(ui->f_strikeout, SIGNAL(clicked()), this, SLOT(textStrikeout()));

    QAction *removeFormat = new QAction(tr("Remove character formatting"), this);
    removeFormat->setShortcut(QKeySequence("CTRL+M"));
    connect(removeFormat, SIGNAL(triggered()), this, SLOT(textRemoveFormat()));
    ui->textEdit->addAction(removeFormat);

    QAction *removeAllFormat = new QAction(tr("Remove all formatting"), this);
    connect(removeAllFormat, SIGNAL(triggered()), this, SLOT(textRemoveAllFormat()));
    ui->textEdit->addAction(removeAllFormat);

    QAction *textsource = new QAction(tr("Edit document source"), this);
    textsource->setShortcut(QKeySequence("CTRL+O"));
    connect(textsource, SIGNAL(triggered()), this, SLOT(textSource()));
    ui->textEdit->addAction(textsource);

    QMenu *menu = new QMenu(this);
    menu->addAction(removeAllFormat);
    menu->addAction(removeFormat);
    menu->addAction(textsource);
    ui->f_menu->setMenu(menu);
    ui->f_menu->setPopupMode(QToolButton::InstantPopup);

    // lists

    ui->f_list_bullet->setShortcut(Qt::CTRL + Qt::Key_Minus);
    ui->f_list_ordered->setShortcut(Qt::CTRL + Qt::Key_Equal);

    connect(ui->f_list_bullet, SIGNAL(clicked(bool)), this, SLOT(listBullet(bool)));
    connect(ui->f_list_ordered, SIGNAL(clicked(bool)), this, SLOT(listOrdered(bool)));

    // indentation

    ui->f_indent_dec->setShortcut(QKeySequence("Shift+Tab"));
    ui->f_indent_inc->setShortcut(QKeySequence("Ctrl+Tab"));
    connect(ui->textEdit, SIGNAL(tab()), this, SLOT(increaseIndentation()));

    connect(ui->f_indent_inc, SIGNAL(clicked()), this, SLOT(increaseIndentation()));
    connect(ui->f_indent_dec, SIGNAL(clicked()), this, SLOT(decreaseIndentation()));

    // font size

    QFontDatabase db;
            foreach(int size, db.standardSizes())ui->f_fontsize->addItem(QString::number(size));

    connect(ui->f_fontsize, SIGNAL(activated(QString)),
            this, SLOT(textSize(QString)));
    ui->f_fontsize->setCurrentIndex(ui->f_fontsize->findText(QString::number(QApplication::font()
                                                                                     .pointSize())));

    // text foreground color

    QPixmap pix(16, 16);
    pix.fill(QApplication::palette().foreground().color());
    ui->f_fgcolor->setIcon(pix);

    connect(ui->f_fgcolor, SIGNAL(clicked()), this, SLOT(textFgColor()));

    // text background color

    pix.fill(QApplication::palette().background().color());
    ui->f_bgcolor->setIcon(pix);

    connect(ui->f_bgcolor, SIGNAL(clicked()), this, SLOT(textBgColor()));

    // images
    connect(ui->f_image, SIGNAL(clicked()), this, SLOT(insertImage()));
}

void Editor::showLocalFileSystem()
{
    dirModel = new QDirModel(this);
    dirModel->setReadOnly(false);
    dirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

    ui->local_filesystem_view->setModel(dirModel);
    ui->local_filesystem_view->header()->setStretchLastSection(false);
    ui->local_filesystem_view->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui->local_filesystem_view->header()->setSortIndicatorShown(true);
    ui->local_filesystem_view->header()->setSectionsClickable(true);
    ui->local_filesystem_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

//    QModelIndex index = dirModel->index(QDir::currentPath());
//    ui->local_filesystem_view->expand(index);
//    ui->local_filesystem_view->scrollTo(index);
//    ui->local_filesystem_view->resizeColumnToContents(index.column());
}

void Editor::showRemoteFiles(QList<QString> *stringList)
{
    remoteFileList = *stringList;
    listModel = new QStringListModel(this);
    ui->remote_file_view->setModel(listModel);
    listModel->setStringList(*stringList);
    delete stringList;
}

void Editor::textSource()
{
    QDialog *dialog = new QDialog(this);
    QPlainTextEdit *pte = new QPlainTextEdit(dialog);
    pte->setPlainText(ui->textEdit->toHtml());
    QGridLayout *gl = new QGridLayout(dialog);
    gl->addWidget(pte, 0, 0, 1, 1);
    dialog->setWindowTitle(tr("Document source"));
    dialog->setMinimumWidth(400);
    dialog->setMinimumHeight(600);
    dialog->exec();

    ui->textEdit->setHtml(pte->toPlainText());

    delete dialog;
}

void Editor::textRemoveFormat()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Normal);
    fmt.setFontUnderline(false);
    fmt.setFontStrikeOut(false);
    fmt.setFontItalic(false);
    fmt.setFontPointSize(9);
//  fmt.setFontFamily     ("Helvetica");
//  fmt.setFontStyleHint  (QFont::SansSerif);
//  fmt.setFontFixedPitch (true);

    ui->f_bold->setChecked(false);
    ui->f_underline->setChecked(false);
    ui->f_italic->setChecked(false);
    ui->f_strikeout->setChecked(false);
    ui->f_fontsize->setCurrentIndex(ui->f_fontsize->findText("9"));

//  QTextBlockFormat bfmt = cursor.blockFormat();
//  bfmt->setIndent(0);

    fmt.clearBackground();

    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textRemoveAllFormat()
{
    ui->f_bold->setChecked(false);
    ui->f_underline->setChecked(false);
    ui->f_italic->setChecked(false);
    ui->f_strikeout->setChecked(false);
    ui->f_fontsize->setCurrentIndex(ui->f_fontsize->findText("9"));
    QString text = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(text);
}

void Editor::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(ui->f_bold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::focusInEvent(QFocusEvent *)
{
    ui->textEdit->setFocus(Qt::TabFocusReason);
}


void Editor::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(ui->f_underline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(ui->f_italic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textStrikeout()
{
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(ui->f_strikeout->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void Editor::textLink(bool checked)
{
    bool unlink = false;
    QTextCharFormat fmt;
    if (checked)
    {
        QString url = ui->textEdit->currentCharFormat().anchorHref();
        bool ok;
        QString newUrl = QInputDialog::getText(this, tr("Create a link"),
                                               tr("Link URL:"), QLineEdit::Normal,
                                               url,
                                               &ok);
        if (ok)
        {
            fmt.setAnchor(true);
            fmt.setAnchorHref(newUrl);
            fmt.setForeground(QApplication::palette().color(QPalette::Link));
            fmt.setFontUnderline(true);
        } else
        {
            unlink = true;
        }
    } else
    {
        unlink = true;
    }
    if (unlink)
    {
        fmt.setAnchor(false);
        fmt.setForeground(QApplication::palette().color(QPalette::Text));
        fmt.setFontUnderline(false);
    }
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textStyle(int index)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.beginEditBlock();

    // standard
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::BlockUnderCursor);
    }
    QTextCharFormat fmt;
    cursor.setCharFormat(fmt);
    ui->textEdit->setCurrentCharFormat(fmt);

    if (index == ParagraphSansSerif)
    {
        fmt = cursor.charFormat();
        fmt.setFontFamily("SansSerif");
        fmt.setFontStyleHint(QFont::SansSerif);
        fmt.setFontFixedPitch(true);
    }
    if (index == ParagraphSystem)
    {
        fmt = cursor.charFormat();
        fmt.setFontFamily("System");
        fmt.setFontStyleHint(QFont::System);
        fmt.setFontFixedPitch(true);
    }
    if (index == ParagraphHelvetica)
    {
        fmt = cursor.charFormat();
        fmt.setFontFamily("Helvetica");
        fmt.setFontStyleHint(QFont::Helvetica);
        fmt.setFontFixedPitch(true);
    }
    if (index == ParagraphCourier)
    {
        fmt = cursor.charFormat();
        fmt.setFontFamily("Courier");
        fmt.setFontStyleHint(QFont::Courier);
        fmt.setFontFixedPitch(true);
    }
//    if (index == ParagraphSystem || index == ParagraphCourier)
//    {
//        fmt.setFontItalic(true);
//    }
    if (index == ParagraphMonospace)
    {
        fmt = cursor.charFormat();
        fmt.setFontFamily("Monospace");
        fmt.setFontStyleHint(QFont::Monospace);
        fmt.setFontFixedPitch(true);
    }
    cursor.setCharFormat(fmt);
    ui->textEdit->setCurrentCharFormat(fmt);

    cursor.endEditBlock();
}

void Editor::textFgColor()
{
    QColor col = QColorDialog::getColor(ui->textEdit->textColor(), this);
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    QTextCharFormat fmt = cursor.charFormat();
    if (col.isValid())
    {
        fmt.setForeground(col);
    } else
    {
        fmt.clearForeground();
    }
    cursor.setCharFormat(fmt);
    ui->textEdit->setCurrentCharFormat(fmt);
    fgColorChanged(col);
}

void Editor::textBgColor()
{
    QColor col = QColorDialog::getColor(ui->textEdit->textBackgroundColor(), this);
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    QTextCharFormat fmt = cursor.charFormat();
    if (col.isValid())
    {
        fmt.setBackground(col);
    } else
    {
        fmt.clearBackground();
    }
    cursor.setCharFormat(fmt);
    ui->textEdit->setCurrentCharFormat(fmt);
    bgColorChanged(col);
}

void Editor::justifyCenter()
{
    ui->textEdit->setAlignment(Qt::AlignCenter);
}
void Editor::justifyLeft()
{
    ui->textEdit->setAlignment(Qt::AlignLeft);
}
void Editor::justifyRight()
{
    ui->textEdit->setAlignment(Qt::AlignRight);
}

void Editor::listBullet(bool checked)
{
    if (checked)
    {
        ui->f_list_ordered->setChecked(false);
    }
    list(checked, QTextListFormat::ListDisc);
}

void Editor::listOrdered(bool checked)
{
    if (checked)
    {
        ui->f_list_bullet->setChecked(false);
    }
    list(checked, QTextListFormat::ListDecimal);
}

void Editor::list(bool checked, QTextListFormat::Style style)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.beginEditBlock();
    if (!checked)
    {
        QTextBlockFormat obfmt = cursor.blockFormat();
        QTextBlockFormat bfmt;
        bfmt.setIndent(obfmt.indent());
        cursor.setBlockFormat(bfmt);
    } else
    {
        QTextListFormat listFmt;
        if (cursor.currentList())
        {
            listFmt = cursor.currentList()->format();
        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);
    }
    cursor.endEditBlock();
}

void Editor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    ui->textEdit->mergeCurrentCharFormat(format);
    ui->textEdit->setFocus(Qt::TabFocusReason);
}

void Editor::slotCursorPositionChanged()
{
    QTextList *l = ui->textEdit->textCursor().currentList();
    if (m_lastBlockList && (l == m_lastBlockList || (l != 0 && m_lastBlockList != 0
                                                     && l->format().style() == m_lastBlockList->format().style())))
    {
        return;
    }
    m_lastBlockList = l;
    if (l)
    {
        QTextListFormat lfmt = l->format();
        if (lfmt.style() == QTextListFormat::ListDisc)
        {
            ui->f_list_bullet->setChecked(true);
            ui->f_list_ordered->setChecked(false);
        } else
        {
            ui->f_list_bullet->setChecked(false);
            ui->f_list_ordered->setChecked(lfmt.style() == QTextListFormat::ListDecimal);
        }
    } else
    {
        ui->f_list_bullet->setChecked(false);
        ui->f_list_ordered->setChecked(false);
    }
}

void Editor::fontChanged(const QFont &f)
{
    ui->f_fontsize->setCurrentIndex(ui->f_fontsize->findText(QString::number(f.pointSize())));
    ui->f_bold->setChecked(f.bold());
    ui->f_italic->setChecked(f.italic());
    ui->f_underline->setChecked(f.underline());
    ui->f_strikeout->setChecked(f.strikeOut());
//    if (f.pointSize() == m_fontsize_h1)
//    {
//        ui->f_paragraph->setCurrentIndex(ParagraphSansSerif);
//    } else if (f.pointSize() == m_fontsize_h2)
//    {
//        ui->f_paragraph->setCurrentIndex(ParagraphSystem);
//    } else if (f.pointSize() == m_fontsize_h3)
//    {
//        ui->f_paragraph->setCurrentIndex(ParagraphHelvetica);
//    } else if (f.pointSize() == m_fontsize_h4)
//    {
//        ui->f_paragraph->setCurrentIndex(ParagraphCourier);
//    } else
//    {
        if (f.fixedPitch() && f.family() == "SansSerif")
        {
            ui->f_paragraph->setCurrentIndex(ParagraphSansSerif);
        }else if (f.fixedPitch() && f.family() == "System")
        {
            ui->f_paragraph->setCurrentIndex(ParagraphSystem);
        } else if (f.fixedPitch() && f.family() == "Helvetica")
        {
            ui->f_paragraph->setCurrentIndex(ParagraphHelvetica);
        } else if (f.fixedPitch() && f.family() == "Courier")
        {
            ui->f_paragraph->setCurrentIndex(ParagraphCourier);
        } else if (f.fixedPitch() && f.family() == "Monospace")
        {
            ui->f_paragraph->setCurrentIndex(ParagraphMonospace);
        } else
        {
            ui->f_paragraph->setCurrentIndex(ParagraphStandard);
        }
//    }
    if (ui->textEdit->textCursor().currentList())
    {
        QTextListFormat lfmt = ui->textEdit->textCursor().currentList()->format();
        if (lfmt.style() == QTextListFormat::ListDisc)
        {
            ui->f_list_bullet->setChecked(true);
            ui->f_list_ordered->setChecked(false);
        } else
        {
            ui->f_list_bullet->setChecked(false);
            ui->f_list_ordered->setChecked(lfmt.style() == QTextListFormat::ListDecimal);
        }
    } else
    {
        ui->f_list_bullet->setChecked(false);
        ui->f_list_ordered->setChecked(false);
    }
}

void Editor::fgColorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    if (c.isValid())
    {
        pix.fill(c);
    } else
    {
        pix.fill(QApplication::palette().foreground().color());
    }
    ui->f_fgcolor->setIcon(pix);
}

void Editor::bgColorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    if (c.isValid())
    {
        pix.fill(c);
    } else
    {
        pix.fill(QApplication::palette().background().color());
    }
    ui->f_bgcolor->setIcon(pix);
}

void Editor::slotCurrentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    bgColorChanged((format.background().isOpaque()) ? format.background().color() : QColor());
    fgColorChanged((format.foreground().isOpaque()) ? format.foreground().color() : QColor());
    ui->f_link->setChecked(format.isAnchor());
}

void Editor::slotClipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        ui->f_paste->setEnabled(md->hasText());
#endif
}

QString Editor::toHtml() const
{
    QString s = ui->textEdit->toHtml();
    // convert emails to links
    s = s.replace(QRegExp(R"((<[^a][^>]+>(?:<span[^>]+>)?|\s)([a-zA-Z\d]+@[a-zA-Z\d]+\.[a-zA-Z]+))"),
                  R"(\1<a href="mailto:\2">\2</a>)");
    // convert links
    s = s.replace(QRegExp(R"((<[^a][^>]+>(?:<span[^>]+>)?|\s)((?:https?|ftp|file)://[^\s'"<>]+))"),
                  R"(\1<a href="\2">\2</a>)");
    // see also: Utils::linkify()
    return s;
}

void Editor::increaseIndentation()
{
    indent(+1);
}

void Editor::decreaseIndentation()
{
    indent(-1);
}

void Editor::indent(int delta)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.beginEditBlock();
    QTextBlockFormat bfmt = cursor.blockFormat();
    int ind = bfmt.indent();
    if (ind + delta >= 0)
    {
        bfmt.setIndent(ind + delta);
    }
    cursor.setBlockFormat(bfmt);
    cursor.endEditBlock();
}

void Editor::setText(const QString &text)
{
    if (text.isEmpty())
    {
        setPlainText(text);
        return;
    }
    if (text[0] == '<')
    {
        setHtml(text);
    } else
    {
        setPlainText(text);
    }
}

void Editor::insertImage()
{
    QSettings s;
    QString attdir = s.value("general/filedialog-path").toString();
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Select an image"),
                                                attdir,
                                                tr("image (*.jpg *.gif *.png *.bmp);; JPEG (*.jpg);; GIF (*.gif);; PNG (*.png);; BMP (*.bmp);; All (*)"));
    if(file!="")
    {
        QImage image = QImageReader(file).read();
        ui->textEdit->dropImage(image, QFileInfo(file).suffix().toUpper().toLocal8Bit().data());
//        ui->textEdit->dropImage(file);
    }

}


void Editor::on_actionopen_triggered()
{
    QFileDialog fileDialog(this);
//    qDebug() << fileDialog->getOpenFileName(this, "打开文件", "", "图片文件(*html);;");
    QFile file(fileDialog.getOpenFileName(this, "打开文件", "", "html文件(*.html);;"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setHtml(file.readAll());
        file.close();
    }
}

void Editor::on_actionopen_folder_triggered()
{
    QFileDialog fileDialog(this);
    ui->local_filesystem_view->setRootIndex(dirModel->index(fileDialog.getExistingDirectory(this, "打开文件夹")));
}

void Editor::on_actionsave_triggered()
{
    QFileDialog fileDialog(this);
    QFile file(fileDialog.getSaveFileName(this, "保存文件", "", "html文件(*.html);;"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        textStream << toHtml();
    }
}

void Editor::on_actionlatex_triggered()
{
    AccountManager * access = new AccountManager();
    QFileDialog fileDialog(this);
    QString file = fileDialog.getSaveFileName(this, "导出文件", "", "tex文件(*.tex);;");
    qDebug() << "open file:" << file;
    QFile tmp("tmp.html");
    if(tmp.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream textStream(&tmp);
        textStream << toHtml();
        tmp.close();
    }
    QProcess process(this);
    qDebug() << process.execute("java -jar plugins/htmltolatex.jar -output "+file);
}

void Editor::on_actionpdf_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "导出文件", "", "pdf文件(*.pdf);;");
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(file);
    document()->print(&printer);
}

void Editor::on_n_refresh_clicked()
{
    manager.getFiles();
}

void Editor::on_n_delete_clicked()
{
    QModelIndex modelIndex = ui->remote_file_view->currentIndex();
    QString filename = ui->remote_file_view->model()->itemData(modelIndex).value(0).toString();
    manager.deleteFile(filename);
}

void Editor::on_n_upload_clicked()
{
    QString fileName = QInputDialog::getText(this, "输入文件名", "File Name:", QLineEdit::Normal);
    manager.uploadFile(fileName, toHtml());
}

void Editor::local_filesystem_view_doublclicked(const QModelIndex &modelIndex)
{
    QFile file(dirModel->filePath(modelIndex));
    qDebug() << file;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setHtml(file.readAll());
        file.close();
    }
}

void Editor::remote_file_view_doublclicked(const QModelIndex & modelIndex)
{
    manager.getContent(ui->remote_file_view->model()->itemData(modelIndex).value(0).toString());
}

Editor::~Editor()
{
    delete ui;
}
