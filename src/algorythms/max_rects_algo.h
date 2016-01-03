#ifndef MAX_RECTS_ALGO
#define MAX_RECTS_ALGO

#include <QSize>
#include <QPointF>
#include <QRectF>

#include "algo_base.h"

class Rectangle : public QRectF
{
public:
    Rectangle(double x, double y, double w, double h);
    Rectangle(const QRectF &rect);

    // Получение разности площадей
    double getScore(const QRectF &rect);

    // Проверка, возможно ли упаковать прямоугольник rect в данный
    bool isPlaced(const QRectF &rect);

    bool isNull() const;
};

// Получение разности 2 прямоугольников
QList<Rectangle> operator-(const Rectangle &rect1, const Rectangle &rect2);

class MaxRectsAlgo : public AlgoBase
{
    Q_OBJECT

public:
    MaxRectsAlgo(QGraphicsScene *parent);
    void start() override;

private:
    Rectangle getBestRect(QRectF rect);
    void intersectRects(QRectF rect);
    void makeUnique();
    QList<Rectangle> rectangles;
};

#endif // MAX_RECTS_ALGO

