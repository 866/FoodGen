#include "fgmain.h"
#include "ui_fgmain.h"

FGmain::FGmain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FGmain)
{
    ui->setupUi(this);
}

FGmain::~FGmain()
{
    delete ui;
}
