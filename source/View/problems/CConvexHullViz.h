/**
 * Project   Graviz
 *
 * @file     CConvexHullViz.h
 * @author   Dmytro Sadovyi
 * @date     17.12.2016
 */

#pragma once

#include "View/problems/IProblemVisualizer.h"

namespace NView
{

class CConvexHullViz : public IProblemVisualizer
{
public: // methods
    CConvexHullViz();

    QString serialize(const std::map<QString, IGravizItem *> &items) override;
    void realize(QString &data,
                 QPainter *painter,
                 const std::map <QString, IGravizItem*>& items) override;
};

} // namespace NView

