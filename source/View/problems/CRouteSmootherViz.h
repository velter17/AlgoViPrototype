/**
 * Project   Graviz
 *
 * @file     CRouteSmootherViz.h
 * @author   Dmytro Sadovyi
 * @date     20.12.2016
 */

#pragma once

#include "View/problems/IProblemVisualizer.h"

namespace NView
{

class CRouteSmootherViz : public IProblemVisualizer
{
public: // methods
    CRouteSmootherViz();

    QString serialize(const std::map<QString, IGravizItem *> &items) override;
    void realize(QString &data,
                 QPainter *painter,
                 const std::map <QString, IGravizItem*>& items) override;
};

} // namespace NView

