#include "algo_base.h"

#include <algorithm>

using std::begin;
using std::end;

#ifdef TEST
void AlgoBase::cheackArea()
{
    int restArea = 0;
    for (auto i : blackList)
        restArea += i->pixmap().width() * i->pixmap().height();

    qDebug() << "======== Report ========"
             << (area - restArea) / ((atlasRect.width() + atlasRect.x()) * (atlasRect.height() + atlasRect.y()));
}
#endif

AlgoBase::AlgoBase(QGraphicsScene *parent)
    : QObject(parent)
{
    this->parent = parent;
#ifdef TEST
    area = 0;
#endif
}

void AlgoBase::saveImages(QList<QGraphicsItem *> items)
{
#ifdef TEST
    area = 0;
#endif
    for(QGraphicsItem *i : items) {
#ifdef TEST
        QGraphicsPixmapItem *item = dynamic_cast<QGraphicsPixmapItem *>(i);
        area += item->pixmap().width() * item->pixmap().height();
        textures.push_back(item);
#else
        textures.push_back(dynamic_cast<QGraphicsPixmapItem *>(i));
#endif
    }
}

void AlgoBase::clearTexturesList()
{
    textures.clear();
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
