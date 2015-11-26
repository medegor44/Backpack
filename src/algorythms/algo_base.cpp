#include "algo_base.h"

#include <algorithm>

using std::begin;
using std::end;

AlgoBase::AlgoBase(QGraphicsScene *parent)
    : QObject(parent)
{
//    atlasRect = parent->sceneRect();
    this->parent = parent;
}

void AlgoBase::saveImages(QList<QGraphicsItem *> items)
{
    for(QGraphicsItem *i : items)
        textures.push_back(dynamic_cast<QGraphicsPixmapItem *>(i));

//    textures.sort([](QGraphicsPixmapItem *p, QGraphicsPixmapItem *p1) {
//                return p->pixmap().height() > p1->pixmap().height();
//            });

    std::sort(begin(textures), end(textures), [](QGraphicsPixmapItem *p, QGraphicsPixmapItem *p1) {
                        return p->pixmap().height() > p1->pixmap().height();
                    });
}

int AlgoBase::getMaxAtlasSide()
{
    int width = 0;
    int height = 0;

    for(QGraphicsPixmapItem *i : textures) {
        width += i->pixmap().width();
        height += i->pixmap().height();
    }

    return width > height ? width : height;
}
