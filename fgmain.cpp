#include <QStringListModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QtWidgets>
#include <QDebug>

#include "fgmain.h"
#include "ui_fgmain.h"
#include "foodeditor.h"


FGmain::FGmain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FGmain)
{
    allDishes = loadDishesFromDB();
    ui->setupUi(this);

    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open_file()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save_file()));

    refreshFoodList();
}

void FGmain::refreshFoodList() // Refreshes ListView of the main form
{
    if(!allDishes) return;

    QStringListModel* model;
    QStringList dishesNamesList;
    if (!ui->foodList->model())
    {
        model = new QStringListModel(this);
        ui->foodList->setModel(model);
    }
    else
        model = static_cast<QStringListModel*> (ui->foodList->model());

    foreach(QString name, (*allDishes).keys())
        if ((*allDishes)[name].inList)
            dishesNamesList << name;

    model->setStringList(dishesNamesList);
}

FGmain::~FGmain()
{
    delete allDishes;
    delete ui;
}

void FGmain::on_generateButton_clicked() // Generates dish schedule
{
    int days = 10;
    QString result = "Generated dishes schedule for the next " + QString::number(days) + " days\n\n";
    QList<QString> dish_names;
    QList<float> dish_probs[3];
    float breakSum = 0, dinSum = 0, supSum = 0;

    foreach(QString name, allDishes->keys())
        if ((*allDishes)[name].inList)
            dish_names << name;

    for(int i=0; i < dish_names.count(); ++i)
    {
        const DishChars& chars = (*allDishes)[dish_names[i]];

        dish_probs[0] << chars.breakfastRank * chars.rank;
        breakSum += dish_probs[0][i];

        dish_probs[1] << chars.dinnerRank * chars.rank;
        dinSum += dish_probs[1][i];

        dish_probs[2] << chars.supperRank * chars.rank;
        supSum += dish_probs[2][i];
    }

    for(int i=0; i < dish_names.count(); ++i)
    {
        dish_probs[0][i] /= breakSum;
        dish_probs[1][i] /= dinSum;
        dish_probs[2][i] /= supSum;
    }

    for(int day = 0; day < days; ++day)
    {
        QList<QString> rolled;
        result += "\n=========\nDay №" + QString::number(day+1) + '\n';
        for(int i=0; i<3; ++i)
        {
            try {
                rolled << rollDice(dish_names, dish_probs[i], rolled);
            }
            catch(std::domain_error e) {QMessageBox::warning(NULL, "Exception", e.what());return;}
        }
        result += ("breakfast: " + rolled[0] +
                "\ndinner: " + rolled[1] +
                "\nsupper: " + rolled[2] +
                "\n=========");
    }

    ui->genFoodResult->setPlainText(result);
}

void FGmain::on_setButton_clicked() // Opens setup dialog
{
    FoodEditor fui(*allDishes, this);
    if (fui.exec() == QDialog::Accepted)
    {
        allDishes->clear();
        auto table = fui.getTable();
        auto list = fui.getList();
        for (int row=0; row < table->rowCount(); ++row)
        {
            DishChars dish;
            dish.rank = table->item(row, 1)->text().toInt();
            dish.breakfastRank = table->item(row, 2)->text().toInt();
            dish.dinnerRank = table->item(row, 3)->text().toInt();
            dish.supperRank = table->item(row, 4)->text().toInt();
            allDishes->insert(table->item(row, 0)->text(), dish);
        }

        for (int row=0; row < list->count(); ++row)
            (*allDishes)[list->item(row)->text()].inList = true;

        refreshFoodList();

    }

}

void FGmain::open_file()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return;

    allDishes->clear();
    QString filename = files[0];
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        while(!stream.atEnd()){
            QStringList fields = stream.readLine().split(",");
            DishChars dc;
            dc.rank = fields[1].trimmed().toInt();
            dc.breakfastRank = fields[2].trimmed().toInt();
            dc.dinnerRank = fields[3].trimmed().toInt();
            dc.supperRank = fields[4].trimmed().toInt();
            dc.inList = fields[5].trimmed().toInt();
            allDishes->insert(fields[0].trimmed(), dc);
        }
    file.close();
    }

    refreshFoodList();
}

void FGmain::save_file()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return;

    QString filename = files[0];
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        foreach(QString name, allDishes->keys()) {
            DishChars& dc = (*allDishes)[name];
            stream << name << ','
                   << dc.rank << ','
                   << dc.breakfastRank << ','
                   << dc.dinnerRank << ','
                   << dc.supperRank << ','
                   << dc.inList << ',' << endl;
        }
    file.close();
    }
}

