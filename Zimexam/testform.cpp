#include "testform.h"
#include "ui_testform.h"
#include "testingform.h"
#include <QKeyEvent>

TestForm::TestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);
}

TestForm::~TestForm()
{
    delete ui;
}

void TestForm::on_pushButton_Exit_clicked()
{
    this->close();
}

void TestForm::keyPressEvent(QKeyEvent *event)
{

}

void TestForm::on_pushButton_EnterP_clicked()
{
    //练习模式判断
    TestingForm* tf = new TestingForm();
    tf->show();
    this->close();
}

void TestForm::on_pushButton_EnterT_clicked()
{
    //考试模式判断
    TestingForm* tf = new TestingForm();
    tf->show();
    this->close();
}
