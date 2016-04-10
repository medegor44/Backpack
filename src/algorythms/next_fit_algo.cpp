#include "next_fit_algo.h"

NextFitAlgo::NextFitAlgo(QGraphicsScene *parent)
    : AlgoBase(parent)
{
    currentPos = QPointF();
}

void NextFitAlgo::reset()
{
    currentPos = QPointF();
    blackList.clear();
}

void NextFitAlgo::start()
{
    atlasRect = parent->sceneRect();

    std::sort(textures.begin(), textures.end(),
              [](QGraphicsPixmapItem *p, QGraphicsPixmapItem *p1) {
        return p->pixmap().height() > p1->pixmap().height();
    });

    double currentHeight = 0;

    for(auto it = textures.begin(); it != textures.end(); ++it) {
        if(atlasRect.width() - currentPos.x() >= (*it)->pixmap().width() &&
                atlasRect.height() - currentHeight >= (*it)->pixmap().height()) {
        // Помещаем изображение на текущий уровень, если это возможно
            (*it)->setPos(currentPos);

            if((*it)->pixmap().height() + currentPos.y() > currentHeight)
                currentHeight = (*it)->pixmap().height() + currentPos.y();

            currentPos.setX(currentPos.x() + (*it)->pixmap().width());
        } else if(atlasRect.height() - currentHeight >= (*it)->pixmap().height()) {
            // Помещаем изображение на следующий уровень, если это возможно
            currentPos.setY(currentHeight);
            currentPos.setX(0);

            (*it)->setPos(currentPos);
            currentPos.setX(currentPos.x() + (*it)->pixmap().width());
            currentHeight = (*it)->pixmap().height() + currentPos.y();
        } else {
            if(m == mode::MinimalAtlas) {
                reset();
                emit done(false);
                return;
            } else {
                blackList.push_back(*it);
                parent->removeItem(*it);
            }
        }
    }

#ifdef TEST
    cheackArea();
#endif

    if(m == mode::MinimalAtlas)
        emit done(true);
    else
        emit done(blackList);

    reset();
}
