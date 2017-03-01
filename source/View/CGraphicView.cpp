/**
 * Project   AlgoVi
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

#define DEFAULT_WIN_HEIGHT 10000
#define DEFAULT_WIN_WIDTH 10000

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
    emit mousePressed(event->scenePos().toPoint());
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

void CGraphicView::setObjectsMap(std::map<QString, IAlgoViItem *>* objectsMap)
{
    mObjectsMap = objectsMap;
}

void CGraphicView::addAlgoViItem(IAlgoViItem *item)
{
    this->addItem(item);
    item->update();
    connect(item, &IAlgoViItem::nameChanged, [this, item](const QString& oldName){
       emit objectChangeName(item, oldName);
    });
    emit objectAdded(item);
}

void CGraphicView::setDataToDraw(const QString &data)
{
    mDataToDraw = data;
}

} // namespace NView
