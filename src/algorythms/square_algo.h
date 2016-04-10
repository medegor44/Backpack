#ifndef SQUARE_ALGO_H
#define SQUARE_ALGO_H

#include "algo_base.h"

class SquareAlgo : public AlgoBase
{
public:
    SquareAlgo(QGraphicsScene *parent);
    void start() override;

private:
    QList<QPointF> nodes; // Список точек, куда можно устанвливать изображения

    // Список окаймляющих прямоугольников использованных изображений
    QList<QRectF> usedRectngles;

    QRectF boundingRect; // Окаймляющий прямоугольник текущей фигуры

    // Вычисление нового окаймляющего прямоугольника
    QRectF computeRect(QPointF p, QRectF pictureRect);

    // Поиск наилучшей позиции для данного изображения
    QPointF getBestPos(QRectF pictureRect);

    // Проверка на возможные пересечения с уже установленными изображениями
    bool isIntersects(QRectF rect);

    // Удаление точек, на которые уже упаковано изображение
    void removeUsedPoints(QRectF rect);
    void addNewNodes(QRectF rect); // Добавление новых точек
    void reset(); // Сброс состояния алгоритма
    // Добавление изображения на атлас
    bool addToAtlas(QGraphicsPixmapItem *item);
};

#endif // SQUARE_ALGO_H
