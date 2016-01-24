/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：helpform.h
 * 文件标识：无
 * 内容摘要：考试系统导入Excel源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2015.12.22
 *
 ********************************************************************/
#include "importquestion.h"
#include "ui_importquestion.h"
#include <QFileDialog>
#include <QAxObject>
#include <QDebug>
#include <windows.h>
#include <QTableView>
#include <QStandardItemModel>
#include <QtWidgets>
#include <QtSql>
#include <QException>
#include <QtConcurrent>

ImportQuestion::ImportQuestion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportQuestion)
{
    ui->setupUi(this);
    for(int i = 0; i < cols_num; i++)
        m_tempnum[i] = i + 1;
}

ImportQuestion::~ImportQuestion()
{
    delete ui;
}

/********************************************************************
 * 功能描述：退出按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.12  版本号     修改人         修改内容
 * 2016.1.12           V1.0a     Jiangs111     关闭窗口
 ********************************************************************/
void ImportQuestion::on_pushButton_Exit_clicked()
{
    this->close();
}

/********************************************************************
 * 功能描述：获得连接数据库
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QString
 * 其他说明：无
 * 完成日期：2016.1.12  版本号     修改人         修改内容
 * 2016.1.12           V1.0a     Jiangs111     获得连接数据库
 ********************************************************************/
QString ImportQuestion::getActivedb()
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
 * 完成日期：2016.1.12  版本号     修改人         修改内容
 * 2016.1.12           V1.0a     Jiangs111     设置连接数据库
 ********************************************************************/
void ImportQuestion::setActivedb(QString db)
{
    m_activeDb = db;
}

/********************************************************************
 * 功能描述：文件打开按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.12  版本号     修改人         修改内容
 * 2016.1.12           V1.0a     Jiangs111     打开要导入的文件
 ********************************************************************/
void ImportQuestion::on_pushButton_File_clicked()
{
    QString fl = QFileDialog::getOpenFileName(this, "打开文件", "/", "Excel 文件(*.xls;*.xlsx)");
    ui->lineEdit_File->setText(fl);
    if(fl == "")
        return;

    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);}

    QAxObject *excel = new QAxObject(this);
    excel->setControl("Excel.Application");
    excel->setProperty("Visible", false);
    excel->setProperty("DisplayAlerts", false);

    QAxObject *work_books = excel->querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", QString(fl));

    //QVariant title_value = excel->property("Caption");  //获取标题
    //qDebug()<<QString("excel title : ")<<title_value;
    QAxObject *work_book = excel->querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用

    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目

    QStandardItemModel *model=new QStandardItemModel();
    //qDebug()<<QString("sheet count : ")<<sheet_count;
    if(sheet_count > 0)
    {
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
        QAxObject *used_range = work_sheet->querySubObject("UsedRange");
        QAxObject *rows = used_range->querySubObject("Rows");
        QAxObject *columns = used_range->querySubObject("Columns");

        int row_start = used_range->property("Row").toInt();  //获取起始行
        int column_start = used_range->property("Column").toInt();  //获取起始列
        QString row_count = rows->property("Count").toString();  //获取行数
        int column_count = columns->property("Count").toInt();  //获取列数

        if(column_count != cols_num)
            QMessageBox::warning(this, tr("打开excel文件"),tr("打开的文件不是列数不对,无法预览导入,请核对导入模板......"));
        else
        {
            for(int i=row_start; i <= 10; i++)
            {
                for(int j=column_start; j<= column_count; j++)
                {
                    QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
                    QVariant cell_value = cell->property("Value");  //获取单元格内容
                    //qDebug() << cell_value.toString();
                    QStandardItem *newItem = 0;
                    newItem = new QStandardItem(cell_value.toString());
                    model->setItem(i-1, j-1, newItem);
                }
            }
            ui->label_Count->setText("共需导入试题 " + row_count + " 个");
        }
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

    work_books->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete work_books;
    delete excel;
    work_books = NULL;
    excel = NULL;
    ui->toolButton_Add->setEnabled(true);
    ui->toolButton_Clear->setEnabled(true);
}

/********************************************************************
 * 功能描述：设置数据列名
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.12  版本号     修改人         修改内容
 * 2016.1.12           V1.0a     Jiangs111     设置数据列名并显示
 ********************************************************************/
