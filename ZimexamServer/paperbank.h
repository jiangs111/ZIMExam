#ifndef PAPERBANK_H
#define PAPERBANK_H

#include <QWidget>

namespace Ui {
class PaperBank;
}

class PaperBank : public QWidget
{
    Q_OBJECT

public:
    explicit PaperBank(QWidget *parent = 0);
    ~PaperBank();

private:
    Ui::PaperBank *ui;
};

#endif // PAPERBANK_H
