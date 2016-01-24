#include "paperbank.h"
#include "ui_paperbank.h"

PaperBank::PaperBank(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaperBank)
{
    ui->setupUi(this);
}

PaperBank::~PaperBank()
{
    delete ui;
}
