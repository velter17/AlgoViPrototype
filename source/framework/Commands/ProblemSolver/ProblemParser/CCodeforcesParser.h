/**
 * Project   AlgoVi
 *
 * @file     CCodeforcesParser.h
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#pragma once

#include "framework/Commands/ProblemSolver/ProblemParser/IProblemParser.h"

namespace NCommand
{

class CCodeforcesParser : public IProblemParser
{
public: // methods
    CCodeforcesParser(const QStringList& args);

    int parse(const QString& path) override;
};

} // namespace NCommand

