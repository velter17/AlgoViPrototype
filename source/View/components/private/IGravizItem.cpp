/**
 * Project   Graviz
 *
 * @file     IGravizItem.cpp
 * @author   Dmytro Sadovyi
 * @date     16.12.2016
 */

#include <QInputDialog>

#include "../IGravizItem.h"

namespace NView
{

const int IGravizItem::sPrevQueueSize = 10;

IGravizItem::IGravizItem(const QString& name, const QPoint& p)
    : mName(name)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPos(p);
    for(int i = 0; i < sPrevQueueSize; ++i)
        mPrevPosQueue.push(p);
}

QString IGravizItem::getName() const
{
    return mName;
}

QPointF IGravizItem::getPrevPos() const
{
    return mPrevPosQueue.front();
}

void IGravizItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QBrush br;
    br.setColor(Qt::black);
    QFont font = painter->font();
    font.setPointSize(font.pointSize() * 1);
    painter->setFont(font);
    painter->drawText(
            QPoint(-1, -1),
                mName);
}

void IGravizItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    bool ok;
    QString text = QInputDialog::getText(nullptr, mName,
                                        QString("New item name:"), QLineEdit::Normal,
                                        QString(""), &ok);
    if (ok && !text.isEmpty())
    {
        QString oldName = mName;
        mName = text;
        this->update();
        emit nameChanged(oldName);
    }
}

void IGravizItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->lastScenePos() != event->scenePos())
        mPrevPosQueue.push(event->lastScenePos());
    while(mPrevPosQueue.size() > sPrevQueueSize)
        mPrevPosQueue.pop();

    QGraphicsItem::mouseMoveEvent(event);
}


} // namespace NView
