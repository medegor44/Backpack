#include "algo_base.h"

#include <algorithm>

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

    std::sort(textures.begin(), textures.end(),
              [](QGraphicsPixmapItem *p, QGraphicsPixmapItem *p1) {
        return p->pixmap().height() > p1->pixmap().height();
    });
}


void AlgoBase::sort()
{
    for(int i = 0; i < textures.size(); ++i) {
        int maxIndex = i;
        for(int j = i; j < textures.size(); ++j)
            if(textures[j]->pixmap().height() > textures[maxIndex]->pixmap().height())
                maxIndex = j;

        QGraphicsPixmapItem *tmp = textures[i];
        textures[i] = textures[maxIndex];
        textures[maxIndex] = tmp;
    }
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
