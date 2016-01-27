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
//    textures.clear();
}

void NextFitAlgo::start()
{
    atlasRect = parent->sceneRect();
    double currentHeight = 0;

    for(auto it = textures.begin(); it != textures.end(); ++it) {
        if(atlasRect.width() - currentPos.x() >= (*it)->pixmap().width() &&
                atlasRect.height() - currentHeight >= (*it)->pixmap().height()) {
            (*it)->setPos(currentPos);

            if((*it)->pixmap().height() + currentPos.y() > currentHeight)
                currentHeight = (*it)->pixmap().height() + currentPos.y();

            currentPos.setX(currentPos.x() + (*it)->pixmap().width());
        } else if(atlasRect.height() - currentHeight >= (*it)->pixmap().height()) {
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

    if(m == mode::MinimalAtlas)
        emit done(true);
    else
        emit done(blackList);

    reset();
}
