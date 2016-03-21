#include "guillotine_algo.h"

#include <QDebug>

// Описание методов класса Rect
Rect::Rect(QRectF rect)
{
    this->rect = rect;
}

bool Rect::isPlaced(QGraphicsPixmapItem *item)
{
    return item->pixmap().height() <= rect.height() &&
           item->pixmap().width() <= rect.width();
}

double Rect::score(QGraphicsPixmapItem *item)
{
    return (rect.width() * rect.height()) -
            (item->pixmap().width() * item->pixmap().height());
}

Rect *Rect::placeHere(QGraphicsPixmapItem *item)
{
    int w = item->pixmap().width();
    int h = item->pixmap().height();

    item->setPos(rect.topLeft());
    Rect *newRect;

    // Разбитие прямоугольника перпендикулярно большей стороне
    if(w >= h) {
        newRect = new Rect(QRectF(
                               rect.x() + w,
                               rect.y(),
                               rect.width() - w,
                               rect.height()));

        rect.setWidth(w);
        //rect.setHeight(rect.height() - h);
        rect.setY(item->y() + h);
    } else {
        newRect = new Rect(QRectF(
                               rect.x(),
                               rect.y() + h,
                               rect.width(),
                               rect.height() - h));

        rect.setHeight(h);
        //rect.setWidth(rect.width() - w);
        rect.setX(item->x() + w);
    }

    //qDebug() << rect;

    return newRect;
}

// Опиание методов класса GuillotineAlgo

GuillotineAlgo::GuillotineAlgo(QGraphicsScene *parent)
    : AlgoBase(parent)
{}
Rect *GuillotineAlgo::getBestRect(QGraphicsPixmapItem *item)
{
    Rect *bestRect = nullptr;

    for(Rect *r : rects) {
        if(r->isPlaced(item) &&
                (bestRect == nullptr ||
                 r->score(item) < bestRect->score(item)))
            bestRect = r;
    }

    return bestRect;
}

void GuillotineAlgo::start()
{
    atlasRect = parent->sceneRect();
    std::sort(textures.begin(), textures.end(),
              [](QGraphicsPixmapItem *p, QGraphicsPixmapItem *p1) {
        return p->pixmap().height() > p1->pixmap().height();
    });
    rects.push_back(new Rect(atlasRect));

    for(auto it = textures.begin(); it != textures.end(); ++it) {
        Rect *bestRect = getBestRect(*it);

        if(bestRect != nullptr)
            rects.push_back(bestRect->placeHere(*it));
        else {
            if(m == mode::MinimalAtlas) { // Включен режим поиска минимального атласа
                reset();
                emit done(false);
                return;
            } else { // Уаковка в статичный атлас
                blackList.push_back(*it);
                parent->removeItem(*it);
            }
        }
    }

    if(m == mode::MinimalAtlas)
        emit done(true);
    else
        emit done(blackList);

    reset();
}

void GuillotineAlgo::reset()
{
    for(Rect *r : rects)
        delete r;

    rects.clear();
    blackList.clear();
//    textures.clear();

    //textures.clear();
}
