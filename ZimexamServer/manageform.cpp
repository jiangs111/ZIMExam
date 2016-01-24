/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：manageform.cpp
 * 文件标识：无
 * 内容摘要：考试系统教师管理源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.7
 *
 ********************************************************************/
#include "manageform.h"
#include "ui_manageform.h"
#include "helpform.h"
#include "assistant.h"
#include "passwordform.h"
#include "studentform.h"
#include "importexcel.h"
#include "questionform.h"
#include "importquestion.h"
#include <QFile>
#include <QtWidgets>
#include <QDebug>
#include <QProcess>
#include <QtSql>
#include <QStringList>

ManageForm::ManageForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManageForm)
{
    ui->setupUi(this);

    // 创建Assistant对象
    assistant = new Assistant;

    //默认不显示内容
    clearshow();
}

ManageForm::~ManageForm()
{
    delete ui;

    // 销毁assistant
    delete assistant;
}

/********************************************************************
 * 功能描述：退出菜单按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.7  版本号     修改人         修改内容
 * 2015.12.7           V1.0a     Jiangs111     关闭窗口
 ********************************************************************/
void ManageForm::on_action_Exit_triggered()
{
    this->close();
}

/********************************************************************
 * 功能描述：设置教师号
 *
 * 输入参数：QString teacher
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.7  版本号     修改人         修改内容
 * 2015.12.7           V1.0a     Jiangs111     设置教师号并在菜单显示
 ********************************************************************/
void ManageForm::setTeacher(QString teacher)
{
    m_teacher = teacher;
    ui->menu_Exit->setTitle("退出(" + teacher +")");
}

/********************************************************************
 * 功能描述：获得教师号
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QString
 * 其他说明：无
 * 完成日期：2015.12.7  版本号     修改人         修改内容
 * 2015.12.7           V1.0a     Jiangs111     获得教师号
 ********************************************************************/
QString ManageForm::getTeacher()
{
    return m_teacher;
}

/********************************************************************
 * 功能描述：关于菜单按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.7  版本号     修改人         修改内容
 * 2015.12.7           V1.0a     Jiangs111     显示关于窗口
 ********************************************************************/
void ManageForm::on_action_About_triggered()
{
    HelpForm* hf = new HelpForm();
    hf->show();
}

/********************************************************************
 * 功能描述：帮助菜单按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.10  版本号     修改人         修改内容
 * 2015.12.10           V1.0a     Jiangs111     显示帮助文件
 ********************************************************************/
void ManageForm::on_action_Help_triggered()
{
    // 按下“help”按钮，运行Qt Assistant，显示index.htm页面
    assistant->showDocumentation("index.htm");
}

/********************************************************************
 * 功能描述：修改登录密码按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.10  版本号     修改人         修改内容
 * 2015.12.10           V1.0a     Jiangs111     显示密码修改窗口
 ********************************************************************/
