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
    const int gridSize = 10;
    painter->setOpacity(0.15);
    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);
    QVarLengthArray<QLineF, 100> lines;
    for (qreal x = left; x < rect.right(); x += gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));
    painter->drawLines(lines.data(), lines.size());
    painter->setOpacity(1.0);

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
    connect(item, &IAlgoViItem::nameChanged, [this, item](QString oldName){
        emit objectChangedName(item, oldName);
    });
    emit objectAdded(item);
}

void CGraphicView::setDataToDraw(const QString &data)
{
    mDataToDraw = data;
}

} // namespace NView
