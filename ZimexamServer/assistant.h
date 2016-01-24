/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：helpform.h
 * 文件标识：无
 * 内容摘要：考试系统帮助文档头文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.10
 *
 ********************************************************************/
#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QtCore/QString>

class QProcess;

class Assistant
{
public:
    Assistant();
    ~Assistant();
    void showDocumentation(const QString &file);    //设置首文档

private:
    bool startAssistant();  //打开帮助文档
    QProcess *proc;
};

#endif // ASSISTANT_H