void ManageForm::on_actionPassword_triggered()
{
    PasswordForm* pf = new PasswordForm();
    pf->setTeacher(this->getTeacher());
    pf->show();
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
QString ManageForm::getActivedb()
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
void ManageForm::setActivedb(QString db)
{
    m_activeDb = db;
}

/********************************************************************
 * 功能描述：下拉考生列表修改标签
 *
 * 输入参数：const QString &arg1
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     修改标签显示
 ********************************************************************/
void ManageForm::on_comboBox__Select_currentIndexChanged(const QString &arg1)
{
    switch(ui->comboBox__Select->currentIndex())
    {
    case 0:
        ui->label_Select->setText("班级名称为:");
        ui->lineEdit_Select->clear();
        break;
    case 1:
        ui->label_Select->setText("考生姓名为:");
        ui->lineEdit_Select->clear();
        break;
    case 2:
        ui->label_Select->setText("考生考号为:");
        ui->lineEdit_Select->clear();
        break;
    }
    arg1.size();
}

/********************************************************************
 * 功能描述：浏览考生菜单显示
 *
 * 输入参数：const QString &arg1
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     浏览考生菜单显示
 ********************************************************************/
void ManageForm::on_actionStuView_triggered()
{
    clearshow();
    ui->groupBox_View->show();
}

/********************************************************************
 * 功能描述：考生浏览按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     浏览考生菜单显示
 ********************************************************************/
void ManageForm::on_pushButton_Select_clicked()
{
    reviewStudent();
}

/********************************************************************
 * 功能描述：考生更新显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     考生更新显示
 ********************************************************************/
void ManageForm::reviewStudent()
{
    QString sql;
        switch(ui->comboBox__Select->currentIndex())
        {
        case 0:
            sql="select TestId as 考号, Name as 姓名, Class as 班级 from Login_student where Class='"
                    + ui->lineEdit_Select->text() + "'";
            break;
        case 1:
            sql="select TestId as 考号, Name as 姓名, Class as 班级 from Login_student where Name='"
                    + ui->lineEdit_Select->text() + "'";
            break;
        case 2:
            sql="select TestId as 考号, Name as 姓名, Class as 班级 from Login_student where TestId='"
                    + ui->lineEdit_Select->text() + "'";

        }
        //qDebug() << sql;
        QSqlDatabase database;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
            database = QSqlDatabase::database("qt_sql_default_connection");
        else
            database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(m_activeDb);

        QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
        model->setQuery(QSqlQuery(sql, database));
        ui->tableView->setModel(model);
        ui->label_Count->setText("共浏览出" + QString::number(model->rowCount()) + "名考生。");

        database.close();
        QSqlDatabase::removeDatabase(m_activeDb);
}

/********************************************************************
 * 功能描述：添加考生按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     添加考生
 ********************************************************************/
void ManageForm::on_pushButton_Add_clicked()
{
    StudentForm* sf = new StudentForm(0,"");
    sf->setActivedb(this->m_activeDb);
    sf->show();
}

/********************************************************************
 * 功能描述：删除考生按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     删除考生
 ********************************************************************/
void ManageForm::on_pushButton_Del_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows(0);
    if (!selection.empty())
    {
        QModelIndex idIndex = selection.at(0);
        //int id = idIndex.data().toInt();
        QString testid = idIndex.sibling(idIndex.row(), 0).data().toString();
        QString name = idIndex.sibling(idIndex.row(), 1).data().toString();
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,tr("删除考生"),QString(tr("确认删除考号为'%1'的'%2'考生吗？").arg(testid).arg(name)),QMessageBox::Yes|QMessageBox::No);

        if(button == QMessageBox::Yes)
        {
            if(delStudent(testid))
               QMessageBox::information(this, tr("删除考生"),tr("考生成功删除......"));
            reviewStudent();
        }else{
            QMessageBox::information(this, tr("删除考生"),tr("删除已取消......"));
        }
    }
    else
        QMessageBox::warning(this, tr("删除考生"), tr("请选择要删除的考生......"));
}

/********************************************************************
 * 功能描述：删除考生
 *
 * 输入参数：QString tid
 * 输出参数：无
 * 返回值　：bool
 * 其他说明：无
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     删除考生
 ********************************************************************/
bool ManageForm::delStudent(QString tid)
{
    QString sql = "delete from Login_student where TestId = '" + tid +"'";
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
        QMessageBox::warning(this, tr("无法打开数据库"), tr("删除考生出错......") + err.text());
        return false;
    }
    else
        return true;
}

/********************************************************************
 * 功能描述：修改考生按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     修改考生
 ********************************************************************/
