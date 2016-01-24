/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：passwordform.cpp
 * 文件标识：无
 * 内容摘要：考试系统教师修改密码源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.14
 *
 ********************************************************************/
#include "passwordform.h"
#include "ui_passwordform.h"
#include <QtWidgets>
#include <QtSql>
#include <QException>
#include <QDebug>

PasswordForm::PasswordForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordForm)
{
    ui->setupUi(this);
}

PasswordForm::~PasswordForm()
{
    delete ui;
}

/********************************************************************
 * 功能描述：设置教师号
 *
 * 输入参数：QString teacher
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     设置教师号并在菜单显示
 ********************************************************************/
void PasswordForm::setTeacher(QString teacher)
{
    m_teacher = teacher;
}

/********************************************************************
 * 功能描述：获得教师号
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QString
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     获得教师号
 ********************************************************************/
QString PasswordForm::getTeacher()
{
    return m_teacher;
}

/********************************************************************
 * 功能描述：退出按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     关闭窗口
 ********************************************************************/
void PasswordForm::on_pushButton_Exit_clicked()
{
    this->close();
}

/********************************************************************
 * 功能描述：修改登录密码
 *
 * 输入参数：QString teacher
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     修改登录密码
 ********************************************************************/
bool PasswordForm::changePassword(QString teacher)
{
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);
    QString str = "update Login_teacher set Password = '"
            + ui->lineEdit_Password->text() + "' where Name = '" + teacher + "'";
    //qDebug() << str;
    QSqlQuery query(str, database);
    if(!query.exec())
    {
        //qDebug() << query.lastError();
        database.close();
        QSqlDatabase::removeDatabase(m_activeDb);
        return false;
    }
    else
    {
        //qDebug() << "update!";
        database.close();
        QSqlDatabase::removeDatabase(m_activeDb);
        return true;
    }
}

/********************************************************************
 * 功能描述：确认修改按钮
 *
 * 输入参数：QString teacher
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     确认新密码
 ********************************************************************/
void PasswordForm::on_pushButton_Ok_clicked()
{
    if(ui->lineEdit_Password->text() != ui->lineEdit_Password_2->text())
    {
        QMessageBox::warning(this, tr("输入错误"), tr("新密码两次输入不同......"));
    }
    else
    {
        if(!changePassword(m_teacher))
        {
            QMessageBox::warning(this, tr("修改错误"), tr("密码修改错误......"));
            ui->lineEdit_Password->setText("");
            ui->lineEdit_Password_2->setText("");
        }
        else
        {
            QMessageBox::information(this, tr("修改成功"), tr("修改密码成功。"));
            this->close();
        }
    }
}

/********************************************************************
 * 功能描述：获得连接数据库
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QString
 * 其他说明：无
 * 完成日期：2015.12.15  版本号     修改人         修改内容
 * 2015.12.15           V1.0a     Jiangs111     获得连接数据库
 ********************************************************************/
QString PasswordForm::getActivedb()
{
    return m_activeDb;
}

/********************************************************************
 * 功能描述：设置连接数据库
 *
 * 输入参数：QString db
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.7  版本号     修改人         修改内容
 * 2015.12.7           V1.0a     Jiangs111     设置连接数据库
 ********************************************************************/
void PasswordForm::setActivedb(QString db)
{
    m_activeDb = db;
}
