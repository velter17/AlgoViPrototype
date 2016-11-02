#ifndef CGEOMETRYVIEW_H
#define CGEOMETRYVIEW_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <fstream>

class CGraphicView : public QGraphicsScene
{
    Q_OBJECT
public:
    CGraphicView();
    ~CGraphicView();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void drawForeground(QPainter *painter, const QRectF &rect);

private:
    bool mSolverEnabled;
};

#endif // CGEOMETRYVIEW_H
