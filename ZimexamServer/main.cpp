/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：main.cpp
 * 文件标识：无
 * 内容摘要：考试系统主函数源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.7
 *
 ********************************************************************/
#include "loginserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginServer w;
    w.show();

    return a.exec();
}
