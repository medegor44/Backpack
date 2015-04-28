#include "scene.h"

Scene::Scene(const QRectF sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent)
{
    save = false;
    mode = false;

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
{
    QGraphicsItem *item; // Графический элемент

    if(item = itemAt(event->scenePos(), QTransform())) {
        startDragging = event->scenePos(); // Позиция начала
        relativePoint = QPointF(event->scenePos().x() - item->x(),
                                event->scenePos().y() - item->y());
        // Сохранение позиции курсора относительно изображения
        qDebug() << "Pos saved";
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(itemAt(event->scenePos(), QTransform())) {
        int dist = (event->scenePos() - startDragging).manhattanLength();
        qDebug() << "dist =" << dist;
        startDrag(event->scenePos());
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::startDrag(QPointF start)
{
    QGraphicsPixmapItem *item = dynamic_cast<QGraphicsPixmapItem *>
            (itemAt(start, QTransform())); // Получить теекущий элемент

    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    stream << item->pixmap(); // Запись изображения в массив байтов

    QDrag *drag = new QDrag(this);
    QMimeData *data = new QMimeData();
    data->setData("Picture", arr);
    drag->setMimeData(data);
    drag->setPixmap(item->pixmap());
    drag->setHotSpot(QPoint(/*startDragging.x() - */item->pixmap().width()/2,
                            /*startDragging.y() - */item->pixmap().height()/2));

    removeItem(item); // удалить картинку
    drag->exec(Qt::MoveAction); // Начать выполнение Drag & Drop
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("Picture")) {
        event->accept();
        relativePoint = QPointF();
    } else
        event->ignore();
}

bool Scene::isInRange(QPointF point)
{
    return !(point.x() < 0 || point.y() < 0) ||
            (point.x() > width() || point.y() > height()); /*return false;*/
    //return true;
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("Picture") &&
            isInRange(event->scenePos())
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
        QByteArray arr;
        QDataStream stream(&arr, QIODevice::ReadOnly);
        QPixmap pic;

        arr = event->mimeData()->data("Picture");
        stream >> pic;

        QGraphicsPixmapItem *item = addPixmap(pic);
        item->setFlags(QGraphicsItem::ItemIsMovable);

        qDebug() << "Pos:" << event->pos();
//        item->setPos(QPointF(event->scenePos().x() - relativePoint.x(),
//                             event->scenePos().y() - relativePoint.y()));

        //if(relativePoint.isNull())
            item->setPos(QPointF(event->scenePos().x() - pic.width()/2,
                                 event->scenePos().y() - pic.height()/2));
//        else
//            item->setPos(QPointF(event->scenePos().x() - relativePoint.x(),
//                                 event->scenePos().y() - relativePoint.y()));
        event->accept();
    } else
        event->ignore();

   // relativePoint = QPointF();
}
