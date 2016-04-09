#include "algo_base.h"

class Level
{
private:
    QRectF rect; // Прямоугольник уровня
    double currentLenght;

public:
    Level(QRectF rect);

    void placeHere(QGraphicsPixmapItem *item);
    // Установка изображения на уровень

    bool isPlaced(QGraphicsPixmapItem *item);
    // Проверка на возможность помещения изображения

    double score(QGraphicsPixmapItem *item);
    // Получение оставшейся площади после установки изображения
};

class BestFitAlgo : public AlgoBase
{
    Q_OBJECT

private:
    Level *getBestLevel(QGraphicsPixmapItem *item);
    void reset();

    QList<Level *> levels;

public:
    BestFitAlgo(QGraphicsScene *parent);
    virtual void start();
    ~BestFitAlgo();
};
