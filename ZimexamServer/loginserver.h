/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：loginserver.h
 * 文件标识：无
 * 内容摘要：考试系统教师登录界面头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.7
 *
 ********************************************************************/
#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class loginServer;
}

class loginServer : public QWidget
{
    Q_OBJECT

public:
    explicit loginServer(QWidget *parent = 0);
    ~loginServer();

private slots:
    void on_pushButton_Ok_clicked();    //登录系统按钮

private:
    Ui::loginServer *ui;
    QString m_activeDb;

    bool open_data();   //连接数据库
};

#endif // LOGINSERVER_H