void ManageForm::on_pushButton_Edit_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows(0);
    if (!selection.empty())
    {
        QModelIndex idIndex = selection.at(0);
        //int id = idIndex.data().toInt();
        QString testid = idIndex.sibling(idIndex.row(), 0).data().toString();
        QString name = idIndex.sibling(idIndex.row(), 1).data().toString();
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,tr("修改考生"),QString(tr("确认修改考号为'%1'的'%2'考生吗？").arg(testid).arg(name)),QMessageBox::Yes|QMessageBox::No);

        if(button == QMessageBox::Yes)
        {
            StudentForm* sf = new StudentForm(0, testid);
            sf->setActivedb(this->m_activeDb);
            sf->show();
            reviewStudent();
        }else{
            QMessageBox::information(this, tr("修改考生"),tr("修改已取消......"));
        }
    }
    else
        QMessageBox::warning(this, tr("修改考生"), tr("请选择要修改的考生......"));
}

/********************************************************************
 * 功能描述：导入考生按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     显示导入考生
 ********************************************************************/
void ManageForm::on_pushButton_Import_clicked()
{
    Importexcel* it = new Importexcel();
    it->setActivedb(m_activeDb);
    it->show();
}

/********************************************************************
 * 功能描述：随机生成考生按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.4  版本号     修改人         修改内容
 * 2016.1.4           V1.0a     Jiangs111     随机生成考生
 ********************************************************************/
void ManageForm::on_pushButton_Build_clicked()
{
    QMessageBox::information(this, tr("尚未完成"), tr("功能稍后实现......"));
}

/********************************************************************
 * 功能描述：批量删除考生按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.4  版本号     修改人         修改内容
 * 2016.1.4           V1.0a     Jiangs111     批量删除考生
 ********************************************************************/
void ManageForm::on_pushButton_Delgroup_clicked()
{
    QMessageBox::information(this, tr("尚未完成"), tr("功能稍后实现......"));
}

/********************************************************************
 * 功能描述：批量修改考生按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.4  版本号     修改人         修改内容
 * 2016.1.4           V1.0a     Jiangs111     批量修改考生
 ********************************************************************/
void ManageForm::on_pushButton_Editgroup_clicked()
{
    QMessageBox::information(this, tr("尚未完成"), tr("功能稍后实现......"));
}

/********************************************************************
 * 功能描述：导入模板下载按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.4  版本号     修改人         修改内容
 * 2016.1.4           V1.0a     Jiangs111     导入模板下载
 ********************************************************************/
void ManageForm::on_pushButton_Template_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "d://",
                               tr("excelfiles (*.xls *.XLS)"));
    if(QFile::copy(".//addin//student_example.xls",fileName))
    {
        QMessageBox::information(this, tr("下载导入模板"), tr("复制导入模板成功."));
    }
    else
    {
        QMessageBox::warning(this,tr("下载导入模板"), tr("复制导入模板错误."));
    }
}

/********************************************************************
 * 功能描述：考试分析菜单显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     考试分析菜单显示
 ********************************************************************/
void ManageForm::on_actionTeststat_triggered()
{
    clearshow();
    ui->groupBox_Teststat->show();
}

/********************************************************************
 * 功能描述：试卷分析菜单显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.21  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     试卷分析菜单显示
 ********************************************************************/
void ManageForm::on_actionPaperstat_triggered()
{
    clearshow();
    ui->groupBox_Paperstat->show();
}

/********************************************************************
 * 功能描述：清除主窗口显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.4  版本号     修改人         修改内容
 * 2016.1.4           V1.0a     Jiangs111     清除主窗口显示
 ********************************************************************/
void ManageForm::clearshow()
{
    ui->groupBox_View->hide();
    ui->groupBox_Teststat->hide();
    ui->groupBox_Paperstat->hide();
    ui->groupBox_Question->hide();
    ui->groupBox_Paperbank->hide();
    ui->groupBox_Paperstat->hide();
    //试题类型
    ui->comboBox_QType->hide();
    ui->label_Difficulty->hide();
    ui->calendarWidget_Q->hide();
    ui->dateEdit_QSelect->hide();
    ui->toolButton_Q->hide();
    ui->comboBox_PBType->hide();
    ui->calendarWidget_PB->hide();
    ui->toolButton_PB->hide();
    ui->calendarWidget_PB->hide();
    ui->dateEdit_PBSelect->hide();
}

