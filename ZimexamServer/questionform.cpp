/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：Questionform.cpp
 * 文件标识：无
 * 内容摘要：考试系统教师添加修改试题源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2016.1.7
 *
 ********************************************************************/
#include "questionform.h"
#include "manageform.h"
#include "ui_questionform.h"
#include <QtWidgets>
#include <QtSql>
#include <QException>
#include <QDebug>
#include <QButtonGroup>
#include <QDate>
#include <QPrinter>
#include <QPalette>

QuestionForm::QuestionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionForm)
{
    ui->setupUi(this);
    ui->lineEdit_Name->setMaxLength(10);

}

QuestionForm::QuestionForm(QWidget *parent, QString id, QStringList tn):
    QWidget(parent),
    ui(new Ui::QuestionForm),
    m_questionid(id),
    m_typenames(tn)
{
    ui->setupUi(this);
    ui->lineEdit_Name->setMaxLength(10);
    ui->graphicsView->hide();

    for(int i = 0; i < m_typenames.size(); i++)
    {
        ui->comboBox_Type->addItem(m_typenames[i]);
    }

    if(m_questionid != "")
        loadQuestion();
}

QuestionForm::~QuestionForm()
{
    delete ui;
}

/********************************************************************
 * 功能描述：获得连接数据库
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QString
 * 其他说明：无
 * 完成日期：2016.1.7  版本号     修改人         修改内容
 * 2016.1.7           V1.0a     Jiangs111     获得连接数据库
 ********************************************************************/
QString QuestionForm::getActivedb()
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
 * 完成日期：2016.1.7  版本号     修改人         修改内容
 * 2016.1.7           V1.0a     Jiangs111     设置连接数据库
 ********************************************************************/
void QuestionForm::setActivedb(QString db)
{
    m_activeDb = db;
    //m_typenames = tn;
}

/********************************************************************
 * 功能描述：退出按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.7  版本号     修改人         修改内容
 * 2016.1.7           V1.0a     Jiangs111     关闭窗口
 ********************************************************************/
void QuestionForm::on_pushButton_Exit_clicked()
{
    this->close();
}

/********************************************************************
 * 功能描述：添加保存按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.7  版本号     修改人         修改内容
 * 2016.1.7           V1.0a     Jiangs111     保存新试题
 ********************************************************************/
void QuestionForm::on_pushButton_Ok_clicked()
{
    if(ui->lineEdit_Name->text() == "")
        QMessageBox::warning(this, tr("输入错误"), tr("请输入试题名称......"));
    else if(ui->textEdit_Content->toPlainText() == "")
        QMessageBox::warning(this, tr("输入错误"), tr("请输入试题题干......"));
    else if(ui->comboBox_Type->currentText() == "")
        QMessageBox::warning(this, tr("输入错误"), tr("请选择题目类型......"));
    else if(ui->lineEdit_Creator->text() == "")
        QMessageBox::warning(this, tr("输入错误"), tr("请输入出题人......"));
    else if(ui->comboBox_Difficulty->currentText() == "")
        QMessageBox::warning(this, tr("输入错误"), tr("请选择题目难度......"));
    else if(!checkBlank())
        QMessageBox::warning(this, tr("输入错误"), tr("请填写试题答案......"));
    else
    {
        if(ui->checkBox_IsText->isChecked())
        {
            QMessageBox::StandardButton button;
            button = QMessageBox::question(this,tr("保存纯文本题干"),tr("注意:保存文本题干时,一些图片、公式编辑器均会被清除,确认保存吗？"), QMessageBox::Yes|QMessageBox::No);
            if(button == QMessageBox::No)
                return;
        }
        if(m_questionid != "")
        {
            if(editQuestion())
                QMessageBox::information(this, tr("保存成功"), tr("修改试题:'")
                                     + ui->lineEdit_Name->text() + "'成功!");
        }
        else
        {
            if(addQuestion())
                QMessageBox::information(this, tr("保存成功"), tr("添加试题:'")
                                     + ui->lineEdit_Name->text() + "'成功!");
        }
        this->close();
    }

}

