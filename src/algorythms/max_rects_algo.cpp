#include "max_rects_algo.h"

#include <QDebug>
#include <QGraphicsPixmapItem>

Rectangle::Rectangle(double x, double y, double w, double h)
    : QRectF(x, y, w, h)
{
}

Rectangle::Rectangle(const QRectF &rect)
    : QRectF(rect)
{
}

double Rectangle::getScore(const QRectF &rect)
{
    double rectArea = width() * height();
    double anotherArea = rect.width() * rect.height();

    return rectArea - anotherArea;
}

bool Rectangle::isPlaced(const QRectF &rect)
{
    return width() >= rect.width() && height() >= rect.height();
}

bool Rectangle::isNull() const
{
    // Прямоугольник считается нулевым, если хотя бы одна из сторон у него равна нулю
    return width() == 0 || height() == 0 || QRectF::isNull();
}

QList<Rectangle> operator-(const Rectangle &rect1, const Rectangle &rect2)
{
    QList<Rectangle> rects;

    if (!rect1.intersects(rect2))
        return rects;

    /* Формируем список возможных прямоугольнков
     * (могу получаться посторонние (с неположительными размерами)) */
    rects.push_back(Rectangle(
                        rect1.x(), rect1.y(),
                        rect1.width(), rect2.y() - rect1.y()
                        )
                    );

    rects.push_back(Rectangle(
                        rect1.x(), rect1.y(),
                        rect2.x() - rect1.x(), rect1.height()
                        )
                    );

    rects.push_back(Rectangle(
                        rect1.x(), rect2.bottom(),
                        rect1.width(), rect1.bottom() - rect2.bottom()
                        )
                    );

    rects.push_back(Rectangle(
                        rect2.right(), rect1.y(),
                        rect1.right() - rect2.right(), rect1.height()
                        )
                    );

    // Удавлем посторонние прямоугольники
    for (auto r : rects)
        if (!rect1.intersects(r) || !r.isValid() || r.isNull())
            rects.removeOne(r);

    return rects;
}

MaxRectsAlgo::MaxRectsAlgo(QGraphicsScene *parent)
    : AlgoBase(parent)
{
}

void MaxRectsAlgo::start()
{
    atlasRect = parent->sceneRect();
    rectangles.push_back(atlasRect);

    for (auto i = textures.begin(); i != textures.end(); ++i) {
        Rectangle bestRect = getBestRect((*i)->pixmap().rect());

        if (bestRect.isNull()) {
            blackList.push_back(*i);
            parent->removeItem(*i);
        } else {
            (*i)->setPos(bestRect.topLeft());
            qDebug() << "Item's pos =" << (*i)->pos();
            QRectF imgRect = (*i)->pixmap().rect();
            imgRect.moveTopLeft((*i)->pos());
            intersectRects(imgRect);
            makeUnique();
        }
    }

    rectangles.clear();
    emit done(blackList);
}

Rectangle MaxRectsAlgo::getBestRect(QRectF rect)
{
    Rectangle bestRect(0, 0, 0, 0);

    for (Rectangle r : rectangles) {
        if (r.isPlaced(rect)
                && (bestRect.isNull()
                    || r.getScore(rect) < bestRect.getScore(rect)))
            bestRect = r;
    }

    return bestRect;
}

void MaxRectsAlgo::intersectRects(QRectF rect)
{
    QList<Rectangle> newRects;
//    for (Rectangle r : rectangles) {
//        if (!r.intersects(rect))
//            continue;

//         = r - rect;
//        rectangles.removeOne(r);
//        rectangles.append(newRects);
//    }

    for (auto i = rectangles.begin(); i != rectangles.end(); )
        if (i->intersects(rect)) {
            newRects.append((*i) - rect);
            i = rectangles.erase(i);
        } else
            i++;

    rectangles.append(newRects);
}

void MaxRectsAlgo::makeUnique()
{
//    for (int i = 0; i < rectangles.size(); i++)
//        for (int j = i + 1; j < rectangles.size(); j++)
//            if (rectangles[i].contains(rectangles[j]))
//                rectangles.removeAt(j);

    for (auto i = rectangles.begin(); i != rectangles.end(); ++i)
        for (auto j = i + 1; j != rectangles.end(); )
            if (i->contains(*j))
                j = rectangles.erase(j);
            else
                j++;
}