/********************************************************************
 * 功能描述：浏览题库菜单显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5           V1.0a     Jiangs111     浏览题库菜单显示
 ********************************************************************/
void ManageForm::on_action_Question_triggered()
{
    clearshow();
    ui->groupBox_Question->show();
}

/********************************************************************
 * 功能描述：批量删除试题按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5           V1.0a     Jiangs111     批量删除试题
 ********************************************************************/
void ManageForm::on_pushButton_QDelgroup_clicked()
{
    QMessageBox::information(this, tr("尚未完成"), tr("功能稍后实现......"));
}

/********************************************************************
 * 功能描述：批量修改试题按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5           V1.0a     Jiangs111     批量修改试题
 ********************************************************************/
void ManageForm::on_pushButton_QEditgroup_clicked()
{
    QMessageBox::information(this, tr("尚未完成"), tr("功能稍后实现......"));
}

/********************************************************************
 * 功能描述：导入试题模板下载按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5           V1.0a     Jiangs111     导入试题模板下载
 ********************************************************************/
void ManageForm::on_pushButton_QTemplate_clicked()
{
    QMessageBox::information(this, tr("尚未完成"), tr("目前暂实现文本试题导入,非文本试题功能稍后实现......"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "d://",
                               tr("excelfiles (*.xls *.XLS)"));
    if(QFile::copy(".//addin//question_example.xls",fileName))
    {
        QMessageBox::information(this, tr("下载导入模板"), tr("复制导入模板成功."));
    }
    else
    {
        QMessageBox::warning(this,tr("下载导入模板"), tr("复制导入模板错误."));
    }
}

/********************************************************************
 * 功能描述：下拉试题列表修改标签
 *
 * 输入参数：const QString &arg1
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5           V1.0a     Jiangs111     修改标签显示
 ********************************************************************/
void ManageForm::on_comboBox__QSelect_currentIndexChanged(const QString &arg1)
{
    if(m_typenames.size() == 0)
    {
        m_typenames = getQType();
        //qDebug() << "good";
    }
    switch(ui->comboBox__QSelect->currentIndex())
    {
    case 0:
        ui->label_QSelect->setText("试题名称为:");
        ui->comboBox_QType->hide();
        ui->label_Difficulty->hide();
        ui->dateEdit_QSelect->hide();
        ui->toolButton_Q->hide();
        ui->lineEdit_QSelect->show();
        ui->lineEdit_QSelect->clear();
        break;
    case 1:
        ui->label_QSelect->setText("试题题型为:");
        ui->lineEdit_QSelect->hide();
        ui->label_Difficulty->hide();
        ui->dateEdit_QSelect->hide();
        ui->toolButton_Q->hide();
        ui->comboBox_QType->show();
        ui->comboBox_QType->clear();
        for(int i = 0; i < m_typenames.size(); i++)
        {
            ui->comboBox_QType->addItem(m_typenames[i]);
        }
        ui->comboBox_QType->update();
        ui->comboBox_QType->setFocus();
        break;
    case 2:
        ui->label_QSelect->setText("出题人为:");
        ui->comboBox_QType->hide();
        ui->label_Difficulty->hide();
        ui->dateEdit_QSelect->hide();
        ui->toolButton_Q->hide();
        ui->lineEdit_QSelect->show();
        ui->lineEdit_QSelect->clear();
        break;
    case 3:
        ui->label_QSelect->setText("出题时间为:");
        ui->comboBox_QType->hide();
        ui->lineEdit_QSelect->hide();
        ui->label_Difficulty->hide();
        ui->dateEdit_QSelect->show();
        ui->toolButton_Q->show();
        break;
    case 4:
        ui->label_QSelect->setText("题目难度为:");
        ui->lineEdit_QSelect->hide();
        ui->dateEdit_QSelect->hide();
        ui->toolButton_Q->hide();
        ui->comboBox_QType->show();
        ui->label_Difficulty->show();
        ui->comboBox_QType->clear();
        ui->comboBox_QType->addItem("0");
        ui->comboBox_QType->addItem("1");
        ui->comboBox_QType->addItem("2");
        break;
    case 5:
        ui->label_QSelect->setText("关键字为：");
        ui->comboBox_QType->hide();
        ui->label_Difficulty->hide();
        ui->dateEdit_QSelect->hide();
        ui->toolButton_Q->hide();
        ui->lineEdit_QSelect->show();
        ui->lineEdit_QSelect->clear();
        break;
    }
    arg1.size();
}

/********************************************************************
 * 功能描述：试题更新显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5          V1.0a     Jiangs111      试题更新显示
 ********************************************************************/
void ManageForm::reviewQuestion()
{
    QString sql;
        switch(ui->comboBox__QSelect->currentIndex())
        {
        case 0:
            sql="select QuestionId as 试题号, Name as 试题名, TypeID as 试题类型, Creator as 出题人, CreateTime as 出题时间, Difficulty as 难度, KeyWord as 关键字 from Question_Info where Name like '%"
                    + ui->lineEdit_QSelect->text() + "%'";
            break;
        case 1:
            sql="select QuestionId as 试题号, Name as 试题名, TypeID as 试题类型, Creator as 出题人, CreateTime as 出题时间, Difficulty as 难度, KeyWord as 关键字 from Question_Info where TypeID="
                        + m_typeIds[ui->comboBox_QType->currentIndex()];
            break;
        case 2:
            sql="select QuestionId as 试题号, Name as 试题名, TypeID as 试题类型, Creator as 出题人, CreateTime as 出题时间, Difficulty as 难度, KeyWord as 关键字 from Question_Info where Creator='"
                    + ui->lineEdit_QSelect->text() + "'";
            break;
        case 3:
            sql="select QuestionId as 试题号, Name as 试题名, TypeID as 试题类型, Creator as 出题人, CreateTime as 出题时间, Difficulty as 难度, KeyWord as 关键字 from Question_Info where CreateTime='"
                    + ui->dateEdit_QSelect->text() + "'";
            break;
        case 4:
            sql="select QuestionId as 试题号, Name as 试题名, TypeID as 试题类型, Creator as 出题人, CreateTime as 出题时间, Difficulty as 难度, KeyWord as 关键字 from Question_Info where Difficulty='"
                    + ui->comboBox_QType->currentText() + "'";
            break;
        case 5:
            sql="select QuestionId as 试题号, Name as 试题名, TypeID as 试题类型, Creator as 出题人, CreateTime as 出题时间, Difficulty as 难度, KeyWord as 关键字 from Question_Info where KeyWord='"
                    + ui->lineEdit_QSelect->text() + "'";
            break;
        }
        //qDebug() << sql;
        QSqlDatabase database;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
            database = QSqlDatabase::database("qt_sql_default_connection");
        else
            database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(m_activeDb);

        QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_Q);
        model->setQuery(QSqlQuery(sql, database));
        ui->tableView_Q->setModel(model);
        ui->tableView_Q->resizeColumnsToContents();
        ui->label_QCount->setText("共显示出" + QString::number(model->rowCount()) + "条试题。"
                                  + "\n\n注意：试题类型：1-判断题，2-单选题，"
                                  + "\n3-多选题，4-填空题，5-简答题");

        database.close();
        QSqlDatabase::removeDatabase(m_activeDb);
}