/********************************************************************
 * 功能描述：下拉试题类型列表
 *
 * 输入参数：const QString &arg1
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.7  版本号     修改人         修改内容
 * 2015.1.7           V1.0a     Jiangs111     下拉试题类型显示
 ********************************************************************/
void QuestionForm::on_comboBox_Type_currentIndexChanged(const QString &arg1)
{
    switch(ui->comboBox_Type->currentIndex())
    {
    case 0:
        break;
    default:
        m_typeids = QString::number(ui->comboBox_Type->currentIndex());
        if(m_typenames[ui->comboBox_Type->currentIndex() - 1] == "判断题")
        {
            clearAnswer();
            ui->groupBox_Bool->show();
        }
        else if(m_typenames[ui->comboBox_Type->currentIndex() - 1] == "单选题")
        {
            clearAnswer();
            ui->groupBox_Single->show();
        }
        else if(m_typenames[ui->comboBox_Type->currentIndex() - 1] == "多选题")
        {
            clearAnswer();
            ui->groupBox_Multi->show();
        }
        else if(m_typenames[ui->comboBox_Type->currentIndex() - 1] == "填空题")
        {
            clearAnswer();
            ui->groupBox_Blank->show();
        }
        else if(m_typenames[ui->comboBox_Type->currentIndex() - 1] == "简答题")
        {
            clearAnswer();
            ui->textEdit_Answer->show();
        }
        else
        {
            clearAnswer();
        }
    }
    arg1.size();
}

/********************************************************************
 * 功能描述：试题答案不显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.8  版本号     修改人         修改内容
 * 2015.1.8           V1.0a     Jiangs111     试题答案不显示
 ********************************************************************/
void QuestionForm::clearAnswer()
{
    ui->groupBox_Bool->hide();
    ui->groupBox_Single->hide();
    ui->groupBox_Multi->hide();
    ui->groupBox_Blank->hide();
    ui->lineEdit_1->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();
    ui->label_1->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->textEdit_Answer->hide();
    ui->lineEdit_1->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->textEdit_Answer->clear();
    ui->checkBox_A->setChecked(false);
    ui->checkBox_B->setChecked(false);
    ui->checkBox_C->setChecked(false);
    ui->checkBox_D->setChecked(false);
    ui->checkBox_E->setChecked(false);
    ui->radioButton_A->setChecked(false);
    ui->radioButton_B->setChecked(false);
    ui->radioButton_C->setChecked(false);
    ui->radioButton_D->setChecked(false);
    ui->radioButton_E->setChecked(false);
    ui->radioButton_Yes->setChecked(false);
    ui->radioButton_No->setChecked(false);
}

/********************************************************************
 * 功能描述：填空题项显示按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.8  版本号     修改人         修改内容
 * 2015.1.8           V1.0a     Jiangs111     填空题显示按钮
 ********************************************************************/
void QuestionForm::on_pushButton_yes_clicked()
{
    showBlank();
}

/********************************************************************
 * 功能描述：填空题项显示
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.8  版本号     修改人         修改内容
 * 2015.1.8           V1.0a     Jiangs111     填空题需填空项显示
 ********************************************************************/
void QuestionForm::showBlank()
{
    if(ui->comboBox_Num->currentIndex() == 0)
    {
        ui->lineEdit_1->show();
        ui->label_1->show();
    }
    else if(ui->comboBox_Num->currentIndex() == 1)
    {
        ui->lineEdit_2->show();
        ui->label_2->show();
        ui->lineEdit_1->show();
        ui->label_1->show();
    }
    else if(ui->comboBox_Num->currentIndex() == 2)
    {
        ui->lineEdit_3->show();
        ui->label_3->show();
        ui->lineEdit_2->show();
        ui->label_2->show();
        ui->lineEdit_1->show();
        ui->label_1->show();
    }
    else if(ui->comboBox_Num->currentIndex() == 3)
    {
        ui->lineEdit_4->show();
        ui->label_4->show();
        ui->lineEdit_3->show();
        ui->label_3->show();
        ui->lineEdit_2->show();
        ui->label_2->show();
        ui->lineEdit_1->show();
        ui->label_1->show();
    }
}

