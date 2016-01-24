/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：helpform.cpp
 * 文件标识：无
 * 内容摘要：考试系统帮助文档源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.10
 *
 ********************************************************************/
#include "assistant.h"
#include <QtCore/QByteArray>
#include <QtCore/QProcess>
#include <QMessageBox>
#include <QDebug>

Assistant::Assistant()
    : proc(0)
{
}

/********************************************************************
 * 功能描述：析构关闭进程
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     关闭销毁进程
 ********************************************************************/
Assistant::~Assistant()
{
    if (proc && proc->state() == QProcess::Running) {
        // 试图终止进程
        proc->terminate();
        proc->waitForFinished(3000);
    }
    // 销毁proc
    delete proc;
}

/********************************************************************
 * 功能描述：显示默认文档
 *
 * 输入参数：const QString &page
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     设置显示默认文档
 ********************************************************************/
void Assistant::showDocumentation(const QString &page)
{
    if (!startAssistant())
        return;

    QByteArray ba("SetSource ");
    ba.append("./qch/");

    proc->write(ba + page.toLocal8Bit() + '\n');
    qDebug() << ba;
}

/********************************************************************
 * 功能描述：启动帮助
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：bool
 * 其他说明：无
 * 完成日期：2015.12.11  版本号     修改人         修改内容
 * 2015.12.11           V1.0a     Jiangs111     启动Qt Assistant
 ********************************************************************/
bool Assistant::startAssistant()
{
    // 如果没有创建进程，则新创建一个
    if (!proc)
        proc = new QProcess();

    // 如果进程没有运行，则运行assistant，并添加参数
    if (proc->state() != QProcess::Running) {
        QString app = QLatin1String("./qch/assistant.exe");
        QStringList args;
        args << QLatin1String("-collectionFile")
            << QLatin1String("./qch/help.qhc");

        proc->start(app, args);

        if (!proc->waitForStarted()) {
            QMessageBox::critical(0, QObject::tr("helpDoc"),
                QObject::tr("无法打开帮助文档 (%1)").arg(app));
            return false;
        }
    }
    return true;
}

