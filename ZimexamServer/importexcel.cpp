/********************************************************************
 * 版权所有 （c） 2015——2020  Jiangs111
 * 文件名　：importquestion.h
 * 文件标识：无
 * 内容摘要：考试系统导入试题源文件
 * 当前版本：V1.0a
 * 作　　者：Jiangs111
 * 完成日期：2016.1.11
 *
 ********************************************************************/
#include "importexcel.h"
#include "ui_importexcel.h"
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

using namespace QtConcurrent;


Importexcel::Importexcel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Importexcel)
{
    ui->setupUi(this);
}

Importexcel::~Importexcel()
{
    delete ui;
}

/********************************************************************
 * 功能描述：文件打开按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     打开要导入的文件
 ********************************************************************/
void Importexcel::on_pushButton_File_clicked()
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
    QVariant title_value = excel->property("Caption");  //获取标题
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

        if(column_count != 3)
            QMessageBox::warning(this, tr("打开excel文件"),tr("打开的文件不是3列,无法预览导入......"));
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
            ui->label_Count->setText("共需导入考生 " + row_count + " 名");
        }
    }
    ui->tableView->setModel(model);
    work_book->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete work_books;
    delete excel;
    work_books = NULL;
    excel = NULL;
    ui->toolButton_Add->setEnabled(true);
    ui->toolButton_Clear->setEnabled(true);
}

/********************************************************************
 * 功能描述：退出按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.22  版本号     修改人         修改内容
 * 2015.12.22           V1.0a     Jiangs111     关闭窗口
 ********************************************************************/
void Importexcel::on_pushButton_Exit_clicked()
{
    this->close();
}

/********************************************************************
 * 功能描述：导入按钮
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.23  版本号     修改人         修改内容
 * 2015.12.23           V1.0a     Jiangs111     导入窗口
 ********************************************************************/
void Importexcel::on_pushButton_Ok_clicked()
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
    QVariant title_value = excel->property("Caption");  //获取标题
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

        int tid, tname, tclass;
        if(ui->label_Col1_2->text() == "考号")
            tid = 1;    //第一列为考号
        else if(ui->label_Col2_2->text() == "考号")
            tid = 2;
        else if(ui->label_Col3_2->text() == "考号")
            tid = 3;
        else
        {
            ui->label_Process->setText("没有设置数据列名,无法导入!");
            return;
        }

        if(ui->label_Col1_2->text() == "姓名")
            tname = 1;    //第一列为姓名
        else if(ui->label_Col2_2->text() == "姓名")
            tname = 2;
        else if(ui->label_Col3_2->text() == "姓名")
            tname = 3;
        else
        {
            ui->label_Process->setText("没有设置数据列名,无法导入!");
            return;
        }

        if(ui->label_Col1_2->text() == "班级")
            tclass = 1;    //第一列为姓名
        else if(ui->label_Col2_2->text() == "班级")
            tclass = 2;
        else if(ui->label_Col3_2->text() == "班级")
            tclass = 3;
        else
        {
            ui->label_Process->setText("没有设置数据列名,无法导入!");
            return;
        }

        //qDebug() << tid << tname << tclass;

        if(ui->checkBox_Top->isChecked())
            row_start += 1;
        //qDebug() << row_start;
        QVector<QString> sqls;
        QVector<int> vector;
        for(int i=row_start; i <= row_count; i++)
        {
            QString str;
            QVariant cell_testId;  //获取单元格内容
            QVariant cell_Name;
            QVariant cell_Class;
            for(int j=column_start; j<= column_count; j++)
            {
                QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);

                if(j == tid)
                {
                    cell_testId = cell->property("Value");
                    str = "select TestId from Login_student where TestId = '"
                                        + cell_testId.toString() + "'";
                    //qDebug() << cell_testId.toString();
                    QSqlQuery query(str, database);
                    int fieldNo = query.record().indexOf("TestId");
                    while (query.next())
                    {
                        QString tmp = query.value(fieldNo).toString();
                        if(tmp == cell_testId.toString())
                        {
                            QMessageBox::warning(this, tr("导入出错"),tr("导入的考号")
                                            + cell_testId.toString() + tr("已经存在,无法导入,请删除后再导......"));
                            ui->label_Process->setText("导入的考号" + cell_testId.toString() + "已经存在,请删除后再导!");
                            database.close();
                            QSqlDatabase::removeDatabase(m_activeDb);
                            work_book->dynamicCall("Close()");
                            excel->dynamicCall("Quit()");
                            delete work_books;
                            delete excel;
                            work_books = NULL;
                            excel = NULL;
                            ui->pushButton_Ok->setEnabled(false);
                            return;
                        }
                    }
                }
                if(j == tname)
                    cell_Name = cell->property("Value");
                if(j == tclass)
                    cell_Class = cell->property("Value");
            }
            sqls << "insert into Login_student(TestId, Name, Class) values('"
                    + cell_testId.toString() + "', '" + cell_Name.toString()
                    + "', '" + cell_Class.toString() + "')";
            //qDebug() << sqls;
        }
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
                QMessageBox::warning(this, tr("导入错误"), tr("导入考生出错......") + err.text());
                database.close();
                QSqlDatabase::removeDatabase(m_activeDb);
                return;
            }
            vector.append(i);
            progressDialog->setValue(i+1);
            if(progressDialog->wasCanceled())
                return;
        }
        QMessageBox::information(this, tr("导入成功"), tr("成功导入考生 ") + rowcount + " 名.");
    }
    else
    {
        QMessageBox::warning(this, tr("导入出错"), tr("导入的excel中没有工作表......"));
    }
    database.close();
    QSqlDatabase::removeDatabase(m_activeDb);
    work_book->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete work_books;
    delete excel;
    work_books = NULL;
    excel = NULL;
    this->close();
}

