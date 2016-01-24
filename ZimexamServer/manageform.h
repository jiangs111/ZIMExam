/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：manageform.h
 * 文件标识：无
 * 内容摘要：考试系统教师管理界面头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.7
 *
 ********************************************************************/
#ifndef MANAGEFORM_H
#define MANAGEFORM_H

#include <QMainWindow>
#include <QtSql>

class Assistant;

namespace Ui {
class ManageForm;
}

class ManageForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManageForm(QWidget *parent = 0);
    ~ManageForm();
    void setTeacher(QString teacher);   //设置教师号
    QString getTeacher();               //获得教师号
    void setActivedb(QString db);       //设置连接数据库
    QString getActivedb();              //获得连接数据库
    QStringList getQType();             //获得试题类型


private slots:
    void on_action_Exit_triggered();    //退出菜单按钮
    void on_action_About_triggered();   //关于菜单按钮
    void on_action_Help_triggered();    //帮助菜单按钮
    void on_actionPassword_triggered(); //修改登录密码按钮
    void on_comboBox__Select_currentIndexChanged(const QString &arg1);  //浏览学生下拉列表
    void on_actionStuView_triggered();      //浏览学生菜单按钮
    void on_pushButton_Select_clicked();    //考生浏览按钮
    void on_pushButton_Add_clicked();       //添加考生按钮
    void on_pushButton_Del_clicked();       //删除考生按钮
    void on_pushButton_Edit_clicked();      //修改考生按钮
    void on_pushButton_Import_clicked();    //导入考生按钮
    void on_pushButton_Build_clicked();     //随机生成考生按钮
    void on_pushButton_Delgroup_clicked();  //批量删除考生按钮
    void on_pushButton_Editgroup_clicked(); //批量修改考生按钮
    void on_pushButton_Template_clicked();  //导入考生模板下载按钮
    void on_actionTeststat_triggered();     //考试分析菜单按钮
    void on_actionPaperstat_triggered();    //试卷分析菜单按钮
    void on_action_Question_triggered();    //试题浏览菜单按钮
    void on_pushButton_QDelgroup_clicked(); //批量删除试题按钮
    void on_pushButton_QEditgroup_clicked();//批量修改试题按钮
    void on_pushButton_QTemplate_clicked(); //导入试题模板下载按钮
    void on_comboBox__QSelect_currentIndexChanged(const QString &arg1);//浏览试题下拉列表
    void on_pushButton_QSelect_clicked();   //试题浏览按钮
    void on_pushButton_QAdd_clicked();      //添加试题按钮
    void on_pushButton_QDel_clicked();      //删除试题按钮
    void on_pushButton_QEdit_clicked();     //修改试题按钮

    void on_pushButton_QImport_clicked();

    void on_action_Paperbank_triggered();

    void on_comboBox__PBSelect_currentIndexChanged(const QString &arg1);

    void on_toolButton_PB_clicked();

    void on_calendarWidget_PB_selectionChanged();

    void on_toolButton_Q_clicked();

    void on_calendarWidget_Q_selectionChanged();

    void on_pushButton_PBSelect_clicked();

private:
    Ui::ManageForm *ui;
    QString m_teacher;
    QString m_activeDb;
    Assistant *assistant;
    QStringList m_typenames;
    QStringList m_typeIds;

    bool delStudent(QString tid);      //删除考生
    void reviewStudent();              //重新浏览考生
    void clearshow();                  //清除显示
    void reviewQuestion();             //重新浏览试题
    bool delQuestion(QString tid);     //删除试题
    void reviewpaperbank();            //重新浏览试卷
};

#endif // MANAGEFORM_H
