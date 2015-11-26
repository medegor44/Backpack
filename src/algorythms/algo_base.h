#ifndef ALGO_BASE
#define ALGO_BASE

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

enum mode { StaticAtlas, MinimalAtlas };

class AlgoBase : public QObject
{
    Q_OBJECT

protected:
    QRectF atlasRect;
    QGraphicsScene *parent;
    QList<QGraphicsPixmapItem *> textures;
    QList<QGraphicsPixmapItem *> blackList;
    mode m;
    void sort();

signals:
    void done(bool ok);
    void done(QList <QGraphicsPixmapItem *> items);

public:
    AlgoBase(QGraphicsScene *parent);

    inline void setMode(mode m)
    { this->m = m; }

    int getMaxAtlasSide();
    void saveImages(QList <QGraphicsItem *> items);

    inline void clearTexturesList()
    { textures.clear(); }

    virtual void start() = 0;

    ~AlgoBase() {}
};

#endif
