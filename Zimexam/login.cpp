#include "login.h"
#include "ui_login.h"
#include "testform.h"
#include "configform.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_Exit_clicked()
{
    this->close();
}

void Login::on_pushButton_Ok_clicked()
{
    TestForm* tf = new TestForm();
    tf->show();
    this->close();
}

void Login::on_pushButton_Config_clicked()
{
    ConfigForm* cf = new ConfigForm();
    cf->show();
}
