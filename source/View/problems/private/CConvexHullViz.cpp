/**
 * Project   Graviz
 *
 * @file     CConvexHullViz.cpp
 * @author   Dmytro Sadovyi
 * @date     17.12.2016
 */

#include "../CConvexHullViz.h"

namespace NView
{

CConvexHullViz::CConvexHullViz()
{

}

QString CConvexHullViz::serialize(const std::map<QString, IGravizItem *> &items)
{
    QString ret = QString::number(items.size()) + "\n";
    for(const auto itr : items)
    {
        ret += itr.second->serialize() + "\n";
    }
    return ret;
}

void CConvexHullViz::realize(QString &data,
                             QPainter *painter,
                             const std::map <QString, IGravizItem*>& items)
{
    QTextStream stream(&data);
    int n;
    stream >> n;
    QPolygon convex_hull;
    for(int i = 0; i < n; ++i)
    {
        int x, y;
        stream >> x >> y;
        convex_hull.append(QPoint(x, y));
    }
    QPen p;
    p.setColor(Qt::red);
    p.setWidth(2);
    painter->setPen(p);
    painter->drawPolygon(convex_hull);
}


} // namespace NView
