/**
 * Project   Graviz
 *
 * @file     CPoint.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include "View/components/IGravizItem.h"

namespace NView
{

class CPoint : public IGravizItem
{
public: // methods
    CPoint(const QString& name, const QPoint& p);
    ~CPoint();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    QString serialize() const override;

private: // fields
    static const int sPointRadius;
};

} // namespace NView
