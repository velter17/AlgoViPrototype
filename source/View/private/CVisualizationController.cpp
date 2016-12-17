/**
 * Project   Graviz
 *
 * @file     CVisualizationController.cpp
 * @author   Dmytro Sadovyi
 * @date     16.12.2016
 */

#include "../CVisualizationController.h"
#include "View/components/CPoint.h"

namespace NView
{

CVisualizationController::CVisualizationController(
        std::shared_ptr<CGraphicView> view,
        std::shared_ptr<IProblemVisualizer> visualizer)
    : mView(view)
    , mVisualizer(visualizer)
{
    connect(mView.get(), &CGraphicView::objectAdded, [this](IGravizItem* item){
        mObjectsMap[item->getName()] = item;
    });
    connect(mView.get(), &CGraphicView::changed, [this](const QList<QRectF>& region){
        update();
    });
    connect(mView.get(), SIGNAL(mousePressed(QPoint)), this, SLOT(addPoint(QPoint)));
    mView->setVisualizer(mVisualizer);
    mView->setObjectsMap(&mObjectsMap);
}

CVisualizationController::~CVisualizationController()
{
    mView->clear();
    mView->setDataToDraw("");
}

void CVisualizationController::addPoint(const QPoint &p)
{
    static int num = 1;
    qDebug () << "add Point in pos " << p;
    QString name = "Point#" + QString::number(num++);
    mView->addGravizItem(new CPoint(name, p));

}

void CVisualizationController::updateResult(const QString &data)
{
    mView->setDataToDraw(data);
}

void CVisualizationController::update()
{
    if(mObjectsMap.size() > 2)
        emit sceneChanged(mVisualizer->serialize(mObjectsMap));
}


} // namespace NView
