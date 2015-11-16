#include "scene.h"

Scene::Scene(const QRectF sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent)
{
    save = false;
    mode = false; // ??

    algorythms[0] = new NextFitAlgo(this);
    algorythms[1] = new BestFitAlgo(this);
    algorythms[2] = new GuillotineAlgo(this);

    for(AlgoBase *a : algorythms) {
        connect(a, SIGNAL(done(bool)), this, SLOT(changeSceneRect(bool)));
        connect(a, SIGNAL(done(QList<QGraphicsPixmapItem*>)),
                this, SIGNAL(finished(QList<QGraphicsPixmapItem*>)));
    }
}

void Scene::startAlgorythm(int index)
{
    algorythms[index]->saveImages(items()); // Сохраниить изображения в алгоритм
    result = false;

    if(mode) { // Поиск минимального атласа
        setSceneRect(0, 0, 2, 2);
        while(!result)
            algorythms[index]->start();
    } else // Работа со статичным атласом
        algorythms[index]->start();

    algorythms[index]->clearTexturesList(); // Очистка списка текстур в атласе
}

void Scene::changeSceneRect(bool res)
{
   if(!(result = res))
       setSceneRect(0, 0, width() * 2, width() * 2);
}

void Scene::setMode(int m)
{
    switch(m) {
    case 0: // Статичный атлас
        mode = false;

        for(AlgoBase *a : algorythms)
            a->setMode(::mode::StaticAtlas);

        break;
    case 2: // Динамический
        mode = true;

        for(AlgoBase *a : algorythms)
            a->setMode(::mode::MinimalAtlas);

        break;
    }
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    painter->setPen(Qt::gray);
    painter->drawRect(-1, -1, width()+1, height()+1);

    if(!save)
        painter->fillRect(sceneRect(), Qt::Dense6Pattern);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{/*
    QGraphicsItem *item;
    if((item = itemAt(event->scenePos(), QTransform()))) {
        // Если существует элемент, на который кликнули

        startDragging = event->scenePos();
        // Сохраняем позицию клика относительно сцены

    }*/

    qDebug() << "Scene's rectangle" << sceneRect();

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(itemAt(event->scenePos(), QTransform()))
        startDrag(event->scenePos());

    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::startDrag(QPointF start)
{
    QGraphicsPixmapItem *item =
            dynamic_cast<QGraphicsPixmapItem *> (itemAt(start, QTransform()));

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream << item->pixmap(); // Сохраняем картинку в буфер

    QDrag *drag = new QDrag(this);
    QMimeData *data = new QMimeData();

    data->setData("Picture", buffer);
    drag->setMimeData(data);
    drag->setPixmap(item->pixmap());

    relativePoint = item->mapFromScene(start);

    qDebug() << "Position at image:" << relativePoint;
    drag->setHotSpot(QPoint(relativePoint.x(), relativePoint.y()));

    removeItem(item); // удалить картинку
    internalMoving = true;

    drag->exec(Qt::MoveAction); // Начать выполнение Drag & Drop
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{/*
    if(event->mimeData()->hasFormat("Picture")) {
        event->accept();
//        relativePoint = QPointF();
    } else
        event->ignore();*/

    QGraphicsScene::dragEnterEvent(event);
}

bool Scene::isInRange(QPointF p)
{
//    return !(point.x() < 0 || point.y() < 0) || !(point.x() > width() || point.y() > height());
    return (p.x() >= 0 && p.y() >= 0 && p.x() < width() && p.y() < height());
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("Picture") && isInRange(event->scenePos())
            && !(itemAt(event->scenePos(), QTransform()))) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("Picture")) {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadOnly);
        QPixmap picture;

        buffer = event->mimeData()->data("Picture");
        stream >> picture;

        QGraphicsPixmapItem *item = addPixmap(picture);
        item->setFlags(QGraphicsItem::ItemIsMovable);

        if (!internalMoving) {
            item->setPos(QPointF(event->scenePos().x() - picture.width()/2,
                                 event->scenePos().y() - picture.height()/2));
        } else {
            item->setPos(event->scenePos());
            item->moveBy(-relativePoint.x(), -relativePoint.y());
        }
        internalMoving = false;
        event->accept();
    } else
        event->ignore();

   // relativePoint = QPointF();
}