/********************************************************************
 * 功能描述：填空题项重填
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.8  版本号     修改人         修改内容
 * 2015.1.8           V1.0a     Jiangs111     填空题项重填
 ********************************************************************/
void QuestionForm::on_pushButton_no_clicked()
{
    ui->lineEdit_1->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();
    ui->label_1->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->lineEdit_1->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

/********************************************************************
 * 功能描述：试题答案是否
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.8  版本号     修改人         修改内容
 * 2015.1.8           V1.0a     Jiangs111     填空题项重填
 ********************************************************************/
bool QuestionForm::checkBlank()
{
    if(ui->groupBox_Bool->isVisible())
    {
        if(ui->radioButton_Yes->isChecked())
            return true;
        if(ui->radioButton_No->isChecked())
            return true;
    }
    if(ui->groupBox_Single->isVisible())
    {
        if(ui->radioButton_A->isChecked())
            return true;
        if(ui->radioButton_B->isChecked())
            return true;
        if(ui->radioButton_C->isChecked())
            return true;
        if(ui->radioButton_D->isChecked())
            return true;
        if(ui->radioButton_E->isChecked())
            return true;
    }
    if(ui->checkBox_A->isChecked())
        return true;
    if(ui->checkBox_B->isChecked())
        return true;
    if(ui->checkBox_C->isChecked())
        return true;
    if(ui->checkBox_D->isChecked())
        return true;
    if(ui->checkBox_E->isChecked())
        return true;
    if(ui->lineEdit_1->text() != "")
        return true;
    if(ui->lineEdit_2->text() != "")
        return true;
    if(ui->lineEdit_3->text() != "")
        return true;
    if(ui->lineEdit_4->text() != "")
        return true;
    if(ui->textEdit_Answer->toPlainText() != "")
        return true;

    return false;
}

/********************************************************************
 * 功能描述：添加新试题
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：bool
 * 其他说明：无
 * 完成日期：2016.1.8  版本号     修改人         修改内容
 * 2015.1.8           V1.0a     Jiangs111     添加新试题
 ********************************************************************/
bool QuestionForm::addQuestion()
{
    QString answer = getAnswer(), answercols = "1";
    if(ui->comboBox_Type->currentIndex() == 4)
      answercols = ui->comboBox_Num->currentText();
    QString sql;
    if(ui->checkBox_IsText->isChecked())
    {
        sql = "INSERT INTO Question_Info (Name, Content, TypeID, Creator, "
              "CreateTime, LastModify, Answer, ExposeTimes, RightTimes, "
              "WrongTimes, Difficulty, Keyword, AnswerCols,IsText) VALUES ('"
                + ui->lineEdit_Name->text() + "', '" + ui->textEdit_Content->toPlainText()
                + "', " + QString::number(ui->comboBox_Type->currentIndex()) + ", '"
                + ui->lineEdit_Creator->text() + "', '" + QDate::currentDate().toString(Qt::ISODate)
                + "', '" + QDate::currentDate().toString(Qt::ISODate) + "', '" + answer + "', 0, 0, 0,"
                + ui->comboBox_Difficulty->currentText() + ", '" + ui->lineEdit_Keyword->text()
                + "', " + answercols +",0)";
    }
    else
    {
        QString fp = ".//addin//"+QDateTime::currentDateTime().toString(Qt::ISODate) + ".pdf";
        fp.replace(":","");
        QPrinter printer_html;
        printer_html.setPageSize(QPrinter::A4);
        printer_html.setOutputFormat(QPrinter::PdfFormat);
        printer_html.setOutputFileName(fp);
        printer_html.setPageMargins(3, 3, 3, 3, QPrinter::Millimeter);
        QTextDocument textDocument;
        textDocument.setHtml(ui->textEdit_Content->toHtml());
        textDocument.setPageSize(QSizeF(printer_html.pageRect().size()));
        textDocument.print(&printer_html);
        textDocument.end();
        sql = "INSERT INTO Question_Info (Name, Content, TypeID, Creator, "
              "CreateTime, LastModify, Answer, ExposeTimes, RightTimes, "
              "WrongTimes, Difficulty, Keyword, AnswerCols,IsText) VALUES ('"
                + ui->lineEdit_Name->text() + "', '" + fp
                + "', " + QString::number(ui->comboBox_Type->currentIndex()) + ", '"
                + ui->lineEdit_Creator->text() + "', '" + QDate::currentDate().toString(Qt::ISODate)
                + "', '" + QDate::currentDate().toString(Qt::ISODate) + "', '" + answer + "', 0, 0, 0,"
                + ui->comboBox_Difficulty->currentText() + ", '" + ui->lineEdit_Keyword->text()
                + "', " + answercols +",1)";
    }
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
        QMessageBox::warning(this, tr("无法打开数据库"), tr("添加试题出错......") + err.text());
        return false;
    }
    else
        return true;
}

