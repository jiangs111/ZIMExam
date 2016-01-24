/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：loginserver.cpp
 * 文件标识：无
 * 内容摘要：考试系统教师登录源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.7
 *
 ********************************************************************/
#include "loginserver.h"
#include "ui_loginserver.h"
#include "manageform.h"
#include <QtWidgets>
#include <QtSql>
#include <QException>
#include <QDebug>

loginServer::loginServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginServer)
{
    ui->setupUi(this);
    m_activeDb = "examdb";
}

loginServer::~loginServer()
{
    delete ui;
}

/********************************************************************
 * 功能描述：登录系统按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.7  版本号     修改人         修改内容
 * 2015.12.7           V1.0a     Jiangs111     创建对象及登录
 ********************************************************************/
void loginServer::on_pushButton_Ok_clicked()
{
    //教师登录
    if(!open_data())
    {
        QMessageBox::warning(this, tr("无法登录"), tr("教师号或密码错误......"));
        ui->lineEdit_No->setText("");
        ui->lineEdit_Password->setText("");
        ui->lineEdit_No->setFocus();
    }
    else
    {
        ManageForm* mf = new ManageForm();
        mf->setTeacher(ui->lineEdit_No->text());
        mf->setActivedb(m_activeDb);
        mf->show();
        this->close();
    }

}

/********************************************************************
 * 功能描述：连接数据库
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：bool
 * 其他说明：无
 * 完成日期：2015.12.7  版本号     修改人         修改内容
 * 2015.12.7           V1.0a     Jiangs111     连接数据库验证教师号
 ********************************************************************/
bool loginServer::open_data()
{
    //连接db
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);
    QSqlError err;

    if(!database.open())
    {
        err = database.lastError();
        database = QSqlDatabase();
        QSqlDatabase::removeDatabase(m_activeDb);
        //qDebug() << m_activeDb <<err;
    }
    else
    {
        QString str = "select Name, Password from Login_teacher where Name='"
                + ui->lineEdit_No->text() +"'";
        QSqlQuery query(str, database);
        int fieldNo = query.record().indexOf("Password");
        while (query.next()) {
                QString password = query.value(fieldNo).toString();
                if(password == ui->lineEdit_Password->text())
                {
                    database.close();
                    QSqlDatabase::removeDatabase(m_activeDb);
                    //qDebug() << "|" << password;
                    return true;
                }
            }
    }
    return false;
}
