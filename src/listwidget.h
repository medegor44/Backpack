/**
  * Контейнер изображений
  **/

#ifndef LISTWIDGET
#define LISTWIDGET

#define DEBUG

#include <QListWidget>
#include <QDrag>
#include <QMimeData>
#include <QListWidgetItem>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDataStream>

#include <time.h>

#ifdef DEBUG
#include <QDebug>
#endif

#include <QGraphicsPixmapItem>

class ListWidget : public QListWidget
{
    Q_OBJECT

protected:
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    bool isContain(QPixmap pix);

public slots:
    void addReturnedItems(QList<QGraphicsPixmapItem *>items);

public:
    ListWidget(QWidget *parent = 0);
    void addImage(QPixmap pic);
    ~ListWidget()
    {}
};

#endif // LISTWIDGET

