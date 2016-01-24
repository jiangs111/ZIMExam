/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：helpform.h
 * 文件标识：无
 * 内容摘要：考试系统关于说明头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.7
 *
 ********************************************************************/
#ifndef HELPFORM_H
#define HELPFORM_H

#include <QWidget>

namespace Ui {
class HelpForm;
}

class HelpForm : public QWidget
{
    Q_OBJECT

public:
    explicit HelpForm(QWidget *parent = 0);
    ~HelpForm();

private slots:
    void on_pushButton_Close_clicked();     //关闭按钮

private:
    Ui::HelpForm *ui;
};

#endif // HELPFORM_H
