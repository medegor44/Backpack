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
#include "algorythms/max_rects_algo.h"
#include "algorythms/square_algo.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

private:
    const int alogoCount = 5;
    QPointF startDragging;
    QPointF relativePoint;
    QRectF dropPlace;
    AlgoBase *algorithms[5];

    bool mode;

    // Режим сохранения изображения
    bool save;

    bool result;
    bool internalMoving;

    void startDrag(QPointF start);
    bool isInRange(QPointF p);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

signals:
    void returnItems(QList<QGraphicsPixmapItem *>blackList);

public slots:
    void setMode(int m);

private slots:
    void changeSceneRect(bool res); // Изменение размера атласа

public:
    Scene(const QRectF sceneRect, QObject *parent = 0);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void startAlgorythm(int index);
    void setUnsetSaveMode(bool m);
    void clearTextures();

    ~Scene();
};

#endif // SCENE

