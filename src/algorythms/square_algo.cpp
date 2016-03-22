#include "square_algo.h"

SquareAlgo::SquareAlgo(QGraphicsScene *parent)
    : AlgoBase(parent)
{
}

QRectF SquareAlgo::computeRect(QPointF p, QRectF pictureRect)
{
    QRectF newBoundingRect = this->boundingRect;
    double distFromOX = p.x() + pictureRect.width();
    double distFromOY = p.y() + pictureRect.height();

    if (distFromOX >= atlasRect.width() ||distFromOY >= atlasRect.height())
        return QRectF(-1, -1, 0, 0);

    if (distFromOX <= boundingRect.width() && distFromOY <= boundingRect.height())
        return boundingRect;

    if (distFromOX > newBoundingRect.width())
        newBoundingRect.setRight(distFromOX);
    if (distFromOY > newBoundingRect.height())
        newBoundingRect.setBottom(distFromOY);

    return newBoundingRect;
}

QPointF SquareAlgo::getBestPos(QRectF pictureRect)
{
    QRectF bestRect = computeRect(nodes.first(), pictureRect);
//    QRectF oldRect = boundingRect;
    QPointF bestPoint = nodes.first();

    for (QPointF p : nodes) {
        QRectF rect = computeRect(p, pictureRect);
        pictureRect.moveTo(p);

        if (rect.isNull() || isIntersects(pictureRect))
            continue;

        if (abs(rect.width() - rect.height()) < abs(bestRect.width() - bestRect.height()) || bestRect.isNull() || bestRect == boundingRect) {
            bestRect = rect;
            bestPoint = p;
        }
    }

    if (bestRect.isNull())
        return QPointF(-1, -1);

    return bestPoint;
}

bool SquareAlgo::isIntersects(QRectF rect)
{
    for (QRectF r : usedRectngles)
        if (r.intersects(rect))
            return true;

    return false;
}

void SquareAlgo::removeUsedPoints(QRectF rect)
{
    for (QPointF p : nodes)
        if (rect.contains(p))
            nodes.removeOne(p);
}

void SquareAlgo::addNewNodes(QRectF rect)
{
    QPointF p1 = rect.topRight();
    QPointF p2 = rect.bottomLeft();

    if (parent->itemAt(QPointF(p1.x(), p1.y() - 1), QTransform()) != nullptr || p1.y() == 0)
        nodes.push_back(p1);
    if (parent->itemAt(QPointF(p2.x() - 1, p2.y()), QTransform()) != nullptr || p2.x() == 0)
        nodes.push_back(p2);
}

void SquareAlgo::reset()
{
    nodes.clear();
    usedRectngles.clear();
    blackList.clear();
}

bool SquareAlgo::addToAtlas(QGraphicsPixmapItem *item)
{
    QRectF pictureRect = item->boundingRect();
    QPointF bestPoint = getBestPos(pictureRect);

    if (bestPoint.x() < 0 || bestPoint.y() < 0)
        return false;

    item->setPos(bestPoint);
    boundingRect = computeRect(bestPoint, pictureRect);

    pictureRect.moveTo(bestPoint);
    usedRectngles.push_back(pictureRect);
    removeUsedPoints(pictureRect);

    addNewNodes(pictureRect);

    return true;
}

void SquareAlgo::start()
{
    atlasRect = parent->sceneRect();
    nodes.push_back(QPointF(0.0, 0.0));
    boundingRect = QRectF(0, 0, 0, 0);

    std::sort(textures.begin(), textures.end(),
              [](QGraphicsPixmapItem *p1, QGraphicsPixmapItem *p2) {
        return p1->pixmap().width() > p2->pixmap().width();
    });

    for (QGraphicsPixmapItem *picture : textures) {
        if (!addToAtlas(picture)) {
            parent->removeItem(picture);
            blackList.push_back(picture);
        }
    }

    emit done(blackList);
    reset();
}