/********************************************************************
 * 功能描述：试题浏览按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5           V1.0a     Jiangs111     浏览考生菜单显示
 ********************************************************************/
void ManageForm::on_pushButton_QSelect_clicked()
{
    reviewQuestion();
}

/********************************************************************
 * 功能描述：添加试题按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5           V1.0a     Jiangs111     添加试题
 ********************************************************************/
void ManageForm::on_pushButton_QAdd_clicked()
{
    if(m_typenames.size() == 0)
    {
        m_typenames = getQType();
    }
    //qDebug() << m_typenames.size();
    QuestionForm *qf = new QuestionForm(0, "", m_typenames);
    qf->setActivedb(m_activeDb);
    qf->show();
}

/********************************************************************
 * 功能描述：试题类型
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QStringList *
 * 其他说明：无
 * 完成日期：2016.1.5  版本号     修改人         修改内容
 * 2016.1.5           V1.0a     Jiangs111     试题类型
 ********************************************************************/
QStringList ManageForm::getQType()
{
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);

    QString sql="select TypeID, TypeName from Paper_Question_type";
    QStringList ql;
    QSqlQuery query(sql, database);
    while (query.next()) {
            m_typeIds << query.value(0).toString();
            ql << query.value(1).toString();
            //qDebug() << country;
    }
    //qDebug() << sql;

    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);

    return ql;
}

