#include "CPoint.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QTextItem>

const int CPoint::sPointRadius = 3;


CPoint::CPoint(const QPoint &p)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPos(p);
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

    QBrush br;
    br.setColor(Qt::black);
    painter->setBrush(br);
    painter->drawEllipse(QPoint(0,0), sPointRadius, sPointRadius);
    QFont font = painter->font();
    font.setPointSize(font.pointSize() * 0.7);
    painter->setFont(font);
    painter->drawText(
            QPoint(-1, -sPointRadius), 
            "(" + QString::number(pos().x()) + "," + QString::number(pos().y()) + ")"); 
}

void CPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    this->scene()->update(this->boundingRect().adjusted(-pos().x(), -pos().y(), pos().x(), pos().y()));
}

