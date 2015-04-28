#ifndef GUILLOTINE_ALGO
#define GUILLOTINE_ALGO

#include "algo_base.h"

class Rect
{
private:
    QRectF rect;

public:
    Rect(QRectF rect);
    Rect *placeHere(QGraphicsPixmapItem *item);
    // Упаковка изображения в прямоугольник и разделение прямоугольника

    bool isPlaced(QGraphicsPixmapItem *item);
    // Проверка на возможность упаковки

    double score(QGraphicsPixmapItem *item);
    // Получение площади, которая останется после упаковки
};

class GuillotineAlgo : public AlgoBase
{
    Q_OBJECT

private:
    QList <Rect *> rects; // Список прямоугольников
    Rect *getBestRect(QGraphicsPixmapItem *item);
    // Поиск лучшего прямоугольника в списке
    void reset(); // Сброс состояния алгоритма

public:
    GuillotineAlgo(QGraphicsScene *parent);
    virtual void start(); // Запуск алгоритма
    ~GuillotineAlgo() { reset(); }
};

#endif
