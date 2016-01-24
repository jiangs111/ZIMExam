/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：importquestion.h
 * 文件标识：无
 * 内容摘要：考试系统导入试题头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2016.1.11
 *
 ********************************************************************/
#ifndef IMPORTQUESTION_H
#define IMPORTQUESTION_H

#include <QWidget>

namespace Ui {
class ImportQuestion;
}

class ImportQuestion : public QWidget
{
    Q_OBJECT
    Q_ENUMS(colNum)

public:
    explicit ImportQuestion(QWidget *parent = 0);
    ~ImportQuestion();
    void setActivedb(QString db);       //设置连接数据库
    QString getActivedb();              //获得连接数据库
    enum colNum{cols_num = 8};

private slots:
    void on_pushButton_Exit_clicked();  //退出按钮
    void on_pushButton_File_clicked();  //打开按钮
    void on_toolButton_Add_clicked();   //添加按钮
    void on_toolButton_Clear_clicked(); //重排按钮
    void on_pushButton_Ok_clicked();    //导入按钮

private:
    Ui::ImportQuestion *ui;
    QString m_activeDb;
    int m_titlenum[cols_num];
    int m_tempnum[cols_num];

};

#endif // IMPORTQUESTION_H
