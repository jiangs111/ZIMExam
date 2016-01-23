#ifndef CONFIGFORM_H
#define CONFIGFORM_H

#include <QWidget>

namespace Ui {
class ConfigForm;
}

class ConfigForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigForm(QWidget *parent = 0);
    ~ConfigForm();

private slots:

    void on_pushButton_Exit_clicked();

private:
    Ui::ConfigForm *ui;
};

#endif // CONFIGFORM_H
