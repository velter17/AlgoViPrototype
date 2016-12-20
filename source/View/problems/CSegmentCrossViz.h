/**
 * Project   Graviz
 *
 * @file     CSegmentCrossViz.h
 * @author   Dmytro Sadovyi
 * @date     20.12.2016
 */

#pragma once

#include "View/problems/IProblemVisualizer.h"

namespace NView
{

class CSegmentCrossViz : public IProblemVisualizer
{
public: // methods
    CSegmentCrossViz();

    QString serialize(const std::map<QString, IGravizItem *> &items) override;
    void realize(QString &data,
                 QPainter *painter,
                 const std::map <QString, IGravizItem*>& items) override;
};

} // namespace NView

