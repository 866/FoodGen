#include <time.h>
#include <stdlib.h>
#include <QException>

#include "dataworker.h"

#define MAX_NUMBER_OF_ITERATIONS 10000

Dishes* loadDishesFromDB()
{
    Dishes* res = nullptr;
    res = new Dishes();
    (*res).insert("Вівсянка", DishChars());
    (*res).insert("Омлет", DishChars());
    (*res).insert("Суп", DishChars());
    return res;
}

QString rollDice(const QList<QString>& samples, const QList<float>& probArray, const QList<QString>& except)
{
    QString res;
    int  iteration = 0, count = probArray.count();
    //srand(time(NULL));

    if ( (count == 0) || (samples.count() != count) )
        throw std::domain_error("Improper set");

    do
    {
        if (iteration > MAX_NUMBER_OF_ITERATIONS) throw std::domain_error("MAX_NUMBER_OF_ITERATIONS exceeded.");

        float randn = static_cast<float>(rand())/RAND_MAX;
        float sum = probArray[0];
        int i = 0;
        while(randn >= sum && i < count) {i++; sum += (probArray[i]);}
        res = samples[i];
        iteration++;
    } while(except.contains(res));

    return res;
}
