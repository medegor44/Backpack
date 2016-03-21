#include "best_fit_algo.h"

Level::Level(QRectF rect)
{
    this->rect = rect;
    currentLenght = 0;
}

bool Level::isPlaced(QGraphicsPixmapItem *item)
{
    return item->pixmap().height() <= rect.height() &&
           item->pixmap().width() <= rect.width();
}

double Level::score(QGraphicsPixmapItem *item)
{
    return rect.width() * rect.height() -
           item->pixmap().width() * item->pixmap().height();
}

void Level::placeHere(QGraphicsPixmapItem *item)
{
    item->setPos(rect.topLeft()); // Установить изображение на позицию прямоугольника

    rect.setX(rect.x() + item->pixmap().width()); // Сдвинуть по X
    //rect.setWidth(rect.width() - item->pixmap().width()); // Уменьшить размер
}

BestFitAlgo::BestFitAlgo(QGraphicsScene *parent)
    : AlgoBase(parent)
{
}

Level *BestFitAlgo::getBestLevel(QGraphicsPixmapItem *item)
{
    Level *bestLevel = nullptr;

    for(Level *l : levels) {
        if(l->isPlaced(item) && (bestLevel == nullptr ||
                                 l->score(item) < bestLevel->score(item))) {
            bestLevel = l;
        }
    }

    return bestLevel;
}

void BestFitAlgo::reset()
{
    for(Level *l : levels)
        delete l;
    levels.clear();
//    textures.clear();
    blackList.clear();
}

void BestFitAlgo::start()
{
    atlasRect = parent->sceneRect();
    std::sort(textures.begin(), textures.end(),
              [](QGraphicsPixmapItem *p, QGraphicsPixmapItem *p1) {
        return p->pixmap().height() > p1->pixmap().height();
    });

    for(auto it = textures.begin(); it != textures.end(); ++it) {
        Level *bestLevel = getBestLevel(*it);

        if(bestLevel != nullptr)
            bestLevel->placeHere(*it);
        else { // Уровень не найден
            if((*it)->pixmap().width() <= atlasRect.width() &&
                    (*it)->pixmap().height() <= atlasRect.height()) {
                // Изображение умаещается в атлас
                Level *level = new Level(QRectF(atlasRect.x(),
                                                atlasRect.y(),
                                                atlasRect.width(),
                                                (*it)->pixmap().height()));
                // Создать новый уровень

                level->placeHere(*it); // Поместить изображение на уровень
                levels.push_back(level); // Занести в список

                atlasRect.setY(atlasRect.y() + (*it)->pixmap().height());
                //atlasRect.setHeight(atlasRect.height() - (*it)->pixmap().height());
                // Изменить размеры и положение атласа
            } else { // Изображение не умещается в атлас
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
    }

    if(m == mode::MinimalAtlas)
        emit done(true);
    else
        emit done(blackList);
    reset();
}
