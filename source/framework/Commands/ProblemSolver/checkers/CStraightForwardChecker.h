/**
 * Project   AlgoVi
 *
 * @file     CStraightForwardChecker.h
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#pragma once

#include "framework/Commands/ProblemSolver/checkers/IProblemChecker.h"

namespace NCommand
{

class CStraightForwardChecker : public IProblemChecker
{
public: // methods
    CStraightForwardChecker(const QStringList& args);

    TCheckerResult check() override;
};

} // namespace NCommand

