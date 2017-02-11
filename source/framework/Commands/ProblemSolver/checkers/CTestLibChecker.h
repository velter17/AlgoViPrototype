/**
 * Project   AlgoVi
 *
 * @file     CTestLibChecker.h
 * @author   Dmytro Sadovyi
 * @date     09.01.2017
 */

#pragma once

#include "IProblemChecker.h"

namespace NCommand
{

class CTestLibChecker : public IProblemChecker
{
public: // methods
    CTestLibChecker(const QStringList& args, const QString& type);

    TCheckerResult check() override;

private: // fields
    QString mPathToChecker;
};

} // namespace NCommand

