/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：passwordform.h
 * 文件标识：无
 * 内容摘要：考试系统教师修改密码头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.14
 *
 ********************************************************************/
#ifndef PASSWORDFORM_H
#define PASSWORDFORM_H

#include <QWidget>

namespace Ui {
class PasswordForm;
}

class PasswordForm : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordForm(QWidget *parent = 0);
    ~PasswordForm();
    void setTeacher(QString teacher);   //设置教师号
    QString getTeacher();               //获得教师号
    void setActivedb(QString db);       //设置连接数据库
    QString getActivedb();              //获得连接数据库

private slots:
    void on_pushButton_Exit_clicked();  //退出按钮
    void on_pushButton_Ok_clicked();    //确认按钮

private:
    Ui::PasswordForm *ui;
    QString m_teacher;
    QString m_activeDb;

    bool changePassword(QString teacher);   //修改登录密码
};

#endif // PASSWORDFORM_H
