#ifndef FOODEDITOR_H
#define FOODEDITOR_H

#include <QDialog>

namespace Ui {
class FoodEditor;
}

class FoodEditor : public QDialog
{
    Q_OBJECT

public:
    explicit FoodEditor(QWidget *parent = 0);
    ~FoodEditor();

private:
    Ui::FoodEditor *ui;
};

#endif // FOODEDITOR_H
