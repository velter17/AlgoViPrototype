/**
 * Project   Graviz
 *
 * @file     CGraphicView.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QDebug>
#include <vector>
#include <QProcess>
#include <fstream>

#include "View/CGraphicView.h"
#include "View/components/CPoint.h"

#define DEFAULT_WIN_HEIGHT 1000
#define DEFAULT_WIN_WIDTH 1000

namespace NView
{

CGraphicView::CGraphicView()
{
    this->setSceneRect(0, 0, DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);
}

CGraphicView::~CGraphicView()
{

}

void CGraphicView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    this->addItem(new CPoint("name", event->scenePos().toPoint));
    if(this->selectedItems().empty())
    {
        emit mousePressed(event->scenePos().toPoint());
    }
    QGraphicsScene::mousePressEvent(event);
}

void CGraphicView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawForeground(painter, rect);
    if(0 != mVisualizer && !mDataToDraw.isEmpty())
    {
        mVisualizer->realize(mDataToDraw, painter, *mObjectsMap);
    }
    this->update();
}

void CGraphicView::setVisualizer(std::shared_ptr<IProblemVisualizer> visualizer)
{
    mVisualizer = visualizer;
}

void CGraphicView::setObjectsMap(std::map<QString, IGravizItem *>* objectsMap)
{
    mObjectsMap = objectsMap;
}

void CGraphicView::addGravizItem(IGravizItem *item)
{
    this->addItem(item);
    item->update();
    emit objectAdded(item);
}

void CGraphicView::setDataToDraw(const QString &data)
{
    mDataToDraw = data;
}

} // namespace NView
