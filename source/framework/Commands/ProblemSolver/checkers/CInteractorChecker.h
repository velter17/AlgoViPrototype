/**
 * Project   AlgoVi
 *
 * @file     CCustomChecker.h
 * @author   Dmytro Sadovyi
 * @date     22.02.2017
 */

#pragma once

#include "IProblemChecker.h"

namespace NCommand
{

class CInteractorChecker : public IProblemChecker
{
public: // methods
    CInteractorChecker(const QStringList& args, const QString& path);

    TCheckerResult check() override;

private: // fields
    QString mPathToChecker;
};

} // namespace NCommand