/********************************************************************
 * 功能描述：删除试题按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.11  版本号     修改人         修改内容
 * 2016.1.11           V1.0a     Jiangs111     删除试题按钮
 ********************************************************************/
void ManageForm::on_pushButton_QDel_clicked()
{
    QModelIndexList selection = ui->tableView_Q->selectionModel()->selectedRows(0);
    if (!selection.empty())
    {
        QModelIndex idIndex = selection.at(0);
        //int id = idIndex.data().toInt();
        QString questionid = idIndex.sibling(idIndex.row(), 0).data().toString();
        QString name = idIndex.sibling(idIndex.row(), 1).data().toString();
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,tr("删除试题"),QString(tr("确认删除试题号为'%1'的'%2'试题吗？").arg(questionid).arg(name)),QMessageBox::Yes|QMessageBox::No);

        if(button == QMessageBox::Yes)
        {
            if(delQuestion(questionid))
               QMessageBox::information(this, tr("删除试题"),tr("考生试题成功删除......"));
            reviewQuestion();
        }else{
            QMessageBox::information(this, tr("删除试题"),tr("删除已取消......"));
        }
    }
    else
        QMessageBox::warning(this, tr("删除试题"), tr("请选择要删除的......"));
}

/********************************************************************
 * 功能描述：删除试题
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.11  版本号     修改人         修改内容
 * 2016.1.11           V1.0a     Jiangs111     删除试题
 ********************************************************************/
bool ManageForm::delQuestion(QString tid)
{
    QString sql = "delete from Question_Info where QuestionId = '" + tid +"'";
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
        QMessageBox::warning(this, tr("无法打开数据库"), tr("删除试题出错......") + err.text());
        return false;
    }
    else
        return true;
}

/********************************************************************
 * 功能描述：修改试题
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.11  版本号     修改人         修改内容
 * 2016.1.11           V1.0a     Jiangs111     修改试题
 ********************************************************************/
void ManageForm::on_pushButton_QEdit_clicked()
{
    QModelIndexList selection = ui->tableView_Q->selectionModel()->selectedRows(0);
    if (!selection.empty())
    {
        QModelIndex idIndex = selection.at(0);
        //int id = idIndex.data().toInt();
        QString questionid = idIndex.sibling(idIndex.row(), 0).data().toString();
        QString name = idIndex.sibling(idIndex.row(), 1).data().toString();
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,tr("修改试题"),QString(tr("确认修改试题号为'%1'的'%2'试题吗？").arg(questionid).arg(name)),QMessageBox::Yes|QMessageBox::No);

        if(button == QMessageBox::Yes)
        {
            if(m_typenames.size() == 0)
            {
                m_typenames = getQType();
            }
            //qDebug() << m_typenames.size();
            QuestionForm *qf = new QuestionForm(0, questionid, m_typenames);
            qf->setActivedb(m_activeDb);
            qf->show();
            reviewQuestion();
        }else{
            QMessageBox::information(this, tr("修改试题"),tr("修改已取消......"));
        }
    }
    else
        QMessageBox::warning(this, tr("修改试题"), tr("请选择要修改的试题......"));
}

