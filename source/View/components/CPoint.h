#ifndef CPOINT_H
#define CPOINT_H

#include <QGraphicsItem>
#include <QPoint>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

class CPoint : public QGraphicsItem
{
public: // functions
    CPoint(const QPoint& p);
    ~CPoint();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    static const int sPointRadius;
};

#endif // CPOINT_H
