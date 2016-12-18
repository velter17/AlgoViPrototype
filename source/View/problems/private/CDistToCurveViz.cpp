/**
 * Project   Graviz
 *
 * @file     CDistToCurveViz.cpp
 * @author   Dmytro Sadovyi
 * @date     17.12.2016
 */

#include <QDebug>

#include "../CDistToCurveViz.h"
#include "View/components/IGravizItem.h"

namespace NView
{

CDistToCurveViz::CDistToCurveViz()
{

}

QString CDistToCurveViz::serialize(const std::map<QString, IGravizItem *> &items)
try
{
    if(items.size() < 4)
        return "";
    QString ret = items.at("Point#1")->serialize() + "\n";
    ret += items.at("Point#2")->serialize() + "\n";
    ret += items.at("Point#3")->serialize() + "\n";
    ret += items.at("Point#4")->serialize() + "\n";
    return ret;
}
catch(const std::out_of_range& error)
{
    qDebug () << "CDistToCurve::serialize(): exception: " << error.what();
}

void CDistToCurveViz::realize(QString &data,
                              QPainter *painter,
                              const std::map<QString, IGravizItem *> &items)
try
{
    if(items.size() < 4)
        return;
    QPainterPath path;
    path.moveTo(items.at("Point#1")->scenePos());
    path.cubicTo(items.at("Point#1")->scenePos(),
                 items.at("Point#2")->scenePos(),
                 items.at("Point#3")->scenePos());
    QPen p;
    p.setColor(Qt::darkBlue);
    p.setWidth(2);
    painter->setPen(p);
//    painter->drawPath(path);
    QPointF prev = items.at("Point#1")->scenePos();
    QPointF pt[3] = {items.at("Point#1")->scenePos(),
                    items.at("Point#2")->scenePos(),
                    items.at("Point#3")->scenePos()};
    for(int i = 1; i <= 100; ++i)
    {
        double t = (i+.0)/100;
        QPointF r;
        r.setX((1-t)*(1-t) * pt[0].x() + 2*t*(1-t)*pt[1].x() + t*t * pt[2].x());
        r.setY((1-t)*(1-t) * pt[0].y() + 2*t*(1-t)*pt[1].y() + t*t * pt[2].y());
        painter->drawLine(prev, r);
        prev = r;
    }
    p.setWidth(1);
    p.setColor(Qt::gray);
    painter->setPen(p);
    painter->drawLine(items.at("Point#1")->scenePos(), items.at("Point#2")->scenePos());
    painter->drawLine(items.at("Point#2")->scenePos(), items.at("Point#3")->scenePos());
    QTextStream stream(&data);
    double x, y;
    stream >> x >> y;
    p.setColor(Qt::red);
    p.setWidth(1);
    painter->setPen(p);
    painter->drawEllipse(QPointF(x, y), 6, 6);
    p.setColor(Qt::green);
    painter->setPen(p);
    painter->drawLine(QPointF(x, y), items.at("Point#4")->scenePos());
    p.setColor(Qt::black);
    painter->setPen(p);
    QPointF orient(items.at("Point#4")->scenePos().x() - items.at("Point#4")->getPrevPos().x(),
                  items.at("Point#4")->scenePos().y() - items.at("Point#4")->getPrevPos().y());
//    orient.rx() *= 10;
//    orient.ry() *= 10;
    qDebug () << "cur: " << items.at("Point#4")->scenePos() << ", prev = " << items.at("Point#4")->getPrevPos();
    qDebug () << "orient = " << orient;
    painter->drawLine(items.at("Point#4")->scenePos(), items.at("Point#4")->scenePos() + orient);
}
catch(const std::out_of_range& error)
{
    qDebug () << "CDistToCurve::realize(): exception: " << error.what();
}


} // namespace NView
