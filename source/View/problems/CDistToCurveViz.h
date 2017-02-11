/**
 * Project   AlgoVi
 *
 * @file     CDistToCurveViz.h
 * @author   Dmytro Sadovyi
 * @date     17.12.2016
 */

#pragma once

#include "View/problems/IProblemVisualizer.h"

namespace NView
{

class CDistToCurveViz : public IProblemVisualizer
{
public: // methods
    CDistToCurveViz();

    QString serialize(const std::map<QString, IAlgoViItem *> &items) override;
    void realize(QString &data,
                 QPainter *painter,
                 const std::map <QString, IAlgoViItem*>& items) override;
};

} // namespace NView