/********************************************************************
 * 功能描述：读取试题答案
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QString
 * 其他说明：无
 * 完成日期：2016.1.11  版本号     修改人         修改内容
 * 2016.1.11           V1.0a     Jiangs111     读取试题答案
 ********************************************************************/
QString QuestionForm::getAnswer()
{
    QString answer = "";
    if(ui->comboBox_Type->currentIndex() == 1)
    {
        if(ui->radioButton_Yes->isChecked())
            answer = ui->radioButton_Yes->text();
        else if(ui->radioButton_No->isChecked())
            answer = ui->radioButton_No->text();
        else
        {
            QMessageBox::warning(this, tr("输入错误"), tr("请填写试题答案......"));
        }
    }
    else if(ui->comboBox_Type->currentIndex() == 2)
    {
        if(ui->radioButton_A->isChecked())
            answer = ui->radioButton_A->text();
        else if(ui->radioButton_B->isChecked())
            answer = ui->radioButton_B->text();
        else if(ui->radioButton_C->isChecked())
            answer = ui->radioButton_C->text();
        else if(ui->radioButton_D->isChecked())
            answer = ui->radioButton_D->text();
        else if(ui->radioButton_E->isChecked())
            answer = ui->radioButton_E->text();
        else
        {
            QMessageBox::warning(this, tr("输入错误"), tr("请填写试题答案......"));
        }
    }
    else if(ui->comboBox_Type->currentIndex() == 3)
    {
        if(ui->checkBox_A->isChecked())
            answer += ui->checkBox_A->text() + ";";
        if(ui->checkBox_B->isChecked())
            answer += ui->checkBox_B->text() + ";";
        if(ui->checkBox_C->isChecked())
            answer += ui->checkBox_C->text() + ";";
        if(ui->checkBox_D->isChecked())
            answer += ui->checkBox_D->text() + ";";
        if(ui->checkBox_E->isChecked())
            answer += ui->checkBox_E->text() + ";";
        if(answer == "")
        {
            QMessageBox::warning(this, tr("输入错误"), tr("请填写试题答案......"));
        }
    }
    else if(ui->comboBox_Type->currentIndex() == 4)
    {
        if(ui->lineEdit_1->text() != "")
            answer += ui->lineEdit_1->text() + ";";
        if(ui->lineEdit_2->text() != "")
            answer += ui->lineEdit_2->text() + ";";
        if(ui->lineEdit_3->text() != "")
            answer += ui->lineEdit_3->text() + ";";
        if(ui->lineEdit_4->text() != "")
            answer += ui->lineEdit_4->text() + ";";
        if(answer == "")
        {
            QMessageBox::warning(this, tr("输入错误"), tr("请填写试题答案......"));
        }
    }
    else if(ui->comboBox_Type->currentIndex() == 5)
    {
        if(ui->textEdit_Answer->toPlainText() != "")
            answer = ui->textEdit_Answer->toPlainText();
        else
        {
            QMessageBox::warning(this, tr("输入错误"), tr("请填写试题答案......"));
        }
    }
    else
    {
        QMessageBox::warning(this, tr("输入错误"), tr("请填写试题答案......"));
    }
    return answer;
}