void ImportQuestion::on_toolButton_Add_clicked()
{

    if(ui->comboBox_Title->count() > 0)
    {
        if(ui->label_Col1_2->text() == "")
        {
            ui->label_Col1_2->setText(ui->comboBox_Title->currentText());
            m_titlenum[0] = m_tempnum[ui->comboBox_Title->currentIndex()];
            for(int i = ui->comboBox_Title->currentIndex(); i < cols_num - 1; i++)
            {
                m_tempnum[i] = m_tempnum[i + 1];
            }
            /*
            for(int i = 0; i < cols_num - 1; i++)
            {
                qDebug() << m_tempnum[i];
            }
            qDebug() << m_titlenum[0];*/
            ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
        }
        else
        {
            if(ui->label_Col2_2->text() == "")
            {
                ui->label_Col2_2->setText(ui->comboBox_Title->currentText());
                m_titlenum[1] = m_tempnum[ui->comboBox_Title->currentIndex()];
                for(int i = ui->comboBox_Title->currentIndex(); i < cols_num - 2; i++)
                {
                    m_tempnum[i] = m_tempnum[i + 1];
                }
                /*
                for(int i = 0; i < cols_num - 2; i++)
                {
                    qDebug() << m_tempnum[i];
                }
                qDebug() << m_titlenum[1];*/
                ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
            }
            else
            {
                if(ui->label_Col3_2->text() == "")
                {
                    ui->label_Col3_2->setText(ui->comboBox_Title->currentText());
                    m_titlenum[2] = m_tempnum[ui->comboBox_Title->currentIndex()];
                    for(int i = ui->comboBox_Title->currentIndex(); i < cols_num - 3; i++)
                    {
                        m_tempnum[i] = m_tempnum[i + 1];
                    }
                    ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
                }
                else
                {
                    if(ui->label_Col4_2->text() == "")
                    {
                        ui->label_Col4_2->setText(ui->comboBox_Title->currentText());
                        m_titlenum[3] = m_tempnum[ui->comboBox_Title->currentIndex()];
                        for(int i = ui->comboBox_Title->currentIndex(); i < cols_num - 4; i++)
                        {
                            m_tempnum[i] = m_tempnum[i + 1];
                        }
                        ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
                    }
                    else
                    {
                        if(ui->label_Col5_2->text() == "")
                        {
                            ui->label_Col5_2->setText(ui->comboBox_Title->currentText());
                            m_titlenum[4] = m_tempnum[ui->comboBox_Title->currentIndex()];
                            for(int i = ui->comboBox_Title->currentIndex(); i < cols_num - 5; i++)
                            {
                                m_tempnum[i] = m_tempnum[i + 1];
                            }
                            ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
                        }
                        else
                        {
                            if(ui->label_Col6_2->text() == "")
                            {
                                ui->label_Col6_2->setText(ui->comboBox_Title->currentText());
                                m_titlenum[5] = m_tempnum[ui->comboBox_Title->currentIndex()];
                                for(int i = ui->comboBox_Title->currentIndex(); i < cols_num - 6; i++)
                                {
                                    m_tempnum[i] = m_tempnum[i + 1];
                                }
                                ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
                            }
                            else
                            {
                                if(ui->label_Col7_2->text() == "")
                                {
                                    ui->label_Col7_2->setText(ui->comboBox_Title->currentText());
                                    m_titlenum[6] = m_tempnum[ui->comboBox_Title->currentIndex()];
                                    for(int i = ui->comboBox_Title->currentIndex(); i < cols_num - 7; i++)
                                    {
                                        m_tempnum[i] = m_tempnum[i + 1];
                                    }
                                    ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
                                }
                                else
                                {
                                    if(ui->label_Col8_2->text() == "")
                                    {
                                        ui->label_Col8_2->setText(ui->comboBox_Title->currentText());
                                        m_titlenum[7] = m_tempnum[ui->comboBox_Title->currentIndex()];
                                        ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
                                        ui->pushButton_Ok->setEnabled(true);
                                        /*
                                        for(int i = 0; i < cols_num; i++)
                                        {
                                            qDebug() << m_titlenum[i];
                                        }*/
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/********************************************************************
 * 功能描述：重排数据列名
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.12  版本号     修改人         修改内容
 * 2016.1.12           V1.0a     Jiangs111     重排数据列名
 ********************************************************************/
void ImportQuestion::on_toolButton_Clear_clicked()
{
    int count = ui->comboBox_Title->count();
    if(count > 0)
    {
        ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
        for(int i = 0; i <= count; i++)
            ui->comboBox_Title->removeItem(i);
    }
    ui->label_Col1_2->setText("");
    ui->label_Col2_2->setText("");
    ui->label_Col3_2->setText("");
    ui->label_Col4_2->setText("");
    ui->label_Col5_2->setText("");
    ui->label_Col6_2->setText("");
    ui->label_Col7_2->setText("");
    ui->label_Col8_2->setText("");
    ui->comboBox_Title->addItem("试题名称");
    ui->comboBox_Title->addItem("试题题干");
    ui->comboBox_Title->addItem("出题人");
    ui->comboBox_Title->addItem("出题日期");
    ui->comboBox_Title->addItem("试题类型");
    ui->comboBox_Title->addItem("试题答案");
    ui->comboBox_Title->addItem("试题难度");
    ui->comboBox_Title->addItem("关键字");
    for(int i = 0; i < cols_num; i++)
        m_tempnum[i] = i + 1;
}

/********************************************************************
 * 功能描述：导入按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2016.1.12  版本号     修改人         修改内容
 * 2016.1.12           V1.0a     Jiangs111     导入窗口
 ********************************************************************/
void ImportQuestion::on_pushButton_Ok_clicked()
{
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);}

    QAxObject *excel = new QAxObject(this);
    excel->setControl("Excel.Application");
    excel->setProperty("Visible", false);
    excel->setProperty("DisplayAlerts", false);

    QAxObject *work_books = excel->querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", QString(ui->lineEdit_File->text()));
    //QVariant title_value = excel->property("Caption");  //获取标题
    //qDebug()<<QString("excel title : ")<<title_value;
    QAxObject *work_book = excel->querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用

    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目

    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(m_activeDb);

    if(sheet_count > 0)
    {
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
        QAxObject *used_range = work_sheet->querySubObject("UsedRange");
        QAxObject *rows = used_range->querySubObject("Rows");
        QAxObject *columns = used_range->querySubObject("Columns");

        int row_start = used_range->property("Row").toInt();  //获取起始行
        int column_start = used_range->property("Column").toInt();  //获取起始列
        int row_count = rows->property("Count").toInt();  //获取行数
        QString rowcount = rows->property("Count").toString();
        int column_count = columns->property("Count").toInt();  //获取列数
        /*
        for(int i = 0; i < cols_num; i++)
        {
            qDebug() << m_titlenum[i];
        }*/

        if(ui->checkBox_Top->isChecked())
            row_start += 1;

        QVector<QString> sqls;
        QVector<int> vector;
        for(int i=row_start; i <= row_count; i++)
        {
            QVariant cellcol[cols_num];
            for(int j=column_start; j<= column_count; j++)
            {
                QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
                for(int k = 0; k < cols_num; k++)
                {
                    //qDebug() << "j:" << j;
                    //qDebug() << "k:" << k;
                    //qDebug() << "m_titlenum[k]:" << m_titlenum[k];
                    if(j == m_titlenum[k])
                    {
                        cellcol[k] = cell->property("Value");
                        //qDebug() << cellcol[k];
                    }
                }
            }
            QStringList acos = cellcol[5].toString().split(";");
            QString answercols = "1";
            if(acos.count() > 1)
                answercols = QString::number(acos.count());

            sqls << "INSERT INTO Question_Info (Name, Content, Creator, CreateTime, "
                      "TypeID, LastModify, Answer, ExposeTimes, RightTimes, "
                      "WrongTimes, Difficulty, Keyword, AnswerCols) VALUES ('"
             + cellcol[0].toString() +"', '" + cellcol[1].toString() + "', '"
             + cellcol[2].toString() +"', '" + cellcol[3].toString() + "', "
             + cellcol[4].toString() +", '" + cellcol[3].toString() + "', '"
             + cellcol[5].toString() +"', 0, 0, 0, " + cellcol[6].toString() +", '"
             + cellcol[7].toString() +"', " + answercols + ")";
        }
        //for(int i = 0; i < sqls.size(); i++)
        //    qDebug() << sqls.at(i);
        int num = sqls.size();
        QProgressDialog *progressDialog=new QProgressDialog(this);
        progressDialog->setWindowModality(Qt::WindowModal);
        progressDialog->setMinimumDuration(5);
        progressDialog->setWindowTitle(tr("请等待"));
        progressDialog->setLabelText(tr("导入中..."));
        progressDialog->setCancelButtonText(tr("取消"));
        progressDialog->setRange(0,num);

        for (int i = 0; i < num; ++i)
        {
            QSqlQuery query(sqls.at(i), database);
            //qDebug() << sqls.at(i);
            QSqlError err = query.lastError();
            if (err.type() != QSqlError::NoError)
            {
                QMessageBox::warning(this, tr("导入错误"), tr("导入试题出错......") + err.text());
                database.close();
                QSqlDatabase::removeDatabase(m_activeDb);
                work_books->dynamicCall("Close()");
                excel->dynamicCall("Quit()");
                delete work_books;
                delete excel;
                work_books = NULL;
                excel = NULL;
                ui->pushButton_Ok->setEnabled(false);
                return;
            }
            vector.append(i);
            progressDialog->setValue(i+1);
            if(progressDialog->wasCanceled())
                return;
        }
        QMessageBox::information(this, tr("导入成功"), tr("成功导入试题 ") + rowcount + " .");
    }
    else
    {
        QMessageBox::warning(this, tr("导入出错"), tr("导入的excel中没有工作表......"));
    }
    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);
    work_books->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete work_books;
    delete excel;
    work_books = NULL;
    excel = NULL;
    this->close();
}
