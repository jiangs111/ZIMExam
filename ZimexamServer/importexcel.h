/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：importexcel.h
 * 文件标识：无
 * 内容摘要：考试系统导入Excel头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.22
 *
 ********************************************************************/
#ifndef IMPORTEXCEL_H
#define IMPORTEXCEL_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class Importexcel;
}

class Importexcel : public QWidget
{
    Q_OBJECT

public:
    explicit Importexcel(QWidget *parent = 0);
    ~Importexcel();
    void setActivedb(QString db);       //设置连接数据库
    QString getActivedb();              //获得连接数据库

private slots:
    void on_pushButton_File_clicked();  //打开文件按钮
    void on_pushButton_Exit_clicked();  //退出按钮
    void on_pushButton_Ok_clicked();    //导入按钮
    void on_toolButton_Add_clicked();   //设置数据列名
    void on_toolButton_Clear_clicked(); //重排数据列名

private:
    Ui::Importexcel *ui;
    QString m_activeDb;
};

#endif // IMPORTEXCEL_H
