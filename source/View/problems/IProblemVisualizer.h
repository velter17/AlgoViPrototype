/**
 * Project   AlgoVi
 *
 * @file     IProblemVisualizer.h
 * @author   Dmytro Sadovyi
 * @date     17.12.2016
 */

#pragma once

#include <QGraphicsScene>
#include <QObject>
#include <QTextStream>
#include "View/components/IAlgoViItem.h"

namespace NView
{

class IProblemVisualizer
{
public: // methods
    virtual QString serialize(const std::map <QString, IAlgoViItem*>& items) = 0;
    virtual void realize(QString& data,
                         QPainter *painter,
                         const std::map <QString, IAlgoViItem*>& items) = 0;
    virtual ~IProblemVisualizer() = 0;

    virtual bool isFree() {return true;}
};

inline IProblemVisualizer::~IProblemVisualizer(){}

} // namespace NView

