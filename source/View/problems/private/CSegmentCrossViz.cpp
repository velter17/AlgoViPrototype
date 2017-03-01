/**
 * Project   AlgoVi
 *
 * @file     CSegmentCrossViz.cpp
 * @author   Dmytro Sadovyi
 * @date     20.12.2016
 */

#include <QDebug>

#include "../CSegmentCrossViz.h"
#include "framework/algorithms/CGeometry.h"

namespace NView
{

using namespace NGeometry;

CSegmentCrossViz::CSegmentCrossViz()
{

}

QString CSegmentCrossViz::serialize(const std::map<QString, IAlgoViItem *> &items)
try
{
   QPointF distA = items.at("a#1")->scenePos();
   QPointF distB = items.at("a#2")->scenePos();
   QPointF angleA = items.at("a#3")->scenePos();
   QPointF angleB = items.at("a#4")->scenePos();
   QPointF angleC = items.at("a#5")->scenePos();
    std::vector <QPointF> route;
    for(int i = 1; ; ++i)
    {
       if(items.find("Point#" + QString::number(i)) == items.end())
          break;
       route.push_back(items.at("Point#" + QString::number(i))->scenePos());
    }
    QString ret = QString("%1").arg(len(distA, distB)) + "\n";
    ret += QString("%1").arg(angleBetweenPoints(angleA, angleB, angleC)) + "\n";
    ret += QString::number(route.size()) + "\n";
    for(int i = 0; i < int(route.size()); ++i)
    {
//       ret += QString("%1 %2\n").arg(route[i].x(), route[i].y()) + "\n";
       ret += QString::number(route[i].x()) + " " + QString::number(route[i].y()) + "\n";
    }
    return ret;
}
catch(const std::out_of_range& error)
{
    qDebug () << "CDistToCurve::serialize(): exception: " << error.what();
    return QString();
}

void CSegmentCrossViz::realize(QString &data,
                               QPainter *painter,
                               const std::map <QString, IAlgoViItem*>& items)
try
{
   QPointF distA = items.at("a#1")->scenePos();
   QPointF distB = items.at("a#2")->scenePos();
   QPointF angleA = items.at("a#3")->scenePos();
   QPointF angleB = items.at("a#4")->scenePos();
   QPointF angleC = items.at("a#5")->scenePos();
   QPen p = painter->pen();
   p.setColor(QColor(0, 0, 255));
   painter->setPen(p);
   painter->drawLine(distA, distB);
   painter->drawLine(angleA, angleB);
   painter->drawLine(angleB, angleC);
   p.setColor(QColor(0,255,0));
   painter->setPen(p);
   std::vector <QPointF> route;
   for(int i = 1; ; ++i)
   {
      if(items.find("Point#" + QString::number(i)) == items.end())
         break;
      route.push_back(items.at("Point#" + QString::number(i))->scenePos());
   }
   for(int i = 1; i < (int)route.size(); ++i)
   {
      painter->drawLine(route[i-1], route[i]);
   }

   QTextStream stream(&data);
   int n;
   stream >> n;
   for(int i = 0; i < n; ++i)
   {
      double x[3], y[3];
      p.setColor(Qt::magenta);
      painter->setPen(p);
      for(int j = 0; j < 3; ++j)
      {
         stream >> x[j] >> y[j];
         painter->drawEllipse(QPointF(x[j], y[j]), 3, 3);
      }
      QPointF prev(x[0], y[0]);
      for(int i = 1; i <= 100; ++i)
      {
          double t = (i+.0)/100;
          QPointF r;
          r.setX((1-t)*(1-t) * x[0] + 2*t*(1-t)*x[1] + t*t * x[2]);
          r.setY((1-t)*(1-t) * y[0] + 2*t*(1-t)*y[1] + t*t * y[2]);
          painter->drawLine(prev, r);
          prev = r;
      }
      p.setColor(Qt::red);
      p.setWidth(1);
      painter->setPen(p);
      painter->drawEllipse(QPointF(x[1], y[1]), 6, 6);
   }
}
catch(const std::out_of_range& error)
{
    qDebug () << "CDistToCurve::realize(): exception: " << error.what();
}


} // namespace NView
