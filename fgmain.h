#ifndef FGMAIN_H
#define FGMAIN_H

#include <QMainWindow>
#include <QException>
#include "dataworker.h"

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
    void refreshFoodList();
    //void loadDataFromDialog()

    Ui::FGmain *ui;
    Dishes* allDishes;

private slots:
    void on_generateButton_clicked();
    void on_setButton_clicked();
    void open_file();
    void save_file();
};

#endif // FGMAIN_H