/********************************************************************
 * 功能描述：读取要修改的试题
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.11  版本号     修改人         修改内容
 * 2016.1.11           V1.0a     Jiangs111     读取要修改的试题信息
 ********************************************************************/
void QuestionForm::loadQuestion()
{
    QString sql;
    sql = "select Name, Content, Creator, TypeId, Answer, AnswerCols, Difficulty, Keyword, IsText from Question_Info where QuestionId = '" + m_questionid + "'";
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);

    QSqlQuery query(sql, database);
    QSqlRecord rec = query.record();
    int namCol = rec.indexOf("Name");
    int conCol = rec.indexOf("Content");
    int creCol = rec.indexOf("Creator");
    int typCol = rec.indexOf("TypeID");
    int ansCol = rec.indexOf("Answer");
    int ancCol = rec.indexOf("AnswerCols");
    int difCol = rec.indexOf("Difficulty");
    int keyCol = rec.indexOf("Keyword");
    int isTCol = rec.indexOf("IsText");
    while(query.next())
    {
        ui->lineEdit_Name->setText(query.value(namCol).toString());

        ui->lineEdit_Creator->setText(query.value(creCol).toString());
        ui->comboBox_Type->setCurrentIndex(query.value(typCol).toInt());
        if(ui->comboBox_Type->currentIndex() == 1)
        {
            if(query.value(ansCol).toString() == "对")
                ui->radioButton_Yes->setChecked(true);
            else if(query.value(ansCol).toString() == "错")
                ui->radioButton_No->setChecked(true);
            else
                QMessageBox::warning(this, tr("读取错误"), tr("请填写试题答案......"));
            clearAnswer();
            ui->groupBox_Bool->show();
        }
        else if(ui->comboBox_Type->currentIndex() == 2)
        {
            if(query.value(ansCol).toString() == "A")
                ui->radioButton_A->setChecked(true);
            else if(query.value(ansCol).toString() == "B")
                ui->radioButton_B->setChecked(true);
            else if(query.value(ansCol).toString() == "C")
                ui->radioButton_C->setChecked(true);
            else if(query.value(ansCol).toString() == "D")
                ui->radioButton_D->setChecked(true);
            else if(query.value(ansCol).toString() == "E")
                ui->radioButton_E->setChecked(true);
            else
                QMessageBox::warning(this, tr("读取错误"), tr("请填写试题答案......"));
            clearAnswer();
            ui->groupBox_Single->show();
        }
        else if(ui->comboBox_Type->currentIndex() == 3)
        {
            QStringList ansStr = query.value(ansCol).toString().split(";");
            clearAnswer();
            ui->groupBox_Multi->show();
            for(int i = 0; i < ansStr.size(); i++)
            {
                if(ansStr[i] == "A")
                    ui->checkBox_A->setChecked(true);
                if(ansStr[i] == "B")
                    ui->checkBox_B->setChecked(true);
                if(ansStr[i] == "C")
                    ui->checkBox_C->setChecked(true);
                if(ansStr[i] == "D")
                    ui->checkBox_D->setChecked(true);
                if(ansStr[i] == "E")
                    ui->checkBox_E->setChecked(true);
            }
        }
        else if(ui->comboBox_Type->currentIndex() == 4)
        {
            clearAnswer();
            ui->groupBox_Blank->show();
            ui->comboBox_Num->setCurrentIndex(query.value(ancCol).toInt() - 1);
            showBlank();
            QStringList ansStr = query.value(ansCol).toString().split(";");
            if(ansStr.size() != query.value(ancCol).toInt())
                QMessageBox::warning(this, tr("读取错误"), tr("请填写试题答案......"));
            else
            {
                for(int i = 0; i < ansStr.size(); i++)
                {
                    if(ansStr[i] != "")
                    {
                        if(i == 0)
                            ui->lineEdit_1->setText(ansStr[i]);
                        else if(i == 1)
                            ui->lineEdit_2->setText(ansStr[i]);
                        else if(i == 2)
                            ui->lineEdit_3->setText(ansStr[i]);
                        else if(i == 3)
                            ui->lineEdit_4->setText(ansStr[i]);
                        else
                          QMessageBox::warning(this, tr("读取错误"), tr("请填写试题答案......"));
                    }
                }
            }
        }
        else if(ui->comboBox_Type->currentIndex() == 5)
        {
            clearAnswer();
            ui->textEdit_Answer->show();
            ui->textEdit_Answer->setText(query.value(ansCol).toString());
        }
        else
        {
            QMessageBox::warning(this, tr("读取错误"), tr("请填写试题答案......"));
        }
        ui->comboBox_Difficulty->setCurrentIndex(query.value(difCol).toInt() + 1);
        ui->lineEdit_Keyword->setText(query.value(keyCol).toString());
        if(query.value(isTCol).toInt() == 0)
        {
            ui->textEdit_Content->setText(query.value(conCol).toString());
        }
        else if(query.value(isTCol).toInt() == 1)
        {
            QString path = query.value(conCol).toString();
            QFile file(path);
            if(!file.exists())
            {
                QMessageBox::warning(this, tr("读取错误"), tr("读取非文本试题题干不存在......"));
                return;
            }
            doc  = Poppler::Document::load(path);
            int index = 0;
            Poppler::Page* pdfPage = doc->page(index);  // Document starts at page 0
            if (pdfPage == 0) {
              QMessageBox::warning(this, tr("读取错误"), tr("读取非文本试题题干错误......"));
              return;
            }
            QImage image = pdfPage->renderToImage(100, 100, -1, -1, -1, -1);
            if (image.isNull()) {
              QMessageBox::warning(this, tr("读取错误"), tr("读取非文本试题题干错误......"));
              return;
            }
            QPalette   palette;
            palette.setBrush(ui->graphicsView->backgroundRole(),QBrush(image));
            ui->graphicsView->setPalette(palette);
            ui->graphicsView->show();
            ui->checkBox_IsText->setChecked(false);
            ui->textEdit_Content->setText(query.value(conCol).toString());
            ui->pushButton_Clear->setEnabled(true);
            //qDebug() << ui->textEdit_Content->toPlainText();
        }
        else
        {
            QMessageBox::warning(this, tr("读取错误"), tr("读取试题题干错误......"));
            return;
        }
    }
    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);
}

