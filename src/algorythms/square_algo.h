#ifndef SQUARE_ALGO_H
#define SQUARE_ALGO_H

#include "algo_base.h"

class SquareAlgo : public AlgoBase
{
public:
    SquareAlgo(QGraphicsScene *parent);
    void start() override;

private:
    QList<QPointF> nodes;
    QList<QRectF> usedRectngles;
    QRectF boundingRect;

    QRectF computeRect(QPointF p, QRectF pictureRect);
    QPointF getBestPos(QRectF pictureRect);

    bool isIntersects(QRectF rect);
    void removeUsedPoints(QRectF rect);
    void addNewNodes(QRectF rect);
    bool addToAtlas(QGraphicsPixmapItem *item);
};

#endif // SQUARE_ALGO_H