/********************************************************************
 * 功能描述：导入试题按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.12  版本号     修改人         修改内容
 * 2016.1.12           V1.0a     Jiangs111     显示导入试题
 ********************************************************************/
void ManageForm::on_pushButton_QImport_clicked()
{
    ImportQuestion *iq = new ImportQuestion();
    iq->setActivedb(m_activeDb);
    iq->show();
}

/********************************************************************
 * 功能描述：题库试卷浏览菜单
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.21  版本号     修改人         修改内容
 * 2016.1.21           V1.0a     Jiangs111     题库试卷浏览菜单
 ********************************************************************/
void ManageForm::on_action_Paperbank_triggered()
{
    clearshow();
    ui->groupBox_Paperbank->show();
}

/********************************************************************
 * 功能描述：下拉试卷列表修改标签
 *
 * 输入参数：const QString &arg1
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.21  版本号     修改人         修改内容
 * 2016.1.21           V1.0a     Jiangs111     修改试卷标签显示
 ********************************************************************/
void ManageForm::on_comboBox__PBSelect_currentIndexChanged(const QString &arg1)
{
    switch(ui->comboBox__PBSelect->currentIndex())
    {
    case 0:
        ui->comboBox_PBType->hide();
        ui->toolButton_PB->hide();
        ui->dateEdit_PBSelect->hide();
        ui->calendarWidget_PB->hide();
        ui->label_PBSelect->setText("试卷名称为:");
        ui->lineEdit_PBSelect->show();
        ui->lineEdit_PBSelect->clear();
        break;
    case 1:
        ui->toolButton_PB->hide();
        ui->dateEdit_PBSelect->hide();
        ui->lineEdit_PBSelect->hide();
        ui->calendarWidget_PB->hide();
        ui->label_PBSelect->setText("是否含主观题:");
        ui->comboBox_PBType->clear();
        ui->comboBox_PBType->addItem("否");
        ui->comboBox_PBType->addItem("是");
        ui->comboBox_PBType->show();
        break;
    case 2:
        ui->comboBox_PBType->hide();
        ui->toolButton_PB->hide();
        ui->dateEdit_PBSelect->hide();
        ui->calendarWidget_PB->hide();
        ui->label_PBSelect->setText("出卷人为:");
        ui->lineEdit_PBSelect->show();
        ui->lineEdit_PBSelect->clear();
        break;
    case 3:
        ui->comboBox_PBType->hide();
        ui->toolButton_PB->hide();
        ui->lineEdit_PBSelect->hide();
        ui->label_PBSelect->setText("出卷时间为:");
        ui->dateEdit_PBSelect->show();
        ui->toolButton_PB->show();
        break;
    case 4:
        ui->toolButton_PB->hide();
        ui->dateEdit_PBSelect->hide();
        ui->lineEdit_PBSelect->hide();
        ui->calendarWidget_PB->hide();
        ui->label_PBSelect->setText("试卷用途为:");
        ui->comboBox_PBType->clear();
        ui->comboBox_PBType->addItem("考试卷");
        ui->comboBox_PBType->addItem("练习卷");
        ui->comboBox_PBType->show();
        break;
    }
    arg1.size();
}

/********************************************************************
 * 功能描述：试卷显示日期面板
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.22  版本号     修改人         修改内容
 * 2016.1.22           V1.0a     Jiangs111     显示日期面板
 ********************************************************************/
void ManageForm::on_toolButton_PB_clicked()
{
    ui->calendarWidget_PB->show();
}

/********************************************************************
 * 功能描述：试卷日期面板选择日期
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.22  版本号     修改人         修改内容
 * 2016.1.22           V1.0a     Jiangs111     日期面板选择日期
 ********************************************************************/
