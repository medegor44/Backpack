#ifndef GUILLOTINE_ALGO
#define GUILLOTINE_ALGO

#include "algo_base.h"

class Rect
{
private:
    QRectF rect;

public:
    Rect(QRectF rect);

    // Упаковка изображения в прямоугольник и разделение прямоугольника
    Rect *placeHere(QGraphicsPixmapItem *item);

    // Проверка на возможность упаковки
    bool isPlaced(QGraphicsPixmapItem *item);

    // Получение площади, которая останется после упаковки
    double score(QGraphicsPixmapItem *item);
};

class GuillotineAlgo : public AlgoBase
{
    Q_OBJECT

private:
    QList <Rect *> rects; // Список прямоугольников

    // Поиск лучшего прямоугольника в списке
    Rect *getBestRect(QGraphicsPixmapItem *item);

    void reset(); // Сброс состояния алгоритма

public:
    GuillotineAlgo(QGraphicsScene *parent);

    virtual void start(); // Запуск алгоритма

    ~GuillotineAlgo() { reset(); }
};

#endif
