/**
 * Project   Graviz
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

QPointF toVector(const QPointF& a, const QPointF& b);

double vector_product(const QPointF& a, const QPointF& b);
double scalar_product(const QPointF& a, const QPointF& b);
double scalar_product(const QPointF& a);
double len(const QPointF& a);
double len(const QPointF& a, const QPointF& b);
double angle(const QPointF a, const QPointF& b, const QPointF& c);
double angle(const QPointF a, const QPointF& b);

} // namespace NAlgo

