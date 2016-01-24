/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：helpform.cpp
 * 文件标识：无
 * 内容摘要：考试系统关于说明源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.7
 *
 ********************************************************************/
#include "helpform.h"
#include "ui_helpform.h"

HelpForm::HelpForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpForm)
{
    ui->setupUi(this);
}

HelpForm::~HelpForm()
{
    delete ui;
}

/********************************************************************
 * 功能描述：退出按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.7  版本号     修改人         修改内容
 * 2015.12.7           V1.0a     Jiangs111     关闭窗口
 ********************************************************************/
void HelpForm::on_pushButton_Close_clicked()
{
    this->close();
}
