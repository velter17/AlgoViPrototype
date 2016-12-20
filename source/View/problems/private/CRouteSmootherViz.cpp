/**
 * Project   Graviz
 *
 * @file     CRouteSmootherViz.cpp
 * @author   Dmytro Sadovyi
 * @date     20.12.2016
 */

#include "../CRouteSmootherViz.h"
#include "framework/algorithms/CGeometry.h"
#include <QDebug>

namespace NView
{

CRouteSmootherViz::CRouteSmootherViz()
{

}

QString CRouteSmootherViz::serialize(const std::map<QString, IGravizItem *> &items)
{
    QPointF pt[3];
    bool flag = false;
    QString ret;
    int i;
    for(i = 1; items.find("Point#"+QString::number(i)) != items.end(); ++i)
    {
        pt[2] = items.at("Point#" + QString::number(i))->scenePos();
        if(i > 2)
        {
            if(flag)
            {
                flag = false;
                pt[0] = pt[1];
                pt[1] = pt[2];
                continue;
            }
            if(std::abs(NGeometry::angle(pt[0], pt[1], pt[2]) - NGeometry::pi / 2) < 0.2 &&
               std::abs(NGeometry::len(pt[0], pt[1]) - NGeometry::len(pt[1], pt[2])) < 10)
            {
                ret += items.at("Point#" + QString::number(i-1))->serialize() + " 0\n";
                ret += items.at("Point#" + QString::number(i))->serialize() + " 1\n";
                flag = true;
            }
            else
            {
                ret += items.at("Point#" + QString::number(i-1))->serialize() + " 0\n";
                flag = false;
            }
        }
        else if(i < 2)
        {
            ret += items.at("Point#" + QString::number(i))->serialize() + " 0\n";
        }
        pt[0] = pt[1];
        pt[1] = pt[2];
    }
    ret = QString::number(i-1) + "\n" + ret;
    if(!flag)
        ret += items.at("Point#" + QString::number(i-1))->serialize() + " 0\n";
    ret += items.at("Gps#1")->serialize() + "\n";
    return ret;
}

void CRouteSmootherViz::realize(QString &data,
                           QPainter *painter,
                           const std::map <QString, IGravizItem*>& items)
{
    qDebug () << "CRouteSmootherViz > realize";
    QPointF pt[3];
    bool flag = false;
    QPen pcurve;
    pcurve.setColor(Qt::gray);
    pcurve.setWidth(1);
    QPen pline;
    pline.setColor(Qt::blue);
    pline.setWidth(2);
    for(int i = 1; items.find("Point#"+QString::number(i)) != items.end(); ++i)
    {
        pt[2] = items.at("Point#" + QString::number(i))->scenePos();
        if(i > 1)
        {
            painter->setPen(pline);
            painter->drawLine(pt[1], pt[2]);
        }
        if(i > 2)
        {
            if(flag)
            {
                flag = false;
                pt[0] = pt[1];
                pt[1] = pt[2];
                continue;
            }
            if(std::abs(NGeometry::angle(pt[0], pt[1], pt[2]) - NGeometry::pi / 2) < 0.2 &&
               std::abs(NGeometry::len(pt[0], pt[1]) - NGeometry::len(pt[1], pt[2])) < 10)
            {
                QPointF prev = pt[0];
                for(int j = 1; j <= 100; ++j)
                {
                    double t = (j+.0)/100;
                    QPointF r;
                    r.setX((1-t)*(1-t) * pt[0].x() + 2*t*(1-t)*pt[1].x() + t*t * pt[2].x());
                    r.setY((1-t)*(1-t) * pt[0].y() + 2*t*(1-t)*pt[1].y() + t*t * pt[2].y());
                    painter->setPen(pcurve);
                    painter->drawLine(prev, r);
                    prev = r;
                }
                flag = true;
            }
            else
            {
                painter->setPen(pline);
                painter->drawLine(pt[0], pt[1]);
                flag = false;
            }
        }
        pt[0] = pt[1];
        pt[1] = pt[2];
    }
//    if(!flag)
//    {
//        painter->setPen(pline);
//        painter->drawLine(pt[0], pt[1]);
//    }
    QTextStream stream(&data);
    double x, y;
    stream >> x >> y;
    QPen p;
    p.setColor(Qt::green);
    p.setWidth(1);
    painter->setPen(p);
    painter->drawLine(items.at("Gps#1")->scenePos(), QPointF(x, y));
    p.setColor(Qt::red);
    p.setWidth(1);
    painter->setPen(p);
    painter->drawEllipse(QPointF(x, y), 3, 3);
}


} // namespace NView