/********************************************************************
 * 功能描述：修改试题
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.11  版本号     修改人         修改内容
 * 2016.1.11           V1.0a     Jiangs111     修改的试题信息
 ********************************************************************/
bool QuestionForm::editQuestion()
{
    QString sql;
    QString answer = getAnswer(), answercols = "1";
    if(ui->comboBox_Type->currentIndex() == 4)
      answercols = ui->comboBox_Num->currentText();
    if(ui->checkBox_IsText->isChecked())
    {
        sql = "update Question_Info set Name = '"
         + ui->lineEdit_Name->text() + "', Content = '" + ui->textEdit_Content->toPlainText()
         +"', TypeID = "+ QString::number(ui->comboBox_Type->currentIndex())
         + ", Creator = '" + ui->lineEdit_Creator->text() + "', LastModify = '"
         + QDate::currentDate().toString(Qt::ISODate) + "', Answer = '"
         + answer +"', AnswerCols = " + answercols + ", Difficulty = "
         + ui->comboBox_Difficulty->currentText() + ", KeyWord = '"
         + ui->lineEdit_Keyword->text() + "', IsText = 0 where QuestionId = " + m_questionid;
    }
    else
    {
        QString fp = ".//addin//"+QDateTime::currentDateTime().toString(Qt::ISODate) + ".pdf";
        fp.replace(":","");
        QPrinter printer_html;
        printer_html.setPageSize(QPrinter::A4);
        printer_html.setOutputFormat(QPrinter::PdfFormat);
        printer_html.setOutputFileName(fp);
        printer_html.setPageMargins(3, 3, 3, 3, QPrinter::Millimeter);
        QTextDocument textDocument;
        textDocument.setHtml(ui->textEdit_Content->toHtml());
        textDocument.setPageSize(QSizeF(printer_html.pageRect().size()));
        textDocument.print(&printer_html);
        textDocument.end();

        sql = "update Question_Info set Name = '"
         + ui->lineEdit_Name->text() + "', Content = '" + fp
         +"', TypeID = "+ QString::number(ui->comboBox_Type->currentIndex())
         + ", Creator = '" + ui->lineEdit_Creator->text() + "', LastModify = '"
         + QDate::currentDate().toString(Qt::ISODate) + "', Answer = '"
         + answer +"', AnswerCols = " + answercols + ", Difficulty = "
         + ui->comboBox_Difficulty->currentText() + ", KeyWord = '"
         + ui->lineEdit_Keyword->text() + "', IsText = 1 where QuestionId = " + m_questionid;
    }
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
        QMessageBox::warning(this, tr("无法打开数据库"), tr("添加试题出错......") + err.text());
        return false;
    }
    else
        return true;
}

