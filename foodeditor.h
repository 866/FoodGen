#ifndef FOODEDITOR_H
#define FOODEDITOR_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include "dataworker.h"

namespace Ui {
class FoodEditor;
}

class FoodEditor : public QDialog
{
    Q_OBJECT

public:
    explicit FoodEditor(const Dishes &dishes, QWidget *parent = 0);
    ~FoodEditor();
    QTableWidget* getTable() const;
    QListWidget* getList() const;

private slots:
    void on_addToTable_clicked();
    void on_tableDishes_itemSelectionChanged();
    void on_copyButton_clicked();
    void on_removeFromTable_clicked();
    void on_removeFromListButton_clicked();
    void on_tableDishes_cellChanged(int row, int column);

private:
    void addDishToTable(const QString& name, const DishChars& dishc);
    int listContainsItem(const QString& item);
    int tableContainsItem(const QString& item);
    Ui::FoodEditor *ui;
    QString lastItem;
    bool userChanges;
};

#endif // FOODEDITOR_H
