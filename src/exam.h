#ifndef EXAM_H
#define EXAM_H

#include <QMainWindow>

namespace Ui
{
    class Exam;
}

class Exam : public QMainWindow
{
Q_OBJECT

public:
    explicit Exam(QWidget *parent = 0);

    ~Exam();

    void init();

private:
    Ui::Exam *ui;
};

#endif // EXAM_H
