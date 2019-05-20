#include "exam.h"
#include "ui_exam.h"

Exam::Exam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Exam)
{
    ui->setupUi(this);

    init();
}

Exam::~Exam()
{
    delete ui;
}

void Exam::init()
{
    setWindowTitle(tr("在线考试"));
}
