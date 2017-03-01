/**
 * Project   AlgoVi
 *
 * @file     CSegmentCrossViz.cpp
 * @author   Dmytro Sadovyi
 * @date     20.12.2016
 */

#include "../CSegmentCrossViz.h"

namespace NView
{

CSegmentCrossViz::CSegmentCrossViz()
{

}

QString CSegmentCrossViz::serialize(const std::map<QString, IAlgoViItem *> &items)
{
    std::vector <std::vector <QPointF> > polygons;
    for(int i = 1; ; ++i)
    {
        polygons.push_back(std::vector <QPointF> ());
        QString pref = "p" + QString::number(i) + "#";
        for(int j = 1; ; ++j)
        {
            if(items.find(pref + QString::number(j)) == items.end())
                break;
            polygons.back().push_back(items.at(pref + QString::number(j))->scenePos());
        }
        if(polygons.back().empty())
        {
            polygons.pop_back();
            break;
        }
    }
    QPointF pt = items.at("Point#1")->scenePos();
    QString ret = QString::number(polygons.size()) + "\n";
    for(const std::vector <QPointF>& v : polygons)
    {
        ret += QString::number(v.size()) + " ";
        for(int i = 0; i < int(v.size()); ++i)
            ret += QString::number(v[i].x()) + " " + QString::number(v[i].y()) + " ";
        ret += "\n";
    }
    ret += QString::number(pt.x()) + " " + QString::number(pt.y()) + "\n";
    return ret;
}

void CSegmentCrossViz::realize(QString &data,
                           QPainter *painter,
                           const std::map <QString, IAlgoViItem*>& items)
{
    std::vector <std::vector <QPointF> > polygons;
    for(int i = 1; ; ++i)
    {
        polygons.push_back(std::vector <QPointF> ());
        QString pref = "p" + QString::number(i) + "#";
        for(int j = 1; ; ++j)
        {
            if(items.find(pref + QString::number(j)) == items.end())
                break;
            polygons.back().push_back(items.at(pref + QString::number(j))->scenePos());
        }
        if(polygons.back().empty())
        {
            polygons.pop_back();
            break;
        }
    }
    QTextStream stream(&data);
    int n;
    stream >> n;
    std::vector <bool> selected(polygons.size());
    for(int i = 0; i < n; ++i)
    {
        int t;
        stream >> t;
        selected[t] = 1;
    }
    for(int j = 0; j < int(polygons.size()); ++j)
    {
        const std::vector <QPointF>&v = polygons[j];

        QPen p = painter->pen();
        p.setWidth(2);
        p.setColor(selected[j] ? Qt::green : Qt::black);
        painter->setPen(p);

        for(int i = 0; i < int(v.size()); ++i)
            painter->drawLine(v[i], v[(i+1) % (v.size())]);
    }
}


} // namespace NView
