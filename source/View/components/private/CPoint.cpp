/**
 * Project   AlgoVi
 *
 * @file     CPoint.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include "View/components/CPoint.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QTextItem>

namespace NView
{

const int CPoint::sPointRadius = 4;


CPoint::CPoint(const QString& name, const QPoint &p)
    : IAlgoViItem(name, p)
{
    qDebug () << "created Point " << name << " " << p;
}

CPoint::~CPoint()
{

}

QRectF CPoint::boundingRect() const
{
    static const int margin = 5;
    return QRectF(QPoint(-sPointRadius-margin,-sPointRadius-margin), 
                  QPoint(sPointRadius+margin+50, sPointRadius+margin));
}

void CPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*QStyleOptionGraphicsItem myoption = (*option);
    myoption.state &= !QStyle::State_Selected;
    QGraphicsItem::paint(painter, &myoption, widget);*/

    Q_UNUSED(option);
    Q_UNUSED(widget);

    IAlgoViItem::paint(painter, option, widget);

    QBrush br;
    br.setColor(Qt::black);
    painter->setBrush(br);
    painter->drawEllipse(QPoint(0,0), sPointRadius, sPointRadius);
}

void CPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    IAlgoViItem::mouseMoveEvent(event);
    this->scene()->update(this->boundingRect().adjusted(-pos().x(), -pos().y(), pos().x(), pos().y()));
}

QString CPoint::serialize() const
{
    return QString::number(this->scenePos().x())  + " " + QString::number(this->scenePos().y());
}

} // namespace NView