void ManageForm::on_calendarWidget_PB_selectionChanged()
{
    ui->dateEdit_PBSelect->setDate(ui->calendarWidget_PB->selectedDate());
    ui->calendarWidget_PB->hide();
}

/********************************************************************
 * 功能描述：试题显示日期面板
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.22  版本号     修改人         修改内容
 * 2016.1.22           V1.0a     Jiangs111     显示日期面板
 ********************************************************************/
void ManageForm::on_toolButton_Q_clicked()
{
    ui->calendarWidget_Q->show();
}

/********************************************************************
 * 功能描述：试题日期面板选择日期
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.22  版本号     修改人         修改内容
 * 2016.1.22           V1.0a     Jiangs111     日期面板选择日期
 ********************************************************************/
void ManageForm::on_calendarWidget_Q_selectionChanged()
{
    ui->dateEdit_QSelect->setDate(ui->calendarWidget_Q->selectedDate());
    ui->calendarWidget_Q->hide();
}

/********************************************************************
 * 功能描述：试卷浏览按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.22  版本号     修改人         修改内容
 * 2016.1.22           V1.0a     Jiangs111     试卷浏览按钮
 ********************************************************************/
void ManageForm::on_pushButton_PBSelect_clicked()
{
    reviewpaperbank();
}

/********************************************************************
 * 功能描述：试卷更新显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.22  版本号     修改人         修改内容
 * 2016.1.22          V1.0a     Jiangs111      试题更新显示
 ********************************************************************/
void ManageForm::reviewpaperbank()
{
    QString sql;
    switch(ui->comboBox__PBSelect->currentIndex())
    {
    case 0:
        sql = "select PaperId as 试卷号, PaperName as 试卷名称, Duration as 考试时长, "
              "Status as 试卷状态, Type as 试卷类型, IsTest as 试卷用途, Creator as 出卷人, "
                "CreateTime as 出卷时间 from Paper_Info where PaperName like '%"
                + ui->lineEdit_PBSelect->text() + "%'";
        break;
    case 1:
        sql = "select PaperId as 试卷号, PaperName as 试卷名称, Duration as 考试时长, "
              "Status as 试卷状态, Type as 试卷类型, IsTest as 试卷用途, Creator as 出卷人, "
                "CreateTime as 出卷时间 from Paper_Info where IsSubjective = "
                + QString::number(ui->comboBox_PBType->currentIndex());
        break;
    case 2:
        sql = "select PaperId as 试卷号, PaperName as 试卷名称, Duration as 考试时长, "
              "Status as 试卷状态, Type as 试卷类型, IsTest as 试卷用途, Creator as 出卷人, "
                "CreateTime as 出卷时间 from Paper_Info where Creator = '"
                + ui->lineEdit_PBSelect->text() + "'";
        break;
    case 3:
        sql = "select PaperId as 试卷号, PaperName as 试卷名称, Duration as 考试时长, "
              "Status as 试卷状态, Type as 试卷类型, IsTest as 试卷用途, Creator as 出卷人, "
                "CreateTime as 出卷时间 from Paper_Info where CreateTime = '"
                + ui->dateEdit_PBSelect->text() + "'";
        break;
    case 4:
        sql = "select PaperId as 试卷号, PaperName as 试卷名称, Duration as 考试时长, "
              "Status as 试卷状态, Type as 试卷类型, IsTest as 试卷用途, Creator as 出卷人, "
                "CreateTime as 出卷时间 from Paper_Info where IsTest = "
                + QString::number(ui->comboBox_PBType->currentIndex());
        break;
    }
    //qDebug() << sql;
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);

    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_PB);
    model->setQuery(QSqlQuery(sql, database));
    ui->tableView_PB->setModel(model);
    ui->tableView_PB->resizeColumnsToContents();
    ui->label_PBCount->setText("共显示出" + QString::number(model->rowCount()) + "套试卷。");

    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);
}
