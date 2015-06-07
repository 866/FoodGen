#ifndef FGMAIN_H
#define FGMAIN_H

#include <QMainWindow>

namespace Ui {
class FGmain;
}

class FGmain : public QMainWindow
{
    Q_OBJECT

public:
    explicit FGmain(QWidget *parent = 0);
    ~FGmain();

private:
    Ui::FGmain *ui;
};

#endif // FGMAIN_H
