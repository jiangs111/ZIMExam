/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：studentForm.h
 * 文件标识：无
 * 内容摘要：考试系统教师添加修改考生头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.21
 *
 ********************************************************************/
#ifndef STUDENTFORM_H
#define STUDENTFORM_H

#include <QWidget>

namespace Ui {
class StudentForm;
}

class StudentForm : public QWidget
{
    Q_OBJECT

public:
    explicit StudentForm(QWidget *parent = 0);
    explicit StudentForm(QWidget *parent = 0, QString id = "");
    ~StudentForm();
    void setActivedb(QString db);       //设置连接数据库
    QString getActivedb();              //获得连接数据库

private slots:
    void on_pushButton_Ok_clicked();    //保存按钮
    void on_pushButton_Exit_clicked();  //退出按钮

private:
    Ui::StudentForm *ui;
    QString m_activeDb;
    QString m_testid;

    bool checkTestId();     //检查考生考号
    bool addStudent();      //添加考生
    void loadStudent();     //读取要修改的考生
    bool editStudent();     //修改考生
};

#endif // STUDENTFORM_H
