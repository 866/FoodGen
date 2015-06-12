#include <QMessageBox>
#include <QException>

#include "foodeditor.h"
#include "ui_foodeditor.h"
#include "dataworker.h"

FoodEditor::FoodEditor(const Dishes& dishes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FoodEditor)
{
    userChanges = true;

    lastItem = "";

    ui->setupUi(this);

    ui->tableDishes->insertColumn(0);
    ui->tableDishes->insertColumn(0);
    ui->tableDishes->insertColumn(0);
    ui->tableDishes->insertColumn(0);
    ui->tableDishes->insertColumn(0);

    ui->tableDishes->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Name")));
    ui->tableDishes->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Rank")));
    ui->tableDishes->setHorizontalHeaderItem(2, new QTableWidgetItem(QString("Breakfast")));
    ui->tableDishes->setHorizontalHeaderItem(3, new QTableWidgetItem(QString("Dinner")));
    ui->tableDishes->setHorizontalHeaderItem(4, new QTableWidgetItem(QString("Supper")));

    ui->tableDishes->setColumnWidth(0, 100);
    ui->tableDishes->setColumnWidth(1, 50);
    ui->tableDishes->setColumnWidth(2, 60);
    ui->tableDishes->setColumnWidth(3, 50);
    ui->tableDishes->setColumnWidth(4, 50);

    foreach(QString name, dishes.keys())
    {
        addDishToTable(name, dishes[name]);
        if (dishes[name].inList) ui->chosenDishes->insertItem(0, name);
    }

    ui->tableDishes->sortByColumn(0, Qt::SortOrder::AscendingOrder);
    ui->chosenDishes->sortItems(Qt::SortOrder::AscendingOrder);
}

void FoodEditor::addDishToTable(const QString& name, const DishChars& dishc) { // Adds new cells to the table
    userChanges = false;
    ui->tableDishes->insertRow(0);
    ui->tableDishes->setItem(0, 0, new QTableWidgetItem(name));
    ui->tableDishes->setItem(0, 1, new QTableWidgetItem(QString::number(dishc.rank)));
    ui->tableDishes->setItem(0, 2, new QTableWidgetItem(QString::number(dishc.breakfastRank)));
    ui->tableDishes->setItem(0, 3, new QTableWidgetItem(QString::number(dishc.dinnerRank)));
    ui->tableDishes->setItem(0, 4, new QTableWidgetItem(QString::number(dishc.supperRank)));
    userChanges = true;
}

void FoodEditor::on_addToTable_clicked() { // Adds default dish
    DishChars dishc;
    addDishToTable("Surprise dish", DishChars());
    ui->tableDishes->sortByColumn(0);
}


FoodEditor::~FoodEditor()
{
    delete ui;
}

void FoodEditor::on_tableDishes_itemSelectionChanged() { // refreshes last selected item
    int row = ui->tableDishes->currentRow();
    int column = ui->tableDishes->currentColumn();
    lastItem = ui->tableDishes->item(row, column)->text();
}

int FoodEditor::listContainsItem(const QString& item) { // checks whether list contains item
    for(int i=0; i<ui->chosenDishes->count(); ++i)
        if (item == ui->chosenDishes->item(i)->text())
            return i;
    return -1;
}

int FoodEditor::tableContainsItem(const QString& item) // checks whether table contains item
{
    for(int i=0; i<ui->tableDishes->rowCount(); ++i)
        if (item == ui->tableDishes->item(i, 0)->text())
            return i;
    return -1;
}

void FoodEditor::on_copyButton_clicked() // copies dish from the table to the list
{
    int currentRow = ui->tableDishes->currentRow();
    if (currentRow == -1) return;
    QString item_text = ui->tableDishes->item(currentRow, 0)->text();
    if (listContainsItem(item_text) == -1)
        ui->chosenDishes->insertItem(0, item_text);
    else
        QMessageBox::warning(NULL, "Warning", "The item is already there!");
    ui->chosenDishes->sortItems(Qt::SortOrder::AscendingOrder);
}

void FoodEditor::on_removeFromTable_clicked() { // removes item from the table
    int currentRow = ui->tableDishes->currentRow();
    if (currentRow == -1) return;
    QString item_text = ui->tableDishes->item(currentRow, 0)->text();
    QListWidgetItem* itemToDelete = ui->chosenDishes->item(listContainsItem(item_text));
    try
    {
        ui->tableDishes->clearSelection();
        ui->tableDishes->removeRow(currentRow);
        delete itemToDelete;
    }
    catch(QException e) {qDebug() << e.what();}

}

void FoodEditor::on_removeFromListButton_clicked() { // removes item from the list
    delete ui->chosenDishes->item(ui->chosenDishes->currentRow());
}

void FoodEditor::on_tableDishes_cellChanged(int row, int column) {
    if (userChanges)
    {
        if (column == 0)
        {
            int itemToChange = listContainsItem(lastItem);
            lastItem = ui->tableDishes->item(row, column)->text();
            if (itemToChange != -1)
                ui->chosenDishes->item(itemToChange)->setText(lastItem);
            ui->tableDishes->sortByColumn(0, Qt::SortOrder::AscendingOrder);
        }
        else
        {
            QString cellText = ui->tableDishes->item(row, column)->text();
            bool success = false;
            int number = cellText.toInt(&success);
            if ((!success) || (number < 1))
            {
                QMessageBox::warning(NULL, "Error", "Enter correct integer value!");
                ui->tableDishes->item(row, column)->setText("1");
            }
        }
    }
}

QTableWidget* FoodEditor::getTable() const {return ui->tableDishes;}

QListWidget* FoodEditor::getList() const {return ui->chosenDishes;}
