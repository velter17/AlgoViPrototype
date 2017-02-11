/**
 * Project   AlgoVi
 *
 * @file     IAlgoViItem.h
 * @author   Dmytro Sadovyi
 * @date     16.12.2016
 */

#pragma once

#include <QGraphicsItem>
#include <QPoint>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <queue>

namespace NView
{

class IAlgoViItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public: // methods
    IAlgoViItem(const QString& name, const QPoint& p);

    QString getName() const;
    QPointF getPrevPos() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QString serialize() const = 0;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void nameChanged(QString);

protected: // fields
    QString mName;
    static const int sPrevQueueSize;
    std::queue<QPointF> mPrevPosQueue;
};

} // namespace NView

