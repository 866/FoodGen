#ifndef DATAWORKER
#define DATAWORKER

#include <QHash>
#include <QString>
#include <QList>
#include <QDebug>


struct DishChars // Dish struct that is the prototype of one food item
{
    int rank = 1;
    int breakfastRank = 1, dinnerRank = 1, supperRank = 1;
    bool inList = false;
};

typedef QHash<QString, DishChars> Dishes; // Dishes container

Dishes* loadDishesFromDB();

QString rollDice(const QList<QString>& samples, const QList<float>& probArray, const QList<QString>& except);

#endif // DATAWORKER

