/**
 * Project   Graviz
 *
 * @file     CGraphicView.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <fstream>

namespace NView
{

class CGraphicView : public QGraphicsScene
{
    Q_OBJECT
public: // methods
    CGraphicView();
    ~CGraphicView();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void drawForeground(QPainter *painter, const QRectF &rect);
};

} // namespace NView
