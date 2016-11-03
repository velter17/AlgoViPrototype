/**
 * Project   Graviz
 *
 * @file     CPoint.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QGraphicsItem>
#include <QPoint>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

namespace NView
{

class CPoint : public QGraphicsItem
{
public: // methods
    CPoint(const QPoint& p);
    ~CPoint();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private: // fields
    static const int sPointRadius;
};

} // namespace NView
