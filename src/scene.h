/**
  * Сцена, на которой происходит сборка графического атласа
  **/
#ifndef SCENE
#define SCENE

#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QGraphicsItem>

#include <QDebug>

#include "algorythms/next_fit_algo.h"
#include "algorythms/best_fit_algo.h"
#include "algorythms/guillotine_algo.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

private:
    QPointF startDragging;
    QPointF relativePoint;
    QRectF dropPlace;
//    AlgoBase *algorythms[3];
    AlgoBase *algorythms[3];

    int upperBound;
    int lowerBound;
    int middleBound; // Медиана

    bool mode; // Режим запуска
    bool save;
    bool result;

    void startDrag(QPointF start);
    bool isInRange(QPointF point);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

signals:
    void finished(QList<QGraphicsPixmapItem *>blackList);

public slots:
    void setMode(int m);

private slots:
    void changeSceneRect(bool res); // Изменение размера атласа

public:
    Scene(const QRectF sceneRect, QObject *parent = 0);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void startAlgorythm(int index);
    void setUnsetSaveMode(bool m)
    { save = m; }

    ~Scene()
    { for(AlgoBase *a : algorythms) delete a;}
};

#endif // SCENE