/********************************************************************
 * 功能描述：清空非文本题干
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.19  版本号     修改人         修改内容
 * 2016.1.19           V1.0a     Jiangs111     清空非文本题干
 ********************************************************************/
void QuestionForm::on_pushButton_Clear_clicked()
{
    if(ui->textEdit_Content->toPlainText() == "")
    {
        ui->graphicsView->hide();
        ui->textEdit_Content->show();
        return;
    }
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this,tr("修改非文本题干"),tr("若要修改非文本题干,必须先清空该题干,确认清空吗？"), QMessageBox::Yes|QMessageBox::No);

    if(button == QMessageBox::Yes)
    {
        delete doc;
        doc = NULL;
        ui->graphicsView->hide();
        QString fp = ui->textEdit_Content->toPlainText();
        //qDebug() << fp;
        if(QFile::remove(fp))
        {
            QMessageBox::information(this, tr("清空非文本题干"), tr("清空非文本题干已清空."));
        }
        else
        {
            QMessageBox::warning(this, tr("清空非文本题干"), tr("清空非文本题干出错......"));
            return;
        }
        ui->textEdit_Content->setText("");
        ui->textEdit_Content->show();
    }
}

/********************************************************************
 * 功能描述：是否为纯文本题干
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.19  版本号     修改人         修改内容
 * 2016.1.19           V1.0a     Jiangs111     是否为纯文本题干
 ********************************************************************/
void QuestionForm::on_checkBox_IsText_clicked()
{
    if(ui->checkBox_IsText->isChecked())
    {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,tr("非文本题干"),tr("修改为文本题干,当前题干将被清空,确认清空吗？"), QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes)
        {
            ui->textEdit_Content->setText("");
            if(ui->graphicsView->isVisible())
                ui->graphicsView->hide();
            ui->pushButton_Clear->setEnabled(false);
        }
    }
    else
    {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,tr("文本题干"),tr("修改为非文本题干(建议在word中编辑完后直接复制),\n"
                                                          "当前题干将被清空,确认清空吗？"), QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes)
        {
            ui->textEdit_Content->setText("");
            if(ui->graphicsView->isHidden())
                ui->graphicsView->show();
            ui->pushButton_Clear->setEnabled(true);
        }
    }
}
