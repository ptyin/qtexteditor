#ifndef _MTEXTEDIT_H_
#define _MTEXTEDIT_H_

#include <QTextEdit>
#include <QMimeData>
#include <QImage>

class MTextEdit : public QTextEdit
{
Q_OBJECT
public:
    MTextEdit(QWidget *parent);

    void dropImage(const QImage &image, const QString &format);

    void dropImage(const QString &name);

signals:
    void tab();

protected:
    bool canInsertFromMimeData(const QMimeData *source) const;

    void insertFromMimeData(const QMimeData *source);

    QMimeData *createMimeDataFromSelection() const;

    void keyPressEvent(QKeyEvent *e) override;
};

#endif
