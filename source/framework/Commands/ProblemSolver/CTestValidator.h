/**
 * Project   AlgoVi
 *
 * @file     CTestValidator.h
 * @author   Dmytro Sadovyi
 * @date     21.02.2017
 */

#pragma once

#include "checkers/IProblemChecker.h"


namespace NCommand
{

class CTestValidator : public IProblemChecker
{
public: // methods
    CTestValidator(const QStringList& args, const QString& appPath);

    TCheckerResult check() override;

private: // fields
    QString mPathToValidator;
};

} // namespace NCommand

