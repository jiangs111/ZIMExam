/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：QuestionForm.h
 * 文件标识：无
 * 内容摘要：考试系统教师添加修改试题头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2016.1.7
 *
 ********************************************************************/
#ifndef QUESTIONFORM_H
#define QUESTIONFORM_H

#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include <poppler/poppler-qt5.h>

namespace Ui {
class QuestionForm;
}

class QuestionForm : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionForm(QWidget *parent = 0);
    explicit QuestionForm(QWidget *parent, QString id, QStringList tn);
    ~QuestionForm();
    void setActivedb(QString db);       //设置连接数据库
    QString getActivedb();              //获得连接数据库

private slots:
    void on_pushButton_Exit_clicked();  //退出按钮
    void on_pushButton_Ok_clicked();    //保存按钮
    void on_comboBox_Type_currentIndexChanged(const QString &arg1);//题型选择
    void on_pushButton_yes_clicked();   //填空框确认
    void on_pushButton_no_clicked();    //重置填空框
    void on_pushButton_Clear_clicked(); //清空非文本输入
    void on_checkBox_IsText_clicked();  //是否为文本试题

private:
    Ui::QuestionForm *ui;
    QString m_activeDb;
    QStringList m_typenames;
    QString m_questionid;
    QString m_typeids;
    Poppler::Document* doc;

    void clearAnswer();     //清除答案
    bool checkBlank();      //查找空白
    bool addQuestion();     //添加试题
    void loadQuestion();    //读取要修改的试题
    void showBlank();       //显示填空项
    bool editQuestion();    //修改试题
    QString getAnswer();    //获取试题答案
};

#endif // QUESTIONFORM_H
