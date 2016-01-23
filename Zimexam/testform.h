#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>

namespace Ui {
class TestForm;
}

class TestForm : public QWidget
{
    Q_OBJECT

public:
    explicit TestForm(QWidget *parent = 0);
    ~TestForm();

private slots:
    void on_pushButton_Exit_clicked();
    void keyPressEvent(QKeyEvent *event);

    void on_pushButton_EnterP_clicked();

    void on_pushButton_EnterT_clicked();

private:
    Ui::TestForm *ui;
};

#endif // TESTFORM_H