/********************************************************************
 * 功能描述：获得连接数据库
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：QString
 * 其他说明：无
 * 完成日期：2015.12.24  版本号     修改人         修改内容
 * 2015.12.21           V1.0a     Jiangs111     获得连接数据库
 ********************************************************************/
QString Importexcel::getActivedb()
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
 * 完成日期：2015.12.24  版本号     修改人         修改内容
 * 2015.12.24           V1.0a     Jiangs111     设置连接数据库
 ********************************************************************/
void Importexcel::setActivedb(QString db)
{
    m_activeDb = db;
}

/********************************************************************
 * 功能描述：设置数据列名
 *
 * 输入参数：无
 * 输出参数：无
 * 返回值　：无
 * 其他说明：无
 * 完成日期：2015.12.28  版本号     修改人         修改内容
 * 2015.12.28           V1.0a     Jiangs111     设置数据列名并显示
 ********************************************************************/
void Importexcel::on_toolButton_Add_clicked()
{
    if(ui->comboBox_Title->count() > 0)
    {
        if(ui->label_Col1_2->text() == "")
        {
            ui->label_Col1_2->setText(ui->comboBox_Title->currentText());
            ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
        }
        else
        {
            if(ui->label_Col2_2->text() == "")
            {
                ui->label_Col2_2->setText(ui->comboBox_Title->currentText());
                ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
            }
            else
            {
                if(ui->label_Col3_2->text() == "")
                {
                    ui->label_Col3_2->setText(ui->comboBox_Title->currentText());
                    ui->comboBox_Title->removeItem(ui->comboBox_Title->currentIndex());
                    ui->pushButton_Ok->setEnabled(true);
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
 * 完成日期：2015.12.28  版本号     修改人         修改内容
 * 2015.12.28           V1.0a     Jiangs111     重排数据列名
 ********************************************************************/
void Importexcel::on_toolButton_Clear_clicked()
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
    ui->comboBox_Title->addItem("考号");
    ui->comboBox_Title->addItem("姓名");
    ui->comboBox_Title->addItem("班级");
}
