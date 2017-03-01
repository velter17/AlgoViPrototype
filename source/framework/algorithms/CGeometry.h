/**
 * Project   AlgoVi
 *
 * @file     CGeometry.h
 * @author   Dmytro Sadovyi
 * @date     20.12.2016
 */

#pragma once

#include <QPointF>
#include <cmath>

namespace NGeometry
{

const double pi = acos(-1.0);
typedef QPointF Point;

inline QPointF toVector(const QPointF &a, const QPointF &b)
{
    return QPointF(b.x() - a.x(), b.y() - a.y());
}

inline double vector_product(const QPointF &a, const QPointF &b)
{
    return a.x() * b.y() - a.y() * b.x();
}

inline double scalar_product(const QPointF &a, const QPointF &b)
{
    return a.x() * b.x() + a.y() * b.y();
}

inline double scalar_product(const QPointF &a)
{
    return a.x() * a.x() + a.y() * a.y();
}

inline double len(const QPointF &a)
{
     return sqrt(scalar_product(a));
}

inline double len(const QPointF& a, const QPointF& b)
{
    return len(toVector(a, b));
}

inline double angle(const QPointF a, const QPointF &b)
{
    return acos(scalar_product(a, b) / abs(len(a)) / abs(len(b)));
}

inline double angle(const QPointF a, const QPointF &b, const QPointF &c)
{
    return angle(toVector(a, b), toVector(b, c));
}

double angleBetweenPoints(const Point& a, const Point& b, const Point& c)
{
   double ab = len(a-b);
   double bc = len(b-c);
   double ac = len(a-c);
   return acos((ab*ab + bc * bc - ac*ac) / (2.0*ab*bc));
}



} // namespace NAlgo

