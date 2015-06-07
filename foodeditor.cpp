#include "foodeditor.h"
#include "ui_foodeditor.h"

FoodEditor::FoodEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FoodEditor)
{
    ui->setupUi(this);
}

FoodEditor::~FoodEditor()
{
    delete ui;
}
