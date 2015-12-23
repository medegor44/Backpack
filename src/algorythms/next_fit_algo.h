#include "algo_base.h"

class NextFitAlgo : public AlgoBase
{
    Q_OBJECT

private:
    QPointF currentPos;
    void reset();

public:
    NextFitAlgo(QGraphicsScene *parent);

    virtual void start();

    ~NextFitAlgo(){}
};
