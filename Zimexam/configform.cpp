#include "configform.h"
#include "ui_configform.h"

ConfigForm::ConfigForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigForm)
{
    ui->setupUi(this);
}

ConfigForm::~ConfigForm()
{
    delete ui;
}


void ConfigForm::on_pushButton_Exit_clicked()
{
    this->close();
}
