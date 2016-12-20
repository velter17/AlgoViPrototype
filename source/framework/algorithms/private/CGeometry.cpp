/**
 * Project   Graviz
 *
 * @file     CGeometry.cpp
 * @author   Dmytro Sadovyi
 * @date     20.12.2016
 */

#include "../CGeometry.h"

namespace NGeometry
{

QPointF toVector(const QPointF &a, const QPointF &b)
{
    return QPointF(b.x() - a.x(), b.y() - a.y());
}

double vector_product(const QPointF &a, const QPointF &b)
{
    return a.x() * b.y() - a.y() * b.x();
}

double scalar_product(const QPointF &a, const QPointF &b)
{
    return a.x() * b.x() + a.y() * b.y();
}

double scalar_product(const QPointF &a)
{
    return a.x() * a.x() + a.y() * a.y();
}

double len(const QPointF &a)
{
     return sqrt(scalar_product(a));
}

double len(const QPointF& a, const QPointF& b)
{
    return len(toVector(a, b));
}

double angle(const QPointF a, const QPointF &b, const QPointF &c)
{
    return angle(toVector(a, b), toVector(b, c));
}

double angle(const QPointF a, const QPointF &b)
{
    return acos(scalar_product(a, b) / abs(len(a)) / abs(len(b)));
}


} // namespace NGeometry
