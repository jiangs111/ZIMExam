#include "testingform.h"
#include "ui_testingform.h"

TestingForm::TestingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestingForm)
{
    ui->setupUi(this);
    //this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);//全屏
}

TestingForm::~TestingForm()
{
    delete ui;
}

void TestingForm::on_pushButton_Exit_clicked()
{
    //交卷操作......
    this->close();
}
