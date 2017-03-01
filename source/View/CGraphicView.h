/**
 * Project   AlgoVi
 *
 * @file     CGraphicView.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <fstream>
#include <memory>

#include "View/components/IAlgoViItem.h"
#include "View/problems/IProblemVisualizer.h"

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

    void setVisualizer(std::shared_ptr<IProblemVisualizer> visualizer);
    void setObjectsMap(std::map <QString, IAlgoViItem*>* objectsMap);

public slots:
    void addAlgoViItem(IAlgoViItem *item);
    void setDataToDraw(const QString& data);

signals:
    void mousePressed(const QPoint& p);
    void objectAdded(IAlgoViItem* item);
    void objectChangeName(IAlgoViItem* item, QString oldName);

private: // fields
    std::shared_ptr<IProblemVisualizer> mVisualizer;
    std::map <QString, IAlgoViItem*>* mObjectsMap;
    QString mDataToDraw;
};

} // namespace NView
