/**
 * Project   AlgoVi
 *
 * @file     CPoint.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include "View/components/IAlgoViItem.h"

namespace NView
{

class CPoint : public IAlgoViItem
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
