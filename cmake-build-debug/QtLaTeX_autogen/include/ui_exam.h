/********************************************************************************
** Form generated from reading UI file 'exam.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXAM_H
#define UI_EXAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Exam
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Exam)
    {
        if (Exam->objectName().isEmpty())
            Exam->setObjectName(QString::fromUtf8("Exam"));
        Exam->resize(673, 527);
        centralwidget = new QWidget(Exam);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(220, 180, 281, 171));
        Exam->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Exam);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 673, 17));
        Exam->setMenuBar(menubar);
        statusbar = new QStatusBar(Exam);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Exam->setStatusBar(statusbar);

        retranslateUi(Exam);

        QMetaObject::connectSlotsByName(Exam);
    } // setupUi

    void retranslateUi(QMainWindow *Exam)
    {
        Exam->setWindowTitle(QApplication::translate("Exam", "Editor", nullptr));
        pushButton->setText(QApplication::translate("Exam", "\346\265\213\350\257\225  to do ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Exam: public Ui_Exam {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXAM_H
