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

class CCustomChecker : public IProblemChecker
{
public: // methods
    CCustomChecker(const QStringList& args, const QString& path);

    TCheckerResult check() override;

private: // fields
    QString mPathToChecker;
};

} // namespace NCommand

