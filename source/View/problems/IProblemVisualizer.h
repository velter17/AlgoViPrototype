/**
 * Project   Graviz
 *
 * @file     IProblemVisualizer.h
 * @author   Dmytro Sadovyi
 * @date     17.12.2016
 */

#pragma once

#include <QGraphicsScene>
#include <QObject>
#include <QTextStream>
#include "View/components/IGravizItem.h"

namespace NView
{

class IProblemVisualizer
{
public: // methods
    virtual QString serialize(const std::map <QString, IGravizItem*>& items) = 0;
    virtual void realize(QString& data,
                         QPainter *painter,
                         const std::map <QString, IGravizItem*>& items) = 0;
    virtual ~IProblemVisualizer() = 0;
};

inline IProblemVisualizer::~IProblemVisualizer(){}

} // namespace NView

