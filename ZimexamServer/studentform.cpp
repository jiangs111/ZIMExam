/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：studentform.cpp
 * 文件标识：无
 * 内容摘要：考试系统教师添加修改考生源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.21
 *
 ********************************************************************/
#include "studentform.h"
#include "ui_studentform.h"
#include <QtWidgets>
#include <QtSql>
#include <QException>
#include <QDebug>

StudentForm::StudentForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentForm)
{
    ui->setupUi(this);
}

StudentForm::StudentForm(QWidget *parent, QString id) :
    QWidget(parent),
    ui(new Ui::StudentForm),
    m_testid(id)
{
    ui->setupUi(this);
    if(m_testid != "")
        loadStudent();
}

StudentForm::~StudentForm()
{
    delete ui;
}

/********************************************************************
 * 功能描述：添加保存按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     保存新考生
 ********************************************************************/
void StudentForm::on_pushButton_Ok_clicked()
{
    if(ui->lineEdit_Id->text() == "")
        QMessageBox::warning(this, tr("输入错误"), tr("请输入考号......"));
    else if(ui->lineEdit_Name->text() == "")
        QMessageBox::warning(this, tr("输入错误"), tr("请输入考生姓名......"));
    else if(ui->lineEdit_Class->text() == "")
        QMessageBox::warning(this, tr("输入错误"), tr("请输入考生班级......"));
    else
    {
        if(m_testid != "")
        {
            if(editStudent())
                QMessageBox::information(this, tr("保存成功"), tr("修改考生")
                                     + ui->lineEdit_Name->text() + "成功!");
        }
        else
        {
            if(!checkTestId())
                QMessageBox::warning(this, tr("输入错误"), tr("该考号的考生已存在......"));
            else
            if(addStudent())
                QMessageBox::information(this, tr("保存成功"), tr("添加考生")
                                     + ui->lineEdit_Name->text() + "成功!");
        }
        this->close();
    }
}

/********************************************************************
 * 功能描述：获得连接数据库
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QString
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     获得连接数据库
 ********************************************************************/
QString StudentForm::getActivedb()
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
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     设置连接数据库
 ********************************************************************/
void StudentForm::setActivedb(QString db)
{
    m_activeDb = db;
}

/********************************************************************
 * 功能描述：退出按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     关闭窗口
 ********************************************************************/
void StudentForm::on_pushButton_Exit_clicked()
{
    this->close();
}

/********************************************************************
 * 功能描述：检查考生考号
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：bool
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     检查考生考号是否已在
 ********************************************************************/
bool StudentForm::checkTestId()
{
    QString sql;
    sql = "select TestId from Login_student where TestId = '" + ui->lineEdit_Id->text() + "'";
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);

    QSqlQuery query(sql, database);
    //qDebug() << sql;
    int fieldNo = query.record().indexOf("TestId");
    while (query.next()) {
            QString tid = query.value(fieldNo).toString();
            if(tid == ui->lineEdit_Id->text())
            {
                database.close();
                QSqlDatabase::removeDatabase(m_activeDb);
                //qDebug() << sql;
                return false;
            }
        }
    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);
    return true;
}

/********************************************************************
 * 功能描述：添加新考生
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：bool
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     添加新考生
 ********************************************************************/
bool StudentForm::addStudent()
{
    QString sql;
    sql = "INSERT INTO Login_student (TestId, Name, Class) VALUES ("
            + ui->lineEdit_Id->text() + ", " + ui->lineEdit_Name->text() +", "
            + ui->lineEdit_Class->text() + ")";
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);

    //qDebug() << sql;
    QSqlQuery query(sql, database);
    QSqlError err = query.lastError();
    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);
    if (err.type() != QSqlError::NoError)
    {
        QMessageBox::warning(this, tr("无法打开数据库"), tr("添加考生出错......") + err.text());
        return false;
    }
    else
        return true;
}

/********************************************************************
 * 功能描述：读取要修改的考生
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     读取要修改的考生信息
 ********************************************************************/
void StudentForm::loadStudent()
{
    QString sql;
    sql = "select Name, Class from Login_student where TestId = '" + m_testid + "'";
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);

    QSqlQuery query(sql, database);
    QSqlRecord rec = query.record();
    int namCol = rec.indexOf("Name");
    int claCol = rec.indexOf("Class");
    ui->lineEdit_Id->setText(m_testid);
    while(query.next())
    {
        ui->lineEdit_Name->setText(query.value(namCol).toString());
        ui->lineEdit_Class->setText(query.value(claCol).toString());
    }
    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);
}

/********************************************************************
 * 功能描述：修改考生
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     修改的考生信息
 ********************************************************************/
bool StudentForm::editStudent()
{
    QString sql;
    if(ui->lineEdit_Id->text() != m_testid)
    {
        if(!checkTestId())
        {
            QMessageBox::warning(this, tr("输入错误"), tr("该考号的考生已存在......"));
            return false;
        }
    }
    sql = "update Login_student set TestId = "
            + ui->lineEdit_Id->text() + ", Name = " + ui->lineEdit_Name->text()
            +", Class = "+ ui->lineEdit_Class->text() + " where TestId = " + m_testid;
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);

    //qDebug() << sql;
    QSqlQuery query(sql, database);
    QSqlError err = query.lastError();
    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);
    if (err.type() != QSqlError::NoError)
    {
        QMessageBox::warning(this, tr("无法打开数据库"), tr("添加考生出错......") + err.text());
        return false;
    }
    else
        return true;
}
